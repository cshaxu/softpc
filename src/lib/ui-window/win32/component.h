#ifndef UI_WIN32_WINDOW_COMPONENT_H
#define UI_WIN32_WINDOW_COMPONENT_H

#include "lib/ui-window/window.h"

lib_status ui_window_worker_start(ui_window *window);
void ui_window_worker_join(ui_window *window);

#endif
