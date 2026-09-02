# M5 Original Illegal-Driver BOP Restoration

## Boundary

`base/system/illegalp.c` is original SoftPC firmware/error-path code. Its
incompatible-driver BOP reports through `host_error` and clears carry. It is
not a DOS, WOW, VDD, CSR or session service, and it does not alter BOP
instruction decoding or selector routing.

The historical build searched the base error-code header before the NT host
header. The standalone target otherwise finds `host/inc/error.h` first, which
hides the original `EG_DRIVER_MISMATCH` declaration. The source-specific
`softpc-port-abi/illegalp/error.h` include overlay restores only that original
base declaration lookup. It creates no error policy or machine-state owner.

## Resolution

The `SOFTPC_STANDALONE` suppression was removed from `illegalp.c`; after
line-ending normalization it is equal to the selected original SoftPC source.
The existing finite standalone `host_error` endpoint remains its only host
capability.

## Verification

- `softpc-bop-smoke` directly invokes the restored original handler with CF
  set and requires its original carry-clear result.
- The focused BOP smoke passed on x86 Clang and x64 MinGW.
- Serial CTest passed `18/18` on both host widths.
