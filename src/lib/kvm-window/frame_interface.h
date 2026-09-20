#ifndef KVM_WINDOW_FRAME_INTERFACE_H
#define KVM_WINDOW_FRAME_INTERFACE_H

#include "lib/kvm-base/frame_interface.h"

#define KVM_WINDOW_GRAPHICS_MAX_WIDTH 1280u
#define KVM_WINDOW_GRAPHICS_MAX_HEIGHT 768u
#define KVM_WINDOW_GRAPHICS_MAX_PIXELS (KVM_WINDOW_GRAPHICS_MAX_WIDTH * KVM_WINDOW_GRAPHICS_MAX_HEIGHT)
#define KVM_WINDOW_GRAPHICS_PALETTE_ENTRIES 256u
#define KVM_WINDOW_FONT_HEIGHT 16u
#define KVM_WINDOW_FONT_GLYPHS 256u

typedef struct kvm_window_text_frame {
    kvm_text_frame base;
    lib_u8 font[KVM_WINDOW_FONT_GLYPHS * KVM_WINDOW_FONT_HEIGHT];
    lib_u8 secondary_font[KVM_WINDOW_FONT_GLYPHS * KVM_WINDOW_FONT_HEIGHT];
} kvm_window_text_frame;

typedef struct kvm_window_graphics_frame {
    lib_u32 width;
    lib_u32 height;
    lib_u32 stride;
    lib_i32 dirty_left;
    lib_i32 dirty_top;
    lib_i32 dirty_right;
    lib_i32 dirty_bottom;
    lib_u32 palette[KVM_WINDOW_GRAPHICS_PALETTE_ENTRIES]; /* 0x00RRGGBB */
    lib_u8 pixels[KVM_WINDOW_GRAPHICS_MAX_PIXELS];
} kvm_window_graphics_frame;

typedef struct kvm_window_frame {
    lib_u32 valid;
    lib_u32 graphics;
    union {
        kvm_window_text_frame text;
        kvm_window_graphics_frame image;
    };
} kvm_window_frame;

static inline lib_bool kvm_window_frame_is_valid(const kvm_window_frame *frame)
{
    if (frame == LIB_NULL || frame->valid == 0u) return LIB_FALSE;
    if (frame->graphics != 0u) {
        return frame->image.width != 0u &&
            frame->image.width <= KVM_WINDOW_GRAPHICS_MAX_WIDTH &&
            frame->image.height != 0u &&
            frame->image.height <= KVM_WINDOW_GRAPHICS_MAX_HEIGHT &&
            frame->image.stride >= frame->image.width &&
            frame->image.stride <= KVM_WINDOW_GRAPHICS_MAX_WIDTH;
    }
    return kvm_text_frame_is_valid(&frame->text.base);
}

/* Call only after validation. No inactive union arm or pixel tail is copied. */
static inline lib_size kvm_window_frame_size_bytes(const kvm_window_frame *frame)
{
    return frame->graphics != 0u ? lib_offsetof(kvm_window_frame, image.pixels) +
        (lib_size)frame->image.stride * frame->image.height :
        lib_offsetof(kvm_window_frame, text) + sizeof(frame->text);
}

static inline lib_bool kvm_window_frame_copy(kvm_window_frame *destination,
    const kvm_window_frame *source)
{
    if (destination == LIB_NULL || !kvm_window_frame_is_valid(source)) return LIB_FALSE;
    if (destination != source)
        lib_memory_copy(destination, source, kvm_window_frame_size_bytes(source));
    return LIB_TRUE;
}

#endif
