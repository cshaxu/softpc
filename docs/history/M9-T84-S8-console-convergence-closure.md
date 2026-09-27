# M9 T84 S8 — Console convergence closure

The owner accepted S8 on 2026-09-27. P1 `b8c1ed3f` separates native Console
backing-store capacity from viewport geometry. P2 `b79769c1` retains an
independent private coverage extent so a failed 50-to-25 write can be retried
through the former tail without treating its incomplete frame as cached.

The sole retained output path prepares backing storage, writes exactly the
larger of the current frame and confirmed/attempted coverage, and commits the
completed-frame cache only after a complete native write. A native backing
shrink clamps coverage even when the next frame itself fits. Viewport, font
fit and scroll position remain host-owned.

Against S7 `5d7b2619`, the final S8 production/test source delta is:
`src/lib/console-broker/win32/console.c` +25/-35,
`src/lib/README.md` +5/-5,
`test/lib/console_broker_display_smoke.c` +14/-16, and
`test/lib/lib_console_io_contract_smoke.c` +58/-11: combined +102/-67,
net +35. The extra private state is necessary because completed-frame cache
validity and native rows still requiring cleanup are distinct facts; no public
API, Common, App, Core mirror, configuration, media or snapshot format changed.

Focused broker contracts cover viewport preservation, backing growth failure,
steady 25-row output, 50-to-25 tail clearing, partial-write retry and an
external 50-to-30 backing shrink. Both Release packages build. Hidden
background CTest passes 121/121 on x64 (301.17 s) and 121/121 on x86
(251.51 s); desktop-labelled tests remain excluded. Lib/test manifests,
component DAG, corpus, documentation-governance and whitespace gates pass.
The accepted package hashes are x86
`BFE2E571A4E10407AD04A9C456AFF2C174268F41E077C33CC534578DBE8AD4EC` and x64
`1A3D4DD12419EA954271932FA4B19EFED083DCCFD2E18EB76C801A204D927003`.
