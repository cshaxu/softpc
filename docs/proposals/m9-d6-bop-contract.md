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
