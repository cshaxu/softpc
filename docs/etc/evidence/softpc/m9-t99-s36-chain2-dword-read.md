# T99 S36 CHAIN2 DWORD read repair

## Admission and scope

Owner requests repair of S_2089_RdMode0Chain2DwordRead and
S_2101_RdMode1Chain2DwordRead: second WORD offset +2, explicit complete DWORD
composition, real controller regression, unchanged CHAIN2 layout. The NTVDM64
report is a research lead; no external patch was copied or used as a build
input. Baseline SoftPC 26d118e5; pinned XP SP1 remains
a2f6d7c93aa4f11efc51147566d5961c59a8ef89.

S35 startup investigation was explicitly withdrawn, with its uncommitted work
removed. S36 does not reuse that identifier or resume that investigation.

## Finite verification ledger

| Requirement | Evidence / disposition |
| --- | --- |
| Consecutive WORD addresses | Both existing calls retained; second address +4 becomes +2. |
| Complete return value | Saved IU16 low part OR returned IU16 shifted by 16 into IU32; no uninitialized upper bits. |
| Independent oracle | Six offsets 100/101/103/107/3fff/fffd, known physical bytes 11/22/33/44/55/66; mode 0 expects 44332211, mode 1 paired-plane intersection expects 40302010. |
| Existing state/ownership | Same GDP, allocation, saved locals, WORD/latch path and cleanup; no new state, interface or helper in production. |
| Real before failure | x64 original rule returned 00002211 rather than 44332211 at 100. |
| Odd mode-1 prerequisite | After DWORD-only fix, 101 returned 00300010 rather than 40302010; correct S_2100 next-group physical constants 4/6 to 8/10. |
| Focused corrected proof | x64 vga_frame_smoke passes (4.81 s); x86 passes (7.71 s). Includes existing graphics/text/frame and CHAIN2 continuation checks. |
| Full qualification | Both cached builds and 388 distinct architecture/test cases pass, including the corrected x64 documentation gate rerun; see below. |

Test setup uses the existing initialized Video7 CHAIN2 fixture. Its direct
physical-plane oracle does not reproduce the two-WORD implementation. An
initial attempted non-default mask variant exposed a separate GDP alias bug;
it is recorded below, not silently reclassified as passing coverage. Final
regression covers default mode-1 masks and differing paired plane values.

## Exact repair and mirror accounting

Production: one file, sevid000.c, +12/-20, net -8. Tests: existing
chain2_rule_smoke.c, +33/-0. No public-eight-tree changes, CMake changes,
snapshot layout change, extra renderer or guest-specific behavior.

Normalized LF comparison against the pinned XP original of this file:
prior local delta +501/-506; after +513/-526. Thus total raw mirror changed-line
count grows by 32, although local production code shrinks by eight. These are
different metrics. Mirror source retains CRLF; test/docs retain LF. The manifest
updates only this file's local hash; original/subset hashes stay unchanged.

## Similar-rule and actual-consumer review

Enumerated all eight DwordRead entries in sevid000.c and searched the C-VID
corpus for others. The two CHAIN2 rules share the lost-high-half operation and
are repaired identically. Mode-0 Unchained explicitly inserts its high WORD,
but passes +8 guest bytes to its second WORD reader, which itself scales by
four: recorded as separate source-derived debt pending a dynamic oracle.
Chain4 and mode-1 Unchained use direct byte/plane composition; disabled and
simple-memory entries have distinct behavior. No claim of exhaustive hardware
validation follows from this source review. Existing CHAIN2 odd dword-fill
value/tail debt remains outside this read task.

The actual CCPU route is c_sas_dw_at -> bios_read_double -> phy_r32, which reads
two WORDs at addr and addr+2. The generated DWORD rules remain registered in
read_mode0_evid/read_mode1_evid and reached by read_dword_ev_glue, but current
CCPU phy_r32 does not call that entry. The odd WORD repair can affect the current
CPU route. None of these observations proves a historic Win31 visual symptom.

Separate newly observed debt: softpc_gdp_rule_slot maps full colour_comp and
dont_care fields (1376/1380), but byte reads at 1379/1383 allocate independent
slots. Whole-field mask changes were dynamically ineffective in the attempted
mode-1 variant. Recorded in TODO for an owner-local subfield alias audit; no
read-side compensation or broad adapter rewrite in this S.

## Review and delivery

Both cached Ninja/ccache builds completed and published the two EXEs. Each full
build reused the focused compile and performed 30 remaining link actions.
This incremental compilation reported zero warnings/errors; it is not a claim
of a clean rebuild of every unchanged mirror translation unit.

- x64 background initial 185/186 in 79.76 s: only documentation governance
  failed because the withdrawn S35 identifier lacked its history record.
  Added the truthful withdrawn record; targeted gate passes 1/1 (0.15 s).
- x86 background 186/186 in 81.95 s.
- Native desktop/audio/package: x64 8/8 (25.32 s), x86 8/8 (24.42 s).
- Final qualified universe: 388 distinct architecture/test cases, all passing.
  The intentional pre-fix regression failed; it is not a remaining failure.
- Latest shared comparison: all 296 files across the eight trees are identical
  to NXVM. No public source/test changes.
- Mirror CRLF only, autonomous test LF only; original trailing tabs are retained
  on changed generated constant lines, without unrelated formatting churn.

Published SHA-256:

| Artifact | SHA-256 |
| --- | --- |
| assets/binary/softpc32.exe | 65eb826debc13cdd84b341ce6de1d6b261b6e9f132b30857d6af62ff90284104 |
| assets/binary/softpc64.exe | 3a7f5f284b2e26e3da6810c4366664a5bb594313428be0d5fb1256871b3ed0d6 |

Executor self-review and separate coordinator actual-diff review checked all
changed source/test lines, mirror identity, owner request and evidence. The
only three production rules changed are the two DWORD readers and their odd
mode-1 WORD dependency. Both DWORD repairs use the same expression; no helper,
state, new branch, alternative path, allocation or ownership change was added.
Low/high casts are explicitly 16 -> 32 bits before shifting, valid on both host
widths. The independent data oracle detects both lost-high and skipped-WORD
errors and the actual odd plane-spacing omission. All prior tests are retained.

The two additional TODO findings are separate and explicitly limited: the
mode-1 mask alias failure was observed, whereas the Unchained +8 issue is source
inference only. Neither was silently fixed, declared covered, or attributed to
the prior Win31 visual report. No new real Win31/Win95 manual acceptance is
claimed. Existing production snapshot format, media and INI are unchanged.

Task-owned build/t99-s36 logs/scripts are disposable after this promotion.
Owner accepted and closed S36 on 2026-11-07; delivery is 8c27a3b6.
See [closure](../../../history/M9-T99-S36-chain2-dword-read.md). T99 stays open.
