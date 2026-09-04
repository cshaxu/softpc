# M9 T23: Unused MVDM Source Retirement

## Status

Closed on 2026-09-04.

## Outcome

The selected SoftPC mirror no longer retains inactive historical source solely
because it existed in the original product tree. This task removed 294 exact
paths from `src/mvdm/softpc.new/`:

- 15 unselected C files (4,921 lines), including obsolete EVID, GFI media,
  product startup, CMOS/event, and duplicate CCPU/C-VID variants;
- 279 headers: inactive C-VID generated-rule variants, unused base interfaces,
  NT/product host declarations, and MIPS-only host/profile variants.

The admission manifest remains in the retained proposal. Every removed C path
was absent from CMake selection, and each removed header was absent from the
fresh x64 and x86 compiler dependency outputs using its exact mirror-relative
path. No active original source was edited.

## Verification

- Fresh GCC MinGW x64 build and full CTest: 20/20 passed.
- Fresh GCC MinGW x86 build and full CTest: 20/20 passed.
- Both builds refreshed `artifacts/binary/softpc32.exe` and
  `artifacts/binary/softpc64.exe`; package smoke passed for both.
- Source-boundary and documentation-governance gates passed for both builds.
