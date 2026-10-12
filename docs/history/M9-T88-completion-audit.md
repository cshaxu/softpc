# M9 T88 — completion audit

## Owner decision and frozen boundary

On 2026-10-11 the owner accepts S6 after x64/x86 each pass all 136 registered
tests, reviews the task-wide original-mirror accounting and the distinction
between original selected-marker machinery and new CHAIN2 unit adapters, then
explicitly approves: “合理，收口提交推送本t”.

T88 and S1--S6 close. No next task is admitted. This closure changes documents
only; runnable source/artifacts remain f1d394ca and final qualification is
a10fce4f. The frozen task comparison is b27bf7d0 through a10fce4f.
The [retained proposal](M9-T88-win31-file-manager-vga-width-transition.md)
holds the finite producer/geometry, colour-mode, generation and continuation
ledgers; this audit maps them to the actual final changes and owner decisions.

## S ledger and requirement disposition

| S | Delivered obligation | Final disposition |
| --- | --- | --- |
| S1 | Diagnose captured Win3.1 File Manager width changes without identifying the guest application or filtering Window geometry. | Producer evidence attributes CPU-chain4/scanout interpretation and incomplete-generation publication separately; owner accepts diagnosis and admits repair. [Record](M9-T88-S1-vga-width-diagnosis.md). |
| S2 | Keep V7 packed scanout independent of temporary CPU chain4 changes. | Painter selection, source stride and pointer conversion repaired at original boundaries; owner accepts. [Record](M9-T88-S2-v7-packed-scanout.md). |
| S3 | Cover other colours/resolutions and retain failures rather than mask them. | Nineteen mode matrix and V7 pointer coverage added; 63h/64h repaint failure transferred and subsequently repaired by S4. Also restores v7_ports predicate order (+1/-1). [Record](M9-T88-S3-colour-resolution-coverage.md). |
| S4 | Repair CHAIN2 dirty through the existing single writer/marker path. | Twenty-eight inline CGA blocks use selected-marker dispatch; 56 obsolete labels/wrappers removed; original Unchained EGA accounting reused with narrow CHAIN2 unit adapters. Owner accepts; no alternate writer. [Record](M9-T88-S4-chain2-dirty-contract.md). |
| S5 | Never publish a newly cleared surface from pointer-only damage before original body reconstruction. | One DIB-owned readiness fact and original full-refresh/update path; owner accepts and authorizes adding their existing snapshot unchanged. [Record](M9-T88-S5-compat-generation-readiness.md). |
| S6 | Fully explain and minimally repair the old nested CHAIN2 failure found by the matrix. | Forty-eight continuation sites individually proved and repaired by local goto; focused, background and native desktop qualification complete; owner accepts. [Record](M9-T88-S6-continuation-acceptance.md). |

The completion predicate is every above member accepted with its receiver and
final regression evidence, each changed production owner accounted for, no
unexplained admitted defect left in TODO, preserved public/asset boundaries,
and owner-approved closure. It is satisfied for this frozen task, not for all
possible VGA registers or all legacy generated entry points.

## Actual end-to-end changed paths

Counts include comments; they are final Git diffs, not sums of repeated edits.
Reproduce with git diff --numstat b27bf7d0 a10fce4f restricted to the indicated
roots. Documentation, generated outputs and assets are not code line counts.

### Original mirror: 12 existing files, +388/-1212, net -824

Paths are relative to src/app-softpc/softpc.new.

| File | Added | Removed | Net |
| --- | ---: | ---: | ---: |
| base/cvidc/ev_glue.c | 42 | 0 | +42 |
| base/cvidc/sevid000.c | 6 | 12 | -6 |
| base/cvidc/sevid001.c | 4 | 8 | -4 |
| base/cvidc/sevid016.c | 17 | 79 | -62 |
| base/cvidc/sevid017.c | 48 | 222 | -174 |
| base/cvidc/sevid018.c | 55 | 223 | -168 |
| base/cvidc/sevid019.c | 170 | 577 | -407 |
| base/cvidc/sevid020.c | 32 | 88 | -56 |
| base/video/gfx_updt.c | 6 | 0 | +6 |
| base/video/v7_ports.c | 1 | 1 | 0 |
| base/video/vga_mode.c | 6 | 1 | +5 |
| host/src/nt_graph.c | 1 | 1 | 0 |
| Total | 388 | 1212 | -824 |

The earlier owner-facing summary “S3 has no mirror change” is corrected:
S3 restores condition ordering in v7_ports.c (+1/-1, net zero). It revisits an
S2 line, so the final task range still totals +388/-1212 across the same 12
files; summing per-commit additions/deletions would count this churn twice.

Relative to OpenNT, these 12 files' existing plus new differences grow from
7917 to 9125 added/removed lines (+1208), not decrease. S2's net display repair,
S4's marker repair and S6's continuation repair respectively add 11, 1053 and
144 mirror-diff lines. Deleting original generated code still counts as mirror
difference. Owner approves these bounded repairs; the
[pristine ledger](../etc/evidence/softpc/pristine-divergence-current.md)
retains per-file reasons and comparison boundaries.

Original setMarkPointers, callback slots and unchained_mark_evid.str_mark are
reused. The CHAIN2 byte/word/string adapters, count/address conversion and
case-4 selection are the new 42 ev_glue lines, not misrepresented as unchanged
OpenNT code. They delegate bitmap/bounds/bank accounting to the original
marker; no second dirty algorithm or writer is created.

### Compat and verification

Compat production changes only dib_surface.c (+38/-4), dib_surface.h (+4/-2)
and v7_pointer.c (+4/-3): +46/-9, net +37. Surface ownership/readiness stays
with the existing DIB and original update callbacks; pointer overlays do not
qualify an incomplete generation. No frontend size filter, guest-specific
condition, delay, second renderer or runtime state owner is introduced.

Verification/build changes are text_console_compat_smoke.c (+31/-6),
vga_frame_smoke.c (+405/-5), chain2_rule_smoke.c (+116/-0),
standalone_source_boundary.cmake (+42/-0) and root CMake (+3/-1):
+597/-12, net +585 across five paths.

All production: +434/-1221, net -787 across 15 files.
Production plus verification/build: +1031/-1233, net -202 across 20 paths.
Machine/App production and all six shared source/test roots have zero task
diff. Snapshot format, media and package INI are not modified.

## Full qualification and accepted assets

Latest code-sensitive proof is S6 P3; subsequent P4 tests the same accepted
binaries and sources, without rebuilding or changing acceptance criteria.

| Selection | x64 | x86 |
| --- | --- | --- |
| Shared background | 84/84 | 84/84 |
| App/check background | 47/47 | 47/47 |
| Native desktop | 5/5 | 5/5 |
| Complete registered union | 136/136 | 136/136 |

Desktop cases are package_smoke, package_smoke/compact_console,
lib.kvm_window_retirement, lib.kvm_window_modal and lib.console_broker_display.
They run serially with reserved desktop authorization, without Computer Use.
The final desktop durations are 11.88/12.04 seconds and both exits are zero.
Focused VGA, CGA/EGA/VGA/V7 mode/pointer/dirty matrix and snapshot/integration
receivers pass on both widths. Owner manual acceptance is a separate explicit
decision, not inferred from program survival or a synthetic diagnostic.

Build/test jobs are owned with declared budgets and actual exit records.
The earlier native fixture header/oracle errors are corrected, not counted as
passes. x86 Ninja log recovery caused a 578-step build and a later 305-step
focused rebuild; this is explicitly disclosed, not claimed as each object
compiled only once or as a new deliberate clean-build policy.

Accepted package EXEs:

- softpc64.exe: 3094348 bytes, SHA-256
  99D8F132642A0A776E0F1E4FE6635D081FA3CE6CD145067AD171E3EF633259E3.
- softpc32.exe: 3494060 bytes, SHA-256
  386DE2AF207FA40845029D3540FC26212B47BC5CCE2F1B7D7035A5731D752D47.

The sole added asset is the owner's existing snapshot.sol, explicitly
authorized in S5 P2, 18404025 bytes and SHA-256
D171247173D4954CC41F4A17A606985FF71F9EA0337C5AD1FA2F6DB31E8F6594.
It was not agent-generated, replaced or converted by T88. INI and media have
zero task diff. Closure neither recompiles nor changes these artifact hashes.

## Remaining boundaries and independent review

The coordinator reviews final changed paths against each owner request,
authority, finite ledger and evidence, including the S3 ordering correction
and S4 new-adapter clarification above. Publication/generation and continuation
repairs converge into existing owners; no temporary diagnostic enters src/test
or the product build, and no in-scope duplicate production path remains.

The S3 repaint and S4 nested-continuation receivers are resolved by S4/S6,
respectively; they are no longer unexplained TODOs. The two existing unrelated
TODOs (intermittent RDP capture escape and x86 null BOP through BIOS[0x52])
remain explicitly deferred. Neither is claimed repaired or absorbed into T88.
Other generated continuation candidates and arbitrary bulk-entry algorithms
are not certified by the bounded 48-site fix.

The ordered queue remains unchanged: mirror/Compat boundary convergence,
then XP SP1 mirror upgrade. No next T/S is admitted by this closure.
Applicable final gates are documentation governance, reference repair and
Git diff-whitespace checks; runtime proof remains the accepted 136/136 evidence.
