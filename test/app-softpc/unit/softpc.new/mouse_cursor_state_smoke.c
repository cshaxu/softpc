#include <assert.h>
#include "machine/machine.h"
#include "../../fixture_cleanup.h"
#include "softpc.new/base/keymouse/mouse_io.c"

static unsigned marks;
static int change_mode;
static boolean mark_cursor(int first, int last)
{
    assert(last == first + 2);
    ++marks;
    if (change_mode) jap_mouse = change_mode > 0;
    return TRUE;
}

/* Outside, excluded, scaled, unscaled, callback disables/enables scaling. */
static int check_case(int kind)
{
    static MM_INSTANCE_DATA instance;
    static MM_INSTANCE_DATA *instance_ptr = &instance;
    int i, failures = 0, draw = kind >= 2, scaled = kind != 3 && kind != 5;
    MOUSE_SCALAR original_y = draw ? 8 : 200;
    MOUSE_SCALAR expected_screen_y = scaled ? original_y * 19 / 8 : original_y;
    IU32 offset;
    lib_memory_set(&instance, 0, sizeof(instance));
    mm_handle = &instance_ptr;
    jap_mouse = scaled;
    marks = 0;
    change_mode = kind == 4 ? -1 : (kind == 5 ? 1 : 0);
    virtual_screen.bottom_right.x = 640;
    virtual_screen.bottom_right.y = 200;
    cursor_status.position.x = kind == 0 ? 1000 : 8;
    cursor_status.position.y = original_y;
    graphics_cursor.size.x = graphics_cursor.size.y = 16;
    black_hole.top_left.x = black_hole.top_left.y = -100;
    black_hole.bottom_right = black_hole.top_left;
    if (kind == 1) {
        black_hole.top_left.x = black_hole.top_left.y = 0;
        black_hole.bottom_right.x = black_hole.bottom_right.y = 2000;
    }
    EGA_GRAPH.actual_offset_per_line = 80;
    for (i = 0; i < 16; ++i)
        graphics_cursor.cursor_lo[i] = graphics_cursor.cursor_hi[i] = 0xffffffffu;
    offset = 80 * expected_screen_y + 1;
    if (draw) ((IU32 *)EGA_planes)[offset] = 0;
    for (i = 0; i < (kind == 0 ? 7 : 1); ++i) {
        EGA_graphics_cursor_display();
        if (cursor_status.position.y != original_y || virtual_screen.bottom_right.y != 200)
            ++failures;
        printf("case %d call %d: y=%d height=%d\n", kind, i + 1,
            cursor_status.position.y, virtual_screen.bottom_right.y);
    }
    assert(marks == (draw ? 16u : 0u));
    assert(save_area_in_use == draw);
    if (draw) {
        assert(save_position.y == expected_screen_y);
        assert(((IU32 *)EGA_planes)[offset] == 0xffffffffu);
    }
    return failures;
}

int main(void)
{
    softpc_machine_options options = {"softpc-mouse-cursor-state.img", NULL};
    unsigned char boot[512] = {0};
    FILE *file = fopen(options.floppy_path, "wb");
    softpc_machine *machine = NULL;
    T_mark_string original_mark;
    int kind, failures = 0;
    assert(file != NULL);
    assert(fwrite(boot, 1, sizeof(boot), file) == sizeof(boot));
    assert(fclose(file) == 0);
    assert(softpc_machine_create(&options, &machine) == SOFTPC_MACHINE_OK);
    assert(softpc_machine_reset(machine) == SOFTPC_MACHINE_OK);
    assert(EGA_planes != NULL);
    sas_store(vd_video_mode, 0x12);
    original_mark = update_alg.mark_string;
    update_alg.mark_string = mark_cursor;
    for (kind = 0; kind < 6; ++kind) failures += check_case(kind);
    update_alg.mark_string = original_mark;
    mm_handle = NULL;
    assert(softpc_machine_destroy(machine) == SOFTPC_MACHINE_OK);
    assert(softpc_test_remove_image(options.floppy_path));
    printf("SoftPC original mouse coordinate restoration: %s\n", failures ? "FAILED" : "OK");
    return failures ? 1 : 0;
}
