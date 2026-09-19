# M9 T70 S7 stack-width repair

## Request and disposition

The owner requested repair of the Windows 95 Setup hardware-detection stall,
using the supplied early-detection checkpoint. The owner has now confirmed
that hardware detection passes and explicitly approved closing S7, committing
and pushing, while keeping T70 open for further instructions.

The executor delivery includes the previously uncommitted bounded S2--S6
repairs documented in [the investigation record](M9-T70-S6-investigation-record.md).
No claim is made that all those repairs caused the Setup improvement.

## Root and finite similar-issue sweep

The CALL gate read the correct 386 TSS ESP, loaded a 32-bit SS, then truncated
ESP using the gate's operand size. Subsequent pushes corrupted a real-mode
trampoline pointer and led to an invalid-opcode loop. Reuse the existing
SS-address-width helper after loading SS. No new helper, state or API is needed.

The covered class is privilege-changing stack-register installation in CCPU.
Search `load_SS_cache`, `set_current_SP`, `SET_SP(new_sp)` and
`SET_ESP(new_sp)` under `base/ccpu386`; review the surrounding call/return flow.
CALL, lower-privilege IRET and RETF now use the helper; the interrupt path
already did. Ordinary non-privilege-changing instructions are not rewritten.
The helper test proves preservation of high ESP bits for a 32-bit SS, not full
instruction-level coverage of all three paths. Only CALL has the supplied
fixture's causal trace and subsequent owner acceptance.

## Verification and limits

- Both x86 and x64 packages rebuilt with the final three stack-load changes.
- Focused checkpoint, snapshot-transaction, IRQ and BOP tests: 4/4 each width.
- Non-integration suites: x64 105/105, x86 105/105.
- Two package Window integration tests failed at their Window-observation
  stage after Machine started. Their cause has not been established; no
  all-tests-pass claim is made. Owner approved closure after this disclosure
  and successful hardware-detection testing.
- External fixture snapshot and disk hashes remained unchanged during agent
  runs. Owner-created later checkpoints are separate owner actions.
- Temporary diagnostic hooks and large owned traces were removed. Lib,
  Common, package INI and guest media have no changes in this delivery.
- Documentation gate and diff whitespace check are required before push.

## Changed-path accounting

Relative to `95c467a`, `git diff --numstat -- src test` counts seven production
paths +37/-34 (net +3) and four test paths +68/-0 (net +68).
Five production paths are in MVDM: +23/-24 (net -1). S7's three CPU paths
alone are +3/-12 (net -9); its helper regression is +13/-0.
Documentation and EXE artifacts are excluded. No file moves or new production
objects occurred. VM adds one one-shot restore rendezvous flag; S7 itself
adds no runtime state. Binary-size attribution to S7 alone was not measured.

Package SHA-256:

- x86: `ACF7CF8349087D09187786339E520E1171E3C6DAB0FE5AAA11CA034303F4EC8D`
- x64: `0437F8B23B0E88BF90ABE8D2B3D1FE49A751B82DE933872DB2D9578F4894478E`

T70 remains open. The later Setup memory warning has no diagnosis or repair
claim in S7 and no new S is admitted by this closure.
