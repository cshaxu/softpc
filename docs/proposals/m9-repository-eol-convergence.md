# Repository LF convergence

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
