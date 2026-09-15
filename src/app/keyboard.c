#include "keyboard.h"

#include "common/machine/machine_interface.h"
#include <string.h>

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
          { 'T', KVM_HOTKEY_MODIFIER_CONTROL | KVM_HOTKEY_MODIFIER_ALT,
              "send-alt-tab" },
          { 'M', KVM_HOTKEY_MODIFIER_CONTROL | KVM_HOTKEY_MODIFIER_ALT,
              "release-window-mouse" } }, 5u };

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

static int app_keyboard_submit_alt_key(void *context, kvm_input_sink sink,
    lib_u32 scan, kvm_key key)
{
    return app_keyboard_release_ctrl_alt(context, sink) &&
        app_keyboard_emit(context, sink, 0x38u, KVM_KEY_ALT, 1) &&
        app_keyboard_emit(context, sink, scan, key, 1) &&
        app_keyboard_emit(context, sink, scan, key, 0) &&
        app_keyboard_emit(context, sink, 0x38u, KVM_KEY_ALT, 0);
}

lib_bool app_keyboard_handle_hotkey(common_machine *machine,
    common_session_machine_state state, const char *identifier,
    common_session_command_result *out)
{
    common_session_request request = COMMON_SESSION_REQUEST_NONE;
    *out = (common_session_command_result) { 0 };
    if (identifier == NULL) return LIB_FALSE;
    if (strcmp(identifier, "pause-toggle") == 0) {
        request = state == COMMON_SESSION_MACHINE_PAUSED ?
            COMMON_SESSION_REQUEST_RESUME : COMMON_SESSION_REQUEST_PAUSE;
        out->request = request;
        return LIB_TRUE;
    }
    if (strcmp(identifier, "release-window-mouse") == 0) {
        out->release_window_mouse = LIB_TRUE;
        return LIB_TRUE;
    }
    if (state != COMMON_SESSION_MACHINE_RUNNING) return LIB_TRUE;
    if (strcmp(identifier, "send-ctrl-alt-del") == 0)
        return app_keyboard_submit_ctrl_alt_del(machine,
            app_keyboard_deliver_input) != 0;
    if (strcmp(identifier, "send-alt-enter") == 0)
        return app_keyboard_submit_alt_key(machine,
            app_keyboard_deliver_input, 0x1cu, KVM_KEY_ENTER) != 0;
    if (strcmp(identifier, "send-alt-tab") == 0)
        return app_keyboard_submit_alt_key(machine,
            app_keyboard_deliver_input, 0x0fu, KVM_KEY_TAB) != 0;
    return LIB_TRUE;
}
