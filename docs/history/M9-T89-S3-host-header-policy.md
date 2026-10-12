# M9 T89 S3: Host-header policy ownership

## Request, boundary and delivery

Original owner requests: "验收通过 收口s任务 准入下一个s" and
"继续啊为啥不做". S2 is accepted; S3 implements the already-admitted narrow
header step. T89 remains open and S4 is not admitted. S3 awaits acceptance.

Baseline: c6119a65 governance over accepted 90850aab production. The read-only
OpenNT reference remains 5e4619ab61c2aa76151e03973cce340be2933e61, at the
corresponding nt/private/mvdm/softpc.new/host/inc paths. Existing historical
notices and remaining port/layout changes are untouched; no new external
implementation or license claim is imported.

The admitted design restores just the original cfpu_def.h NTVDM conditional
and nt_graph.h mutex block. Two small Compat headers include the original
headers and replace the eight existing Host FENV macros and two existing
single-executor no-op lock macros. Original types, structures, algorithms,
painter declarations and endian fixes retain their existing owners.
There is no new state, API, threading policy, guest behavior or snapshot format.

The estimate was mirror +1/-28, net -27; Compat +45--65; total production
net +18--38; build/tests +40--80/-2--8. Actuals are smaller because the
wrappers need only the original include, macro replacements and short comments.

## Changed-path accounting

Counts use git diff --numstat against c6119a65, including new files, excluding
documents and executable artifacts. Original-mirror counts below instead use
git diff --no-index --numstat against each OpenNT original.

| Path | Added | Removed | Net |
| --- | ---: | ---: | ---: |
| src/app-softpc/softpc.new/host/inc/cfpu_def.h | 1 | 23 | -22 |
| src/app-softpc/softpc.new/host/inc/nt_graph.h | 0 | 5 | -5 |
| src/app-softpc/compat/ccpu/cfpu_def.h | 26 | 0 | +26 |
| src/app-softpc/compat/nt_graph.h | 12 | 0 | +12 |
| Production total | 39 | 28 | +11 |
| CMakeLists.txt | 3 | 1 | +2 |
| test/app-softpc/unit/softpc.new/x87_layout_smoke.c | 35 | 0 | +35 |
| test/app-softpc/unit/machine/vga_frame_smoke.c | 3 | 0 | +3 |
| tools/checks/standalone_source_boundary.cmake | 8 | 0 | +8 |
| Build/test/check total | 49 | 1 | +48 |
| Combined code total | 88 | 29 | +59 |

Against OpenNT, cfpu_def.h changes from +58/-1 to +35/-0; nt_graph.h changes
from +9/-2 to +4/-2. Total mirror added-plus-removed lines fall from 70 to 41:
**29 fewer lines of original-mirror diff**, distinct from the current patch's
net removal of 27 mirror lines. Remaining FPU layout and painter declaration
differences are deliberately retained.

## Finite include and similar-issue audit

The frozen surface is every cfpu_def.h/nt_graph.h include and its effective
compiled consumer. rg finds two CCPU, five original renderer, four Compat
and two direct test sites. The platform failure test also includes platform.c,
giving fourteen compiled consumers. Both widths resolve Compat then original:

- FENV: fpu.c, zfrsrvd.c and x87_layout_smoke.c.
- Graph: nt_ega.c, nt_graph.c, nt_munge.c, nt_cga.c, nt_vga.c;
  graphics_console_compat.c, dib_surface.c, video.c, platform.c;
  vga_frame_smoke.c and platform_failure_smoke.c.

Production include order already selects Compat. Only the two independent
test target include lists need adjustment. No same-directory original-header
include bypass was found. x64 Ninja dependency records prove the fourteen
consumers. x86 Ninja reports the known premature-end cache recovery; its stale
dependency records are not evidence. Direct i686 compiler -M output using the
actual Ninja target definitions/includes independently proves all fourteen.
The first diagnostic filename filter also selected original base/video/video.c;
the final audit uses the actual Compat video path, not that unrelated file.

The permanent source-boundary gate rejects SOFTPC_STANDALONE policy in these
two restored headers. No other mirror conditional is silently migrated;
the remaining finite T89 ledger stays assigned to S4.

## Verification and artifacts

Owned hidden supervisors completed within declared budgets: 30 minutes per
build, 20 minutes per test phase, 20 MiB per phase output. No foreground
desktop or Computer Use operation ran. Ignored build/t89/s3 retains bounded
logs/exit records and include evidence; no new diagnostic executable ships.

Both Release builds passed and refreshed the two package EXEs. x64 rebuilt
48 steps; x86 rebuilt 577 after Ninja's existing cache recovery. This is not
reported as a complete cache-hit build or a cache repair.

| Width | Focused | Shared background first | Other background | Suite real times |
| --- | --- | --- | --- | --- |
| x64 | 8/8 | 84/84 | 47/47 | 60.73 s + 93.87 s |
| x86 | 8/8 | 84/84 | 47/47 | 59.73 s + 70.49 s |

Focused coverage retains FP layout assertions and adds all four rounding
selections, overflow/underflow/inexact observation and exception clearing,
restoring the saved host environment afterward. The renderer test exercises
the no-op lock contract with no native mutex. Existing video/text/cursor and
snapshot cases pass. All eight background App integration cases pass per
width. Five native desktop tests per width were not run; 131/131 background
passes must not be described as all 136 registered tests passing.

Documentation governance and git diff --check pass. Self-review confirms
the sole original type/renderer owners, unchanged macro expansions, preserved
mirror CRLF, and LF for new standalone headers. Shared six-component source
and tests, owner INI, media and snapshots are unchanged.

- softpc32.exe SHA256: `5171FC9D96D6439FC67033CA3E2604B5EA7F5B078D12E107A8E6C4448B2E9C1F`.
- softpc64.exe SHA256: `5E6147547A9B241473A144415ECF38370699659131E3978D267709289316DE63`.

P1 delivers implementation, tests, artifacts and this audit together. Owner
acceptance, S3 closure and admission of S4 are not inferred from these tests.
