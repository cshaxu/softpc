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

The objective is not merely to add a library dependency. Every MVDM-external,
product-neutral host capability covered by the imported `host`, `storage`, or
`ux` contract must use that contract: clocks, task/event/wait mechanics,
native presentation mechanics, file I/O, and byte-medium leases. `src/app/`
and the non-library portion of `src/host/` retain only SoftPC policy and
original-host adaptation around those calls. They must not retain, move, or
recreate a local platform implementation that lib already owns.

Any current SoftPC `src/lib/` code is deleted when the NXVM replacement covers
it. A current local mechanism that has no product-neutral lib equivalent is
not silently moved: S1 must classify it either as an irreducibly
SoftPC-specific machine/original-host boundary or as an upstream lib gap to
be resolved before its duplicate is removed.

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

The imported library owns every product-neutral capability it exposes. SoftPC
must route all non-MVDM use of those capabilities through lib, rather than
through a direct SDK call or an old local wrapper: `host` owns clock and
task/event/wait mechanics, `storage` owns ordinary owned-file access, writers,
and byte-medium leases, and `ux` owns copied frames, host input
normalization, mailboxes, presenters, routing, and capture.

SoftPC retains its selected original machine, original-host ABI, fixed-machine
assembly, package configuration, media topology/geometry, monitor syntax, and
product-visible policy. Those owners choose when to call lib and translate
their own copied values, but do not recreate lib's generic mechanism. If both
products need a missing generic mechanism, it is an NXVM lib change, not a
new SoftPC-local platform module. The current generic input-queue gap is
therefore an upstream prerequisite: do not move the former
`event_queue.c/.h` implementation into `src/app/`.

This boundary starts strictly outside `src/mvdm/softpc.new/`. Any direct Win32
API use or WinNT-derived implementation retained within that preserved MVDM
tree remains there unchanged. T41 neither audits it as a migration candidate
nor modifies it; a separate admitted port-ABI task would be required for any
such change.

SoftPC binding code may adapt copied values and opaque handles, but must not
fork or locally reimplement an imported-library capability. A proposed removal
is valid only when the replacement ownership, input/output contract, and
x86/x64 evidence are explicit.

## Non-goals

- Do not modify `src/mvdm/softpc.new` except through separately admitted,
  narrow portability-diff work.
- Do not migrate, wrap, replace, or otherwise alter any MVDM-local Win32 API
  use or WinNT-derived implementation as part of T41.
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
compile the imported library locally without altering its source. It must
inventory every non-MVDM direct platform/file/media operation and classify it
as a required `host`/`storage`/`ux` migration, an irreducibly
SoftPC-specific machine/original-host boundary, or an upstream lib gap. A
generic input queue is specifically such a gap until it is supplied by lib.

**Exit:** `src/lib/` is exactly the approved NXVM library import and is
hash-verifiable; every displaced current SoftPC library file and every other
active SoftPC non-library platform owner has a disposition; every generic
duplicate has either a named lib replacement or an explicit upstream blocker;
no deletion or functional routing change has occurred without a proved
replacement.

### S2 — Bind SoftPC to imported library capabilities

Migrate every S1-approved generic operation to the imported library: direct
clock/event/task/wait use to `host`; direct ordinary file and media lease use
to `storage`; and copied-frame, input-normalization, mailbox, router,
capture, and native presenter use to `ux`. Bindings may retain SoftPC product
policy, original-host ABI, media geometry, and machine-specific input/media
translation, but may not retain a second generic implementation. Preserve one
fixed SoftPC machine and its executor-safe boundaries; teach neither lib nor
MVDM about SoftPC product policy.

### S3 — Remove superseded SoftPC implementations

Delete every superseded old platform implementation, including the complete
former `src/lib/platform/win32/` corpus and any direct local wrapper that S2
has replaced. Delete only after dual-width focused and full-regression proof.
Keep a compact deletion/retention ledger: every remaining non-library file
must be explained by a real SoftPC machine/original-host/product responsibility,
not historical platform placement. A missing shared generic facility blocks
this step; it is not permission to retain or relocate a local clone.

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
