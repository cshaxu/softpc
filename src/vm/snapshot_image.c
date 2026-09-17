#include "snapshot.h"

#include "compat/devices/archive.h"

#include <stdlib.h>
#include <string.h>

enum {
    SOFTPC_SNAPSHOT_IMAGE_MAGIC = 0x53435053u,
    SOFTPC_SNAPSHOT_IMAGE_VERSION = 1u,
    /* The virtual machine contract is IA-32.  This is an image-format word
       width, not the pointer width of the process that happened to save it. */
    SOFTPC_SNAPSHOT_MACHINE_WORD_BITS = 32u,
    SOFTPC_SNAPSHOT_SECTION_CORE = 1u,
    SOFTPC_SNAPSHOT_SECTION_DEVICES = 2u,
    SOFTPC_SNAPSHOT_SECTION_COUNT = 2u,
    SOFTPC_SNAPSHOT_DEVICE_MAX_BYTES = 4u * 1024u * 1024u
};

typedef struct softpc_snapshot_memory_stream {
    lib_u8 *bytes;
    lib_size count;
    lib_size capacity;
    lib_size offset;
    lib_size limit;
} softpc_snapshot_memory_stream;

static lib_status
softpc_snapshot_memory_write(void *context, const lib_u8 *bytes,
    lib_size byte_count)
{
    softpc_snapshot_memory_stream *stream = context;
    lib_size required;
    lib_u8 *replacement;
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

static lib_status
softpc_snapshot_memory_read(void *context, lib_u8 *bytes, lib_size byte_count)
{
    softpc_snapshot_memory_stream *stream = context;
    if (stream == NULL || (bytes == NULL && byte_count != 0u) ||
        stream->offset > stream->limit || byte_count > stream->limit - stream->offset)
        return LIB_STATUS_INVALID_ARGUMENT;
    if (byte_count != 0u) memcpy(bytes, stream->bytes + stream->offset, byte_count);
    stream->offset += byte_count;
    return LIB_STATUS_OK;
}

static lib_status
softpc_snapshot_image_write_section(softpc_snapshot_bytes_write write,
    void *context, lib_u32 identifier, const softpc_snapshot_memory_stream *stream)
{
    lib_status status;
    if (stream == NULL || stream->count > UINT32_MAX) return LIB_STATUS_LIMIT_EXCEEDED;
    status = softpc_snapshot_stream_write_u32(write, context, identifier);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_write_u32(write,
        context, (uint32_t)stream->count);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_write_bytes(write,
        context, stream->bytes, stream->count);
    return status;
}

void softpc_snapshot_image_dispose(softpc_snapshot_image *image)
{
    if (image == NULL) return;
    softpc_ccpu_archive_dispose(&image->ccpu);
    image->entry = (softpc_ccpu_entry){0};
}

lib_status softpc_snapshot_image_capture(softpc_snapshot_image *image,
    const softpc_ccpu_entry *entry)
{
    if (image == NULL || entry == NULL ||
        (entry->halted != 0 && entry->halted != 1) || entry->trap > 1ul ||
        (!entry->halted && entry->trap != 0ul))
        return LIB_STATUS_INVALID_ARGUMENT;
    if (!softpc_ccpu_archive_capture(&image->ccpu)) return LIB_STATUS_IO_ERROR;
    image->entry = *entry;
    return LIB_STATUS_OK;
}

lib_status softpc_snapshot_image_restore(const softpc_snapshot_image *image,
    softpc_ccpu_entry *entry)
{
    if (image == NULL || entry == NULL || !image->ccpu.valid)
        return LIB_STATUS_INVALID_ARGUMENT;
    if (!softpc_ccpu_archive_restore(&image->ccpu)) return LIB_STATUS_IO_ERROR;
    *entry = image->entry;
    return LIB_STATUS_OK;
}

lib_status softpc_snapshot_image_write(const softpc_snapshot_image *image,
    softpc_snapshot_bytes_write write, void *context)
{
    softpc_snapshot_memory_stream core = {0}, devices = {0};
    lib_status status;
    if (image == NULL || !image->ccpu.valid || image->ccpu.devices == NULL ||
        write == NULL || (image->entry.halted != 0 && image->entry.halted != 1) ||
        image->entry.trap > 1ul || (!image->entry.halted && image->entry.trap != 0ul))
        return LIB_STATUS_INVALID_ARGUMENT;
    status = softpc_ccpu_archive_write_core(&image->ccpu,
        softpc_snapshot_memory_write, &core);
    if (status == LIB_STATUS_OK) status = softpc_device_archive_write(
        image->ccpu.devices, softpc_snapshot_memory_write, &devices);
    if (status == LIB_STATUS_OK &&
        (core.count > UINT32_MAX || devices.count > SOFTPC_SNAPSHOT_DEVICE_MAX_BYTES))
        status = LIB_STATUS_LIMIT_EXCEEDED;
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_write_u32(write,
        context, SOFTPC_SNAPSHOT_IMAGE_MAGIC);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_write_u32(write,
        context, SOFTPC_SNAPSHOT_IMAGE_VERSION);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_write_u32(write,
        context, SOFTPC_SNAPSHOT_MACHINE_WORD_BITS);
    if (status == LIB_STATUS_OK && image->ccpu.sas.memory_bytes > UINT32_MAX)
        status = LIB_STATUS_LIMIT_EXCEEDED;
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_write_u32(write,
        context, SOFTPC_SNAPSHOT_SECTION_COUNT);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_write_u32(write,
        context, (uint32_t)image->ccpu.sas.memory_bytes);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_image_write_section(write,
        context, SOFTPC_SNAPSHOT_SECTION_CORE, &core);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_image_write_section(write,
        context, SOFTPC_SNAPSHOT_SECTION_DEVICES, &devices);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_write_u32(write,
        context, (uint32_t)image->entry.halted);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_write_u32(write,
        context, (uint32_t)image->entry.trap);
    free(core.bytes); free(devices.bytes);
    return status;
}

lib_status softpc_snapshot_image_read(softpc_snapshot_image *image,
    lib_u32 expected_memory_bytes, softpc_snapshot_bytes_read read,
    void *context)
{
    softpc_snapshot_memory_stream core = {0}, devices = {0};
    softpc_snapshot_image decoded = {0};
    lib_u32 magic, version, source_width, sections, memory_bytes, identifier, length, halted, trap;
    lib_status status;
    if (image == NULL || expected_memory_bytes == 0u || read == NULL)
        return LIB_STATUS_INVALID_ARGUMENT;
    status = softpc_snapshot_stream_read_u32(read, context, &magic);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_read_u32(read, context, &version);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_read_u32(read, context, &source_width);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_read_u32(read, context, &sections);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_read_u32(read, context, &memory_bytes);
    if (status != LIB_STATUS_OK) return status;
    /* This is the virtual machine's IA-32 wire contract, never a host
       pointer-width compatibility switch. */
    if (magic != SOFTPC_SNAPSHOT_IMAGE_MAGIC || version != SOFTPC_SNAPSHOT_IMAGE_VERSION ||
        source_width != SOFTPC_SNAPSHOT_MACHINE_WORD_BITS ||
        sections != SOFTPC_SNAPSHOT_SECTION_COUNT ||
        memory_bytes != expected_memory_bytes) {
        return LIB_STATUS_INVALID_ARGUMENT;
    }
    status = softpc_snapshot_stream_read_u32(read, context, &identifier);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_read_u32(read, context, &length);
    if (status != LIB_STATUS_OK || identifier != SOFTPC_SNAPSHOT_SECTION_CORE || length == 0u)
        return status == LIB_STATUS_OK ? LIB_STATUS_INVALID_ARGUMENT : status;
    if (length < memory_bytes ||
        length > (lib_u64)memory_bytes + 2u * 1024u * 1024u)
        return LIB_STATUS_INVALID_ARGUMENT;
    core.bytes = malloc(length); core.count = core.capacity = core.limit = length;
    if (core.bytes == NULL) return LIB_STATUS_NO_MEMORY;
    status = softpc_snapshot_stream_read_bytes(read, context, core.bytes, length);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_read_u32(read, context, &identifier);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_read_u32(read, context, &length);
    if (status != LIB_STATUS_OK || identifier != SOFTPC_SNAPSHOT_SECTION_DEVICES ||
        length == 0u || length > SOFTPC_SNAPSHOT_DEVICE_MAX_BYTES) {
        free(core.bytes); return status == LIB_STATUS_OK ? LIB_STATUS_INVALID_ARGUMENT : status;
    }
    devices.bytes = malloc(length); devices.count = devices.capacity = devices.limit = length;
    if (devices.bytes == NULL) { free(core.bytes); return LIB_STATUS_NO_MEMORY; }
    status = softpc_snapshot_stream_read_bytes(read, context, devices.bytes, length);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_read_u32(read, context, &halted);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_read_u32(read, context, &trap);
    if (status == LIB_STATUS_OK && (halted > 1u || trap > 1u || (halted == 0u && trap != 0u))) status = LIB_STATUS_INVALID_ARGUMENT;
    if (status == LIB_STATUS_OK) status = softpc_ccpu_archive_read_core(
        &decoded.ccpu, memory_bytes, softpc_snapshot_memory_read, &core);
    if (status == LIB_STATUS_OK && core.offset != core.limit) status = LIB_STATUS_INVALID_ARGUMENT;
    if (status == LIB_STATUS_OK) status = softpc_device_archive_read(&decoded.ccpu.devices,
        softpc_snapshot_memory_read, &devices);
    if (status == LIB_STATUS_OK && devices.offset != devices.limit) status = LIB_STATUS_INVALID_ARGUMENT;
    free(core.bytes); free(devices.bytes);
    if (status != LIB_STATUS_OK) {
        softpc_snapshot_image_dispose(&decoded);
        return status;
    }
    decoded.ccpu.valid = 1;
    decoded.entry.halted = (int)halted; decoded.entry.trap = (unsigned long)trap;
    softpc_snapshot_image_dispose(image); *image = decoded;
    return LIB_STATUS_OK;
}
