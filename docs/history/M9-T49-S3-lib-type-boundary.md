# M9 T49 S3 — Shared-library type boundary

## Objective

Make the public base contract expose only the C definitions it actually owns,
and make library-owned storage/UX scalar state use the neutral `lib_*` aliases
instead of direct standard fixed-width types.

## Boundaries

- `src/lib/**`, its manifest, and focused library checks may change.
- Application, outer host, and preserved MVDM sources do not change.
- Native SDK/POSIX types remain valid at private adapter call boundaries.

## Required work

1. Reduce `base_interface.h` to `<stddef.h>` and `<stdint.h>`; move
   `stdio.h`, `stdlib.h`, `string.h`, and `stdatomic.h` to direct private
   consumers.
2. Replace library-owned `size_t`, `uint32_t`, and `int64_t` fields,
   parameters, locals, and conversions with `lib_size`, `lib_u32`, and
   `lib_i64` throughout the admitted lib sweep.
3. Do not replace SDK-required values such as `DWORD`, `HANDLE`, or platform
   function signatures merely to make a textual search empty.

## Verification and exit

Private consumers compile without transitive base includes; the public base
header has no accidental implementation headers; library-owned scalar state is
neutral; x64/x86 tests and the manifest pass without ABI or behavior changes.

## Completion evidence

- `base_interface.h` now contains only `<stddef.h>` and `<stdint.h>`; every
  private consumer declares its own standard-library dependency.
- The corpus search leaves direct standard scalar types only at the base alias
  definitions. Library-owned values now use `lib_u8`/`lib_u32`/`lib_u64`,
  `lib_i32`/`lib_i64`, and `lib_size`.
- The standalone strict-lib test and root x64/x86 CTest each passed 34/34.
  `d16331c` contains the implementation and refreshed package pair.
