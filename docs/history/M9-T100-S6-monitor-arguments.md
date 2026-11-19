# T100 S6 Monitor argument ownership closure

Owner acceptance: 验收通过，收口S6，保持T100开放，确保工作区干净。
S6 is closed; T100 remains open. No next S is admitted.

## Completion audit

Delivery 4cd56d018abd5328f0578dbfe0b7524f4bcfcce4 implements the final
owner-approved boundary: Emulator Monitor alone lexes complete commands and
passes counted, borrowed arguments to fixed and extension consumers. X86 IBMPC
routes without reparsing; App checks counts and business meanings. DEBUG retains
its independent grammar and quoted filename fix. INI is unchanged.

The entry ledger covers Monitor, X86 IBMPC forwarding, App floppy/save/load,
DEBUG N/L/W, INI file settings and startup. All rows have implementation or
unchanged dispositions and focused proof. Removed App split/unquote paths and
the public raw-tail parser; no compatibility entry, per-argument allocation,
new queue or lifecycle state. Consumers must not retain borrowed pointers.

Against e3e75612, tracked C/H production is +110/-119, net -9; tests +153/-75,
net +78; root CMake +2/-2. Mirrors, Lib, user INI, media and snapshot formats
are unchanged. Shared changes affect Emulator, X86 IBMPC and DEBUG source/tests
and manifests. Downstream callbacks require migration; NXVM was not synchronized.

Both cached architectures built/published and passed 188 background plus eight
native tests each, 392/392, with no final build warnings/errors. The final
normal-eject test-only addition passed on both architectures. Accepted EXEs
remain P1 publications; closure performs no rebuild or new guest validation.
[Implementation, hashes and migration evidence](../proposals/m9-xp-mirror-original-first-audit.md#s6-p1-delivery-evidence).

Coordinator review checked the actual committed change set against the amended
request, callback lifetime, pre-dispatch syntax rejection, preservation of
resource cleanup, tests and manifest boundaries. No S6 requirement remains open.
Existing package Console TODO and runtime-performance Queue candidate are
separate and unchanged; this closure does not close T100 or that debt.

Documentation governance, whitespace and build-residue checks pass. Only
closure documentation changes; the working tree was clean before closure.

## Retained admission packet

## M9 T100 S6 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Continuation |
| Admission And Approval | Owner explicitly admits quoted-path repair and full delivery. |
| Objective | Monitor alone lexes command and all arguments; fixed and extension callbacks consume borrowed argument lists. DEBUG retains its own grammar. |
| Non-goals | No mirror, Lib, guest, snapshot schema, INI, threading or queue changes. |
| Reference Baseline | e3e75612; clean main worktree. |
| Candidate Proposal | [T100](../proposals/m9-xp-mirror-original-first-audit.md). |
| Files And ABI Surface | Emulator Monitor and X86 IBMPC callback ABI; App extensions, DEBUG N, corresponding tests/CMake/docs/manifests. INI remains unchanged. |
| Applicable Rules | Execution, Coding, Architecture, Documentation; source policy and UI. |
| Verification | Cached x86/x64 builds, dual publication, focused and full background/native regression; actual-diff self-review. |
| Expected Markers | All tests pass; one Monitor lexer; no raw-tail callback; malformed syntax has no effects; quoted UTF-8 preserved; unquoted spaces delimit arguments. |
| Asset Needs | Test-generated disposable files only; preserve original INI/media/snapshots. |
| Reporting Requirements | Actual production/test diff, entry audit, counts, artifact hashes, pushed commit and limitations. |
| Stop Conditions | Protect unrelated changes. Executor owns build/t100-s6 scratch/cleanup; two-hour job and fifteen-minute no-progress budgets. |
| Exit Criteria | Verified, published, reviewed, committed, pushed, clean worktree; await owner acceptance. |
| Original Owner Request | Admit S6 to fix quoted paths including save/load, allow spaces within quotes, minimal unified repair and necessary tests; dual build, publish, full tests, commit/push, self-review and await acceptance. |
| Similar-Issue Sweep | Freeze monitor floppy/save/load, DEBUG N/L/W, INI floppy/hard_disk/serial_output/printer_output and startup arguments; record dispositions. |
