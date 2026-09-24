#include "../../lib/types/types_interface.h"
#ifndef SOFTPC_PLATFORM_H
#define SOFTPC_PLATFORM_H

#include "lib/storage/medium_interface.h"

/* Host support used by the machine bootstrap and VM adapter. Implementations
 * include this contract too; it does not own CPU/device state. */
int softpc_platform_write_physical(unsigned long address,
    const unsigned char *bytes, unsigned long length);
int softpc_platform_read_physical(unsigned long address,
    unsigned char *bytes, unsigned long length);
void softpc_device_bop_register_machine_services(void);
int softpc_platform_keyboard_scancode(unsigned char scan_code);
int softpc_platform_keyboard_key(int key, int released);
void softpc_platform_keyboard_discard_stale_output(void);
void softpc_platform_request_executor_wake(void);
int softpc_platform_executor_ready(void);
void softpc_platform_presentation_request_refresh(void);
void softpc_platform_set_boot_clock(int active);
/* Executor-owned capture barrier. Disabling joins the producer without
 * consuming pending ticks or closing the executor wake event. Enable before
 * resuming execution; this does not advance any original device clock. */
int softpc_platform_set_clock_running(int running);
void softpc_platform_set_runtime_heartbeat(int enabled);
void softpc_platform_set_executor_callback(void (*callback)(void *),
    void *context);
int softpc_host_com_set_output_path(int adapter, const char *path);
int softpc_host_lpt_set_output_path(int adapter, const char *path);
int softpc_platform_hdd_attach(const char *hard_disk_path,
    lib_storage_medium_mode mode);
void softpc_platform_hdd_detach(void);
int softpc_platform_floppy_attach(const char *path, lib_storage_medium_mode mode);
void softpc_platform_floppy_detach(void);
int softpc_platform_video_buffers_init(void);
void softpc_platform_bind_reset_host_functions(void);
void softpc_platform_executor_event(void);
char *softpc_platform_floppy_config_value(void);
int softpc_platform_presentation_is_graphics(void);
int softpc_platform_presentation_state(lib_u32 *, lib_u32 *);
int softpc_platform_presentation_text_extent(unsigned long *, unsigned long *);
int softpc_platform_presentation_cursor(long *, long *,
    unsigned long *);
int softpc_platform_presentation_fonts(lib_u8 *, lib_u8 *,
    unsigned long *, unsigned long *);

#endif
