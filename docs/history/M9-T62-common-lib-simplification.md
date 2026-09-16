# M9 T62: bounded Common / Lib simplification delivery

The [retained proposal](M9-T62-common-lib-simplification-proposal.md) preserves
the original owner request and pre-implementation estimates. The finite scope
is its four rows, not a claim that every possible simplification is exhausted.
T61 was explicitly closed in 6741721 before S3; the premature S1/S2 admission
and reverted governance relaxation are recorded in that proposal.

## Request-to-evidence ledger

| S | Delivered ownership change | Production C/H | Evidence |
| --- | --- | --- | --- |
| S1 | One copied-event fault latch/take path; two first-wins classes preserved | +35/-69, net -34 | [S1](M9-T62-S1-session-fault-slots.md), dce462e |
| S2 | Machine embeds its queue; one mutex initialize/dispose owner | +24/-37, net -13 | [S2](M9-T62-S2-machine-input-ownership.md), db3b6d8 |
| S3 | One file-length algorithm over selected seek/tell primitives | +26/-37, net -11 | [S3](M9-T62-S3-storage-position.md), 0811c31 |
| S4 | Neutral Console event state in root; native worker/button decoding in platform | +85/-71, net +14 | [S4](M9-T62-S4-console-event-ownership.md), aa1eabd |

Counts use git diff --numstat from 58d3d75 through aa1eabd, selecting source
C/H only: 12 production paths +170/-214 = -44. Eleven test C/H paths
+167/-14 = +153; two test CMake files +3/-0. Manifests, README/governance and
binary artifacts are separately excluded. Relocation is not deletion.
No public interface, App, VM, Compat or MVDM source changes; no final tools
change. Queue and original keyboard-boundary TODO remain separate and unchanged.

## Verification and artifacts

Every S rebuilt both fixed EXEs and passed its full suites, followed by
actual-commit focused review before S closure. Final suites: x64 101/101
(61.73s), x86 101/101 (63.32s); final post-commit 14/14 each. Strict standalone
Lib build passed. Linux platform fakes/placeholder compilation are bounded
proof, not Linux product-runtime acceptance. No human interaction test is claimed.

Final fixed EXEs from aa1eabd:

- softpc32.exe SHA256: 443ECCD4537D54A9F461624F646E0F94BCBB7901CD5E38103CB33DC4B0D5B571
- softpc64.exe SHA256: 9FC8837224959AE9DBBD6678C203D730AD747BF10929AA7DAB1B042A129908CA

User INI/media preserved. Owned temporary T62 build logs and placeholder object
are disposable after their evidence is recorded; existing build trees remain.

## Acceptance state

S1-S4 are closed and the owner reports testing passed. T62 remains open.
The owner admitted four follow-up simplifications as S5-S8, in sequence;
after their delivery, wait for new manual acceptance before T closure.
