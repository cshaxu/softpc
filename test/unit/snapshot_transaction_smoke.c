#include "vm/driver.h"
#include "compat/media_snapshot.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>

#undef assert
#define assert(condition) do { \
    if (!(condition)) { \
        fprintf(stderr, "snapshot transaction check failed: %s at line %d\n", \
            #condition, __LINE__); \
        return 1; \
    } \
} while (0)

typedef struct snapshot_bytes {
    lib_u8 *bytes;
    lib_size count;
    lib_size capacity;
    lib_size offset;
} snapshot_bytes;

static lib_status snapshot_write(void *opaque, const lib_u8 *bytes,
    lib_size byte_count)
{
    snapshot_bytes *stream = opaque;
    lib_u8 *replacement;
    lib_size required;
    if (stream == NULL || (bytes == NULL && byte_count != 0u) ||
        byte_count > (lib_size)-1 - stream->count)
        return LIB_STATUS_INVALID_ARGUMENT;
    required = stream->count + byte_count;
    if (required > stream->capacity) {
        replacement = realloc(stream->bytes, required);
        if (replacement == NULL) return LIB_STATUS_NO_MEMORY;
        stream->bytes = replacement;
        stream->capacity = required;
    }
    if (byte_count != 0u) memcpy(stream->bytes + stream->count, bytes, byte_count);
    stream->count = required;
    return LIB_STATUS_OK;
}

static lib_status snapshot_read(void *opaque, lib_u8 *bytes,
    lib_size byte_count)
{
    snapshot_bytes *stream = opaque;
    if (stream == NULL || (bytes == NULL && byte_count != 0u) ||
        stream->offset > stream->count || byte_count > stream->count - stream->offset)
        return LIB_STATUS_INVALID_ARGUMENT;
    if (byte_count != 0u) memcpy(bytes, stream->bytes + stream->offset, byte_count);
    stream->offset += byte_count;
    return LIB_STATUS_OK;
}

static void snapshot_set_u32_le(lib_u8 *bytes, lib_u32 value)
{
    bytes[0] = (lib_u8)value;
    bytes[1] = (lib_u8)(value >> 8u);
    bytes[2] = (lib_u8)(value >> 16u);
    bytes[3] = (lib_u8)(value >> 24u);
}

static void snapshot_set_u64_le(lib_u8 *bytes, lib_u64 value)
{
    lib_size index;
    for (index = 0u; index < 8u; ++index)
        bytes[index] = (lib_u8)(value >> (index * 8u));
}

static lib_u64 snapshot_get_u64_le(const lib_u8 *bytes)
{
    lib_u64 value = 0u;
    lib_size index;
    for (index = 0u; index < 8u; ++index)
        value |= (lib_u64)bytes[index] << (index * 8u);
    return value;
}

static lib_status snapshot_file_write(void *opaque, const lib_u8 *bytes,
    lib_size byte_count)
{
    FILE *file = opaque;
    if (file == NULL || (bytes == NULL && byte_count != 0u))
        return LIB_STATUS_INVALID_ARGUMENT;
    return byte_count == 0u || fwrite(bytes, 1u, byte_count, file) == byte_count ?
        LIB_STATUS_OK : LIB_STATUS_IO_ERROR;
}

static lib_status snapshot_file_read(void *opaque, lib_u8 *bytes,
    lib_size byte_count)
{
    FILE *file = opaque;
    if (file == NULL || (bytes == NULL && byte_count != 0u))
        return LIB_STATUS_INVALID_ARGUMENT;
    return byte_count == 0u || fread(bytes, 1u, byte_count, file) == byte_count ?
        LIB_STATUS_OK : LIB_STATUS_IO_ERROR;
}

static lib_bool wait_for_state(common_machine *machine,
    common_machine_state expected)
{
    DWORD deadline = GetTickCount() + 5000u;
    do {
        if (common_machine_state_get(machine) == expected) return LIB_TRUE;
        Sleep(10u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
    return LIB_FALSE;
}

static lib_bool snapshot_write_media(const char *path)
{
    lib_u8 sector[512] = { 0 };
    static const lib_u8 program[] = {
        0xb8, 0x13, 0x00, 0xcd, 0x10, /* BIOS mode 13h */
        0xb8, 0x00, 0xa0, 0x8e, 0xc0, /* ES = video memory */
        0xfa, 0xb0, 0x0c,             /* poll directly, without BIOS IRQ1 */
        0x31, 0xff, 0xb9, 0x00, 0xfa, /* 64000 pixels */
        0xfc, 0xf3, 0xaa,
        0xe4, 0x64, 0xa8, 0x01, 0x74, 0xfa, /* poll keyboard status */
        0xe4, 0x60, 0xb0, 0x0a, 0xeb, 0xec /* consume key; repaint */
    };
    FILE *file;

    if (path == NULL) return LIB_FALSE;
    memcpy(sector, program, sizeof(program));
    sector[510] = 0x55u; sector[511] = 0xaau;
    file = fopen(path, "wb");
    if (file == NULL) return LIB_FALSE;
    if (fwrite(sector, 1u, sizeof(sector), file) != sizeof(sector)) {
        fclose(file);
        return LIB_FALSE;
    }
    memset(sector, 0, sizeof(sector));
    for (unsigned i = 1; i < 17; ++i)
        if (fwrite(sector, 1u, sizeof(sector), file) != sizeof(sector)) {
            fclose(file); return LIB_FALSE;
        }
    return fclose(file) == 0 ? LIB_TRUE : LIB_FALSE;
}

static void snapshot_options(softpc_machine_options *options, const char *path)
{
    *options = (softpc_machine_options) { 0 };
    options->floppy_path = path;
    options->hard_disk_path = path;
    options->media_mode = LIB_STORAGE_MEDIUM_OVERLAY;
}

/* The executor is parked for these direct host-media fixture operations. */
static lib_bool snapshot_media_bytes(lib_bool write, lib_bool later)
{
    unsigned i;
    for (i = 0; i < 2; ++i) {
        softpc_media_view view;
        lib_u8 bytes[512];
        lib_u8 value = (lib_u8)(0x41u + i);
        if (i == 0) softpc_floppy_media_view(0, &view);
        else softpc_hdd_media_view(0, &view);
        if (view.medium == NULL) return LIB_FALSE;
        if (i == 0) {
            lib_storage_medium *no_replacement = NULL;
            if (write) {
                if (softpc_floppy_media_restore(0, &no_replacement,
                        later ? 1u : 37u) != LIB_STATUS_OK) return LIB_FALSE;
            } else if (view.cylinder != 37u) return LIB_FALSE;
        }
        memset(bytes, later ? 0x99 : value, sizeof(bytes));
        if (write) {
            if (lib_storage_medium_write_at(view.medium, later ? 4096 : 8192,
                    bytes, sizeof(bytes)) != LIB_STATUS_OK) return LIB_FALSE;
        } else {
            if (lib_storage_medium_read_at(view.medium, 8192, bytes,
                    sizeof(bytes)) != LIB_STATUS_OK) return LIB_FALSE;
            for (unsigned n = 0; n < sizeof(bytes); ++n)
                if (bytes[n] != value) return LIB_FALSE;
            if (lib_storage_medium_read_at(view.medium, 4096, bytes,
                    sizeof(bytes)) != LIB_STATUS_OK) return LIB_FALSE;
            for (unsigned n = 0; n < sizeof(bytes); ++n)
                if (bytes[n] != 0) return LIB_FALSE;
        }
    }
    return LIB_TRUE;
}

static lib_bool snapshot_has_pixels(common_machine *machine, lib_u8 colour)
{
    kvm_frame *frame = calloc(1, sizeof(*frame));
    lib_bool found = LIB_FALSE;
    if (frame == NULL) return LIB_FALSE;
    if (common_machine_copy_published_frame(machine, frame,
            common_machine_run_generation(machine)) && frame->graphics) {
        lib_size i;
        found = LIB_TRUE;
        for (i = 0; i < frame->graphics_height * frame->graphics_stride; ++i)
            if (frame->graphics_pixels[i] != colour) { found = LIB_FALSE; break; }
    }
    free(frame);
    return found;
}

static lib_bool snapshot_wait_for_pixels(common_machine *machine, lib_u8 colour)
{
    DWORD deadline = GetTickCount() + 10000u;
    do {
        if (snapshot_has_pixels(machine, colour)) return LIB_TRUE;
        Sleep(10u);
    } while ((LONG)(GetTickCount() - deadline) < 0);
    return LIB_FALSE;
}

static int snapshot_run_transaction(void)
{
    const char *path = "softpc-snapshot-transaction-smoke.img";
    softpc_machine_options options = { 0 };
    common_machine_driver description = { 0 };
    softpc_machine *product = NULL;
    vm_driver *driver = NULL;
    common_machine *machine = NULL;
    snapshot_bytes stream = { 0 };
    assert(snapshot_write_media(path));
    snapshot_options(&options, path);
    assert(softpc_machine_create(&options, &product) == SOFTPC_MACHINE_OK);
    assert(vm_driver_create(&driver, product) == LIB_STATUS_OK);
    vm_driver_describe(driver, &description);
    assert(common_machine_create(&machine, &description) == LIB_STATUS_OK);

    assert(common_machine_start(machine));
    assert(wait_for_state(machine, COMMON_MACHINE_RUNNING));
    assert(snapshot_wait_for_pixels(machine, 0x0c));
    assert(common_machine_pause(machine));
    assert(wait_for_state(machine, COMMON_MACHINE_PAUSED));
    assert(snapshot_media_bytes(LIB_TRUE, LIB_FALSE));
    assert(common_machine_resume(machine));
    assert(wait_for_state(machine, COMMON_MACHINE_RUNNING));
    assert(common_machine_read_state(machine,
        &(common_machine_state_writer) { snapshot_write, &stream }) ==
        LIB_STATUS_OK);
    assert(stream.count != 0u);
    assert(common_machine_state_get(machine) == COMMON_MACHINE_PAUSED);
    assert(snapshot_has_pixels(machine, 0x0c));
    assert(snapshot_media_bytes(LIB_TRUE, LIB_TRUE));
    assert(common_machine_stop(machine));
    assert(wait_for_state(machine, COMMON_MACHINE_STOPPED));

    /* The container carries no host or machine word-width field. */
    assert(stream.count >= 16u);
    assert(stream.bytes[4] == 6u && stream.bytes[5] == 0u &&
        stream.bytes[6] == 0u && stream.bytes[7] == 0u);
    assert(stream.bytes[8] == 3u && stream.bytes[9] == 0u &&
        stream.bytes[10] == 0u && stream.bytes[11] == 0u);
    stream.offset = 0u;
    assert(common_machine_write_state(machine,
        &(common_machine_state_reader) { snapshot_read, &stream }) ==
        LIB_STATUS_OK);
    assert(common_machine_state_get(machine) == COMMON_MACHINE_PAUSED);
    assert(snapshot_media_bytes(LIB_FALSE, LIB_FALSE));
    assert(common_machine_stop(machine));
    assert(wait_for_state(machine, COMMON_MACHINE_STOPPED));

    /* The retired v1 host-width header is not a supported image variant. */
    snapshot_set_u32_le(stream.bytes + 4u, 1u);
    stream.offset = 0u;
    assert(common_machine_write_state(machine,
        &(common_machine_state_reader) { snapshot_read, &stream }) !=
        LIB_STATUS_OK);
    assert(common_machine_state_get(machine) == COMMON_MACHINE_STOPPED);
    snapshot_set_u32_le(stream.bytes + 4u, 6u);

    /* v4 lost the live map/read/bit masks; do not guess missing registers. */
    snapshot_set_u32_le(stream.bytes + 4u, 4u);
    stream.offset = 0u;
    assert(common_machine_write_state(machine,
        &(common_machine_state_reader) { snapshot_read, &stream }) !=
        LIB_STATUS_OK);
    assert(common_machine_state_get(machine) == COMMON_MACHINE_STOPPED);
    snapshot_set_u32_le(stream.bytes + 4u, 6u);

    /* A declared section boundary must be consumed exactly, never ignored. */
    {
        lib_u64 core_bytes = snapshot_get_u64_le(stream.bytes + 20u);
        assert(core_bytes != UINT64_MAX);
        snapshot_set_u64_le(stream.bytes + 20u, core_bytes + 1u);
    }
    stream.offset = 0u;
    assert(common_machine_write_state(machine,
        &(common_machine_state_reader) { snapshot_read, &stream }) !=
        LIB_STATUS_OK);
    assert(common_machine_state_get(machine) == COMMON_MACHINE_STOPPED);
    snapshot_set_u64_le(stream.bytes + 20u,
        snapshot_get_u64_le(stream.bytes + 20u) - 1u);

    stream.bytes[0] ^= 0xffu;
    stream.offset = 0u;
    assert(common_machine_write_state(machine,
        &(common_machine_state_reader) { snapshot_read, &stream }) !=
        LIB_STATUS_OK);
    assert(common_machine_state_get(machine) == COMMON_MACHINE_STOPPED);
    stream.bytes[0] ^= 0xffu;
    stream.offset = 0u;
    assert(common_machine_write_state(machine,
        &(common_machine_state_reader) { snapshot_read, &stream }) ==
        LIB_STATUS_OK);
    assert(common_machine_state_get(machine) == COMMON_MACHINE_PAUSED);
    {
        kvm_frame frame = { 0 };
        assert(common_machine_copy_published_frame(machine, &frame,
            common_machine_run_generation(machine)));
        assert(frame.valid != 0u);
    }
    assert(common_machine_resume(machine));
    assert(wait_for_state(machine, COMMON_MACHINE_RUNNING));
    assert(common_machine_stop(machine));
    assert(wait_for_state(machine, COMMON_MACHINE_STOPPED));

    common_machine_destroy(machine);
    vm_driver_destroy(driver);
    softpc_machine_destroy(product);
    free(stream.bytes);
    assert(remove(path) == 0);
    return 0;
}

static int snapshot_run_save(const char *media_path, const char *snapshot_path)
{
    softpc_machine_options options;
    common_machine_driver description = { 0 };
    softpc_machine *product = NULL;
    vm_driver *driver = NULL;
    common_machine *machine = NULL;
    FILE *file;

    assert(snapshot_write_media(media_path));
    snapshot_options(&options, media_path);
    assert(softpc_machine_create(&options, &product) == SOFTPC_MACHINE_OK);
    assert(vm_driver_create(&driver, product) == LIB_STATUS_OK);
    vm_driver_describe(driver, &description);
    assert(common_machine_create(&machine, &description) == LIB_STATUS_OK);
    file = fopen(snapshot_path, "wb");
    assert(file != NULL);
    assert(common_machine_start(machine));
    assert(wait_for_state(machine, COMMON_MACHINE_RUNNING));
    assert(snapshot_wait_for_pixels(machine, 0x0c));
    assert(common_machine_pause(machine));
    assert(wait_for_state(machine, COMMON_MACHINE_PAUSED));
    assert(snapshot_media_bytes(LIB_TRUE, LIB_FALSE));
    assert(common_machine_resume(machine));
    assert(wait_for_state(machine, COMMON_MACHINE_RUNNING));
    assert(common_machine_read_state(machine,
        &(common_machine_state_writer) { snapshot_file_write, file }) ==
        LIB_STATUS_OK);
    assert(fclose(file) == 0);
    assert(common_machine_state_get(machine) == COMMON_MACHINE_PAUSED);
    common_machine_destroy(machine);
    vm_driver_destroy(driver);
    softpc_machine_destroy(product);
    return 0;
}

static int snapshot_run_load(const char *media_path, const char *snapshot_path,
    lib_bool expect_success)
{
    softpc_machine_options options;
    common_machine_driver description = { 0 };
    softpc_machine *product = NULL;
    vm_driver *driver = NULL;
    common_machine *machine = NULL;
    FILE *file = fopen(snapshot_path, "rb");

    assert(file != NULL);
    snapshot_options(&options, media_path);
    if (!expect_success) options.memory_bytes = 1024u * 1024u;
    assert(softpc_machine_create(&options, &product) == SOFTPC_MACHINE_OK);
    assert(vm_driver_create(&driver, product) == LIB_STATUS_OK);
    vm_driver_describe(driver, &description);
    assert(common_machine_create(&machine, &description) == LIB_STATUS_OK);
    if (!expect_success) {
        assert(common_machine_write_state(machine,
            &(common_machine_state_reader) { snapshot_file_read, file }) !=
            LIB_STATUS_OK);
        assert(common_machine_state_get(machine) == COMMON_MACHINE_STOPPED);
        assert(fclose(file) == 0);
        common_machine_destroy(machine);
        vm_driver_destroy(driver);
        softpc_machine_destroy(product);
        return 0;
    }
    assert(common_machine_write_state(machine,
        &(common_machine_state_reader) { snapshot_file_read, file }) ==
        LIB_STATUS_OK);
    assert(fclose(file) == 0);
    assert(common_machine_state_get(machine) == COMMON_MACHINE_PAUSED);
    {
        kvm_frame frame = { 0 };
        assert(common_machine_copy_published_frame(machine, &frame,
            common_machine_run_generation(machine)));
        assert(frame.valid != 0u);
    }
    assert(snapshot_has_pixels(machine, 0x0c));
    assert(snapshot_media_bytes(LIB_FALSE, LIB_FALSE));
    assert(common_machine_resume(machine));
    assert(wait_for_state(machine, COMMON_MACHINE_RUNNING));
    {
        kvm_input_event event = { 0 };
        event.type = KVM_EVENT_KEY;
        event.data.key.key = KVM_KEY_ENTER;
        event.data.key.scan_code = 0x1c;
        event.data.key.pressed = 1;
        assert(common_machine_enqueue_input(machine, &event));
        event.data.key.pressed = 0;
        assert(common_machine_enqueue_input(machine, &event));
        /* The restored CPU must redraw every pixel through its actual video
           write rules, not merely retain the first reconstructed frame. */
        assert(snapshot_wait_for_pixels(machine, 0x0a));
    }
    assert(common_machine_stop(machine));
    assert(wait_for_state(machine, COMMON_MACHINE_STOPPED));
    common_machine_destroy(machine);
    vm_driver_destroy(driver);
    softpc_machine_destroy(product);
    return 0;
}

int main(int argc, char **argv)
{
    if (argc == 1) return snapshot_run_transaction();
    if (argc == 4 && strcmp(argv[1], "save") == 0)
        return snapshot_run_save(argv[2], argv[3]);
    if (argc == 4 && strcmp(argv[1], "load") == 0)
        return snapshot_run_load(argv[2], argv[3], LIB_TRUE);
    if (argc == 4 && strcmp(argv[1], "load-mismatch") == 0)
        return snapshot_run_load(argv[2], argv[3], LIB_FALSE);
    fprintf(stderr, "usage: %s [save|load media snapshot]\n", argv[0]);
    return 1;
}
#else
int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    return 0;
}
#endif
