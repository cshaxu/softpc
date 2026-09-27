#include "lib/types/test.h"
#include "lib/types/win32/test.h"
#include "lib/types/win32/window.h"
#include "lib/kvm-window/window.h"
#include "common/session/control.h"

static void *test_window_context;
static lib_win32_long_ptr LIB_WIN32_WINAPI test_get_window_context(
    lib_win32_hwnd window, lib_i32 index)
{
    (void)window;
    return index == LIB_WIN32_GWLP_USERDATA ?
        (lib_win32_long_ptr)test_window_context : 0;
}

/* Exercise the native Window loss notifications through the same final input
 * sink that a product uses to reach Common Session's delivered-key ledger. */
#undef lib_win32_get_window_long_ptr_a
#define lib_win32_get_window_long_ptr_a test_get_window_context
#include "lib/kvm-window/win32/component.c"

typedef struct input_capture {
    lib_u32 makes;
    lib_u32 breaks;
} input_capture;

static common_session_queue queue;
static kvm_window window;
static kvm_win32_window_context context;
static input_capture capture;
static lib_u32 reset_events, failures;

static lib_bool receive_guest(void *opaque, const kvm_input_event *event)
{
    input_capture *received = (input_capture *)opaque;

    lib_test_assert(event->type == KVM_EVENT_KEY);
    if (event->data.key.pressed) ++received->makes;
    else ++received->breaks;
    return LIB_TRUE;
}

static lib_bool receive_window(void *opaque, const kvm_input_event *event)
{
    lib_bool accepted;

    (void)opaque;
    if (event->type == KVM_EVENT_INPUT_RESET) ++reset_events;
    accepted = common_session_dispatch_input(&queue, event,
        COMMON_SESSION_MACHINE_RUNNING, receive_guest, &capture);
    return accepted;
}

static void failure(void *opaque, lib_u64 source_identity, lib_status status)
{
    (void)opaque;
    lib_test_assert(source_identity);
    lib_test_assert(status == LIB_STATUS_IO_ERROR);
    ++failures;
}

static lib_status join(kvm_component *component, lib_u32 timeout_ms)
{
    (void)component;
    (void)timeout_ms;
    return LIB_STATUS_OK;
}

static void dispose(kvm_component *component)
{
    kvm_component_mailboxes_destroy(&component->mailboxes);
}

static void initialize(void)
{
    kvm_component_options options = { 0 };

    options.input_sink = receive_window;
    options.failure_sink = failure;
    lib_memory_set(&window, 0, sizeof(window));
    lib_memory_set(&context, 0, sizeof(context));
    lib_memory_set(&capture, 0, sizeof(capture));
    reset_events = failures = 0u;
    lib_test_assert(common_session_queue_initialize(&queue));
    lib_test_assert(kvm_component_initialize(&window.base, &options, join, dispose,
        &window.pending_frame, sizeof(window.pending_frame)) == LIB_STATUS_OK);
    lib_test_assert(kvm_component_mailboxes_select_notify(&window.base.mailboxes,
        LIB_NULL, LIB_NULL) == LIB_STATUS_OK);
    context.component = &window;
    test_window_context = &context;
}

static void make(kvm_key key, lib_u16 scan_code)
{
    kvm_input_event event = { .type = KVM_EVENT_KEY };

    event.data.key.key = key;
    event.data.key.scan_code = scan_code;
    event.data.key.pressed = LIB_TRUE;
    lib_test_assert(win32_window_emit_normalized(&context, &event));
}

int main(void)
{
    initialize();
    make('A', 0x1eu);
    lib_test_assert(capture.makes == 1u && capture.breaks == 0u &&
        queue.pressed_count == 1u);
    win32_window_proc((lib_win32_hwnd)1, LIB_WIN32_WM_KILLFOCUS, 0u, 0u);
    lib_test_assert(reset_events == 1u && failures == 0u &&
        capture.breaks == 1u && queue.pressed_count == 0u);
    win32_window_proc((lib_win32_hwnd)1, LIB_WIN32_WM_KILLFOCUS, 0u, 0u);
    lib_test_assert(reset_events == 2u && failures == 0u &&
        capture.breaks == 1u); /* Duplicate loss is harmless. */

    make('B', 0x30u);
    lib_test_assert(capture.makes == 2u && queue.pressed_count == 1u);
    win32_window_proc((lib_win32_hwnd)1, LIB_WIN32_WM_ACTIVATEAPP, LIB_WIN32_FALSE, 0u);
    lib_test_assert(reset_events == 3u && failures == 0u &&
        capture.breaks == 2u && queue.pressed_count == 0u);
    win32_window_proc((lib_win32_hwnd)1, LIB_WIN32_WM_ACTIVATEAPP, LIB_WIN32_FALSE, 0u);
    lib_test_assert(reset_events == 4u && failures == 0u && capture.breaks == 2u);

    lib_test_assert(kvm_component_destroy(&window.base) == LIB_STATUS_OK);
    common_session_queue_dispose(&queue);
    return 0;
}
