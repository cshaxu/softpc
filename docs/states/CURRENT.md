# Project Status

## Current Work

No implementation subtask is active. Open task awaiting owner: T98.

Owner accepted and closed T98 S3 on 2026-10-19. Its exact NXVM shared import,
dual package refresh and 194/194 qualification per architecture are recorded
in [S3 history](../history/M9-T98-S3-shared-import.md). T98 remains open: the
former S3/S4 repairs are now S4/S5 and require separate owner admission.
Td S22 remains verified awaiting owner acceptance; its
[cache governance](../proposals/m9-build-cache-governance.md) delivery
f805483f is preserved, not implicitly owner-closed. User-owned untracked
assets/binary/snapshot.debug remains untouched/excluded.

## Current Technical Baseline

- T98 S3 optional Types math import: public source +12/-0, test C +10/-0,
  CMake +3/-0, docs +5/-0 and manifests +4/-3; total +34/-3 (net +31).
  No existing ABI or product/mirror implementation changes. All 194 tests
  pass per architecture with exact XML/registration equality and zero skips.
- S3 packages: softpc32 SHA-256
  704D873D121307CDAB765454C4CAF388C6AD221208B5FE12722D016C566C3873;
  softpc64 SHA-256
  78A32B672B1A2D49BF742C99EA6B2647FC364717B7BE4414CB73CFF54D0E4838.
- T98 S2 fixes Compat media close reporting and address bounds only. Production
  +30/-16 (net +14); tests +195/-0; root test registration +3/-2. Public and
  mirror code unchanged. Historical qualification: build/t98/s2/qualification,
  exit 0; raw logs retired by Td S22 after preserving the checkpoint;
  x64/x86 each 177 unit/gates, eight background integrations, five desktop and
  three native audio, exact XML/registration match. Existing Ninja recovery
  and original compiler warnings remain; no warning-removal claim is made.
- Historical accepted S2 packages: softpc32 SHA-256
  4E67DBD9AAEBE99BB874BA77E0BFA7DBE7819DE5927D920BE4998769089231AC;
  softpc64 SHA-256
  5C692E0DE7ADFDF9D1AEF2446D90C7C4B733FD3ECA589B94CBA33D2E89D216A8.
- T97 is investigation/documentation only; production, tests and source mirror
  have no changes. Installed Win95 driver rejects mixer/volume capabilities for
  DSP 2.01. The observed hardware-change confirmation explicitly requests
  shutdown; normal Restart succeeds twice. The owner's original installation
  dialog was not recorded, so its exact wording/action remains unverified.
- T96 runtime/reference: 5ea4e55e; S8 delivery a404740f and review 5ea4e55e.
  All 191 tests per architecture passed with zero failures/skips: 175 unit/gates,
  eight background integrations, five desktop and three native audio.
  Evidence: build/t96/s8/qualification, qualification.exit=0; exact CTest/XML
  registration equality was checked. Existing compiler/Ninja warnings remain.
- Previously accepted T96 package reference, without a T97 rebuild:
  softpc32 SHA-256 6A65D5127C6EBF567DCB7FD32F506541BF028FD42B69F9113CB09B363DF52C9A;
  softpc64 SHA-256 E1B95ED71ACB371098C854FEF6FD4E3573B71BE98169243958F13DD2900A53C1.
- Current transport: RGB graphics, separate leaf text contracts, Machine-owned
  fixed four-slot exchanges shared through opaque Base mechanics; Driver owns
  readiness and Machine publishes every produced frame. Session/UI route
  references. See current architecture and T96 audit for accounting.
- Win3.1 verified MIDI configuration is MPU330/IRQ7, SB220/IRQ5, all sixteen
  Mapper channels to MPU. [Product UI](../design/UI.md#windows-31-midi-playback)
  owns instructions. No FM or SB-MIDI capability was added.
- Existing [TODO](TODO.md) items remain open. No unrelated debt is repaired or
  removed by this investigation closure; the [Queue](QUEUE.md) is empty.

## Recent M9 Closures

| Task | Closure | Evidence |
| --- | --- | --- |
| T97 | Owner accepted S1 and T97, 2026-10-18. | [Audit](../history/M9-T97-completion-audit.md) |
| T96 | Owner accepted S8 and T96, 2026-10-17. | [Audit](../history/M9-T96-completion-audit.md) |
| T95 | Delegated closure, 2026-10-17. | [Audit](../history/M9-T95-completion-audit.md) |

## Recent Governance

- M9 Td S21 approved bounded frame ownership; see the
  [record](../history/M9-Td-S21-frame-ownership-transfer.md).
  T96 completed that implementation; no rule change accompanies T97 closure.
