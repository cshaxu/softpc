# M9 T89 S5: Desktop qualification and final review

## Request and scope

Owner: "验收通过 收口 准入next s task"; subsequently "批准" for the
reserved desktop test period. S4 closure/S5 admission is cd85c0c3 over accepted
implementation d1583eda. This S completes the outstanding qualification; it
does not silently close T89 or admit the queued XP baseline upgrade.

Plan and actual implementation changes: production +0/-0, test/build +0/-0.
Reuse accepted dual EXEs and intermediates; no redundant build, Computer Use,
source change, INI change or guest-media mutation. Only task records change.

## Verification

CTest inventory confirms exactly five desktop cases on each architecture.
The owner-approved reserved period uses one hidden owned supervisor, x64
then x86, serial CTest -L desktop --no-tests=error --output-on-failure -j 1.
Budget: 20 minutes per architecture, 20 MiB captured output. Both child exit
codes and the supervisor exit code are zero; no retry or assertion waiver.

| Existing test | x64 | x86 |
| --- | --- | --- |
| app-softpc/integration/package_smoke | Pass | Pass |
| app-softpc/integration/package_smoke/compact_console | Pass | Pass |
| lib.kvm_window_retirement | Pass | Pass |
| lib.kvm_window_modal | Pass | Pass |
| lib.console_broker_display | Pass | Pass |

x64: 5/5, 12.89 seconds. x86: 5/5, 13.12 seconds. The reserved desktop
period ended after both runs. These are actual native test executions, not
test discovery or owner manual testing counted as automated evidence.

Together with [S4](M9-T89-S4-retained-port-audit.md)'s 131 background passes
per width on these same accepted artifacts, qualification covers all 136
registered tests per width. This combines two runs; it is not a claim that
S5 reran the background suite. No Linux runtime qualification is claimed.

Both published artifacts retain their accepted SHA256:

- softpc32.exe: 50900AC57EBA81A9B00558FDF424F73D047965A74E59864D697A890DFD67F3DC.
- softpc64.exe: A12EED74F50388CB4F15018529DAAE7688DFE67E878FA80ADD91E3D7D5780DF3.

## Whole-task review and disposition

Rechecked the original owner scope, S1 finite ledger, S2/S3 accepted repairs,
S4 actual-change audit, current numstat and S5 desktop inventory. Every one
of the 36 original directives has a disposition: seven removed and 29
retained/converted with an explicit owner and receiver. No hidden hardware
rewrite remains required under the admitted narrowed design.

Whole-T production remains +48/-78, net -30; build/tests/checks +88/-7,
net +81. Combined code +136/-85, net +51. Original-mirror distance in the
frozen fourteen-file universe falls from 584 to 526 added-plus-removed lines,
a reduction of 58. Shared six-component source/test roots are unchanged.
S4 retains the full per-file accounting; S5 does not create a second ledger.

Existing RDP escape and historical null-BOP TODOs remain separately deferred;
no new defect or qualification exclusion was found. The XP upgrade remains
unadmitted. Documentation governance and whitespace checks must pass before
delivery. Review lesson: distinguish accepted background evidence from the
remaining desktop qualification, and reuse identical artifacts rather than
rebuilding solely to update governance.

S5 is delivered for owner acceptance. T89 has no remaining planned
implementation or test work, but remains open pending the owner's closure
decision. No automatic next task is admitted.
