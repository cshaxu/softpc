# M9: NXVM Library Adoption And SoftPC Binding

## Purpose

Replace SoftPC's current `src/lib/` directory in full with the reviewed NXVM
`lib/` directory. The approved NXVM library content is imported without source
modification, at a recorded upstream revision and content hash. It is not
merged with, supplemented by, or locally edited beside the present SoftPC
library. SoftPC then binds its fixed machine to that local library and removes
duplicative SoftPC code wherever the imported library owns the same capability.

This is a local source import, not a cross-repository build, runtime, or
acceptance dependency. NXVM remains read-only comparison material; after an
approved import, SoftPC builds solely from its own checked-in sources.

The desired source responsibility is:

```text
src/mvdm/softpc.new/  selected SoftPC machine and its narrow porting diffs
src/lib/              complete, unmodified approved NXVM library content
src/host/             SoftPC-specific host and original-host compatibility
src/app/              minimal SoftPC binding, monitor, and product policy
```

The objective is not merely to add a library dependency. It is to make
`src/app/` and the non-library portion of `src/host/` as small as the fixed
SoftPC machine boundary permits, deleting or moving duplicated mechanisms
only after the imported library supplies their required behavior. Any current
SoftPC `src/lib/` code is either deleted when the NXVM replacement covers it,
or moved out of `src/lib/` only when the ownership audit proves it is an
irreducibly SoftPC-specific host or product binding.

## Admission Record

M9 T41 S1 is admitted against the reviewed complete corpus
`O:/repos.hobby/nxvm/src/lib` at
`cb3ce7b6b46795e000ad25983b7977387331ed9e`. Its `MANIFEST.sha256` file hash
is `F6970E6A5D300B7D797D59F18028EB3074EBE4FD0729ED983E74BB9CAD47E625`.
The review records the public headers, Windows-first platform boundary,
ownership model, and the required unchanged-import rule in the active T41
packet. Linux sources remain in the corpus; Linux runnable acceptance is
deferred, not removed. The corpus now deliberately contains only `base`,
`host`, `storage`, and `ux`; the upstream README must be reconciled before the
import because it still describes the retired `session` and `observability`
roots.

No SoftPC task may guess that selection, selectively copy a library subset,
retain part of the current SoftPC `src/lib/` beside it, or modify imported
NXVM source to satisfy a SoftPC boundary.

## Required Boundary

The imported library owns only the capabilities it already exposes. SoftPC
retains ownership of its selected original machine, original-host ABI, fixed
machine assembly, package configuration, media contract, monitor syntax, and
product-visible policy unless the approved NXVM library explicitly owns an
equivalent generic mechanism.

SoftPC binding code may adapt copied values and opaque handles, but must not
fork or locally reimplement an imported-library capability. A proposed removal
is valid only when the replacement ownership, input/output contract, and
x86/x64 evidence are explicit.

## Non-goals

- Do not modify `src/mvdm/softpc.new` except through separately admitted,
  narrow portability-diff work.
- Do not modify the imported NXVM library source after import.
- Do not introduce an NXVM runtime dependency, session catalog, profile
  system, debugger, asset loader, product console, or multi-machine policy
  into SoftPC.
- Do not replace SoftPC CPU, C-VID, controller, BIOS, ROM, BOP, timer, media,
  or original renderer behavior.
- Do not delete a SoftPC owner merely because it has a similar name; prove the
  imported implementation is a contract-compatible replacement first.

## Serial Subtasks

### S1 — Verbatim import and duplicate-ownership audit

After the admission prerequisite is met, replace `src/lib/` atomically with
the approved NXVM `lib/` scope byte-for-byte and record its source revision
and content hash. Do not edit imported files or retain a current SoftPC
library file in that directory. Produce a complete ownership ledger for every
active non-`mvdm` SoftPC source file, including every displaced current library
file: retained SoftPC binding/host responsibility outside `src/lib/`,
imported-library equivalent, candidate deletion, or blocked semantic mismatch.

The audit must identify build-system and public-header adaptation needed to
compile the imported library locally without altering its source. It must also
record whether each candidate is shared mechanical lifecycle/presentation/
queue functionality or an irreducibly SoftPC-specific machine/host boundary.

**Exit:** `src/lib/` is exactly the approved NXVM library import and is
hash-verifiable; every displaced current SoftPC library file and every other
active SoftPC non-library owner has a disposition; no deletion or functional
routing change has occurred without a proved replacement.

### S2 — Bind SoftPC to imported library capabilities

Replace approved duplicate mechanisms with the imported library through small,
explicit SoftPC binding adapters. Preserve one fixed SoftPC machine and its
proven executor-safe boundaries. Move product policy, original-host ABI, and
machine-specific translation out of the imported library rather than teaching
the library about SoftPC.

### S3 — Remove superseded SoftPC implementations

Delete only code whose ownership is demonstrably supplied by the imported
library and whose binding has dual-width focused and full-regression proof.
Keep a compact deletion/retention ledger so the remaining non-library code is
explained by a real SoftPC responsibility rather than historical placement.

### S4 — Freeze synchronization and source boundaries

Add a source-hash manifest and boundary checks proving the imported `src/lib/`
content remains byte-identical to its approved NXVM review revision. Document
the local update procedure: a future NXVM library update is a new reviewed,
atomic import, never an in-place SoftPC edit to library files.

## Verification

Each admitted implementation subtask must preserve local, independent SoftPC
builds and run focused tests for every replaced capability. Runnable changes
require clean GCC x64 and i686 x86 builds, full CTest, and both package smokes
without modifying user-owned `assets/binary/softpc.ini` or guest media.

## Stop Conditions

Stop for owner direction if the NXVM review does not provide a complete
unchanged import scope; an imported interface would require SoftPC-specific
source changes; a proposed deletion affects a machine/original-host contract;
or an x86/x64 behavior difference cannot be attributed to an explicit binding
boundary.
