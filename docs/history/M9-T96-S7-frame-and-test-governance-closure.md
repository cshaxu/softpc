# T96 S7 frame and test governance closure

## Owner decision

Owner accepted the delivery: "验收通过，收口S7，保持T96开放。"
S7 is closed. T96 remains open; no S8 or other implementation is admitted.
This closure changes documentation only, not production, tests, EXEs or media.

## Delivered scope

- Direct Console frame production; Session remains the routing coordinator.
- Separate Window text/graphics payloads, exclusive pointer discrimination and
  Window-owned sequence; remove the Machine frame envelope and transport copies.
- Retained Console input restores raw output through the existing output path;
  no extra backup, Machine publication or second renderer.
- One content-opaque four-slot exchange in KVM Base for both consumers.
  Driver decides readiness; Machine publishes every produced frame. Consumer
  comparison and native invalidation determine actual output work.
- Active-extent Console conversion and actual font-register readiness tests.
- Owner-local test cleanup preserves unique failure/platform/native assertions,
  adds exchange failure/allocation proof and precise negative-test diagnostics.

The detailed bounded ledgers and historical evidence remain in the
[proposal](../proposals/m9-lib-rgb-graphics-frame.md).

## Final delivery and review

Executor d62567e5 and coordinator review dfc85aac are committed and pushed.
The final cleanup changes no production code. Test C/H totals +86/-189,
net -103; test CMake +46/-40, net +6. Three duplicate targets were removed
with unique assertions retained in their responsible tests.

Each architecture passes focused 25/25 and all 191 distinct registered tests:
175 unit/gates, eight background integration, five desktop and three native
audio. Zero failures/skips in the final qualification, exit 0.
Evidence is under build/t96/s7/test-governance. All 289 public manifest entries
match. Existing mirror/ABI warnings and x86 Ninja recovery remain disclosed;
this is not a zero-warning or measured-performance claim.

Published SHA-256:

- softpc32: D16647A87671559B1D114AD282E904529615DB25267D91640D72039758D5297B
- softpc64: E1B95ED71ACB371098C854FEF6FD4E3573B71BE98169243958F13DD2900A53C1

The cached x64 target was unchanged and reused; x86 rebuilt after Ninja
recovery. This documentation-only acceptance does not require another rebuild.
User-owned untracked assets/binary/snapshot.debug is preserved and excluded.

## Remaining T96 disposition

All admitted S1-S7 work is complete. Whole-T closure still requires the final
request-to-delivery audit, including the final schema/ownership contract,
fixed-frame storage accounting, downstream adoption instructions and evidence
reconciliation after the S5-S7 revisions. No new runtime defect or additional
numbered S is inferred merely from that pending audit.

The intermittent Win3.1 prompt-roundtrip readiness investigation remains in
[TODO](../states/TODO.md). Later passing runs do not prove its cause or repair.
Other pre-existing TODOs and queued product bugs are not silently closed by
this S acceptance. T96 may only close on the owner's subsequent decision.
