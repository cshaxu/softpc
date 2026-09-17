#include "vm/driver.h"

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

int main(void)
{
    const char *path = "softpc-snapshot-transaction-smoke.img";
    lib_u8 sector[512] = { 0 };
    softpc_machine_options options = { 0 };
    common_machine_driver description = { 0 };
    softpc_machine *product = NULL;
    vm_driver *driver = NULL;
    common_machine *machine = NULL;
    snapshot_bytes stream = { 0 };
    FILE *file;

    sector[0] = 0xebu; sector[1] = 0xfeu;
    sector[510] = 0x55u; sector[511] = 0xaau;
    file = fopen(path, "wb");
    assert(file != NULL);
    assert(fwrite(sector, 1u, sizeof(sector), file) == sizeof(sector));
    assert(fclose(file) == 0);
    options.floppy_path = path;
    options.media_mode = LIB_STORAGE_MEDIUM_OVERLAY;
    assert(softpc_machine_create(&options, &product) == SOFTPC_MACHINE_OK);
    assert(vm_driver_create(&driver, product) == LIB_STATUS_OK);
    vm_driver_describe(driver, &description);
    assert(common_machine_create(&machine, &description) == LIB_STATUS_OK);

    assert(common_machine_start(machine));
    assert(wait_for_state(machine, COMMON_MACHINE_RUNNING));
    assert(common_machine_read_state(machine,
        &(common_machine_state_writer) { snapshot_write, &stream }) ==
        LIB_STATUS_OK);
    assert(stream.count != 0u);
    assert(common_machine_state_get(machine) == COMMON_MACHINE_PAUSED);
    assert(common_machine_stop(machine));
    assert(wait_for_state(machine, COMMON_MACHINE_STOPPED));

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
#else
int main(void)
{
    return 0;
}
#endif
