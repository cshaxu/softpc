# T100 S2 candidate repairs

CURRENT owns admission/status. S2 P2 implements only the owner-approved D005
and D006 candidates from the [S1 decisions](m9-t100-owner-decisions.md).
The owner requests minimal permanent regressions, dual-width cached builds,
publication, full regression, actual diff review and one P commit/push.
Other S2 candidates remain unimplemented and require their own review.

## P2 scope and mirror differences

| Point | Original/current failure | Authorized correction |
| --- | --- | --- |
| D005, base/system/timer.c issueIREQ0 | An already-active periodic backlog calculates integer int_delay=0 and lowers timer_multiple_delay to zero. add_event immediately invokes zero-delay callbacks, so timer_multiple_ints recursively reschedules itself until stack overflow. | Add the same two-line zero-to-one guard used by the first-event branch, before the existing minimum comparison. Preserve existing backlog, callback and queue ownership. |
| D006, base/video/egwrtm12.c ega_mode2_chn_b_write | The easy even-address write targets plane2 but checks plane3's enable bit8. Plane2-only mask4 misses the write; plane3-only mask8 can overwrite disabled plane2. | Change only that enable mask from 8 to 4; preserve address mapping, expansion, logical operations and dirty marking. |

XP SP1 remains a2f6d7c93aa4f11efc51147566d5961c59a8ef89. Both faulty statements
are in selected original paths. These newly approved exceptions deliberately
add two new repair locations: D005 +2/-0 and D006 +1/-1 relative to the prior
local baseline, increasing anchored XP added-plus-deleted difference by four
lines. No excluded body, conditional framework, original formatting or line
ending is changed. The current manifest updates only the two local hashes;
original/subset identities remain untouched. No new implementation lives in
Compat or any public component.

## Minimal permanent regressions

The existing PIT fixture already programs actual ports and exercises modes
0/2/3 with divisors 1/16384/65536. Extend that same path after two heartbeats:
capture actual PIT state, assert the divisor1 periodic backlog exists, advance
the existing CCPU quick-event counter and dispatch when an event is active.
Require more IRQ delivery and a positive delay for any remaining active event.
There is no new test-only production API, timer or queue.

The existing VGA fixture selects real BIOS mode3 and controller write mode2,
then invokes the selected BIOS CHAIN2 byte writer. Mask4/value4/even offset0
must write plane2=ff; mask8 with the same value/address must leave plane2=5a.
Both cases preserve sentinel bytes in planes0/1/3. Tests use the original
selector and actual frame memory, not a copied write implementation.

Production changes +3/-1 (net +2); tests +35/-0. Public source/test trees,
guest binaries, media, INI and snapshots are unchanged. These tests prove the
named device paths, not a recorded DOS/Win3.1/Win95 customer fault or all PIT/
VGA modes. D005 deliberately controls dispatch ordering; it is not a claim
that a particular ordinary guest naturally reached the same timing.

## Similar-issue and independent review

Both issueIREQ0 scheduling divisions and timer_multiple_ints rescheduling were
read: the existing first-event guard already enforces positive delay, while
the active branch was the missing counterpart. The generic queue's intentional
zero-delay immediate-call behavior remains unchanged.

For D006, mode1 and mode2 byte/word branches, fill selection and per-plane move
gating in egwrtm12.c were checked. Plane2 consistently uses bit4 elsewhere;
odd-address plane3 uses bit8. No other same-class enable-bit correction was
identified in that bounded file scan. No claim about every generated video rule.

Counter-review checks actual code diff against these precise faults, immutable
XP reference and the owner-approved minimal shapes. Tests must distinguish
the unfixed functions from corrected functions before full qualification.

## Verification

The first x86 build exposed a new fixture declaration mismatch:
host_q_ev_set_count was not macro-mapped in that test translation unit.
The test now calls the already-exported c_cpu_q_ev_set_count with its actual
IU32 declaration. This setup failure is not a production failure or a pass.
The corrected build succeeds; the failed setup is recorded here before disposable
logs are removed.

Cached x86/x64 builds and both focused PIT/VGA tests pass after that correction.
Temporary counterexamples compile actual production bodies with just the
respective repair removed, then link before the unchanged production archive
into the same permanent test objects. They are task-owned diagnostic inputs,
never build-time transformations of shipped source. Both architectures reproduce the old timer stack overflow (0xC00000FD) and
the old plane-enable assertion failure. Corrected focused tests pass 2/2 per
architecture. The negative controls prove these tests detect the original
faults; they do not prove a particular customer symptom was caused by them.

The cached build first compiles 178 warning diagnostics on x86 and 195 on
x64, all inside the preserved mirror. The corrected incremental x86 build
has zero warnings; no diagnostic originates in owned/public production or
test code. Existing mirror warnings are retained within the approved scope.

Publication identities:

| Artifact | Compiler timestamp | SHA-256 |
| --- | --- | --- |
| softpc32.exe | Nov 14 2026 08:13:27 | 683409622d31fe5176a97f80642aebb5509d785ca3a1597fdfe3d54f15319f36 |
| softpc64.exe | Nov 14 2026 08:13:41 | 353e0d6e518dfd856fff37ec0773a0f2dcd6eebab9e78cf67382d42a794840a5 |

Reproducible scope count: git diff --numstat 894a92d2 -- src test.
Mirror CRLF and owned test LF were checked. Actual code review found no extra
interface, product special case, alternative queue/writer or public-tree change.
The bound of the similarity scan is this timer scheduling family and egwrtm12
plane gates, not all mirrored code. Existing zero-delay queue semantics and
all unrelated candidate repairs remain unchanged.

Final qualification: x86 188/188 background plus 8/8 native; x64 188/188
background plus 8/8 native, total 392/392, no test retries or skips. This
includes shared units, gates, product integration and native Console/audio/
package tests. Background elapsed times were 154.83s and 150.87s; native
15.25s and 15.16s. No fresh interactive DOS/Win3.1/Win95 manual validation is
claimed. XP manifest and documentation governance checks pass. A direct
manifest-gate invocation initially supplied an incorrectly parsed relative
argument; the explicit absolute source argument passes, as do both full-suite
gate executions. No source workaround was made for that command error.

Task-owned diagnostic copies, logs and media are removed after terminal runner
completion; persistent Ninja/ccache trees remain. P2 is an executor delivery,
not S2 closure or approval of the remaining six candidates.
