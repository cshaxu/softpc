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
product-neutral host capability actually covered by the imported `host`,
`storage`, or `ux` contract must use that contract: clocks, task/event/wait
mechanics, native presentation mechanics, ordinary owned-file access, and
byte-medium leases. `src/app/` and the non-library portion of `src/host/`
retain SoftPC policy, runtime coordination, device protocols, and
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
`c7b5e668b7d9fc8e8710c3dd77c863ce07801553`. Its `MANIFEST.sha256` file hash
is `C22012E985A30E130369AAC8A5E0C0E93DA9F56C842843A4C5B3685769E3E9A6`.
The review records the public headers, Windows-first platform boundary,
ownership model, and the required unchanged-import rule in the active T41
packet. Linux sources remain in the corpus; Linux runnable acceptance is
deferred, not removed. The corpus now deliberately contains only `base`,
`host`, `storage`, and `ux`; its README and manifest agree with that scope.

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
assembly, package configuration, media topology/geometry, monitor syntax,
product-visible policy, runtime input/command queue, and device endpoint
protocols. Those owners choose when to call lib and translate their own copied
values, but do not recreate lib's generic mechanism. If both products actually
need a missing generic mechanism, it is an NXVM lib change rather than a new
SoftPC-local platform module. NXVM UX deliberately leaves product input-queue
ownership outside lib, so the former `event_queue.c/.h` behavior may be
rehomed under `src/app/` as SoftPC executor coordination.

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
as a required `host`/`storage`/`ux` migration, a SoftPC-specific runtime,
machine/original-host, or device-endpoint boundary, or an upstream lib gap
that both products actually need.

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
policy, runtime queueing, original-host ABI, media geometry, device endpoint
protocols, and machine-specific input/media translation, but may not retain a
second generic implementation. Preserve one fixed SoftPC machine and its
executor-safe boundaries; teach neither lib nor MVDM about SoftPC product
policy.

### S3 — Remove superseded SoftPC implementations

Delete every superseded old platform implementation, including the complete
former `src/lib/platform/win32/` corpus and any direct local wrapper that S2
has replaced. Delete only after dual-width focused and full-regression proof.
Keep a compact deletion/retention ledger: every remaining non-library file
must be explained by a real SoftPC runtime, machine/original-host, product, or
device-endpoint responsibility, not historical platform placement. A missing
shared generic facility that both products need blocks this step; a
product-specific queue or endpoint remains locally owned rather than forcing
an over-broad lib API.

### S4 — Freeze synchronization and source boundaries

Add a source-hash manifest and boundary checks proving the imported `src/lib/`
content remains byte-identical to its approved NXVM review revision. Document
the local update procedure: a future NXVM library update is a new reviewed,
atomic import, never an in-place SoftPC edit to library files.

### S5 — Console ownership contract and upstream UX redesign

**Admission.** The owner added this subtask after runtime acceptance found
that an experimental SoftPC-side monitor/frontend handoff was unreliable. Its
uncommitted source is withdrawn from the working tree; it remains only in a
recoverable Git stash for forensic comparison and is not a candidate
implementation. S5 is a design and upstream-alignment subtask. It does not
authorize a local `src/lib/` edit or an MVDM edit.

**Problem.** A Windows process Console is one input resource. The existing
NXVM Win32 runner's private atomic flag is acquired only while its Console
runner exists, and `ux_run()` unconditionally starts that runner before it
considers the presenter target. Neither behavior expresses whether a Window
target keeps the Console for the presenter session or releases it to a
product's cooked command UI. A product must not infer that fact from a target
request, a frame, or a timing delay.

**Required public model.** The next reviewed NXVM UX contract needs one
immutable Boolean in `ux_binding`, fixed at `ux_run()` startup:

```text
retain_console_while_window = true
    the presenter owns the process Console from ux_run entry until ux_run
    returns, including while its active surface is Window.

retain_console_while_window = false
    the presenter owns the process Console only while its active surface is
    Console; a Window surface releases it.
```

The native Console runner's raw input mode and event reader exist only while
the active surface is Console. A `SESSION` lease held during Window is a
logical reservation, not a hidden raw reader. On a transition to Console the
native owner must first acquire the presenter lease, configure raw input, and
then report Console active. On a transition away it must stop its reader,
restore the saved mode, release the lease when the selected scope requires it,
and only then report the resulting active state. `ux_run()` must capture the
initial target before it creates a Console runner.

The public binding must report completed facts, not requested intent: active
surface (`NONE`, `WINDOW`, `CONSOLE`) and whether the presenter currently owns
the Console. It must not expose Win32 handles or encode SoftPC lifecycle,
commands, or guest hotkeys. A callback or copied-state/event interface is
acceptable, but it must be non-reentrant and documented as an observation of
completed native ownership transfer.

**Product boundary.** SoftPC sets this Boolean; lib does not name or infer a
product mode. When it is true, the original simple monitor model applies: the
monitor has no reader between `ux_run()` entry and return. When it is false,
SoftPC may run its monitor while Window is active, but it may request a Console
target only after its cooked reader has stopped, joined, and released its
Console lease. It may start that reader only after lib reports that Window is
active and the presenter lease is released. The generic lib must supply the
lease and cooked/raw platform transition required to make this exclusive; it
must not leave independent `stdin` and `CONIN$` readers to race over the same
process Console. Parsing `start`, `pause`, `resume`, `reset`, and `stop`
remains SoftPC product code.

**NXVM alignment and gate.** S5 records a clean NXVM revision and manifest,
then reviews that exact corpus against this contract. If NXVM adopts the
generic lease capability, SoftPC imports the resulting whole `src/lib/`
corpus atomically and byte-identically; it does not carry a local UX fork.
The same Boolean contract must have a Linux-compatible public shape, though
Linux runnable parity remains deferred.

**Exit:** an approved, minimal ownership/state table; an exact public API and
native transition order; a clean upstream revision/manifest implementing it;
and a SoftPC audit proving that the withdrawn coordinator/global-`stdin`
approach is absent. Follow-on implementation requires focused ownership tests,
dual-width builds, and owner runtime acceptance before it can replace the
current package behavior.

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
