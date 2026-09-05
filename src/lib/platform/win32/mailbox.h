#ifndef WIN32_PRESENTATION_MAILBOX_H
#define WIN32_PRESENTATION_MAILBOX_H

#include "frame.h"

#include <stdint.h>

#ifdef _WIN32
/* A latest-value, copied-frame handoff.  The producer may fill the returned
 * destination while holding the update lock; consumers never observe it until
 * finish_update publishes the selected slot and signals the auto-reset event. */
typedef struct win32_presentation_mailbox win32_presentation_mailbox;

int win32_presentation_mailbox_create(win32_presentation_mailbox **out);
void win32_presentation_mailbox_destroy(win32_presentation_mailbox *mailbox);
int win32_presentation_mailbox_begin_update(
    win32_presentation_mailbox *mailbox,
    win32_presentation_frame **destination,
    const win32_presentation_frame **current);
uint32_t win32_presentation_mailbox_finish_update(
    win32_presentation_mailbox *mailbox, int publish);
int win32_presentation_mailbox_copy(win32_presentation_mailbox *mailbox,
    win32_presentation_frame *destination);
uint32_t win32_presentation_mailbox_sequence(
    const win32_presentation_mailbox *mailbox);
void *win32_presentation_mailbox_event(
    const win32_presentation_mailbox *mailbox);
void win32_presentation_mailbox_reset_event(
    win32_presentation_mailbox *mailbox);
#endif

#endif
