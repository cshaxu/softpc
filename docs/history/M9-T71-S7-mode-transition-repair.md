# M9 T71 S7: Ready Text Frames And ERROR Admission

Owner accepted the delivered binaries: "测试通过。准入下一个S任务".
S7 closes; T71 remains open and S8 is separately admitted.

The original report covered failed Win3.1 entry and process exit after start
in ERROR, followed by failures during fullscreen/windowed DOS Prompt changes.
The retained repair uses selected-renderer dimensions, rejects disabled or
pending-mode text as not ready, uses controller font height and checks required
font acquisition. ERROR is preserved through Session/App so rejected commands
retain the monitor rather than terminating it. No guest-specific condition,
capacity expansion or snapshot-format change was introduced.

Delivery 1785787c / review 83e185c7: production +16/-7 (net +9), tests +72/-5
(net +67). Continued delivery ff499749 / review 92be0e6a: production +5/-3
(net +2), tests +117/-2 (net +115). The latter changes only Compat/VM production;
neither delivery changes Lib or MVDM. Full final x64 110/110 (124.22s) and x86
110/110 (142.06s), including real Win3.1 entry, both PIF initial modes and six
Alt+Enter/CLS roundtrips per mode, snapshots and native package checks.

Earlier package failures contained unexpected input; interference remained a
hypothesis, not a claimed product fix. Isolated repeats and both final suites
passed, followed by owner acceptance. Mouse symptoms were not reproduced and
received no speculative production patch. INI/media stayed unchanged.

The finite before/after ledger and detailed evidence remain in the active
[T71 regression proposal](../proposals/m9-kvm-mode-transition-regression.md).
Outstanding request completion belongs to S8, frame FIFO to S9, and the
retained nonstandard-stride audit to S10; none is claimed solved by S7.
