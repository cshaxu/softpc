# Project Status

## Current Work

T94 S3 is active: Base input ownership cleanup. The owner accepted and closed
S2 on 2026-10-17. S1
diagnosis/design is complete after delivery 20298b44 and separate review.
S2 implemented reset binding, physical DMA access and input-channel PCM gain
repairs. Delivery cd665a3d and review 7f64ec53 are pushed; T94 remains open. See the
[S1 review](../history/M9-T94-S1-win95-sb-diagnosis.md).

See the [T93 audit](../history/M9-T93-completion-audit.md),
[S15 closure](../history/M9-T93-S15-portable-sound-archive.md) and
[retained proposal](../history/M9-T93-xp-sound-blaster-standalone-proposal.md).
Historical requests/checkpoints are retained there and in the individual S
records, not as competing active packets in this status file.

## M9 T94 S3 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner accepts S2 and explicitly admits S3 input-definition ownership repair; no NXVM checkout access. |
| Objective | Move the sole neutral input contract to Base, remove hotkey aliases and unused native linking, and separate boolean delivery from native decode results without changing behavior. |
| Non-goals | No input algorithm, encoding, layout, hotkey, device, firmware, audio or media change; no compatibility alias or second decoder. |
| Reference Baseline | 7f64ec53; S2 owner accepted on 2026-10-17. |
| Candidate Proposal | [Win95 sound repair and admitted shared-input follow-up](../proposals/m9-win95-sound-blaster-recognition.md). |
| Files And ABI Surface | Lib Types/Base/KVM/Console headers and callers; Emulator, X86 IBM PC and App caller spelling; owner-local tests, manifests, documentation and dual EXEs. Source API rename and submit_input boolean return; values/layout unchanged. |
| Applicable Rules | Execution, Document, Architecture and Coding authorities and their governance skills; single-person executor/coordinator actual-change review. |
| Verification | Cached hidden x86/x64 full builds; input-focused regressions, complete shared units/gates and remaining units followed by integrations. Report real desktop/audio capability separately. Verify numeric/layout identity and complete caller scan. |
| Expected Markers | One Base input definition and decoder; no legacy input header/aliases; KVM retains only policy, synthesis and delivery; UNMAPPED Window translation unchanged. |
| Asset Needs | Ignored build/t94/s3 logs and exit marker; owned hidden job, 3600-second observation and 600-second no-progress budget, no interactive-timeout kill. Publish dual EXEs only; preserve configuration and media. |
| Reporting Requirements | Estimate about 40 caller files, production +500-700/-510-720 net -10-20 including relocation; test/gate net +30-60. Report actual production/test added/deleted/net and root infrastructure changes. |
| Stop Conditions | Unexpected numeric/layout/input behavior change, need for new decoder/state or external repository dependency: stop affected work and revise scope. |
| Exit Criteria | Ownership and caller migration, focused/full verification, manifest/dependency/naming gates, dual publication, complete executor commit/push and separate actual-diff review; await owner acceptance. |
| Original Owner Request | Close S2; admit a new S to implement sections A-E of the input ownership request, without reading NXVM, for exact downstream import. Preserve neutral behavior and legitimate Types CRT/SDK/atomic wrappers; no aliases or parallel paths. |
| Similar-Issue Sweep | Search all src/test and current documentation for legacy input types/constants/header and hotkey synonyms; enumerate submit_input callers and native User32 references; negative Types ownership cases protect the moved domain. |

## Current Technical Baseline

- T94 S2 is owner-accepted and closed (delivery cd665a3d, review 7f64ec53).
  Reset callbacks restore SB binding; four DSP accesses use physical SAS;
  PCM gain operates on input channels. Native Win95 WAV progress and
  post-play snapshot save/resume were instrumented successfully, not claimed
  from human listening. Details remain in the S2 history and proposal.
- S3 is implemented and verified, awaiting executor delivery and independent
  coordinator review before owner acceptance. Base owns the single neutral
  input header, native decoder and layout query; KVM retains hotkey/held-key,
  synthesis and event delivery. Six hotkey aliases and unused KVM user32
  linkage are removed. Numeric values and record layout are unchanged.
  Source consumers must rebuild for the renamed API and boolean submission.
- Latest qualification: x64 and x86 each pass 193/193 distinct registered
  tests: 177 units/gates, eight background integrations and eight native
  integrations (five desktop, three audio). Exact executed/registered name
  sets match, with no skipped or disabled tests. Final units include the
  extra left/right Alt cases and corrected documentation state.
- Earlier S3 x86 documentation-gate failure was an executor history-filename
  error, corrected and followed by a complete 177/177 rerun. Full-target
  cached builds pass; existing x86 Ninja log recovery causes unnecessary
  rebuilds and unchanged mirror warnings. This is not a zero-warning claim.
- Published assets/binary SHA-256:
  softpc32 BF8CDD793C762FA275D25AFAB0ABEA5B001C239E18B88B5CB31EC0A1391777C9;
  softpc64 2EB65B25A4F5E8EBA1CCCC9233C61D024D7A175BBACB354A3AA92F88CEC37DDE.
  User INI, media, snapshots and the XP mirror are untouched by S3.
- Reusable packages remain Lib, Emulator, X86 Debug and X86 IBM PC (eight
  source/test trees). No root test infrastructure changed. NXVM was neither
  read nor modified; downstream import and actual Linux/RDP runs are not
  inferred from Windows contract tests. Audio classification/implementation
  is unchanged; native passes establish this session's availability only.
- Queue remains empty. Independent deferred issues remain in TODO; no
  unrelated debt is declared fixed. The planned whole-T sound qualification
  and closure audit shifts to S4 and is not admitted by S3.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T93 | S1-S15 complete; owner accepts S15 and authorizes whole-T closure, 2026-10-16. Fresh dual-width 193/193 including all native integrations and four archive routes. | [Completion audit](../history/M9-T93-completion-audit.md) |
| T92 | Owner accepted and closed 2026-10-15; S1-S6 complete, dual-width 150/150 including all desktop tests. | [Completion audit](../history/M9-T92-completion-audit.md) |
| T91 | Owner accepted and closed 2026-10-15; all S1-S5 obligations complete. | [Completion audit](../history/M9-T91-completion-audit.md) |

Earlier closures and superseded technical checkpoints remain in history and
Git, rather than repeating stale active/delivery statements here.

## Recent Governance

T94 S1 admission consumes the sole queued candidate. Existing TODO items and
T93 closure evidence are unchanged. No rule is edited.
