# T60 reference-repair completion audit

Owner acceptance: "我测试完成了。请你收口T60，准入T61."
Audit baseline is 549ed44; delivered tree is 3df951c. This closes the bounded
reference-guided repair plan, not every historical MVDM divergence or x87
conformance. The original request and exclusions remain in the
[admitted proposal](M9-T60-reference-repairs-proposal.md).

## Finite completion ledger

| Stage | Disposition and retained owner | Evidence |
| --- | --- | --- |
| S1 | PIC rejection guard at the original CPU interrupt entry; valid interrupts retained. | [S1](M9-T60-S1-pic-rejection.md), dual-width full 97/97. |
| S2 | Bounds for seven selected painters; existing pixel kernels retained. | [S2](M9-T60-S2-painter-bounds.md), 91 boundary cases, full 97/97 each. |
| S3 | Low address bits, row intersections, split/wrap and dirty clearing corrected in existing update paths. | [S3](M9-T60-S3-dirty-address.md), 864 configurations, 8,640 surface comparisons, full suites. |
| S4 | Existing planar painters consume horizontal pan; register invalidation and predecessor dirty dependencies included. | [S4](M9-T60-S4-pel-panning.md), 384 pixel cases and 1,728 update configurations, full suites. |
| S5 | Byte/word writer declarations corrected; erased prototypes explain why current-build truncation was not reproduced. | [S5](M9-T60-S5-cvid-writer-contract.md), complete signature audit and actual glue test. |
| S6 | No adoption: retain D6 and C4/C4 BOP, reject SALC substitution per explicit owner decision. | [S6](M9-T60-S6-d6-bop-contract.md), no implementation change. |
| S7 | FIST64 rounds before conversion; safe representation copy; existing double-based backend retained. | [S7](M9-T60-S7-fpu-assessment.md), 228 conversions, 17 roundtrips, finite arithmetic/precision characterization. |

All seven have actual-commit reviews. Final serial regression is x64 97/97
(98.60 s), x86 97/97 (75.64 s); post-commit focused proof is 3/3 each plus
five repeated command-provider passes each. The owner now confirms manual
testing. The earlier isolated debugger expected-IP assertion is recorded in
S7, not claimed repaired or erased by reruns; no reproducible FPU regression
remains in the bounded acceptance matrix.

## Original-diff and boundary audit

Reviewed the ten functional MVDM paths against task baseline and original
OpenNT. They retain original CPU, video state, searches/coalescing and painter
owners; there is no alternate renderer or FPU. S3 already withdrew the broad
update rewrite and restored avoidable conditional/shift changes. Necessary
row-intersection and wrap-placeholder repairs are not merely code reduction.
S7 changes only conversion representation and FIST64 rounding. Native FPU,
NTVDM/WOW/HAXM work and compatibility-breaking substitutions remain excluded.

Reproducible accounting: git diff --numstat 549ed44 3df951c -- src test.
Functional production: 10 files, +304/-214. Owner-approved restoration of
trailing blank lines in 18 other mirror files: +0/-21; these match their
original peers and are formatting-only, not functional repairs. Total
production +304/-235 (net +69); three product tests +471/-10 (net +461).
These are task deltas, not total divergence of the whole mirror from OpenNT.

Lib/Common and both shared test roots have zero task diff. App/VM/Compat,
firmware and media are unchanged. Fixed EXE hashes remain those in S7;
this closure does not rebuild or modify them. Worktree was clean at admission
of this closure. No build-time transformation or external runtime dependency
was introduced.

## Closure and successor boundary

T60 closes on owner acceptance. Existing Queue candidates retain their order;
TODO has no deferred items. The new owner-admitted T61 takes the independent
whole-mirror divergence inventory and safe simplification request. It is not
an assertion that T60 minimized every pre-existing mirror difference. T61
must preserve T60 repairs and D6 compatibility while auditing those older
differences. There is no outstanding T60 implementation stage.
