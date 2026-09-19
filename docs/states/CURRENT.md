# Project Status

## Current Work

M9 T70 S8 is closed with owner acceptance; delivery is `f54e19f`.
M9 T70 S9's candidate PIT read-time interpolation repair reaches the next
Win95 graphical installation stage; dual-width regression and further
installation verification remain in progress.
T70 remains open.

## M9 T70 S9 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner approved S8 closure and S9 research, supplied cold-start steps, then explicitly requested root-cause repair and verification of the next graphical installation stage. |
| Objective | Determine whether Win95's first boot after Setup file copying is stalled or waiting; find and repair the evidenced cause so installation reaches its next graphical stage and can complete. |
| Non-goals | No speculative repair, guest-specific workaround, guest-media mutation, Lib/Common change, or snapshot/media contract change. |
| Reference Baseline | S8 delivery `f54e19f`, accepted by owner; dual-width package hashes and limitations in the S8 closure. |
| Candidate Proposal | [Win95 first boot](../proposals/m9-win95-first-boot-investigation.md). |
| Files And ABI Surface | Investigate VM/Compat/MVDM and display pipeline; evidence-backed minimal repair plus focused tests, with pre-change estimate when the responsible path is established. Lib/Common unchanged. |
| Applicable Rules | Execution, documentation, architecture, coding and source/research authorities. |
| Verification | Reproduce owner steps; correlate CPU/interrupt/disk progress with video state, published frames and visible output; record unresolved alternatives. |
| Expected Markers | A repeatable failure stage and evidence distinguishing broken rendering from stalled or failed boot; no inference from a screenshot alone. |
| Asset Needs | Owner supplied installed disk and cold-start recipe; confirmed overlay configuration and accepted x64 package identity. Preserve originals. Ignored build/t70-s9 diagnostics capped at 600 seconds and 64 MiB per run to accommodate the minutes-long initial boot. |
| Reporting Requirements | Reproduction result, observed boot progress, display evidence, component attribution, minimal repair plan and estimated diff if a repair is justified. |
| Stop Conditions | Missing reproduction input; required alteration of original owner media; need for shared-corpus change or unrelated behavioral change. |
| Exit Criteria | Root cause proven, minimal repair reviewed, dual-width build/regression and next graphical installation-stage evidence recorded; unresolved completion evidence keeps task open. |
| Original Owner Request | “批准收口当前S8，提交推送，然后准入一个S9：调研win95安装程序文件复制完成后第一次启动系统的显示异常故障（不确定是不是只是显示异常，还是系统其实根本无法启动，需要观测）。我会给你截图和如何复现的指令。” |
| Similar-Issue Sweep | Once the failing contract is identified, enumerate matching production paths in its owning component; do not presume an emulator or rendering cause before observation. |

Owner's follow-up objective: “找出win95安装程序第一次启动系统的显示异常问题原因并修复，并确定系统是卡死还是正常等待；目标是让win95安装程序正常进入下一阶段的图形界面安装状态，用户可以正常完成系统安装”。

## Current Technical Baseline

- S9 isolates read-time PIT guesses generating spurious IRQ0 work. The
  candidate timer.c repair and fixed-clock test are present; both package
  EXEs are rebuilt. Cold start reaches Win95's hardware/PnP dialog.
  Both widths pass the nine-case PIT test and 107/109 full regression tests;
  both retain the previously disclosed stage-16 package-test failures.
  Longer installation observation and final delivery review remain pending.
  See [bounded observations](../proposals/m9-win95-first-boot-investigation.md#cold-start-observation).
- S8 removes INI mode conversion and same-path attachment retention. Snapshot
  restore first detaches all slots, then reopens saved paths/modes and applies
  saved overlay pages. Codec and SHA/size checks are unchanged. See the
  [S8 delivery evidence](../history/M9-T70-S8-snapshot-media-remount.md#delivery-evidence).
- S8 x86/x64 builds and focused snapshot tests pass. Both widths pass 106/108
  aggregate regression cases; the two package Window checks fail at stage 16,
  before any snapshot load. Root cause is unestablished; no full pass is claimed.
- T70 implementation delivery `ce5f535` fixes the CCPU privilege-changing
  stack-load width using the existing SS-based helper in CALL, IRET and RETF.
  Owner confirmed that the supplied checkpoint passes Win95 hardware detection.
- The same delivery includes the bounded S2--S6 controller/PIC, SAS allocation,
  TLS setjmp and restored-continuation repairs; their distinct evidence is in
  the [investigation record](../history/M9-T70-S6-investigation-record.md).
- Both x86/x64 package EXEs are rebuilt. Non-integration tests pass 105/105
  on each width; focused checkpoint/snapshot/IRQ/BOP tests pass 4/4 each.
  Two package Window integration tests failed at Window observation; their
  cause remains unproven. Owner approved S7 closure with this disclosed limit
  and successful hardware-detection acceptance. No full-suite pass is claimed.
- Lib/Common, user INI and guest media are unchanged. Snapshots retain their
  width-independent format; S8 updates media semantics as above. Restoring a snapshot
  parks its exact continuation before another guest instruction executes.
- T69's accepted Compat display transaction, packed-painter width and text
  surface corrections remain. The nonreproducible native Window-height report
  remains in TODO; the later Win95 memory warning is not diagnosed by S7.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T69 | S1--S4 complete; owner-reopened cleanup removed its own obsolete code. | [Audit](../history/M9-T69-completion-audit.md) |
| T68 | S1 complete; owner-directed safety repair; final dual-width 107/107. | [Audit](../history/M9-T68-completion-audit.md) |
| T67 | S1--S4 complete; owner accepted; final dual-width 107/107. | [Audit](../history/M9-T67-completion-audit.md) |
| T66 | S1--S4 complete; owner-validated dual-width package. | [Audit](../history/M9-T66-completion-audit.md) |
| T65 | S1 complete; request-slot refactor cancelled after contract audit; dual-width 107/107. | [Audit](../history/M9-T65-completion-audit.md) |
| T64 | S1 audit accepted; S2 narrowed frame copy verified; owner accepted. | [Audit](../history/M9-T64-completion-audit.md) |

## Recent Governance

T70 S8 closes after owner acceptance of pushed delivery `f54e19f` and actual
change review. S9 is the sole active investigation; queue order is unchanged.
