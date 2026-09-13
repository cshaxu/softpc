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
common/ui      shared monitor/raw-Console/Window presentation composition
```

`common/ui` is intentionally broader than `lib/kvm-*`: it owns the cooked
monitor Console, raw VM Console, Window, and their broker handoff. It is the
sole common caller of `lib/console`, `lib/host`, and `lib/kvm-*` for this
presentation path. Product command words and policy remain outside it.

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

NXVM `common` is comparison material, not this task's source authority. Its
`common/ui` ownership is the intended simple shape: Session decides the next
surface action, while UI owns monitor/raw Console/Window objects and performs
broker replacement. Its stale CMake component names and any SoftPC behavioral
gap are design inputs for the SoftPC extraction, not a license to import a
behavior-changing variant. The final SoftPC `common/ui` public contract,
manifest, and tests are what NXVM must adopt unchanged.

## Proposed serial S breakdown

1. **S1: frozen SoftPC behavior and extraction ledger.** Treat current
   Window/Console/lifecycle behavior and its tests as the non-regression
   specification. Record every `app/`/`host/` responsibility as common,
   product policy, or immutable MVDM adapter; use NXVM common only as a design
   comparison.
2. **S2: extract common UI.** Move the existing accepted monitor/raw Console,
   Window, and broker composition into `common/ui` without changing calls,
   queues, completion ordering, or tests. Session continues to derive actions;
   UI alone owns the objects and applies them. UI alone invokes the relevant
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
- Dependency gates prove shared control/UI product source does not call a lib
  component directly and no common source depends on MVDM or
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
