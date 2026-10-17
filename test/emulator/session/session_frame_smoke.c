#include "lib/types/test.h"
#include "emulator/session/session_interface.h"

static emulator_machine_frame published;
static lib_u32 current_run = 7u, published_run = 7u, copies, deliveries;
static lib_u32 delivered_sequence;
static lib_bool delivered_graphics, delivered_status;
static lib_u32 fake_run(const emulator_machine *machine)
{ (void)machine; return current_run; }
static lib_bool fake_read(emulator_machine *machine, lib_u32 run,
    lib_u32 *sequence, lib_bool *graphics, kvm_console_text_frame *target)
{
    (void)machine;
    ++copies;
    if (!published.window.valid || run != published_run) return LIB_FALSE;
    *sequence = published.sequence;
    *graphics = published.window.graphics;
    if (!*graphics && target != LIB_NULL) {
        target->base = published.window.text.base;
        target->characters = published.characters;
    }
    return LIB_TRUE;
}
static kvm_frame_exchange *fake_exchange(emulator_machine *machine)
{ (void)machine; return (kvm_frame_exchange *)&published; }

/* Run the production completion consumer with publication independently ahead
 * of its queued notification. No timing guess or second reducer is involved. */
#define emulator_machine_run_generation fake_run
#define emulator_machine_read_presentation fake_read
#define emulator_machine_frame_exchange fake_exchange
#include "emulator/session/session.c"
#undef emulator_machine_run_generation
#undef emulator_machine_read_presentation
#undef emulator_machine_frame_exchange

void emulator_ui_set_run_generation(emulator_ui *ui, lib_u32 run)
{ (void)ui; lib_test_assert(run == current_run); }
lib_status emulator_ui_apply_action(emulator_ui *ui, emulator_ui_action action, emulator_ui_state state)
{ (void)ui; (void)action; (void)state; return LIB_STATUS_OK; }
lib_status emulator_ui_set_state(emulator_ui *ui, emulator_ui_state state)
{ (void)ui; (void)state; return LIB_STATUS_OK; }
lib_status emulator_ui_write_monitor(emulator_ui *ui, const char *text)
{ (void)ui; (void)text; return LIB_STATUS_OK; }
lib_status emulator_ui_request_monitor_line(emulator_ui *ui)
{ (void)ui; return LIB_STATUS_OK; }
lib_status emulator_ui_cancel_monitor_line(emulator_ui *ui, lib_bool *out_completed)
{ (void)ui; *out_completed = LIB_FALSE; return LIB_STATUS_OK; }
lib_status emulator_ui_release_window_mouse(emulator_ui *ui)
{ (void)ui; return LIB_STATUS_OK; }
kvm_console_text_frame *emulator_ui_frame_target(emulator_ui *ui)
{ static kvm_console_text_frame target; (void)ui; return &target; }
lib_status emulator_ui_publish_frame(emulator_ui *ui, kvm_frame_exchange *frames,
    lib_bool graphics, lib_u32 sequence,
    lib_bool window, lib_bool console, lib_bool status)
{
    (void)ui;
    lib_test_assert(frames == (kvm_frame_exchange *)&published);
    const kvm_window_frame *frame = &published.window;
    lib_test_assert(graphics == frame->graphics);
    /* An old Window can remain actual until its queued destruction completion;
     * graphics, however, must never publish before Window creation completes. */
    lib_test_assert(console && (!frame->graphics || window));
    if (frame->graphics) {
        /* Delayed notification A receives full B, including A's earlier pixels. */
        lib_test_assert(frame->image.width == 4 && frame->image.height == 4);
        lib_test_assert(frame->image.pixels[0] == 0x123456 && frame->image.pixels[15] == 0xabcdef);
    }
    ++deliveries;
    delivered_sequence = sequence;
    delivered_graphics = frame->graphics != 0u;
    delivered_status = status;
    return LIB_STATUS_OK;
}
static void publication_value_contract(void)
{
    static emulator_machine_frame source, copy;
    source.sequence = 5u;
    source.window.valid = LIB_TRUE;
    source.window.text.base.text_columns = source.window.text.base.text_rows = 1u;
    source.window.text.base.font_height = 16u;
    source.window.text.base.cells[0].glyph_index = 'X';
    source.characters.primary['X'] = 0x263au;
    source.characters.secondary['X'] = 0x2665u;
    lib_test_assert(!emulator_machine_frame_copy(LIB_NULL, &source));
    lib_test_assert(!emulator_machine_frame_copy(&copy, LIB_NULL));
    lib_test_assert(emulator_machine_frame_copy(&copy, &source));
    source.characters.primary['X'] = 'Y';
    lib_test_assert(copy.sequence == 5u && copy.window.text.base.cells[0].glyph_index == 'X' &&
        copy.characters.primary['X'] == 0x263au && copy.characters.secondary['X'] == 0x2665u);
    lib_test_assert(emulator_machine_frame_copy(&copy, &copy));
    source.sequence = 6u;
    source.window.graphics = LIB_TRUE;
    source.window.image.width = source.window.image.height = source.window.image.stride = 1u;
    source.window.image.pixels[0] = 0x123456u;
    lib_test_assert(emulator_machine_frame_copy(&copy, &source));
    lib_test_assert(copy.sequence == 6u && copy.window.graphics && copy.window.image.pixels[0] == 0x123456u &&
        copy.characters.primary['X'] == 0x263au);
    /* Invalid Window payload cannot overwrite the publication's serial/map. */
    source.sequence = 7u;
    source.window.image.width = 0u;
    lib_test_assert(!emulator_machine_frame_copy(&copy, &source));
    lib_test_assert(copy.sequence == 6u && copy.window.image.width == 1u &&
        copy.characters.primary['X'] == 0x263au);
}

int main(void)
{
    publication_value_contract();
    static emulator_session session;
    emulator_session_event event = { 0 };
    emulator_session *output = &session;
    emulator_session_options options = { 0 };
    lib_test_assert(emulator_session_create(LIB_NULL, LIB_NULL) == LIB_STATUS_INVALID_ARGUMENT);
    lib_test_assert(emulator_session_create(&output, LIB_NULL) == LIB_STATUS_INVALID_ARGUMENT && output == LIB_NULL);
    output = &session;
    lib_test_assert(emulator_session_create(&output, &options) == LIB_STATUS_INVALID_ARGUMENT && output == LIB_NULL);
    /* Opaque handles are never dereferenced by the fakes. */
    session.machine = (emulator_machine *)&current_run;
    session.ui = (emulator_ui *)&current_run;
    emulator_session_state_initialize(&session.state, EMULATOR_SESSION_DISPLAY_CONSOLE, 0);
    emulator_session_state_note_runtime(&session.state, EMULATOR_SESSION_MACHINE_RUNNING);
    emulator_session_state_note_vm_console(&session.state, 1);
    emulator_session_state_note_current_console(&session.state, 1);
    lib_test_assert(emulator_session_state_note_frame(&session.state, 1u, 0));

    published.window.valid = LIB_TRUE;
    published.sequence = 3u;
    published.window.graphics = 1u;
    published.window.image.width = published.window.image.stride = 4;
    published.window.image.height = 4;
    published.window.image.pixels[0] = 0x123456; /* A changed the first corner. */
    published.window.image.pixels[15] = 0xabcdef; /* B changed the opposite corner. */
    event.kind = EMULATOR_SESSION_EVENT_FRAME_COMPLETED;
    event.run_generation = 7u;
    lib_test_assert(emulator_session_process_completed(&session, &event));
    lib_test_assert(copies == 1u && session.state.observed_frame_sequence == 3u);
    lib_test_assert(session.state.graphics_actual);
    lib_test_assert(session.state.in_flight == EMULATOR_UI_ACTION_CREATE_WINDOW);
    lib_test_assert(deliveries == 0u); /* Wait for component completion before publishing. */
    event.kind = EMULATOR_SESSION_EVENT_COMPONENT_COMPLETED;
    event.value.component.component = EMULATOR_SESSION_EVENT_COMPONENT_WINDOW;
    event.value.component.exists = 1;
    lib_test_assert(emulator_session_process_completed(&session, &event));
    lib_test_assert(deliveries == 1u && delivered_sequence == 3u &&
        delivered_graphics && delivered_status);

    event.kind = EMULATOR_SESSION_EVENT_FRAME_COMPLETED;
    lib_test_assert(emulator_session_process_completed(&session, &event));
    lib_test_assert(delivered_sequence == 3u); /* Reobserving never regresses content. */
    published.window.valid = LIB_FALSE;
    lib_test_assert(emulator_session_process_completed(&session, &event));
    lib_test_assert(session.state.observed_frame_sequence == 3u &&
        session.state.graphics_actual);
    published.window.valid = LIB_TRUE;
    published_run = 8u; /* Reset advanced between notification and snapshot. */
    lib_test_assert(emulator_session_process_completed(&session, &event));
    lib_test_assert(session.state.observed_frame_sequence == 3u);
    current_run = 8u;
    { lib_u32 prior = copies;
      lib_test_assert(emulator_session_process_completed(&session, &event));
      lib_test_assert(copies == prior); } /* Old-run event rejected before snapshot access. */
    current_run = published_run = 7u;
    published.sequence = 5u;
    published.window.graphics = 0u;
    lib_test_assert(emulator_session_process_completed(&session, &event));
    lib_test_assert(session.state.observed_frame_sequence == 5u &&
        !session.state.graphics_actual);
    lib_test_assert(session.state.in_flight == EMULATOR_UI_ACTION_DESTROY_WINDOW);
    event.kind = EMULATOR_SESSION_EVENT_COMPONENT_COMPLETED;
    event.value.component.component = EMULATOR_SESSION_EVENT_COMPONENT_WINDOW;
    event.value.component.exists = 0;
    lib_test_assert(emulator_session_process_completed(&session, &event));
    lib_test_assert(delivered_sequence == 5u && !delivered_graphics && !delivered_status);
    /* First consumption may already be near wrap; zero is never a frame. */
    {
        emulator_session_state state;
        emulator_session_state_initialize(&state, EMULATOR_SESSION_DISPLAY_WINDOW, 1);
        lib_test_assert(!emulator_session_state_note_frame(&state, 0u, 1));
        lib_test_assert(emulator_session_state_note_frame(&state, LIB_UINT32_MAX, 0));
        lib_test_assert(emulator_session_state_note_frame(&state, 1u, 1));
        lib_test_assert(!emulator_session_state_note_frame(&state, LIB_UINT32_MAX, 0));
        lib_test_assert(!emulator_session_state_note_frame(&state, 1u, 0));
        lib_test_assert(!emulator_session_state_note_frame(&state, 0u, 0));
        lib_test_assert(state.observed_frame_sequence == 1u && state.graphics_actual);
        /* Half-range is ambiguous and must not be accepted as newer. */
        lib_test_assert(!emulator_session_state_note_frame(&state, 0x80000001u, 0));
        lib_test_assert(emulator_session_state_note_frame(&state, 0x80000000u, 0));
    }
    session.state.observed_frame_sequence = LIB_UINT32_MAX - 1u;
    published.sequence = 1u;
    event.kind = EMULATOR_SESSION_EVENT_FRAME_COMPLETED;
    lib_u32 before = copies;
    lib_test_assert(emulator_session_process_completed(&session, &event));
    lib_test_assert(copies == before + 1u && session.state.observed_frame_sequence == 1u);
    lib_test_assert(session.state.observed_frame_sequence == 1u && delivered_sequence == 1u);
    /* Repeated wake hints always inspect the same latest publication. */
    for (lib_u32 i = 0u; i < 4u; ++i) {
        lib_test_assert(emulator_session_process_completed(&session, &event));
        lib_test_assert(copies == before + 2u + i && session.state.observed_frame_sequence == 1u);
    }
    before = copies;
    current_run = published_run = 8u;
    published.sequence = 2u;
    lib_test_assert(emulator_session_process_completed(&session, &event));
    lib_test_assert(copies == before); /* Old run cannot cross the wrap boundary. */
    emulator_session_state_note_runtime(&session.state, EMULATOR_SESSION_MACHINE_STOPPED);
    emulator_session_state_note_runtime(&session.state, EMULATOR_SESSION_MACHINE_RUNNING);
    event.run_generation = current_run;
    lib_test_assert(emulator_session_process_completed(&session, &event));
    lib_test_assert(copies == before + 1u && session.state.observed_frame_sequence == 2u);
    return 0;
}
