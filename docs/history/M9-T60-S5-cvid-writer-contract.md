# C-VID writer declaration consistency

T60 S5 follows the owner-admitted
[reference-repair plan](../history/M9-T60-reference-repairs-proposal.md).
Baseline is S4 implementation 8ac53ac. Preserve minimum original-source diff;
Lib/Common and their tests/manifests remain protected.

Inspect EVID_WRT_POINTERS byte/word declarations in cpu_vid.h against all
selected table initializers, call sites, generated-rule implementations and
the Compat C-VID bridge. Do not infer runtime corruption from a suspicious
declaration alone: generated rules may use their existing register protocol.

The finite ledger is byte write, word write, dword write and related fill/move
signatures. Each receives a verified correction or an evidence-backed no-change
disposition. Keep original dispatch and generated files; no new generation,
backend or broad prototype cleanup.

Verify address widths above 0xff/0xffff and value truncation through the actual
selected path. Any code delivery requires both EXEs, focused tests, serial
full x86/x64 suites, protected-path checks, commit/push and actual-commit review.
If the actual ABI cannot be changed narrowly, present that boundary rather
than introduce an unapproved replacement interface.

## Initial evidence

The table's byte/word prototypes reverse address/value widths relative to
evidfunc.h and write_byte_ev_glue/write_word_ev_glue. The selected non-ANSI
build erases IPT2 to (), however, and generated sevid001 rules consume r2/r3
(jccc parameters), not their four C wrapper arguments. Consequently this is
a latent prototype correction, not a reproduced selected-build VRAM fault.

A product test temporarily installs promoted-argument recording callbacks in
the original c_ev_write_ptr and calls the real glue. Before the header repair,
both 0xA1234 byte and 0xB1234 word addresses and values arrive intact on x64.
The first test attempt used narrow callback values incompatible with the old
unprototyped pointer, failed compilation and was corrected to promoted values;
the old binary's subsequent pass was not counted as new-test evidence.
The successful rebuilt test proves current glue transport, not every generated
rule's C ABI. The two-line declaration correction plus local reason marker
does not redesign that legacy protocol. Remaining ledger and dual-width proof
are pending.

## Finite signature audit

Grouping every Write/Fill/Move declaration in evidfunc.h finds 53 byte,
53 word and 53 dword writers; 53 of each fill width; and 318 move declarations.
All writers use IU32 addresses with IU8/IU16/IU32 values. Fill uses those same
types plus IU32 count; move uses IU32 destination, IHPE source, IU32 count,
IBOOL source flag. The corrected table now matches those declarations.
Dword/fill/move need no type changes; the historical move parameter name eaVal
does not change its IU32 ABI and is retained to minimize the original diff.

ev_glue assigns the chosen table and sets jccc parameters before byte/word/
dword, fill and move calls. Its additional chain2 byte/word entries use the
same protocol. j_c_lang maps jccc_parm1/2 to r2/r3; original generated simple
byte/word rules extract IU32 address and IU8/IU16 value from those registers.
The generated four-argument IUH wrappers remain untouched: this task does not
claim to modernize or prove their entire historical C type compatibility.

Production diff is cpu_vid.h +3/-2 (two corrected declarations and a reason),
product test +32/-0; no new target or production state. Current fixed packages
were rebuilt for both widths. Shared protected paths remain unchanged.

## Executor verification

The read-only NTVDMx64 cvidc.patch cpu_vid.h hunk names these exact two
address/value declaration corrections. They are also independently justified
by the in-tree writer declarations and glue audit above; no patch payload or
generated implementation is imported.

Both full builds succeeded. Serial full CTest passed 97/97 on x64 (83.25 s)
and 97/97 on x86 (73.03 s), including the rebuilt writer transport probe.
Documentation governance and git diff --check pass. Against 549ed44 the four
protected shared roots still have zero diff. Comparing cpu_vid.h with OpenNT
also shows the older READ_POINTERS divergence; S5 neither changes nor counts
that pre-existing difference as its repair. S5 production is +3/-2, tests
+32/-0, combined net +33; the objective remains minimum necessary original
source divergence, not minimum net lines.

Fixed x86 SHA256:
01BDE429A9DF82CF7BE66BF251A058621B5C69C60A940CEA1BD3E7DBC7F7384B.
Fixed x64 SHA256:
BBC571741BC7E9592E9F183DAA6E16E1ECE96141E9E60DEF9736863456991C24.
Executor delivery is verified; coordinator actual-commit review remains
required before S5 closure. T60 S6/S7 remain outstanding.

## Coordinator actual-commit review

Reviewed pushed executor 3dbaf32, including both declaration edits, the real
glue probe with restored table, artifact paths and finite signature ledger.
No generated rule, dispatch, unrelated mirror code or protected corpus changed.
The callback probe proves transport under the selected erased-prototype build;
source comparison proves the declaration correction. Neither is misreported as
a reproduced runtime truncation. Existing READ_POINTERS differences predate T60.
Post-commit VGA/BOP/package checks pass 3/3 on x64 (7.01 s) and x86 (7.53 s).
Full suites and hashes are recorded above; worktree was clean after delivery.
S5 meets its bounded exit criteria and closes. T60 remains open for S6/S7.
