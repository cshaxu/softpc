#ifndef SOFTPC_CCPU_SNAPSHOT_H
#define SOFTPC_CCPU_SNAPSHOT_H

#include <stdint.h>

/*
 * This is the narrow standalone port ABI for copied CCPU state.  It is not
 * the on-disk snapshot format: VM owns that later encoding.  No field here
 * may be a host pointer, function address, native handle, or C stack value.
 */

enum {
    SOFTPC_CCPU_SEGMENT_COUNT = 6,
    SOFTPC_CCPU_DEBUG_REGISTER_COUNT = 8,
    SOFTPC_CCPU_TEST_REGISTER_COUNT = 8,
    SOFTPC_CCPU_TLB_SET_COUNT = 4,
    SOFTPC_CCPU_TLB_ENTRY_COUNT = 8,
    SOFTPC_CCPU_FAST_TLB_PAGE_COUNT = 1048576,
    SOFTPC_CCPU_FAST_TLB_ADDRESS_COUNT = 128,
    SOFTPC_CCPU_FPU_STACK_COUNT = 8
};

typedef struct softpc_ccpu_segment_state {
    uint32_t base;
    uint32_t limit;
    uint16_t selector;
    uint16_t access_rights;
} softpc_ccpu_segment_state;

typedef struct softpc_ccpu_register_state {
    uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
    uint32_t eip, eflags;
    uint32_t cr0, cr2, cr3, cpl;
    uint32_t gdt_base, idt_base, ldt_base, tr_base;
    uint32_t ldt_limit, tr_limit;
    uint16_t gdt_limit, idt_limit, ldt_selector, tr_selector, tr_access_rights;
    softpc_ccpu_segment_state segments[SOFTPC_CCPU_SEGMENT_COUNT];
} softpc_ccpu_register_state;

typedef struct softpc_ccpu_execution_state {
    uint32_t debug_registers[SOFTPC_CCPU_DEBUG_REGISTER_COUNT];
    uint32_t test_registers[SOFTPC_CCPU_TEST_REGISTER_COUNT];
    uint32_t interrupt_map;
    uint32_t start_trap;
} softpc_ccpu_execution_state;

typedef struct softpc_ccpu_instruction_break_state {
    uint32_t linear_address;
    uint32_t register_index;
} softpc_ccpu_instruction_break_state;

typedef struct softpc_ccpu_data_break_state {
    uint32_t start_address;
    uint32_t end_address;
    uint32_t type;
    uint32_t register_index;
} softpc_ccpu_data_break_state;

typedef struct softpc_ccpu_debug_state {
    uint32_t instruction_break_count;
    uint32_t data_break_count;
    softpc_ccpu_instruction_break_state instruction_breaks[4];
    softpc_ccpu_data_break_state data_breaks[4];
} softpc_ccpu_debug_state;

typedef struct softpc_ccpu_tlb_entry_state {
    uint32_t linear_page;
    uint32_t physical_page;
    uint32_t mode;
    uint32_t valid;
    uint32_t dirty;
} softpc_ccpu_tlb_entry_state;

typedef struct softpc_ccpu_tlb_state {
    softpc_ccpu_tlb_entry_state entries[SOFTPC_CCPU_TLB_SET_COUNT]
        [SOFTPC_CCPU_TLB_ENTRY_COUNT];
    uint32_t next_set[SOFTPC_CCPU_TLB_ENTRY_COUNT];
    uint32_t page_address[SOFTPC_CCPU_FAST_TLB_ADDRESS_COUNT];
} softpc_ccpu_tlb_state;

typedef struct softpc_ccpu_fpu_stack_state {
    uint64_t value_bits;
    uint32_t tag;
} softpc_ccpu_fpu_stack_state;

typedef struct softpc_ccpu_fpu_state {
    uint32_t control, status;
    uint32_t last_selector, last_offset, fea, fds, fip, fop, fcs;
    uint32_t pop_stack, double_pop, unordered, reverse;
    uint32_t address_size_32, protected_mode, exception_pending;
    uint32_t delayed_interrupt, delayed_exception_eip;
    softpc_ccpu_fpu_stack_state stack[SOFTPC_CCPU_FPU_STACK_COUNT];
} softpc_ccpu_fpu_state;

typedef struct softpc_ccpu_sas_state {
    uint32_t memory_bytes;
    uint32_t page_type_bytes;
    uint32_t wrap_mask;
    uint32_t selectors_set, code_selector, data_selector;
} softpc_ccpu_sas_state;

void softpc_ccpu_snapshot_capture_registers(softpc_ccpu_register_state *state);
void softpc_ccpu_snapshot_restore_registers(
    const softpc_ccpu_register_state *state);
void softpc_ccpu_snapshot_capture_execution(softpc_ccpu_execution_state *state);
void softpc_ccpu_snapshot_restore_execution(
    const softpc_ccpu_execution_state *state);
void softpc_ccpu_snapshot_capture_debug(softpc_ccpu_debug_state *state);
int softpc_ccpu_snapshot_restore_debug(const softpc_ccpu_debug_state *state);
void softpc_ccpu_snapshot_capture_tlb(softpc_ccpu_tlb_state *state,
    uint8_t *page_index, uint32_t page_index_bytes);
int softpc_ccpu_snapshot_restore_tlb(const softpc_ccpu_tlb_state *state,
    const uint8_t *page_index, uint32_t page_index_bytes);
void softpc_ccpu_snapshot_capture_fpu(softpc_ccpu_fpu_state *state);
void softpc_ccpu_snapshot_restore_fpu(const softpc_ccpu_fpu_state *state);
void softpc_ccpu_snapshot_capture_sas(softpc_ccpu_sas_state *state,
    uint8_t *memory, uint32_t memory_bytes, uint8_t *page_types,
    uint32_t page_type_bytes);
int softpc_ccpu_snapshot_restore_sas(const softpc_ccpu_sas_state *state,
    const uint8_t *memory, uint32_t memory_bytes, const uint8_t *page_types,
    uint32_t page_type_bytes);

#endif
