# NXVM source-local orphan-release import

## Objective

Synchronize the committed NXVM shared Lib fix that consumes a key-release event
when the receiving source never accepted that key's make. The matcher must not
replay pending modifiers or forward that orphan release.

## Selected upstream baseline

Read-only NXVM HEAD `572293efc` is clean. The selected shared implementation
is introduced by `064b9619b` and its key-specific clarification by
`268464d49`.

## Scope

Import exactly these five shared-corpus files:

- `src/lib/kvm-base/hotkey.c`
- `src/lib/kvm-base/README.md`
- `src/lib/MANIFEST.sha256`
- `test/lib/kvm_keyboard_lifetime_smoke.c`
- `test/lib/MANIFEST.sha256`

The change is one existing release branch: an unmatched release returns success
without delivery or pending-key replay. Tests cover direct matcher behavior and
both real Window/Console adapter paths after new-source and reset boundaries.

## Non-goals

No new state, API, input event, source-reset behavior, Common Session change,
App/Core change, configuration/media/snapshot change, or product policy.

## Verification

- Exact path/content/hash comparison of `src/lib` and `test/lib` against
  NXVM `572293efc` after import.
- Focused `kvm_keyboard_lifetime_smoke` on x64 and x86.
- Both Release packages and serial background CTest on x64 and x86.
- Shared manifests, Lib DAG, documentation governance, whitespace and
  changed-path review.

## Completion condition

SoftPC's five selected paths are byte-identical to NXVM's committed corpus,
the complete existing regression is green on both Windows widths, and no
unrelated path is retained.

## Executor evidence

- Clean committed NXVM baseline `572293efc` compares exactly: `src/lib` 109
  paths and `test/lib` 51 paths, including manifests.
- The focused `library.kvm_keyboard_lifetime` test passes on x64 and x86.
- Release packages build on both widths. Serial background CTest passes x64
  121/121 (122.31 s) and x86 121/121 (123.66 s); desktop-labelled tests remain
  excluded by the standard presets.
- Lib/test manifests, Lib dependency DAG, documentation governance and
  `git diff --check` pass.
- Counted C/H change: production +1/-1 (net 0) in the shared matcher; test
  +38/-0 (net +38). No Common/App/Core/INI/media/snapshot path changes.
- Refreshed package SHA-256: x86
  `B56538D327C255B29C402EE4DC15F65C717F0927C750F3C9EBE9BE53BD5DCB02`; x64
  `7364DF6F4210771A64A124D114A18599AEAAA019BA36A05926424B0C69AD964F`.
