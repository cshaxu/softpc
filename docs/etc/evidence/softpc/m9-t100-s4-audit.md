# T100 S4 fresh ordered mirror audit

## Frozen universe and method

SoftPC baseline: `6ff24aee`; source delivery: `cda2d427`. Original: XP SP1
`a2f6d7c93aa4f11efc51147566d5961c59a8ef89`, counterpart prefix
`Source/XPSP1/NT/base/mvdm/softpc.new/`. No OpenNT comparison is used.

The [fresh file ledger](m9-t100-s4-files.tsv) captures raw original/local SHA-256
and literal `cmd /d /c "dir /a /b /s"` file order, without an ordering option.
There are 472 files and 15 directories; the file subsequence matches S1 exactly
and equals the tracked mirror set. Directories are structure, not audit points.
254 files are identical after CRLF/LF normalization (binary bytes remain exact).
218 differ before excluding deliberately removed bodies and line controls.
These identity results are not 218 qualified retention decisions or completed
semantic reviews. The hashes/order are frozen; the comparison column records
the completed screening disposition, separately from semantic review below.

Owner then directs excluding the 254 identical files and all remaining files
whose only changes are excluded bodies and their line-control scaffolding.
Screening excludes another 97 files and yields the ordered
[121-file real-diff list](m9-t100-s4-real-diff-files.md). That list is the remaining
review workload; the 351 excluded files need no repeated semantic audit.

Existing excluded branch bodies are matched to their original conditional
parent/arm and omitted without recording their contents or counting them.
Other differences are reviewed in ascending original/local source order.
Line controls are diagnostic scaffolding and are not hidden as semantic fixes.
Counters distinguish selected text hunks, semantic points, added/deleted lines,
evidence-qualified pending decisions and approval. All previous retention
approvals and automatic small-diff delegation remain revoked.

## P1 source review, positions 1-9

| Order | File under base/bios | Selected-body result | Review disposition |
| ---: | --- | --- | --- |
| 1 | bootstra.c | No residual selected-body difference | No retention decision in this body. |
| 2 | build_id.c | Identical after newline normalization | No retention decision. |
| 3 | cmos_bis.c | No residual selected-body difference | No retention decision in this body. |
| 4 | equipmnt.c | No residual selected-body difference | No retention decision in this body. |
| 5 | mem_size.c | No residual selected-body difference | No retention decision in this body. |
| 6 | reset.c | One deletion, +0/-2 | S4-001: product selection, pending decision. |
| 7 | rom_basc.c | No residual selected-body difference | No retention decision in this body. |
| 8 | slave_bs.c | One replacement, +1/-1 | S4-002: original device defect correction, pending decision. |
| 9 | tape_io.c | One replacement, +1/-1 | S4-003: selected CPU40 declaration adaptation, pending decision. |

This first selected-body pass covers nine files and finds three semantic points,
three text hunks and **+2/-4 (six added+deleted lines)**. After owner-directed
screening, 118 of the 121 real-difference files have not received this sequential
semantic review. Diagnostic-anchor necessity
still requires the separate all-file reconciliation; this table does not claim
completion of every source-location difference. No production change or fresh
dynamic test is made in P1.

### S4-001: reset.c, retired software-driver lifecycle

XP lines 910-911 call mouse_driver_termination and mouse_driver_initialisation
on warm reset; current reset.c omits them before its line-912 anchor (local 629).
Category: product/subsystem selection, not an original mouse hardware bug.
Historical correspondence R0006 is context, not approval.

Fresh source inspection finds reset.c in the production device list and its
per-file Compat BIOS include boundary in CMakeLists.txt. The complete current
src/test search finds neither software-driver lifecycle symbol. The actual
hardware mouse initialization remains mouse_init at reset.c:926; the keyboard
controller and host input path are not removed by this omission. Restoring just
the calls is not a coherent subsystem restoration: the old optional software
driver implementation has been retired.

Counter-review: absence of the symbols does not prove an XP defect or that a
coherent reintroduction could never work. This is an explicit current product
scope decision, distinct from make/break input or PS/2 controller correctness.
No original-restoration build or guest reproduction is claimed in this P.
Recommendation: present the two-line product-selection difference for owner
disposition; do not label it a hardware fix or manufacture stub replacements.

### S4-002: slave_bs.c, slave PIC EOI port

XP line 110 sends 0x20 to ICA1_PORT_1; current line 107 sends it to ICA1_PORT_0.
Category: original device logic correction. Historical correspondence: R0008.
Production BOP registrations in Compat device_bop.c bind D11_int to unused
slave vectors 72h/73h/74h/76h/77h. ios.h defines the command port as A0h and
the data/mask port as A1h. In SWPIC_outb, the data-port write sets ica_imr,
whereas the command-port OCW2 non-specific EOI clears an in-service request.

The current D11 path first masks the serviced slave line, sends slave EOI,
then master EOI. The original port would overwrite the slave mask with 20h
instead of acknowledging the slave ISR. This follows the current actual port
implementation, not merely an analogy to another emulator.

Counter-review: this affects D11's serviced-slave branch, not every interrupt
or a claim that normal DOS always exhibits a fault. The existing
device_contract_smoke fixture checks slave service, master service, spurious
cascade and no-service cases, including ISR and both masks. Its code was read
fresh; no new execution or pre-fix failure is claimed. Recommend retaining the
one-token correction, subject to fresh owner approval.

### S4-003: tape_io.c, descriptor limit output type

XP line 96 declares the limit output as word*; current line 83 uses sys_addr*.
Category: selected CPU implementation/ABI declaration adaptation, not an
unqualified claim that the original pre-CPU40 product was broken. Historical
correspondence: R0010.

Production tape_io.c supplies sys_addr source_limit/target_limit to its two
retrieve_descr_fields calls. The built Compat ccpu/facade.c definition takes
sys_addr* and delegates descriptor decoding to read_descriptor_linear, then
copies descriptor.limit. It does not introduce another descriptor decoder.
The original declaration's narrower pointer type disagrees with this real
definition and its callers. The fixture contains 64-KiB, 0x12345 and 0xffffffff
limits with neighboring canaries and exercises the BIOS block-copy caller.

Counter-review: a mismatched pointer declaration is not itself proof that the
machine-code call immediately truncates its result; the current function writes
through its own sys_addr* declaration. The proven issue is the incompatible C
interface for the selected CPU path. Do not misreport this as a demonstrated
runtime truncation. Recommend the one-line declaration correction pending owner
approval; no new dynamic control or build is claimed here.

## Progress and verification boundary

All 472 identities and order are frozen; first selected-body frontier is 9/472.
Category subtotal for the first three points: original defect +1/-1, selected
CPU ABI +1/-1, product selection +0/-2. All three are unapproved. There is no
restored source, new helper, second execution path or changed artifact in P1.
The historical 381-ID count is not asserted as the new audit's final count.
Documentation/manifest checks qualify this read-only delivery, not runtime
behavior or all-file audit completion. After screening, the next remaining file
is position 41, base/ccpu386/c-files; dependency reads do not advance that frontier.
