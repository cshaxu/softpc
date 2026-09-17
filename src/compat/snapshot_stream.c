#include "snapshot_stream.h"

lib_status softpc_snapshot_stream_write_bytes(softpc_snapshot_bytes_write write,
    void *context, const lib_u8 *bytes, lib_size byte_count)
{
    return write == NULL || (byte_count != 0u && bytes == NULL) ?
        LIB_STATUS_INVALID_ARGUMENT : write(context, bytes, byte_count);
}

lib_status softpc_snapshot_stream_read_bytes(softpc_snapshot_bytes_read read,
    void *context, lib_u8 *bytes, lib_size byte_count)
{
    return read == NULL || (byte_count != 0u && bytes == NULL) ?
        LIB_STATUS_INVALID_ARGUMENT : read(context, bytes, byte_count);
}

lib_status softpc_snapshot_stream_write_u16(softpc_snapshot_bytes_write write,
    void *context, uint16_t value)
{
    const lib_u8 bytes[2] = { (lib_u8)value, (lib_u8)(value >> 8u) };
    return softpc_snapshot_stream_write_bytes(write, context, bytes,
        sizeof(bytes));
}

lib_status softpc_snapshot_stream_write_u32(softpc_snapshot_bytes_write write,
    void *context, uint32_t value)
{
    const lib_u8 bytes[4] = { (lib_u8)value, (lib_u8)(value >> 8u),
        (lib_u8)(value >> 16u), (lib_u8)(value >> 24u) };
    return softpc_snapshot_stream_write_bytes(write, context, bytes,
        sizeof(bytes));
}

lib_status softpc_snapshot_stream_write_u64(softpc_snapshot_bytes_write write,
    void *context, uint64_t value)
{
    lib_u8 bytes[8];
    unsigned index;

    for (index = 0u; index < sizeof(bytes); ++index)
        bytes[index] = (lib_u8)(value >> (index * 8u));
    return softpc_snapshot_stream_write_bytes(write, context, bytes,
        sizeof(bytes));
}

lib_status softpc_snapshot_stream_read_u16(softpc_snapshot_bytes_read read,
    void *context, uint16_t *out_value)
{
    lib_u8 bytes[2];
    lib_status status;

    if (out_value == NULL) return LIB_STATUS_INVALID_ARGUMENT;
    status = softpc_snapshot_stream_read_bytes(read, context, bytes,
        sizeof(bytes));
    if (status == LIB_STATUS_OK)
        *out_value = (uint16_t)bytes[0] | ((uint16_t)bytes[1] << 8u);
    return status;
}

lib_status softpc_snapshot_stream_read_u32(softpc_snapshot_bytes_read read,
    void *context, uint32_t *out_value)
{
    lib_u8 bytes[4];
    lib_status status;

    if (out_value == NULL) return LIB_STATUS_INVALID_ARGUMENT;
    status = softpc_snapshot_stream_read_bytes(read, context, bytes,
        sizeof(bytes));
    if (status == LIB_STATUS_OK)
        *out_value = (uint32_t)bytes[0] | ((uint32_t)bytes[1] << 8u) |
            ((uint32_t)bytes[2] << 16u) | ((uint32_t)bytes[3] << 24u);
    return status;
}

lib_status softpc_snapshot_stream_read_u64(softpc_snapshot_bytes_read read,
    void *context, uint64_t *out_value)
{
    lib_u8 bytes[8];
    uint64_t value = 0u;
    lib_status status;
    unsigned index;

    if (out_value == NULL) return LIB_STATUS_INVALID_ARGUMENT;
    status = softpc_snapshot_stream_read_bytes(read, context, bytes,
        sizeof(bytes));
    if (status != LIB_STATUS_OK) return status;
    for (index = 0u; index < sizeof(bytes); ++index)
        value |= (uint64_t)bytes[index] << (index * 8u);
    *out_value = value;
    return LIB_STATUS_OK;
}

lib_status softpc_snapshot_stream_write_u32_array(
    softpc_snapshot_bytes_write write, void *context, const uint32_t *values,
    lib_size count)
{
    lib_size index;
    lib_status status;
    if (values == NULL && count != 0u) return LIB_STATUS_INVALID_ARGUMENT;
    for (index = 0u; index < count; ++index) {
        status = softpc_snapshot_stream_write_u32(write, context, values[index]);
        if (status != LIB_STATUS_OK) return status;
    }
    return LIB_STATUS_OK;
}

lib_status softpc_snapshot_stream_read_u32_array(
    softpc_snapshot_bytes_read read, void *context, uint32_t *values,
    lib_size count)
{
    lib_size index;
    lib_status status;
    if (values == NULL && count != 0u) return LIB_STATUS_INVALID_ARGUMENT;
    for (index = 0u; index < count; ++index) {
        status = softpc_snapshot_stream_read_u32(read, context, &values[index]);
        if (status != LIB_STATUS_OK) return status;
    }
    return LIB_STATUS_OK;
}
