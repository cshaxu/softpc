# M9 T56 S3: common session extraction

## Owner requirement

Move the accepted neutral control queue, reducer, presenter-action derivation
and prompt scheduling into `common/session`. Keep SoftPC's CLI as an injected
provider, delete the old app session implementation, and deliver both package
architectures without waiting for synchronous manual acceptance.

## Executor delivery

P1 creates `src/common/session` and makes it the production control-loop
owner. It owns the copied event FIFO, desired/actual state reduction, frame
sequence gate, UI action order and monitor prompt scheduling. Its public
contract accepts three injected facts: a neutral machine adapter, the product
command provider, and copied UI/runtime completions.

`src/app/main.c` now performs only configuration, object assembly and the
SoftPC-specific adapters: lifecycle enum conversion, guest-input conversion,
floppy command handling and hotkey meanings. It neither owns a control queue
nor reduces state, schedules prompts, or executes UI actions. The old
`app/control`, `control_state`, `reconciler` and `presentation_plan` ownership
is deleted in the same delivery; no forwarding route remains.

Completion wording remains provider-owned and is held until the cooked monitor
is Current. This preserves the raw-Console rule: a runtime completion cannot
write monitor status text onto a raw Console. The source-boundary gate rejects
both restored old session paths and new reducer/queue implementation in app.
The types-layout self-test now clears every fixture probe at entry so a prior
interrupted negative probe cannot corrupt a later positive check.

## Verification

- x64: `cmake --build --preset tests-x64`, then `ctest --preset test-x64` —
  58/58 passed.
- x86: `cmake --build --preset tests-x86`, then `ctest --preset test-x86` —
  58/58 passed.
- strict standalone `src/lib` build and CTest — 8/8 passed.
- Documentation/DAG gates and `git diff --check` passed.
- Package SHA-256: `softpc32.exe`
  `341AD4B96301BB8249F30AC4697E932DB640E1E7A4CB4316ADBE276FD32A394C`;
  `softpc64.exe`
  `1246E4BB3557CC63EA5B12187676E9D341BE6C58F6479908BC0F550F4D97F637`.

Only the two package executables were refreshed; the user-owned INI and all
media bytes remain unchanged.

## Reviewer closure

Reviewed commit `cc60255` against the S3 packet, product UI authority and S1
ledger. The initial enum-boundary regression was corrected before delivery:
common and app runtime states are explicitly mapped rather than cast by
numeric value. The final source has one production control route, one command
provider and one completion-to-monitor path. S3 is closed; S4 may extract the
generic machine executor while retaining the SoftPC driver.
