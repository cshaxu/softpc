#ifndef WIN32_PRESENTATION_ROUTER_H
#define WIN32_PRESENTATION_ROUTER_H

#include "frame.h"

#include <stdint.h>

typedef enum win32_presentation_display_policy {
    WIN32_PRESENTATION_DISPLAY_WINDOW,
    WIN32_PRESENTATION_DISPLAY_CONSOLE
} win32_presentation_display_policy;

typedef enum win32_presentation_target {
    WIN32_PRESENTATION_TARGET_WINDOW,
    WIN32_PRESENTATION_TARGET_CONSOLE
} win32_presentation_target;

typedef struct win32_presentation_router {
    win32_presentation_display_policy policy;
    win32_presentation_target target;
    uint32_t stable_text_frames;
} win32_presentation_router;

#define WIN32_PRESENTATION_STABLE_TEXT_FRAMES 3u

void win32_presentation_router_init(win32_presentation_router *router,
    win32_presentation_display_policy policy);
win32_presentation_target win32_presentation_router_target(
    const win32_presentation_router *router);
/* Observe one copied frame.  `WINDOW` never routes to console. `CONSOLE`
 * switches immediately to graphics and only returns after stable text. */
win32_presentation_target win32_presentation_router_observe(
    win32_presentation_router *router,
    const win32_presentation_frame *frame);

#endif
