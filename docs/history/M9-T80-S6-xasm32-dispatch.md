# M9 T80 S6: immutable xasm32 dispatch

## Admission and scope

Owner: “准入S6”; baseline `6ebd1397`, clean worktree. S5 accepted for
continuation, without claiming a separate manual-test result. S6 is delivered
for testing; T80 remains open, S7 is not admitted. Existing architecture,
coding, execution and documentation skills and repository rules apply.

Only dasm32.c production changes: two invariant 256-entry tables become
file-local const arrays. The 0F table has one early forward declaration;
initialized definitions follow the handlers. Two context fields, their two
accessor macros and per-call initialization disappear. Parsing remains
call-local; no public API, mutable cache or fallback dispatch branch added.

## Finite sweep and equivalence

Frozen scope: xasm32 context dispatch arrays, initialization and readers.
`rg -n 'dtable|initialized' src/x86/xasm32` and context inspection finds:

- Two arrays/one initialization block: replaced by const definitions.
- Initialized flag/two macros: deleted.
- Two readers, INS_0F and dasm32_execute: unchanged.
- Assembler and other context buffers: no similar per-call fixed dispatch
  table; buffers retained because they contain parse state.

Extract original (table, opcode, handler) assignments and new designated
entries: 512/512 identical in order, including UndefinedOpcode. Remove only
these structures: all remaining source tokens are identical (blank-line
placement differs). No decoder/formatting body changed.

Original linked libraries produce digest 70F241BE on both widths:
256 opcodes x two maps x three prefixes (none/66/67) x eight addressing
bytes x two alternating default modes = 24,576 cases. Hash status success,
instruction/text lengths and successful output bytes, not native-width or
pointer representations. New libraries match. Existing guarded bounds,
prefix and exhaustive ModRM/SIB tests pass. This is not exhaustive ISA proof.

## Accounting and footprint

`git diff --numstat 6ebd1397` over the two C files:
production dasm32.c +520/-521 (net -1); smoke +42/-0; total +562/-521
(net +41). Manifests, documents and EXEs separate. Matches the estimate:
roughly 520 each way for production, 40--70 test additions. Most diff is
512 assignment-to-initializer edits, not new behavior.

GCC -O3 probes (x86 GNU16.2, x64 GNU16.1), bytes:

| Measurement | x86 before/after | x64 before/after |
| --- | ---: | ---: |
| sizeof(context) | 5152 / 3104 | 7216 / 3112 |
| dasm32 stack-usage | 5212 / 3148 | 7328 / 3216 |
| object .text | 77328 / 72560 | 73168 / 67264 |
| object .rdata | 3220 / 4640 | 3872 / 6688 |

Read-only tables occupy 2048/4096 pointer bytes. Object section deltas also
reflect former initialization constants/alignment. Stack is this function's
report, not complete call-stack depth. No speed benchmark claim.

## Verification and artifacts

Strict C11 -Wall -Wextra -Wpedantic -Werror builds pass on both widths.
Background presets: x64 111/111 (177.48s), x86 111/111 (169.15s).
Standalone `cmake -S test/x86` builds/tests: x64 10/10 (7.77s),
x86 10/10 (9.86s), independent of product targets.
Six manifests and dependency gates pass within product tests.
Final documentation governance/diff checks pass. Five desktop cases excluded
per product width; no Linux runtime or new downstream acceptance claim.
S5 retains prior four/six-directory copy evidence; no new copy claim here.
Task-owned build/t80-s6 probes/builds removed after evidence capture.
Lib/Common/Core, INI and media unchanged.

| Package | Bytes | SHA256 |
| --- | ---: | --- |
| softpc32.exe | 3692151 | E40E65DC4B3314E92A22D466D87EEE8A539C0F9847FCA0BE83C8469714065EEF |
| softpc64.exe | 3079065 | 9AEF41D012B9A3C74F64A8A68BED41FDF10F7C7A5D7386E29E3D3FA8E51F1EB9 |

EXEs decrease 3537/1490 bytes relative to S5.

## Actual-change review

Executor `5d0515fd` is pushed. The same session switched to coordinator
and reviewed `git diff 6ebd1397..5d0515fd`: eleven paths, one production C,
one test C, two manifests, five governance documents and two package EXEs.
Re-extracted both committed versions: 512 distinct mapping keys, 512 equal
handlers; all other production tokens equal after removing the admitted
structures. Test digest was captured before relocation, not invented from
new output. Counts, strict builds, complete background/standalone results
and package hashes match the recorded evidence. Public headers, Lib/Common/
Core, INI and media untouched; temporary outputs removed. Final documentation
gate passes. No remaining implementation gap found in the frozen S6 scope.
Owner S6 testing remains pending; S6/T80 are not closed and S7 is not admitted.
