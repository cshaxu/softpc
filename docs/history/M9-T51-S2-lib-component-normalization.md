# M9 T51 S2 — Shared-library component normalization

## Objective

Apply the admitted neutral component graph in one buildable migration: universal
`types`, neutral logical `console`, and the independent `ui-base`, `ui-window`,
and `ui-console` components.

## Completed change

- Moved the finite S1-ledger corpus with `git mv`; no forwarding headers,
  target aliases, or compatibility path remain.
- Extracted the former `base` scalar/status contract into the universal
  `types` interface target and moved the logical Console implementation into
  its own `console` component.
- Renamed the UI implementation and its public ABI from `ux-*` / `ux_*` to
  `ui-*` / `ui_*`, including every active SoftPC application and test consumer.
- Made the direct link graph explicit: `console -> types`, `host -> types +
  console`, `storage -> types`, `ui-base -> types`, `ui-window -> types +
  ui-base`, and `ui-console -> types + console + ui-base`.
- Refreshed only the agent-owned x86/x64 package executables. Configuration and
  guest media were not modified; MVDM was untouched.

## Evidence

- The manifest verifier, library Linux-contract verifier, source-boundary
  verifier, documentation-governance verifier, and old-name scan pass.
- Fresh fixed-width package/test builds passed all 36 CTest cases for both
  x64 and x86.
- The implementation diff contains 96 pre-existing tracked paths plus the
  new neutral Console README; 65 are library paths, 13 are application paths,
  and 14 are test paths. The remaining paths are root build wiring, active
  packet, and package executables.

## Non-goals

This was a path/name/dependency normalization only. It does not alter VM,
Console, Window, storage, or input behavior, and it does not modify the
preserved MVDM corpus.
