#include "lib/types/types_interface.h"
#include "product/keyboard.h"
#include <assert.h>

/* Fake only the machine input boundary; use the real App dispatcher/matcher. */
struct common_machine {
    kvm_input_event events[8];
    unsigned count, attempts, reject_at, hotkeys, ordinary;
};

lib_bool common_machine_enqueue_input(common_machine *machine,
    const kvm_input_event *event)
{
    assert(machine != NULL && event != NULL);
    if (++machine->attempts == machine->reject_at) return LIB_FALSE;
    assert(machine->count < 8u);
    machine->events[machine->count++] = *event;
    return LIB_TRUE;
}

static void check_sequence(common_machine *machine, kvm_key key, lib_u32 scan)
{
    const kvm_key keys[] = { KVM_KEY_CONTROL, KVM_KEY_ALT, KVM_KEY_ALT,
        key, key, KVM_KEY_ALT };
    const lib_u32 scans[] = { 0x1du, 0x38u, 0x38u, scan, scan, 0x38u };
    const lib_bool pressed[] = { LIB_FALSE, LIB_FALSE, LIB_TRUE, LIB_TRUE,
        LIB_FALSE, LIB_FALSE };
    assert(machine->count == 6u && machine->attempts == 6u);
    for (unsigned i = 0; i < 6u; ++i) {
        const kvm_input_event *event = &machine->events[i];
        assert(event->type == KVM_EVENT_KEY);
        assert(event->data.key.key == keys[i]);
        assert(event->data.key.scan_code == scans[i]);
        assert(event->data.key.pressed == pressed[i]);
        assert(event->data.key.flags == 0u);
    }
}

static lib_bool matched(void *context, const kvm_input_event *event)
{
    common_machine *machine = context;
    common_session_command_result result;
    if (event->type != KVM_EVENT_HOTKEY) {
        ++machine->ordinary;
        return LIB_TRUE;
    }
    ++machine->hotkeys;
    return app_keyboard_handle_hotkey(machine, COMMON_SESSION_MACHINE_RUNNING,
        event->data.hotkey.identifier, &result);
}

int main(void)
{
    kvm_hotkey_registry registry;
    common_session_command_result result;
    common_machine machine = { 0 };
    const common_session_machine_state inactive[] = {
        COMMON_SESSION_MACHINE_INIT, COMMON_SESSION_MACHINE_STOPPED,
        COMMON_SESSION_MACHINE_PAUSED, COMMON_SESSION_MACHINE_ERROR,
        COMMON_SESSION_MACHINE_RESET_COMPLETED
    };
    assert(app_keyboard_hotkeys(&registry) && registry.count == 5u);
    unsigned found = 0u;
    for (unsigned i = 0; i < registry.count; ++i) {
        if (registry.entries[i].key == 'T') {
            assert(registry.entries[i].modifiers ==
                (KVM_HOTKEY_MODIFIER_CONTROL | KVM_HOTKEY_MODIFIER_ALT));
            assert(!lib_text_compare(registry.entries[i].identifier, "send-alt-tab"));
            ++found;
        }
    }
    assert(found == 1u);
    for (unsigned i = 0; i < sizeof(inactive) / sizeof(inactive[0]); ++i) {
        assert(app_keyboard_handle_hotkey(&machine, inactive[i], "send-alt-tab", &result));
        assert(result.request == COMMON_SESSION_REQUEST_NONE && machine.attempts == 0u);
    }
    /* Each leaf receives this registry and owns an independent matcher. */
    for (unsigned source = 0; source < 2u; ++source) {
        kvm_hotkey_matcher matcher;
        const kvm_key keys[] = { KVM_KEY_CONTROL, KVM_KEY_ALT, 'T',
            'T', KVM_KEY_ALT, KVM_KEY_CONTROL };
        const lib_u32 scans[] = { 0x1du, 0x38u, 0x14u, 0x14u, 0x38u, 0x1du };
        machine = (common_machine){ 0 };
        kvm_hotkey_matcher_initialize(&matcher, &registry);
        for (unsigned i = 0; i < 6u; ++i) {
            kvm_input_event event = { .type = KVM_EVENT_KEY };
            event.data.key.key = keys[i];
            event.data.key.scan_code = scans[i];
            event.data.key.pressed = i < 3u;
            event.data.key.modifiers = KVM_KEY_MODIFIER_CONTROL | KVM_KEY_MODIFIER_ALT;
            assert(kvm_hotkey_matcher_submit(&matcher, &event, matched, &machine, LIB_TRUE));
        }
        assert(machine.hotkeys == 1u && machine.ordinary == 0u);
        check_sequence(&machine, KVM_KEY_TAB, 0x0fu);
        kvm_hotkey_matcher_discard(&matcher);
    }
    machine = (common_machine){ 0 };
    assert(app_keyboard_handle_hotkey(&machine, COMMON_SESSION_MACHINE_RUNNING,
        "send-alt-enter", &result));
    check_sequence(&machine, KVM_KEY_ENTER, 0x1cu);
    for (unsigned reject = 1u; reject <= 6u; ++reject) {
        machine = (common_machine){ .reject_at = reject };
        assert(!app_keyboard_handle_hotkey(&machine, COMMON_SESSION_MACHINE_RUNNING,
            "send-alt-tab", &result));
        assert(machine.attempts == reject && machine.count == reject - 1u);
    }
    return 0;
}
