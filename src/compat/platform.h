#ifndef SOFTPC_PLATFORM_H
#define SOFTPC_PLATFORM_H

#include "machine.h"

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
void softpc_platform_set_runtime_heartbeat(int enabled);
void softpc_platform_set_executor_callback(void (*callback)(void *),
    void *context);
int softpc_host_com_set_output_path(int adapter, const char *path);
int softpc_host_lpt_set_output_path(int adapter, const char *path);
int softpc_platform_hdd_attach(const char *hard_disk_path,
    softpc_media_mode mode);
void softpc_platform_hdd_detach(void);
int softpc_platform_floppy_attach(const char *path, softpc_media_mode mode);
void softpc_platform_floppy_detach(void);
int softpc_platform_video_buffers_init(void);
void softpc_platform_bind_reset_host_functions(void);
void softpc_platform_install_timer2_sound_gate(void);
int softpc_platform_debug_memory(unsigned long address,
    unsigned char *data, unsigned long bytes, int write);
void softpc_platform_executor_event(void);
char *softpc_platform_floppy_config_value(void);
int softpc_platform_presentation_is_graphics(void);
int softpc_platform_presentation_state(uint32_t *, uint32_t *);
int softpc_platform_presentation_cursor(long *, long *,
    unsigned long *);
int softpc_platform_presentation_font(uint8_t *, unsigned long *);
int softpc_platform_presentation_fonts(uint8_t *, uint8_t *,
    unsigned long *, unsigned long *);

#endif
