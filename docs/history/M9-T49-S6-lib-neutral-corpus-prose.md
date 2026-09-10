# M9 T49 S6 — Shared-library neutral corpus prose

## Original request

Reopen T49 and append S6: audit the complete `src/lib/` tree because the
shared corpus still contains product/machine terminology such as `guest`,
`VM`, and `machine` in component README files and comments.

## Objective

Make the complete reusable library corpus product-neutral.  No C, header,
README, or other checked-in `src/lib/` prose may use the conceptual terms
`guest`, `VM`, or `machine` as standalone words.

## Boundaries

- Only `src/lib/`, the source-boundary verifier, the library manifest, and the
  active/closure task records may change.
- This is terminology and verifier work.  It must not rename a public API,
  alter compiled behavior, or change SoftPC product semantics.
- Product-specific architecture outside `src/lib/` is not in scope.

## Baseline audit

The current full-tree word-boundary audit finds eight occurrences, all in
component/root README prose: `src/lib/README.md`, `ux-base/README.md`,
`ux-window/README.md`, and `ux-console/README.md`.  The prior S4 C/H corpus
scan is clean.

## Required work

1. Replace every remaining in-scope conceptual term with a neutral description
   such as application, content, raw Console, or component state, without
   weakening the documented ownership boundary.
2. Extend the existing standalone source-boundary verifier to scan every
   checked-in library `.c`, `.h`, and `.md` file for the case-insensitive
   standalone-word set `{guest, vm, machine}`.
3. Regenerate `src/lib/MANIFEST.sha256`.

## Verification and exit

- The recursive library scan has zero matches for the prohibited standalone
  words, while non-library product sources remain outside this check.
- The source-boundary verifier fails if a prohibited word is reintroduced into
  any in-scope file.
- Manifest and documentation governance checks pass.
- Strict Windows x64 and x86 builds and their full non-package CTest suites
  pass without behavior changes.
- The worktree is clean after committed, pushed implementation and closure
  records.

## Completion evidence

- The recursive C/H/README scan now has zero standalone-word matches for
  `guest`, `vm`, and `machine` in `src/lib/`.
- `standalone_source_boundary.cmake` now makes that complete corpus rule
  executable, in addition to the existing product-identity boundary.
- The library manifest, source-boundary check, and documentation governance
  check passed.
- Fresh strict Windows x64 and x86 builds each passed all 35 CTest cases,
  including their fixed-package smoke test.
