#ifndef LIB_TYPES_WIN32_INPUT_H
#define LIB_TYPES_WIN32_INPUT_H

#include <windows.h>

/* Exact SDK queries. Key/modifier interpretation belongs to ui-base. */
typedef SHORT lib_win32_key_state;
#define lib_win32_get_key_state GetKeyState
#define lib_win32_map_virtual_key MapVirtualKeyA
#define lib_win32_key_scan VkKeyScanA

#endif
