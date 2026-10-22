# M9 T98 S14 Closure

On 2026-10-22 the owner confirms RDP reconnection and authorizes successful
self-verification followed by S closure. The executor completes qualification,
commits and pushes 0d9f1803. The coordinator then reviews that actual commit
against the approved request, source ledger, ownership rules and test evidence.
No additional in-scope blocker is found. S14 closes; T98 remains open and no
next S is admitted. This does not claim another owner-run manual test.
The [retained proposal](M9-T98-S14-kvm-lifetime-failure-proposal.md) owns the
source ledger, implementation reasoning and earlier qualification failures.

## Result and review

The confirmed creation-rollback defect could return NULL/error while leaving
an inaccessible live worker using caller-owned context. Both KVM leaves now
either finish safe cleanup or terminate the process without application cleanup
callbacks when worker exit cannot be established. Ordinary allocation, startup
and I/O failures remain ordinary failures when safely bounded.

Normal destroy joins before disposal and consumes the object on every normal
return, including post-join cleanup errors. The four Emulator UI call sites
clear consumed pointers before handling status. Independent native cleanup
continues after errors, clears successful handles and never deletes a bitmap
still selected into a DC whose deletion failed. Failed terminal wake makes
one immediate exit observation: an already-exited worker is safe to clean up;
otherwise the path cannot return. This is not polling or a retry policy.

The coordinator reviews actual production/test changes, public contracts,
manifests, documentation and artifacts. Existing four-slot ownership, dynamic
allocation, Linux unsupported leaves, Base task and Console broker contracts
remain unchanged. No new state machine, thread, application callback, recovery
manager, second render path or test-only production API is introduced. Types
adds only one-to-one CRT/SDK declarations. Whole-package gates supplement this
changed-path review; they are not proof that every unchanged line was reaudited.

## Verification and limits

- Cached x64/x86 builds and both published EXEs are complete. Changed public
  code has no compiler warnings; original mirror-header warnings are retained.
- Each architecture: 184/184 background, 5/5 desktop, 3/3 native audio.
  Final XML names equal all 192 registered tests exactly, with no duplicate,
  failed, error or skipped cases. The complete final native rerun passes without
  isolated supplements, after RDP reconnection.
- Final test-only held-payload assertions have additional focused 4/4 and
  full unit/gate 177/177 passes per width, the latter while RDP was disconnected.
- Fatal cases use isolated children with exact exit code and branch output,
  rejecting ordinary return/crash and application atexit cleanup. They cover
  startup rollback and destroy, for failed stop and failed join on both leaves.
- Native retirement keeps all 24 scenarios and synchronized worker holding
  beyond five seconds. Dynamic frame concurrency holds old text and graphics
  while a producer changes type/size; release hooks reject premature freeing,
  at most four payloads coexist, and final allocations/releases balance.
- Earlier disconnected audio/package failures and Active compact package
  stage 1/error 6 are disclosed in the proposal. Later passes do not establish
  their cause or repair; the existing package TODO remains open.

Callers still serialize destruction, stop external publishing before disposal,
and must not synchronously destroy from their worker/callback. Post-join failed
OS resource deletion may leak that resource while reporting error; no residual
object manager is added. Base event destruction remains void. Injected native
failures prove handling, not actual host corruption. Downstream repositories
were not modified or claimed synchronized.

## Accounting and delivery

Against audit P1 06a86ad7, `git diff --numstat` over C/H paths gives production
+64/-66 (net -2), tests +225/-49 (net +176). Test CMake/controller adds +26/-2
(net +24). Manifests, documentation and rebuilt binaries are separate. Test
growth is the missing fatal/cleanup/concurrent-lifetime proof, not a production
recovery mechanism; necessary coverage was not removed to force negative size.

Downstream import baseline: **0d9f1803ee04bd89bd6150306912a3e8674d0a83**.
Changed public trees are src/lib, test/lib, src/emulator, test/emulator.
The X86 pairs and root test infrastructure are unchanged. Importers must adopt
the documented consumed-on-normal-return KVM destroy contract.

Published SHA-256:

- softpc32: A583D165349382242CAB44F132DA98573CE1A431249D785436437304AFC5F891
- softpc64: BECB19DBD2A64C72727BB7E9F142E77C2B57832C23176C30EDF187795B9F48B1

User INI, media and pre-existing untracked snapshot.debug are preserved.
All owned jobs exited; disposable task scratch was removed after evidence
promotion. Persistent Ninja/ccache trees remain. Residue, documentation and
diff checks pass. The only untracked residue is the preserved user snapshot,
not an uncommitted implementation file.
