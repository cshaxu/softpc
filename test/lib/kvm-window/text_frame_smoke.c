#include "lib/types/test.h"
#include "lib/kvm-window/frame_interface.h"
int main(void)
{
    kvm_window_frame_storage *frame = lib_allocate_zero(1u, sizeof(*frame));
    lib_test_assert(frame != LIB_NULL);
    (void)kvm_window_frame_begin_text(frame);
    frame->data.text.base.text_columns = 80u;
    frame->data.text.base.text_rows = 25u;
    frame->data.text.base.cells[0].glyph_index = 'X';
    lib_test_assert(kvm_window_frame_validate(&frame->frame) == LIB_STATUS_OK);

    lib_release(frame);
    return 0;
}
