# M9 T55 S11 — Window Activation and Closed-Pause Lifetime

Owner requests: “噢好的。请你修复这两点。”; follow-up “还真是，之前没点过X就没事！”;
final authorization “请帮我修复，然后收口提交推送这个S任务！”.
S11 is closed; T55 remains open. Original ledger, design, peer sweep and
evidence are retained in the [T55 proposal](../history/M9-T55-lib-types-external-boundary-proposal.md).

## Acceptance

- CLS: baseline native DOS ver/cls succeeds, confirmed by owner. Speculative
  compatibility edits were removed; package regression retained. Owner's
  Win3.1 fullscreen/CAF/Alt+Enter display defect remains separately deferred
  in [TODO](../states/TODO.md), not claimed fixed.
- Focus: actual frozen-to-unfrozen transition requests activation once,
  following Console ownership work. No automatic capture or permanent focus.
- X: its closed-paused override expires on entry to RUNNING, not on Window
  destruction. Later CAP preserves Window. No new state or lib/MVDM fix.

## Verification and committed-diff review

Executor P1 `16fa420`, review P2 `d3852a4`, follow-up executor P3 `bcf2353`
were pushed. Coordinator inspected `git show bcf2353` and the full source/test
diff from `54fe10e`, not only executor prose. All four ledger entries have
either proof or the owner's explicit separate-handling disposition.

The eight-case reducer matrix failed before the five-line fix and passes
afterward: both displays, both console_control values, X/resume/CAP and
X/stop/start/CAP. Duplicate RUNNING and paused destruction preserve pending X.
Fresh builds refreshed both fixed EXEs; x64 52/52 (34.96 s), x86 52/52
(36.02 s), standalone lib checks 3/3; prior strict build remains applicable
(P3 does not modify lib). Governance, boundary, manifest and whitespace pass.
These are automated results, not a claimed fresh manual Win3.1 verification.

`git diff --numstat 54fe10e bcf2353 -- src test`, excluding documentation,
manifest and binaries: 4 production paths +24/-6, net +18; 4 test paths
+72/-10, net +62. Reconciler remains the sole close-state owner; CAP has
no close-state write. No extra flag, compatibility path or timing workaround.
Owned generated test fixtures were removed; INI/media/MVDM untouched.
