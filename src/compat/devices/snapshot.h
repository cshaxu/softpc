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
    SOFTPC_DEVICE_PIT_COUNTER_COUNT = 3
};

enum {
    SOFTPC_DEVICE_QUEUE_TIMER_CLEAR_TOO_SOON = 1,
    SOFTPC_DEVICE_QUEUE_TIMER_MULTIPLE_INTERRUPTS,
    SOFTPC_DEVICE_QUEUE_RTC_PERIODIC,
    SOFTPC_DEVICE_QUEUE_RTC_ALARM,
    SOFTPC_DEVICE_QUEUE_RTC_SYNC,
    SOFTPC_DEVICE_QUEUE_FDC_INTERRUPT,
    SOFTPC_DEVICE_QUEUE_HDD_PAUSE,
    SOFTPC_DEVICE_QUEUE_HDD_INTERRUPT
};

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
