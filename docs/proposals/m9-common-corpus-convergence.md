# M9 candidate: common corpus convergence

## Objective

Replace SoftPC's product-local `src/app/` and `src/host/` orchestration with a
shared `src/common/` corpus that is byte-identical in SoftPC and NXVM wherever
both products need the capability. The common corpus is the only consumer of
`src/lib/`; each product retains only its machine adapter, product policy, and
entry composition.

The intended common components are:

```text
common/xasm32  shared assembler/disassembler
common/debug   shared debugger command/runtime layer
common/machine shared ordered machine-adapter boundary
common/session shared reducer/control lifecycle
common/kvm     shared Window/raw-Console presentation composition
```

`common/kvm` is the accurate successor to NXVM's current `common/ui`: it owns
keyboard/video/mouse presentation composition, not the cooked product monitor.

## Frozen principles

- Import shared common source from NXVM as a corpus, not as copied ideas or a
  SoftPC-specific rewrite. Every retained common file must match the approved
  NXVM revision byte-for-byte.
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

## Known prerequisite

NXVM's current `common` still includes its former `ui-*` component names,
while canonical lib names are now `kvm-*`. Before a common import is admitted,
NXVM must make the equivalent common rename (`common/ui` to `common/kvm`) and
update its lib dependencies. SoftPC must import that approved common revision
unchanged; it must not manufacture a SoftPC-only shim.

## Proposed serial S breakdown

1. **S1: frozen common/adoption audit.** Compare every SoftPC app/host source
   with NXVM `common/{machine,session,ui,xasm32,debug}`; record exact shared,
   product-only, and adapter-only responsibilities. Freeze the upstream common
   manifest and verify the prerequisite KVM-name alignment.
2. **S2: exact independent common import.** Import `common/xasm32` and
   `common/debug` unchanged with their common CMake targets, manifest, and
   standalone corpus verifier. Do not yet make a product command path depend
   on debug.
3. **S3: common machine boundary.** Import `common/machine` unchanged and
   implement one SoftPC machine adapter outside common. Route all machine
   request/event traffic through it; MVDM remains untouched.
4. **S4: common KVM composition.** Import `common/kvm` unchanged and delete
   SoftPC-local presentation/Console composition it replaces. Common KVM alone
   invokes `kvm-window`, `kvm-console`, `kvm-base`, Console, and Host.
5. **S5: common session reducer.** Import `common/session` unchanged and move
   the central control/reconciler logic into it. Keep SoftPC monitor command
   policy and VM adapter callbacks outside common.
6. **S6: deletion, parity, and closure audit.** Remove duplicate app/host
   capabilities, enforce `app -> common -> lib` and `adapter -> common`
   boundaries, compare SoftPC/NXVM common manifests, and run focused plus
   fresh x86/x64 package regressions.

## Verification and exit criteria

- A manifest and static diff gate prove that imported `src/common/` is exactly
  the approved NXVM common corpus.
- Dependency gates prove no product source calls a lib component directly and
  no common source depends on MVDM or product types.
- Machine request, lifecycle, monitor-to-raw-Console, raw-to-monitor, KVM
  input, frame, pause/resume/reset/stop, and source-retirement matrix tests
  pass through common boundaries.
- Fresh x86 and x64 package builds/full CTest pass; owner accepts runtime
  package behavior before task closure.
- The completion ledger accounts for every replaced `app/`/`host/` production
  path: deleted, retained product adapter, or moved into common.

## Non-goals

No MVDM source change, guest-media/INI mutation, Linux parity expansion,
debugger product UX rollout, new emulator feature, or softpc-only common fork.
The next numeric T is allocated only when the owner admits this candidate.
