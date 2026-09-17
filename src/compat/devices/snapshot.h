#ifndef SOFTPC_DEVICE_SNAPSHOT_H
#define SOFTPC_DEVICE_SNAPSHOT_H

#include <stdint.h>

/*
 * Fixed-width private state boundary for machine-device archives.  It is not
 * an on-disk format and must never acquire host pointers, callbacks, native
 * handles, or raw legacy structs.
 */
enum {
    SOFTPC_DEVICE_DMA_CONTROLLER_COUNT = 2,
    SOFTPC_DEVICE_DMA_CHANNEL_COUNT = 4,
    SOFTPC_DEVICE_DMA_PAGE_COUNT = 16,
    SOFTPC_DEVICE_PIC_COUNT = 2,
    SOFTPC_DEVICE_PIC_LINE_COUNT = 8,
    SOFTPC_DEVICE_PIC_ISR_DEPTH = 3,
    SOFTPC_DEVICE_CMOS_SIZE = 64,
    SOFTPC_DEVICE_FDC_COMMAND_BYTES = 9,
    SOFTPC_DEVICE_FDC_RESULT_BYTES = 7,
    SOFTPC_DEVICE_FDC_DRIVE_COUNT = 4,
    SOFTPC_DEVICE_FDC_NDMA_BYTES = 8192,
    SOFTPC_DEVICE_HDD_DRIVE_COUNT = 2,
    SOFTPC_DEVICE_HDD_SECTOR_WORDS = 256,
    SOFTPC_DEVICE_PIT_COUNTER_COUNT = 3,
    SOFTPC_DEVICE_KEYBOARD_FIFO_COUNT = 48,
    SOFTPC_DEVICE_KEYBOARD_KEY_COUNT = 127,
    SOFTPC_DEVICE_KEYBOARD_HELD_EVENT_COUNT = 16,
    SOFTPC_DEVICE_DOS_MOUSE_BUTTON_COUNT = 2,
    SOFTPC_DEVICE_DOS_MOUSE_ALT_HANDLER_COUNT = 3,
    SOFTPC_DEVICE_DOS_MOUSE_CURSOR_DEPTH = 16,
    SOFTPC_DEVICE_DOS_MOUSE_CURSOR_WIDTH = 16,
    SOFTPC_DEVICE_DOS_MOUSE_ACCELERATION_CURVE_COUNT = 4,
    SOFTPC_DEVICE_DOS_MOUSE_ACCELERATION_MICKEY_COUNT = 32,
    SOFTPC_DEVICE_DOS_MOUSE_ACCELERATION_SCALE_COUNT = 32,
    SOFTPC_DEVICE_DOS_MOUSE_ACCELERATION_NAME_COUNT = 16,
    SOFTPC_DEVICE_DOS_MOUSE_EGA_CRTC_COUNT = 25,
    SOFTPC_DEVICE_DOS_MOUSE_EGA_GRAPH_COUNT = 9,
    SOFTPC_DEVICE_DOS_MOUSE_EGA_SEQ_COUNT = 4,
    SOFTPC_DEVICE_DOS_MOUSE_EGA_ATTR_COUNT = 20,
    /* The selected V7 build has four 128 KiB physical EGA/VGA planes. */
    SOFTPC_DEVICE_VIDEO_PLANE_BYTES = 0x80000,
    SOFTPC_DEVICE_VIDEO_DAC_COUNT = 256,
    SOFTPC_DEVICE_VIDEO_DAC_COMPONENT_COUNT = 3,
    SOFTPC_DEVICE_SERIAL_PORT_COUNT = 4,
    SOFTPC_DEVICE_SERIAL_RX_COUNT = 256,
    SOFTPC_DEVICE_PARALLEL_PORT_COUNT = 3,
    SOFTPC_DEVICE_PARALLEL_BUFFER_BYTES = 1024
};

enum {
    SOFTPC_DEVICE_QUEUE_TIMER_CLEAR_TOO_SOON = 1,
    SOFTPC_DEVICE_QUEUE_TIMER_MULTIPLE_INTERRUPTS,
    SOFTPC_DEVICE_QUEUE_RTC_PERIODIC,
    SOFTPC_DEVICE_QUEUE_RTC_ALARM,
    SOFTPC_DEVICE_QUEUE_RTC_SYNC,
    SOFTPC_DEVICE_QUEUE_FDC_INTERRUPT,
    SOFTPC_DEVICE_QUEUE_HDD_PAUSE,
    SOFTPC_DEVICE_QUEUE_HDD_INTERRUPT,
    SOFTPC_DEVICE_QUEUE_KEYBOARD_INTERRUPT,
    SOFTPC_DEVICE_QUEUE_KEYBOARD_REFILL,
    SOFTPC_DEVICE_QUEUE_SERIAL_RECEIVE,
    SOFTPC_DEVICE_QUEUE_SERIAL_SEND,
    SOFTPC_DEVICE_QUEUE_PRINTER_OUT,
    SOFTPC_DEVICE_QUEUE_PRINTER_OUT_ACK
};

/*
 * The selected standalone COM/LPT host endpoints are finite virtual devices
 * when no output file is configured.  File-backed endpoints are deliberately
 * rejected by their capture functions: external output is not snapshot data.
 */
typedef struct softpc_device_serial_port_state {
    uint16_t tx_buffer, rx_buffer, divisor_latch;
    uint16_t int_enable, int_id, line_control, modem_control;
    uint16_t line_status, modem_status, scratch;
    int32_t break_state, loopback_state, dtr_state, rts_state;
    int32_t out1_state, out2_state;
    int32_t receiver_line_status_interrupt, data_available_interrupt;
    int32_t tx_empty_interrupt, modem_status_interrupt;
    int32_t interrupt_priority, baud_index, had_first_read;
} softpc_device_serial_port_state;

typedef struct softpc_device_serial_controller_state {
    softpc_device_serial_port_state port[SOFTPC_DEVICE_SERIAL_PORT_COUNT];
    int32_t critical[SOFTPC_DEVICE_SERIAL_PORT_COUNT];
    uint16_t line_control_flush_mask;
    int32_t transmit_pacing;
} softpc_device_serial_controller_state;

typedef struct softpc_device_serial_host_port_state {
    uint8_t rx[SOFTPC_DEVICE_SERIAL_RX_COUNT];
    uint32_t rx_count, tx_count;
    int32_t baud, data_bits, stop_bits, parity, break_enabled;
    int32_t dtr, rts, opened, xon_enabled, modem;
    uint16_t last_msr;
} softpc_device_serial_host_port_state;

typedef struct softpc_device_serial_host_state {
    softpc_device_serial_host_port_state port[SOFTPC_DEVICE_SERIAL_PORT_COUNT];
} softpc_device_serial_host_state;

typedef struct softpc_device_parallel_controller_state {
    uint16_t output[SOFTPC_DEVICE_PARALLEL_PORT_COUNT];
    uint16_t control[SOFTPC_DEVICE_PARALLEL_PORT_COUNT];
    uint16_t status[SOFTPC_DEVICE_PARALLEL_PORT_COUNT];
    int32_t state[SOFTPC_DEVICE_PARALLEL_PORT_COUNT];
    int64_t out_event[SOFTPC_DEVICE_PARALLEL_PORT_COUNT];
    int64_t out_ack_event[SOFTPC_DEVICE_PARALLEL_PORT_COUNT];
    uint8_t retry_error_count;
} softpc_device_parallel_controller_state;

typedef struct softpc_device_parallel_host_port_state {
    uint8_t buffer[SOFTPC_DEVICE_PARALLEL_BUFFER_BYTES];
    uint32_t port_status;
    int32_t inactive_counter, inactive_trigger, bytes_in_buffer;
    int32_t flush_threshold, active, direct_access, no_device_attached;
} softpc_device_parallel_host_port_state;

typedef struct softpc_device_parallel_host_state {
    softpc_device_parallel_host_port_state port[SOFTPC_DEVICE_PARALLEL_PORT_COUNT];
} softpc_device_parallel_host_state;

int softpc_device_snapshot_capture_serial_controller(
    softpc_device_serial_controller_state *state);
int softpc_device_snapshot_restore_serial_controller(
    const softpc_device_serial_controller_state *state);
int softpc_device_snapshot_capture_serial_host(
    softpc_device_serial_host_state *state);
int softpc_device_snapshot_restore_serial_host(
    const softpc_device_serial_host_state *state);
int softpc_device_snapshot_capture_parallel_controller(
    softpc_device_parallel_controller_state *state);
int softpc_device_snapshot_restore_parallel_controller(
    const softpc_device_parallel_controller_state *state);
int softpc_device_snapshot_capture_parallel_host(
    softpc_device_parallel_host_state *state);
int softpc_device_snapshot_restore_parallel_host(
    const softpc_device_parallel_host_state *state);

typedef struct softpc_device_dma_state {
    uint8_t base_address[SOFTPC_DEVICE_DMA_CONTROLLER_COUNT]
        [SOFTPC_DEVICE_DMA_CHANNEL_COUNT][2];
    uint8_t base_count[SOFTPC_DEVICE_DMA_CONTROLLER_COUNT]
        [SOFTPC_DEVICE_DMA_CHANNEL_COUNT][2];
    uint8_t current_address[SOFTPC_DEVICE_DMA_CONTROLLER_COUNT]
        [SOFTPC_DEVICE_DMA_CHANNEL_COUNT][2];
    uint8_t current_count[SOFTPC_DEVICE_DMA_CONTROLLER_COUNT]
        [SOFTPC_DEVICE_DMA_CHANNEL_COUNT][2];
    uint8_t temporary_address[SOFTPC_DEVICE_DMA_CONTROLLER_COUNT]
        [SOFTPC_DEVICE_DMA_CHANNEL_COUNT][2];
    uint8_t temporary_count[SOFTPC_DEVICE_DMA_CONTROLLER_COUNT]
        [SOFTPC_DEVICE_DMA_CHANNEL_COUNT][2];
    uint8_t status[SOFTPC_DEVICE_DMA_CONTROLLER_COUNT];
    uint8_t command[SOFTPC_DEVICE_DMA_CONTROLLER_COUNT];
    uint8_t temporary[SOFTPC_DEVICE_DMA_CONTROLLER_COUNT];
    uint8_t mode[SOFTPC_DEVICE_DMA_CONTROLLER_COUNT]
        [SOFTPC_DEVICE_DMA_CHANNEL_COUNT];
    uint8_t mask[SOFTPC_DEVICE_DMA_CONTROLLER_COUNT];
    uint8_t request[SOFTPC_DEVICE_DMA_CONTROLLER_COUNT];
    uint32_t first_last[SOFTPC_DEVICE_DMA_CONTROLLER_COUNT];
    uint8_t page[SOFTPC_DEVICE_DMA_PAGE_COUNT];
} softpc_device_dma_state;

void softpc_device_snapshot_capture_dma(softpc_device_dma_state *state);
int softpc_device_snapshot_restore_dma(const softpc_device_dma_state *state);

typedef struct softpc_device_pic_adapter_state {
    uint32_t master;
    uint8_t irr, isr, imr, ssr;
    uint16_t base, hipri, mode;
    int32_t count[SOFTPC_DEVICE_PIC_LINE_COUNT];
    uint32_t interrupt_line, cpu_interrupt;
    uint32_t callback_parameter[SOFTPC_DEVICE_PIC_LINE_COUNT];
    int32_t isr_depth[SOFTPC_DEVICE_PIC_LINE_COUNT];
    int32_t isr_progress[SOFTPC_DEVICE_PIC_LINE_COUNT]
        [SOFTPC_DEVICE_PIC_ISR_DEPTH + 1];
    int32_t isr_time_decay[SOFTPC_DEVICE_PIC_LINE_COUNT]
        [SOFTPC_DEVICE_PIC_ISR_DEPTH];
} softpc_device_pic_adapter_state;

typedef struct softpc_device_pic_state {
    softpc_device_pic_adapter_state adapter[SOFTPC_DEVICE_PIC_COUNT];
    uint32_t iret_hooks_enabled;
    uint16_t iret_hook_mask, iret_hook_active;
} softpc_device_pic_state;

int softpc_device_snapshot_capture_pic(softpc_device_pic_state *state);
int softpc_device_snapshot_restore_pic(const softpc_device_pic_state *state);

typedef struct softpc_device_cmos_state {
    uint8_t bytes[SOFTPC_DEVICE_CMOS_SIZE];
    int32_t selected_index;
    int32_t data_mode_yes, twenty4_hour_clock;
    int32_t reset_alarm, rtc_int_enabled, cmos_count;
    int32_t host_time_valid;
    int32_t host_time[9];
    int64_t user_time;
    uint32_t periodic_milliseconds;
    int64_t periodic_event_handle;
} softpc_device_cmos_state;

void softpc_device_snapshot_capture_cmos(softpc_device_cmos_state *state);
int softpc_device_snapshot_restore_cmos(const softpc_device_cmos_state *state);

typedef struct softpc_device_fdc_state {
    uint8_t command[SOFTPC_DEVICE_FDC_COMMAND_BYTES];
    uint8_t result[SOFTPC_DEVICE_FDC_RESULT_BYTES];
    uint8_t sense[SOFTPC_DEVICE_FDC_DRIVE_COUNT][3];
    uint8_t ndma[SOFTPC_DEVICE_FDC_NDMA_BYTES];
    uint8_t status, current_command, interrupt_line, dor, drive_selected;
    uint8_t interrupt_pending, busy, ndma_enabled;
    uint32_t command_count, result_count;
    int32_t ndma_count, ndma_sector_size;
} softpc_device_fdc_state;

void softpc_device_snapshot_capture_fdc(softpc_device_fdc_state *state);
int softpc_device_snapshot_restore_fdc(const softpc_device_fdc_state *state);

typedef struct softpc_device_hdd_drive_state {
    int32_t drive_id;
    int32_t max_head, max_cylinder, max_sector;
    int32_t sectors_per_track, bytes_per_cylinder, bytes_per_track;
    int32_t wired_up, current_offset;
} softpc_device_hdd_drive_state;

typedef struct softpc_device_hdd_state {
    softpc_device_hdd_drive_state drive[SOFTPC_DEVICE_HDD_DRIVE_COUNT];
    uint8_t taskfile[10];
    uint16_t sector[SOFTPC_DEVICE_HDD_SECTOR_WORDS];
    uint8_t fixed_disk_register, digital_input_register, sector_index;
    int32_t selected_drive, active_command;
} softpc_device_hdd_state;

int softpc_device_snapshot_capture_hdd(softpc_device_hdd_state *state);
int softpc_device_snapshot_restore_hdd(const softpc_device_hdd_state *state);

typedef struct softpc_device_ppi_state {
    uint8_t register_value;
    uint8_t gate_2_was_low;
    uint8_t speaker_data_was_low;
} softpc_device_ppi_state;

void softpc_device_snapshot_capture_ppi(softpc_device_ppi_state *state);
int softpc_device_snapshot_restore_ppi(const softpc_device_ppi_state *state);

/*
 * Microsoft InPort hardware state only.  The DOS INT 33h driver has a
 * separate continuation and remains outside this receiver until its guest
 * callback and cursor backing state have their own fixed-width map.
 */
typedef struct softpc_device_inport_mouse_state {
    int32_t button_left, button_right;
    int32_t delta_x, delta_y;
    uint16_t data_1, data_2, status;
    uint16_t last_button_left, last_button_right;
    uint16_t mode, address, test_data;
    int32_t startup_interrupt_bursts, id_toggle, test_state;
} softpc_device_inport_mouse_state;

int softpc_device_snapshot_capture_inport_mouse(
    softpc_device_inport_mouse_state *state);
int softpc_device_snapshot_restore_inport_mouse(
    const softpc_device_inport_mouse_state *state);

typedef struct softpc_device_keyboard_state {
    uint16_t fifo[SOFTPC_DEVICE_KEYBOARD_FIFO_COUNT];
    uint16_t set_3_key_state[SOFTPC_DEVICE_KEYBOARD_KEY_COUNT];
    int32_t key_down_count[SOFTPC_DEVICE_KEYBOARD_KEY_COUNT];
    int32_t held_key[SOFTPC_DEVICE_KEYBOARD_HELD_EVENT_COUNT];
    int32_t held_type[SOFTPC_DEVICE_KEYBOARD_HELD_EVENT_COUNT];
    int32_t fifo_count, sent_overrun;
    int32_t anomalous_index, anomalous_size, anomalous_key, anomalous_active;
    int32_t held_count, scan_set;
    int32_t repeat_delay_target, repeat_target, repeat_delay_count, repeat_count;
    int32_t typematic_key, input_port_value, typematic_key_valid;
    int32_t waiting_for_next_code, waiting_for_next_8042_code, num_lock_on;
    int32_t shift_on, left_shift_on, right_shift_on;
    int32_t ctrl_on, left_ctrl_on, right_ctrl_on;
    int32_t alt_on, left_alt_on, right_alt_on;
    int32_t waiting_for_upcode, next_code_sequence, next_8042_sequence;
    int32_t set_3_key_type_change_destination;
    int32_t translating, keyboard_disabled, interrupt_enabled, output_full;
    int32_t pending_8042, interface_disabled, scanning_discontinued;
    int32_t gate_a20_status, reset_was_by_keyboard;
    uint16_t output_contents, pending_8042_value, status;
    uint16_t output_port_bits, command_byte, light_pattern;
    uint32_t refill_event_handle;
} softpc_device_keyboard_state;

int softpc_device_snapshot_capture_keyboard(softpc_device_keyboard_state *state);
int softpc_device_snapshot_restore_keyboard(
    const softpc_device_keyboard_state *state);

/*
 * DOS INT 33h driver state.  This is deliberately a field map, rather than
 * the historical MOUSE_CONTEXT/MM_INSTANCE_DATA layout: the latter contains
 * host-width fields, derived addresses and host drawing resources.  Cursor
 * backing bytes are guest-visible data because the driver needs them to
 * remove an already drawn cursor after restore.
 */
typedef struct softpc_device_dos_mouse_state {
    uint32_t initialized;
    uint16_t interrupt_rate;
    uint16_t com_revision;
    int16_t button_press_x[SOFTPC_DEVICE_DOS_MOUSE_BUTTON_COUNT];
    int16_t button_press_y[SOFTPC_DEVICE_DOS_MOUSE_BUTTON_COUNT];
    int16_t button_release_x[SOFTPC_DEVICE_DOS_MOUSE_BUTTON_COUNT];
    int16_t button_release_y[SOFTPC_DEVICE_DOS_MOUSE_BUTTON_COUNT];
    int16_t button_press_count[SOFTPC_DEVICE_DOS_MOUSE_BUTTON_COUNT];
    int16_t button_release_count[SOFTPC_DEVICE_DOS_MOUSE_BUTTON_COUNT];
    int16_t mouse_gear_x, mouse_gear_y, mouse_sensitivity_x,
        mouse_sensitivity_y, mouse_sensitivity_value_x,
        mouse_sensitivity_value_y;
    uint16_t mouse_double_threshold, archive_text_cursor_type;
    uint16_t text_cursor_screen, text_cursor_cursor;
    int16_t graphics_hot_spot_x, graphics_hot_spot_y;
    int16_t graphics_size_x, graphics_size_y;
    uint16_t graphics_screen[SOFTPC_DEVICE_DOS_MOUSE_CURSOR_DEPTH];
    uint16_t graphics_cursor_words[SOFTPC_DEVICE_DOS_MOUSE_CURSOR_DEPTH];
    uint32_t graphics_screen_lo[SOFTPC_DEVICE_DOS_MOUSE_CURSOR_DEPTH];
    uint32_t graphics_screen_hi[SOFTPC_DEVICE_DOS_MOUSE_CURSOR_DEPTH];
    uint32_t graphics_cursor_lo[SOFTPC_DEVICE_DOS_MOUSE_CURSOR_DEPTH];
    uint32_t graphics_cursor_hi[SOFTPC_DEVICE_DOS_MOUSE_CURSOR_DEPTH];
    uint16_t user_handler_segment, user_handler_offset, user_handler_mask;
    uint32_t alternate_handlers_active;
    uint16_t alternate_handler_segment
        [SOFTPC_DEVICE_DOS_MOUSE_ALT_HANDLER_COUNT];
    uint16_t alternate_handler_offset
        [SOFTPC_DEVICE_DOS_MOUSE_ALT_HANDLER_COUNT];
    uint16_t alternate_handler_mask
        [SOFTPC_DEVICE_DOS_MOUSE_ALT_HANDLER_COUNT];
    int16_t black_hole_left, black_hole_top, black_hole_right,
        black_hole_bottom;
    int16_t archive_double_speed_threshold;
    int32_t archive_cursor_flag;
    int16_t cursor_x, cursor_y, button_status;
    int16_t cursor_window_left, cursor_window_top, cursor_window_right,
        cursor_window_bottom;
    uint32_t archive_light_pen_mode;
    int16_t motion_x, motion_y, raw_motion_x, raw_motion_y;
    int16_t default_cursor_x, default_cursor_y, cursor_position_x,
        cursor_position_y, fractional_cursor_x, fractional_cursor_y;
    int32_t archive_cursor_page, archive_active_acceleration_curve,
        archive_next_video_mode;
    uint16_t acceleration_length[SOFTPC_DEVICE_DOS_MOUSE_ACCELERATION_CURVE_COUNT];
    uint16_t acceleration_count[SOFTPC_DEVICE_DOS_MOUSE_ACCELERATION_CURVE_COUNT]
        [SOFTPC_DEVICE_DOS_MOUSE_ACCELERATION_MICKEY_COUNT];
    uint16_t acceleration_scale[SOFTPC_DEVICE_DOS_MOUSE_ACCELERATION_CURVE_COUNT]
        [SOFTPC_DEVICE_DOS_MOUSE_ACCELERATION_SCALE_COUNT];
    uint16_t acceleration_name[SOFTPC_DEVICE_DOS_MOUSE_ACCELERATION_CURVE_COUNT]
        [SOFTPC_DEVICE_DOS_MOUSE_ACCELERATION_NAME_COUNT];
    uint16_t driver_disabled;
    int16_t archive_current_video_mode;
    uint16_t archive_text_cursor_background;
    uint16_t archive_graphics_cursor_background[SOFTPC_DEVICE_DOS_MOUSE_CURSOR_DEPTH];
    uint32_t archive_save_area_in_use;
    int16_t save_position_x, save_position_y;
    int16_t save_area_left, save_area_top, save_area_right, save_area_bottom;
    uint32_t archive_user_subroutine_critical;
    uint16_t archive_last_condition_mask;
    uint16_t saved_ax, saved_bx, saved_cx, saved_dx, saved_si, saved_di,
        saved_es, saved_bp, saved_ds;
    int16_t virtual_screen_left, virtual_screen_top, virtual_screen_right,
        virtual_screen_bottom;
    int16_t cursor_grid_x, cursor_grid_y, text_grid_x, text_grid_y;
    int16_t default_black_hole_left, default_black_hole_top,
        default_black_hole_right, default_black_hole_bottom;
    uint16_t archive_saved_int33_segment, archive_saved_int33_offset,
        archive_saved_int10_segment, archive_saved_int10_offset,
        archive_saved_int0a_segment, archive_saved_int0a_offset;
    uint32_t archive_int10_chained;
    uint8_t archive_vga_background[SOFTPC_DEVICE_DOS_MOUSE_CURSOR_DEPTH]
        [SOFTPC_DEVICE_DOS_MOUSE_CURSOR_WIDTH];
    uint32_t ega_background_lo[SOFTPC_DEVICE_DOS_MOUSE_CURSOR_DEPTH];
    uint32_t ega_background_mid[SOFTPC_DEVICE_DOS_MOUSE_CURSOR_DEPTH];
    uint32_t ega_background_hi[SOFTPC_DEVICE_DOS_MOUSE_CURSOR_DEPTH];
    uint16_t archive_ega_current_crtc[SOFTPC_DEVICE_DOS_MOUSE_EGA_CRTC_COUNT];
    uint16_t archive_ega_current_graph[SOFTPC_DEVICE_DOS_MOUSE_EGA_GRAPH_COUNT];
    uint16_t archive_ega_current_seq[SOFTPC_DEVICE_DOS_MOUSE_EGA_SEQ_COUNT];
    uint16_t archive_ega_current_attr[SOFTPC_DEVICE_DOS_MOUSE_EGA_ATTR_COUNT];
    uint16_t archive_ega_current_misc;
    uint32_t cursor_em_disabled;
} softpc_device_dos_mouse_state;

int softpc_device_snapshot_capture_dos_mouse(
    softpc_device_dos_mouse_state *state);
int softpc_device_snapshot_restore_dos_mouse(
    const softpc_device_dos_mouse_state *state);

/*
 * Guest video memory and the programmable DAC are fixed-size semantic data.
 * Controller registers, C-VID state and renderer resources deliberately have
 * separate receivers: they contain a mixture of values and process bindings.
 */
typedef struct softpc_device_video_memory_state {
    uint8_t plane[SOFTPC_DEVICE_VIDEO_PLANE_BYTES];
    uint8_t dac[SOFTPC_DEVICE_VIDEO_DAC_COUNT]
        [SOFTPC_DEVICE_VIDEO_DAC_COMPONENT_COUNT];
} softpc_device_video_memory_state;

int softpc_device_snapshot_capture_video_memory(
    softpc_device_video_memory_state *state);
int softpc_device_snapshot_restore_video_memory(
    const softpc_device_video_memory_state *state);
int softpc_device_snapshot_rebuild_video_presentation(void);

/*
 * Controller state is a register-value map, never a copy of the original
 * bitfield carriers or C-VID GDP allocation.  The V7 array uses the explicit
 * selector order owned by the original video hook.
 */
enum {
    SOFTPC_DEVICE_VIDEO_SEQUENCE_REGISTER_COUNT = 5,
    SOFTPC_DEVICE_VIDEO_CRTC_REGISTER_COUNT = 25,
    SOFTPC_DEVICE_VIDEO_GRAPHICS_REGISTER_COUNT = 9,
    SOFTPC_DEVICE_VIDEO_ATTRIBUTE_REGISTER_COUNT = 21,
    SOFTPC_DEVICE_VIDEO_V7_REGISTER_COUNT = 28
};

typedef struct softpc_device_video_controller_state {
    uint8_t sequencer[SOFTPC_DEVICE_VIDEO_SEQUENCE_REGISTER_COUNT];
    uint8_t crtc[SOFTPC_DEVICE_VIDEO_CRTC_REGISTER_COUNT];
    uint8_t graphics[SOFTPC_DEVICE_VIDEO_GRAPHICS_REGISTER_COUNT];
    uint8_t attribute[SOFTPC_DEVICE_VIDEO_ATTRIBUTE_REGISTER_COUNT];
    uint8_t v7[SOFTPC_DEVICE_VIDEO_V7_REGISTER_COUNT];
    uint8_t sequencer_extension_control;
    uint8_t miscellaneous_output, feature_control;
    uint8_t sequencer_index, crtc_index, graphics_index, attribute_index;
    uint8_t dac_mask, dac_read_address, dac_write_address, dac_component,
        dac_state;
    uint8_t currently_emulated_video_mode;
    uint32_t cvid_latches, cvid_v7_foreground_latches;
} softpc_device_video_controller_state;

int softpc_device_snapshot_capture_video_controller(
    softpc_device_video_controller_state *state);
int softpc_device_snapshot_restore_video_controller(
    const softpc_device_video_controller_state *state);

/*
 * The PIT uses function pointers and host clock timestamps internally.  The
 * archive stores only the finite state-machine identities and elapsed phase;
 * restore rebinds the functions and rebases that phase on the new host clock.
 */
typedef struct softpc_device_pit_counter_state {
    int32_t mode, bcd, read_load;
    uint32_t state, state_prior_wait, state_on_gate;
    uint32_t action_on_wait_complete, action_on_gate_enabled;
    uint8_t output_lsb, output_msb, latch_value_lsb, latch_value_msb;
    uint8_t latch_status;
    uint32_t initial_count;
    int32_t read_state, count_latched;
    uint16_t count, new_count, tick_adjust;
    int64_t activation_age_microseconds;
    int32_t terminal_count, freeze_counter;
    uint32_t last_ticks;
    int32_t microtick, time_frig;
    uint16_t saved_count;
    int32_t guesses_per_host_tick, guesses_so_far;
    uint32_t delay;
    int32_t trigger;
    uint16_t gate, clock;
    int32_t waveform_low, waveform_high, waveform_period;
    int32_t waveform_start_level, waveform_repeats;
} softpc_device_pit_counter_state;

typedef struct softpc_device_pit_state {
    softpc_device_pit_counter_state counter[SOFTPC_DEVICE_PIT_COUNTER_COUNT];
    int32_t current_counter;
    int32_t ticks_blocked, timer_interrupt_enabled;
    int32_t time_lock, need_tick;
    int32_t hack_active, too_soon_after_previous, ticks_lost_this_time;
    uint32_t real_mode_ticks_in_a_row;
    uint32_t instructions_per_tick, adjusted_instructions_per_tick;
    uint32_t real_mode_instruction_limit, adjusted_real_mode_tick_limit;
    uint32_t maximum_backlog, more_timer_multiple, timer_multiple_delay;
    int32_t active_interrupt_event;
} softpc_device_pit_state;

int softpc_device_snapshot_capture_pit(softpc_device_pit_state *state);
int softpc_device_snapshot_restore_pit(const softpc_device_pit_state *state);

#endif
