# M9 T72 S7: Unified Window Pixel Damage Acceptance

Owner: 测试通过，请将所有未提交内容一并提交收口本S任务。
S7 is closed; T72 remains open, Common S8 is not started.

P1 035d2845 unified text/graphics pixel comparison and cursor-region damage.
P2 c813f698 removed intermediate row scratch at owner's request; P3 6fa729a1
recorded actual-change review. Both decoders now directly use one inline
compare/write/damage helper. No public API, mailbox or mouse changes.

Finite ledger, similar-issue sweep, actual-change review and artifact hashes:
[S7 evidence](../etc/evidence/softpc/m9-t72-s7-window-pixel-damage.md).
Production +58/-62 (net -4), tests +97/-27 (net +70), relative to 2cf87250.
Both Release builds passed; background x64 105/105 (169.98s), x86 105/105
(160.76s). Five desktop tests per width were excluded from automation; owner
has now reported manual testing passed. No pending S7 implementation item.

Owner requested including the existing snapshot unchanged with all outstanding
content: 18395778 bytes, SHA256
F62B12D479471B6E6DCC2D85BE76879E95C321083552D5C35F3702852EE00393.
It is not a new test dependency, generated trace or claimed validation fixture.
Automatic safety review blocked exporting this guest-state file without explicit
destination-specific approval. It remains untracked and unchanged locally;
only closure documentation is committed pending that approval.
No source or EXE change at closure; prior build/test evidence remains applicable.
Documentation governance and diff checks are rerun for closure.
