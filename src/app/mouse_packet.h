#ifndef APP_MOUSE_PACKET_H
#define APP_MOUSE_PACKET_H

#include "lib/ui-base/event_interface.h"

/* The original InPort driver applies acceleration per hardware packet. */
void app_mouse_packet_take(ui_event *remainder, ui_event *packet);
int app_mouse_packet_pending(const ui_event *remainder);

#endif
