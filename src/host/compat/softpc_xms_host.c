/* Standalone host endpoints used by the original xms.486 library. */
#include "softpc.h"
#include "xms.h"

NTSTATUS xmsCommitBlock(ULONG base_address, ULONG size)
{
    return softpc_xms_physical_pointer(base_address, size) == NULL ?
        (NTSTATUS)-1 : STATUS_SUCCESS;
}

NTSTATUS xmsDecommitBlock(ULONG base_address, ULONG size)
{
    return softpc_xms_physical_pointer(base_address, size) == NULL ?
        (NTSTATUS)-1 : STATUS_SUCCESS;
}

VOID xmsMoveMemory(ULONG destination, ULONG source, ULONG size)
{
    (void)softpc_xms_copy_physical(destination, source, size);
}

/* The selected fixed profile has no standalone UMB manager yet.  Returning
   no range is the original XMS failure contract; it does not fabricate DOS
   memory semantics or introduce NTVDM mapping state. */
BOOL ReserveUMB(WORD owner, PVOID *address, DWORD *size)
{
    UNUSED(owner);
    if (address != NULL) *address = NULL;
    if (size != NULL) *size = 0;
    return FALSE;
}

BOOL ReleaseUMB(WORD owner, PVOID address, DWORD size)
{
    UNUSED(owner);
    UNUSED(address);
    UNUSED(size);
    return FALSE;
}

/* In NTVDM this only maintains an INT 15h keyboard-callout optimization.
   There is no separate monitor dispatcher in the standalone machine. */
VOID UpdateKbdInt15(WORD segment, WORD offset)
{
    UNUSED(segment);
    UNUSED(offset);
}
