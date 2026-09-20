#ifndef KVM_CONSOLE_FRAME_INTERFACE_H
#define KVM_CONSOLE_FRAME_INTERFACE_H

#include "lib/kvm-base/frame_interface.h"

/* One BMP character per glyph index. Both banks are copied frame data, not
 * resources retained from a caller. Attribute selection has the base meaning. */
typedef struct kvm_console_character_map {
    lib_u16 primary[256u];
    lib_u16 secondary[256u];
} kvm_console_character_map;

typedef struct kvm_console_text_frame {
    kvm_text_frame base;
    kvm_console_character_map characters;
} kvm_console_text_frame;

#endif
