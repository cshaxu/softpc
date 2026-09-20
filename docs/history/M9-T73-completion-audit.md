# M9 T73 Completion Audit

## Authority And Frozen Scope

Original request: audit Lib/Common x86 dependencies for reuse by another emulator,
using NNES only as an initial reference. The owner retained host-PC input and KVM
capacities, approved neutral Machine debug transport, then revised the package
layout to src/lib, src/common, src/x86 and their three matching test directories.
The four Lib/Common directories must also build/test with x86 entirely absent.
No receiving emulator implementation, new generic debugger or Lib change was
requested. The [retained proposal](M9-T73-shared-x86-dependency-audit-proposal.md)
preserves original requests, superseded plans and the final five-step ledger.

Owner now reports: 测试通过，收口S5. 然后验收T任务（如果没有其他S任务）。
Frozen universe: every admitted S1--S5, original/revised requirements, changed
production paths, shared build/test ownership and explicit follow-up receivers.
All five steps are accepted; no S6 or other T73 implementation remains. This is
the separate T-level audit, not acceptance inferred from one passing test.

## Requirement And S Coverage

| Member | Result and evidence |
| --- | --- |
| Lib unchanged reuse | src/lib and test/lib have zero endpoint diff. Host input mapping remains the receiving adapter's responsibility; supported host/capacities remain explicit. |
| S1 naming | [S1](M9-T73-S1-x86-component-rename.md), 03a954dd: mechanical explicit x86 names, preserved commands and callers, dual-width 105/105 and subsequent owner acceptance. |
| S2 audit | [S2](M9-T73-S2-machine-x86-audit.md), 471b089c/2c724e10: all 22 Machine APIs and 16 callbacks, Session/UI and consumers inventoried; protocol coupling separated from neutral mechanisms. Owner approved migration and closure. |
| S3 neutral Machine | [S3](M9-T73-S3-neutral-machine-debug.md), 006ecf32/dc06671d: bounded copied bytes use the existing paused lease/executor; aligned x86 adapters retain validation. Dual-width 105/105 and owner acceptance. |
| S4 x86 corpus | [S4](M9-T73-S4-x86-source-corpus.md), 0ef82055/fb07a0b7: debugger, assembler and protocol live under src/x86, with explicit product inclusion and own manifest/DAG. Mechanical relocation proof, dual-width 108/108 and owner acceptance. |
| S5 three test packages | [S5](M9-T73-S5-shared-test-corpora.md), 879c30ac/ad665a86: no mixed neutral/x86 test dependency; one neutral fixture, independent four-/six-directory proofs, final dual-width 110/110 and current owner acceptance. |
| Semantics and ownership | DOS/X CLI style and commands remain x86-owned. Machine has one executor and opaque rendezvous; VM interprets CPU operations; Session/UI runtime unchanged. No second parser, dispatch registry, executor or compatibility tree. |
| Open findings | Immediate debug-close/paused-destroy wake race is explicitly owned by the unadmitted terminal-wake Queue proposal; it is not certified fixed. Test-injection ordering was corrected within S5 with a completion event, not a production workaround. |
| Other Queue/TODO | Floppy identification, mirror rebase and overlay lookup remain separate candidates. TODO stays empty under owner policy. No admitted migration was moved out to make this T appear complete. |

## Endpoint Accounting And Actual-Change Review

Recomputed `git diff --numstat -M10% ad66b615 ad665a86`, tracked C/H only,
excluding documentation, manifests, build/gate scripts and artifacts:

| Group | Changed paths, rename-aware | Added | Removed | Net |
| --- | ---: | ---: | ---: | ---: |
| Production C/H | 19 | 487 | 428 | +59 |
| Test C/H | 10 | 1104 | 819 | +285 |

These are endpoint counts, not sums of the intermediate renames. Production
changes cover App command names/includes, VM debug/driver protocol adapters,
Machine payload/storage checks, and relocated x86 declarations/commands. CLI
and VM dispatch preservation is proved by the per-S reverse comparisons.
Lib, Common Session/UI, Compat, MVDM, user INI and guest media have zero diff.
Machine's fixed 128/1536-byte slots replace typed storage; no per-request heap
allocation, new worker, CPU schema or operation registry was introduced.

The coordinator review checked actual Git paths, every admitted step, the final
source/test membership and build boundaries, protected-path equality, package
hashes, all-S reviews and current user acceptance. Source/test manifests and
DAG/negative-edge checks remain independently available. Historical names in
archived plans are provenance, not retained production aliases.

## Verification, Limits And Closure

Final accepted baseline: ad665a86. Both Release builds passed; background x64
110/110 (181.72s), x86 110/110 (165.87s). Five desktop cases per width were
excluded, not claimed passed. Native Common/x86 tests each passed 50 repetitions
per width. Isolated neutral set: Lib 41/41, Common 18/18; isolated x86 set:
Lib 41/41, Common 18/18, x86 9/9. Tests need no product adapters/ROM/media.
This does not claim NEC integration, Linux presenter support or universal absence
of concurrency defects; the separately queued race remains disclosed.

Closure rechecked the accepted artifacts:

- softpc32.exe SHA256: 5ACA0FD034D59D37B576867435D60419EF4ED31A8CCD6BBAC2BC9C0C330327A0
- softpc64.exe SHA256: AE59961C7BEDC9EFD0F5FCFF813824EBC4D4AC2D12117E6A0DE7948766467B09

S5 and T73 are closed under the owner's acceptance. No next task is admitted.
Closure is documentation-only: no rebuild, new process, medium, source/test or
EXE change. Existing S5 full-regression evidence applies to the unchanged code;
archival/reference, corpus and documentation gates are rechecked at closure.

Closure validation passed: documentation governance, all T73 Markdown links,
whitespace, and x64's documentation/self-test plus Common/x86 manifest, corpus
and negative probes (10/10, 12.94s). The closure diff contains no source, test,
build-configuration or artifact changes. Previous accepted full-regression and
manual-test results remain applicable; no new full dynamic run is claimed.
