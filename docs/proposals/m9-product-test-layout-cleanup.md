# SoftPC Product Test Ownership And Layout Cleanup

## Status and owner request

Owner accepted T76 and requested "测试通过 收口提交t任务 准入下一个".
Admitted as T77, beginning with S1. CURRENT.md owns the executable packet;
this proposal retains the bounded plan and evidence ledger. S2/S3 are planned,
not simultaneously active. T closure requires owner acceptance.

Original owner requests:

> softpc的test里面太谜了：有integration，合理，所有集成测试；有 common, lib, x86，给这3个公共组件，也合理；但还有support和unit这两个目录，是干嘛的？

> 将这个整理工作加入队列首位作为新的T任务proposal

## Findings and goal

The inspected tree has 33 C files under `test/unit`. They test SoftPC App and
Core behavior; some assemble a real machine and are not isolated unit tests.
`test/support` mixes product machine fixtures, legacy `app_runtime_*` and
`app_input_queue_*` adapters, CMake boundary checks, cross-process snapshot
orchestration, documentation, and two unregistered manual diagnostics.

Organize these files by actual ownership and test purpose, remove redundant
test-only interfaces, and make the test inventory understandable without
changing production behavior or reducing verification coverage.

## Proposed ownership

| Location | Responsibility |
| --- | --- |
| `test/lib`, `test/common`, `test/x86` | Existing independently reusable test corpora; unchanged by this task |
| `test/app` | Tests focused on App configuration, commands and bindings |
| `test/core` | Tests focused on Core machine adaptation, compatibility and devices |
| `test/integration` | Cross-component product, boot, package and cross-process flows |
| `test/checks` | Product source-boundary and build-ownership checks |

Classify by inspected dependencies and assertions, not filenames alone. Keep
fixtures beside their owning tests. A genuinely shared product fixture has one
implementation at the narrowest appropriate product-test location; it must not
be moved into a reusable corpus merely because its name contains `common`.
Do not introduce a fixture framework, generic support object, or extra build
aggregation layer just to reorganize directories.

Preserve the useful machine/driver assembly performed by
`common_machine_test.c/h`. Replace legacy naming-only wrappers in `runtime.h`
and `input_queue.h` with direct calls to the existing contracts after auditing
every caller. Do not duplicate assembly or force tests through a different
production path to remove a header.

Inspect both manual diagnostics for actual consumers and unique coverage.
Delete obsolete, unreferenced programs only after recording that evidence;
retain useful probes with an explicit owner and documented invocation. Do not
silently discard unique tests or merely rename the miscellaneous directory.

Replace the stale `test/support/TESTS.md` inventory with a concise
`test/README.md`, linking the reusable packages and explaining product test
ownership, execution and desktop requirements. Update affected current design
documentation rather than creating a second layout authority.

## Proposed S tasks on admission

### S1: Inventory and ownership relocation

- Freeze the then-current file, target and CTest inventory, including helper
  callers, manual probes and cross-directory dependencies.
- Record each file's destination or justified retention/removal in a finite
  disposition ledger. Map every registered test to its existing assertions.
- Move App, Core and integration tests to their proper locations, preserving
  history and behavior; update root CMake and all live path references.
- Keep existing CTest names and operational labels unless an audited correction
  is explicitly recorded. Directory names alone must not change execution mode.
- Exit: relocated tests build and run with equivalent coverage; no stale live
  paths or duplicated implementations remain.

### S2: Fixtures, wrappers and checks

- Remove obsolete naming-only test adapters and update all callers to canonical
  interfaces while retaining one necessary product fixture assembly path.
- Place static gates in `test/checks` and dynamic snapshot orchestration with
  integration tests; resolve the two manual diagnostics using the S1 ledger.
- Remove emptied `test/unit` and `test/support` only when every entry has an
  evidenced disposition; do not replace them with another catch-all directory.
- Exit: no legacy forwarding aliases or dead helper registrations remain;
  focused tests and source/build boundary checks pass.

### S3: Documentation and complete acceptance

- Finish the test inventory and current layout documentation; verify CMake
  inputs, labels and assertions against the pre-change inventory.
- Verify the three shared source/test pairs remain untouched and independently
  usable; no product fixture dependency may leak into a reusable test package.
- Run applicable x86/x64 full tests and package acceptance, using the established
  background test configuration. Publish both EXEs for owner verification.
- Exit: complete per-file ledger, no lost coverage, all gates passing, and no
  unfinished cleanup hidden in TODO. Owner acceptance precedes T closure.

## Change estimate and reporting

Admission baseline: 7ebb435f. Read-only enumeration still finds 33 C files in
test/unit, 13 files in test/support (including two diagnostics), and two existing
integration C files. This 48-file product inventory is the frozen file universe;
S1 must add the exact registered-target/CTest and caller mapping before moves.
Disposition is relocation without body changes, justified retention for S2, or
evidenced removal in S2; no entry can disappear without its proof. S3 verifies
the complete before/after inventory, operational labels and six shared corpora.

Initial S1 estimate: production +0/-0; up to 33 test C files relocated, test
assertion changes +0/-0, build/include reference edits roughly +60--120/-60--120
(net near zero). Exact moved lines and destination counts follow caller audit,
before relocation. No throughput or runtime improvement is claimed.

Production source change budget: **+0/-0**. The initial relocation inventory is
33 product test C files plus the contents of `test/support`; the exact movement
count depends on ownership inspection at admission. Existing integration files
also participate in the ownership review. Root CMake and live documentation
references need updates. Reusable corpora are outside the change budget.

Movement should preserve test bodies. Removing aliases should reduce helper
code, but no numerical net reduction is promised before caller analysis.
Before each S task, report affected files/functions, estimated moved lines and
added/deleted lines separately. After each S task, report actual rename-aware
diffs, additions, deletions, net change, preserved test inventory, build/test
results and both executable links; commit and push the completed scoped work.

## Boundaries and stopping conditions

- No production API, runtime, guest behavior or preserved Core mirror changes.
- No changes to INI, disk images or snapshots; no test data written into assets.
- No modification of `src/lib`, `src/common`, `src/x86`, or their test corpora.
- Do not weaken assertions, drop tests, or change desktop isolation to make
  acceptance pass. Unexpected behavior changes require separate diagnosis.
- If cleanup requires production fixes or a reusable contract change, report
  the dependency for separate admission instead of expanding this task.

## Authority and references

- [Execution rules](../rules/EXECUTION.md)
- [Current source layout](../design/CODING.md)
- [Current architecture](../design/ARCHITECTURE.md)
- [Ordered candidate queue](../states/QUEUE.md)
