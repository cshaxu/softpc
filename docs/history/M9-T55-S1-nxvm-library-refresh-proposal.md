# M9 T55 — Exact NXVM Library Refresh

> Historical admitted proposal. T55 S1 closed; see
> [the closure record](M9-T55-S1-nxvm-library-refresh-closure.md).

## Objective

Replace SoftPC's checked-in `src/lib/` corpus with the current NXVM corpus at
`64d211c9`, byte for byte, then make only the non-MVDM SoftPC adaptations
needed to compile, link, and retain its existing behavior.

## Scope

- Import the complete NXVM `src/lib/` tree unchanged, including its manifest.
- Adapt SoftPC's prompt-trace writer to the writer's explicit
  `{bytes, byte_count}` contract. Preserve the trace's deliberate line-ending
  behavior explicitly rather than relying on C text-mode translation.
- Replace the existing runtime-input restart smoke's unstable total-IRQ
  comparison with a non-repeating exact test-only scan-code marker. The
  assertion then proves whether first-run input reaches the second run,
  without treating ordinary BIOS/controller records or original typematic
  output as leaked input.
- Clear the completed standalone run's residual keyboard IRQ line after the
  original cold reset. This narrow host integration correction preserves the
  MVDM controller and guest warm-reset behavior; the next run receives only
  newly asserted input.
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
length-bearing API and preserve their intended bytes. The standalone cold-run
boundary removes an old IRQ1 and its invalid backing byte without modifying
MVDM. Fresh x64 and x86 builds and complete CTest pass; the restart smoke
checks its exact first-run scan set, library manifests verify, and the two
package EXEs are refreshed without modifying the INI or media.
