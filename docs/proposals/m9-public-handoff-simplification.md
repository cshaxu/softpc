# Public Handoff Simplification Review

## Admission and scope

Owner initially admitted T98 S16 against 8165664a, then on 2026-10-22 closed
T98 and immediately transferred this unimplemented research to new T99 S1.
CURRENT owns status. No S16 research completion is claimed; scope and evidence
transfer intact.
The aim is fewer competing responsibilities and states, not clearing TODOs
with individual patches. Review the public eight src/test trees: Lib,
Emulator, X86 Debug and X86 IBMPC. Actual App callers and fixtures are evidence
for attribution, not permission to repair App or the preserved core.
Do not read or modify downstream repositories.

This S first researches and reports. No production/API or permanent test repair
is authorized. Temporary probes may exercise existing paths without a second
implementation, guest modifications or Computer Use. Implementation options
require owner approval after the report. No new EXE/build claim accompanies
this documentation admission.

## Frozen observation ledger

These five observations originate in TODO, not five established public bugs.
Owner explicitly transfers all five entries out of TODO into this S. Their
unresolved status and historical evidence are retained below, not marked fixed.
This proposal is their sole investigation record; no duplicate TODO or queued
candidate remains. QUEUE contained none of these five entries; its separate
product-performance candidate is unchanged and outside this admission.

1. Paused key release/source retirement. Session control removes host pressed
   records while PAUSED without delivering release to Machine. Source behavior
   is confirmed; an end-to-end stuck-key reproduction is not. Emulator Session
   is the direct public owner to inspect. Distinguish host observation, FIFO
   acceptance and actual driver delivery. S15's frozen FIFO is accepted and
   is not authority to add pause-release exceptions or restore-time bursts.
2. Command-provider completion wait. T98 S7 observed a 10000ms assertion after
   debug resume/pause in the App integration fixture; unchanged follow-up
   passed. No Window is created. Trace X86 IBMPC command handling, Emulator
   request/completion and actual driver safe points. Identify the exact missing
   completion before blaming RDP, Machine or test event consumption.
3. Console attach/prompt failure. Package tests have observed empty live
   buffers, missing prompts and machine-composition exits, stages 1/error 6
   and 3/error 0. Some occurred while Active. A 30-second startup budget fixed
   one observed slow-prompt case, not all failures. Inspect Lib Console/Broker,
   Emulator Product and App fixture boundaries without assuming one root cause.
4. Compact Console input loss. T93 S12 reached DOS but failed stage 14/error 0
   after injected ver; unchanged rerun passed. Distinguish native injection,
   Broker read, KVM decoding, Session/FIFO acceptance and guest consumption.
   Startup failure and post-start input loss are separate observations.
5. Native child initialization failure. Earlier package probes observed
   0xC0000142, AttachConsole error 5 and subsequent error 6. This may explain
   a subset of observation 3, but the responsible DLL/path is unproved.
   Investigate jointly with startup ordering; do not treat downstream Console
   errors after child death as proof of a Console implementation defect.

All five require a disposition: confirmed public defect, removable redundancy,
necessary behavior retained, product/fixture/host-owned, or unresolved with
specific missing evidence and receiver. Completion means all five have honest
dispositions, not that all intermittent failures were reproduced or fixed.

## Three responsibility reviews

### Input ownership

Inventory existing Session/KVM ledgers and Machine FIFO, their writers,
readers, locks and teardown. Separate their distinct necessary purposes from
genuine duplicate authority. Trace press-before-pause, release-during-pause,
reset/retirement, genuine resume, capture continuation and stop/reset.
First evaluate correcting/removing contradictory accounting branches. Do not
add a second ledger, release queue, epoch or product callback. If a coherent
solution needs changed admission semantics, report the tradeoff before coding.

### Command completion

Trace request acceptance to executor action to completion publication to the
waiting consumer. Check whether status reads or extra waits duplicate a known
completion fact, but do not assume all state reads are redundant. Preserve
necessary request identity and synchronization. A wrong test wait should be
fixed in its owner, not compensated by a public retry/acknowledgment protocol.

### Console ownership

Use the existing Broker as the native handoff owner. Identify actual callers
of activation, display restore and input-reader handoff. Seek duplicated upper
layer interference or a missing operation in the one existing path. Do not add
replay, redraw, reattach loops or recovery state to mask child startup failure.
Combine observations 3 and 5 where evidence supports it; keep 4 distinct.

## Decision and evidence standard

For each candidate report actual user impact, source/call locations, one
responsible owner, the current sequence and synchronization, any duplicate
state/branch, the smallest alternative and a no-change alternative. Estimate
production/test additions, deletions and net size separately. Explain the
number of states, owners and paths removed, not merely line-count savings.
Necessary memory/resource safety and explicit errors must remain.

Use existing owner-local tests and bounded real-path probes to discriminate
hypotheses; no fixed-sleep proof, test-only production API or replacement
implementation. Record failure stages, session state and whether a run is a
repeat. Passing later does not erase earlier failure or prove repair.
Do not rerun the full suite merely to make a source-only review appear complete;
name exactly what was exercised and what was not. Follow background execution
and scratch retirement rules for any potentially long probe/test.

If the only proposed gain requires new compensation state for an unconfirmed
edge case, recommend no change or explicit deferral. Zero production diff is
an acceptable result. Shared layer dependence and lib/types authority remain
unchanged; broad shared-library cleanup is outside this S.

## Exclusions and delivery

The separate queued performance T owns C-VID lookup, product mouse/graphics
conversion, CPU hot-path checks and snapshot I/O. Original VGA/SB/BOP faults,
Compat physical-memory adapters and Ninja corruption are not folded into T99.
X86 Debug is reviewed only where the admitted command path actually reaches
it; do not manufacture changes to touch all eight trees.

Deliver the five dispositions, exact responsibility maps, concrete proposed
deletions/changes and minimal verification plan for any recommended repair.
Only return unresolved work to TODO on explicit deferral with a receiver;
do not keep duplicate active and deferred records. Complete
actual-diff self-review and coordinator review under the single-person mode,
documentation checks and one complete research delivery before acceptance.
This is not a planning-only implementation P or an automatic repair admission.

## Transferred historical evidence

The following five former TODO entries are retained as source observations,
including their original admission conditions, now questions owned by T99 S1.
Moving them does not establish a fix or a public-component root cause.

### Observation 1: Audit key release across paused Session input. Existing
  session/control.c forgets host held keys on release/source retirement while
  PAUSED without delivering them to the driver. T98 S15 freezes Machine FIFO
  but does not turn the host ledger into a guest-delivery ledger or resolve
  device-held keys. Source ordering is confirmed; a complete user-visible stuck
  key reproduction is not claimed. Owner: Session/driver input ownership.
  Admission condition: reproduce make-before-pause and release-during-pause,
  then define neutral reconciliation without product callbacks, treating
  host-pending FIFO and already-delivered device input as different states.

### Observation 2: Diagnose the intermittent command-provider event wait failure.
  T98 S7's second x64 aggregate run failed the 10000ms event assertion after
  debug resume/pause observations; the first aggregate and unchanged isolated
  follow-up pass. This fixture does not create a Window; RDP is not an
  established explanation. Owner: App command-provider integration fixture/
  executor event boundary. Admission condition: identify the exact awaited
  event and producer/consumer ordering under aggregate execution; do not relax
  assertions or increase product timeouts. S7 does not claim this repaired.

### Observation 3: Diagnose package integration Console attach/prompt intermittency.
  T98 S6 observed stage 1/error 6 and stage 3/error 0, including after RDP
  reconnect. Unchanged final tests later pass on both architectures; this is
  passing execution evidence, not a repair or proof of environmental cause.
  Owner: App package fixture/Console integration boundary. Admission condition:
  capture child exit, active buffer identity and cursor at the first failure
  against the unchanged baseline; do not weaken prompt assertions or add retries
  as a production fix. Temporary diagnostic code was removed. T98 S7 also
  observed a live blank initial buffer and, in a separate attempt, child exit 1
  with cannot compose machine. Some attempts failed even after RDP was Active;
  later unchanged package tests pass. These are distinct observations, not
  proof that all stage-3 failures share one cause.
  T98 S8 also observed these failures while Active: a live child with blank
  31x30 buffer/cursor 0,0, and a separate pre-attach exit 0xC0000142. Ordinary
  package subsequently passed on both architectures while compact failed.
  The temporary diagnostic was removed; no causal repair is claimed.
  A subsequent S8 probe proved one live child reached the unchanged prompt
  another 5157 ms after the five-second check. The startup-only test budget
  was corrected to 30 seconds, without a retry or product timeout change.
  This does not explain the separate initialization exits or prove that all
  attach/prompt failures are timing-budget failures.
  T98 S9 repeats stage 1/error 6 in compact package on each architecture while
  RDP is Active; ordinary package and all other cases pass. Each unchanged
  isolated compact rerun passes. No fixture/product change or causal repair
  is claimed; retain both first failures and supplemental qualification facts.
  T98 S10 final x86 compact package again failed stage 1/error 6; an unchanged
  complete desktop-group repeat passed 5/5. No causal repair is claimed.
  T98 S13 x64 compact package repeats stage 1/error 6 while RDP is Active;
  its unchanged isolated repeat passes. x86 passes all desktop cases first run.
  S13 does not claim to repair this existing package-initialization debt.
  T98 S15 x64 repeats compact stage 1/error 6 after reconnection while Active;
  the unchanged isolated case passes. All other desktop/audio cases pass.
  This supplemental qualification does not establish a repair or root cause.

### Observation 4: Diagnose intermittent package compact-Console raw input loss.
  T93 S12's first desktop x64 compact case reached DOS but timed out at
  stage=14/error=0 waiting for Version after injected ver; the captured screen
  contained only C:\>. Normal package passed immediately before it. A complete
  unchanged ten-case desktop rerun passed both widths. This differs from the
  stage=1 child-startup debt below and does not prove the cause or a repair.
  Owner: App-SoftPC package-test/input handoff. Admission condition: bounded
  native input/activation instrumentation around first raw command and hide/
  focus transition; do not add retry or relax assertions. See
  [S12 verification](../history/M9-T93-xp-sound-blaster-standalone-proposal.md#s12-implementation-and-review-record).

### Observation 5: Diagnose native package-test child startup under sequential
  CTest execution. T93 S5 x64/x86 compact-console cases intermittently fail
  before start at stage=1/error=6. A scratch diagnostic reproduces child exit
  0xC0000142 (DLL initialization failed), AttachConsole error 5, then code-page
  query error 6. Direct native/package probes pass; this does not identify the
  responsible DLL or prove a CTest defect. Admission condition: test-owned
  child-startup/Console lifetime investigation with loader evidence, without
  retrying product startup or changing guest/device behavior to hide it.
  Evidence: [T93 S5 checkpoint](../history/M9-T93-xp-sound-blaster-standalone-proposal.md#s5--original-dma-prerequisite-repair).
