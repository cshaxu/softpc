#include "mailbox.h"

#ifdef _WIN32
#include <windows.h>
#include <stdlib.h>
#include <string.h>

struct win32_presentation_mailbox {
    CRITICAL_SECTION lock;
    win32_presentation_frame frames[2];
    unsigned int published;
    volatile LONG sequence;
    HANDLE event;
};

int win32_presentation_mailbox_create(win32_presentation_mailbox **out)
{
    win32_presentation_mailbox *mailbox;

    if (out == NULL) return 0;
    *out = NULL;
    mailbox = (win32_presentation_mailbox *)calloc(1u, sizeof(*mailbox));
    if (mailbox == NULL) return 0;
    InitializeCriticalSection(&mailbox->lock);
    mailbox->event = CreateEventA(NULL, FALSE, FALSE, NULL);
    if (mailbox->event == NULL) {
        DeleteCriticalSection(&mailbox->lock);
        free(mailbox);
        return 0;
    }
    *out = mailbox;
    return 1;
}

void win32_presentation_mailbox_destroy(win32_presentation_mailbox *mailbox)
{
    if (mailbox == NULL) return;
    CloseHandle(mailbox->event);
    DeleteCriticalSection(&mailbox->lock);
    free(mailbox);
}

int win32_presentation_mailbox_begin_update(
    win32_presentation_mailbox *mailbox,
    win32_presentation_frame **destination,
    const win32_presentation_frame **current)
{
    unsigned int next;

    if (mailbox == NULL || destination == NULL || current == NULL) return 0;
    EnterCriticalSection(&mailbox->lock);
    next = mailbox->published == 0u ? 1u : 0u;
    *destination = &mailbox->frames[next];
    *current = &mailbox->frames[mailbox->published];
    return 1;
}

uint32_t win32_presentation_mailbox_finish_update(
    win32_presentation_mailbox *mailbox, int publish)
{
    uint32_t sequence = 0u;

    if (mailbox == NULL) return 0u;
    if (publish) {
        mailbox->published = mailbox->published == 0u ? 1u : 0u;
        sequence = (uint32_t)InterlockedIncrement(&mailbox->sequence);
        mailbox->frames[mailbox->published].sequence = sequence;
    }
    LeaveCriticalSection(&mailbox->lock);
    if (publish) SetEvent(mailbox->event);
    return sequence;
}

int win32_presentation_mailbox_copy(win32_presentation_mailbox *mailbox,
    win32_presentation_frame *destination)
{
    if (mailbox == NULL || destination == NULL ||
        !TryEnterCriticalSection(&mailbox->lock)) return 0;
    memcpy(destination, &mailbox->frames[mailbox->published],
        sizeof(*destination));
    LeaveCriticalSection(&mailbox->lock);
    return destination->valid != 0u;
}

uint32_t win32_presentation_mailbox_sequence(
    const win32_presentation_mailbox *mailbox)
{
    return mailbox == NULL ? 0u : (uint32_t)InterlockedCompareExchange(
        (volatile LONG *)&mailbox->sequence, 0, 0);
}

void *win32_presentation_mailbox_event(
    const win32_presentation_mailbox *mailbox)
{
    return mailbox == NULL ? NULL : (void *)mailbox->event;
}

void win32_presentation_mailbox_reset_event(
    win32_presentation_mailbox *mailbox)
{
    if (mailbox != NULL) ResetEvent(mailbox->event);
}
#endif
