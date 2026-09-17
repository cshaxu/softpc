#include "snapshot.h"

#include "compat/devices/archive.h"

enum {
    SOFTPC_SNAPSHOT_IMAGE_MAGIC = 0x53435053u,
    SOFTPC_SNAPSHOT_IMAGE_VERSION = 4u,
    SOFTPC_SNAPSHOT_SECTION_CORE = 1u,
    SOFTPC_SNAPSHOT_SECTION_DEVICES = 2u,
    SOFTPC_SNAPSHOT_SECTION_COUNT = 2u
};

typedef struct softpc_snapshot_count_stream {
    lib_u64 count;
} softpc_snapshot_count_stream;

static lib_status
softpc_snapshot_count_write(void *context, const lib_u8 *bytes,
    lib_size byte_count)
{
    softpc_snapshot_count_stream *stream = context;
    if (stream == NULL || (bytes == NULL && byte_count != 0u) ||
        (lib_u64)byte_count > UINT64_MAX - stream->count)
        return LIB_STATUS_INVALID_ARGUMENT;
    stream->count += (lib_u64)byte_count;
    return LIB_STATUS_OK;
}

typedef struct softpc_snapshot_section_reader {
    softpc_snapshot_bytes_read read;
    void *context;
    lib_u64 remaining;
} softpc_snapshot_section_reader;

static lib_status
softpc_snapshot_section_read(void *opaque, lib_u8 *bytes, lib_size byte_count)
{
    softpc_snapshot_section_reader *reader = opaque;
    lib_status status;

    if (reader == NULL || reader->read == NULL ||
        (bytes == NULL && byte_count != 0u) ||
        (lib_u64)byte_count > reader->remaining)
        return LIB_STATUS_INVALID_ARGUMENT;
    status = reader->read(reader->context, bytes, byte_count);
    if (status == LIB_STATUS_OK) reader->remaining -= (lib_u64)byte_count;
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
    softpc_snapshot_count_stream core = {0}, devices = {0};
    lib_status status;
    if (image == NULL || !image->ccpu.valid || image->ccpu.devices == NULL ||
        write == NULL || (image->entry.halted != 0 && image->entry.halted != 1) ||
        image->entry.trap > 1ul || (!image->entry.halted && image->entry.trap != 0ul))
        return LIB_STATUS_INVALID_ARGUMENT;
    status = softpc_ccpu_archive_write_core(&image->ccpu,
        softpc_snapshot_count_write, &core);
    if (status == LIB_STATUS_OK) status = softpc_device_archive_write(
        image->ccpu.devices, softpc_snapshot_count_write, &devices);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_write_u32(write,
        context, SOFTPC_SNAPSHOT_IMAGE_MAGIC);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_write_u32(write,
        context, SOFTPC_SNAPSHOT_IMAGE_VERSION);
    if (status == LIB_STATUS_OK && image->ccpu.sas.memory_bytes > UINT32_MAX)
        status = LIB_STATUS_LIMIT_EXCEEDED;
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_write_u32(write,
        context, SOFTPC_SNAPSHOT_SECTION_COUNT);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_write_u32(write,
        context, (uint32_t)image->ccpu.sas.memory_bytes);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_write_u32(write,
        context, SOFTPC_SNAPSHOT_SECTION_CORE);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_write_u64(write,
        context, core.count);
    if (status == LIB_STATUS_OK) status = softpc_ccpu_archive_write_core(
        &image->ccpu, write, context);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_write_u32(write,
        context, SOFTPC_SNAPSHOT_SECTION_DEVICES);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_write_u64(write,
        context, devices.count);
    if (status == LIB_STATUS_OK) status = softpc_device_archive_write(
        image->ccpu.devices, write, context);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_write_u32(write,
        context, (uint32_t)image->entry.halted);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_write_u32(write,
        context, (uint32_t)image->entry.trap);
    return status;
}

lib_status softpc_snapshot_image_read(softpc_snapshot_image *image,
    lib_u32 expected_memory_bytes, softpc_snapshot_bytes_read read,
    void *context)
{
    softpc_snapshot_section_reader core = {0}, devices = {0};
    softpc_snapshot_image decoded = {0};
    lib_u32 magic, version, sections, memory_bytes, identifier, halted, trap;
    lib_u64 length;
    lib_status status;
    if (image == NULL || expected_memory_bytes == 0u || read == NULL)
        return LIB_STATUS_INVALID_ARGUMENT;
    status = softpc_snapshot_stream_read_u32(read, context, &magic);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_read_u32(read, context, &version);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_read_u32(read, context, &sections);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_read_u32(read, context, &memory_bytes);
    if (status != LIB_STATUS_OK) return status;
    if (magic != SOFTPC_SNAPSHOT_IMAGE_MAGIC || version != SOFTPC_SNAPSHOT_IMAGE_VERSION ||
        sections != SOFTPC_SNAPSHOT_SECTION_COUNT ||
        memory_bytes != expected_memory_bytes) {
        return LIB_STATUS_INVALID_ARGUMENT;
    }
    status = softpc_snapshot_stream_read_u32(read, context, &identifier);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_read_u64(read, context, &length);
    if (status != LIB_STATUS_OK || identifier != SOFTPC_SNAPSHOT_SECTION_CORE || length == 0u)
        return status == LIB_STATUS_OK ? LIB_STATUS_INVALID_ARGUMENT : status;
    core = (softpc_snapshot_section_reader) { read, context, length };
    status = softpc_ccpu_archive_read_core(&decoded.ccpu, memory_bytes,
        softpc_snapshot_section_read, &core);
    if (status == LIB_STATUS_OK && core.remaining != 0u)
        status = LIB_STATUS_INVALID_ARGUMENT;
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_read_u32(read, context, &identifier);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_read_u64(read, context, &length);
    if (status != LIB_STATUS_OK || identifier != SOFTPC_SNAPSHOT_SECTION_DEVICES || length == 0u) {
        status = status == LIB_STATUS_OK ? LIB_STATUS_INVALID_ARGUMENT : status;
        goto failed;
    }
    devices = (softpc_snapshot_section_reader) { read, context, length };
    if (status == LIB_STATUS_OK) status = softpc_device_archive_read(&decoded.ccpu.devices,
        softpc_snapshot_section_read, &devices);
    if (status == LIB_STATUS_OK && devices.remaining != 0u)
        status = LIB_STATUS_INVALID_ARGUMENT;
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_read_u32(read, context, &halted);
    if (status == LIB_STATUS_OK) status = softpc_snapshot_stream_read_u32(read, context, &trap);
    if (status == LIB_STATUS_OK && (halted > 1u || trap > 1u || (halted == 0u && trap != 0u))) status = LIB_STATUS_INVALID_ARGUMENT;
    if (status != LIB_STATUS_OK) {
failed:
        softpc_snapshot_image_dispose(&decoded);
        return status;
    }
    decoded.ccpu.valid = 1;
    decoded.entry.halted = (int)halted; decoded.entry.trap = (unsigned long)trap;
    softpc_snapshot_image_dispose(image); *image = decoded;
    return LIB_STATUS_OK;
}
