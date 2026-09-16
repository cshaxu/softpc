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
