#ifndef SOFTPC_SNAPSHOT_STREAM_H
#define SOFTPC_SNAPSHOT_STREAM_H

#include "../lib/types/types_interface.h"

typedef lib_status (*softpc_snapshot_bytes_write)(void *context,
    const lib_u8 *bytes, lib_size byte_count);
typedef lib_status (*softpc_snapshot_bytes_read)(void *context,
    lib_u8 *bytes, lib_size byte_count);

lib_status softpc_snapshot_stream_write_bytes(softpc_snapshot_bytes_write write,
    void *context, const lib_u8 *bytes, lib_size byte_count);
lib_status softpc_snapshot_stream_read_bytes(softpc_snapshot_bytes_read read,
    void *context, lib_u8 *bytes, lib_size byte_count);
lib_status softpc_snapshot_stream_write_u16(softpc_snapshot_bytes_write write,
    void *context, uint16_t value);
lib_status softpc_snapshot_stream_write_u32(softpc_snapshot_bytes_write write,
    void *context, uint32_t value);
lib_status softpc_snapshot_stream_write_u64(softpc_snapshot_bytes_write write,
    void *context, uint64_t value);
lib_status softpc_snapshot_stream_read_u16(softpc_snapshot_bytes_read read,
    void *context, uint16_t *out_value);
lib_status softpc_snapshot_stream_read_u32(softpc_snapshot_bytes_read read,
    void *context, uint32_t *out_value);
lib_status softpc_snapshot_stream_read_u64(softpc_snapshot_bytes_read read,
    void *context, uint64_t *out_value);
lib_status softpc_snapshot_stream_write_u32_array(
    softpc_snapshot_bytes_write write, void *context, const uint32_t *values,
    lib_size count);
lib_status softpc_snapshot_stream_read_u32_array(
    softpc_snapshot_bytes_read read, void *context, uint32_t *values,
    lib_size count);

#endif
