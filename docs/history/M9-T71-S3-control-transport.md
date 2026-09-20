# T71 S3 Opaque Control Transport

## Request And Preflight

Owner: "好的，接下来准入修复，开始执行。" Reporting refinement:
"你汇报的时候，要汇报增加多少减少多少，净增加还是净减少".
Baseline: 85940465. Scope is the approved control part of the
[T71 design](../history/M9-T71-kvm-text-frame-contract-proposal.md), not frame migration.
Preflight estimated 70--130 changed production lines and 90--170 test lines.
Keep the existing fixed record, 32 ordinary entries plus reserved STOP slot,
independent frame/control locks, wake path and typed leaf APIs. Move command
ownership; do not add allocation, workers, public options or lifecycle states.

## Implementation And Finite Review

- Base mailbox header/C: copied u32 kind plus 128 opaque payload bytes replaces
  the Window-specific enum/union. STOP alone remains transport-owned. No ordinary
  opcode whitelist; capacity, FIFO, reserved STOP, close and lock order unchanged.
- Window window.h/window.c: three private commands, title bytes and one-byte
  frozen value. Existing public entry points and title capacity remain unchanged.
- Window Win32 worker: interpret those commands and reject unknown kind,
  unterminated title or invalid boolean through existing terminal failure path.
  Native title, freeze/release, unfreeze activation and repeated-unfreeze behavior
  remain the original implementation. No added dispatch wrapper.
- Console Win32 worker: only STOP is meaningful. An illegal private ordinary
  record now fails once and retires, instead of silently ignoring Window commands.
  No public Console caller can submit an ordinary command.
- Both Linux leaves: explicitly unsupported startup, no active consumer loop;
  unchanged. component.c retains the sole enqueue/notify/STOP route unchanged.
- Search: rg control kinds/enqueue_control/take_control under src and test.
  All eight direct test consumers migrate; no old Window opcode remains in base.
  A permanent dependency-gate negative fixture prevents that ownership regression.
- Tests cover full-byte copy after caller mutation, arbitrary nonzero opcodes,
  full rejection without overwrite, FIFO/one-slot freeze, repeated STOP, accepted
  wake failure, worker failure/retirement and independence from the frame lock.
  Actual Window consumer tests cover all three malformed record forms; actual
  Console worker test covers illegal opcode and exactly one failure/retirement.

## Changed-Path Accounting

Method: git diff --numstat 85940465, restricted to this delivery's paths;
the new negative fixture is included as added source. Unrelated queue/proposal
work is excluded. No files moved; mostly declaration/field migration, not new
functionality. README/manifests/EXEs are not counted as production C/H.

| Category | Added | Removed | Net |
| --- | ---: | ---: | ---: |
| Production C/H, six paths | 44 | 32 | +12 |
| Production dependency verifier, one CMake path | 4 | 0 | +4 |
| Test C/H, nine existing paths plus one fixture | 102 | 28 | +74 |
| Test CMake registration | 6 | 0 | +6 |

Production C/H changed lines: 76; tests including registration: 136. Both fit
preflight. The new lines are bounded leaf validation and tests, not a new model.
Control payload capacity is unchanged; no new mutable state or heap allocation.
Common/VM/Compat/MVDM, INI and guest media are untouched.

## Delivery Evidence

Both Release build presets succeeded. Focused x64 control/failure/input/lock
tests passed 7/7. Full x64 passed 110/110 (111.67 seconds), including shared
manifests, ownership gates, package and snapshot checks. First x86 full run passed
109/110: keyboard-lifetime's synthetic Window/Console comparison observed unequal
modifiers. Ten focused repetitions passed unchanged. Source audit found Window
queried live desktop key state while Console supplied fixed zero modifiers.
The synthetic fixture now supplies the same zero-modifier input to both adapters
(four test lines, no production change); actual native modifier conversion is
separately covered by win32_vocabulary_ownership. Final serial full reruns passed
x64 110/110 (74.39 seconds) and x86 110/110 (73.88 seconds).
An intermediate documentation gate rejected prematurely placing this active-S
evidence in history; evidence stays here until actual coordinator closure.
Documentation governance and whitespace checks pass. Native Linux behavior and
new manual Win3.1/Win95 interaction are not claimed by this bounded S.

Package SHA-256:

- softpc32.exe: 94F8D349F3554192A80E995BCDA48A9E3538E4981F6979AE4CF6184F6F68D044
- softpc64.exe: 50ADC05E08271B210AE24141DABA08E643E003CA5B9ACA21A0FEEDAF1BBE2F29

Executor self-review and verification are complete for P1; coordinator must
review the pushed actual diff before closure. S4--S6 and T71 remain open.

## Coordinator Closure

Reviewed pushed c1782fcc against the original packet and every changed source,
test, gate, manifest and artifact path. Verified only transport admission and
leaf interpretation changed; FIFO, STOP, wake ownership, frame code and native
actions remain the same. Tests exercise actual consumers, not only copied fakes.
The four-line synthetic modifier correction does not change production input.
All S3 exit criteria are met; S3 closes. No whole-T completion claim: S4--S6
remain required. Baseline and artifact hashes are those above; no new build is
claimed by this documentation-only closure. Separate queue/proposal edits are
preserved and excluded from this S's accounting and commits.
