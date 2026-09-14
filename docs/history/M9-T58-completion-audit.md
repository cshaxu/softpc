# M9 T58: Whole-Task Closure

Owner closure request: “队列首位任务好像已经完成了是吗，你帮我收口这个t任务吧。”
Reviewed baseline a1c24ce through final delivery 433f57e. The original request,
approved scope extensions and finite per-S ledgers are retained in the
[archived proposal](M9-T58-common-test-sync-proposal.md).

## Completion ledger

| Scope | Disposition and evidence |
| --- | --- |
| S1: import NXVM Common test assertions without src changes | Complete; owner approved correcting the stale upstream manifest and making SoftPC canonical. [S1](M9-T58-S1-common-test-sync.md). |
| S2: separate command, keyboard and composition | Complete; old command_binding C/H removed, one provider wiring path. [S2](M9-T58-S2-app-composition.md). |
| S3: move entity assembly from main | Complete; only composition includes VM; main retains config/entry. [S3](M9-T58-S3-application-assembly.md). |
| S4: safe ordered disposal | Complete; existing stop/join extracted as shutdown, callbacks quiesce before UI/session/command/machine/VM disposal. [S4](M9-T58-S4-machine-shutdown.md). |
| S5: x86 Release cache repair | Complete; both GNU presets pin optimization, tests retain assertions. [S5](M9-T58-S5-release-flags.md). |
| Queue and proposal | Completed candidate removed; original request/evidence archived with repaired links. XP candidate remains unadmitted. |
| Remaining debt | All nine TODO entries retained. T58 startup/viewport and tool-exit observations have named owners and reentry conditions; none is claimed repaired by this closure. Older display, x87 and storage debt remains outside scope. |

Every admitted S has a pushed executor and actual-diff acceptance record.
Later source work was separately owner-approved, not retroactively included
under S1's test-only permission. No requirement for ongoing equality to a
moving NXVM checkout remains: the owner selected SoftPC as the source for its
subsequent adoption. No external checkout was modified.

Actual changed-path review confirms one command provider, one App assembly
owner and one machine shutdown path. Lib, VM, Compat, MVDM and test/lib have
zero task diff. Common changes are confined to approved shutdown and its
documentation/manifest; test/common retains the imported assertion change and
adds shutdown proof. No new runtime thread, queue or state machine.
Net production C/H from git diff a1c24ce..433f57e is +53 lines (+469/-416);
per-S test/build/tool accounting remains in the archived ledger.

Final S5 builds and serial full suites passed x64 85/85 (54.73 s), x86 85/85
(68.97 s). No executable or source changed since then; closure verifies the
same package hashes and reruns documentation gates rather than inventing a
new runtime build. x86 SHA256 is
57B7F7069D1FAF12FAB3D56A08BDE65268284620A300AEF895C63C1E38CE489A;
x64 is 2FC00B4E09124D21EAC5FC30B4099F641B8807B8A4D698AED736374D85F4A64B.
No INI/media changes. Owner approval ends the T-open wait; it is not a claim
that every deferred interactive issue or downstream adoption is complete.

T58 is closed. No next T/S is admitted.
