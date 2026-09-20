# M9 T72 S1: Neutral Text Design Review

## Request And Boundary

Owner: "好嘞。准入队列第一条，KVM文本属性中性化，先来个设计稿，我审一审".
Baseline bea66d3f; T71 is closed. T72 is newly admitted for design only.
The [design draft](../proposals/m9-kvm-text-cell-glyph-refactor.md) owns proposed
schema, finite call-site ledger, semantics, costs and estimates. It is not yet
an approved replacement for current architecture or API.

S1 first recorded its New-mode packet in CURRENT and passed the documentation
gate before drafting. The draft is now delivered for owner review; no S2 code
admission is inferred. No numeric build version or artifact changes yet.

## Findings And Design Decisions

- Existing KVM Window and Console both interpret the PC byte attribute and
  attribute_font_select. VM and Common UI status text are the two producers.
- T71 mapping ownership and frame transport are already complete; retain them.
- Propose retaining text[] and parallel arrays, with explicit foreground,
  background and glyph_bank; avoid a broad cell-struct rewrite for no size gain.
- The logical Console output ABI also carries packed native attributes. Include
  its narrow color contract and Broker native conversion in the review proposal,
  rather than hiding raw native encoding inside KVM Console.
- Preserve all current 256-byte-attribute/two-font-select outcomes, including
  intensity/background treatment. Do not introduce blinking or correct devices.
- Static layout estimate: +1996 bytes per text frame; unchanged graphics union
  and Common maximum frame, unchanged logical Console frame size. These are
  arithmetic estimates, not compiler or performance measurements.

Read-only search: attribute_font_select, attributes accesses and
lib_console_write_text_frame across src/lib, src/common, src/vm and test C/H.
Confirmed production output caller is KVM Console only; tests have independent
direct calls. Compat's hardware fact remains private and unchanged.
The draft lists every changed production owner, unchanged boundaries, direct
fixtures, 512-case compatibility matrix and out-of-range/independent-bank cases.

## Accounting, Evidence And Review

This S changes documentation only: production +0/-0, tests +0/-0, no build or
runtime test. Proposed later migration: ten production files about +83/-40
(net +43), tests roughly +180/-35 (net +145); these are not actual changes.
No desktop interaction, temporary diagnostic, original mirror or media change.
The draft explicitly makes the small extra text-copy/validation cost visible;
no claim of performance improvement or code reduction.

Executor checks: documentation governance and diff whitespace; request-to-draft
mapping and source-boundary review. P1 delivers the complete design work only.
Coordinator reviews its actual committed changes before handing it back to owner.
Owner design approval remains outstanding; T72 stays open and S2 is not active.

## P2 Coordinator Review

Reviewed pushed P1 01338e5d: five documentation files, +265/-79; no src, test,
assets or build-configuration changes. Rechecked source ownership, the proposed
ten-file migration arithmetic (+83/-40), byte-layout arithmetic and distinction
between historical evidence and future tests. Console's native output boundary
is explicitly proposed, not already changed. Documentation governance passes.
S1 draft delivery is reviewed; this does not accept the design on the owner's
behalf or authorize S2. Wait for owner feedback with T72 open.
