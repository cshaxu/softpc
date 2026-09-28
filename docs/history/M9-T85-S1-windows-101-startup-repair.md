# M9 T85 S1 — Windows 1.01 startup and InPort repair

Owner acceptance: 2026-09-28, "牛！验收通过 批准s1收口提交推送".
S1 is closed; T85 remains open. No next S is admitted.

## Request and finite completion ledger

The original request is to diagnose and repair Windows 1.01 startup, followed
by correct minimal InPort emulation and locating a compatible mouse driver.
The owner additionally approves a snapshot format change without backward
compatibility and subsequently authorizes replacement of the external test
HDD and EXE. The original HDD is backed up; INI is preserved.

The frozen scope is P1 through P3, based on `4a6aca27`, with these dispositions:

- Mono graphics: P1 `fc74d65e` preserves the original 1-bit painter contract
  and converts completed dirty pixels into Compat's published 8-bit surface.
  The 9-pixel regression covers the second packed source byte. Actual guest
  observation reaches MS-DOS Executive; no guest-version branch is added.
- InPort identity, deltas, buttons and HOLD: original register owner retained;
  invented 8255 echo removed, focused mouse tests cover the device path.
- Interrupt scheduling: one original quick event implements enabled timer
  rates; data IRQ and reset use the same controller, not a second input path.
- Snapshot: one semantic callback ID and timer handle replace obsolete state.
  Timer serialization, restoration and subsequent IRQ delivery pass. Old
  snapshots must be recreated, as explicitly approved.
- Driver: Microsoft Mouse 6.24's updated Windows 1.01 MOUSE.DRV is verified
  by installation and actual guest input. Provenance and hash are in the
  [proposal](../proposals/m9-windows-101-startup-repair.md). No downloaded driver
  or modified guest image is added to Git.
- Delivery: P2 `ee62ad01`, reviewed in P3 `9d102563`, contains dual EXEs.
  External x64 copy matches the package. The separately authorized test-HDD
  replacement carries the verified installation; owner testing passed.

## Actual-change review and line accounting

Coordinator reviewed the committed changes against the original request and
approved expansion. Reproduce counts with
`git diff --numstat 4a6aca27..9d102563 -- src test`.

| Counted path | Added | Removed |
| --- | ---: | ---: |
| src/app-softpc/compat/dib_surface.c | 73 | 10 |
| src/app-softpc/compat/dib_surface.h | 1 | 1 |
| src/app-softpc/compat/devices/archive.c | 9 | 6 |
| src/app-softpc/compat/devices/snapshot.h | 5 | 3 |
| src/app-softpc/softpc.new/base/keymouse/mouse.c | 70 | 80 |
| test/app-softpc/unit/machine/text_console_compat_smoke.c | 35 | 0 |
| test/app-softpc/unit/machine/checkpoint_smoke.c | 22 | 8 |
| test/app-softpc/unit/machine/mouse_smoke.c | 92 | 16 |

Whole-S production: +158/-100, net +58. Tests: +149/-24, net +125.
Combined C/H: +307/-124, net +183. Documentation and EXEs are excluded.
P2 alone is production +84/-89, net -5; that is not the whole-S count.
The only changed original-mirror file is mouse.c. Shared six-component
corpora and repository media/configuration remain unchanged.

## Verification and limits

Both Release builds pass. Final serial background CTest: x64 121/121
(124.02s), x86 121/121 (122.86s). Five desktop tests per width are excluded
by the preset; no Linux execution is claimed. Cross-process new-format
snapshots pass x64-to-x86 and x86-to-x64. Real updated-driver Win1.01 x64
movement, A: selection and File menu click pass; x86 movement/click reaches
the expected no-floppy error. Existing Win3.1 opens its Options menu without
driver replacement. Owner manual acceptance supplies final product evidence.

The proposal retains the similar-issue sweep of painter formats and InPort
identity/mode/HOLD/reset/timer/snapshot paths. No Windows-specific predicate,
second controller, host thread or shared API was introduced. No new deferred
item is produced; the unrelated pre-existing TODO remains unchanged.

Accepted EXE SHA-256:

- x86: `1C1D2CA700AE823FC0FDC0DD60A4B364E0E4C16CA24600312D77FBA3A23955C8`
- x64 and external copy: `9A01E522D40E9FF199133C62990248E516D8133D812F4E6577BA09BCF865130C`

Closure is documentation-only: no rebuild or repeated runtime test is claimed.
Documentation governance and whitespace checks are rerun. Task-local evidence
and the disposable installed-media source remain available while T85 is open;
they are not package dependencies. No probe process is left running by S1.
