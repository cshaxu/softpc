# M9 T71 S10 Text Contract Cleanup

## Request And Boundary

Owner: "准入修复，S10完成上述4个项目收口。" Baseline a1e7b945.
This admits the reviewed minimal nt_cga layout correction, KVM cursor semantics,
Machine request documentation and Console lifetime alias removal. The owner
explicitly chose the two-line original-host correction over a Compat workaround.
No neutral-attribute redesign, new capacity, state, worker, cache or queue.

## Delivered Requirements

| Request | Production result | Proof |
| --- | --- | --- |
| Fixed destination stride | Original nt_text bulk copy requires both visible 80 columns and 80-cell source pitch; its existing row copy advances by source pitch. | Actual linked painter memory matrix: 40/80 visible columns, matching/nonmatching pitch, partial/full columns, nonzero destination row and one/three rows. Old code fails; repaired code passes. |
| Same KVM cursor meaning | Shared text header owns zero-height default and scanline bounds. KVM Console normalizes at its existing conversion; Window keeps its correct geometry. | Conversion matrix plus existing Window geometry and native Console mock size/visibility assertions; old zero-height conversion fails. |
| Machine request contract | Header includes state I/O in sole-control serialization, copied descriptors and borrowed callback contexts; failed native wait is not completion proof. | Actual request payload/registration/callback/termination review. No Machine C implementation changed. |
| Remove duplicate lifetime name | lib_console_destroy removed; retain/release is the only reference lifetime vocabulary. | Six test calls migrated, no remaining src/test reference, both-width build and lifetime tests. |

Console cannot reproduce arbitrary scanline vertical placement; its native
height approximation remains. Inverted cursor ranges preserve existing full
cell behavior. No Broker-to-KVM dependency was introduced. The public destroy
alias deletion requires consumers to use release; no object layout changed.

## Verification And Accounting

Implementation P1: f3ff1b11, pushed to origin/main. Both Release builds succeed.
Serial background suites: x64 105/105 (163.44s), x86 105/105 (145.85s), including
real headless Win3.1 PIF roundtrips, snapshots, restart, failure/ownership tests
and shared gates. Changed Lib C sources pass strict C17 warnings on both widths.
Documentation governance, manifests, dependency gates and diff --check pass.

Five desktop-only tests per width were not executed. No current native visual
or Linux-runtime acceptance is claimed. The earlier S8 native modal-test early
exit has no proved root cause; static review found the one STOP/WM_CANCELMODE
route, not a demonstrated repair. It is explicitly retained in TODO with a
reserved-desktop reproduction trigger. It is outside these four repairs.

Counts use git diff --numstat a1e7b945 f3ff1b11, C/H paths only:

- Production: seven files, +24/-14, net +10. MVDM +3/-2 (two logic lines and
  one reason comment); Lib +11/-10; Common header comments +10/-2.
- Tests: seven files, +80/-7, net +73.
- Shared manifests: three files, +15/-15, separate from code. No test/common
  content changed, so its manifest remains valid and unchanged.
- nt_cga versus OpenNT: +21/-3 total. No other original mirror change.
- No Compat, VM, INI, media, snapshot-format or input change.

Package x86 is 3655192 bytes (-57), x64 3058735 bytes (-56); exact hashes and
the finite similar-issue ledger are in the
[implementation brief](../history/M9-T71-mode-transition-regression-proposal.md#s10-changed-path-accounting).
No new diagnostic files/processes are retained. Test-created media is disposable;
the initial failed fixture's disk was removed by the subsequent successful run.

## Coordinator Review And Closure

After P1 push, the same agent switched roles and inspected the actual committed
production/tests/manifests and artifact accounting, not only the executor report.
Source pitch and destination stride have separate owners; cursor normalization
stays at the KVM-to-Console boundary; borrowed contexts gain no hidden ownership;
the removed alias has no production caller. Counts and tested package hashes
match. P1 HEAD equalled origin/main with a clean worktree before this review.

The finite four-item ledger meets its declared exit criteria. S10 closes under
the owner's explicit admission/closure instruction. This is not a whole-T or
all-video correctness claim. T71 remains open pending the owner's next direction.

## Owner Acceptance

Owner subsequently confirms: "测试通过，准许收口S10". Manual acceptance is now
recorded in addition to P1 verification and P2 coordinator closure. This P3
updates records only; source, tests, EXEs, configuration and media are unchanged.
Documentation governance and whitespace checks pass. No build or automated
desktop test is rerun or inferred from this owner report. T71 stays open with
no active S; no next task is admitted by this confirmation.
