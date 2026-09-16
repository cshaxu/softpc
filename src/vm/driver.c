#include "vm/driver.h"
#include "input.h"
#include "vm/trace.h"
#include "vm/debug.h"
#include "compat/audio.h"
#include "lib/types/atomic.h"

#include <windows.h>
#include <stdlib.h>
#include <string.h>

struct vm_driver {
    softpc_machine *machine;
    softpc_debug_state debug;
};

/* The recovered core and host endpoints are process-global. This is resource
 * admission, not a second machine lifecycle state. */
static lib_atomic_flag vm_owned = LIB_ATOMIC_FLAG_INITIALIZER;

lib_status vm_create(const vm_options *options, vm_driver **out_driver)
{
    softpc_machine_options machine_options = { 0 };
    softpc_machine *machine = NULL;
    softpc_machine_result result;
    lib_status status;
    if (out_driver == NULL) return LIB_STATUS_INVALID_ARGUMENT;
    *out_driver = NULL;
    if (options == NULL) return LIB_STATUS_INVALID_ARGUMENT;
    machine_options.floppy_path = options->floppy_path;
    machine_options.hard_disk_path = options->hard_disk_path;
    machine_options.serial_output_path = options->serial_output_path;
    machine_options.printer_output_path = options->printer_output_path;
    machine_options.memory_bytes = options->memory_bytes;
    machine_options.media_mode = options->media_mode;
    if (options->media_mode > LIB_STORAGE_MEDIUM_OVERLAY)
        return LIB_STATUS_INVALID_ARGUMENT;
    if (lib_atomic_flag_test_and_set_explicit(&vm_owned, LIB_MEMORY_ORDER_ACQUIRE))
        return LIB_STATUS_INVALID_STATE;
    result = softpc_machine_create(&machine_options, &machine);
    if (result != SOFTPC_MACHINE_OK) {
        status = result == SOFTPC_MACHINE_INVALID_ARGUMENT ?
            LIB_STATUS_INVALID_ARGUMENT : LIB_STATUS_IO_ERROR;
        goto failed;
    }
    status = softpc_platform_audio_start();
    if (status == LIB_STATUS_OK)
        status = vm_driver_create(out_driver, machine);
    if (status == LIB_STATUS_OK) return status;
    softpc_platform_audio_shutdown();
    softpc_machine_destroy(machine);
failed:
    lib_atomic_flag_clear_explicit(&vm_owned, LIB_MEMORY_ORDER_RELEASE);
    return status;
}

void vm_destroy(vm_driver *driver)
{
    if (driver == NULL) return;
    softpc_platform_audio_shutdown();
    softpc_machine_destroy(driver->machine);
    vm_driver_destroy(driver);
    lib_atomic_flag_clear_explicit(&vm_owned, LIB_MEMORY_ORDER_RELEASE);
}

static void vm_driver_trace_frame(void *opaque, const kvm_frame *frame)
{
    vm_driver *driver = (vm_driver *)opaque;
    static lib_u32 prior_mode = UINT32_MAX;
    static lib_u32 prior_screen = UINT32_MAX;
    static lib_u32 prior_graphics = UINT32_MAX;
    static lib_u32 prior_columns = UINT32_MAX;
    static lib_u32 prior_rows = UINT32_MAX;
    static lib_u32 prior_width = UINT32_MAX;
    static lib_u32 prior_height = UINT32_MAX;
    lib_u32 mode = 0u;
    lib_u32 screen = 0u;

    if (driver == NULL || frame == NULL || !vm_trace_enabled()) return;
    (void)softpc_machine_presentation_state(driver->machine, &mode, &screen);
    if (prior_mode == mode && prior_screen == screen &&
        prior_graphics == frame->graphics &&
        prior_columns == frame->text_columns && prior_rows == frame->text_rows &&
        prior_width == frame->graphics_width && prior_height == frame->graphics_height)
        return;
    vm_trace("softpc prompt frame=%lu mode=%lu state=%lu graphics=%lu text=%ux%u dib=%ux%u dirty=%ld,%ld,%ld,%ld",
        (unsigned long)frame->sequence, (unsigned long)mode,
        (unsigned long)screen, (unsigned long)frame->graphics,
        (unsigned)frame->text_columns, (unsigned)frame->text_rows,
        (unsigned)frame->graphics_width, (unsigned)frame->graphics_height,
        (long)frame->dirty_left, (long)frame->dirty_top,
        (long)frame->dirty_right, (long)frame->dirty_bottom);
    prior_mode = mode; prior_screen = screen; prior_graphics = frame->graphics;
    prior_columns = frame->text_columns; prior_rows = frame->text_rows;
    prior_width = frame->graphics_width; prior_height = frame->graphics_height;
}

void vm_driver_cursor_shape(kvm_frame *frame, lib_u32 percent)
{
    lib_u32 height = frame->font_height;
    lib_u32 lines;
    if (height == 0u || height > 16u) height = 16u;
    if (percent == 0u || percent > 100u) percent = 100u;
    lines = (height * percent + 99u) / 100u;
    frame->cursor_top = (lib_u8)(height - lines);
    frame->cursor_bottom = (lib_u8)(height - 1u);
}

static lib_bool vm_driver_reset(void *opaque)
{
    vm_driver *driver = (vm_driver *)opaque;
    if (driver != NULL) driver->debug = (softpc_debug_state) { 0 };
    return driver != NULL && softpc_machine_reset(driver->machine) ==
        SOFTPC_MACHINE_OK;
}

static lib_bool vm_driver_run(void *opaque)
{
    vm_driver *driver = (vm_driver *)opaque;
    lib_bool result;
    if (driver == NULL) return LIB_FALSE;
    softpc_debug_bind(&driver->debug);
    result = softpc_machine_run(driver->machine, UINT64_MAX) == SOFTPC_MACHINE_OK;
    softpc_debug_bind(NULL);
    return result;
}

static void vm_driver_request_stop(void *opaque)
{
    vm_driver *driver = (vm_driver *)opaque;
    if (driver != NULL) softpc_machine_request_stop(driver->machine);
}

static void vm_driver_request_wake(void *opaque)
{
    vm_driver *driver = (vm_driver *)opaque;
    if (driver != NULL) softpc_machine_request_wake(driver->machine);
}

static void vm_driver_set_heartbeat(void *opaque, lib_bool enabled)
{
    vm_driver *driver = (vm_driver *)opaque;
    if (driver != NULL) softpc_machine_set_heartbeat(driver->machine, enabled != 0);
}

static void vm_driver_set_executor_callback(void *opaque,
    common_machine_executor_callback callback, void *callback_context)
{
    vm_driver *driver = (vm_driver *)opaque;
    if (driver != NULL) softpc_machine_set_executor_callback(driver->machine,
        callback, callback_context);
}

static void vm_driver_deliver_input(void *opaque,
    const kvm_input_event *event)
{
    vm_driver *driver = (vm_driver *)opaque;
    if (driver == NULL || event == NULL) return;
    if (event->type == KVM_EVENT_KEY)
        (void)vm_keyboard_inject_machine_event(driver->machine, event);
    else if (event->type == KVM_EVENT_MOUSE)
        (void)softpc_machine_mouse_input(driver->machine,
            event->data.mouse.delta_x, event->data.mouse.delta_y,
            (event->data.mouse.buttons & KVM_MOUSE_BUTTON_LEFT) != 0u,
            (event->data.mouse.buttons & KVM_MOUSE_BUTTON_RIGHT) != 0u);
}

static lib_bool vm_driver_copy_graphics(vm_driver *driver,
    kvm_frame *frame)
{
    const void *bits;
    const void *info;
    lib_u32 width;
    lib_u32 height;
    lib_u32 row_stride;
    lib_i32 left;
    lib_i32 top;
    lib_i32 right;
    lib_i32 bottom;
    const BITMAPINFO *dib;
    lib_u32 row;
    lib_u32 palette_index;

    if (!softpc_machine_presentation_take_dirty(driver->machine, &left, &top,
            &right, &bottom)) return LIB_FALSE;
    if (!softpc_machine_presentation_dib(driver->machine, &bits, &info, &width,
            &height) || bits == NULL || info == NULL ||
        width > KVM_GRAPHICS_MAX_WIDTH || height > KVM_GRAPHICS_MAX_HEIGHT)
        return LIB_FALSE;
    row_stride = (width + 3u) & ~3u;
    if (width * height > KVM_GRAPHICS_MAX_PIXELS) return LIB_FALSE;
    memset(frame, 0, sizeof(*frame));
    for (row = 0u; row < height; ++row)
        memcpy(frame->graphics_pixels + row * width,
            (const lib_u8 *)bits + row * row_stride, width);
    dib = (const BITMAPINFO *)info;
    for (palette_index = 0u; palette_index < KVM_GRAPHICS_PALETTE_ENTRIES;
            ++palette_index) {
        const RGBQUAD *colour = &dib->bmiColors[palette_index];
        frame->graphics_palette[palette_index] =
            ((lib_u32)colour->rgbRed << 16u) |
            ((lib_u32)colour->rgbGreen << 8u) | (lib_u32)colour->rgbBlue;
    }
    frame->graphics_width = width;
    frame->graphics_height = height;
    frame->graphics_stride = width;
    frame->dirty_left = left < 0 ? 0 : left;
    frame->dirty_top = top < 0 ? 0 : top;
    frame->dirty_right = right >= (lib_i32)width ? (lib_i32)width - 1 : right;
    frame->dirty_bottom = bottom >= (lib_i32)height ? (lib_i32)height - 1 : bottom;
    frame->graphics = 1u;
    frame->valid = 1u;
    return LIB_TRUE;
}

static lib_bool vm_driver_copy_text(vm_driver *driver,
    kvm_frame *frame)
{
    const void *surface;
    lib_u32 columns;
    lib_u32 rows;
    lib_u32 stride;
    lib_u32 cell_bytes;
    const lib_u8 *cells;
    lib_i32 column = -1;
    lib_i32 row = -1;
    lib_u32 cursor_size = 0u;
    lib_u32 text_row;

    if (!softpc_machine_presentation_text(driver->machine, &surface, &columns,
            &rows, &stride, &cell_bytes) || surface == NULL || cell_bytes == 0u ||
        stride < columns) return LIB_FALSE;
    memset(frame, 0, sizeof(*frame));
    memset(frame->text, ' ', sizeof(frame->text));
    memset(frame->attributes, 0x07, sizeof(frame->attributes));
    if (columns > KVM_TEXT_COLUMNS) columns = KVM_TEXT_COLUMNS;
    if (rows > KVM_TEXT_ROWS) rows = KVM_TEXT_ROWS;
    cells = (const lib_u8 *)surface;
    for (text_row = 0u; text_row < rows; ++text_row) {
        lib_u32 text_column;
        for (text_column = 0u; text_column < columns; ++text_column) {
            size_t source = ((size_t)text_row * stride + text_column) * cell_bytes;
            size_t destination = (size_t)text_row * KVM_TEXT_COLUMNS + text_column;
            frame->text[destination] = cells[source];
            if (cell_bytes >= 2u) frame->attributes[destination] = cells[source + 1u];
        }
    }
    {
        const void *bits;
        const void *info;
        lib_u32 width;
        lib_u32 height;
        if (softpc_machine_presentation_dib(driver->machine, &bits, &info, &width,
                &height) && info != NULL) {
            const BITMAPINFO *dib = (const BITMAPINFO *)info;
            lib_u32 index;
            for (index = 0u; index < 16u; ++index)
                frame->text_palette[index] = ((lib_u32)dib->bmiColors[index].rgbRed << 16u) |
                    ((lib_u32)dib->bmiColors[index].rgbGreen << 8u) |
                    (lib_u32)dib->bmiColors[index].rgbBlue;
        }
    }
    (void)softpc_machine_presentation_cursor(driver->machine, &column, &row,
        &cursor_size);
    frame->cursor_column = column;
    frame->cursor_row = row;
    (void)softpc_machine_presentation_fonts(driver->machine, frame->font,
        frame->secondary_font, &frame->font_height,
        &frame->attribute_font_select);
    frame->text_columns = (lib_u16)columns;
    frame->text_rows = (lib_u16)rows;
    vm_driver_cursor_shape(frame, cursor_size);
    frame->cursor_visible = column >= 0 && row >= 0;
    frame->cursor_phase = 1u;
    frame->dirty_left = 0;
    frame->dirty_top = 0;
    frame->dirty_right = -1;
    frame->dirty_bottom = -1;
    frame->valid = 1u;
    return LIB_TRUE;
}

static lib_bool vm_driver_copy_frame(void *opaque, kvm_frame *frame)
{
    vm_driver *driver = (vm_driver *)opaque;
    if (driver == NULL || frame == NULL) return LIB_FALSE;
    return softpc_machine_presentation_is_graphics(driver->machine) ?
        vm_driver_copy_graphics(driver, frame) :
        vm_driver_copy_text(driver, frame);
}

static lib_bool vm_driver_set_removable_media(void *opaque,
    const char *path)
{
    vm_driver *driver = (vm_driver *)opaque;
    return driver != NULL && softpc_machine_set_floppy(driver->machine, path) ==
        SOFTPC_MACHINE_OK;
}

static lib_status vm_driver_debug(void *opaque,
    const common_machine_debug_request *request, common_machine_debug_result *result)
{
    vm_driver *driver = opaque;
    return softpc_machine_debug(driver->machine, &driver->debug, request, result);
}

static lib_bool vm_driver_take_debug_stop(void *opaque)
{
    vm_driver *driver = opaque;
    lib_bool pending = driver->debug.stop_pending && driver->debug.result_ready;
    if (pending) driver->debug.stop_pending = LIB_FALSE;
    return pending;
}

static void vm_driver_cancel_debug(void *opaque)
{
    vm_driver *driver = opaque;
    driver->debug = (softpc_debug_state) { 0 };
}

lib_status vm_driver_create(vm_driver **out_driver,
    softpc_machine *machine)
{
    vm_driver *driver;
    if (out_driver == NULL || machine == NULL) return LIB_STATUS_INVALID_ARGUMENT;
    *out_driver = NULL;
    driver = calloc(1u, sizeof(*driver));
    if (driver == NULL) return LIB_STATUS_NO_MEMORY;
    driver->machine = machine;
    *out_driver = driver;
    return LIB_STATUS_OK;
}

void vm_driver_destroy(vm_driver *driver)
{
    free(driver);
}

void vm_driver_describe(vm_driver *driver,
    common_machine_driver *out_driver)
{
    if (out_driver == NULL) return;
    *out_driver = (common_machine_driver) { 0 };
    out_driver->context = driver;
    out_driver->reset = vm_driver_reset;
    out_driver->run = vm_driver_run;
    out_driver->request_stop = vm_driver_request_stop;
    out_driver->request_wake = vm_driver_request_wake;
    out_driver->set_heartbeat = vm_driver_set_heartbeat;
    out_driver->set_executor_callback = vm_driver_set_executor_callback;
    out_driver->deliver_input = vm_driver_deliver_input;
    out_driver->copy_frame = vm_driver_copy_frame;
    out_driver->set_removable_media = vm_driver_set_removable_media;
    out_driver->execute_debug = vm_driver_debug;
    out_driver->take_debug_stop = vm_driver_take_debug_stop;
    out_driver->cancel_debug = vm_driver_cancel_debug;
    out_driver->frame_published = vm_driver_trace_frame;
}
