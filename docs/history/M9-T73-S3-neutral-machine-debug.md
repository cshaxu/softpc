# M9 T73 S3: Neutral Machine Debug Transport

Owner request: 批准照此实现S3.请你收口S2先，然后准入S3，执行完成后编译测试提交推送等我验证。

Owner subsequently reported 测试通过 and admitted the next migration.
S3 is closed; T73 remains open. Baseline 56261c63, implementation 006ecf32,
actual-change review dc06671d. The [proposal](M9-T73-shared-x86-dependency-audit-proposal.md)
retains the finite coverage ledger, failures, disposition and package hashes.

Machine retains its sole executor/lease/completion path and copies bounded opaque
128/1536-byte requests/responses. x86 protocol values belong to the x86 debugger;
VM validates operations. No second executor, registry, allocation or cancellation
path was added. CLI behavior and protected components remained unchanged.

Production C/H +327/-268 (net +59); test C/H +405/-222 (net +183);
boundary gates +2/-0. Both Release builds passed. Final background x64 105/105
(162.58s), x86 105/105 (125.26s). Five desktop tests per width were excluded;
owner manual acceptance now completes the exit criteria. An initial x86
allowlist failure was repaired explicitly and the full suite rerun.

Actual-change review checked original requirements, all transport endpoints,
error lengths/lease/cancellation/terminal handling, reverse mechanical comparison
of CLI/VM code, protected paths and shared manifests/DAG. No outstanding S3
implementation work remains. Independent neutral build qualification remains a
separate planned step, now S5 under the owner's revised plan.
