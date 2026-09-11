#include "mouse_packet.h"

#define APP_MOUSE_PACKET_LIMIT 4

static lib_i32 app_mouse_packet_component(lib_i32 value)
{
    if (value > APP_MOUSE_PACKET_LIMIT) return APP_MOUSE_PACKET_LIMIT;
    if (value < -APP_MOUSE_PACKET_LIMIT) return -APP_MOUSE_PACKET_LIMIT;
    return value;
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
