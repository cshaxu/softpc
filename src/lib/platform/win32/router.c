#include "router.h"

void win32_presentation_router_init(win32_presentation_router *router,
    win32_presentation_display_policy policy)
{
    if (router == NULL) return;
    router->policy = policy;
    router->target = policy == WIN32_PRESENTATION_DISPLAY_WINDOW ?
        WIN32_PRESENTATION_TARGET_WINDOW : WIN32_PRESENTATION_TARGET_CONSOLE;
    router->stable_text_frames = 0u;
}

win32_presentation_target win32_presentation_router_target(
    const win32_presentation_router *router)
{
    return router == NULL ? WIN32_PRESENTATION_TARGET_WINDOW : router->target;
}

win32_presentation_target win32_presentation_router_observe(
    win32_presentation_router *router,
    const win32_presentation_frame *frame)
{
    if (router == NULL || frame == NULL || frame->valid == 0u)
        return win32_presentation_router_target(router);
    if (router->policy == WIN32_PRESENTATION_DISPLAY_WINDOW) {
        router->target = WIN32_PRESENTATION_TARGET_WINDOW;
        return router->target;
    }
    if (frame->graphics != 0u) {
        router->target = WIN32_PRESENTATION_TARGET_WINDOW;
        router->stable_text_frames = 0u;
    } else if (router->target == WIN32_PRESENTATION_TARGET_WINDOW &&
        ++router->stable_text_frames >= WIN32_PRESENTATION_STABLE_TEXT_FRAMES) {
        router->target = WIN32_PRESENTATION_TARGET_CONSOLE;
        router->stable_text_frames = 0u;
    }
    return router->target;
}
