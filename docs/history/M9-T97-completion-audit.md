# T97 Completion Audit

## Owner decision and scope

Owner accepts the investigation and authorizes closure on 2026-10-18:
"好的 既然如此 批准收口本T". S1 and T97 close together.
The [retained proposal](M9-T97-win95-volume-and-restart-proposal.md) preserves
the original requests, nine-item evidence ledger and reproduction limits.
Frozen scope is both admitted Win95 symptoms, S1 findings, actual diff,
qualification, package status and outstanding work. No further S was admitted.

## Requirement reconciliation

| Obligation | Disposition and evidence |
| --- | --- |
| Startup PCM works but volume/tray controls are disabled | Reproduced. Installed Sndvol32 reports no active mixer. Actual MSSBLST driver binaries reject mixer enumeration and wave-volume operations below DSP 0300h; the existing model reports 0201h. |
| Determine a correct remedy | Existing CT1335 registers do not establish complete SB Pro compatibility. A compatible model/driver requires separate design; owner declines further capability work. No version spoof or forced controls added. |
| Installation Yes reaches safe-shutdown screen | Same installed driver's hardware-change flow explicitly asks to shut down and reaches that screen. This explains the reproduced case; first-time installation and the owner's original click were not recorded. |
| Distinguish shutdown from broken reset | Explicit normal Restart succeeds twice through BIOS/audio reinitialization; ordinary Shutdown reaches safe-off without BIOS reset. No demonstrated restart failure was repaired or concealed. |
| Check audio after reset | Second restart records 139373 accepted PCM frames after BIOS reinitialization. Submission is proved; subjective audibility and complete native playback are not claimed. |
| S1 evidence and independent review | Executor delivery 2ea2441f; coordinator actual-commit review 9b2a524d. Proposal reconciles all nine investigation items with proof or explicit limits. |
| Runtime, assets and debt | Production/tests/mirror +0/-0, net 0; no ABI or package change. Owner configuration, media and snapshots preserved. Existing TODO entries remain; Queue is empty. |

## Verification and change review

Read-only source/binary inspection and task-owned x64 snapshot replay used
guest input probes and direct frame capture, without Computer Use. Bounded
BIOS/controller/PCM observations and captured dialog text distinguish the
actions. Diagnostic runs ended with successful cleanup and exit code zero.
Ignored evidence remains under build/t97/s1 as indexed by the retained proposal.

S1 delivery changed documentation only: P1 +136/-3, P2 +12/-0.
This closure archives the proposal, updates Current/Queue and records this
separate T-level audit. Documentation governance and whitespace checks are the
applicable closure checks. No new full-suite or dual-build result is claimed.
Accepted T96 qualification remains 191/191 tests per architecture; existing
compiler/Ninja warnings were disclosed there and are not repaired by T97.

Both EXEs remain unchanged, with accepted SHA-256 values:

- softpc32: 6A65D5127C6EBF567DCB7FD32F506541BF028FD42B69F9113CB09B363DF52C9A
- softpc64: E1B95ED71ACB371098C854FEF6FD4E3573B71BE98169243958F13DD2900A53C1

Closure does not claim every installer requests shutdown or that all possible
restart failures are excluded. The exact original dialog remains unverified.
Future mixer support needs its own admission; no new candidate is created
against the owner's decision. User-owned untracked snapshot.debug stays
untouched and excluded from commits. No implementation subtask remains active.
