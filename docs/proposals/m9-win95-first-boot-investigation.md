# Windows 95 first-boot investigation

Owner reports abnormal display on the first system startup after Setup has
finished copying files. It is not yet established whether the machine boots
with broken output or fails to boot. Screenshots and reproduction instructions
will be supplied by the owner. Active admission is recorded in Current.

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
at 120 seconds and 64 MiB per run, with early stop on no additional evidence.
The agent owns process termination and deletion of its exhausted diagnostic
outputs; retain only the minimal checkpoint needed to support conclusions.
No external fixture content enters source control. T70 remains open.
