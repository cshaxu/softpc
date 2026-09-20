#ifndef COMMON_X86_DEBUG_PROTOCOL_INTERFACE_H
#define COMMON_X86_DEBUG_PROTOCOL_INTERFACE_H

#include "lib/types/types_interface.h"

/* Copied in-process x86 values shared by frontend and CPU adapters.
 * Not a serialized format and not a dependency on the command parser. */
#define COMMON_X86_DEBUG_BYTES 32u

typedef enum common_x86_debug_operation {
    COMMON_X86_DEBUG_READ_REGISTER,
    COMMON_X86_DEBUG_WRITE_REGISTER,
    COMMON_X86_DEBUG_READ_LINEAR,
    COMMON_X86_DEBUG_WRITE_LINEAR,
    COMMON_X86_DEBUG_READ_REAL,
    COMMON_X86_DEBUG_WRITE_REAL,
    COMMON_X86_DEBUG_READ_PORT,
    COMMON_X86_DEBUG_WRITE_PORT,
    COMMON_X86_DEBUG_GET_CODE_DEFAULT_SIZE,
    COMMON_X86_DEBUG_GET_CODE_BASE,
    COMMON_X86_DEBUG_GET_CPU_SNAPSHOT,
    COMMON_X86_DEBUG_SET_WATCH,
    COMMON_X86_DEBUG_CLEAR_WATCH,
    COMMON_X86_DEBUG_GET_WATCH,
    COMMON_X86_DEBUG_SET_EXECUTION_PLAN,
    COMMON_X86_DEBUG_CLEAR_EXECUTION_PLAN,
    COMMON_X86_DEBUG_GET_EXECUTION_RESULT
} common_x86_debug_operation;

typedef enum common_x86_debug_watch_kind {
    COMMON_X86_DEBUG_WATCH_READ,
    COMMON_X86_DEBUG_WATCH_WRITE,
    COMMON_X86_DEBUG_WATCH_EXECUTE
} common_x86_debug_watch_kind;

typedef enum common_x86_debug_execution_plan_kind {
    COMMON_X86_DEBUG_EXECUTION_NONE,
    COMMON_X86_DEBUG_EXECUTION_TRACE,
    COMMON_X86_DEBUG_EXECUTION_BREAK_REAL,
    COMMON_X86_DEBUG_EXECUTION_BREAK_LINEAR
} common_x86_debug_execution_plan_kind;

typedef struct common_x86_debug_segment_snapshot {
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
} common_x86_debug_segment_snapshot;

typedef struct common_x86_debug_cpu_snapshot {
    common_x86_debug_segment_snapshot es;
    common_x86_debug_segment_snapshot cs;
    common_x86_debug_segment_snapshot ss;
    common_x86_debug_segment_snapshot ds;
    common_x86_debug_segment_snapshot fs;
    common_x86_debug_segment_snapshot gs;
    common_x86_debug_segment_snapshot tr;
    common_x86_debug_segment_snapshot ldtr;
    common_x86_debug_segment_snapshot gdtr;
    common_x86_debug_segment_snapshot idtr;
    lib_u32 cr0;
    lib_u32 cr2;
    lib_u32 cr3;
} common_x86_debug_cpu_snapshot;

typedef struct common_x86_debug_request {
    common_x86_debug_operation operation;
    lib_u32 register_id;
    lib_u32 address;
    lib_u16 segment;
    lib_u16 offset;
    lib_u16 port;
    common_x86_debug_watch_kind watch_kind;
    common_x86_debug_execution_plan_kind execution_kind;
    lib_u64 instruction_count;
    lib_u8 bytes; /* Memory payload size; port I/O explicitly requires 1 (byte). */
    lib_u8 data[COMMON_X86_DEBUG_BYTES];
} common_x86_debug_request;

#define COMMON_X86_DEBUG_ACCESS_CAPACITY 32u
typedef struct common_x86_debug_memory_access {
    lib_bool write;
    lib_u32 linear;
    lib_u32 bytes;
    lib_u64 data; /* Lowest-addressed up to eight bytes, little endian. */
} common_x86_debug_memory_access;

typedef struct common_x86_debug_observation {
    common_x86_debug_memory_access accesses[COMMON_X86_DEBUG_ACCESS_CAPACITY];
    lib_u8 count;
    lib_bool truncated;
    lib_bool watch_hit;
    common_x86_debug_watch_kind watch_kind;
    lib_u32 watch_address;
} common_x86_debug_observation;

typedef struct common_x86_debug_response {
    lib_u32 value;
    lib_bool enabled;
    lib_u8 bytes;
    lib_u8 data[COMMON_X86_DEBUG_BYTES];
    common_x86_debug_cpu_snapshot cpu;
    common_x86_debug_observation observation;
} common_x86_debug_response;

typedef enum common_x86_debug_register {
    COMMON_X86_DEBUG_EAX, COMMON_X86_DEBUG_ECX, COMMON_X86_DEBUG_EDX, COMMON_X86_DEBUG_EBX,
    COMMON_X86_DEBUG_ESP, COMMON_X86_DEBUG_EBP, COMMON_X86_DEBUG_ESI, COMMON_X86_DEBUG_EDI,
    COMMON_X86_DEBUG_EIP, COMMON_X86_DEBUG_EFLAGS, COMMON_X86_DEBUG_ES, COMMON_X86_DEBUG_CS,
    COMMON_X86_DEBUG_SS, COMMON_X86_DEBUG_DS, COMMON_X86_DEBUG_FS, COMMON_X86_DEBUG_GS,
    COMMON_X86_DEBUG_CR0, COMMON_X86_DEBUG_CR1, COMMON_X86_DEBUG_CR2, COMMON_X86_DEBUG_CR3,
    COMMON_X86_DEBUG_CR4, COMMON_X86_DEBUG_REGISTER_COUNT
} common_x86_debug_register;

#endif
