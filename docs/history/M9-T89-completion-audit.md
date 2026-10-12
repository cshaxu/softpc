# M9 T89: Completion audit

Owner: "批准收口 然后队列任务列一下". T89 and S5 are accepted and closed.
This governance delivery changes no implementation, tests, binaries, INI or
guest media. The accepted runtime baseline remains d1583eda; S5 qualification
delivery is f8fe518f. No next task is admitted.

## Requirements and finite coverage

The admitted scope is the narrowed S1 design in the
[archived proposal](M9-T89-mvdm-compat-boundary-convergence-proposal.md), not
unconditional elimination of every standalone branch.

| Admitted work | Result and evidence |
| --- | --- |
| S1 audit/research/design | [Frozen ledger](M9-T89-S1-mirror-compat-convergence.md): 36 directives, 14 mirror files, explicit owner and receiver for each. |
| S2 existing endpoints | [Accepted repair](M9-T89-S2-existing-endpoint-cleanup.md): original includes and key lookup reused; target owns profile definition. |
| S3 host-header policy | [Accepted repair](M9-T89-S3-host-header-policy.md): bounded Compat FENV/lock overrides preserve original declarations and layouts. |
| S4 retained-port review | [Final ledger](M9-T89-S4-retained-port-audit.md): seven directives removed, 29 retained/converted with reasons; reset comment corrected; full background qualification. |
| S5 remaining qualification | [Actual desktop results](M9-T89-S5-desktop-qualification.md): five cases per width pass without retry, completing registered test coverage. |

Coordinator reviewed actual Git accounting from 3c2e6e69, the S4 per-file
original-source comparison and S5 documentation-only delivery. All admitted
requirements have receivers and evidence; there is no remaining T89 repair.
Keeping necessary hardware/NT endpoint boundaries is the approved result,
not an undeclared deferral or duplicate implementation.

## Accounting and acceptance

Production +48/-78, net -30; build/tests/checks +88/-7, net +81.
Combined code +136/-85, net +51. Within the frozen mirror universe,
OpenNT added-plus-removed distance falls 584 to 526, a reduction of 58.
The six shared components are unchanged. S4 owns the detailed file ledger.

Both Release EXEs were built and published in S4. On these identical
artifacts, each width passes 84 shared background +47 other background +5
native desktop tests: 136/136. This combines S4/S5 runs, not a fresh build or
full-suite rerun during closure. Package integration includes both native
Console variants. No Linux execution claim is made. Hashes remain those
recorded in S5. Documentation governance and diff whitespace checks pass.

## Remaining work and review

The XP SP1 mirror baseline upgrade remains the sole queued candidate,
unnumbered and unadmitted. Existing RDP capture and null-BOP TODOs are outside
this task and retain their original admission conditions. No new debt was
created. The proposal is archived and its direct links repaired.

The key review outcome is to minimize original-mirror distance without
duplicating hardware state in Compat. Qualification distinguishes background
from native desktop coverage and reuses accepted artifacts instead of
rebuilding for document-only closure.
