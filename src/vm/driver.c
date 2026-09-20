#include "vm/driver.h"
#include "input.h"
#include "vm/trace.h"
#include "vm/debug.h"
#include "vm/snapshot.h"
#include "compat/ccpu/lifecycle.h"
#include "compat/audio.h"
#include "lib/types/atomic.h"

#include <windows.h>
#include <stdlib.h>
#include <string.h>

/* Traditional PC glyph approximation belongs to this machine adapter, not Lib. */
static const lib_u16 vm_driver_pc_glyphs[256] = {
    0x0020u, 0x263au, 0x263bu, 0x2665u, 0x2666u, 0x2663u, 0x2660u, 0x2022u,
    0x25d8u, 0x25cbu, 0x25d9u, 0x2642u, 0x2640u, 0x266au, 0x266bu, 0x263cu,
    0x25bau, 0x25c4u, 0x2195u, 0x203cu, 0x00b6u, 0x00a7u, 0x25acu, 0x21a8u,
    0x2191u, 0x2193u, 0x2192u, 0x2190u, 0x221fu, 0x2194u, 0x25b2u, 0x25bcu,
    0x0020u, 0x0021u, 0x0022u, 0x0023u, 0x0024u, 0x0025u, 0x0026u, 0x0027u,
    0x0028u, 0x0029u, 0x002au, 0x002bu, 0x002cu, 0x002du, 0x002eu, 0x002fu,
    0x0030u, 0x0031u, 0x0032u, 0x0033u, 0x0034u, 0x0035u, 0x0036u, 0x0037u,
    0x0038u, 0x0039u, 0x003au, 0x003bu, 0x003cu, 0x003du, 0x003eu, 0x003fu,
    0x0040u, 0x0041u, 0x0042u, 0x0043u, 0x0044u, 0x0045u, 0x0046u, 0x0047u,
    0x0048u, 0x0049u, 0x004au, 0x004bu, 0x004cu, 0x004du, 0x004eu, 0x004fu,
    0x0050u, 0x0051u, 0x0052u, 0x0053u, 0x0054u, 0x0055u, 0x0056u, 0x0057u,
    0x0058u, 0x0059u, 0x005au, 0x005bu, 0x005cu, 0x005du, 0x005eu, 0x005fu,
    0x0060u, 0x0061u, 0x0062u, 0x0063u, 0x0064u, 0x0065u, 0x0066u, 0x0067u,
    0x0068u, 0x0069u, 0x006au, 0x006bu, 0x006cu, 0x006du, 0x006eu, 0x006fu,
    0x0070u, 0x0071u, 0x0072u, 0x0073u, 0x0074u, 0x0075u, 0x0076u, 0x0077u,
    0x0078u, 0x0079u, 0x007au, 0x007bu, 0x007cu, 0x007du, 0x007eu, 0x2302u,
    0x00c7u, 0x00fcu, 0x00e9u, 0x00e2u, 0x00e4u, 0x00e0u, 0x00e5u, 0x00e7u,
    0x00eau, 0x00ebu, 0x00e8u, 0x00efu, 0x00eeu, 0x00ecu, 0x00c4u, 0x00c5u,
    0x00c9u, 0x00e6u, 0x00c6u, 0x00f4u, 0x00f6u, 0x00f2u, 0x00fbu, 0x00f9u,
    0x00ffu, 0x00d6u, 0x00dcu, 0x00a2u, 0x00a3u, 0x00a5u, 0x20a7u, 0x0192u,
    0x00e1u, 0x00edu, 0x00f3u, 0x00fau, 0x00f1u, 0x00d1u, 0x00aau, 0x00bau,
    0x00bfu, 0x2310u, 0x00acu, 0x00bdu, 0x00bcu, 0x00a1u, 0x00abu, 0x00bbu,
    0x2591u, 0x2592u, 0x2593u, 0x2502u, 0x2524u, 0x2561u, 0x2562u, 0x2556u,
    0x2555u, 0x2563u, 0x2551u, 0x2557u, 0x255du, 0x255cu, 0x255bu, 0x2510u,
    0x2514u, 0x2534u, 0x252cu, 0x251cu, 0x2500u, 0x253cu, 0x255eu, 0x255fu,
    0x255au, 0x2554u, 0x2569u, 0x2566u, 0x2560u, 0x2550u, 0x256cu, 0x2567u,
    0x2568u, 0x2564u, 0x2565u, 0x2559u, 0x2558u, 0x2552u, 0x2553u, 0x256bu,
    0x256au, 0x2518u, 0x250cu, 0x2588u, 0x2584u, 0x258cu, 0x2590u, 0x2580u,
    0x03b1u, 0x00dfu, 0x0393u, 0x03c0u, 0x03a3u, 0x03c3u, 0x00b5u, 0x03c4u,
    0x03a6u, 0x0398u, 0x03a9u, 0x03b4u, 0x221eu, 0x03c6u, 0x03b5u, 0x2229u,
    0x2261u, 0x00b1u, 0x2265u, 0x2264u, 0x2320u, 0x2321u, 0x00f7u, 0x2248u,
    0x00b0u, 0x2219u, 0x00b7u, 0x221au, 0x207fu, 0x00b2u, 0x25a0u, 0x00a0u,
};

struct vm_driver {
    softpc_machine *machine;
    softpc_debug_state debug;
    softpc_snapshot capture;
    softpc_snapshot_image captured_image;
    common_machine_state_writer state_writer;
    lib_status state_read_status;
    lib_bool state_read_ready;
    softpc_snapshot_image staged_image;
    lib_bool restore_pending;
    lib_bool restore_active;
    lib_bool restore_rendezvous_pending;
    common_machine_executor_callback executor_callback;
    void *executor_context;
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
    machine_options.floppy_mode = options->floppy_mode;
    machine_options.hard_disk_mode = options->hard_disk_mode;
    if (options->floppy_mode > LIB_STORAGE_MEDIUM_OVERLAY ||
        options->hard_disk_mode > LIB_STORAGE_MEDIUM_OVERLAY)
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

static void vm_driver_trace_frame(void *opaque, const common_machine_frame *frame)
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
    lib_u32 columns, rows, width, height;
    const kvm_window_frame *window;

    if (driver == NULL || frame == NULL || !vm_trace_enabled()) return;
    window = &frame->window;
    columns = window->graphics ? 0u : window->text.base.text_columns;
    rows = window->graphics ? 0u : window->text.base.text_rows;
    width = window->graphics ? window->image.width : 0u;
    height = window->graphics ? window->image.height : 0u;
    (void)softpc_machine_presentation_state(driver->machine, &mode, &screen);
    if (prior_mode == mode && prior_screen == screen &&
        prior_graphics == window->graphics &&
        prior_columns == columns && prior_rows == rows &&
        prior_width == width && prior_height == height)
        return;
    vm_trace("softpc prompt frame=%lu mode=%lu state=%lu graphics=%lu text=%ux%u dib=%ux%u dirty=%ld,%ld,%ld,%ld",
        (unsigned long)frame->sequence, (unsigned long)mode,
        (unsigned long)screen, (unsigned long)window->graphics,
        (unsigned)columns, (unsigned)rows,
        (unsigned)width, (unsigned)height,
        (long)(window->graphics ? window->image.dirty_left : 0), (long)(window->graphics ? window->image.dirty_top : 0),
        (long)(window->graphics ? window->image.dirty_right : -1), (long)(window->graphics ? window->image.dirty_bottom : -1));
    prior_mode = mode; prior_screen = screen; prior_graphics = window->graphics;
    prior_columns = columns; prior_rows = rows;
    prior_width = width; prior_height = height;
}

void vm_driver_cursor_shape(kvm_text_frame *frame, lib_u32 percent)
{
    lib_u32 height = frame->font_height;
    lib_u32 lines;
    if (height == 0u) height = KVM_WINDOW_FONT_HEIGHT;
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

static void vm_driver_executor_event(void *opaque)
{
    vm_driver *driver = (vm_driver *)opaque;
    if (driver != NULL && !driver->restore_active &&
        driver->executor_callback != NULL)
        driver->executor_callback(driver->executor_context);
}

static void vm_driver_snapshot_observe(void *opaque, unsigned long depth,
    const softpc_ccpu_entry *entry)
{
    vm_driver *driver = (vm_driver *)opaque;
    lib_status status;
    lib_status finish_status;
    if (driver == NULL || entry == NULL) return;
    if (driver->restore_rendezvous_pending) {
        driver->restore_rendezvous_pending = LIB_FALSE;
        vm_driver_executor_event(driver);
        return;
    }
    if (!softpc_snapshot_checkpoint(&driver->capture, depth, entry)) return;
    status = driver->capture.status;
    if (status == LIB_STATUS_OK) {
        softpc_snapshot_image_dispose(&driver->captured_image);
        status = softpc_snapshot_image_capture(&driver->captured_image, entry);
        if (status == LIB_STATUS_OK)
            status = softpc_snapshot_image_write(&driver->captured_image,
                (softpc_snapshot_bytes_write)driver->state_writer.write,
                driver->state_writer.context);
    }
    finish_status = softpc_snapshot_finish(&driver->capture);
    if (finish_status != LIB_STATUS_OK)
        status = finish_status;
    driver->state_read_status = status;
    driver->state_read_ready = LIB_TRUE;
    vm_driver_executor_event(driver);
    softpc_snapshot_image_dispose(&driver->captured_image);
    if (finish_status != LIB_STATUS_OK)
        softpc_ccpu_lifecycle_request_exit();
    driver->state_read_ready = LIB_FALSE;
}

static lib_bool vm_driver_run(void *opaque)
{
    vm_driver *driver = (vm_driver *)opaque;
    lib_bool result;
    if (driver == NULL) return LIB_FALSE;
    softpc_debug_bind(&driver->debug);
    softpc_ccpu_lifecycle_observe(vm_driver_snapshot_observe, driver);
    if (driver->restore_pending) {
        softpc_ccpu_entry entry;
        driver->restore_active = LIB_TRUE;
        result = softpc_machine_reset(driver->machine) == SOFTPC_MACHINE_OK &&
            softpc_snapshot_image_restore(&driver->staged_image, &entry) ==
            LIB_STATUS_OK;
        /* The restored CPU has an exact continuation boundary.  Let its first
           checkpoint enter Common's already-requested pause rendezvous before
           decoding another guest instruction. */
        if (result) {
            driver->restore_active = LIB_FALSE;
            driver->restore_rendezvous_pending = LIB_TRUE;
            result = softpc_ccpu_lifecycle_resume(&entry) != 0;
        }
        driver->restore_active = LIB_FALSE;
        driver->restore_rendezvous_pending = LIB_FALSE;
        softpc_snapshot_image_dispose(&driver->staged_image);
        driver->restore_pending = LIB_FALSE;
        if (result) vm_driver_executor_event(driver);
    } else
        result = softpc_machine_run(driver->machine, UINT64_MAX) == SOFTPC_MACHINE_OK;
    /* finish retains READY only when clock restart fails. Do not resume CCPU
       after that terminal capture failure. */
    result = result && driver->capture.phase != SOFTPC_SNAPSHOT_READY;
    softpc_ccpu_lifecycle_observe(NULL, NULL);
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
    if (driver == NULL) return;
    driver->executor_callback = callback;
    driver->executor_context = callback_context;
    softpc_machine_set_executor_callback(driver->machine,
        callback == NULL ? NULL : vm_driver_executor_event,
        callback == NULL ? NULL : driver);
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

static lib_status vm_driver_copy_graphics(vm_driver *driver,
    common_machine_frame *frame)
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

    if (!softpc_machine_presentation_dib(driver->machine, &bits, &info, &width,
            &height)) return LIB_STATUS_OK;
    if (bits == NULL || info == NULL || width == 0u || height == 0u)
        return LIB_STATUS_INVALID_ARGUMENT;
    if (width > KVM_WINDOW_GRAPHICS_MAX_WIDTH || height > KVM_WINDOW_GRAPHICS_MAX_HEIGHT)
        return LIB_STATUS_UNSUPPORTED;
    if (!softpc_machine_presentation_take_dirty(driver->machine, &left, &top,
            &right, &bottom)) return LIB_STATUS_OK;
    row_stride = (width + 3u) & ~3u;
    memset(&frame->window, 0, lib_offsetof(kvm_window_frame, image.pixels));
    for (row = 0u; row < height; ++row)
        memcpy(frame->window.image.pixels + row * width,
            (const lib_u8 *)bits + row * row_stride, width);
    dib = (const BITMAPINFO *)info;
    for (palette_index = 0u; palette_index < KVM_WINDOW_GRAPHICS_PALETTE_ENTRIES;
            ++palette_index) {
        const RGBQUAD *colour = &dib->bmiColors[palette_index];
        frame->window.image.palette[palette_index] =
            ((lib_u32)colour->rgbRed << 16u) |
            ((lib_u32)colour->rgbGreen << 8u) | (lib_u32)colour->rgbBlue;
    }
    frame->window.image.width = width;
    frame->window.image.height = height;
    frame->window.image.stride = width;
    frame->window.image.dirty_left = left < 0 ? 0 : left;
    frame->window.image.dirty_top = top < 0 ? 0 : top;
    frame->window.image.dirty_right = right >= (lib_i32)width ? (lib_i32)width - 1 : right;
    frame->window.image.dirty_bottom = bottom >= (lib_i32)height ? (lib_i32)height - 1 : bottom;
    frame->window.graphics = 1u;
    frame->window.valid = 1u;
    return LIB_STATUS_OK;
}

static lib_status vm_driver_copy_text(vm_driver *driver,
    common_machine_frame *frame)
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
            &rows, &stride, &cell_bytes)) return LIB_STATUS_OK;
    if (surface == NULL || cell_bytes == 0u || columns == 0u || rows == 0u)
        return LIB_STATUS_INVALID_ARGUMENT;
    if (columns > KVM_TEXT_COLUMNS || rows > KVM_TEXT_ROWS)
        return LIB_STATUS_UNSUPPORTED;
    if (stride < columns) return LIB_STATUS_INVALID_ARGUMENT;
    memset(&frame->window, 0, lib_offsetof(kvm_window_frame, text) +
        sizeof(frame->window.text));
    memset(frame->window.text.base.text, ' ', sizeof(frame->window.text.base.text));
    memset(frame->window.text.base.attributes, 0x07, sizeof(frame->window.text.base.attributes));
    cells = (const lib_u8 *)surface;
    for (text_row = 0u; text_row < rows; ++text_row) {
        lib_u32 text_column;
        for (text_column = 0u; text_column < columns; ++text_column) {
            size_t source = ((size_t)text_row * stride + text_column) * cell_bytes;
            size_t destination = (size_t)text_row * KVM_TEXT_COLUMNS + text_column;
            frame->window.text.base.text[destination] = cells[source];
            if (cell_bytes >= 2u) frame->window.text.base.attributes[destination] = cells[source + 1u];
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
                frame->window.text.base.text_palette[index] = ((lib_u32)dib->bmiColors[index].rgbRed << 16u) |
                    ((lib_u32)dib->bmiColors[index].rgbGreen << 8u) |
                    (lib_u32)dib->bmiColors[index].rgbBlue;
        }
    }
    (void)softpc_machine_presentation_cursor(driver->machine, &column, &row,
        &cursor_size);
    frame->window.text.base.cursor_column = column;
    frame->window.text.base.cursor_row = row;
    (void)softpc_machine_presentation_fonts(driver->machine, frame->window.text.font,
        frame->window.text.secondary_font, &frame->window.text.base.font_height,
        &frame->window.text.base.attribute_font_select);
    frame->window.text.base.text_columns = (lib_u16)columns;
    frame->window.text.base.text_rows = (lib_u16)rows;
    frame->window.valid = 1u;
    {
        lib_status status = kvm_window_frame_validate(&frame->window);
        if (status != LIB_STATUS_OK) return status;
    }
    vm_driver_cursor_shape(&frame->window.text.base, cursor_size);
    frame->window.text.base.cursor_visible = column >= 0 && row >= 0;
    frame->window.text.base.cursor_phase = 1u;
    memcpy(frame->characters.primary, vm_driver_pc_glyphs, sizeof(vm_driver_pc_glyphs));
    memcpy(frame->characters.secondary, vm_driver_pc_glyphs, sizeof(vm_driver_pc_glyphs));
    return LIB_STATUS_OK;
}

static lib_status vm_driver_copy_frame(void *opaque, common_machine_frame *frame)
{
    vm_driver *driver = (vm_driver *)opaque;
    if (driver == NULL || frame == NULL) return LIB_STATUS_INVALID_ARGUMENT;
    frame->window.valid = 0u;
    /* A graphics route has one valid representation: a complete graphics
       frame.  In particular, restoration must wait for the rebuilt painter
       rather than publishing an 80x25 text fallback as a false graphics
       result; that fallback would create a wrongly sized black Window. */
    if (softpc_machine_presentation_is_graphics(driver->machine))
        return vm_driver_copy_graphics(driver, frame);
    return vm_driver_copy_text(driver, frame);
}

static lib_bool vm_driver_set_removable_media(void *opaque,
    const char *path, lib_storage_medium_mode mode)
{
    vm_driver *driver = (vm_driver *)opaque;
    return driver != NULL && softpc_machine_set_floppy(driver->machine, path,
        mode) == SOFTPC_MACHINE_OK;
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

static lib_status vm_driver_begin_state_read(void *opaque,
    const common_machine_state_writer *writer)
{
    vm_driver *driver = (vm_driver *)opaque;
    if (driver == NULL || writer == NULL || writer->write == NULL)
        return LIB_STATUS_INVALID_STATE;
    /* A prior save has already stopped at the VM-owned checkpoint. Its image
       remains stable until that paused executor is resumed, so another save
       may write it without running another guest instruction. */
    if (driver->capture.phase == SOFTPC_SNAPSHOT_READY) {
        driver->state_read_status = softpc_snapshot_image_write(
            &driver->captured_image, (softpc_snapshot_bytes_write)writer->write,
            writer->context);
        driver->state_read_ready = LIB_TRUE;
        return LIB_STATUS_OK;
    }
    if (driver->capture.phase != SOFTPC_SNAPSHOT_IDLE)
        return LIB_STATUS_INVALID_STATE;
    driver->state_writer = *writer;
    driver->state_read_status = LIB_STATUS_INVALID_STATE;
    driver->state_read_ready = LIB_FALSE;
    return softpc_snapshot_begin(&driver->capture);
}

static lib_bool vm_driver_take_state_read_result(void *opaque,
    lib_status *out_status)
{
    vm_driver *driver = (vm_driver *)opaque;
    if (driver == NULL || out_status == NULL || !driver->state_read_ready)
        return LIB_FALSE;
    *out_status = driver->state_read_status;
    return LIB_TRUE;
}

static lib_status vm_driver_write_state(void *opaque,
    const common_machine_state_reader *reader)
{
    vm_driver *driver = opaque;
    softpc_snapshot_image staged = {0};
    lib_status status;
    if (driver == NULL || reader == NULL || reader->read == NULL ||
        driver->restore_pending) return LIB_STATUS_INVALID_STATE;
    status = softpc_snapshot_image_read(&staged,
        softpc_machine_memory_bytes(driver->machine),
        (softpc_snapshot_bytes_read)reader->read, reader->context);
    if (status == LIB_STATUS_OK)
        status = softpc_machine_prepare_media(driver->machine, staged.media);
    if (status != LIB_STATUS_OK) {
        softpc_snapshot_image_dispose(&staged);
        return status;
    }
    softpc_snapshot_image_dispose(&driver->staged_image);
    driver->staged_image = staged;
    driver->restore_pending = LIB_TRUE;
    return LIB_STATUS_OK;
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
    if (driver == NULL) return;
    softpc_snapshot_image_dispose(&driver->captured_image);
    softpc_snapshot_image_dispose(&driver->staged_image);
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
    out_driver->begin_state_read = vm_driver_begin_state_read;
    out_driver->take_state_read_result = vm_driver_take_state_read_result;
    out_driver->execute_debug = vm_driver_debug;
    out_driver->take_debug_stop = vm_driver_take_debug_stop;
    out_driver->cancel_debug = vm_driver_cancel_debug;
    out_driver->write_state = vm_driver_write_state;
    out_driver->frame_published = vm_driver_trace_frame;
}
