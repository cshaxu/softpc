# M9 T72 S8: Common Quality Cleanup

## Request And Scope

Owner split Lib/Common cleanup, postponed Common to S8, then admitted S8 after
the approved snapshot push. Baseline c0355b2f; admission documents were the only
initial worktree changes. This is audit A/B/C/D/F, not another whole-tree rewrite.
E was completed in S6. Lib, VM, Compat, MVDM, INI, media and snapshot are unchanged.

## Finite Ledger And Actual Implementation

| Item | Retained owner and proof |
| --- | --- |
| A: teardown | UI stops broker reader/output through its existing destroy barrier, then joins Window/Console and releases monitor/UI. Failure returns immediately; only successful child disposal clears its pointer. Composition mock injects each of three failure points with raw or cooked current: six cases prove no UI release and retained callback delivery, and track no duplicate successful child destruction. App fixture executes actual composition with exit intercepted, proves Session still receives events on UI failure and no Session destruction follows. |
| B: status colours | Common's existing status builder sets palette entry7 to C0C0C0, background0 stays black. Existing composition test checks final RGB as well as every cell, unchanged text and sequence suppression. No guest palette or PC mapping introduced into Lib. |
| C: TEXT | Session forwards TEXT at the same running-only sink as MOUSE, without a held-key entry. Physical-key fixture proves scalar/source preservation, paused/stopped suppression, sink rejection and empty ledger. Existing KEY/retirement tests unchanged. Product VM decides character support. |
| D: create outputs | UI, Session and Machine check output address, clear it, then validate remaining inputs. Composition/session-frame/machine-wait fixtures test null out, null options/driver and invalid structures with dirty output. Existing successful create paths remain exercised. |
| F: argument table | The same Debug object embeds 256 pointers. Independent allocation/free/NULL guards removed; parser, commands and output unchanged. debug-output proves one object allocation across open/close/reopen, continuation cancellation on reopen, cleared table and maximum255-character parse including command-prefix split/sentinel; original debug-linear/common-machine command and continuation coverage retained. |

Final destruction previously attempted raw-to-monitor replacement even when
monitor was already current, then disposed everything despite error. A does not
add a current-console flag or ignore an error: final destruction needs no new
reader, so broker's existing stop/unbind path is used directly. During that
barrier producers and callback targets still live. After failure the product
terminates rather than inventing a recovery loop. Fixture cleanup clears its
scripted failure only to release test objects; it is not a supported retry policy.

App's terminal receiver must participate: retaining UI while freeing Session
would still invalidate the event context. It now follows the existing machine
join failure exit convention. The product shutdown fixture includes actual
composition.c to intercept exit and observe Session lifetime; root CMake removes
only its duplicate test compilation entry. Product target membership is unchanged.

## Similar-Issue Sweep

- `rg` over Common constructors: four public creators. Three corrected; Debug
  already clears output before allocation. Xasm has copied output operations,
  not an owned create/destroy lifecycle. Leaf creation delegates Lib contracts.
- UI destroy/action/create cleanup and all UI-destroy callers inspected. Action
  destruction already returns on failure before nulling; creation failure has
  no surviving KVM producer. Final App caller corrected; Session disposal has
  no asynchronous worker. Machine shutdown already precedes callback teardown.
- Input dispatch/queue/retirement paths inspected: TEXT retains copied value
  and existing queue acceptance semantics, no coalescing or key ledger entry.
  Raw Console mouse suppression while Window exists remains intentionally separate.
- Every `arguments` reference in command.c inspected: table always belongs to
  the one Debug object; closed-state checks use machine, no heap alias remains.
  Default register addresses, command parsing and continuation branches unchanged.
- No independent cache, queue, state machine, platform code or Lib change added.

## Accounting And Verification

Estimate production seven files +35--55/-30--50. Actual seven C/H files +33/-36,
net -3 (`git diff --numstat c0355b2f -- src` excluding README/manifest).
Tests six C files +149/-5, net +144; test-only CMake and the existing
source-boundary gate together +2/-2, net0.
Documents, manifests and two EXEs are separate. Counts are lower than estimated
because broker replacement and the Debug allocation lifecycle were removed,
not compressed into single-line production statements.

The unopened Debug object now includes 256 pointers (~1/2KiB on x86/x64);
the open object no longer needs a separate allocation. This simplifies lifetime,
not a claim of lower total memory or measured performance improvement.

Both Release builds passed. Eight focused tests passed x64 8/8 (2.51s),
x86 8/8 (5.86s). First full runs both passed 103/105: the source-boundary
gate still required ignoring UI destruction status, and the naming gate
rejected a bare legacy-named header in admission documents. The first gate
now requires terminal failure handling without relaxing teardown order;
documents use the actual Common-qualified path. Final full rerun passed:
`ctest --preset test-x64 --output-on-failure` 105/105 (134.41s),
`ctest --preset test-x86 --output-on-failure` 105/105 (135.13s).
Five desktop tests per width are excluded by these background presets.
No desktop windows, native Linux or new GUI acceptance are claimed. Existing
incremental build trees retained; no disposable probe or trace created.

## Test Packages

| File | Bytes | SHA256 |
| --- | ---: | --- |
| softpc32.exe | 3659012 | 9AEF4AC097DC3666ECCFFB804979FF99800A48AE4AAA37A1395C29DD7CD4BC0C |
| softpc64.exe | 3060994 | 59518ED08C74440247639CEFAEF59566182EEA8AADB89C4E0624714774DDDEE7 |

Snapshot, package INI and guest media are unchanged from c0355b2f.

## Coordinator Actual-Change Review

After executor commit 95850780 was pushed, the same session switched roles and
reviewed `git diff c0355b2f..95850780` against the admitted five-item ledger.
The seven production C/H paths match that scope. App participates only in the
terminal teardown receiver; CMake changes only the fixture compilation source.
Successful disposal precedes pointer clearing, failed disposal retains callback
dependencies, and no raw-to-monitor activation survives in final teardown.
The existing input sink owns TEXT admission; Debug parsing and output code are
unchanged. Constructor outputs and RGB contrast are proved in existing fixtures.
Test/gate counts and package hashes were independently checked against Git/files.
No Lib/VM/Compat/MVDM, INI/media/snapshot change appears in the committed diff.

Both final background suites and documentation governance passed. First-run
static failures remain disclosed above. This accepts implementation delivery
only; S8 and T72 remain open for the owner's manual test. No next S is admitted.
