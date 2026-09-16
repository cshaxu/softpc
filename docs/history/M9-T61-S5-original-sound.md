# M9 T61 S5 — Share the original sound state machine

## Request and implementation

Owner requested minimal original-source difference while preserving sound,
not a replacement audio design. P1 `a65dc18` removes the copied standalone
state machine and uses the original functions. NT beep resources remain
conditionally compiled for the original host; standalone uses its existing
tone sink, clock conversion and three PPI/Timer-2 entries. No Compat or device
callback changes, new thread, state owner or public ABI.

The finite state/function/caller table and verification are in the
[ledger](../etc/evidence/softpc/pristine-divergence-current.md). Six functions
now have exactly the original body text. Original content diff +222/-0 becomes
+64/-0. Production +62/-220, net -158; tests +77/-0, test build wiring +9/-0.
The aggregate code/build delta is -72, without deleting necessary behavior.

New controlled-clock/tone/config test passed both before and after on x64/x86.
It covers pulse smoothing/expiry, duplicate tone, click, gate and waveform
limits, sound initialization and bell configuration. Existing port/reset,
audio lifecycle and failure tests remain. Both builds and full suites pass
98/98. Fixed EXE hashes and tooling corrections are retained in the ledger.

## Coordinator review

Reviewed actual pushed a65dc18 diff and original-source comparison. All
standalone state and functions are accounted; original NT resource code was
not deleted or accidentally selected. The equal-tick pulse behavior remains,
not asserted repaired. Post-commit sound/state/lifecycle/failure tests pass
4/4 on each width. Shared corpora, App/VM/Compat, INI/media are unchanged;
documentation and diff gates pass, no temporary evidence files remain.

S5 closes. S6 is admitted for final whole-tree recount and task-level audit;
T61 remains open, with no claim that all remaining mirror changes are absent.
