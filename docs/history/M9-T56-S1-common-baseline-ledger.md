# M9 T56 S1: common extraction baseline ledger

## Owner requirements

The owner admitted T56 to establish common components for shared VM
infrastructure while preserving SoftPC behavior. S1 freezes the reference
before responsibility moves. The full original request, ownership design and
finite 60-path ledger are in the retained live
[common convergence proposal](M9-T56-common-convergence-proposal.md).

The owner subsequently approved asynchronous hand testing: every S still
requires dual packages, automated proof, review, commit and push, but the next
S does not wait for synchronous manual confirmation.

## Executor evidence

P1 `b22a32e` recorded the 24-path app plus 36-path host ledger, including
threads and direct-lib ownership. It found no behavior-test gap that could be
filled without creating a duplicate test route. It did not modify production
source, MVDM, library source, configuration or media.

- `cmake --build --preset tests-x64` then `ctest --preset test-x64`: 58/58.
- `cmake --build --preset tests-x86` then `ctest --preset test-x86`: 58/58.
- Standalone strict library build and CTest: 8/8.
- Documentation gate and `git diff --check`: pass.
- Package SHA-256: x86
  `FDE1E2BEF0802AE92FC6434A0F8B4524AC9AE53F57F8BAC9C55DCC335922797A`; x64
  `8BC57B396190BC97994AF042E817E95C23A3E6E6E437AAC8D1D4EBD5D15D696C`.

## Reviewer closure

Reviewed P1 actual diff against the S1 packet and proposal. The changed paths
are only the active packet and candidate proposal. The ledger covers the
frozen universe and assigns each responsibility, worker and lib-call family a
single receiver; it explicitly preserves legitimate app/product direct lib
use. The newly identified speaker-worker assembly obligation has S5 as its
receiver and does not create a second thread in S1.

S1 is closed. S2 may now extract common/ui without altering the frozen product
semantics.
