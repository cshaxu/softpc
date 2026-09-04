# Project Status

## Current Work

| Field | Required record |
| --- | --- |
| Identifier Mode | M8 T18 S1 |
| Admission And Approval | Owner authorized sequential SoftPC queue execution through the active code-layout objective on 2026-09-03 and explicitly approved the completed T17 push on 2026-09-04. |
| Objective | Move the user-visible standalone application shell from `src/vm/` to `src/app/`, grouping monitor, runtime, input queue, and console/Win32 frontends while preserving the one-executor and copied-frame contracts. |
| Non-goals | No import or copy from `opennt-src-2`; no controller/BIOS/ROM/BOP/CCPU/C-VID semantic change; no generated source or compiler intermediate checked into `src/`; no public SDK, profile selection, or multi-session surface. |
| Baseline | T17 is closed and pushed at `afba203`: the host taxonomy is enforced, the recovered tree (including `src/mvdm/softpc.new/roms/`) remains reference-shaped, and GCC x64/x86 each passed 20/20 CTest. |
| Affected Boundary | Only repository-owned application-shell paths move from `src/vm/` to `src/app/`; `src/mvdm/softpc.new/` and its embedded BIOS/VGA/CMOS firmware remain unchanged. |
| Subtask Plan | S1 move the application shell with `git mv` and repair CMake/private includes; S2 prove executor and copied-frame ownership remains singular; S3 update source-boundary and direct-diff ledger evidence; S4 verify x64/x86 runtime, monitor, frontend, input, pause/resume, mouse-capture, and packaged launch behavior. |
| Requirement Ledger | R1: all user-visible shell code lives beneath `src/app/`. R2: exactly one executor mutates the machine and frontends consume copied snapshots. R3: app code has no direct CPU/RAM/controller/renderer state access. R4: CMake, tests, and the 98-row direct-diff ledger name the new paths. |
| Focused Verification | Static application ownership sweep; x64/x86 build and runtime/keyboard/window/monitor/pause-resume/mouse-capture/direct-launch tests; both packages launch from `artifacts/binary/` using only adjacent `softpc.ini`. |
| Stop Conditions | Stop for owner direction if the relocation needs machine semantics, a new host/device policy, a new runtime facility, a public SDK surface, or an unproved ABI representation. |
| Exit Criteria | `src/app/` contains every user-visible shell source, `src/vm/` is absent, application code has no direct machine-state access, and x64/x86 regression evidence matches the T17 baseline. |
| Original Owner Request | Persistent objective: “单人双角色模式执行SoftPC 的队列任务：代码布局优化。” T18 is the active ordered queue task. |

## Current Technical Baseline

- The fixed standalone package is `artifacts/binary/softpc32.exe` and
  `softpc64.exe`, with adjacent `softpc.ini` and guest media below
  `artifacts/media/`.
- M8 T13 separates `test/unit`, `test/integration`, and `test/support`; clean
  GCC x64/x86 CTest was 19/19 for each host width.
- The selected original ROM inputs are byte-identical to OpenNT and embedded
  from `src/mvdm/softpc.new/roms/`; no runtime ROM artifact root is active.

## Recent Governance

- M8 Td S1 established the NXVM-style authority topology, linked the four
  applicable shared governance skills, and added the documentation gate.
- M8 Td S2 established the public product identity as Insignia SoftPC and
  added current, owner-provided SoftPC product captures to the root README.
- M8 T14 established a deterministic, 1,222-row source-map against the
  selected OpenNT revision. It found 60 direct differences and assigned every
  one a future overlay, host, or removal disposition without altering code.
- Implementation task identifiers are repository-wide build versions: the
  12 historical tasks end at T12, the completed test-boundary work is M8 T13,
  and the queued work begins with M8 T14. Td governance work does not consume
  a build-version number.
