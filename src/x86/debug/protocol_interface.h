#ifndef x86_debug_PROTOCOL_INTERFACE_H
#define x86_debug_PROTOCOL_INTERFACE_H

#include "lib/types/types_interface.h"

/* Copied in-process x86 values shared by frontend and CPU adapters.
 * Not a serialized format and not a dependency on the command parser. */
#define x86_debug_BYTES 32u

typedef enum x86_debug_operation {
    x86_debug_READ_REGISTER,
    x86_debug_WRITE_REGISTER,
    x86_debug_READ_LINEAR,
    x86_debug_WRITE_LINEAR,
    x86_debug_READ_REAL,
    x86_debug_WRITE_REAL,
    x86_debug_READ_PORT,
    x86_debug_WRITE_PORT,
    x86_debug_GET_CODE_DEFAULT_SIZE,
    x86_debug_GET_CODE_BASE,
    x86_debug_GET_CPU_SNAPSHOT,
    x86_debug_SET_WATCH,
    x86_debug_CLEAR_WATCH,
    x86_debug_GET_WATCH,
    x86_debug_SET_EXECUTION_PLAN,
    x86_debug_CLEAR_EXECUTION_PLAN,
    x86_debug_GET_EXECUTION_RESULT
} x86_debug_operation;

typedef enum x86_debug_watch_kind {
    x86_debug_WATCH_READ,
    x86_debug_WATCH_WRITE,
    x86_debug_WATCH_EXECUTE
} x86_debug_watch_kind;

typedef enum x86_debug_execution_plan_kind {
    x86_debug_EXECUTION_NONE,
    x86_debug_EXECUTION_TRACE,
    x86_debug_EXECUTION_BREAK_REAL,
    x86_debug_EXECUTION_BREAK_LINEAR
} x86_debug_execution_plan_kind;

typedef struct x86_debug_segment_snapshot {
    lib_u16 selector;
    lib_u32 base;
    lib_u32 limit;
    lib_u8 dpl;
    lib_u8 type;
    lib_bool accessed;
    lib_bool executable;
    lib_bool conform;
    lib_bool readable;
    lib_bool defsize;
    lib_bool big;
    lib_bool expdown;
    lib_bool writable;
} x86_debug_segment_snapshot;

typedef struct x86_debug_cpu_snapshot {
    x86_debug_segment_snapshot es;
    x86_debug_segment_snapshot cs;
    x86_debug_segment_snapshot ss;
    x86_debug_segment_snapshot ds;
    x86_debug_segment_snapshot fs;
    x86_debug_segment_snapshot gs;
    x86_debug_segment_snapshot tr;
    x86_debug_segment_snapshot ldtr;
    x86_debug_segment_snapshot gdtr;
    x86_debug_segment_snapshot idtr;
    lib_u32 cr0;
    lib_u32 cr2;
    lib_u32 cr3;
} x86_debug_cpu_snapshot;

typedef struct x86_debug_request {
    x86_debug_operation operation;
    lib_u32 register_id;
    lib_u32 address;
    lib_u16 segment;
    lib_u16 offset;
    lib_u16 port;
    x86_debug_watch_kind watch_kind;
    x86_debug_execution_plan_kind execution_kind;
    lib_u64 instruction_count;
    lib_u8 bytes; /* Memory payload size; port I/O explicitly requires 1 (byte). */
    lib_u8 data[x86_debug_BYTES];
} x86_debug_request;

#define x86_debug_ACCESS_CAPACITY 32u
typedef struct x86_debug_memory_access {
    lib_bool write;
    lib_u32 linear;
    lib_u32 bytes;
    lib_u64 data; /* Lowest-addressed up to eight bytes, little endian. */
} x86_debug_memory_access;

typedef struct x86_debug_observation {
    x86_debug_memory_access accesses[x86_debug_ACCESS_CAPACITY];
    lib_u8 count;
    lib_bool truncated;
    lib_bool watch_hit;
    x86_debug_watch_kind watch_kind;
    lib_u32 watch_address;
} x86_debug_observation;

typedef struct x86_debug_response {
    lib_u32 value;
    lib_bool enabled;
    lib_u8 bytes;
    lib_u8 data[x86_debug_BYTES];
    x86_debug_cpu_snapshot cpu;
    x86_debug_observation observation;
} x86_debug_response;

typedef enum x86_debug_register {
    x86_debug_EAX, x86_debug_ECX, x86_debug_EDX, x86_debug_EBX,
    x86_debug_ESP, x86_debug_EBP, x86_debug_ESI, x86_debug_EDI,
    x86_debug_EIP, x86_debug_EFLAGS, x86_debug_ES, x86_debug_CS,
    x86_debug_SS, x86_debug_DS, x86_debug_FS, x86_debug_GS,
    x86_debug_CR0, x86_debug_CR1, x86_debug_CR2, x86_debug_CR3,
    x86_debug_CR4, x86_debug_REGISTER_COUNT
} x86_debug_register;

#endif
