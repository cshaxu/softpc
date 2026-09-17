#include "archive.h"
#include "snapshot.h"

#include <stdlib.h>

#include "insignia.h"
#include "host_def.h"
#include "quick_ev.h"

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
    if (!softpc_device_snapshot_restore_serial_controller(
            &archive->serial_controller) ||
        !softpc_device_snapshot_restore_serial_host(&archive->serial_host) ||
        !softpc_device_snapshot_restore_parallel_controller(
            &archive->parallel_controller) ||
        !softpc_device_snapshot_restore_parallel_host(&archive->parallel_host))
        return FALSE;
    return TRUE;
}
