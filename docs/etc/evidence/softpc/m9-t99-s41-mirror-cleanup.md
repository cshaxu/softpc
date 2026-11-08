# T99 S41: retained mirror cleanup

## Scope and accounting

Baseline 76e1d839; fixed XP SP1 a2f6d7c93aa4f11efc51147566d5961c59a8ef89.
Five admitted units C1/C2/C3/F1/F2 stay within four mirror owners. No public
component, guest binary, configuration, media or snapshot changes.

Primary measure is mirror added+deleted lines, LF-normalized with
SequenceMatcher(autojunk=False), comparing the same pinned XP bytes before/after:

| File | Before + / - | After + / - | Diff reduction |
| --- | --- | --- | --- |
| base/disks/fdisk.c | 176 / 73 | 166 / 73 | 10 |
| host/src/nt_sbmxr.c | 70 / 65 | 67 / 65 | 3 |
| base/system/quick_ev.c | 303 / 135 | 303 / 135 | 0 |
| base/keymouse/keyba.c | 418 / 1214 | 418 / 1214 | 0 |

Total mirror diff decreases **13 lines**; XP deletion count unchanged. S39's
previous six-line reduction is separate. Correctness changes are not booked as
additional diff savings. Source production Git numstat: +16/-29, net -13.

## Minimality and ownership review

HDD mapping retains exact 0=NULL, 1=restore, 2=seek, 3=rsector, 4=wsector,
5=format, 6=rverify, 7=diagnose, 8=setparams, 9=bad. Array length supplies bounds;
invalid functions/IDs retain -1/NULL. It is a private fixed mapping, never written
at runtime. The original non-ANSI compatibility header erases const; this task
does not change that global mirror convention or claim memory-page protection.
No runtime registry or second device dispatch is introduced.

The queue search now reaches its existing FALSE return instead of dereferencing
NULL when the hash contains an absent node. The keyboard guard now rejects
active unencodable pointers while keeping inactive -1 valid. No recovery state,
new production API or timer is added. Existing field encoding stays in its owner.

Similar-issue sweep: review local snapshot callback maps in fdisk/fla/com/printer/
keyba/timer, Mixer declarations in implementation/header, and queue/keyba guards.
Single/two-callback maps remain simpler as conditionals. PIT wait/gate allowed
sets differ and remain separate. Mixer implementations still consume their sole
header declarations. No remaining identical NULL/NULL ternary or impossible
anomalous_index < -1 check exists in the two affected source files.

## Focused verification design

- Existing checkpoint test covers HDD IDs 0–9 roundtrip, rejects -1/10 and
  restores original state. Source review independently checks unchanged ID order.
- Existing checkpoint test injects an unknown keyboard break pointer into
  existing globals: inactive capture returns index -1, active capture rejects;
  original state is then restored. No test production entry point is added.
- A 33-line source-local queue fixture uses the real private capture function:
  valid two-node hash chain, orphan after a valid prefix, and empty ordered list
  with an orphan hash node. Existing checkpoint test retains real queue restore,
  callback order and no-callback-during-restore proof.
- The fixture is a standalone optimized translation unit using existing device
  compile definitions. Whole-program compilation discards unused original host
  entry points; no duplicated host stubs or transformed production source.

First build failed only at the new fixture link: section GC alone retained
unrelated MinGW host references. Replaced those fixture-only flags with
-fwhole-program; this is not a product failure or a passed first build.

## Qualification

Cached x64/x86 builds pass and publish both package EXEs. Each architecture
passes 188 background + 8 serial native cases: **392/392**, zero failures,
skips or disabled cases. JUnit names match the full 196-case CTest registration
per architecture, without missing or duplicate entries. Background elapsed time
is about 90 seconds each; native x64 24.68 seconds, x86 24.06 seconds.

Both focused tests pass in both architectures. No manual guest acceptance or
normal-guest reachability of injected corruption is claimed. No Computer Use.

Published SHA-256:

- softpc32.exe: f451d415b5d71bb8a3fed62154a9f444408f315c900ff2faedcee9c1da1ebae9
- softpc64.exe: 41e73f07eddd9c39c4c31f882d38adce1e226f019a80de4951dc3be88d6c54e8

Test C changes: +58/-1, net +57 (33-line private fixture plus existing checkpoint
extension). Root test registration: +12/-0. These are not mirror-diff savings.
Production counts exclude tests, CMake, docs/manifests and binary artifacts.

Final successful incremental build logs contain 89 x64 and 356 x86 warning
lines, all located in preserved mirror headers/source; zero non-mirror warnings.
The earlier x64 attempt logged 357 mirror warnings and the recorded fixture link
failure. No warning suppression or removal of required checks was used.

Documentation and 475-file mirror gates pass. New/edited mirror files retain
CRLF; new tests and documents use LF. Public eight trees and INI are unchanged.
Actual-diff self-review checks exact command IDs, failure-before-dereference,
restoration after test mutation, no production-only test interfaces and no
#error changes. Task scratch is removed after promoting this evidence.
Owner acceptance and closure followed on 2026-11-08; see [closure](../../../history/M9-T99-S41-mirror-cleanup.md).
