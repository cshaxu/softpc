#include "archive.h"
#include "snapshot.h"

#include <limits.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "insignia.h"
#include "host_def.h"
#include "quick_ev.h"

/* Legacy CCPU headers erase const for their K&R declarations.  The new
   archive boundary is ordinary ANSI C and keeps its read-only contract. */
#ifdef const
#undef const
#endif

struct softpc_device_archive {
    Q_EVENT_SNAPSHOT_STATE events;
    Q_EVENT_SNAPSHOT_ENTRY *quick_entries;
    Q_EVENT_SNAPSHOT_ENTRY *tick_entries;
    unsigned long quick_capacity;
    unsigned long tick_capacity;
    softpc_device_dma_state dma;
    softpc_device_pic_state pic;
    softpc_device_pit_state pit;
    softpc_device_cmos_state cmos;
    softpc_device_fdc_state fdc;
    softpc_device_hdd_state hdd;
    softpc_device_ppi_state ppi;
    softpc_device_inport_mouse_state inport_mouse;
    softpc_device_keyboard_state keyboard;
    softpc_device_dos_mouse_state dos_mouse;
    softpc_device_video_memory_state video_memory;
    softpc_device_video_controller_state video_controller;
    softpc_device_serial_controller_state serial_controller;
    softpc_device_serial_host_state serial_host;
    softpc_device_parallel_controller_state parallel_controller;
    softpc_device_parallel_host_state parallel_host;
    int valid;
};

extern int softpc_device_snapshot_encode_timer_callback();
extern Q_CALLBACK_FN softpc_device_snapshot_decode_timer_callback();
extern int softpc_device_snapshot_encode_cmos_callback();
extern Q_CALLBACK_FN softpc_device_snapshot_decode_cmos_callback();
extern int softpc_device_snapshot_encode_fla_callback();
extern Q_CALLBACK_FN softpc_device_snapshot_decode_fla_callback();
extern int softpc_device_snapshot_encode_fdisk_callback();
extern Q_CALLBACK_FN softpc_device_snapshot_decode_fdisk_callback();
extern int softpc_device_snapshot_encode_keyboard_callback();
extern Q_CALLBACK_FN softpc_device_snapshot_decode_keyboard_callback();
extern int softpc_device_snapshot_encode_serial_callback();
extern Q_CALLBACK_FN softpc_device_snapshot_decode_serial_callback();
extern int softpc_device_snapshot_encode_parallel_callback();
extern Q_CALLBACK_FN softpc_device_snapshot_decode_parallel_callback();

LOCAL int
encode_callback(callback, callback_id)
Q_CALLBACK_FN callback;
unsigned long *callback_id;
{
    return softpc_device_snapshot_encode_timer_callback(callback, callback_id) ||
        softpc_device_snapshot_encode_cmos_callback(callback, callback_id) ||
        softpc_device_snapshot_encode_fla_callback(callback, callback_id) ||
        softpc_device_snapshot_encode_fdisk_callback(callback, callback_id) ||
        softpc_device_snapshot_encode_keyboard_callback(callback, callback_id) ||
        softpc_device_snapshot_encode_serial_callback(callback, callback_id) ||
        softpc_device_snapshot_encode_parallel_callback(callback, callback_id);
}

softpc_device_archive *
softpc_device_archive_create(void)
{
    return (softpc_device_archive *)calloc(1, sizeof(softpc_device_archive));
}

LOCAL Q_CALLBACK_FN
decode_callback(callback_id)
unsigned long callback_id;
{
    Q_CALLBACK_FN callback;

    callback = softpc_device_snapshot_decode_timer_callback(callback_id);
    if (callback != NULL) return callback;
    callback = softpc_device_snapshot_decode_cmos_callback(callback_id);
    if (callback != NULL) return callback;
    callback = softpc_device_snapshot_decode_fla_callback(callback_id);
    if (callback != NULL) return callback;
    callback = softpc_device_snapshot_decode_fdisk_callback(callback_id);
    if (callback != NULL) return callback;
    callback = softpc_device_snapshot_decode_keyboard_callback(callback_id);
    if (callback != NULL) return callback;
    callback = softpc_device_snapshot_decode_serial_callback(callback_id);
    if (callback != NULL) return callback;
    return softpc_device_snapshot_decode_parallel_callback(callback_id);
}

LOCAL int
resize_entries(entries, capacity, required)
Q_EVENT_SNAPSHOT_ENTRY **entries;
unsigned long *capacity;
unsigned long required;
{
    Q_EVENT_SNAPSHOT_ENTRY *replacement;

    if (required <= *capacity) return TRUE;
    replacement = (Q_EVENT_SNAPSHOT_ENTRY *)realloc(*entries,
        required * sizeof(*replacement));
    if (replacement == NULL) return FALSE;
    *entries = replacement;
    *capacity = required;
    return TRUE;
}

void
softpc_device_archive_dispose(archive)
softpc_device_archive *archive;
{
    if (archive == NULL) return;
    free(archive->quick_entries);
    free(archive->tick_entries);
    free(archive);
}

int
softpc_device_archive_capture(archive)
softpc_device_archive *archive;
{
    Q_EVENT_SNAPSHOT_STATE measured;

    if (archive == NULL) return FALSE;
    archive->valid = FALSE;
    q_event_snapshot_measure(&measured);
    if (!resize_entries(&archive->quick_entries, &archive->quick_capacity,
            measured.quick_entries) ||
        !resize_entries(&archive->tick_entries, &archive->tick_capacity,
            measured.tick_entries) ||
        !q_event_snapshot_capture(&archive->events, archive->quick_entries,
            archive->quick_capacity, archive->tick_entries,
            archive->tick_capacity, encode_callback) ||
        !softpc_device_snapshot_capture_pic(&archive->pic) ||
        !softpc_device_snapshot_capture_pit(&archive->pit))
        return FALSE;
    softpc_device_snapshot_capture_dma(&archive->dma);
    softpc_device_snapshot_capture_cmos(&archive->cmos);
    softpc_device_snapshot_capture_fdc(&archive->fdc);
    softpc_device_snapshot_capture_ppi(&archive->ppi);
    if (!softpc_device_snapshot_capture_inport_mouse(&archive->inport_mouse))
        return FALSE;
    if (!softpc_device_snapshot_capture_keyboard(&archive->keyboard))
        return FALSE;
    if (!softpc_device_snapshot_capture_dos_mouse(&archive->dos_mouse))
        return FALSE;
    if (!softpc_device_snapshot_capture_video_memory(&archive->video_memory))
        return FALSE;
    if (!softpc_device_snapshot_capture_video_controller(
            &archive->video_controller))
        return FALSE;
    if (!softpc_device_snapshot_capture_serial_controller(
            &archive->serial_controller) ||
        !softpc_device_snapshot_capture_serial_host(&archive->serial_host) ||
        !softpc_device_snapshot_capture_parallel_controller(
            &archive->parallel_controller) ||
        !softpc_device_snapshot_capture_parallel_host(&archive->parallel_host))
        return FALSE;
    if (!softpc_device_snapshot_capture_hdd(&archive->hdd))
        return FALSE;
    archive->valid = TRUE;
    return TRUE;
}

int
softpc_device_archive_restore(archive)
softpc_device_archive *archive;
{
    if (archive == NULL || !archive->valid ||
        !softpc_device_snapshot_restore_pic(&archive->pic) ||
        !softpc_device_snapshot_restore_dma(&archive->dma) ||
        !softpc_device_snapshot_restore_pit(&archive->pit) ||
        !softpc_device_snapshot_restore_cmos(&archive->cmos) ||
        !softpc_device_snapshot_restore_fdc(&archive->fdc) ||
        !softpc_device_snapshot_restore_ppi(&archive->ppi) ||
        !softpc_device_snapshot_restore_inport_mouse(&archive->inport_mouse) ||
        !softpc_device_snapshot_restore_keyboard(&archive->keyboard) ||
        !softpc_device_snapshot_restore_video_memory(&archive->video_memory) ||
        !softpc_device_snapshot_restore_video_controller(
            &archive->video_controller) ||
        !softpc_device_snapshot_restore_dos_mouse(&archive->dos_mouse) ||
        !softpc_device_snapshot_restore_hdd(&archive->hdd) ||
        !q_event_snapshot_restore(&archive->events, archive->quick_entries,
            archive->quick_capacity, archive->tick_entries,
            archive->tick_capacity, decode_callback))
        return FALSE;
    if (!softpc_device_snapshot_rebuild_video_presentation()) return FALSE;
    if (!softpc_device_snapshot_restore_serial_controller(
            &archive->serial_controller) ||
        !softpc_device_snapshot_restore_serial_host(&archive->serial_host) ||
        !softpc_device_snapshot_restore_parallel_controller(
            &archive->parallel_controller) ||
        !softpc_device_snapshot_restore_parallel_host(&archive->parallel_host))
        return FALSE;
    return TRUE;
}

/* The fixed snapshot types deliberately name every transferable field.  This
   small table interpreter writes those fields with their declared width; it
   never serializes struct padding or a legacy host representation. */
typedef enum softpc_device_wire_kind {
    SOFTPC_DEVICE_WIRE_U8,
    SOFTPC_DEVICE_WIRE_U16,
    SOFTPC_DEVICE_WIRE_U32,
    SOFTPC_DEVICE_WIRE_U64
} softpc_device_wire_kind;

typedef struct softpc_device_wire_field {
    lib_size offset;
    lib_size count;
    softpc_device_wire_kind kind;
} softpc_device_wire_field;

#define DEVICE_WIRE_FIELD(type, member, field_kind, element_type) \
    { (lib_size)offsetof(type, member), \
      (lib_size)(sizeof(((type *)0)->member) / sizeof(element_type)), field_kind }
#define DEVICE_U8(type, member) \
    DEVICE_WIRE_FIELD(type, member, SOFTPC_DEVICE_WIRE_U8, uint8_t)
#define DEVICE_U16(type, member) \
    DEVICE_WIRE_FIELD(type, member, SOFTPC_DEVICE_WIRE_U16, uint16_t)
#define DEVICE_U32(type, member) \
    DEVICE_WIRE_FIELD(type, member, SOFTPC_DEVICE_WIRE_U32, uint32_t)
#define DEVICE_U64(type, member) \
    DEVICE_WIRE_FIELD(type, member, SOFTPC_DEVICE_WIRE_U64, uint64_t)

static lib_status
device_write_map(const void *object, const softpc_device_wire_field *fields,
    lib_size field_count, softpc_snapshot_bytes_write write, void *context)
{
    const uint8_t *base = (const uint8_t *)object;
    lib_size field_index;

    if (object == NULL || fields == NULL || write == NULL)
        return LIB_STATUS_INVALID_ARGUMENT;
    for (field_index = 0u; field_index < field_count; ++field_index) {
        const softpc_device_wire_field *field = &fields[field_index];
        const uint8_t *bytes = base + field->offset;
        lib_size index;
        lib_status status;

        if (field->kind == SOFTPC_DEVICE_WIRE_U8) {
            status = softpc_snapshot_stream_write_bytes(write, context, bytes,
                field->count);
            if (status != LIB_STATUS_OK) return status;
            continue;
        }
        for (index = 0u; index < field->count; ++index) {
            switch (field->kind) {
            case SOFTPC_DEVICE_WIRE_U16: {
                uint16_t value;
                memcpy(&value, bytes + index * sizeof(value), sizeof(value));
                status = softpc_snapshot_stream_write_u16(write, context, value);
                break;
            }
            case SOFTPC_DEVICE_WIRE_U32: {
                uint32_t value;
                memcpy(&value, bytes + index * sizeof(value), sizeof(value));
                status = softpc_snapshot_stream_write_u32(write, context, value);
                break;
            }
            case SOFTPC_DEVICE_WIRE_U64: {
                uint64_t value;
                memcpy(&value, bytes + index * sizeof(value), sizeof(value));
                status = softpc_snapshot_stream_write_u64(write, context, value);
                break;
            }
            default:
                return LIB_STATUS_INVALID_ARGUMENT;
            }
            if (status != LIB_STATUS_OK) return status;
        }
    }
    return LIB_STATUS_OK;
}

static lib_status
device_read_map(void *object, const softpc_device_wire_field *fields,
    lib_size field_count, softpc_snapshot_bytes_read read, void *context)
{
    uint8_t *base = (uint8_t *)object;
    lib_size field_index;

    if (object == NULL || fields == NULL || read == NULL)
        return LIB_STATUS_INVALID_ARGUMENT;
    for (field_index = 0u; field_index < field_count; ++field_index) {
        const softpc_device_wire_field *field = &fields[field_index];
        uint8_t *bytes = base + field->offset;
        lib_size index;
        lib_status status;

        if (field->kind == SOFTPC_DEVICE_WIRE_U8) {
            status = softpc_snapshot_stream_read_bytes(read, context, bytes,
                field->count);
            if (status != LIB_STATUS_OK) return status;
            continue;
        }
        for (index = 0u; index < field->count; ++index) {
            switch (field->kind) {
            case SOFTPC_DEVICE_WIRE_U16: {
                uint16_t value;
                status = softpc_snapshot_stream_read_u16(read, context, &value);
                memcpy(bytes + index * sizeof(value), &value, sizeof(value));
                break;
            }
            case SOFTPC_DEVICE_WIRE_U32: {
                uint32_t value;
                status = softpc_snapshot_stream_read_u32(read, context, &value);
                memcpy(bytes + index * sizeof(value), &value, sizeof(value));
                break;
            }
            case SOFTPC_DEVICE_WIRE_U64: {
                uint64_t value;
                status = softpc_snapshot_stream_read_u64(read, context, &value);
                memcpy(bytes + index * sizeof(value), &value, sizeof(value));
                break;
            }
            default:
                return LIB_STATUS_INVALID_ARGUMENT;
            }
            if (status != LIB_STATUS_OK) return status;
        }
    }
    return LIB_STATUS_OK;
}

#define DEVICE_FIELD_COUNT(fields) (sizeof(fields) / sizeof((fields)[0]))

static const softpc_device_wire_field dma_fields[] = {
    DEVICE_U8(softpc_device_dma_state, base_address),
    DEVICE_U8(softpc_device_dma_state, base_count),
    DEVICE_U8(softpc_device_dma_state, current_address),
    DEVICE_U8(softpc_device_dma_state, current_count),
    DEVICE_U8(softpc_device_dma_state, temporary_address),
    DEVICE_U8(softpc_device_dma_state, temporary_count),
    DEVICE_U8(softpc_device_dma_state, status),
    DEVICE_U8(softpc_device_dma_state, command),
    DEVICE_U8(softpc_device_dma_state, temporary),
    DEVICE_U8(softpc_device_dma_state, mode),
    DEVICE_U8(softpc_device_dma_state, mask),
    DEVICE_U8(softpc_device_dma_state, request),
    DEVICE_U32(softpc_device_dma_state, first_last),
    DEVICE_U8(softpc_device_dma_state, page)
};

static const softpc_device_wire_field pic_adapter_fields[] = {
    DEVICE_U32(softpc_device_pic_adapter_state, master),
    DEVICE_U8(softpc_device_pic_adapter_state, irr),
    DEVICE_U8(softpc_device_pic_adapter_state, isr),
    DEVICE_U8(softpc_device_pic_adapter_state, imr),
    DEVICE_U8(softpc_device_pic_adapter_state, ssr),
    DEVICE_U16(softpc_device_pic_adapter_state, base),
    DEVICE_U16(softpc_device_pic_adapter_state, hipri),
    DEVICE_U16(softpc_device_pic_adapter_state, mode),
    DEVICE_U32(softpc_device_pic_adapter_state, count),
    DEVICE_U32(softpc_device_pic_adapter_state, interrupt_line),
    DEVICE_U32(softpc_device_pic_adapter_state, cpu_interrupt),
    DEVICE_U32(softpc_device_pic_adapter_state, callback_parameter),
    DEVICE_U32(softpc_device_pic_adapter_state, isr_depth),
    DEVICE_U32(softpc_device_pic_adapter_state, isr_progress),
    DEVICE_U32(softpc_device_pic_adapter_state, isr_time_decay)
};

static const softpc_device_wire_field pic_fields[] = {
    DEVICE_U32(softpc_device_pic_state, iret_hooks_enabled),
    DEVICE_U16(softpc_device_pic_state, iret_hook_mask),
    DEVICE_U16(softpc_device_pic_state, iret_hook_active)
};

static const softpc_device_wire_field cmos_fields[] = {
    DEVICE_U8(softpc_device_cmos_state, bytes),
    DEVICE_U32(softpc_device_cmos_state, selected_index),
    DEVICE_U32(softpc_device_cmos_state, data_mode_yes),
    DEVICE_U32(softpc_device_cmos_state, twenty4_hour_clock),
    DEVICE_U32(softpc_device_cmos_state, reset_alarm),
    DEVICE_U32(softpc_device_cmos_state, rtc_int_enabled),
    DEVICE_U32(softpc_device_cmos_state, cmos_count),
    DEVICE_U32(softpc_device_cmos_state, host_time_valid),
    DEVICE_U32(softpc_device_cmos_state, host_time),
    DEVICE_U64(softpc_device_cmos_state, user_time),
    DEVICE_U32(softpc_device_cmos_state, periodic_milliseconds),
    DEVICE_U64(softpc_device_cmos_state, periodic_event_handle)
};

static const softpc_device_wire_field fdc_fields[] = {
    DEVICE_U8(softpc_device_fdc_state, command),
    DEVICE_U8(softpc_device_fdc_state, result),
    DEVICE_U8(softpc_device_fdc_state, sense),
    DEVICE_U8(softpc_device_fdc_state, ndma),
    DEVICE_U8(softpc_device_fdc_state, status),
    DEVICE_U8(softpc_device_fdc_state, current_command),
    DEVICE_U8(softpc_device_fdc_state, interrupt_line),
    DEVICE_U8(softpc_device_fdc_state, dor),
    DEVICE_U8(softpc_device_fdc_state, drive_selected),
    DEVICE_U8(softpc_device_fdc_state, interrupt_pending),
    DEVICE_U8(softpc_device_fdc_state, busy),
    DEVICE_U8(softpc_device_fdc_state, ndma_enabled),
    DEVICE_U32(softpc_device_fdc_state, command_count),
    DEVICE_U32(softpc_device_fdc_state, result_count),
    DEVICE_U32(softpc_device_fdc_state, ndma_count),
    DEVICE_U32(softpc_device_fdc_state, ndma_sector_size)
};

static const softpc_device_wire_field hdd_drive_fields[] = {
    DEVICE_U32(softpc_device_hdd_drive_state, drive_id),
    DEVICE_U32(softpc_device_hdd_drive_state, max_head),
    DEVICE_U32(softpc_device_hdd_drive_state, max_cylinder),
    DEVICE_U32(softpc_device_hdd_drive_state, max_sector),
    DEVICE_U32(softpc_device_hdd_drive_state, sectors_per_track),
    DEVICE_U32(softpc_device_hdd_drive_state, bytes_per_cylinder),
    DEVICE_U32(softpc_device_hdd_drive_state, bytes_per_track),
    DEVICE_U32(softpc_device_hdd_drive_state, wired_up),
    DEVICE_U32(softpc_device_hdd_drive_state, current_offset)
};

static const softpc_device_wire_field hdd_fields[] = {
    DEVICE_U8(softpc_device_hdd_state, taskfile),
    DEVICE_U16(softpc_device_hdd_state, sector),
    DEVICE_U8(softpc_device_hdd_state, fixed_disk_register),
    DEVICE_U8(softpc_device_hdd_state, digital_input_register),
    DEVICE_U8(softpc_device_hdd_state, sector_index),
    DEVICE_U32(softpc_device_hdd_state, selected_drive),
    DEVICE_U32(softpc_device_hdd_state, active_command)
};

static const softpc_device_wire_field ppi_fields[] = {
    DEVICE_U8(softpc_device_ppi_state, register_value),
    DEVICE_U8(softpc_device_ppi_state, gate_2_was_low),
    DEVICE_U8(softpc_device_ppi_state, speaker_data_was_low)
};

static const softpc_device_wire_field inport_mouse_fields[] = {
    DEVICE_U32(softpc_device_inport_mouse_state, button_left),
    DEVICE_U32(softpc_device_inport_mouse_state, button_right),
    DEVICE_U32(softpc_device_inport_mouse_state, delta_x),
    DEVICE_U32(softpc_device_inport_mouse_state, delta_y),
    DEVICE_U16(softpc_device_inport_mouse_state, data_1),
    DEVICE_U16(softpc_device_inport_mouse_state, data_2),
    DEVICE_U16(softpc_device_inport_mouse_state, status),
    DEVICE_U16(softpc_device_inport_mouse_state, last_button_left),
    DEVICE_U16(softpc_device_inport_mouse_state, last_button_right),
    DEVICE_U16(softpc_device_inport_mouse_state, mode),
    DEVICE_U16(softpc_device_inport_mouse_state, address),
    DEVICE_U16(softpc_device_inport_mouse_state, test_data),
    DEVICE_U32(softpc_device_inport_mouse_state, startup_interrupt_bursts),
    DEVICE_U32(softpc_device_inport_mouse_state, id_toggle),
    DEVICE_U32(softpc_device_inport_mouse_state, test_state)
};

static const softpc_device_wire_field keyboard_fields[] = {
    DEVICE_U16(softpc_device_keyboard_state, fifo),
    DEVICE_U16(softpc_device_keyboard_state, set_3_key_state),
    DEVICE_U32(softpc_device_keyboard_state, key_down_count),
    DEVICE_U32(softpc_device_keyboard_state, held_key),
    DEVICE_U32(softpc_device_keyboard_state, held_type),
    DEVICE_U32(softpc_device_keyboard_state, fifo_count),
    DEVICE_U32(softpc_device_keyboard_state, sent_overrun),
    DEVICE_U32(softpc_device_keyboard_state, anomalous_index),
    DEVICE_U32(softpc_device_keyboard_state, anomalous_size),
    DEVICE_U32(softpc_device_keyboard_state, anomalous_key),
    DEVICE_U32(softpc_device_keyboard_state, anomalous_active),
    DEVICE_U32(softpc_device_keyboard_state, held_count),
    DEVICE_U32(softpc_device_keyboard_state, scan_set),
    DEVICE_U32(softpc_device_keyboard_state, repeat_delay_target),
    DEVICE_U32(softpc_device_keyboard_state, repeat_target),
    DEVICE_U32(softpc_device_keyboard_state, repeat_delay_count),
    DEVICE_U32(softpc_device_keyboard_state, repeat_count),
    DEVICE_U32(softpc_device_keyboard_state, typematic_key),
    DEVICE_U32(softpc_device_keyboard_state, input_port_value),
    DEVICE_U32(softpc_device_keyboard_state, typematic_key_valid),
    DEVICE_U32(softpc_device_keyboard_state, waiting_for_next_code),
    DEVICE_U32(softpc_device_keyboard_state, waiting_for_next_8042_code),
    DEVICE_U32(softpc_device_keyboard_state, num_lock_on),
    DEVICE_U32(softpc_device_keyboard_state, shift_on),
    DEVICE_U32(softpc_device_keyboard_state, left_shift_on),
    DEVICE_U32(softpc_device_keyboard_state, right_shift_on),
    DEVICE_U32(softpc_device_keyboard_state, ctrl_on),
    DEVICE_U32(softpc_device_keyboard_state, left_ctrl_on),
    DEVICE_U32(softpc_device_keyboard_state, right_ctrl_on),
    DEVICE_U32(softpc_device_keyboard_state, alt_on),
    DEVICE_U32(softpc_device_keyboard_state, left_alt_on),
    DEVICE_U32(softpc_device_keyboard_state, right_alt_on),
    DEVICE_U32(softpc_device_keyboard_state, waiting_for_upcode),
    DEVICE_U32(softpc_device_keyboard_state, next_code_sequence),
    DEVICE_U32(softpc_device_keyboard_state, next_8042_sequence),
    DEVICE_U32(softpc_device_keyboard_state, set_3_key_type_change_destination),
    DEVICE_U32(softpc_device_keyboard_state, translating),
    DEVICE_U32(softpc_device_keyboard_state, keyboard_disabled),
    DEVICE_U32(softpc_device_keyboard_state, interrupt_enabled),
    DEVICE_U32(softpc_device_keyboard_state, output_full),
    DEVICE_U32(softpc_device_keyboard_state, pending_8042),
    DEVICE_U32(softpc_device_keyboard_state, interface_disabled),
    DEVICE_U32(softpc_device_keyboard_state, scanning_discontinued),
    DEVICE_U32(softpc_device_keyboard_state, gate_a20_status),
    DEVICE_U32(softpc_device_keyboard_state, reset_was_by_keyboard),
    DEVICE_U16(softpc_device_keyboard_state, output_contents),
    DEVICE_U16(softpc_device_keyboard_state, pending_8042_value),
    DEVICE_U16(softpc_device_keyboard_state, status),
    DEVICE_U16(softpc_device_keyboard_state, output_port_bits),
    DEVICE_U16(softpc_device_keyboard_state, command_byte),
    DEVICE_U16(softpc_device_keyboard_state, light_pattern),
    DEVICE_U32(softpc_device_keyboard_state, refill_event_handle)
};

static const softpc_device_wire_field video_memory_fields[] = {
    DEVICE_U8(softpc_device_video_memory_state, plane),
    DEVICE_U8(softpc_device_video_memory_state, dac)
};

static const softpc_device_wire_field video_controller_fields[] = {
    DEVICE_U8(softpc_device_video_controller_state, sequencer),
    DEVICE_U8(softpc_device_video_controller_state, crtc),
    DEVICE_U8(softpc_device_video_controller_state, graphics),
    DEVICE_U8(softpc_device_video_controller_state, attribute),
    DEVICE_U8(softpc_device_video_controller_state, v7),
    DEVICE_U8(softpc_device_video_controller_state, sequencer_extension_control),
    DEVICE_U8(softpc_device_video_controller_state, miscellaneous_output),
    DEVICE_U8(softpc_device_video_controller_state, feature_control),
    DEVICE_U8(softpc_device_video_controller_state, sequencer_index),
    DEVICE_U8(softpc_device_video_controller_state, crtc_index),
    DEVICE_U8(softpc_device_video_controller_state, graphics_index),
    DEVICE_U8(softpc_device_video_controller_state, attribute_index),
    DEVICE_U8(softpc_device_video_controller_state, dac_mask),
    DEVICE_U8(softpc_device_video_controller_state, dac_read_address),
    DEVICE_U8(softpc_device_video_controller_state, dac_write_address),
    DEVICE_U8(softpc_device_video_controller_state, dac_component),
    DEVICE_U8(softpc_device_video_controller_state, dac_state),
    DEVICE_U8(softpc_device_video_controller_state, currently_emulated_video_mode),
    DEVICE_U32(softpc_device_video_controller_state, cvid_latches),
    DEVICE_U32(softpc_device_video_controller_state, cvid_v7_foreground_latches)
};

static const softpc_device_wire_field serial_port_fields[] = {
    DEVICE_U16(softpc_device_serial_port_state, tx_buffer),
    DEVICE_U16(softpc_device_serial_port_state, rx_buffer),
    DEVICE_U16(softpc_device_serial_port_state, divisor_latch),
    DEVICE_U16(softpc_device_serial_port_state, int_enable),
    DEVICE_U16(softpc_device_serial_port_state, int_id),
    DEVICE_U16(softpc_device_serial_port_state, line_control),
    DEVICE_U16(softpc_device_serial_port_state, modem_control),
    DEVICE_U16(softpc_device_serial_port_state, line_status),
    DEVICE_U16(softpc_device_serial_port_state, modem_status),
    DEVICE_U16(softpc_device_serial_port_state, scratch),
    DEVICE_U32(softpc_device_serial_port_state, break_state),
    DEVICE_U32(softpc_device_serial_port_state, loopback_state),
    DEVICE_U32(softpc_device_serial_port_state, dtr_state),
    DEVICE_U32(softpc_device_serial_port_state, rts_state),
    DEVICE_U32(softpc_device_serial_port_state, out1_state),
    DEVICE_U32(softpc_device_serial_port_state, out2_state),
    DEVICE_U32(softpc_device_serial_port_state, receiver_line_status_interrupt),
    DEVICE_U32(softpc_device_serial_port_state, data_available_interrupt),
    DEVICE_U32(softpc_device_serial_port_state, tx_empty_interrupt),
    DEVICE_U32(softpc_device_serial_port_state, modem_status_interrupt),
    DEVICE_U32(softpc_device_serial_port_state, interrupt_priority),
    DEVICE_U32(softpc_device_serial_port_state, baud_index),
    DEVICE_U32(softpc_device_serial_port_state, had_first_read)
};
static const softpc_device_wire_field serial_controller_fields[] = {
    DEVICE_U32(softpc_device_serial_controller_state, critical),
    DEVICE_U16(softpc_device_serial_controller_state, line_control_flush_mask),
    DEVICE_U32(softpc_device_serial_controller_state, transmit_pacing)
};
static const softpc_device_wire_field serial_host_port_fields[] = {
    DEVICE_U8(softpc_device_serial_host_port_state, rx),
    DEVICE_U32(softpc_device_serial_host_port_state, rx_count),
    DEVICE_U32(softpc_device_serial_host_port_state, tx_count),
    DEVICE_U32(softpc_device_serial_host_port_state, baud),
    DEVICE_U32(softpc_device_serial_host_port_state, data_bits),
    DEVICE_U32(softpc_device_serial_host_port_state, stop_bits),
    DEVICE_U32(softpc_device_serial_host_port_state, parity),
    DEVICE_U32(softpc_device_serial_host_port_state, break_enabled),
    DEVICE_U32(softpc_device_serial_host_port_state, dtr),
    DEVICE_U32(softpc_device_serial_host_port_state, rts),
    DEVICE_U32(softpc_device_serial_host_port_state, opened),
    DEVICE_U32(softpc_device_serial_host_port_state, xon_enabled),
    DEVICE_U32(softpc_device_serial_host_port_state, modem),
    DEVICE_U16(softpc_device_serial_host_port_state, last_msr)
};
static const softpc_device_wire_field parallel_controller_fields[] = {
    DEVICE_U16(softpc_device_parallel_controller_state, output),
    DEVICE_U16(softpc_device_parallel_controller_state, control),
    DEVICE_U16(softpc_device_parallel_controller_state, status),
    DEVICE_U32(softpc_device_parallel_controller_state, state),
    DEVICE_U64(softpc_device_parallel_controller_state, out_event),
    DEVICE_U64(softpc_device_parallel_controller_state, out_ack_event),
    DEVICE_U8(softpc_device_parallel_controller_state, retry_error_count)
};
static const softpc_device_wire_field parallel_host_port_fields[] = {
    DEVICE_U8(softpc_device_parallel_host_port_state, buffer),
    DEVICE_U32(softpc_device_parallel_host_port_state, port_status),
    DEVICE_U32(softpc_device_parallel_host_port_state, inactive_counter),
    DEVICE_U32(softpc_device_parallel_host_port_state, inactive_trigger),
    DEVICE_U32(softpc_device_parallel_host_port_state, bytes_in_buffer),
    DEVICE_U32(softpc_device_parallel_host_port_state, flush_threshold),
    DEVICE_U32(softpc_device_parallel_host_port_state, active),
    DEVICE_U32(softpc_device_parallel_host_port_state, direct_access),
    DEVICE_U32(softpc_device_parallel_host_port_state, no_device_attached)
};
static const softpc_device_wire_field pit_counter_fields[] = {
    DEVICE_U32(softpc_device_pit_counter_state, mode), DEVICE_U32(softpc_device_pit_counter_state, bcd),
    DEVICE_U32(softpc_device_pit_counter_state, read_load), DEVICE_U32(softpc_device_pit_counter_state, state),
    DEVICE_U32(softpc_device_pit_counter_state, state_prior_wait), DEVICE_U32(softpc_device_pit_counter_state, state_on_gate),
    DEVICE_U32(softpc_device_pit_counter_state, action_on_wait_complete), DEVICE_U32(softpc_device_pit_counter_state, action_on_gate_enabled),
    DEVICE_U8(softpc_device_pit_counter_state, output_lsb), DEVICE_U8(softpc_device_pit_counter_state, output_msb),
    DEVICE_U8(softpc_device_pit_counter_state, latch_value_lsb), DEVICE_U8(softpc_device_pit_counter_state, latch_value_msb), DEVICE_U8(softpc_device_pit_counter_state, latch_status),
    DEVICE_U32(softpc_device_pit_counter_state, initial_count), DEVICE_U32(softpc_device_pit_counter_state, read_state),
    DEVICE_U32(softpc_device_pit_counter_state, count_latched), DEVICE_U16(softpc_device_pit_counter_state, count),
    DEVICE_U16(softpc_device_pit_counter_state, new_count), DEVICE_U16(softpc_device_pit_counter_state, tick_adjust),
    DEVICE_U64(softpc_device_pit_counter_state, activation_age_microseconds), DEVICE_U32(softpc_device_pit_counter_state, terminal_count),
    DEVICE_U32(softpc_device_pit_counter_state, freeze_counter), DEVICE_U32(softpc_device_pit_counter_state, last_ticks),
    DEVICE_U32(softpc_device_pit_counter_state, microtick), DEVICE_U32(softpc_device_pit_counter_state, time_frig),
    DEVICE_U16(softpc_device_pit_counter_state, saved_count), DEVICE_U32(softpc_device_pit_counter_state, guesses_per_host_tick),
    DEVICE_U32(softpc_device_pit_counter_state, guesses_so_far), DEVICE_U32(softpc_device_pit_counter_state, delay),
    DEVICE_U32(softpc_device_pit_counter_state, trigger), DEVICE_U16(softpc_device_pit_counter_state, gate),
    DEVICE_U16(softpc_device_pit_counter_state, clock), DEVICE_U32(softpc_device_pit_counter_state, waveform_low),
    DEVICE_U32(softpc_device_pit_counter_state, waveform_high), DEVICE_U32(softpc_device_pit_counter_state, waveform_period),
    DEVICE_U32(softpc_device_pit_counter_state, waveform_start_level), DEVICE_U32(softpc_device_pit_counter_state, waveform_repeats)
};
static const softpc_device_wire_field pit_fields[] = {
    DEVICE_U32(softpc_device_pit_state, current_counter), DEVICE_U32(softpc_device_pit_state, ticks_blocked),
    DEVICE_U32(softpc_device_pit_state, timer_interrupt_enabled), DEVICE_U32(softpc_device_pit_state, time_lock),
    DEVICE_U32(softpc_device_pit_state, need_tick), DEVICE_U32(softpc_device_pit_state, hack_active),
    DEVICE_U32(softpc_device_pit_state, too_soon_after_previous), DEVICE_U32(softpc_device_pit_state, ticks_lost_this_time),
    DEVICE_U32(softpc_device_pit_state, real_mode_ticks_in_a_row), DEVICE_U32(softpc_device_pit_state, instructions_per_tick),
    DEVICE_U32(softpc_device_pit_state, adjusted_instructions_per_tick), DEVICE_U32(softpc_device_pit_state, real_mode_instruction_limit),
    DEVICE_U32(softpc_device_pit_state, adjusted_real_mode_tick_limit), DEVICE_U32(softpc_device_pit_state, maximum_backlog),
    DEVICE_U32(softpc_device_pit_state, more_timer_multiple), DEVICE_U32(softpc_device_pit_state, timer_multiple_delay),
    DEVICE_U32(softpc_device_pit_state, active_interrupt_event)
};

static const softpc_device_wire_field dos_mouse_fields[] = {
    DEVICE_U32(softpc_device_dos_mouse_state, initialized), DEVICE_U16(softpc_device_dos_mouse_state, interrupt_rate), DEVICE_U16(softpc_device_dos_mouse_state, com_revision),
    DEVICE_U16(softpc_device_dos_mouse_state, button_press_x), DEVICE_U16(softpc_device_dos_mouse_state, button_press_y), DEVICE_U16(softpc_device_dos_mouse_state, button_release_x), DEVICE_U16(softpc_device_dos_mouse_state, button_release_y),
    DEVICE_U16(softpc_device_dos_mouse_state, button_press_count), DEVICE_U16(softpc_device_dos_mouse_state, button_release_count),
    DEVICE_U16(softpc_device_dos_mouse_state, mouse_gear_x), DEVICE_U16(softpc_device_dos_mouse_state, mouse_gear_y), DEVICE_U16(softpc_device_dos_mouse_state, mouse_sensitivity_x), DEVICE_U16(softpc_device_dos_mouse_state, mouse_sensitivity_y), DEVICE_U16(softpc_device_dos_mouse_state, mouse_sensitivity_value_x), DEVICE_U16(softpc_device_dos_mouse_state, mouse_sensitivity_value_y),
    DEVICE_U16(softpc_device_dos_mouse_state, mouse_double_threshold), DEVICE_U16(softpc_device_dos_mouse_state, archive_text_cursor_type), DEVICE_U16(softpc_device_dos_mouse_state, text_cursor_screen), DEVICE_U16(softpc_device_dos_mouse_state, text_cursor_cursor),
    DEVICE_U16(softpc_device_dos_mouse_state, graphics_hot_spot_x), DEVICE_U16(softpc_device_dos_mouse_state, graphics_hot_spot_y), DEVICE_U16(softpc_device_dos_mouse_state, graphics_size_x), DEVICE_U16(softpc_device_dos_mouse_state, graphics_size_y),
    DEVICE_U16(softpc_device_dos_mouse_state, graphics_screen), DEVICE_U16(softpc_device_dos_mouse_state, graphics_cursor_words), DEVICE_U32(softpc_device_dos_mouse_state, graphics_screen_lo), DEVICE_U32(softpc_device_dos_mouse_state, graphics_screen_hi), DEVICE_U32(softpc_device_dos_mouse_state, graphics_cursor_lo), DEVICE_U32(softpc_device_dos_mouse_state, graphics_cursor_hi),
    DEVICE_U16(softpc_device_dos_mouse_state, user_handler_segment), DEVICE_U16(softpc_device_dos_mouse_state, user_handler_offset), DEVICE_U16(softpc_device_dos_mouse_state, user_handler_mask), DEVICE_U32(softpc_device_dos_mouse_state, alternate_handlers_active),
    DEVICE_U16(softpc_device_dos_mouse_state, alternate_handler_segment), DEVICE_U16(softpc_device_dos_mouse_state, alternate_handler_offset), DEVICE_U16(softpc_device_dos_mouse_state, alternate_handler_mask),
    DEVICE_U16(softpc_device_dos_mouse_state, black_hole_left), DEVICE_U16(softpc_device_dos_mouse_state, black_hole_top), DEVICE_U16(softpc_device_dos_mouse_state, black_hole_right), DEVICE_U16(softpc_device_dos_mouse_state, black_hole_bottom), DEVICE_U16(softpc_device_dos_mouse_state, archive_double_speed_threshold),
    DEVICE_U32(softpc_device_dos_mouse_state, archive_cursor_flag), DEVICE_U16(softpc_device_dos_mouse_state, cursor_x), DEVICE_U16(softpc_device_dos_mouse_state, cursor_y), DEVICE_U16(softpc_device_dos_mouse_state, button_status),
    DEVICE_U16(softpc_device_dos_mouse_state, cursor_window_left), DEVICE_U16(softpc_device_dos_mouse_state, cursor_window_top), DEVICE_U16(softpc_device_dos_mouse_state, cursor_window_right), DEVICE_U16(softpc_device_dos_mouse_state, cursor_window_bottom), DEVICE_U32(softpc_device_dos_mouse_state, archive_light_pen_mode),
    DEVICE_U16(softpc_device_dos_mouse_state, motion_x), DEVICE_U16(softpc_device_dos_mouse_state, motion_y), DEVICE_U16(softpc_device_dos_mouse_state, raw_motion_x), DEVICE_U16(softpc_device_dos_mouse_state, raw_motion_y),
    DEVICE_U16(softpc_device_dos_mouse_state, default_cursor_x), DEVICE_U16(softpc_device_dos_mouse_state, default_cursor_y), DEVICE_U16(softpc_device_dos_mouse_state, cursor_position_x), DEVICE_U16(softpc_device_dos_mouse_state, cursor_position_y), DEVICE_U16(softpc_device_dos_mouse_state, fractional_cursor_x), DEVICE_U16(softpc_device_dos_mouse_state, fractional_cursor_y),
    DEVICE_U32(softpc_device_dos_mouse_state, archive_cursor_page), DEVICE_U32(softpc_device_dos_mouse_state, archive_active_acceleration_curve), DEVICE_U32(softpc_device_dos_mouse_state, archive_next_video_mode),
    DEVICE_U16(softpc_device_dos_mouse_state, acceleration_length), DEVICE_U16(softpc_device_dos_mouse_state, acceleration_count), DEVICE_U16(softpc_device_dos_mouse_state, acceleration_scale), DEVICE_U16(softpc_device_dos_mouse_state, acceleration_name),
    DEVICE_U16(softpc_device_dos_mouse_state, driver_disabled), DEVICE_U16(softpc_device_dos_mouse_state, archive_current_video_mode), DEVICE_U16(softpc_device_dos_mouse_state, archive_text_cursor_background), DEVICE_U16(softpc_device_dos_mouse_state, archive_graphics_cursor_background), DEVICE_U32(softpc_device_dos_mouse_state, archive_save_area_in_use),
    DEVICE_U16(softpc_device_dos_mouse_state, save_position_x), DEVICE_U16(softpc_device_dos_mouse_state, save_position_y), DEVICE_U16(softpc_device_dos_mouse_state, save_area_left), DEVICE_U16(softpc_device_dos_mouse_state, save_area_top), DEVICE_U16(softpc_device_dos_mouse_state, save_area_right), DEVICE_U16(softpc_device_dos_mouse_state, save_area_bottom),
    DEVICE_U32(softpc_device_dos_mouse_state, archive_user_subroutine_critical), DEVICE_U16(softpc_device_dos_mouse_state, archive_last_condition_mask),
    DEVICE_U16(softpc_device_dos_mouse_state, saved_ax), DEVICE_U16(softpc_device_dos_mouse_state, saved_bx), DEVICE_U16(softpc_device_dos_mouse_state, saved_cx), DEVICE_U16(softpc_device_dos_mouse_state, saved_dx), DEVICE_U16(softpc_device_dos_mouse_state, saved_si), DEVICE_U16(softpc_device_dos_mouse_state, saved_di), DEVICE_U16(softpc_device_dos_mouse_state, saved_es), DEVICE_U16(softpc_device_dos_mouse_state, saved_bp), DEVICE_U16(softpc_device_dos_mouse_state, saved_ds),
    DEVICE_U16(softpc_device_dos_mouse_state, virtual_screen_left), DEVICE_U16(softpc_device_dos_mouse_state, virtual_screen_top), DEVICE_U16(softpc_device_dos_mouse_state, virtual_screen_right), DEVICE_U16(softpc_device_dos_mouse_state, virtual_screen_bottom), DEVICE_U16(softpc_device_dos_mouse_state, cursor_grid_x), DEVICE_U16(softpc_device_dos_mouse_state, cursor_grid_y), DEVICE_U16(softpc_device_dos_mouse_state, text_grid_x), DEVICE_U16(softpc_device_dos_mouse_state, text_grid_y),
    DEVICE_U16(softpc_device_dos_mouse_state, default_black_hole_left), DEVICE_U16(softpc_device_dos_mouse_state, default_black_hole_top), DEVICE_U16(softpc_device_dos_mouse_state, default_black_hole_right), DEVICE_U16(softpc_device_dos_mouse_state, default_black_hole_bottom),
    DEVICE_U16(softpc_device_dos_mouse_state, archive_saved_int33_segment), DEVICE_U16(softpc_device_dos_mouse_state, archive_saved_int33_offset), DEVICE_U16(softpc_device_dos_mouse_state, archive_saved_int10_segment), DEVICE_U16(softpc_device_dos_mouse_state, archive_saved_int10_offset), DEVICE_U16(softpc_device_dos_mouse_state, archive_saved_int0a_segment), DEVICE_U16(softpc_device_dos_mouse_state, archive_saved_int0a_offset), DEVICE_U32(softpc_device_dos_mouse_state, archive_int10_chained),
    DEVICE_U8(softpc_device_dos_mouse_state, archive_vga_background), DEVICE_U32(softpc_device_dos_mouse_state, ega_background_lo), DEVICE_U32(softpc_device_dos_mouse_state, ega_background_mid), DEVICE_U32(softpc_device_dos_mouse_state, ega_background_hi),
    DEVICE_U16(softpc_device_dos_mouse_state, archive_ega_current_crtc), DEVICE_U16(softpc_device_dos_mouse_state, archive_ega_current_graph), DEVICE_U16(softpc_device_dos_mouse_state, archive_ega_current_seq), DEVICE_U16(softpc_device_dos_mouse_state, archive_ega_current_attr), DEVICE_U16(softpc_device_dos_mouse_state, archive_ega_current_misc), DEVICE_U32(softpc_device_dos_mouse_state, cursor_em_disabled)
};

static lib_status
device_write_queue(const softpc_device_archive *archive,
    softpc_snapshot_bytes_write write, void *context)
{
    const Q_EVENT_SNAPSHOT_STATE *state = &archive->events;
    unsigned long index;
    lib_status status;
#define QUEUE_U32(value) do { \
    if ((unsigned long)(value) > UINT32_MAX) return LIB_STATUS_INVALID_ARGUMENT; \
    status = softpc_snapshot_stream_write_u32(write, context, (uint32_t)(value)); \
    if (status != LIB_STATUS_OK) return status; \
} while (0)
    QUEUE_U32(state->next_quick_handle); QUEUE_U32(state->next_tick_handle);
    QUEUE_U32(state->quick_count); QUEUE_U32(state->tick_count);
    QUEUE_U32(state->quick_entries); QUEUE_U32(state->tick_entries);
    for (index = 0u; index < 16u; ++index) { QUEUE_U32(state->quick_hash_head[index]); }
    for (index = 0u; index < 16u; ++index) { QUEUE_U32(state->tick_hash_head[index]); }
    for (index = 0u; index < state->quick_entries; ++index) {
        const Q_EVENT_SNAPSHOT_ENTRY *entry = &archive->quick_entries[index];
        QUEUE_U32(entry->time_from_last); QUEUE_U32(entry->original_time);
        QUEUE_U32(entry->handle); QUEUE_U32(entry->param); QUEUE_U32(entry->event_type);
        QUEUE_U32(entry->callback_id); QUEUE_U32(entry->hash_next_index);
    }
    for (index = 0u; index < state->tick_entries; ++index) {
        const Q_EVENT_SNAPSHOT_ENTRY *entry = &archive->tick_entries[index];
        QUEUE_U32(entry->time_from_last); QUEUE_U32(entry->original_time);
        QUEUE_U32(entry->handle); QUEUE_U32(entry->param); QUEUE_U32(entry->event_type);
        QUEUE_U32(entry->callback_id); QUEUE_U32(entry->hash_next_index);
    }
#undef QUEUE_U32
    return LIB_STATUS_OK;
}

static lib_status
device_read_queue(softpc_device_archive *archive, softpc_snapshot_bytes_read read,
    void *context)
{
    Q_EVENT_SNAPSHOT_STATE *state = &archive->events;
    uint32_t value;
    unsigned long index;
    lib_status status;
#define READ_QUEUE_U32(target) do { \
    status = softpc_snapshot_stream_read_u32(read, context, &value); \
    if (status != LIB_STATUS_OK) return status; (target) = (unsigned long)value; \
} while (0)
    READ_QUEUE_U32(state->next_quick_handle); READ_QUEUE_U32(state->next_tick_handle);
    READ_QUEUE_U32(state->quick_count); READ_QUEUE_U32(state->tick_count);
    READ_QUEUE_U32(state->quick_entries); READ_QUEUE_U32(state->tick_entries);
    if (state->quick_entries > 65536u || state->tick_entries > 65536u)
        return LIB_STATUS_INVALID_ARGUMENT;
    for (index = 0u; index < 16u; ++index) { READ_QUEUE_U32(state->quick_hash_head[index]); }
    for (index = 0u; index < 16u; ++index) { READ_QUEUE_U32(state->tick_hash_head[index]); }
    if (!resize_entries(&archive->quick_entries, &archive->quick_capacity, state->quick_entries) ||
        !resize_entries(&archive->tick_entries, &archive->tick_capacity, state->tick_entries))
        return LIB_STATUS_NO_MEMORY;
    for (index = 0u; index < state->quick_entries + state->tick_entries; ++index) {
        Q_EVENT_SNAPSHOT_ENTRY *entry = index < state->quick_entries ?
            &archive->quick_entries[index] : &archive->tick_entries[index - state->quick_entries];
        READ_QUEUE_U32(entry->time_from_last); READ_QUEUE_U32(entry->original_time);
        READ_QUEUE_U32(entry->handle); READ_QUEUE_U32(entry->param); READ_QUEUE_U32(entry->event_type);
        READ_QUEUE_U32(entry->callback_id); READ_QUEUE_U32(entry->hash_next_index);
    }
#undef READ_QUEUE_U32
    return LIB_STATUS_OK;
}

static lib_status
device_write_all(const softpc_device_archive *archive,
    softpc_snapshot_bytes_write write, void *context)
{
    lib_size index; lib_status status;
#define WRITE_MAP(object, map) do { status = device_write_map(object, map, DEVICE_FIELD_COUNT(map), write, context); if (status != LIB_STATUS_OK) return status; } while (0)
    if ((status = device_write_queue(archive, write, context)) != LIB_STATUS_OK) return status;
    WRITE_MAP(&archive->dma, dma_fields);
    for (index = 0u; index < SOFTPC_DEVICE_PIC_COUNT; ++index) WRITE_MAP(&archive->pic.adapter[index], pic_adapter_fields);
    WRITE_MAP(&archive->pic, pic_fields);
    for (index = 0u; index < SOFTPC_DEVICE_PIT_COUNTER_COUNT; ++index) WRITE_MAP(&archive->pit.counter[index], pit_counter_fields);
    WRITE_MAP(&archive->pit, pit_fields); WRITE_MAP(&archive->cmos, cmos_fields); WRITE_MAP(&archive->fdc, fdc_fields);
    for (index = 0u; index < SOFTPC_DEVICE_HDD_DRIVE_COUNT; ++index) WRITE_MAP(&archive->hdd.drive[index], hdd_drive_fields);
    WRITE_MAP(&archive->hdd, hdd_fields); WRITE_MAP(&archive->ppi, ppi_fields); WRITE_MAP(&archive->inport_mouse, inport_mouse_fields); WRITE_MAP(&archive->keyboard, keyboard_fields); WRITE_MAP(&archive->dos_mouse, dos_mouse_fields); WRITE_MAP(&archive->video_memory, video_memory_fields); WRITE_MAP(&archive->video_controller, video_controller_fields);
    for (index = 0u; index < SOFTPC_DEVICE_SERIAL_PORT_COUNT; ++index) WRITE_MAP(&archive->serial_controller.port[index], serial_port_fields);
    WRITE_MAP(&archive->serial_controller, serial_controller_fields);
    for (index = 0u; index < SOFTPC_DEVICE_SERIAL_PORT_COUNT; ++index) WRITE_MAP(&archive->serial_host.port[index], serial_host_port_fields);
    WRITE_MAP(&archive->parallel_controller, parallel_controller_fields);
    for (index = 0u; index < SOFTPC_DEVICE_PARALLEL_PORT_COUNT; ++index) WRITE_MAP(&archive->parallel_host.port[index], parallel_host_port_fields);
#undef WRITE_MAP
    return LIB_STATUS_OK;
}

static lib_status
device_read_all(softpc_device_archive *archive, softpc_snapshot_bytes_read read,
    void *context)
{
    lib_size index; lib_status status;
#define READ_MAP(object, map) do { status = device_read_map(object, map, DEVICE_FIELD_COUNT(map), read, context); if (status != LIB_STATUS_OK) return status; } while (0)
    if ((status = device_read_queue(archive, read, context)) != LIB_STATUS_OK) return status;
    READ_MAP(&archive->dma, dma_fields);
    for (index = 0u; index < SOFTPC_DEVICE_PIC_COUNT; ++index) READ_MAP(&archive->pic.adapter[index], pic_adapter_fields);
    READ_MAP(&archive->pic, pic_fields);
    for (index = 0u; index < SOFTPC_DEVICE_PIT_COUNTER_COUNT; ++index) READ_MAP(&archive->pit.counter[index], pit_counter_fields);
    READ_MAP(&archive->pit, pit_fields); READ_MAP(&archive->cmos, cmos_fields); READ_MAP(&archive->fdc, fdc_fields);
    for (index = 0u; index < SOFTPC_DEVICE_HDD_DRIVE_COUNT; ++index) READ_MAP(&archive->hdd.drive[index], hdd_drive_fields);
    READ_MAP(&archive->hdd, hdd_fields); READ_MAP(&archive->ppi, ppi_fields); READ_MAP(&archive->inport_mouse, inport_mouse_fields); READ_MAP(&archive->keyboard, keyboard_fields); READ_MAP(&archive->dos_mouse, dos_mouse_fields); READ_MAP(&archive->video_memory, video_memory_fields); READ_MAP(&archive->video_controller, video_controller_fields);
    for (index = 0u; index < SOFTPC_DEVICE_SERIAL_PORT_COUNT; ++index) READ_MAP(&archive->serial_controller.port[index], serial_port_fields);
    READ_MAP(&archive->serial_controller, serial_controller_fields);
    for (index = 0u; index < SOFTPC_DEVICE_SERIAL_PORT_COUNT; ++index) READ_MAP(&archive->serial_host.port[index], serial_host_port_fields);
    READ_MAP(&archive->parallel_controller, parallel_controller_fields);
    for (index = 0u; index < SOFTPC_DEVICE_PARALLEL_PORT_COUNT; ++index) READ_MAP(&archive->parallel_host.port[index], parallel_host_port_fields);
#undef READ_MAP
    return LIB_STATUS_OK;
}

lib_status
softpc_device_archive_write(const softpc_device_archive *archive,
    softpc_snapshot_bytes_write write, void *context)
{
    if (archive == NULL || !archive->valid || write == NULL) return LIB_STATUS_INVALID_ARGUMENT;
    return device_write_all(archive, write, context);
}

lib_status
softpc_device_archive_read(softpc_device_archive **out_archive,
    softpc_snapshot_bytes_read read, void *context)
{
    softpc_device_archive *archive; lib_status status;
    if (out_archive == NULL || read == NULL) return LIB_STATUS_INVALID_ARGUMENT;
    *out_archive = NULL;
    archive = softpc_device_archive_create();
    if (archive == NULL) return LIB_STATUS_NO_MEMORY;
    status = device_read_all(archive, read, context);
    if (status != LIB_STATUS_OK) { softpc_device_archive_dispose(archive); return status; }
    archive->valid = TRUE; *out_archive = archive; return LIB_STATUS_OK;
}
