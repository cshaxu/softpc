#ifndef SOFTPC_STANDALONE_XMS_HOST_H
#define SOFTPC_STANDALONE_XMS_HOST_H

/* This is the complete standalone host boundary for the restored XMS
   library.  It intentionally exposes only guest physical-memory access and
   unavailable UMB reservation; no VDM address mapping or NTVDM session state
   crosses this boundary. */
#include "insignia.h"
#include "host_def.h"
#include "cpu4.h"
#include "sas.h"

/* Original xms.486 emits NT kernel diagnostics and assertions.  Standalone
   uses its normal build/test diagnostics instead; these calls carry no guest
   state and must not create an NTVDM logging dependency. */
#ifndef ASSERT
#define ASSERT(condition) ((void)0)
#endif
#ifdef DbgPrint
#undef DbgPrint
#endif
#define DbgPrint(...) 0

#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS ((NTSTATUS)0)
#endif

PVOID softpc_xms_guest_pointer(USHORT segment, USHORT offset);
PVOID softpc_xms_physical_pointer(ULONG address, ULONG bytes);
BOOL softpc_xms_copy_physical(ULONG destination, ULONG source, ULONG bytes);
BOOL ReserveUMB(WORD owner, PVOID *address, DWORD *size);
BOOL ReleaseUMB(WORD owner, PVOID address, DWORD size);

#define GetVDMAddr(segment, offset) softpc_xms_guest_pointer((segment), (offset))

#endif
