# Shared Base synchronization continuation

## S14-S16 continuation plan

Owner request: "写入proposal后 准入s14 s14完成后编译测试提交推送后等我".
S14 is admitted from dc5811f; S15/S16 are planned only. S13 is delivered,
not retrospectively claimed manually accepted. T59 remains open.

| Step | Scope | Exit proof |
| --- | --- | --- |
| S14 | Host output/transaction locks, Console metadata lock and KVM control lock use existing Base mutex; remove local spin helpers | Failure cleanup, deterministic contention, independent frame/control, broker replacement/output barriers, full dual-width suite |
| S15 | Existing Base Event exposes automatic/manual reset and checked signal/reset; KVM reuses it, deletes duplicate platform wake implementation | Reset modes, timeout/failure/no-lost-wake, one notifier per mailbox; native Window messages unchanged |
| S16 | Resolve Base task bounded completion/join/disposal and cancellation needs, then reuse for KVM workers | Startup failure/early exit, bounded join, FIFO STOP and no live worker after successful disposal; no pointer-only wrapper |

Each step delivers both fixed EXEs, tests, manifests, source/test line accounting,
executor commit/push and actual-patch coordinator review/push. Stop after S14
for owner testing; later steps require admission. Types stays header-only.
Host reader's native I/O cancellation and Window message loop remain with
their owners; neither is forced into a generic thread/cancellation framework.

### S14 finite ledger and implementation boundary

The complete bounded universe is the four consumer lock roles below. Each
must preserve its protected data, acquisition/release sites and ordering;
only the primitive implementation changes. No new Base API or lock wrapper.

| Member | Change | Evidence |
| --- | --- | --- |
| Host output + transaction | Existing backend fields become Base mutex pointers; checked creation and cleanup | Both creation failures release resources; existing replacement/rollback/output serialization tests |
| Console metadata | Existing field becomes Base mutex; delete spin helper pair | All three mutex creation failures; metadata and sink barriers |
| KVM control | Separate Base mutex, retain frame-before-control terminal order | Either allocation failure clean; contention blocks; ordinary control progresses during frame copy |
| Boundary/delivery | Host direct Base dependency; gates reject consumer primitive reimplementation | DAG exhaustive tests, primitive scan, manifests, strict x86/x64 builds/full tests |

Single atomic process claim and atomic reference/status values are not busy-wait
locks and remain Types operations. Base's own platform mutex/condition is the
primitive implementation. KVM's existing wake condition belongs to planned S15;
native thread/wait paths are reviewed in S16, not changed in S14. No App, Common,
Compat, VM, MVDM, INI/media or product behavior changes are admitted here.

The S13 exclusions in the linked history describe that delivery, not S14's scope.

S13 extraction evidence is retained in
[history](../history/M9-T59-S13-base-synchronization.md).

## S14 executor evidence

All four ledger members are implemented. The production C/H patch is limited
to console/console.c, host/win32/console.c, kvm-base/mailbox.c and
kvm-base/mailbox_interface.h. Existing lock scopes and acquisition order are
unchanged. No Base API or implementation changes, no forwarding aliases and
no additional state machine. Host creates both mutexes before opening native
Console handles; allocation failure frees the partial pair without takeover.
An existing native-open failure also releases the pair. Console releases any
partial three-mutex allocation; mailbox creation either owns both independent
mutexes or leaves an empty destroyable object.

Focused real-mutex probes use TryEnterCriticalSection failure to prove actual
contention before asserting a contender remains blocked. They cover Console
metadata, event/output barriers, both Host locks, KVM frame publication/capture,
STOP and control consumption. Frame/control progress is checked in both
directions: a held frame lock does not stop ordinary control, and a held control
lock does not stop frame capture. No Sleep or CPU/timing threshold is a proof.
Console creation rejects each of three allocations with zero tracked live
mutexes; Host rejects each allocation with zero live mutexes and unchanged
process handle count; mailbox rejects either allocation and safely disposes.
Existing broker prepare/commit rollback, output serialization, event generation,
retirement and input-admission tests remain in the full regression.

Similar-issue sweep: rg over all src/lib C/H for critical_section,
pthread_mutex, atomic_flag and lock loops. Base owns the Win32 mutex and Linux
mutex/wait implementation. Host process claim remains one atomic operation, not
a spin loop. The only remaining independent pthread mutex is the KVM Linux
wake condition explicitly assigned to planned S15; native thread/wait paths
remain assigned to S16 or Host-specific I/O cancellation. No consumer mutex
implementation remains in S14's finite universe. The source gate rejects
non-Base Win32 mutex implementations and Console/mailbox atomic-lock regressions;
five negative probes plus the exhaustive 64-edge source/build DAG pass.

Count method: git diff dc5811f --numstat restricted to tracked src/test C/H.
Production four paths: +52/-59, net -7. Test five paths: +113/-22, net +91.
Documentation, CMake/gates, manifests and binaries are excluded from those counts.
Tests add deterministic proof, not production abstractions. S13 history moved
unchanged; the replacement proposal contains only this continuation plan/evidence.
Only src/lib and test/lib manifests advance to shared-t59-s14-p1; unchanged
Common source/test manifests still verify. No other production corpus changes.

Strict GNU builds completed for x64 and x86 using existing build trees.
Full x64: 92/92 in 77.48 s. First x86 full run: 91/92 in 64.93 s;
the native modal fixture observed an early OS loop exit at line 75, before
freeze/STOP. No production/test change was made to suppress it. The exact
unchanged executable then passed ten consecutive isolated modal runs (26.98 s).
The cause is not established; fixture-isolation follow-up is recorded in TODO.
Second complete x86 run: 92/92 in 48.34 s. The isolated runs are not substituted
for that full-suite proof, and the transient is not claimed fixed.
Four manifest and documentation gates pass. Native Linux execution is not
claimed; existing Linux fake wait coverage remains in the suite.
Build trees are retained for reproducible testing; no new media/trace directory.

Fixed tested package hashes:

- softpc32.exe: 3,487,113 bytes,
  E26F26C3CB58F69955096E0CFE81D6D3EE797F17630240D7831FF0EB33F3C0DE.
- softpc64.exe: 2,846,251 bytes,
  BCF71B4013B4F4BDEC70A55D9C213E97587D19EF3BA1A7DCBEDE95205F82D31C.

Full verification is complete; push executor P1 then review the actual committed patch
as coordinator and push P2. Await owner testing; S15/S16 and T closure remain
unadmitted.
