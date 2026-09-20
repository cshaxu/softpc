# M9 T72 S8: Common Cleanup Acceptance

Owner reported "测试通过，本T任务还有哪些", then approved "行，收口T72".
S8 is accepted and closed. Delivery 95850780 and actual-change review ae1dc6e0
were pushed before acceptance. [Evidence](../etc/evidence/softpc/m9-t72-s8-common-cleanup.md)
contains the finite A/B/C/D/F ledger, similar-issue sweep and package hashes.

- A: broker stop/unbind precedes KVM destruction; failure preserves live
  UI/Session dependencies and App uses its existing terminal failure policy.
- B: status text has an explicit readable foreground palette entry.
- C: TEXT uses the existing running-only sink, not a held-key ledger.
- D: UI/Session/Machine clear valid outputs before other argument validation.
- F: Debug embeds its fixed pointer table; original CLI semantics remain.

Production seven C/H +33/-36 (net -3); six C tests +149/-5 (net +144);
test build/gate +2/-2 (net0), measured from c0355b2f to 95850780.
Both Release builds passed; final background x64 105/105 (134.41s),
x86 105/105 (135.13s). Initial static-gate failures and corrections remain
disclosed. Five desktop automation tests per width were excluded, not claimed
executed by subsequent owner acceptance. No native Linux runtime proof claimed.

All five ledger items have production proof and tests; no remaining S8 item or
new debt transfer. Lib/VM/Compat/MVDM, INI/media/snapshot were unchanged by S8.
Closure changes documentation only (+0/-0 production/test), retains the accepted
EXEs and records acceptance without rerunning the machine. Documentation
governance, reference and whitespace checks precede push.
