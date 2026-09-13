# M9 candidate: common corpus convergence

## Objective

Extract SoftPC's currently accepted app/host orchestration into a shared
`src/common/` corpus without changing its product behavior. SoftPC is the
initial canonical owner of that corpus because its Window/Console/lifecycle
experience is the accepted reference. NXVM adopts the resulting common corpus
unchanged only after SoftPC's full behavior matrix remains green.

The common corpus is the only consumer of `src/lib/` for shared control,
Console, and KVM composition. Each product retains only its machine adapter,
product policy, and entry composition.

The intended common components are:

```text
common/xasm32  shared assembler/disassembler
common/debug   shared debugger command/runtime layer
common/machine shared ordered machine-adapter boundary
common/session shared reducer/control lifecycle
common/kvm     shared Window/raw-Console presentation composition
common/console shared cooked-Console/broker composition
```

`common/kvm` is the accurate successor to the KVM part of NXVM's current
`common/ui`: it owns keyboard/video/mouse presentation composition, not the
cooked product monitor. `common/console` is the neutral common Console/broker
owner required by the "common alone calls lib" boundary; product command words
and policy remain outside it.

## Frozen principles

- Extract common source from the current accepted SoftPC behavior; do not first
  replace it with NXVM's incomplete semantic variant. Once accepted, this
  corpus becomes the byte-identical import source for NXVM.
- `lib` is called only by `common`. `app`, the SoftPC machine adapter, MVDM,
  and product entry code cannot include or link a lib component directly.
- `src/mvdm/softpc.new/` remains unmodified. The SoftPC-specific VM adapter is
  the sole bridge between common machine requests/events and MVDM execution.
- Product configuration, monitor vocabulary, media selection, and executable
  composition remain product-owned. Common accepts injected callbacks/contracts
  and does not name SoftPC or NXVM.
- Do not create compatibility wrappers, dual control loops, or parallel
  presentation paths. Once a common component has adopted a responsibility,
  delete the superseded SoftPC-local implementation in the same admitted
  change.

## Baseline and upstream disposition

NXVM `common` is comparison material, not this task's source authority. It
already exposes useful machine/session/KVM concepts, but its current
`common/ui` also owns cooked Console behavior and its CMake still carries old
component targets. Those are design inputs for the SoftPC extraction, not a
license to import a behavior-changing mixed component. The final SoftPC
`common/kvm`/`common/console` split, public contracts, manifest, and tests are
what NXVM must adopt unchanged.

## Proposed serial S breakdown

1. **S1: frozen SoftPC behavior and extraction ledger.** Treat current
   Window/Console/lifecycle behavior and its tests as the non-regression
   specification. Record every `app/`/`host/` responsibility as common,
   product policy, or immutable MVDM adapter; use NXVM common only as a design
   comparison.
2. **S2: extract common console and KVM.** Move the existing accepted broker,
   cooked Console composition, and KVM composition into separate
   `common/console` and `common/kvm` components without changing calls,
   queues, completion ordering, or tests. Common alone invokes the relevant
   lib components; delete the replaced app implementation in the same change.
3. **S3: extract common session.** Move the existing central control/reducer
   into `common/session`, retaining SoftPC's product CLI provider, state
   messages, configuration, and MVDM callbacks as injected product policy.
   Prove the full state/presentation matrix remains unchanged.
4. **S4: extract common machine.** Move the generic ordered request/event
   boundary into `common/machine`; retain one SoftPC MVDM executor adapter for
   start/reset/frame/safe-point behavior. MVDM remains untouched.
5. **S5: import dormant common debug and xasm32.** After common boundaries are
   stable, import the original common `xasm32` and `debug` corpus into the
   SoftPC canonical tree. Build and test them, but do not expose a debugger
   product path until the SoftPC machine adapter implements its full contract.
6. **S6: canonicalization and NXVM adoption handoff.** Remove duplicate
   product-local common capabilities, enforce the intended dependency gates,
   freeze a SoftPC common manifest and exact corpus tests, then send that
   canonical revision to NXVM for unchanged adoption.

## Verification and exit criteria

- A frozen behavior ledger and complete before/after state/presentation matrix
  prove the extraction does not change accepted SoftPC experience.
- A manifest and static diff gate prove the final SoftPC `src/common/` corpus
  is self-contained and is the exact revision offered to NXVM.
- Dependency gates prove shared control/Console/KVM product source does not
  call a lib component directly and no common source depends on MVDM or
  product types.
- Machine request, lifecycle, monitor-to-raw-Console, raw-to-monitor, KVM
  input, frame, pause/resume/reset/stop, and source-retirement matrix tests
  pass through common boundaries.
- Fresh x86 and x64 package builds/full CTest pass; owner accepts runtime
  package behavior before task closure.
- The completion ledger accounts for every replaced `app/`/`host/` production
  path: deleted, retained product adapter, or moved into common.

## Non-goals

No MVDM source change, guest-media/INI mutation, Linux parity expansion,
debugger product UX rollout, new emulator feature, NXVM-first source import,
or long-lived parallel common implementation. The next numeric T is allocated
only when the owner admits this candidate.
