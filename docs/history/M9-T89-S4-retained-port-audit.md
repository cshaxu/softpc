# M9 T89 S4: Retained-port and whole-task audit

## Request and boundary

Owner: "收口s3 准入s4". S3 closure/admission is 44fcc7e5 over accepted
1c698a9c. This S executes the final review selected in S1, not the superseded
candidate's unconditional removal of every standalone branch. Whole-T baseline
is 3c2e6e6939651187f17b97495b6018f82f258667. Read-only OpenNT remains
5e4619ab61c2aa76151e03973cce340be2933e61 at matching softpc.new paths.
No external source or license terms are imported.

Planned changes: no runtime logic; approximately +3/-6 comment lines, final
ledger and qualification. Actual S4 code change is reset.c **+3/-5, net -2**,
all comments. No test/build implementation changed. The comment now distinguishes
mouse-driver instance storage from guest INT33 installation and physical mouse
initialization. It does not enable the excluded DOS driver or its BOP entries.

## Frozen 36-directive ledger

S1 line numbers below identify the frozen occurrences, not current positions.
Re-running the same conditional/definition scan finds **29 directives in eight
files**. Seven were removed by S2/S3. The second nt_keycd directive was changed
from ifdef to ifndef when its extra helper was removed: it remains one directive,
not a removed occurrence. Every original member is accounted for below.

| Mirror path and S1 lines | Final disposition and receiver |
| --- | --- |
| host/src/nt_keycd.c: 11,442 | Both retained to exclude unavailable NT headers/OEM BIOS conversion. S2 deletes the extra Scan-1 helper; Compat and VM share original KeyMsgToKeyCode and its tables. keycode_smoke and irq_smoke cover table bounds, make/break and controller ingress. |
| host/src/nt_sound.c: 1,8,42,60,113,125,184,190,209,437 | All ten retained: NT declarations/name collision, executor locking, Beep handle lifecycle, native request parameters and the Compat sink. Original PlaySound/HostPpiState/Timer2/LazyBeep still own frequency/state. sound_state, sound, audio first-tone/delivery and lifecycle tests are receivers. No generic NT I/O emulation is added. |
| host/src/nt_graph.c: 773,1138,1578,1602,2113 | All five retained: original NT fullscreen/initialization assumptions do not describe detached DIB output; selected packed painter owns width units. Existing begin/end callbacks retain T88 publication boundaries. VGA fullscreen clear/cursor, mode/panning/CHAIN2 and snapshot tests remain receivers. |
| host/src/nt_cga.c: 22,41 | BIGWIN exclusion retained: host_def.h defines it after command-line macro processing; moving this would add another override. Header-selection directive removed by S2 through existing conapi.h. Text/CGA tests verify the same original renderer. |
| host/src/nt_ega.c: 38 | Removed by S2; original conapi.h resolves to the existing Compat endpoint. EGA/VGA frame regression retained. |
| host/src/nt_vga.c: 32 | Removed by S2; same endpoint, original VGA/V7 painters remain owner. VGA frame regression retained. |
| host/src/nt_munge.c: 27 | Removed by S2; original angle-bracket conapi.h lookup reaches the same endpoint. Planar/panning regression retained. |
| base/bios/reset.c: 527,751,838 | All three retained: CMOS-derived drive topology and excluded software-driver instance lifecycle. S4 corrects the inaccurate comment only. mouse_init remains separate. BOP test asserts all seven guest mouse-driver slots absent; mouse/media and boot/reset tests preserve physical hardware behavior. |
| base/keymouse/ppi.c: 70,137,168,212,228,234 | All six retained: complete PPI notification follows timer_gate, rather than the old edge-only path; snapshot read/restore preserves that same state contract. Moving sequencing to Compat would duplicate hardware state. Sound and snapshot receivers retained. |
| base/system/ica.c: 1362 | Retained: private PIC request withdrawal determines INTR, then calls existing host_clear_hw_int. Predicate belongs with adapter_state, not Compat. irq_smoke exercises rejected/withdrawn requests and keyboard IRQ release. |
| host/inc/cfpu_def.h: 160 | Removed by S3; small Compat override owns the same eight FENV operations. Original layouts remain owner. Both-width layout, rounding and exception tests pass. |
| host/inc/nt_graph.h: 212 | Removed by S3; small Compat override owns single-executor no-op lock policy. Original declarations remain owner. Include provenance and renderer tests pass. |
| base/inc/ios.h: 23 | Retained: 64 KiB I/O array profile, not a second I/O controller. Both compilers preprocess PC_IO_MEM_SIZE as 0x10000. Existing device I/O tests pass; no new exhaustive high-port test is claimed. |
| host/inc/host_def.h: 158 | Removed by S2; concrete CMake targets own standalone selection. Existing NTVDM/CPU-profile exclusions remain, as restoring them changes behavior. Build-ownership and independent-target tests pass. |

Scope scan: rg over all mirror conditional/define lines naming
SOFTPC_STANDALONE, current versus S1 line ledger, full T89 production diff,
original peer numstats, endpoint call sites and focused assertions. Seven
removed plus 29 retained/converted occurrences equal all 36. No new occurrence
or undispositioned member was found. Retained ports are intentional outcomes,
not deferred rewrites or a new TODO backlog.

The source audit reconfirms mouse_driver_initialisation allocates instance
storage; mouse_install1 patches interrupt vectors later. S1's diagnostic
evidence remains historical evidence, not a newly rerun DOS-driver experiment.
The current BOP, physical mouse and snapshot regression tests were rerun.

## Original-mirror and whole-T accounting

Method: git diff --numstat from 3c2e6e69 for current-tree changes; git diff
--no-index --ignore-space-at-eol --numstat against each OpenNT original for
mirror distance. These counts exclude artifacts and documents. They do not
claim a whole-mirror audit outside the fourteen-file frozen universe.

| Mirror file | OpenNT + / - before T89 | OpenNT + / - after S4 |
| --- | --- | --- |
| nt_keycd.c | 16 / 2 | 9 / 2 |
| nt_sound.c | 29 / 0 | 29 / 0 |
| nt_graph.c | 62 / 14 | 62 / 14 |
| nt_cga.c | 21 / 3 | 17 / 3 |
| nt_ega.c | 40 / 12 | 36 / 12 |
| nt_vga.c | 45 / 16 | 41 / 16 |
| nt_munge.c | 29 / 1 | 25 / 1 |
| reset.c | 14 / 6 | 12 / 6 |
| ppi.c | 41 / 3 | 41 / 3 |
| ica.c | 136 / 0 | 136 / 0 |
| cfpu_def.h | 58 / 1 | 35 / 0 |
| nt_graph.h | 9 / 2 | 4 / 2 |
| ios.h | 1 / 1 | 1 / 1 |
| host_def.h | 20 / 2 | 16 / 2 |
| Total | 521 / 63 | 464 / 62 |

Original-mirror added-plus-removed distance falls **584 to 526, down 58**:
S2 removes 27, S3 removes 29, S4's comment correction removes two.

Whole-T production: **+48/-78, net -30**, across fourteen changed paths:
nine mirror files (+5/-61) and five Compat files (+43/-17, including deleted
input.h). Runtime machine files and all six shared packages are unchanged.
Build/tests/checks: **+88/-7, net +81**, across root CMake, four tests
(keycode, irq, vga_frame, x87_layout), and two owner gates. Combined code:
**+136/-85, net +51**. The net test increase is not mirror growth.

## Qualification and remaining closure condition

Owned hidden build/t89/s4 supervisor completed with exit 0. Budgets: 30 minutes
per build, 20 minutes per test phase, 20 MiB output per phase. No Computer Use
or native foreground desktop case ran. Incremental build reused existing
directories; x64 executed 31 steps. x86 again reported the previously observed
Ninja premature-end recovery and rebuilt 577 steps; no cache fix or complete
cache-hit claim is made.

| Width | Shared background first | Remaining background | Real times |
| --- | --- | --- | --- |
| x64 | 84/84 | 47/47 | 60.03 s + 93.72 s |
| x86 | 84/84 | 47/47 | 67.46 s + 87.49 s |

The remaining suites include all eight App background integration tests per
width and the retained-port focused cases. Both package EXEs are refreshed;
owner INI, media and snapshots are unchanged. No new test/framework or hidden
guest behavior was needed. Documentation governance and diff whitespace checks
pass. Final changed-path review confirms reset.c differs only in its comment.

- softpc32.exe SHA256: `50900AC57EBA81A9B00558FDF424F73D047965A74E59864D697A890DFD67F3DC`.
- softpc64.exe SHA256: `A12EED74F50388CB4F15018529DAAE7688DFE67E878FA80ADD91E3D7D5780DF3`.

**Five native desktop cases per width remain unrun.** Their reserved desktop
qualification is still needed for the proposed whole-T full acceptance; this
background delivery does not waive it or close T89. S4's audit/comment repair
and background qualification are delivered for review, with this condition
explicitly retained in CURRENT. Existing RDP capture and historical BOP TODOs
remain unchanged and outside T89; the XP mirror proposal stays queued.
