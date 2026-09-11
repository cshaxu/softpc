# M9 T51 S3 — Normalized component boundary authority

## Objective

Make the current source-layout and component-boundary authorities say exactly
what the T51 S2 corpus builds, without reviving an aggregate component or a
legacy name.

## Completed change

- Current design now names `types`, `console`, `host`, `storage`, `ui-base`,
  `ui-window`, and `ui-console` and gives the only permitted direct library
  edges.
- `types` is universal; `ui-base` is a library dependency only of `ui-window`
  and `ui-console`. Application code may use `ui-base` public copied-value ABI
  where required, never its private implementation contracts.
- Current UI and source-layout authorities use the same `ui_*` names as the
  implementation, CMake targets, component READMEs, and boundary verifier.

## Evidence

- Scoped scans found no active old component path/name or `ux_*` ABI spelling
  in current design, active state, or library corpus.
- The source-boundary gate and documentation-governance verifier pass.
- This is authority and boundary documentation only: no library/application
  behavior, build asset, MVDM source, configuration, or media changed.
