# M9 T51 S4 — Shared-library normalization closure

## Closure decision

T51 is closed. The shared corpus now has the approved `types`, `console`,
`host`, `storage`, `ui-base`, `ui-window`, and `ui-console` component shape;
no legacy aggregate or forwarding compatibility surface remains.

## Final verification

- `verify_manifest.cmake` passed for the exact LF shared-library corpus.
- The Linux shared-library contract, source-boundary gate, and documentation
  governance verifier passed.
- The admitted old-name scan found no production/build reference to old paths,
  aggregate targets, or `ux_*` ABI spelling.
- Fresh fixed package builds and full CTest passed 36/36 on x64 and 36/36 on
  x86. The package build refreshed only `assets/binary/softpc64.exe` and
  `assets/binary/softpc32.exe`.
- The final diff has no `src/mvdm/` path, `softpc.ini`, or guest-media change.

## Change accounting

- S2's atomic migration commit is `d299f68`; it moved/renamed the library,
  all direct application consumers, test corpus, CMake wiring, manifest, and
  both agent-owned package executables as one buildable change.
- S3's authority/boundary commit is `3b76a84`; it aligned current design,
  source layout, UI authority, and component dependency documentation with the
  executable graph.
- This S4 closure records the final proof and retires the T51 proposal from
  the active queue. No behavioral or MVDM change was made.
