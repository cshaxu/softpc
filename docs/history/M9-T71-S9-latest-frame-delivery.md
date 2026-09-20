# M9 T71 S9 Complete-Frame Delivery Closure

## Requests And Disposition

Owner implementation request: "我觉得可以用这个方案重构，准入修复S9，现在开始。"
Follow-up: "不是，我要你现在就配置清楚，不是等到以后" (background tests).
Closure: "收口当前S9". S9 is owner-accepted and closed; T71 remains open.
No S10 implementation is admitted by this closure.

Baseline: 65ad144a. P1 838b076c delivers frame changes; P2 949b71f1 reviews
them. P3 b5cc140a delivers test selection; P4 61fc333a reviews it. All were
pushed before this closure review. The original scope, finite coverage ledger,
performance measurements and failure disclosures remain in the
[regression brief](../proposals/m9-kvm-mode-transition-regression.md).

| Request | Retained implementation and evidence |
| --- | --- |
| Independent complete-frame delivery | VM copies a complete snapshot on completed producer dirty; Common retains latest publication and no-frame readiness. Upstream A/B tests retain both changes. |
| Opaque Base latest-wins | Deleted frame-update callback; copied bytes, independent locks, STOP and generation-bound acknowledgements retained. Mailbox tests cover overwrite and rejection. |
| Window-local damage | Resolve indexed colours and compare existing RGB surface; redraw changed bounds, full on first/recreated/text-to-graphics. No new cache/thread/queue. Pixel/palette/stride and native invalidation tests cover skipped frames and accumulated updates. |
| Immediate background configuration | Default presets exclude five desktop tests; explicit desktop presets run serially. Dual-width dry-run sets are disjoint and complete (105 + 5). No test deleted or product rendering hidden. |
| Build/test/delivery | Both package EXEs rebuilt for P1; full x64 110/110 (173.66s), x86 110/110 (163.13s). Configuration follow-up background x64 105/105 (124.87s), x86 105/105 (121.36s); desktop group not rerun during that follow-up. |

## Accounting And Boundaries

Rechecked with git diff --numstat 65ad144a..61fc333a, grouping tracked C/H by
src and test: production 12 files +41/-89, net -48; tests 8 files +174/-44,
net +130. Test configuration +8/-3, net +5. Documentation, manifests and binary
replacements are separate. Graphics ABI loses producer dirty fields; all
callers migrate together. Common production changes only its frame contract
comment. No Compat/MVDM, input ABI, snapshot format, INI or guest-media changes.

Each graphics frame loses 16 bytes; Window loses a 1024-byte palette cache.
Packages: x86 3655249 bytes (-1085), x64 3058791 (-572). Closure hashes still match:

- x86: F7A7508332A992E3697A88CB71F8672B7D47381F4831F94A9056AE53FA0D4C78
- x64: 3665A3D3FBE516A900AAD7A11DC62045AD286F6DA758B0FE2C6DC32C32433CD1

## Closure Review

Coordinator rechecked delivered path accounting, original request, package
hashes and prior actual-change reviews against the finite ledger. Owner approval
completes the remaining acceptance gate. Temporary probes were already removed;
the worktree was clean on entry. This closure changes documentation only and
runs governance/whitespace checks, not another interactive test or rebuild.
No exhaustive pixel-level or native Linux verification is claimed. Nonstandard
text-stride proof and earlier intermittent modal-test investigation remain
assigned to S10; neither is declared fixed by S9. Queue/TODO priorities are
unchanged. No active implementation subtask remains pending owner direction.
