# M9 T61 S8: unused support and local duplication

Original owner request: “请拆解一下S任务 并更新当前proposal记录 然后开始准入执行”。
The approved sequence is retained in the [proposal](M9-T61-S7-support-ownership-proposal.md).
S8 implementation P1 is 258e1f7, pushed to main.

Removed six uncalled helpers and their declarations. Kept the actual DIB bind,
palette-only dirty notification, original keyboard reset, stale 8042 clearing
and SAS bus access. Scancode dispatch uses the existing key entry; floppy
success commits its path once; HDD read/write share their unchanged range
validation locally. No new ownership layer or behavior branch.

Coordinator reviewed the actual six production-path diffs against da32558,
caller census and both archive symbol inventories. No mirror or shared corpus
changed. Production +20/-130, net -110; tests unchanged, no deleted assertions.
The finite candidate coverage and artifact hashes are in the
[evidence ledger](../etc/evidence/softpc/pristine-divergence-current.md).

Both builds passed. Full x64 98/98 (88.82s), x86 98/98 (74.47s).
Actual-commit dual-media/FDC/keycode/VGA/restart checks passed 5/5 per width
(4.22s / 4.80s). Documentation and diff checks passed. Fixed EXEs refreshed;
user INI/media untouched. No new temporary diagnostic directory remains.
Automated checks do not claim GUI/RDP manual acceptance.

S8 closes. S9 is next under the owner's sequential implementation admission;
T61 remains open. Existing TODO and three Queue candidates are unchanged.
