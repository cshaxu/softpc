#include "lib/types/test.h"
#include "lib/types/win32/test.h"
#include "lib/kvm-base/input_interface.h"
#include "lib/kvm-base/hotkey_interface.h"
#include "lib/types/win32/input.h"


#ifdef _WIN32
typedef struct shared_keyboard_capture {
    lib_u8 keys[16];
    lib_u8 releases[16];
    lib_u8 modifiers[16];
    lib_u32 identities[16];
    lib_u32 flags[16];
    lib_u32 count;
} shared_keyboard_capture;

static lib_bool capture_key(void *context, const kvm_input_event *event)
{
    shared_keyboard_capture *capture = (shared_keyboard_capture *)context;
    if (event == LIB_NULL || event->type != KVM_EVENT_KEY ||
        capture->count == sizeof(capture->keys)) return LIB_FALSE;
    capture->keys[capture->count] = (lib_u8)event->data.key.scan_code;
    capture->releases[capture->count] = (lib_u8)!event->data.key.pressed;
    capture->modifiers[capture->count] = event->data.key.modifiers;
    capture->identities[capture->count] = event->data.key.key;
    capture->flags[capture->count++] = event->data.key.flags;
    return LIB_TRUE;
}

typedef struct shared_hotkey_capture {
    kvm_hotkey_matcher matcher;
    kvm_input_event events[8];
    lib_u32 count;
} shared_hotkey_capture;

static lib_bool capture_hotkey(void *context, const kvm_input_event *event)
{
    shared_hotkey_capture *capture = (shared_hotkey_capture *)context;
    if (capture == LIB_NULL || event == LIB_NULL || capture->count == 8u) return LIB_FALSE;
    capture->events[capture->count++] = *event;
    return LIB_TRUE;
}

static lib_bool normalize_and_match(void *context, const kvm_input_event *event)
{
    shared_hotkey_capture *capture = (shared_hotkey_capture *)context;
    return capture != LIB_NULL && kvm_hotkey_matcher_submit(&capture->matcher,
        event, capture_hotkey, capture, LIB_TRUE);
}

static void assert_registered_raw_chord(lib_u32 trigger, lib_u16 scan, const char *identifier)
{
    kvm_hotkey_registry registry;
    shared_hotkey_capture capture = { 0 };
    const lib_u8 control_alt = KVM_HOTKEY_MODIFIER_CONTROL |
        KVM_HOTKEY_MODIFIER_ALT;

    kvm_hotkey_registry_initialize(&registry);
    lib_test_assert(kvm_hotkey_registry_register(&registry, trigger, control_alt,
        identifier) == LIB_STATUS_OK);
    kvm_hotkey_matcher_initialize(&capture.matcher, &registry);
    lib_test_assert(kvm_keyboard_submit_record(&(base_keyboard_decoder){ 0 }, LIB_NULL,
        &capture, normalize_and_match, &(base_keyboard_record){ BASE_KEYBOARD_TRANSITION,
            0x1du, LIB_WIN32_KEY_CONTROL, 0u, 0u, KVM_HOTKEY_MODIFIER_CONTROL, 1, 1u }));
    lib_test_assert(kvm_keyboard_submit_record(&(base_keyboard_decoder){ 0 }, LIB_NULL,
        &capture, normalize_and_match, &(base_keyboard_record){ BASE_KEYBOARD_TRANSITION,
            0x38u, LIB_WIN32_KEY_ALT, 0u, 0u, control_alt, 1, 1u }));
    lib_test_assert(kvm_keyboard_submit_record(&(base_keyboard_decoder){ 0 }, LIB_NULL,
        &capture, normalize_and_match, &(base_keyboard_record){ BASE_KEYBOARD_TRANSITION,
            scan, (lib_u16)trigger, 0u, 0u, control_alt, 1, 1u }));
    lib_test_assert(capture.count == 1u && capture.events[0].type == KVM_EVENT_HOTKEY);
    lib_test_assert(lib_text_compare((const char *)capture.events[0].data.hotkey.identifier, identifier) == 0);
    /* Every make and break in the matched raw chord is private to KVM. */
    lib_test_assert(kvm_keyboard_submit_record(&(base_keyboard_decoder){ 0 }, LIB_NULL,
        &capture, normalize_and_match, &(base_keyboard_record){ BASE_KEYBOARD_TRANSITION,
            scan, (lib_u16)trigger, 0u, 0u, control_alt, 0, 1u }));
    lib_test_assert(kvm_keyboard_submit_record(&(base_keyboard_decoder){ 0 }, LIB_NULL,
        &capture, normalize_and_match, &(base_keyboard_record){ BASE_KEYBOARD_TRANSITION,
            0x38u, LIB_WIN32_KEY_ALT, 0u, 0u, KVM_HOTKEY_MODIFIER_CONTROL, 0, 1u }));
    lib_test_assert(kvm_keyboard_submit_record(&(base_keyboard_decoder){ 0 }, LIB_NULL,
        &capture, normalize_and_match, &(base_keyboard_record){ BASE_KEYBOARD_TRANSITION,
            0x1du, LIB_WIN32_KEY_CONTROL, 0u, 0u, 0u, 0, 1u }));
    lib_test_assert(capture.count == 1u);
    kvm_hotkey_matcher_discard(&capture.matcher);
}

int main(void)
{
    /* PC set-1 identities distinguish physical sides and keypad keys without
       requiring a consumer to interpret a native virtual-key value. */
    const struct {
        lib_u16 scan, native_key;
        lib_bool extended;
        lib_key key;
    } physical[] = {
        { 0x2au, LIB_WIN32_KEY_LEFT_SHIFT, LIB_FALSE, LIB_KEY_SHIFT },
        { 0x36u, LIB_WIN32_KEY_RIGHT_SHIFT, LIB_FALSE, LIB_KEY_SHIFT },
        { 0x1du, LIB_WIN32_KEY_CONTROL, LIB_FALSE, LIB_KEY_CONTROL },
        { 0x1du, LIB_WIN32_KEY_CONTROL, LIB_TRUE, LIB_KEY_CONTROL },
        { 0x1cu, LIB_WIN32_KEY_RETURN, LIB_FALSE, LIB_KEY_ENTER },
        { 0x1cu, LIB_WIN32_KEY_RETURN, LIB_TRUE, LIB_KEY_ENTER },
        { 0x47u, LIB_WIN32_KEY_NUMPAD_0 + 7u, LIB_FALSE, LIB_KEY_KEYPAD_7 },
        { 0x47u, LIB_WIN32_KEY_HOME, LIB_TRUE, LIB_KEY_HOME },
        { 0x45u, LIB_WIN32_KEY_PAUSE, LIB_FALSE, LIB_KEY_PAUSE },
        { 0x37u, LIB_WIN32_KEY_SNAPSHOT, LIB_TRUE, LIB_KEY_PRINT_SCREEN }
    };
    for (lib_size i = 0; i < sizeof(physical) / sizeof(physical[0]); ++i) {
        base_keyboard_decoder decoder = { 0 };
        base_keyboard_record native = {
            .kind = BASE_KEYBOARD_COMBINED, .scan = physical[i].scan,
            .key = physical[i].native_key,
            .flags = physical[i].extended ? LIB_KEY_FLAG_EXTENDED : 0u,
            .pressed = LIB_TRUE, .repeat_count = 3u
        };
        lib_key_input neutral;
        lib_test_assert(base_keyboard_decode(&decoder, &native, &neutral) ==
            BASE_KEYBOARD_ACCEPTED);
        lib_test_assert(neutral.key == physical[i].key && neutral.scalar == 0u &&
            neutral.scan_code == physical[i].scan &&
            neutral.extended == physical[i].extended && neutral.repeat_count == 3u);
        native.pressed = LIB_FALSE;
        lib_test_assert(base_keyboard_decode(&decoder, &native, &neutral) ==
            BASE_KEYBOARD_ACCEPTED);
        lib_test_assert(!neutral.pressed && neutral.repeat_count == 1u);
    }
    shared_keyboard_capture capture = { 0 };
    base_keyboard_decoder normalizer = { 0 };
    shared_hotkey_capture text = { 0 };
    lib_test_assert(kvm_keyboard_submit_record(&normalizer, LIB_NULL, &text, capture_hotkey,
        &(base_keyboard_record){ .kind=BASE_KEYBOARD_CHARACTER, .utf16=0xd83du, .repeat_count=1u }));
    lib_test_assert(text.count == 0u);
    lib_test_assert(kvm_keyboard_submit_record(&normalizer, LIB_NULL, &text, capture_hotkey,
        &(base_keyboard_record){ .kind=BASE_KEYBOARD_CHARACTER, .utf16=0xde00u, .repeat_count=1u }));
    lib_test_assert(text.count == 1u && text.events[0].type == KVM_EVENT_TEXT &&
        text.events[0].data.text.scalar == 0x1f600u);
    lib_test_assert(!kvm_keyboard_submit_record(&normalizer, LIB_NULL, &text, capture_hotkey,
        &(base_keyboard_record){ .kind=BASE_KEYBOARD_CHARACTER, .utf16=0xdc00u, .repeat_count=1u }));
    lib_test_assert(kvm_keyboard_submit_record(&normalizer, LIB_NULL, &text, capture_hotkey,
        &(base_keyboard_record){ .kind=BASE_KEYBOARD_CHARACTER, .utf16=0xd800u, .repeat_count=1u }));
    lib_test_assert(kvm_keyboard_submit_record(&normalizer, LIB_NULL, &text, capture_hotkey,
        &(base_keyboard_record){ .kind=BASE_KEYBOARD_CHARACTER, .utf16=0xd800u, .repeat_count=1u }));
    lib_test_assert(normalizer.pending_high_surrogate == 0xd800u);
    lib_test_assert(kvm_keyboard_submit_record(&normalizer, LIB_NULL, &text, capture_hotkey,
        &(base_keyboard_record){ .kind=BASE_KEYBOARD_CHARACTER, .utf16=0xd800u, .repeat_count=1u }));
    lib_test_assert(kvm_keyboard_submit_record(&normalizer, LIB_NULL, &text, capture_hotkey,
        &(base_keyboard_record){ .kind=BASE_KEYBOARD_CHARACTER, .utf16='a', .repeat_count=1u }));
    lib_test_assert(kvm_keyboard_submit_record(&normalizer, LIB_NULL, &text, capture_hotkey,
        &(base_keyboard_record){ .kind=BASE_KEYBOARD_CHARACTER, .utf16=0x4e00u, .repeat_count=1u }));
    text.count = 8u;
    lib_test_assert(!kvm_keyboard_submit_record(&normalizer, LIB_NULL, &text, capture_hotkey,
        &(base_keyboard_record){ .kind=BASE_KEYBOARD_CHARACTER, .utf16=0x4e00u, .repeat_count=1u }));

    /* The shared component preserves the host physical scan; each project maps it. */
    lib_test_assert(kvm_keyboard_submit_record(&(base_keyboard_decoder){ 0 }, LIB_NULL,
        &capture, capture_key, &(base_keyboard_record){ BASE_KEYBOARD_TRANSITION,
            0x1eu, 'A', 0u, 0u, 0u, 1, 1u }));
    lib_test_assert(kvm_keyboard_submit_record(&(base_keyboard_decoder){ 0 }, LIB_NULL,
        &capture, capture_key, &(base_keyboard_record){ BASE_KEYBOARD_TRANSITION,
            0x1eu, 'A', 0u, 0u, 0u, 0, 1u }));
    lib_test_assert(capture.count == 2u);
    lib_test_assert(capture.keys[0] == 0x1eu && capture.releases[0] == 0u);
    lib_test_assert(capture.keys[1] == 0x1eu && capture.releases[1] == 1u);
    lib_test_assert(capture.identities[0] == 'A' && capture.identities[1] == 'A');

    /* Esc is an ordinary original key-table entry (key 110), not a host
       stop command. */
    capture.count = 0u;
    lib_test_assert(kvm_keyboard_submit_record(&(base_keyboard_decoder){ 0 }, LIB_NULL,
        &capture, capture_key, &(base_keyboard_record){ BASE_KEYBOARD_TRANSITION,
            0x01u, LIB_WIN32_KEY_ESCAPE, 0u, 0u, 0u, 1, 1u }));
    lib_test_assert(kvm_keyboard_submit_record(&(base_keyboard_decoder){ 0 }, LIB_NULL,
        &capture, capture_key, &(base_keyboard_record){ BASE_KEYBOARD_TRANSITION,
            0x01u, LIB_WIN32_KEY_ESCAPE, 0u, 0u, 0u, 0, 1u }));
    lib_test_assert(capture.count == 2u);
    lib_test_assert(capture.keys[0] == 0x01u && capture.releases[0] == 0u);
    lib_test_assert(capture.keys[1] == 0x01u && capture.releases[1] == 1u);

    /* A raw Console record from RDP can retain LIB_WIN32_KEY_RETURN while reporting no
       physical scan.  Both KVM leaves use this shared recovery path, so the
       guest still receives the normal Enter make/break pair. */
    capture.count = 0u;
    lib_test_assert(kvm_keyboard_submit_record(&(base_keyboard_decoder){ 0 }, LIB_NULL,
        &capture, capture_key, &(base_keyboard_record){ BASE_KEYBOARD_TRANSITION,
            0u, LIB_WIN32_KEY_RETURN, 0u, 0u, 0u, 1, 1u }));
    lib_test_assert(kvm_keyboard_submit_record(&(base_keyboard_decoder){ 0 }, LIB_NULL,
        &capture, capture_key, &(base_keyboard_record){ BASE_KEYBOARD_TRANSITION,
            0u, LIB_WIN32_KEY_RETURN, 0u, 0u, 0u, 0, 1u }));
    lib_test_assert(capture.count == 2u);
    lib_test_assert(capture.keys[0] == 0x1cu && capture.releases[0] == 0u);
    lib_test_assert(capture.keys[1] == 0x1cu && capture.releases[1] == 1u);
    lib_test_assert(capture.identities[0] == LIB_KEY_ENTER &&
        capture.identities[1] == LIB_KEY_ENTER);

    /* Extended state is a neutral KVM flag, not a copied Win32 control bit. */
    capture.count = 0u;
    lib_test_assert(kvm_keyboard_submit_record(&(base_keyboard_decoder){ 0 }, LIB_NULL,
        &capture, capture_key, &(base_keyboard_record){ BASE_KEYBOARD_TRANSITION,
            0xe04du, LIB_WIN32_KEY_RIGHT, 0u, LIB_KEY_FLAG_EXTENDED, 0u, 1, 1u }));
    lib_test_assert(capture.count == 1u && capture.identities[0] == LIB_KEY_RIGHT);
    lib_test_assert(capture.flags[0] == LIB_KEY_FLAG_EXTENDED);
    lib_test_assert(capture.keys[0] == 0x4du);
    lib_test_assert(kvm_keyboard_submit_record(&(base_keyboard_decoder){ 0 }, LIB_NULL,
        &capture, capture_key, &(base_keyboard_record){ BASE_KEYBOARD_TRANSITION,
            0x4du, LIB_WIN32_KEY_RIGHT, 0u, LIB_KEY_FLAG_EXTENDED, 0u, 1, 1u }));
    lib_test_assert(capture.keys[1] == capture.keys[0] && capture.flags[1] == capture.flags[0]);

    /* Raw Console input provides its own per-record modifier state.  These
       registrations must not depend on process-global lib_win32_get_key_state(), which
       RDP does not reliably update for lib_win32_input_record delivery. */
    assert_registered_raw_chord('P', 0x19, "pause-toggle");
    assert_registered_raw_chord('D', 0x20, "send-ctrl-alt-del");
    assert_registered_raw_chord('F', 0x21, "send-alt-enter");

    /* Window supplies a mask at its own native boundary; the common
       normalizer preserves that value rather than replacing it globally. */
    capture.count = 0u;
    lib_test_assert(kvm_keyboard_submit_record(&(base_keyboard_decoder){ 0 }, LIB_NULL,
        &capture, capture_key, &(base_keyboard_record){ BASE_KEYBOARD_TRANSITION,
            0x19u, 'P', 0u, 0u, KVM_HOTKEY_MODIFIER_CONTROL |
        KVM_HOTKEY_MODIFIER_ALT, 1, 1u }));
    lib_test_assert(capture.count == 1u);
    lib_test_assert(capture.modifiers[0] == (KVM_HOTKEY_MODIFIER_CONTROL |
        KVM_HOTKEY_MODIFIER_ALT));

    /* Only unmapped transitions ask the native adapter for character translation. */
    capture.count = 0;
    base_keyboard_record record = { .kind = BASE_KEYBOARD_TRANSITION, .key = 'A', .pressed = LIB_TRUE };
    lib_test_assert(kvm_keyboard_submit_record(&normalizer, LIB_NULL, &capture, capture_key, &record) ==
        BASE_KEYBOARD_ACCEPTED);
    record.pressed = LIB_FALSE;
    lib_test_assert(kvm_keyboard_submit_record(&normalizer, LIB_NULL, &capture, capture_key, &record) ==
        BASE_KEYBOARD_ACCEPTED);
    lib_test_assert(capture.count == 2);
    record.key = LIB_WIN32_KEY_PACKET; record.pressed = LIB_TRUE;
    lib_test_assert(kvm_keyboard_submit_record(&normalizer, LIB_NULL, &capture, capture_key, &record) ==
        BASE_KEYBOARD_UNMAPPED);
    lib_test_assert(capture.count == 2);

    /* UTF-16 input uses the active host layout to synthesize make/break;
       it never places text directly in guest memory. */
    capture.count = 0u;
    lib_test_assert(kvm_keyboard_submit_record(&normalizer, LIB_NULL, &capture, capture_key,
        &(base_keyboard_record){ .kind=BASE_KEYBOARD_CHARACTER, .utf16=L'a', .repeat_count=1u }));
    lib_test_assert(capture.count == 2u);
    lib_test_assert(capture.keys[0] == 0x1eu && capture.releases[0] == 0u);
    lib_test_assert(capture.keys[1] == 0x1eu && capture.releases[1] == 1u);
    return 0;
}
#else
int main(void) { return 0; }
#endif
