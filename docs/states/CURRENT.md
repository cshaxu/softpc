# Project Status

## Current Work

M9 T70 S7 is active: correct the generic CCPU call-gate stack-load boundary
found by the bounded Windows 95 Setup hardware-detection investigation.

## M9 T70 S7 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner: use supplied `snapshot.02`, the closest bounded early-detection checkpoint, to continue from the measured CCPU root without a product-specific workaround. |
| Objective | Make every privilege-changing CCPU stack load use the new SS descriptor's stack-address width, preserving a full 386 ESP when SS is a 32-bit stack segment. |
| Non-goals | No Setup/snapshot/mode special case, display workaround, controller replacement, timing retry, Lib/Common redesign, owner media/INI write, tracked external fixture, or second executor. |
| Reference Baseline | S6 source state. The fixture's 16-bit call gate correctly reads `ESP0=0x80010df0` from its 386 TSS for `SS=0x0030`, whose B bit is 32-bit; the old branch used call-gate operand size and truncated it to `SP=0x0df0`. |
| Candidate Proposal | [Win95 Setup hardware-detection display-loss investigation](../proposals/m9-win95-setup-hardware-detection.md). |
| Files And ABI Surface | `src/mvdm/softpc.new/base/ccpu386/{call,iret,ret}.c`, using their existing `set_current_SP()` helper; focused CCPU proof and existing regressions. No public ABI changes. An x64 EXE may be copied beside the owner fixture as a disposable diagnostic artifact. |
| Applicable Rules | EXECUTION, ARCHITECTURE, CODING, DOCUMENT and PRODUCT UI. Original 8042 remains the single device owner; fixture configuration/media are user-owned. |
| Verification | Focused CCPU/call-stack and snapshot transaction tests; input/PIC tests; x86/x64 builds and suites; bounded owner-fixture repetition with `snapshot.02`; fixture hash comparison; documentation gate and original-diff check. |
| Expected Markers | A 386-TSS call gate to a 32-bit SS retains its full ESP independent of 16/32-bit call-gate operand size. The fixture no longer writes the real-mode trampoline table through the truncated stack or loops at the resulting `#UD` address. |
| Asset Needs | External owner fixture only, read-only except the requested EXE copy. No fixture enters source control; diagnostic logs stay under ignored `build/t70-s2/` with a time/size budget. |
| Reporting Requirements | Report the retained narrow MVDM diff, focused/full test results, fixture outcome, and unchanged snapshot/image hashes. |
| Stop Conditions | Fixture mutation, failure to preserve normal call/return or input/PIC behavior, need for a Setup-specific branch, or a different later hardware failure stops S7 for a new bounded task. |
| Exit Criteria | Both widths build and pass all tests; the generic stack-width proof passes; x64 fixture run reaches beyond the prior `#UD` recovery path or documents the next independent device failure; fixture hashes match. |
| Original Owner Request | “启动后 load snapshot.1, resume，不多久即可复现。…我想找到卡死的原因，让 win95 setup 通过硬件检测阶段。” |
| Similar-Issue Sweep | Audit all privilege-changing CCPU stack-load paths. Interrupt gates already call `set_current_SP()`; call gates, lower-privilege IRET and lower-privilege RETF now do too. The item width remains instruction-defined; stack register width is SS-defined. |

## Current Technical Baseline

- Source: T68 is closed. The preserved Scan-1 table mapper rejects both first
  out-of-range input values without changing valid key mappings. No Lib,
  Common, App, VM, Compat interface or input behavior changed.
- Both widths have 107 passing test cases; package EXEs were refreshed without
  changing owner INI/media. See [T65 audit](../history/M9-T65-completion-audit.md).
- Snapshots are width-independent fixed-order binary streams with no magic,
  version or section identifier. They restore CPU/device/media state into a
  normal PAUSED machine; Window creation remains deferred until resume.
- `save` accepts RUNNING or PAUSED. A paused save writes an already-held VM
  checkpoint directly, or privately advances the existing executor to one
  while the product remains PAUSED and guest input stays gated.
- DIRECT/READONLY media retain and verify their external references; FDD/HDD
  OVERLAY effective differences and cylinder state are in the same binary.
- T63 snapshot behavior remains the owner-accepted baseline. Current package
  EXEs include T67; the owner’s current package INI edit ships with T67.
- T69 is closed: its source-neutral Compat display transaction, painter-width
  contract and detached text/cursor updates remain in force. The intermediate
  V7 helper and unused linear fill family have been removed; the fullscreen
  native Window-height observation remains reproducibility-gated TODO debt.
- T70 S1 proved the graphical/text change is Setup recovery during hardware
  detection. S2 repaired the original `0xC0` input-port response, but bounded
  instrumentation showed that it is not sufficient to explain this fixture.
  T70 S3 repaired the generic rejected-PIC/CCPU dispatch boundary. T70 S4
  restored the original SAS no-wrap allocation contract but did not change the
  fixture result. T70 S5 corrects the standalone TLS CCPU `setjmp` ABI; S6
  parks a restored continuation before another instruction is decoded.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T69 | S1--S4 complete; owner-reopened cleanup removed its own obsolete code. | [Audit](../history/M9-T69-completion-audit.md) |
| T68 | S1 complete; owner-directed safety repair; final dual-width 107/107. | [Audit](../history/M9-T68-completion-audit.md) |
| T67 | S1--S4 complete; owner accepted; final dual-width 107/107. | [Audit](../history/M9-T67-completion-audit.md) |
| T66 | S1--S4 complete; owner-validated dual-width package. | [Audit](../history/M9-T66-completion-audit.md) |
| T65 | S1 complete; request-slot refactor cancelled after contract audit; dual-width 107/107. | [Audit](../history/M9-T65-completion-audit.md) |
| T64 | S1 audit accepted; S2 narrowed frame copy verified; owner accepted. | [Audit](../history/M9-T64-completion-audit.md) |
| T63 | S1–S12 complete; owner acceptance; final dual-width 106/106. | [Audit](../history/M9-T63-completion-audit.md) |
| T62 | S1–S8 complete; owner acceptance; dual-width 101/101 with recorded intermittent tick debt. | [Audit](../history/M9-T62-completion-audit.md) |
| T61 | S1–S13 complete; mirror and VM/Compat ownership audit; dual-width 98/98. | [Audit](../history/M9-T61-completion-audit.md) |

## Recent Governance

T69 closed after S4's source deletion/restoration and dual-width verification.
Its archived proposal and completion audit retain the causal record.
T70 S7 is the current bounded fixture investigation. Its direct CCPU repair
uses the already-existing stack-address-width helper after loading call-gate SS.
