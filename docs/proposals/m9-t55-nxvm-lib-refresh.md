# M9 T55 — Exact NXVM Library Refresh

## Objective

Replace SoftPC's checked-in `src/lib/` corpus with the current NXVM corpus at
`64d211c9`, byte for byte, then make only the non-MVDM SoftPC adaptations
needed to compile, link, and retain its existing behavior.

## Scope

- Import the complete NXVM `src/lib/` tree unchanged, including its manifest.
- Adapt SoftPC's prompt-trace writer to the writer's explicit
  `{bytes, byte_count}` contract. Preserve the trace's deliberate line-ending
  behavior explicitly rather than relying on C text-mode translation.
- Prove exact corpus equality against the frozen NXVM source, build and test
  both package widths, and refresh only the two agent-owned package executables.

## Boundaries

`src/mvdm/softpc.new/`, guest media, and user-owned
`assets/binary/softpc.ini` are out of scope. This task does not refactor
SoftPC's general C-runtime calls to the new `types` vocabulary, does not alter
NXVM, and does not invent a SoftPC-specific variant of any library file.

## Acceptance

After completion, every `src/lib/` path and SHA-256 equals the frozen NXVM
`src/lib/` corpus at `64d211c9`; no source change remains under SoftPC's
library directory. The two existing SoftPC prompt-trace writer calls use the
length-bearing API and preserve their intended bytes. Fresh x64 and x86 builds
and complete CTest pass, library manifests verify, and the two package EXEs
are refreshed without modifying the INI or media.
