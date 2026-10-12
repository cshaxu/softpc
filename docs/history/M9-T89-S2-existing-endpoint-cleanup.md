# M9 T89 S2: Reuse Existing Host Endpoints

## Admission and scope

Original owner request: "批准开始s2".
Baseline: `9bd90cf91e98bd8ace69606f954ac6e4ac6f86a6`.
The owner admits the S2 portion of the
[proposal](../proposals/m9-mvdm-compat-boundary-convergence.md), following
the [S1 audit](M9-T89-S1-mirror-compat-convergence.md).
[CURRENT](../states/CURRENT.md) alone owns current acceptance status.

Objective: reuse existing painter and keyboard endpoints, remove the redundant
lookup interface and move standalone selection to the concrete build target.
Non-goals: S3 overlays, guest behavior changes, shared-component edits,
new hardware or renderer paths, user configuration/media/snapshot changes.
The executed packet and verification contract are retained in CURRENT's
S2 delivery brief. The preliminary estimate was production +7--12/-43--49,
net -31--42; mirror approximately +1/-28, net -27.

## Actual implementation and finite ledger

| Path under src/app-softpc | Added | Removed | Reason / receiver |
| --- | ---: | ---: | --- |
| softpc.new/host/src/nt_cga.c | 0 | 4 | Restore original conapi include; retain CGA BIGWIN selection. |
| softpc.new/host/src/nt_ega.c | 0 | 4 | Restore original conapi include. |
| softpc.new/host/src/nt_vga.c | 0 | 4 | Restore original conapi include. |
| softpc.new/host/src/nt_munge.c | 0 | 4 | Restore original angle-bracket conapi include. |
| softpc.new/host/src/nt_keycd.c | 1 | 8 | Delete duplicate Scan-1 helper; keep original KeyMsgToKeyCode and unavailable NT conversion exclusion. |
| softpc.new/host/inc/host_def.h | 0 | 4 | Delete redundant standalone macro/comment; retain original blank separation and excluded NTVDM/CPU_30_STYLE. |
| compat/keyboard.c | 5 | 3 | Build one zeroed native record and call original KeyMsgToKeyCode; retain make/break delivery. |
| compat/platform.c | 0 | 1 | Remove unused include. |
| compat/input.h | 0 | 13 | Delete the now-unused duplicate lookup declaration/header. |
| **Production total** | **6** | **45** | **Net -39; nine paths.** |

The four painter translation units use InvalidateConsoleDIBits through the
same existing DIB contract. They do not call the other Console API names
remapped by conapi.h. No surface, geometry or transaction code changes.
The original key tables and previously corrected one-past-end checks are
unchanged. The Scan-1 boundary still masks bit 7 for lookup and passes it
separately as release; the record's zero control flags select the same table.
VM native key records already use KeyMsgToKeyCode and remain unchanged.

Root CMake changes +2/-2: softpc-machine exports only SOFTPC_STANDALONE=1 to
its concrete consumers. Pointer-width definitions remain private; original
OBJECT targets keep their existing private profiles. No reverse dependency
propagates this definition into shared Lib/Emulator/X86.

Tests/checks:

- keycode_smoke: +0/-4; only obsolete helper assertions removed. Full original
  table fingerprints, modifier/make/break lookup invariance and both OOB checks
  remain. No fixture state or failure coverage was removed.
- irq_smoke: +22/-0; real Compat Scan-1 input reaches the original controller
  and produces A make/break bytes at port 60h. Unmapped make and break are
  rejected without creating output. Existing IRQ/reset assertions remain.
- build_ownership gate: +4/-0; concrete consumers must inherit standalone.
- standalone_source_boundary gate: +11/-0; reject the duplicate lookup/header
  and header-owned standalone define, require original lookup at ingress.

Build/tests/checks total **+39/-6, net +33**, five paths. Combined counted
production/build/tests/checks: **+45/-51, net -6**, fourteen paths. Counts use
`git diff --numstat 9bd90cf9` with documentation and binaries excluded.
No public six-component path changes.

## Original-mirror accounting

Read-only comparison uses OpenNT `5e4619ab61c2aa76151e03973cce340be2933e61`,
same relative paths, `git diff --no-index --ignore-space-at-eol --numstat`.
These are whole-file differences including older accepted changes:

| File | Before + / - | After + / - | Fewer diff lines |
| --- | --- | --- | ---: |
| nt_cga.c | 21 / 3 | 17 / 3 | 4 |
| nt_ega.c | 40 / 12 | 36 / 12 | 4 |
| nt_vga.c | 45 / 16 | 41 / 16 | 4 |
| nt_munge.c | 29 / 1 | 25 / 1 | 4 |
| nt_keycd.c | 16 / 2 | 9 / 2 | 7 |
| host_def.h | 20 / 2 | 16 / 2 | 4 |
| **Total** | **171 / 36** | **144 / 36** | **27** |

Original CRLF spelling is retained; other changed text is LF. There is no
new mirror behavior branch, copied table, macro wrapper or algorithm.

## Similar-issue sweep and actual-change review

The frozen S2 universe is the six mirror files, all lookup callers/declarations,
the concrete compile targets and their test consumers. Search of src/app-softpc
and test/app-softpc finds no remaining softpc_host_scan1_to_key or Compat
input.h use. Other input.h files belong to their existing Machine owners.
All four restored conapi includes resolve to the existing Compat endpoint.
CMake retains private standalone definitions for original object groups and
the independent x87 layout test; those are distinct compiled consumers, not
competing runtime paths. PIC, PPI, audio, mouse initialization, renderer geometry,
T88 dirty/continuation fixes and S3 host-header policy are unchanged.

Role-switched review checks the actual changed paths and line deltas against
the approved S2 scope, not just test summaries. No out-of-scope repair or new
deferred functionality is introduced. The S1 history filename gains its S1
identifier with git mv because the existing continuation gate reads S numbers
from filenames; all direct links were updated, without changing the gate.

## Verification and artifacts

Both existing Ninja Release trees were rebuilt without deleting reusable
intermediates. Bounded hidden supervisors own logs, exit records and child
cleanup; no Computer Use or foreground desktop runner was used.

Focused irq/keycode/VGA/text tests pass **4/4 on both architectures**.
The first complete background run also passes **131/131 on both widths**.
An initial shared-only selection used a slash rather than the registered dot
separator and selected zero tests: that step is explicitly not acceptance
evidence. The final runner corrects the regex, adds --no-tests=error and runs
the required shared-first sequence:

| Architecture | Shared background | Remaining background | Real times |
| --- | --- | --- | --- |
| x64 | 84/84 | 47/47 | 72.06 s + 99.60 s |
| x86 | 84/84 | 47/47 | 68.09 s + 75.49 s |

The 47 include all eight background App integration cases: snapshot and
cross-process snapshot paths, runtime boot/restart, command and presentation
flows remain under the existing tests. Five native desktop cases per width
were not run (including the two native package integration cases); this is
complete background qualification, not all 136 cases.
The x86 Ninja cache reported premature-end recovery and rebuilt objects;
neither that recovery nor the final incremental build is described as a cache
hit. Cache repair is not folded into this source-cleanup task.

Documentation governance and git diff --check pass. No source/test change
follows these final test runs. Published artifacts:

- softpc64.exe SHA256: `557E756AAEC46D6352AA799BD71CFB079043FC54BACCF7276C55DD9F9A7F6E4C`.
- softpc32.exe SHA256: `DFD666BD0D466AAB3078771242E2258D5A5E41371FB52BBC0CBE7D9BE647A3A0`.

Owner INI, guest media and snapshots are unchanged. Small supervisor/log
evidence is retained under ignored build space; no new diagnostic executable
or raw trace is shipped. S2 is delivered for owner acceptance; S3 and T89
closure are not inferred.
