# M9 T85 S8 delivery handoff

Original request: 批准新的s任务进行以上各项优化，一个解决common，一个解决x86，一个处理ibmpc！

P1 9fed40d1 and actual-change review P2 3537e87a are pushed. Runtime is one
Common C path +2/-2, net zero; two existing tests add 27 lines. Invalidation
uses existing valid markers; complete-frame/generation/locking/first-frame
contracts remain. Dual Release and focused Machine 2/2 per width pass;
final serial background x64/x86 pass 443/443 each, desktop excluded.
The proposal retains the initial x86 shutdown timeout and diagnostic/retry
evidence; TODO owns the unproven native timing cause. It is not an S8 repair.
No Lib/x86/IBM PC/App implementation changes or new runtime state are included.

On 2026-10-05 the owner requests: 请你继续执行，完成S8以后继续走S9。每一个S呢你都要编译测试提交推送，不需要等我验收，我只在S9完成后来验收。

S8 delivery is complete and this handoff permits S9 progression. It does not
invent owner manual acceptance or close T85. S9 will wait for testing after
its independently verified and pushed delivery. S10 is not activated.
Original owner INI/snapshot edits remain untouched under the prior exclusion.
