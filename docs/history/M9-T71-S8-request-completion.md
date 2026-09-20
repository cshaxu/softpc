# M9 T71 S8: Executor Request Completion

Owner acceptance: "测试通过。下一个S". S8 closes after delivery 81d52730
and review 81dacb61; T71 remains open and S9 is separately admitted.

Common Machine serializes synchronous admission with terminal cleanup. After
executor unwind, pending save/load/debug/media requests fail through existing
completion events. Completed results remain unchanged; ordinary pause cannot
prematurely complete an unready save. One mutex replaces an unused ready event.
No public API, Lib, VM, Compat, MVDM, snapshot format or media change was needed.

Against 92be0e6a: production +85/-39 (net +46), tests +161/-9 (net +152).
Final Release builds and serial suites pass: x64 110/110 (169.85s), x86
110/110 (124.72s), including request fault matrices, PIF roundtrips and snapshots.
Owner then accepted the EXEs. Detailed evidence and hashes remain in the
[regression proposal](../proposals/m9-kvm-mode-transition-regression.md).

Earlier unchanged modal tests failed once per width, then passed isolated
repeats and final suites. S10 retains that observation; no modal repair is
claimed. S9 FIFO is separate. The owner's subsequent display=console INI edit
is preserved verbatim with this acceptance record, not an agent edit.
