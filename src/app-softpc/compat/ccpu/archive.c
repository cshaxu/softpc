#include "lib/types/types_interface.h"
#include "archive.h"
#include "../devices/archive.h"


static int softpc_ccpu_archive_allocate(softpc_ccpu_archive *archive,
    lib_u32 memory_bytes, lib_u32 page_type_bytes)
{
    lib_u8 *memory;
    lib_u8 *page_types;
    lib_u8 *tlb_page_index;

    if (archive->memory != NULL && archive->sas.memory_bytes == memory_bytes &&
        archive->sas.page_type_bytes == page_type_bytes)
        return 1;
    memory = lib_allocate(memory_bytes);
    page_types = lib_allocate(page_type_bytes);
    tlb_page_index = lib_allocate(SOFTPC_CCPU_FAST_TLB_PAGE_COUNT);
    if (memory == NULL || page_types == NULL || tlb_page_index == NULL) {
        lib_release(memory);
        lib_release(page_types);
        lib_release(tlb_page_index);
        return 0;
    }
    lib_release(archive->memory);
    lib_release(archive->page_types);
    lib_release(archive->tlb_page_index);
    softpc_device_archive_dispose(archive->devices);
    archive->devices = NULL;
    archive->memory = memory;
    archive->page_types = page_types;
    archive->tlb_page_index = tlb_page_index;
    return 1;
}

void softpc_ccpu_archive_dispose(softpc_ccpu_archive *archive)
{
    if (archive == NULL) return;
    lib_release(archive->memory);
    lib_release(archive->page_types);
    lib_release(archive->tlb_page_index);
    softpc_device_archive_dispose(archive->devices);
    *archive = (softpc_ccpu_archive){0};
}

int softpc_ccpu_archive_capture(softpc_ccpu_archive *archive)
{
    softpc_ccpu_sas_state sas;

    if (archive == NULL) return 0;
    archive->valid = 0;
    lib_memory_set(&sas, 0, sizeof(sas));
    softpc_ccpu_snapshot_capture_sas(&sas, NULL, 0u, NULL, 0u);
    if (sas.memory_bytes == 0u || sas.page_type_bytes == 0u ||
        !softpc_ccpu_archive_allocate(archive, sas.memory_bytes,
            sas.page_type_bytes))
        return 0;
    softpc_ccpu_snapshot_capture_sas(&archive->sas, archive->memory,
        sas.memory_bytes, archive->page_types, sas.page_type_bytes);
    if (archive->sas.memory_bytes != sas.memory_bytes ||
        archive->sas.page_type_bytes != sas.page_type_bytes)
        return 0;
    softpc_ccpu_snapshot_capture_registers(&archive->registers);
    softpc_ccpu_snapshot_capture_execution(&archive->execution);
    softpc_ccpu_snapshot_capture_debug(&archive->debug);
    softpc_ccpu_snapshot_capture_tlb(&archive->tlb, archive->tlb_page_index,
        SOFTPC_CCPU_FAST_TLB_PAGE_COUNT);
    softpc_ccpu_snapshot_capture_fpu(&archive->fpu);
    if (archive->devices == NULL)
        archive->devices = softpc_device_archive_create();
    if (archive->devices == NULL ||
        !softpc_device_archive_capture(archive->devices)) return 0;
    archive->valid = 1;
    return 1;
}

int softpc_ccpu_archive_restore(const softpc_ccpu_archive *archive)
{
    if (archive == NULL || !archive->valid || archive->memory == NULL ||
        archive->page_types == NULL || archive->tlb_page_index == NULL ||
        !softpc_ccpu_snapshot_restore_sas(&archive->sas, archive->memory,
            archive->sas.memory_bytes, archive->page_types,
            archive->sas.page_type_bytes))
        return 0;
    softpc_ccpu_snapshot_restore_registers(&archive->registers);
    softpc_ccpu_snapshot_restore_execution(&archive->execution);
    if (!softpc_ccpu_snapshot_restore_debug(&archive->debug) ||
        !softpc_ccpu_snapshot_restore_tlb(&archive->tlb,
            archive->tlb_page_index, SOFTPC_CCPU_FAST_TLB_PAGE_COUNT))
        return 0;
    softpc_ccpu_snapshot_restore_fpu(&archive->fpu);
    return softpc_device_archive_restore(archive->devices);
}

#define SNAPSHOT_WRITE(value) do { \
    if ((status = softpc_snapshot_stream_write_u32(write, context, \
            (lib_u32)(value))) != LIB_STATUS_OK) return status; \
} while (0)
#define SNAPSHOT_READ(value) do { \
    if ((status = softpc_snapshot_stream_read_u32(read, context, \
            &(value))) != LIB_STATUS_OK) return status; \
} while (0)

static lib_status snapshot_write_registers(const softpc_ccpu_register_state *state,
    softpc_snapshot_bytes_write write, void *context)
{
    lib_status status; unsigned index;
    if (state == NULL) return LIB_STATUS_INVALID_ARGUMENT;
    SNAPSHOT_WRITE(state->eax); SNAPSHOT_WRITE(state->ecx); SNAPSHOT_WRITE(state->edx); SNAPSHOT_WRITE(state->ebx);
    SNAPSHOT_WRITE(state->esp); SNAPSHOT_WRITE(state->ebp); SNAPSHOT_WRITE(state->esi); SNAPSHOT_WRITE(state->edi);
    SNAPSHOT_WRITE(state->eip); SNAPSHOT_WRITE(state->eflags); SNAPSHOT_WRITE(state->cr0); SNAPSHOT_WRITE(state->cr2);
    SNAPSHOT_WRITE(state->cr3); SNAPSHOT_WRITE(state->cpl); SNAPSHOT_WRITE(state->gdt_base); SNAPSHOT_WRITE(state->idt_base);
    SNAPSHOT_WRITE(state->ldt_base); SNAPSHOT_WRITE(state->tr_base); SNAPSHOT_WRITE(state->ldt_limit); SNAPSHOT_WRITE(state->tr_limit);
    if ((status = softpc_snapshot_stream_write_u16(write, context, state->gdt_limit)) != LIB_STATUS_OK) return status;
    if ((status = softpc_snapshot_stream_write_u16(write, context, state->idt_limit)) != LIB_STATUS_OK) return status;
    if ((status = softpc_snapshot_stream_write_u16(write, context, state->ldt_selector)) != LIB_STATUS_OK) return status;
    if ((status = softpc_snapshot_stream_write_u16(write, context, state->tr_selector)) != LIB_STATUS_OK) return status;
    if ((status = softpc_snapshot_stream_write_u16(write, context, state->tr_access_rights)) != LIB_STATUS_OK) return status;
    for (index = 0u; index < SOFTPC_CCPU_SEGMENT_COUNT; ++index) {
        SNAPSHOT_WRITE(state->segments[index].base); SNAPSHOT_WRITE(state->segments[index].limit);
        if ((status = softpc_snapshot_stream_write_u16(write, context, state->segments[index].selector)) != LIB_STATUS_OK) return status;
        if ((status = softpc_snapshot_stream_write_u16(write, context, state->segments[index].access_rights)) != LIB_STATUS_OK) return status;
    }
    return LIB_STATUS_OK;
}

static lib_status snapshot_read_registers(softpc_ccpu_register_state *state,
    softpc_snapshot_bytes_read read, void *context)
{
    lib_status status; unsigned index;
    if (state == NULL) return LIB_STATUS_INVALID_ARGUMENT;
    SNAPSHOT_READ(state->eax); SNAPSHOT_READ(state->ecx); SNAPSHOT_READ(state->edx); SNAPSHOT_READ(state->ebx);
    SNAPSHOT_READ(state->esp); SNAPSHOT_READ(state->ebp); SNAPSHOT_READ(state->esi); SNAPSHOT_READ(state->edi);
    SNAPSHOT_READ(state->eip); SNAPSHOT_READ(state->eflags); SNAPSHOT_READ(state->cr0); SNAPSHOT_READ(state->cr2);
    SNAPSHOT_READ(state->cr3); SNAPSHOT_READ(state->cpl); SNAPSHOT_READ(state->gdt_base); SNAPSHOT_READ(state->idt_base);
    SNAPSHOT_READ(state->ldt_base); SNAPSHOT_READ(state->tr_base); SNAPSHOT_READ(state->ldt_limit); SNAPSHOT_READ(state->tr_limit);
    if ((status = softpc_snapshot_stream_read_u16(read, context, &state->gdt_limit)) != LIB_STATUS_OK) return status;
    if ((status = softpc_snapshot_stream_read_u16(read, context, &state->idt_limit)) != LIB_STATUS_OK) return status;
    if ((status = softpc_snapshot_stream_read_u16(read, context, &state->ldt_selector)) != LIB_STATUS_OK) return status;
    if ((status = softpc_snapshot_stream_read_u16(read, context, &state->tr_selector)) != LIB_STATUS_OK) return status;
    if ((status = softpc_snapshot_stream_read_u16(read, context, &state->tr_access_rights)) != LIB_STATUS_OK) return status;
    for (index = 0u; index < SOFTPC_CCPU_SEGMENT_COUNT; ++index) {
        SNAPSHOT_READ(state->segments[index].base); SNAPSHOT_READ(state->segments[index].limit);
        if ((status = softpc_snapshot_stream_read_u16(read, context, &state->segments[index].selector)) != LIB_STATUS_OK) return status;
        if ((status = softpc_snapshot_stream_read_u16(read, context, &state->segments[index].access_rights)) != LIB_STATUS_OK) return status;
    }
    return LIB_STATUS_OK;
}

static lib_status snapshot_write_execution(const softpc_ccpu_execution_state *state,
    softpc_snapshot_bytes_write write, void *context)
{
    lib_status status;
    if (state == NULL) return LIB_STATUS_INVALID_ARGUMENT;
    status = softpc_snapshot_stream_write_u32_array(write, context,
        state->debug_registers, SOFTPC_CCPU_DEBUG_REGISTER_COUNT);
    if (status != LIB_STATUS_OK) return status;
    status = softpc_snapshot_stream_write_u32_array(write, context,
        state->test_registers, SOFTPC_CCPU_TEST_REGISTER_COUNT);
    if (status != LIB_STATUS_OK) return status;
    SNAPSHOT_WRITE(state->interrupt_map); SNAPSHOT_WRITE(state->start_trap);
    return LIB_STATUS_OK;
}

static lib_status snapshot_read_execution(softpc_ccpu_execution_state *state,
    softpc_snapshot_bytes_read read, void *context)
{
    lib_status status;
    if (state == NULL) return LIB_STATUS_INVALID_ARGUMENT;
    status = softpc_snapshot_stream_read_u32_array(read, context,
        state->debug_registers, SOFTPC_CCPU_DEBUG_REGISTER_COUNT);
    if (status != LIB_STATUS_OK) return status;
    status = softpc_snapshot_stream_read_u32_array(read, context,
        state->test_registers, SOFTPC_CCPU_TEST_REGISTER_COUNT);
    if (status != LIB_STATUS_OK) return status;
    SNAPSHOT_READ(state->interrupt_map); SNAPSHOT_READ(state->start_trap);
    return LIB_STATUS_OK;
}

static lib_status snapshot_write_debug(const softpc_ccpu_debug_state *state,
    softpc_snapshot_bytes_write write, void *context)
{
    lib_status status; unsigned index;
    if (state == NULL) return LIB_STATUS_INVALID_ARGUMENT;
    SNAPSHOT_WRITE(state->instruction_break_count); SNAPSHOT_WRITE(state->data_break_count);
    for (index = 0u; index < 4u; ++index) {
        SNAPSHOT_WRITE(state->instruction_breaks[index].linear_address);
        SNAPSHOT_WRITE(state->instruction_breaks[index].register_index);
    }
    for (index = 0u; index < 4u; ++index) {
        SNAPSHOT_WRITE(state->data_breaks[index].start_address);
        SNAPSHOT_WRITE(state->data_breaks[index].end_address);
        SNAPSHOT_WRITE(state->data_breaks[index].type);
        SNAPSHOT_WRITE(state->data_breaks[index].register_index);
    }
    return LIB_STATUS_OK;
}

static lib_status snapshot_read_debug(softpc_ccpu_debug_state *state,
    softpc_snapshot_bytes_read read, void *context)
{
    lib_status status; unsigned index;
    if (state == NULL) return LIB_STATUS_INVALID_ARGUMENT;
    SNAPSHOT_READ(state->instruction_break_count); SNAPSHOT_READ(state->data_break_count);
    if (state->instruction_break_count > 4u || state->data_break_count > 4u)
        return LIB_STATUS_INVALID_ARGUMENT;
    for (index = 0u; index < 4u; ++index) {
        SNAPSHOT_READ(state->instruction_breaks[index].linear_address);
        SNAPSHOT_READ(state->instruction_breaks[index].register_index);
    }
    for (index = 0u; index < 4u; ++index) {
        SNAPSHOT_READ(state->data_breaks[index].start_address);
        SNAPSHOT_READ(state->data_breaks[index].end_address);
        SNAPSHOT_READ(state->data_breaks[index].type);
        SNAPSHOT_READ(state->data_breaks[index].register_index);
    }
    return LIB_STATUS_OK;
}

static lib_status snapshot_write_tlb(const softpc_ccpu_tlb_state *state,
    softpc_snapshot_bytes_write write, void *context)
{
    lib_status status; unsigned set, entry;
    if (state == NULL) return LIB_STATUS_INVALID_ARGUMENT;
    for (set = 0u; set < SOFTPC_CCPU_TLB_SET_COUNT; ++set)
        for (entry = 0u; entry < SOFTPC_CCPU_TLB_ENTRY_COUNT; ++entry) {
            const softpc_ccpu_tlb_entry_state *item = &state->entries[set][entry];
            SNAPSHOT_WRITE(item->linear_page); SNAPSHOT_WRITE(item->physical_page);
            SNAPSHOT_WRITE(item->mode); SNAPSHOT_WRITE(item->valid); SNAPSHOT_WRITE(item->dirty);
        }
    status = softpc_snapshot_stream_write_u32_array(write, context,
        state->next_set, SOFTPC_CCPU_TLB_ENTRY_COUNT);
    if (status != LIB_STATUS_OK) return status;
    return softpc_snapshot_stream_write_u32_array(write, context,
        state->page_address, SOFTPC_CCPU_FAST_TLB_ADDRESS_COUNT);
}

static lib_status snapshot_read_tlb(softpc_ccpu_tlb_state *state,
    softpc_snapshot_bytes_read read, void *context)
{
    lib_status status; unsigned set, entry;
    if (state == NULL) return LIB_STATUS_INVALID_ARGUMENT;
    for (set = 0u; set < SOFTPC_CCPU_TLB_SET_COUNT; ++set)
        for (entry = 0u; entry < SOFTPC_CCPU_TLB_ENTRY_COUNT; ++entry) {
            softpc_ccpu_tlb_entry_state *item = &state->entries[set][entry];
            SNAPSHOT_READ(item->linear_page); SNAPSHOT_READ(item->physical_page);
            SNAPSHOT_READ(item->mode); SNAPSHOT_READ(item->valid); SNAPSHOT_READ(item->dirty);
            if (item->valid > 1u || item->dirty > 1u) return LIB_STATUS_INVALID_ARGUMENT;
        }
    status = softpc_snapshot_stream_read_u32_array(read, context,
        state->next_set, SOFTPC_CCPU_TLB_ENTRY_COUNT);
    if (status != LIB_STATUS_OK) return status;
    return softpc_snapshot_stream_read_u32_array(read, context,
        state->page_address, SOFTPC_CCPU_FAST_TLB_ADDRESS_COUNT);
}

static lib_status snapshot_write_fpu(const softpc_ccpu_fpu_state *state,
    softpc_snapshot_bytes_write write, void *context)
{
    lib_status status; unsigned index;
    if (state == NULL) return LIB_STATUS_INVALID_ARGUMENT;
    SNAPSHOT_WRITE(state->control); SNAPSHOT_WRITE(state->status);
    SNAPSHOT_WRITE(state->last_selector); SNAPSHOT_WRITE(state->last_offset);
    SNAPSHOT_WRITE(state->fea); SNAPSHOT_WRITE(state->fds); SNAPSHOT_WRITE(state->fip);
    SNAPSHOT_WRITE(state->fop); SNAPSHOT_WRITE(state->fcs); SNAPSHOT_WRITE(state->pop_stack);
    SNAPSHOT_WRITE(state->double_pop); SNAPSHOT_WRITE(state->unordered); SNAPSHOT_WRITE(state->reverse);
    SNAPSHOT_WRITE(state->address_size_32); SNAPSHOT_WRITE(state->protected_mode);
    SNAPSHOT_WRITE(state->exception_pending); SNAPSHOT_WRITE(state->delayed_interrupt);
    SNAPSHOT_WRITE(state->delayed_exception_eip);
    for (index = 0u; index < SOFTPC_CCPU_FPU_STACK_COUNT; ++index) {
        if ((status = softpc_snapshot_stream_write_u64(write, context, state->stack[index].value_bits)) != LIB_STATUS_OK) return status;
        SNAPSHOT_WRITE(state->stack[index].tag);
    }
    return LIB_STATUS_OK;
}

static lib_status snapshot_read_fpu(softpc_ccpu_fpu_state *state,
    softpc_snapshot_bytes_read read, void *context)
{
    lib_status status; unsigned index;
    if (state == NULL) return LIB_STATUS_INVALID_ARGUMENT;
    SNAPSHOT_READ(state->control); SNAPSHOT_READ(state->status);
    SNAPSHOT_READ(state->last_selector); SNAPSHOT_READ(state->last_offset);
    SNAPSHOT_READ(state->fea); SNAPSHOT_READ(state->fds); SNAPSHOT_READ(state->fip);
    SNAPSHOT_READ(state->fop); SNAPSHOT_READ(state->fcs); SNAPSHOT_READ(state->pop_stack);
    SNAPSHOT_READ(state->double_pop); SNAPSHOT_READ(state->unordered); SNAPSHOT_READ(state->reverse);
    SNAPSHOT_READ(state->address_size_32); SNAPSHOT_READ(state->protected_mode);
    SNAPSHOT_READ(state->exception_pending); SNAPSHOT_READ(state->delayed_interrupt);
    SNAPSHOT_READ(state->delayed_exception_eip);
    for (index = 0u; index < SOFTPC_CCPU_FPU_STACK_COUNT; ++index) {
        if ((status = softpc_snapshot_stream_read_u64(read, context, &state->stack[index].value_bits)) != LIB_STATUS_OK) return status;
        SNAPSHOT_READ(state->stack[index].tag);
    }
    return LIB_STATUS_OK;
}

static lib_status snapshot_write_sas(const softpc_ccpu_archive *archive,
    softpc_snapshot_bytes_write write, void *context)
{
    lib_status status;
    if (archive == NULL || archive->memory == NULL || archive->page_types == NULL ||
        archive->tlb_page_index == NULL) return LIB_STATUS_INVALID_ARGUMENT;
    SNAPSHOT_WRITE(archive->sas.memory_bytes); SNAPSHOT_WRITE(archive->sas.page_type_bytes);
    SNAPSHOT_WRITE(archive->sas.wrap_mask); SNAPSHOT_WRITE(archive->sas.selectors_set);
    SNAPSHOT_WRITE(archive->sas.code_selector); SNAPSHOT_WRITE(archive->sas.data_selector);
    status = softpc_snapshot_stream_write_bytes(write, context, archive->memory, archive->sas.memory_bytes);
    if (status != LIB_STATUS_OK) return status;
    status = softpc_snapshot_stream_write_bytes(write, context, archive->page_types, archive->sas.page_type_bytes);
    if (status != LIB_STATUS_OK) return status;
    return softpc_snapshot_stream_write_bytes(write, context, archive->tlb_page_index,
        SOFTPC_CCPU_FAST_TLB_PAGE_COUNT);
}

static lib_status snapshot_read_sas(softpc_ccpu_archive *archive,
    lib_u32 expected_memory_bytes, softpc_snapshot_bytes_read read,
    void *context)
{
    softpc_ccpu_sas_state sas = {0};
    lib_status status;
    SNAPSHOT_READ(sas.memory_bytes); SNAPSHOT_READ(sas.page_type_bytes);
    SNAPSHOT_READ(sas.wrap_mask); SNAPSHOT_READ(sas.selectors_set);
    SNAPSHOT_READ(sas.code_selector); SNAPSHOT_READ(sas.data_selector);
    /* The decoder can run before the target CCPU has been reset, so it must
       validate against the image's already checked machine configuration,
       never against the uninitialized live SAS globals.  This is the same
       page-type length the original SAS implementation derives. */
    if (expected_memory_bytes > UINT32_MAX - 0x10020u ||
        sas.memory_bytes == 0u || sas.page_type_bytes == 0u ||
        sas.memory_bytes != expected_memory_bytes ||
        sas.page_type_bytes !=
            ((expected_memory_bytes + 0x10020u) >> 12))
        return LIB_STATUS_INVALID_ARGUMENT;
    if (!softpc_ccpu_archive_allocate(archive, sas.memory_bytes,
        sas.page_type_bytes)) return LIB_STATUS_NO_MEMORY;
    archive->sas = sas;
    status = softpc_snapshot_stream_read_bytes(read, context, archive->memory, sas.memory_bytes);
    if (status != LIB_STATUS_OK) return status;
    status = softpc_snapshot_stream_read_bytes(read, context, archive->page_types, sas.page_type_bytes);
    if (status != LIB_STATUS_OK) return status;
    return softpc_snapshot_stream_read_bytes(read, context, archive->tlb_page_index,
        SOFTPC_CCPU_FAST_TLB_PAGE_COUNT);
}

lib_status softpc_ccpu_archive_write_core(const softpc_ccpu_archive *archive,
    softpc_snapshot_bytes_write write, void *context)
{
    lib_status status;
    if (archive == NULL || !archive->valid || write == NULL) return LIB_STATUS_INVALID_ARGUMENT;
    status = snapshot_write_sas(archive, write, context);
    if (status == LIB_STATUS_OK) status = snapshot_write_registers(&archive->registers, write, context);
    if (status == LIB_STATUS_OK) status = snapshot_write_execution(&archive->execution, write, context);
    if (status == LIB_STATUS_OK) status = snapshot_write_debug(&archive->debug, write, context);
    if (status == LIB_STATUS_OK) status = snapshot_write_tlb(&archive->tlb, write, context);
    if (status == LIB_STATUS_OK) status = snapshot_write_fpu(&archive->fpu, write, context);
    return status;
}

lib_status softpc_ccpu_archive_read_core(softpc_ccpu_archive *archive,
    lib_u32 expected_memory_bytes, softpc_snapshot_bytes_read read,
    void *context)
{
    lib_status status;
    if (archive == NULL || expected_memory_bytes == 0u || read == NULL)
        return LIB_STATUS_INVALID_ARGUMENT;
    archive->valid = 0;
    status = snapshot_read_sas(archive, expected_memory_bytes, read, context);
    if (status == LIB_STATUS_OK) status = snapshot_read_registers(&archive->registers, read, context);
    if (status == LIB_STATUS_OK) status = snapshot_read_execution(&archive->execution, read, context);
    if (status == LIB_STATUS_OK) status = snapshot_read_debug(&archive->debug, read, context);
    if (status == LIB_STATUS_OK) status = snapshot_read_tlb(&archive->tlb, read, context);
    if (status == LIB_STATUS_OK) status = snapshot_read_fpu(&archive->fpu, read, context);
    return status;
}

#undef SNAPSHOT_WRITE
#undef SNAPSHOT_READ
