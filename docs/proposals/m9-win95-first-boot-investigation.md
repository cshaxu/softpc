# Windows 95 first-boot investigation

Owner reports abnormal display on the first system startup after Setup has
finished copying files. It is not yet established whether the machine boots
with broken output or fails to boot. Owner supplied a cold-start recipe:
start the installed disk with the accepted x64 package; the startup banner,
first graphics initialization and a minutes-long blinking text cursor precede
a second graphics initialization which remains black. No snapshot load is
involved. Active admission is recorded in Current.

The investigation first records the supplied steps, exact executable/config,
media identity and any snapshot dependency. Observe guest execution progress
and disk activity alongside video registers, copied frames and final output.
Use existing debugger/observation interfaces first. A static screen alone is
not evidence of a CPU hang; continued execution alone does not prove boot
progress. Classify the earliest confirmed divergence before proposing repair.

| Coverage member | Required disposition |
| --- | --- |
| Reproduction | Owner steps reproduced, or precise difference/unavailable input recorded. |
| Guest progress | Boot stage and bounded CPU/interrupt/disk evidence; distinguish useful progress from a loop or wait. |
| Display | Compare guest video state, frame publication and visible result at the failing stage. |
| Restore dependency | Record whether the case requires snapshot load; compare a normal boot only when matching disposable input is available. |
| Root and repair boundary | Evidence-backed fault owner and minimal proposed fix, or explicit unresolved hypotheses and next discriminating observation. |

Initial production/test diff estimate is zero: this admission is research.
Do not alter Lib/Common, patch guest binaries, add Setup-specific behavior,
or change snapshot/media semantics. Any implementation scope requires an
evidenced brief and estimate before proceeding under owner approval.

Use owner media read-only or disposable copies as required by the reproduction
recipe. Diagnostic output belongs only under ignored `build/t70-s9`, capped
at 600 seconds and 64 MiB per run, with early stop on no additional evidence
after reaching the reported stage. The longer time budget accommodates the
owner's observed minutes-long initial boot; observations remain sparse.
The agent owns process termination and deletion of its exhausted diagnostic
outputs; retain only the minimal checkpoint needed to support conclusions.
No external fixture content enters source control. T70 remains open.

## Cold-start observation

The external x64 executable hash matches the accepted package. Its INI uses
16 MiB RAM, overlay hard disk, console display and console_control=0. The
supplied disk SHA-256 is unchanged after investigation. No snapshot was loaded.

Two ignored diagnostic executables linked the existing x64 production VM,
Compat, MVDM and Common machine objects. They invoke ordinary machine start
and copy published frames; they do not instantiate the desktop presenters or
exercise the monitor parser. Thus the core black-output symptom is reproduced,
not yet an independently witnessed end-to-end desktop interaction.

- Around 20 seconds, execution reaches protected-mode code; around 35 seconds
  the published output changes from the startup image to text.
- Around 95 seconds, output changes to 640x480 graphics and is black. The first
  run initially holds frame sequence 9 and unchanged storage-operation counts.
  Around 129 seconds disk activity resumes briefly. A later frame contains a
  small hourglass on a black background.
- After approximately 160 seconds, frames continue at roughly 10/second but
  remain black apart from cursor content. Storage counters stabilize at 3450
  reads and 4890 writes. These are medium API operation counts, not sectors or
  physical-disk writes; all writes remain overlay-only.
- CPU instruction sampling continues across different addresses, including
  kernel and ring-3 code. The second run reaches over 864 million observed
  instruction entries by 237 seconds. Repeated GP/invalid-opcode entries are
  counted, but their presence alone is not diagnosed as an emulator defect.
- Executor-side video capture at approximately 180 seconds finds only 415
  nonzero bytes in the 524288-byte plane allocation and a nonzero DAC palette.
  The black image is already present upstream of KVM output; a missing final
  window repaint is not sufficient to explain it. Incorrect emulated video
  writes, or guest initialization which never produces the intended picture,
  remain unresolved alternatives.

The first run was stopped after roughly 270 seconds once its observations
stabilized; the second exited normally at 240 seconds. This is not proof that
an indefinitely longer boot cannot progress. No production/test source or
package was modified. No repair or S closure is claimed.

| Coverage member | Current disposition and next proof |
| --- | --- |
| Reproduction | Cold-start core symptom reproduced twice; full desktop confirmation remains separate. |
| Guest progress | CPU remains active, late disk activity ceases; identify the sustained wait/fault/service path before calling it a boot hang. |
| Display | Published black frames agree with almost-empty emulated video memory; trace first intended display writes and their physical destination. |
| Restore dependency | Reproduced without load; snapshot restoration is not a necessary trigger. |
| Root and repair boundary | Not established; retain research scope, with no speculative Lib/Common or guest-specific fix. |
