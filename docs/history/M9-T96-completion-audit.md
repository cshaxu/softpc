# T96 Completion Audit

## Owner decision and finite ledger

Owner confirms the corrected Win3.1 configuration works and requests:
"works! 好了收口本S和本T，准入下一个T任务".
On 2026-10-17 this accepts S8 and authorizes T96 closure and queue-head admission.
Frozen scope: S1-S8, final RGB/schema/ownership contracts, storage accounting,
downstream adoption, qualification, owner inputs and explicit debt transfers.
The [retained proposal](M9-T96-lib-rgb-graphics-frame-proposal.md) preserves the
original design and subsequent owner revisions; later four-slot decisions
supersede copied-frame and three-slot designs, not silently coexist with them.

| Obligation | Final disposition and proof |
| --- | --- |
| S1 design | 7d0d3dea/c951e24c; inventoried producer/consumer/storage and atomic migration. |
| S2 RGB | fa569001/94a0debd; one numeric 0x00RRGGBB graphics contract, source palette resolved in SoftPC Driver. Synthetic >256-colour tests do not claim a downstream game is fixed. |
| S3 adoption | cba71230/3758a33e; initial corpus handoff, superseded by final adoption notes below where schema changed. |
| S4 buffer audit | 6ec405bd/9cf0ab43; accepted initial retention analysis, later owner-authorized S5-S7 replace that intermediate design. |
| S5 pointer ownership | ccecf953/d76355ed; bounded references replace copied transport under approved ownership rules. |
| S6 repair | 21b64548/b25f9969; exchange/failure/concurrency audit corrections and qualification. |
| S7 final schema, recovery and tests | Owner closure 5e76dd49; direct leaf production, shared four-slot exchange, retained Console restoration, owner-local test cleanup. |
| S8 MIDI and mouse | a404740f/5ea4e55e; IRQ9 conflict and Mapper routing corrected in configuration guidance; both widths play Canyon then Chord and restart with working mouse. Owner now confirms works. |
| Final regression | S8 qualification exit0; each width 191/191, no failures/skips, exact registration-to-XML name equality. |
| Assets and debt | Original mirror, INI, media and owner snapshot unchanged. Existing TODOs remain, particularly prompt readiness, unsupported SB-MIDI and Ninja recovery. |

## Final architecture and storage

Read current leaf headers, Machine allocation/publication, Base exchange and
current architecture. Window owns text-with-fonts and graphics-with-RGB; exactly
one pointer is non-null, with sequence in the Window reference. Console owns
text-with-character-maps and has no bitmap fonts. There is no Machine frame
envelope or graphics character-map payload. Driver fills the writable leaf
backing; Machine publishes every produced result. Session/UI route exchange
references, not whole-frame copies. Driver owns readiness; consumer owns output
comparison. Explicit diagnostic copies are not the presentation path.

Machine allocates one Window and one Console exchange. Base owns the single
opaque algorithm, four slots each: producing, pending, processing, displayed.
Pending can be replaced without touching processing/displayed; successful finish
promotes the new input. Publication neither allocates nor copies payloads and
does not wait for consumer rendering; pointer bookkeeping still uses a short
mutex. No lock-free or unconditional speedup claim is made. Window's native RGB
surface and Console's transient native-format conversion remain separate duties.

Fresh sizeof probes built against current headers, both widths:

| Storage | x64 bytes | x86 bytes |
| --- | ---: | ---: |
| One Window backing | 3,932,200 | 3,932,184 |
| Four Window backings | 15,728,800 | 15,728,736 |
| One Console text backing | 17,116 | 17,116 |
| Four Console backings | 68,464 | 68,464 |
| Combined fixed payload allocation | 15,797,264 | 15,797,200 |

These totals exclude exchange/mutex/allocator metadata, original indexed DIB,
native Window surface and transient output conversion. They are not process RSS.
The payload bounds are fixed at creation; no three-slot variant or per-frame
malloc/free remains. Evidence: build/t96/closure/layout.c and dual executables.

## Final downstream adoption

Import the final public src/test corpus together, not the earlier S3 hash list
alone. Adopt Base exchange API and current Window/Console headers atomically
with Emulator Machine/Session/UI. Product drivers must fill both appropriate
leaf outputs, publish complete RGB words for graphics, and report produced
only for complete frames; always-produced drivers remain valid. Text producers
provide Window glyph bitmaps and Console character maps independently.
Do not shallow-copy slot storage or retain borrowed pointers beyond ownership.
Current diagnostic copy helpers rebind pointers into owned backing. Consumer
destruction precedes exchange destruction; native rendering stays with Lib.
No changes or acceptance claims are made for other repositories.

## Qualification, changes and closure

Final runtime baseline is 5ea4e55e. S8 reruns cached dual builds and all 191
distinct tests per width: 175 unit/gates, eight background integrations, five
desktop and three native audio. Ten XML reports per width exactly match CTest
registration; qualification.exit=0 under build/t96/s8/qualification. Original
warnings and x86 Ninja-log recovery remain disclosed, not claimed fixed.
S8's x86 guest launcher empty exit file is not counted as exit0; its inspected
guest stages/cleanup and separate full-suite results retain their own evidence.

Against pre-T96 f6385a76, git diff --numstat on tracked C/H paths yields:
production 41 paths +692/-400, net +292; tests 47 paths +1691/-1017, net +674.
Documentation, manifests, build scripts and binary artifacts are excluded from
those code counts. Original softpc.new has no diff across T96. Individual
delivery ledgers retain intermediate accounting; do not sum them as final diff.

Final package SHA-256:

- softpc32: 6A65D5127C6EBF567DCB7FD32F506541BF028FD42B69F9113CB09B363DF52C9A
- softpc64: E1B95ED71ACB371098C854FEF6FD4E3573B71BE98169243958F13DD2900A53C1

S8 and T96 close under explicit owner acceptance. This closure is documentation
only and reuses the just-qualified EXEs; no rebuild or test pass is invented.
User-owned untracked snapshot.debug remains preserved/excluded. Next queue head
becomes T97 S1: Win95 unavailable volume control; safe-shutdown restart remains
queued. No unrelated debt is removed or considered repaired by this closure.
