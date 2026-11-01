#include "lib/types/types_interface.h"
#include "mirror_types.h"
#include "platform.h"
#include "devices/snapshot.h"
#include "cvidc/gdp_state.h"

#ifdef const
#error "Mirror types must not leak the legacy const macro"
#endif

_Static_assert(_Generic(&softpc_platform_floppy_attach,
    int (*)(const char *, lib_storage_medium_mode): 1, default: 0),
    "Floppy path must remain const-qualified");
_Static_assert(_Generic(&softpc_device_snapshot_restore_floppy_host,
    int (*)(const softpc_device_floppy_host_state *): 1, default: 0),
    "Snapshot input must remain const-qualified");
_Static_assert(_Generic(&softpc_gdp_slot,
    void *(*)(const void *, lib_u32, lib_size): 1, default: 0),
    "GDP state input must remain const-qualified");

int main(void) { return 0; }
