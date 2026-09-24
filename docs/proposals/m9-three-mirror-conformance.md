# M9: Three-Mirror Conformance

## Purpose

Use the selected OpenNT `softpc.new` mirror and NTVDM64's corresponding
read-only mirror as comparison material to reduce SoftPC's unnecessary source
divergence without changing guest behavior or importing either product.

The first admitted scope is deliberately mechanical: restore OpenNT CRLF bytes
for files whose content is otherwise byte-for-byte identical after line-ending
normalization.  It is not a code cleanup, a formatting policy change, or a
vehicle for copying NTVDM64 fixes.

## Frozen comparison roots

```text
SoftPC:  src/app-softpc/softpc.new @ 07c53c9c
NTVDM64: O:/repos.hobby/ntvdm64/src/mvdm/softpc.new @ 52e330bdf
OpenNT:  O:/repos.external/opennt-src-2/nt/private/mvdm/softpc.new @ 5e4619ab6
```

The two external trees are read-only comparison material.  SoftPC retains a
selected 498-path subset of OpenNT's 1,148-path tree; unselected paths are
outside the scope of a local-diff count.

## S1 — Restore content-identical CRLF mirror bytes

The frozen S1 universe is exactly these twelve SoftPC retained files:

```text
base/ccpu386/c_bsic.c
base/ccpu386/c_prot.c
base/ccpu386/c_seg.c
base/ccpu386/c_tsksw.c
base/ccpu386/in.c
base/ccpu386/jmp.c
base/ccpu386/mov.c
base/ccpu386/out.c
base/inc/egavideo.h
base/inc/gfx_upd.h
base/system/illegalp.c
base/video/ega_vide.c
```

For each path, OpenNT and SoftPC have equal bytes after CRLF/LF normalization
and unequal raw SHA-256 values.  S1 restores only the OpenNT raw bytes and
adds one exact CRLF checkout rule per frozen path to `.gitattributes`; Git
stores text canonically, so this rule is what makes the byte repair durable
across clones rather than a local `core.autocrlf` accident.  It verifies raw
equality for every listed path and proves the list is exhausted for this
comparison rule.  It must not alter a token, whitespace other than line
endings, source selection, behavior, declarations, tests, external source, or
package configuration/media.

## Deferred scope

The three-way audit found 72 SoftPC-only semantic differences and ten
NTVDM64-only differences in SoftPC's selected path set.  They remain outside
S1.  A later admitted subtask must classify each proposed semantic change by
owner, reason and focused proof before altering it; NTVDM64 is never an
automatic patch source.

## Verification

- Compare raw SHA-256 for all twelve paths against OpenNT.
- Re-run the normalized-content enumeration and prove no CRLF-only SoftPC
  difference remains in the selected mirror.
- Run `git diff --check`, mirror audit, documentation governance, clean x64
  and x86 Release builds, and background CTest suites on both widths.
- Refresh only the two package EXEs; preserve the owner INI and all media.

## Stop conditions

Stop if any listed file differs from OpenNT after line-ending normalization,
if raw copying changes a token, if an external file cannot be read as a
baseline, or if any build/test result changes.  Do not compensate with a local
source edit; report the discrepancy instead.

## S1 delivery evidence

- All twelve frozen paths now have raw SHA-256 equality with OpenNT.  A fresh
  enumeration of the selected 498-file corpus finds no remaining SoftPC path
  whose only difference is CRLF versus LF.
- The mirror audit reports 498 retained paths, 388 raw-byte matches, and zero
  missing OpenNT peers.  The remaining 110 differences are the deferred
  semantic or port-ABI divergences; S1 did not alter their membership.
- Production source accounting is `+0/-0` semantic/text lines, net zero.  The
  only source mutation is restoring OpenNT line terminators in the frozen
  twelve files.  Twelve exact `.gitattributes` checkout rules make those raw
  bytes reproducible; documentation adds this proposal and the active task
  packet.
- Clean package builds completed for both host widths.  The resulting assets
  are `softpc32.exe` SHA-256
  `C11ED83C1B5730E852F847B5242957325B0A133194D6864454D1FFC944297551` and
  `softpc64.exe` SHA-256
  `289AD608ED1CE84048450FC1012FD20080E6E966A0E0CFB28B517662CAC64E0B`.
- Background CTest passed 116/116 on x64 (246.40 s) and 116/116 on x86
  (255.17 s).  Documentation governance and `git diff --check` pass.

## S1 P2 — Owner snapshot refresh

After P1, the owner reopened the still-unclosed S1 solely to replace the
tracked package `assets/binary/snapshot` with a newly created state image.
This is a package-fixture update, not a change to the CRLF repair or a new
snapshot-format claim.  The replacement is 18,387,571 bytes with SHA-256
`28A605291D12946D2F775B0220CF033BA47E4660A9D1491DFC78B4B0820D2B8E`;
the preceding tracked snapshot was 18,395,778 bytes.  Its owner-controlled
`softpc.ini`, both EXEs, and every other package payload remain unchanged.

No source, test or executable changes accompany P2, so the P1 dual-width
build and 116/116 background CTest evidence remains the applicable runnable
proof.  P2 verifies its checksum, package-path scope, documentation governance
and `git diff --check`, then waits for owner acceptance with T83 still open.

## Candidate S2 — Multi-application SoftPC source and test layout repair

This is a proposed structural task, not an admitted task and not authority for
the active S1 packet.  Its input is the owner-relocated SoftPC product tree:

```text
src/app-softpc/
  product/
  machine/
  compat/
  softpc.new/
```

The task repairs every product build reference, include spelling, source-owner
gate, manifest path, test registration and documentation reference that still
names the former `src/app/` or `src/core/` layout.  It preserves byte content
of `softpc.new`, keeps the existing target topology and changes no guest,
machine, Common, Lib, x86, configuration or package behavior.

Product tests are organized by the production owner they primarily prove:

```text
test/app-softpc/
  unit/product/
  unit/machine/
  unit/compat/
  unit/softpc.new/
  integration/
```

A test that needs an assembled Product, Machine, Compat, Common or recovered
machine flow belongs in `integration/`; it must not be placed into a unit
directory merely because it happens to include one owner header.  This avoids
the previous misleading `machine` umbrella: Machine, Compat and the recovered
mirror retain independently reviewable unit suites.

Repository-wide static build and architecture gates are not runnable App
behavior tests and do not belong below `test/app-softpc/`.  Following NXVM's
root-build pattern, move the existing product-only CMake scripts from
`test/checks/` to `tools/checks/`; root `CMakeLists.txt` registers them.  The
shared `src/lib`, `src/common`, `src/x86` and matching test packages retain
their own corpus/manifest checks exactly where they are now.

The task must use Git-recognized relocations, remove every former product path
from active root CMake and active static gates, and must not introduce a
compatibility include root, duplicate source tree, forwarding header, or
second target route.  It verifies a clean x64/x86 Release configure/build,
the background CTest presets, root build-ownership and product-boundary
negative cases, the shared six-package corpus checks, and a finite search for
former active path spellings.  The accounting distinguishes owner relocations
from any content edit and proves the recovered mirror's bytes are unchanged.

## Candidate S3 — Central C-VID declaration boundary and `cga.c` conformance

This is a proposed narrow port-ABI task, not an admitted task.  It addresses
the first class of semantic three-mirror difference: OpenNT's historical
unprototyped C-VID declarations, which NTVDM64 repaired locally in multiple
video source files.

The task introduces no wrapper, no new runtime object and no behavior branch.
It selects one existing C-VID interface header as the sole direct declaration
boundary for the already-used selectors:

```c
IMPORT void setReadPointers IPT1(IUH, readset);
IMPORT void setWritePointers IPT0();
```

`base/video/cga.c` then makes the definitions of its two BIOS channel-2 write
pointers exactly match their existing public declarations in `base/inc/video.h`:

```c
GLOBAL VOID (*bios_ch2_byte_wrt_fn)
    IPT2(ULONG, ch_attr, ULONG, ch_addr);
GLOBAL VOID (*bios_ch2_word_wrt_fn)
    IPT2(ULONG, ch_attr, ULONG, ch_addr);
```

The selected C-VID header is included by its current consumers.  No local
copy of the selector declarations is added to `cga.c`; the task must not
create a new generic header solely for two declarations.  The completed state
therefore has one C-VID ABI declaration site, while `video.h` remains the sole
owner of the two global pointer declarations.

The scope is deliberately limited to this `cga.c` conformance repair.  It does
not yet import NTVDM64's mode-handler conditional compilation from
`egawrtm0.c`/`egwrtm12.c`, WOW instrumentation from `c_xfer.c`, the broad CCPU
debug lifecycle patch, or any SAS-video product branch.  Those changes have
different product/port assumptions and require separate proof.

Verification is a direct OpenNT diff showing only the required declaration
correction in `cga.c` plus the selected pre-existing C-VID header; compile the
actual `CPU_40_STYLE + C_VID` source selection for x86 and x64; run focused
video/C-VID smoke coverage and the background product suites on both widths.
The mirror ledger records the exact added/removed lines and why the small
source-visible declaration diff is necessary.  The acceptance condition is
unchanged guest-visible behavior with compiler-checked agreement among the
declaration, definition and assignments of both pointer variables.
