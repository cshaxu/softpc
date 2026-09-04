# Project Status

## Current Work

| Field | Required record |
| --- | --- |
| Identifier Mode | M9 Td S1 closed |
| Admission And Approval | Owner directed that the standing prohibition on agent modification of `softpc.ini` be written into repository governance. |
| Objective | Make the package-configuration ownership rule enforceable and reachable from the mandatory task reading set. |
| Non-goals | No executable, build-system, source, artifact, guest-media, or configuration-file change. |
| Baseline | `artifacts/binary/softpc.ini` is user-maintained adjacent package configuration; the executable pair is the only agent-refreshable package output. |
| Affected Boundary | `docs/rules/EXECUTION.md`, its contributor pointer, and this current packet. |
| Subtask Plan | S1 record the binding rule once; link it from contributor guidance; run the documentation-governance gate. |
| Requirement Ledger | R1: forbid every form of agent mutation of the package INI. R2: reserve INI modification to the owner. R3: preserve agent packaging authority for the two executables only. |
| Focused Verification | Documentation governance check and `git diff --check`; confirm the user-owned INI is not staged. |
| Stop Conditions | Stop if the rule would contradict a higher authority or require a change to the INI itself. |
| Exit Criteria | A single binding execution rule is reachable from mandatory reading, contributor guidance does not duplicate it, validation passes, and no configuration file is staged. |
| Closure | Completed: the execution rule now protects `artifacts/binary/softpc.ini` from all agent mutation and permits package refresh only for the two executables. The contributor guide points to that single rule. |
| Original Owner Request | “禁止你再去修改softpc.ini!!” and “写入治理规范！” |

## Current Technical Baseline

- The fixed standalone package is `artifacts/binary/softpc32.exe` and
  `softpc64.exe`, with adjacent `softpc.ini` and guest media below
  `artifacts/media/`.
- M8 T19 completes the source layout as `src/{mvdm,overlay,host,app}` and
  removes transitional `src/core` and `src/vm` routes. Fresh GCC x64/x86
  builds each passed full CTest, 20/20.
- The selected original ROM inputs are byte-identical to OpenNT and embedded
  from `src/mvdm/softpc.new/roms/`; no runtime ROM artifact root is active.

## Recent Governance

- M9 Td S1 makes `artifacts/binary/softpc.ini` permanently user-owned package
  configuration: agents may refresh the executable pair but cannot modify,
  stage, or commit that INI.
- M8 Td S1 established the NXVM-style authority topology, linked the four
  applicable shared governance skills, and added the documentation gate.
- M8 Td S2 established the public product identity as Insignia SoftPC and
  added current, owner-provided SoftPC product captures to the root README.
- M8 T14 established a deterministic, 1,222-row source-map against the
  selected OpenNT revision. It found 60 direct differences and assigned every
  one a future overlay, host, or removal disposition without altering code.
- Implementation task identifiers are repository-wide build versions: the
  12 historical tasks end at T12 and M8 completes at T19. Td governance work
  does not consume a build-version number.
