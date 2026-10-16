#ifndef LIB_BASE_TEXT_INTERFACE_H
#define LIB_BASE_TEXT_INTERFACE_H

#include "lib/types/types_interface.h"

/* True for a NUL-terminated UTF-8 scalar sequence; false for NULL, overlong
 * encodings, surrogate values, incomplete sequences or values above U+10FFFF. */
lib_bool base_text_utf8_valid(const char *text);

#endif
