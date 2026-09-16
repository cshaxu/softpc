# D6/SALC and retained BOP contract assessment

T60 S6 follows the admitted
[reference-repair plan](../history/M9-T60-reference-repairs-proposal.md).
Baseline is S5 implementation 3dbaf32. This stage assesses instruction decoding
against the selected firmware/host contract before proposing any repair.

Inspect the selected CPU D6 dispatch, original OpenNT behavior, NTVDMx64's
research lead, firmware BOP encodings, Compat dispatch and existing BOP tests.
The finite ledger is D6 execution, C4/C4 BOP decoding, firmware return/escape
contracts including BOP FE, and alternate decoder compile-time branches.
Each entry needs a source-backed disposition and focused proof where runnable.

Do not automatically replace D6 with SALC or change firmware, BOP numbering,
Lib/Common, guest media or the executor backend. If SALC conflicts with a
retained product contract, document the concrete conflicting callers and ask
the owner for a decision rather than silently broadening this stage. External
patches remain read-only research, not import payloads.

For changes, retain minimal original-source diff and deliver both fixed EXEs,
focused and serial full suites, executor commit/push and coordinator review.
For an assessment-only disposition, record exact paths, selected build branch,
test evidence and limitations; do not claim complete opcode compatibility.

## Assessment evidence and decision boundary

The selected c_main.c D6 case consumes an immediate byte and dispatches BOP;
FE exits through c_cpu_unsimulate. The independent C4 case recognizes C4 C4
and retains FE unsimulation plus C4 C5..C7 fast operations. The selected build
defines CCPU/CPU_40_STYLE/CPU_486, not PIG or SFELLOW. The NTVDMx64 minnt
ccpu.patch research lead instead makes D6 SALC unless BOP8086 is selected.
These are different instruction-length and state contracts, not just different
implementations of the same instruction.

Raw ROM searching is not instruction decoding: bios4 D6 FE hits at decimal
7465/7721 lie in the traditional 8-row character data beginning at offset
0x1a6e. V7 hits 9977/10426, 13339/13595 and 16136/16649 lie in the font ranges
identified by the existing 0x2230, 0x3160 and 0x3990 definitions in sas.h.
bios1 has no D6 FE sequence. This removes those byte hits as evidence of an
executable D6 return, but does not prove there are no other D6 BOP callers.

rom.c documents an option-ROM return through BOP FE; bios4 contains C4 C4 FE
sequences. Its literal FE00:095A return pointer must not be claimed to point
directly at one: byte inspection there is not C4 C4 FE (a nearby sequence is
at 0x965). This is an assessment limitation, not an admitted firmware repair.
compat/edl_fast_bop.c retains explicit failure for unsupported extended BOPs.
Existing bop_smoke mainly invokes the service dispatcher directly, so its
passing result does not independently prove the instruction decoder contract.

## Owner disposition

The owner rejected changing the historical contract: "不批准。类似请求一律不许可。"
Then requested continuing implementation. D6 BOP is retained; SALC substitution
is not adopted. Future reference-guided repairs must not sacrifice existing
compatibility or repeatedly seek the same exception. This is an explicit
non-adoption disposition, not a claim that D6 implements SALC.

All four assessment ledger entries retain their existing behavior: D6 BOP;
C4/C4 BOP; firmware/FE escape; PIG/SFELLOW alternate branches (not selected).
No decoder, firmware, Compat, test, EXE or shared corpus is changed by S6.
Production/test line delta is zero. The existing S5 dual-width evidence remains
the artifact baseline; no new build or opcode-level verification is claimed.
The proposed SALC-specific tests are not added because that behavior was
rejected. Assessment limitations above remain limitations, not new repair scope.
Documentation governance and whitespace checks are required for this delivery;
coordinator actual-commit review precedes archival and S7 activation.

Coordinator reviewed actual commit 2da47d4: only this assessment and the parent
plan changed. The owner decision is accurately recorded, no implementation
was smuggled into the disposition, and the four ledger entries remain intact.
S6 closes as owner-rejected adoption. T60 stays open for S7 and whole-task review.
