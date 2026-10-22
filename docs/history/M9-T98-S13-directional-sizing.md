# M9 T98 S13 Closure

Owner reports manual verification on 2026-10-22 and requests complete
integration qualification before closure. Qualification and coordinator review
are complete; S13 closes, T98 remains open. No next S is admitted.

Executor 1e07dcce is committed and pushed. The coordinator reviewed its actual
diff against the nine sizing cases, existing ownership and error paths, tests,
manifests, documentation and published artifacts. No further in-scope blocker
was found. This is not a claim of exhaustive correctness on every Windows host.

Production C/H +76/-10 (net +66); test C +108/-4 (net +104), measured against
c1d6e645. Changes belong to Lib Window geometry and one-to-one Types SDK
declarations. A private none/top/side value remembers the restore axis; scale
and correction still have their existing single owners. No public ABI,
polling, second renderer/input path, App or source-mirror modification.

Both cached builds and refreshed EXEs are verified. Each architecture has
passing evidence for exactly 192 registered tests, no skips. This includes
all 15 integrations rerun after RDP reconnection: x86 passes 15/15 first run;
x64 passes 14/15, then the unchanged compact Console case passes separately.
Its original stage 1/error 6 failure remains documented TODO, not repaired
or hidden by the supplemental pass. Background unit/gate evidence is 184/184
per width, including seven background integrations. Native groups contain
five desktop and three audio tests. Original mirror warnings remain allowed;
no non-mirror build warning was found.

Actual native eight-edge sizing/maximize/natural restore tests pass. Controlled
tests cover snap fit, top-width/side-height restoration, reentry and native
errors. A separate native probe establishes maximize/restore ordering, but
SendInput fails even with RDP Active; it does not prove Shell side gestures.
Owner manual verification is recorded separately. Side identification requires
Windows 10 1903+; older hosts keep width-led native restoration.

Published SHA-256:

- softpc32: E858D10FB8D328870802295FBDA75EFFC345C79E458A044D5C7461B0A1C6E762
- softpc64: 73429E55067820087ECF1BBA0D8194010A52F4B8502A6DC0013EB9BAEC206006

Task scratch is retired after evidence promotion; Ninja/ccache are retained.
User INI, media and the pre-existing untracked snapshot.debug are preserved.
See the [proposal](M9-T98-nonpublic-quality-performance-proposal.md#s13---manual-edges-and-native-snap-regions)
for the finite ledger and first-run failure disposition.
