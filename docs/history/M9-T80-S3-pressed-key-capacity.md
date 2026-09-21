# M9 T80 S3: pressed-key capacity admission

## Request and implementation

Owner admitted the next S after S2 closure, then said “批准修复”. Baseline
`df7d8704`; the [proposal](../proposals/m9-shared-corpus-boundary-and-simplification.md)
contains the pre-change estimate and finite similar-issue ledger. S3 remains
open for owner testing; S4 is not admitted.

The Session ledger still owns exactly 256 physical-key entries. The existing
remember function now returns success/failure, and the existing dispatcher
checks it before downstream delivery. Existing-key repeats use the original
forget/reinsert identity handling. New keys at capacity fail without changing
the ledger or reaching the machine. No new allocations, flags, API or queue.
Session's existing failure return leads to App's existing shutdown sequence;
the change does not silently discard input and continue a compromised stream.

Production control.c: +6/-4, net +2 (estimated +5/-3, same net; explicit branch
braces account for the difference). Test physical_key_identity_smoke.c: +56/-0,
less than estimated +60--90, reusing the existing fixture. Total code +62/-4,
net +58. Two manifests, package EXEs and task documents are counted separately.
No Lib, Core, x86, user configuration or guest-media changes.

## Verification and review boundary

The new test first failed against baseline production: full-capacity dispatch
incorrectly returned success. It then passed with the repair. It covers all
256 slots, a repeat at capacity, a new extended key on the same source, a new
source, rejected-source retirement, explicit release/reuse, and idempotent
retirement. Existing tests retain zero-scan and left/right physical identity
coverage. The test directly calls the existing internal dispatch boundary;
its continued calls after rejection test local invariants, not a new product
policy to continue Session after input failure.

Both Release builds succeeded; common-session flags are
`-O3 -DNDEBUG -std=c11 -Wall -Wextra -Wpedantic -Werror`. No new independent
package configuration or Linux execution is claimed. Background regression
and final artifact results are recorded in the proposal before executor push.
Five desktop cases per width are excluded, not reported as passed.

Executor `aea41c16` is pushed. The same session then switched to coordinator
and reviewed `git diff df7d8704..aea41c16` and all ten changed paths: one
production file, one test, two matching manifests, two tested EXEs and four
task documents. The original ledger remains the only state owner; caller
failure reaches existing cleanup. No public ABI or out-of-scope source changed.
Background x64 111/111 (141.46 s), x86 111/111 (140.30 s), manifests and final
documentation governance pass. Package hashes match the proposal; INI/media
remain unchanged. No remaining admitted implementation gap found in this
bounded review. S3 awaits owner test; no automatic S/T closure is authorized.
