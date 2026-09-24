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
SoftPC:  src/core/softpc.new @ 07c53c9c
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
