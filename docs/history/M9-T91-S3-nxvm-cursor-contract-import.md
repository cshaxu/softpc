# M9 T91 S3: Cursor-contract import delivery

Status: owner-accepted and closed on 2026-10-13. T91 stays open.
Owner: "批准开始导入，完成后编译、部署、测试、提交、推送后等我验收。"
Baseline: `2ea997fe`; executor P1: `e96b2983`, pushed to main.
Upstream: `97d3cf9482913f2a0e0872158149ff646a563f3c`.
[Audit](M9-T91-S2-nxvm-cursor-contract-audit.md) and
[retained import ledger](M9-T91-nxvm-shared-glyph-import-proposal.md).

## Actual-change review

After the executor push the same session switched to coordinator. The committed
path/diff inventory, original approval, artifact hashes and actual test logs
were checked. All 246 shared paths are byte-identical to the clean pinned NXVM
tree. Only the 18 audited shared paths changed, with no additions/deletions.
No public package was forked, and no external build/runtime dependency exists.

SoftPC has exactly three local one-line adaptations: driver visibility becomes
HOST/DISABLED, runtime cursor fixture selects HOST, and the real DOS runtime
fixture asserts HOST rather than a generic nonzero visibility. The existing
Compat cursor query and constant source phase remain unchanged; thus Window
continues host blinking rather than accidentally selecting a permanently bright
SOURCE cursor. Mirror, Compat, snapshots, user INI and media have no diff.

The field-reference sweep found only distinct native Compat visibility and
original controller visibility calls after adaptation; those are intentionally
retained. No second clock, renderer, buffer or lifecycle path was introduced.
The imported tests cover the alternate SOURCE contract without requiring SoftPC
to implement hardware cursor phase. Existing Console limitations are explicit.

## Qualification

Both existing Ninja Release graphs rebuilt affected targets and refreshed the
two package EXEs. Shared background suites ran before other background suites.

| Width | Shared background | Other background | Desktop recheck | Total |
| --- | ---: | ---: | ---: | ---: |
| x64 | 84/84 | 48/48 | 5/5 | 137/137 |
| x86 | 84/84 | 48/48 | 5/5 | 137/137 |

This includes all shared suites, App unit and integration tests and repository
checks. There are no waivers. Initial x64 desktop package cases failed with
extra characters not sent by the fixture; all three Lib desktop cases passed.
After another owner-confirmed input-idle reservation, both complete desktop
suites passed without code changes. Initial logs remain separate. This is
consistent with input contamination, but the exact external source is unknown.
The original failure is not erased or described as a first-attempt pass.
No Computer Use was used. Governance and whitespace gates also pass.

## Accounting and artifacts

Reproduce code counts with `git diff --numstat 2ea997fe e96b2983 -- '*.c' '*.h'`:

- Shared production: +45/-12, net +33; App production: +1/-1, net zero.
- Shared tests: +79/-19, net +60; App tests: +2/-2, net zero.
- Total production: +46/-13, net +33; total tests: +81/-21, net +60.
- Shared files including documentation/manifests: +145/-47, net +98.
- Original mirror: zero changed files/lines.

Published SHA256:

- softpc32.exe: `CF504B2E72233445E9197AF66C28854DA70DAA7340314E580AEA21838A3E8310`
- softpc64.exe: `FF3B770B4656D83152A3146242868BF1652375D4753DEBD74637AA487FC5D8B8`

EXE size increases are 574 bytes (x86) and 573 bytes (x64). Shared text schemas
grow eight bytes; the outer Window frame size remains unchanged. Coordinator
accepts the technical delivery; owner acceptance and S/T closure remain pending.

## Owner acceptance and S3 closure

On 2026-10-13 the owner confirmed: "验收通过，批准收口S3，保持T91开放。"
S3 is closed against the P1 implementation `e96b2983` and P2 coordinator review
`74b45795`. All admitted import, adaptation, build, deployment and test outcomes
are verified above, with no waived tests or outstanding S3 work. The initial
desktop failures and unchanged-code successful recheck remain part of the record.

T91 stays open; no next S is admitted. This closure changes only governance
records, verified by the documentation and whitespace gates. Production code,
shared manifests, both published EXEs, user configuration and media are unchanged;
the qualified builds and runtime tests are not rerun for this documentation-only
closure.
