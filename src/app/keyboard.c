#include "keyboard.h"

#include "common/machine/machine_interface.h"

int app_keyboard_deliver_input(void *context, const kvm_input_event *event)
{
    return context != NULL && event != NULL && common_machine_enqueue_input(
        (common_machine *)context, event) != 0;
}

int app_keyboard_hotkeys(kvm_hotkey_registry *registry)
{
    static const kvm_hotkey_registry hotkeys = {
        { { 'P', KVM_HOTKEY_MODIFIER_CONTROL | KVM_HOTKEY_MODIFIER_ALT,
              "pause-toggle" },
          { 'D', KVM_HOTKEY_MODIFIER_CONTROL | KVM_HOTKEY_MODIFIER_ALT,
              "send-ctrl-alt-del" },
          { 'F', KVM_HOTKEY_MODIFIER_CONTROL | KVM_HOTKEY_MODIFIER_ALT,
              "send-alt-enter" },
          { 'M', KVM_HOTKEY_MODIFIER_CONTROL | KVM_HOTKEY_MODIFIER_ALT,
              "release-window-mouse" } }, 4u };

    if (registry == NULL) return 0;
    *registry = hotkeys;
    return 1;
}

static int app_keyboard_emit(void *context, kvm_input_sink sink, lib_u32 scan,
    lib_u32 key, int pressed)
{
    kvm_input_event event = { 0 };
    if (sink == NULL) return 0;
    event.type = KVM_EVENT_KEY;
    event.data.key.scan_code = scan;
    event.data.key.key = key;
    event.data.key.flags = (scan & 0x0100u) != 0u ? KVM_KEY_FLAG_EXTENDED : 0u;
    event.data.key.pressed = pressed != 0;
    return sink(context, &event);
}

int app_keyboard_release_ctrl_alt(void *context, kvm_input_sink sink)
{
    return app_keyboard_emit(context, sink, 0x1du, KVM_KEY_CONTROL, 0) &&
        app_keyboard_emit(context, sink, 0x38u, KVM_KEY_ALT, 0);
}

int app_keyboard_submit_ctrl_alt_del(void *context, kvm_input_sink sink)
{
    return app_keyboard_emit(context, sink, 0x1du, KVM_KEY_CONTROL, 1) &&
        app_keyboard_emit(context, sink, 0x38u, KVM_KEY_ALT, 1) &&
        app_keyboard_emit(context, sink, 0x0153u, KVM_KEY_DELETE, 1) &&
        app_keyboard_emit(context, sink, 0x0153u, KVM_KEY_DELETE, 0) &&
        app_keyboard_emit(context, sink, 0x38u, KVM_KEY_ALT, 0) &&
        app_keyboard_emit(context, sink, 0x1du, KVM_KEY_CONTROL, 0);
}

int app_keyboard_submit_alt_enter(void *context, kvm_input_sink sink)
{
    return app_keyboard_release_ctrl_alt(context, sink) &&
        app_keyboard_emit(context, sink, 0x38u, KVM_KEY_ALT, 1) &&
        app_keyboard_emit(context, sink, 0x1cu, KVM_KEY_ENTER, 1) &&
        app_keyboard_emit(context, sink, 0x1cu, KVM_KEY_ENTER, 0) &&
        app_keyboard_emit(context, sink, 0x38u, KVM_KEY_ALT, 0);
}
