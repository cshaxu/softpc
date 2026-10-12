# M9 T88 S6 — CHAIN2 continuation acceptance

Owner accepts the delivery after complete dual-width qualification, then approves
T88 closure on 2026-10-11. S6 is closed. This is a document-only actual-change
review of implementation f1d394ca and qualification a10fce4f, not a new repair.

## Accepted contract and scope

Seven retained original files contain 48 proved same-procedure tail transfers:
38 write continuations and ten read continuations. Their old function calls
re-enter crules without saving a parent carrier; the existing LEAVE restores
LocalIUH from a new activation's zero CopyLocalIUH. A local goto preserves the
current activation. The independent 192 diagnostic experiments reach every
site on both widths; original calls all lose saved locals, local-transfer
references all restore them. Outward original outcomes are separately classified,
not represented as 48 normal guest crashes.

Actual production diff is +48/-96, net -48, with inline reason markers.
Real S_* calls, continuation wrappers, device read/write algorithms, dirty
accounting and snapshot format remain unchanged. Owner expressly approves
the original-mirror delta after clarification: these seven files' OpenNT
diff grows by 144 touched lines, not shrinks.

The existing VGA receiver gains one native-rule ABI translation unit and a
minimal call site. ModeX decomposition, all five writers' compound replay,
both read modes/layout flags and destination canaries are exercised; the
existing source gate enforces the exact 48-site ledger. Verification/build
changes total +142/-1, net +141. Neither test-only native state nor the ignored
diagnostic sources enter the product link. No shared-root changes exist.

## Qualification and artifacts

Each width passes 84 shared plus 47 App/check tests (131 background), followed
by all five native desktop cases, serially and without Computer Use.
x64 desktop is 5/5 in 11.88 s; x86 is 5/5 in 12.04 s.
Complete registered coverage is 136/136 per width. Focused VGA and snapshot/
integration cases pass. Earlier fixture failures are not relabelled as passes.

Accepted EXEs remain those of f1d394ca:

- x64: 3094348 bytes, SHA-256
  99D8F132642A0A776E0F1E4FE6635D081FA3CE6CD145067AD171E3EF633259E3.
- x86: 3494060 bytes, SHA-256
  386DE2AF207FA40845029D3540FC26212B47BC5CCE2F1B7D7035A5731D752D47.

INI, disks and the owner snapshot are untouched. Existing RDP capture and
null-BOP debt are unrelated and remain explicitly deferred; no TODO residue
for the diagnosed continuation bug remains. Other mechanical continuation
candidates and arbitrary legacy bulk algorithms are not certified by this scope.

[Retained diagnosis, site ledger and implementation evidence](M9-T88-win31-file-manager-vga-width-transition.md#s6-nested-chain2-rule-investigation).
[T-level actual-change audit](M9-T88-completion-audit.md).
