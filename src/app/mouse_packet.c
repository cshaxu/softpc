#include "mouse_packet.h"
#include <limits.h>

#define APP_MOUSE_PACKET_LIMIT 4

static lib_i32 app_mouse_packet_component(lib_i32 value)
{
    if (value > APP_MOUSE_PACKET_LIMIT) return APP_MOUSE_PACKET_LIMIT;
    if (value < -APP_MOUSE_PACKET_LIMIT) return -APP_MOUSE_PACKET_LIMIT;
    return value;
}

void app_mouse_packet_prepare(ui_event *event)
{
    if (event == NULL || event->type != UI_EVENT_MOUSE) return;
    /* UI reports copied surface pixels. The selected InPort's vertical
     * mickey-to-pixel ratio is twice its horizontal ratio (16:8), so one
     * vertical surface pixel requires two mickeys before bounded delivery. */
    if (event->data.mouse.delta_y > INT32_MAX / 2)
        event->data.mouse.delta_y = INT32_MAX;
    else if (event->data.mouse.delta_y < INT32_MIN / 2)
        event->data.mouse.delta_y = INT32_MIN;
    else
        event->data.mouse.delta_y *= 2;
}

void app_mouse_packet_take(ui_event *remainder, ui_event *packet)
{
    if (remainder == NULL || packet == NULL) return;
    *packet = *remainder;
    packet->data.mouse.delta_x = app_mouse_packet_component(
        remainder->data.mouse.delta_x);
    packet->data.mouse.delta_y = app_mouse_packet_component(
        remainder->data.mouse.delta_y);
    remainder->data.mouse.delta_x -= packet->data.mouse.delta_x;
    remainder->data.mouse.delta_y -= packet->data.mouse.delta_y;
}

int app_mouse_packet_pending(const ui_event *remainder)
{
    return remainder != NULL && (remainder->data.mouse.delta_x != 0 ||
        remainder->data.mouse.delta_y != 0);
}
