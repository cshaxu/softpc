# M9 T87 S1: repository LF convergence

## Objective

Make LF the one repository spelling for all ordinary tracked text: source,
tests, CMake, tools and documentation.  Remove existing CRLF and mixed-EOL
drift, then enforce the rule through `.gitattributes`.

## Preserved exceptions

`src/app-softpc/softpc.new/` is a selected OpenNT comparison mirror.  Its
existing bytes, including legacy CRLF and mixed historical files, are source
evidence and remain untouched.  `assets/binary/softpc.ini` is user-owned
package configuration and likewise remains byte-for-byte untouched.

## Verification

Enumerate every tracked non-binary text file.  Outside those two explicit
exceptions, require zero CR bytes; confirm the six shared corpus roots remain
LF and byte-identical to NXVM; run the documentation governance gate and a
normal configure/build sanity check only if the normalization changes build
input bytes.

## Delivered evidence

- Before normalization, 61 ordinary tracked text paths had CRLF or mixed
  working-tree bytes: 13 documentation/evidence paths, 29 App production
  paths, 17 App test paths and two test/check scripts.
- After normalization, every ordinary tracked non-binary text path has zero
  CR bytes.  The only CRLF checkout exceptions are
  `src/app-softpc/softpc.new/**` (the preserved OpenNT mirror) and
  `assets/binary/softpc.ini` (user-owned package configuration).
- The six shared roots remain raw-byte identical to NXVM:
  `src/lib` 109 files, `test/lib` 49, `src/emulator` 30,
  `test/emulator` 22, `src/x86` 24 and `test/x86` 12.
- No App production or test semantic diff was introduced; the executor commit
  is `1298dd77`. Documentation governance passes before and after closure.
