# M9 T93: XP Sound Capability Completion Audit

## Request and audit boundary

Owner accepts S15 and requests whole-task closure: "好的，验收通过。接下来，
进行全T审计收口" and "全量编译和测试（单元测试，门禁，集成测试）都要通过噢！"
Fresh full qualification and the separate actual-change review of pushed
delivery 161305b2 pass. S15 and T93 are closed on 2026-10-16 under the owner's
acceptance and closure request. No next task is admitted.

Original scope: reusable Lib PCM/MIDI/FM host contracts and the necessary XP
Sound Blaster subset, with one existing device/executor path. The owner explicitly
prioritizes XP behavior/source fidelity, selects host MIDI with approximate
restoration, and accepts unsupported host FM without adding software synthesis.
Later approved scope includes shared quality repairs, platform neutrality,
portable sound archives, host-independent units and owner-aligned test layout.

The frozen universe is S1-S15; all changed production/test paths from admission
6ac9bf0b9 to reviewed delivery 91a3fde2; all registered tests in both Release
builds; current Queue/TODO obligations; and both published EXEs. A member must
have reviewed delivery and proof, or a recorded capability limit/separate
receiver. Passing tests do not settle an unverified guest-driver requirement.
Completion requires full receiving qualification, exact inventory accounting,
actual-change review and pushed closure records. No subsequent T is admitted.

## S-level convergence ledger

| Step | Delivered responsibility | Review/evidence receiver |
| --- | --- | --- |
| S1 | Pinned XP host-call census and revised PCM/MIDI/FM plan; no software synth or instrument bank. | [S1](M9-T93-S1-xp-host-audio.md), executor 2860b484. |
| S2 | Move PCM to audio/pcm and rename consumers without a second implementation. | [S2](M9-T93-S2-pcm-relocation.md), 7f642848; 150/150 each. |
| S3 | PCM short delivery, progress/epoch, native admission and stream gain using the existing worker/FIFO. | [S3](M9-T93-S3-neutral-pcm-contract.md), b31b6e17; 150/150 each and focused repetitions. |
| S4 | Copied host MIDI and explicit unsupported FM capability. | [S4](M9-T93-S4-neutral-midi-fm.md), 46ad2e3f; 154/154 each. |
| S5 | Existing DMA decrement/wrap transfer repair, no new controller. | [S5](M9-T93-S5-dma-transfer-repair.md), a34f858f; 154/154 each and real transfer matrix. |
| S6 | Selected XP DSP/PCM/Mixer with Compat host progress/EOI and existing executor pause hook. | [S6](M9-T93-S6-xp-dsp-pcm-mixer.md), 065ac9ed; 160/160 each. |
| S7 | Shared formatting/path/naming/Types quality repairs and approved NXVM import. | [S7](M9-T93-S7-shared-contract-quality.md), 7bb98d79/e1e44d0c; 161/161 each. |
| S8 | Selected XP MIDI/FM parser/device attachment and host lifetime. | [S8](M9-T93-S8-xp-midi-fm-attachment.md), 980984d0; 165/165 each. |
| S9 | Seven-area platform-neutrality audit and bounded repair split. | [S9](M9-T93-S9-platform-neutrality-audit.md), 896aa2fd/449c0646; baseline 165/165 each. |
| S10 | Base-owned native input normalization; neutral Console/KVM field semantics. | [S10](M9-T93-S10-neutral-input.md), e7d37b65/45d6f860; 165/165 each. |
| S11 | UTF-8 filesystem paths, native directory rules, Base-owned validator tests. | [S11](M9-T93-S11-utf8-paths.md), 33a9c9f7 and b9b654ac correction; 166/166 each plus affected-owner checks. |
| S12 | UTF-8 Console/title boundary and owner-approved local simplification. | [S12](M9-T93-S12-utf8-console-titles.md), 2fc25069/f43ea903; 167/167 each. |
| S13 | PCM consumed/submitted semantics and batch-independent destroy/flush contract. | [S13](M9-T93-S13-pcm-lifecycle.md), 9960b37f/e85a0015; 167/167 each. |
| S14 | Neutral capture ownership; corrected partial-acquisition cleanup. | [S14](M9-T93-S14-neutral-window-capture.md), 683f85d4/e2de3d3f; 167/167 each. |
| S15 | Portable sound state and cross-width restoration, isolated units, actual native integration and source-owner-aligned shared tests. | [Detailed S15 ledger](../history/M9-T93-xp-sound-blaster-standalone-proposal.md#s15--portable-archive-and-final-qualification), 66058ab4 through ac98a7b6, reviewed by 91a3fde2; owner accepted. |

Earlier failed attempts remain in the retained proposal. A later green run
does not retrospectively turn those attempts into passing evidence. Original
archive step numbers were superseded by the owner-approved insertions; S15 is
the actual archive delivery, not an omitted second archive implementation.

## Production ownership and compatibility audit

- Lib owns PCM/MIDI host resources and native input/text/path/capture adaptation.
  Emulator and X86 consume neutral values. No WinMM/WASAPI handle becomes a
  caller obligation, and no second audio worker or software synthesizer exists.
- The XP DSP/Mixer/MIDI/FM owners retain guest protocol/state. Compat supplies
  host output, EOI and fixed-width archive hooks; Machine retains assembly and
  lifecycle. The original DMA/PIC and I/O tables remain authoritative.
- The approved Emulator heartbeat hook parks host output at the executor
  boundary. The eight shared packages' gates test Types, naming, inward edges,
  corpus identity and negative cases. Their passes are finite boundary proof,
  not a claim of universal absence of defects.
- PCM state captures the frozen guest block/consumed prefix; restore recreates
  host resources and rebases progress before existing resume service. Queue/PIC
  ordering and the x86 queue-handle conversion are tested. No host pointer or
  handle is serialized, and no second serializer is introduced.
- UTF-8 strings and PC set-1 physical identifiers are explicit contracts.
  Unsupported Linux output/presenters remain unsupported. Windows tests of
  Linux leaves with injected calls do not establish a native Linux product.
- MIDI restores the existing logical parser/note/pedal model approximately;
  instrument/bank/pitch-controller history, SysEx side effects and exact host
  envelopes are not promised. FM without a suitable native host capability
  remains silent by approved design, not a simulated successful synthesizer.
- Snapshot device format changes intentionally do not support older snapshots.
  Native audio/device tests remain real integrations; non-audio units use
  test-owned sinks only. Product targets never link those sinks. Cross-owner
  units stay at the tested outer owner, not in integration merely for linking
  multiple components. Four flat negative fixtures preserve their bytes.

## Whole-T changed-path accounting

Reproduce with `git diff -M20% --numstat 6ac9bf0b9 91a3fde2 -- src test`.
Count C/H destinations after normalizing Git rename notation; deleted files
remain counted. Path counts include renames even when their body delta is zero.
Do not sum intermediate S totals: later simplifications and relocations would
be double-counted. Manifests, docs, CMake, scripts and binary assets are excluded.

| Production owner | Paths | Added | Removed | Net |
| --- | ---: | ---: | ---: | ---: |
| Lib | 60 | 1721 | 740 | +981 |
| Emulator | 9 | 264 | 254 | +10 |
| X86 Debug | 2 | 19 | 12 | +7 |
| X86 IBM PC | 5 | 45 | 80 | -35 |
| App product | 7 | 20 | 17 | +3 |
| App machine | 4 | 54 | 29 | +25 |
| Compat | 10 | 649 | 22 | +627 |
| Preserved XP source subset | 11 | 2915 | 16 | +2899 |
| Total | 108 | 5687 | 1170 | +4517 |

| Test owner | Paths | Added | Removed | Net |
| --- | ---: | ---: | ---: | ---: |
| Lib | 74 | 2647 | 949 | +1698 |
| Emulator | 18 | 312 | 130 | +182 |
| X86 Debug | 1 | 29 | 1 | +28 |
| X86 IBM PC | 3 | 97 | 18 | +79 |
| App | 18 | 2509 | 144 | +2365 |
| Total | 114 | 5594 | 1242 | +4352 |

The eleven mirror files are dma.h, ios.h, sndblst.h, at_dma.c, ica.c, nt_sb.h
and five nt_sb*.c files. The +2915/-16 is change from the pre-T local tree,
including newly selected XP bodies; it is NOT the local-patch distance from
XP. Pinned upstream/subset/local identities remain separate in the
[505-body manifest](../etc/evidence/softpc/xp-sp1-current-manifest.tsv) and
[S6](../etc/evidence/softpc/t93-s6-xp-sound-subset-ranges.tsv)/
[S8](../etc/evidence/softpc/t93-s8-xp-sound-subset-ranges.tsv) selection ledgers.
S15's five sound-owner changes add +324/-4 against unchanged selected subsets;
they are visible local patches, never excluded upstream material.

Root build/registration integrates existing objects and test sinks, not a
second runtime. Source checks recognize the new selected-subset labels and
the shared machine-composition contract's inward owner. The documentation
gate's explicit same-S owner-reopen handling includes positive/negative cases.
No rule file changes occurred in T93. Owner-edited Monitor text and snapshot
replacement/removal were explicitly included by request in 65dacdd5; subsequent
manifest synchronization 0dfc02cf repairs its disclosed temporary failures.
INI and guest media have no T93 delta. This closure does not modify either.

## Final qualification

The fresh closure job finishes with exit zero on 2026-10-16. Both complete
Release build targets pass, using existing caches rather than deleting them.
x64 has no work to rebuild; x86 recovers its already-recorded incomplete Ninja
log and rebuilds 687 actions, publishing its EXE. Source/test C/H is unchanged
from the owner-accepted delivery. This is a full-target cached build, not a
clean-cache claim. No interactive timeout stops a build or test.

| Qualification group | x64 | x86 |
| --- | ---: | ---: |
| Shared packages, including their corpus/manifest/Types/negative gates | 121/121 | 121/121 |
| All other background unit/integration/repository checks | 64/64 | 64/64 |
| Real native desktop | 5/5 | 5/5 |
| Real native audio | 3/3 | 3/3 |
| Complete registered inventory | 193/193 | 193/193 |
| Separate-process sound archive writer/reader combinations | Both readers pass | Both readers pass |

The independent name-set check compares the union of final JUnit cases with
unfiltered CTest JSON: no missing, foreign, duplicate, failed, disabled or
skipped cases. Each width contains 164 unit-labelled cases (including shared
gates), 13 repository checks and 16 integrations. Thus neither the eight native
cases nor the non-native integrations disappear behind background selectors.
All four x64/x86 archive routes pass again after full qualification.

Owned ignored evidence: closure-qualification shared/other/native JUnit files,
result.exit=0 and check.exit=0; closure.log/closure.err and closure-check.log.
The 121 shared cases split as Lib 76, Emulator 23, X86 Debug 12 and X86 IBM PC
10. The log records exact native names and results. Documentation/whitespace
gates are checked again after the final records change.

Current published SHA-256:

- softpc32.exe: `CA4D9B152C44E901354ADE7DB7635717B47BD11596B39E41F792BD584A60ACBE`
- softpc64.exe: `56BEA1E29C4EB06C48050D444E640631AFF7865203A2653F89AE13495D9E5CCF`

The INI and current owner snapshot retain respectively
`38D9EECD0A002FA6BF924D5F87BDA154C71D89A2790674965739179C2D56623D` and
`6E7127648EC188B92DA17D24669492E6D08DCB61B4D137F2AC3C36C586BF29E1`.
This round refreshes only the rebuilt x86 artifact; the verified x64 artifact
is byte-identical. Build log recovery and warnings from unchanged preserved
mirror files remain disclosed; this is not a zero-warning claim. The warning
path sweep finds no T93-changed C/H owner among those compiler warning sites.
No warning suppression or mirrored-source cleanup is added for closure.

## Separate final actual-change review

Coordinator inspects pushed 161305b2: only the audit, status checkpoint and
rebuilt x86 artifact change. Production and test source still equal reviewed
91a3fde2. Both artifact hashes match the final receiving builds; all final XML
names match CTest registration, both exit records are zero, and four archive
routes pass. Original request/S1-S15 mapping, owner-supplied asset changes,
necessary-subset ledger, known limits and all independent receivers are
rechecked before closure. No gate/assertion is relaxed to obtain this result.

The closure commit archives the proposal with Git provenance and fixes its
direct links, creates the S15 acceptance record and replaces accumulated stale
CURRENT narratives with one compact current baseline. History retains the
failed attempts and prior decisions. Current documentation and whitespace
checks pass before commit. This is the governance follow-up to the already
pushed complete P9, not a partially verified implementation delivery.

## Explicit limits and independent receivers

[Queue](../states/QUEUE.md) retains exactly one unnumbered task: real Windows 95
Sound Blaster recognition/playback repair. Owner-observed Code 24 is NOT fixed
or covered by a unit-suite pass. The owner separately queued that investigation;
T93 delivers XP-aligned capability, not successful Win95 driver installation.

[TODO](../states/TODO.md) retains six independent debts: Ninja log recovery;
compact-console raw input loss; aggregate checkpoint timeout; package child
startup failure; host-time-dependent INT 1Ah test oracle; and the previously
deferred x86 null-BOP investigation. No retries, skipped assertions or synthetic
success are added here. Fresh complete passes establish this receiving result,
not root-cause repairs for those intermittent observations.

Physical audibility, exact MIDI instruments/envelopes, unavailable FM hardware,
native Linux product support and downstream MyNES/NXVM qualification are not
inferred. These are disclosed scope/capability limits, not hidden S tasks.
The closure audit maps requests to existing detailed actual-change reviews
and independently checks final inventory, Git scope, artifacts and debt
receivers; it does not claim a new line-by-line proof of all historical code.
