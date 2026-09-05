#ifndef WIN32_PRESENTATION_EVENT_QUEUE_H
#define WIN32_PRESENTATION_EVENT_QUEUE_H

#include "event.h"

#include <stdint.h>

#ifdef _WIN32
typedef struct win32_presentation_event_queue win32_presentation_event_queue;

int win32_presentation_event_queue_create(
    win32_presentation_event_queue **out);
void win32_presentation_event_queue_destroy(
    win32_presentation_event_queue *queue);
int win32_presentation_event_queue_push(win32_presentation_event_queue *queue,
    const win32_presentation_event *event);
int win32_presentation_event_queue_pop(win32_presentation_event_queue *queue,
    win32_presentation_event *event);
int win32_presentation_event_queue_pending(
    win32_presentation_event_queue *queue);
void *win32_presentation_event_queue_event(
    const win32_presentation_event_queue *queue);
#endif

#endif
