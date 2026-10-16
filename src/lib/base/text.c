#include "lib/base/text_interface.h"

/* NUL-terminated Unicode scalar encoding; no overlong forms or surrogates. */
lib_bool base_text_utf8_valid(const char *text)
{
    const lib_u8 *next = (const lib_u8 *)text;
    if (next == LIB_NULL) return LIB_FALSE;
    while (*next != 0u) {
        lib_u32 value = *next++;
        lib_u32 count, minimum;
        if (value < 0x80u) continue;
        if (value >= 0xc2u && value <= 0xdfu) {
            value &= 0x1fu; count = 1u; minimum = 0x80u;
        } else if (value >= 0xe0u && value <= 0xefu) {
            value &= 0x0fu; count = 2u; minimum = 0x800u;
        } else if (value >= 0xf0u && value <= 0xf4u) {
            value &= 0x07u; count = 3u; minimum = 0x10000u;
        } else return LIB_FALSE;
        while (count-- != 0u) {
            if ((*next & 0xc0u) != 0x80u) return LIB_FALSE;
            value = (value << 6) | (*next++ & 0x3fu);
        }
        if (value < minimum || value > 0x10ffffu ||
            (value >= 0xd800u && value <= 0xdfffu)) return LIB_FALSE;
    }
    return LIB_TRUE;
}
