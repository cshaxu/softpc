#ifndef UX_WIN32_PRESENTER_WAKE_H
#define UX_WIN32_PRESENTER_WAKE_H

#include "lib/ux/presenter.h"

#ifdef _WIN32
#include <windows.h>

HANDLE ux_win32_presenter_wait_handle(const ux_presenter *presenter);
HANDLE ux_win32_mailbox_wait_handle(const ux_mailbox_native *native_mailbox);
#endif

#endif
