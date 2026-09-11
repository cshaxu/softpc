#include "mouse_packet.h"
#include <assert.h>

int main(void)
{
    ui_event remainder = { 0 }, packet = { 0 };
    remainder.type = UI_EVENT_MOUSE;
    remainder.data.mouse.delta_x = 8;
    remainder.data.mouse.delta_y = 16;
    app_mouse_packet_prepare(&remainder);
    assert(remainder.data.mouse.delta_x == 8);
    assert(remainder.data.mouse.delta_y == 32);
    app_mouse_packet_take(&remainder, &packet);
    assert(packet.data.mouse.delta_x == 4 && packet.data.mouse.delta_y == 4);
    assert(app_mouse_packet_pending(&remainder));
    app_mouse_packet_take(&remainder, &packet);
    assert(packet.data.mouse.delta_x == 4 && packet.data.mouse.delta_y == 4);
    app_mouse_packet_take(&remainder, &packet);
    assert(packet.data.mouse.delta_x == 0 && packet.data.mouse.delta_y == 4);
    app_mouse_packet_take(&remainder, &packet);
    assert(packet.data.mouse.delta_x == 0 && packet.data.mouse.delta_y == 4);
    app_mouse_packet_take(&remainder, &packet);
    assert(packet.data.mouse.delta_x == 0 && packet.data.mouse.delta_y == 4);
    app_mouse_packet_take(&remainder, &packet);
    assert(packet.data.mouse.delta_x == 0 && packet.data.mouse.delta_y == 4);
    app_mouse_packet_take(&remainder, &packet);
    assert(packet.data.mouse.delta_x == 0 && packet.data.mouse.delta_y == 4);
    app_mouse_packet_take(&remainder, &packet);
    assert(packet.data.mouse.delta_x == 0 && packet.data.mouse.delta_y == 4);
    assert(!app_mouse_packet_pending(&remainder));

    remainder.data.mouse.delta_y = INT32_MAX;
    app_mouse_packet_prepare(&remainder);
    assert(remainder.data.mouse.delta_y == INT32_MAX);
    return 0;
}
