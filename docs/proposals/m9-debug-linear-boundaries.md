# Debug linear command boundaries

## S21 admission

Original request: 按照你的建议，准入一个S任务修复；完成后：编译测试提交推送。

M9 T59 S21 continues from clean main 2ec18a1. S20 owner testing passed;
its record is [archived](../history/M9-T59-S20-hlt-audio-failure.md).
Read-only reference: NXVM 9bd08dd8 src/nxvm-baseline/debug.c.

Retain command syntax, original sequential formatting and Common machine's
paused-access boundary. Repair XM with original M's directional copy, retain
32-bit XU instruction counts, remove XS's real-address parse, and require the
complete search pattern inside its byte range. Check related linear ranges
before access; never wrap an address into low memory. Keep E/F/XE/XF incremental
byte validation/write behavior, generic parser permissiveness and 16-bit
command semantics. No new parser, transaction, public ABI or temporary bulk
copy buffer. Only Common Debug, shared tests/manifests, UI contract, task
records and fixed EXEs may change; Lib/VM/Compat/MVDM/INI/media are unchanged.

## Finite convergence ledger

| Member | Required disposition and proof |
| --- | --- |
| XM | Forward/backward overlap, same address and zero count; bytewise injected machine access retained. |
| XU | Counts above 255 retained, zero count, decode failure and address-end stop; original defaults unchanged. |
| XS | Linear-only parse, full-pattern bounded search, zero/short range, overlapping matches. |
| Related linear arithmetic | Sweep XA/XC/XD/XE/XF/XM/XS/XU accesses/progression; reject overflowing byte ranges before access, stop progression at address end; test upper boundary through copied fake memory. |
| Retained semantics | E/F/XE/XF partial writes on invalid tail and existing command UI remain; no generic parser redesign. |
| Delivery | Dual-width builds/full tests, shared manifest/corpus/docs gates, counted production/test diff and executor/reviewer pushed commits. |

Use existing build trees, bounded deterministic fixtures with no media input or
trace. Clean owned generated test children after verification. Count tracked
C/H and test registration against 2ec18a1 separately from docs/manifests/EXEs.
Stop if a machine ABI or generic parser redesign is required. Completion means
each ledger member has code/test evidence or an explicit retained-semantics
decision; it does not claim all original debugger defects fixed.

## Implementation and sweep

One private subtraction-based xcheckrange checks counted byte ranges before
memory access. XM keeps one bytewise loop with a direction-selected offset;
it neither allocates a copy buffer nor bypasses the machine adapter. XU's count
is 32-bit end to end. XS removes the unrelated real-address parse, parses its
pattern once and visits only count-pattern_length+1 candidate starts.

Sweep command: rg -n 'linear \+|xalin \+|xulin \+|lin[12] \+|read_linear|write_linear'
src/common/debug/command.c. Extended XA checks instruction write length;
XC checks both ranges; XD rejects overflow rather than clipping silently and
rounds its printed row end without arithmetic overflow; XE checks list address
extent but keeps incremental value validation; XF checks its counted range;
XM checks source and destination; XS checks the range and full-pattern extent;
XU reads at most the remaining address bytes and never wraps its continuation.
XA ends input at exhaustion. At exhaustion, saved addresses retain the last
representable command position, not zero. Register/code-base-derived addresses
retain machine-provided 32-bit semantics; watch/break commands take single
addresses and have no counted memory loop. Real-mode commands are unchanged.

The xasm32 decoder requires a 15-byte host buffer. At address end the debug
caller zero-initializes that buffer, reads only available bytes, and rejects
any decoded instruction longer than those bytes. The first focused test caught
an attempted short-buffer call rejected by that existing contract; corrected
locally without widening the shared decoder API or accepting padded bytes.

debug_linear_smoke runs the real public command submission path over copied
fake machine access. It covers both overlap directions, non-overlap, same/zero
copy; 256-instruction XU and preserved defaults; high linear XS addresses,
overlapping matches and short/zero/exact ranges; both XC/XM overflowing ranges,
XD/XE/XF/XS overflow before any access; final-address decode/truncation, dump,
write/search/fill/move/compare/assembly; and unchanged partial E/F/XE/XF writes.
No timing, OS, media or product adapter is needed. Existing actual-machine
and threaded debug integration remains in the full suites.

Original 16-bit S/U/M boundary behavior and permissive parsing remain explicitly
outside the approved linear fixes and are retained in TODO. The accepted
partial-write semantics are now documented, not treated as pending transactions.

## Executor verification and accounting

Both tests-x86/tests-x64 builds pass. Changed command.c and debug_linear_smoke.c
pass -std=c17 -Wall -Wextra -Wpedantic -Werror -fsyntax-only on both compilers.
Full x86 suite: 97/97, 55.11 s. First x64 suite: 96/97, 55.34 s; compact package
Console failed stage 14 (ver output absent, error=0), before any debugger use.
The unchanged executable passed three isolated repeats (14.43 s); a second
complete x64 suite passed 97/97 in 50.08 s. Cause remains unconfirmed in TODO;
reruns are not a claim that this separate integration issue is repaired.
All four manifests, Common corpus, documentation and whitespace gates pass.
No Linux runtime or owner manual acceptance is claimed.

Against 2ec18a1, git diff --numstat for tracked C/H: production command.c
+41/-18, net +23; debug_linear_smoke.c +121/-0. Test CMake adds one line;
code/build total net +145. Documentation, manifests and binaries are excluded.
The sole production path is the existing Common command engine and machine
adapter. No parser, state owner, interface or forwarding layer was added.

Fixed outputs: softpc32.exe 3,485,558 bytes, SHA256
3F4491FCBEE68459F97855A901BC28DED474A7A60D65D3FA38C41D6CD76C4FE2;
softpc64.exe 2,845,177 bytes, SHA256
AB63C45F494558A3B400B114BA197676ACE00874A3131CAA44C225CE9E73EAC2.
Common source/test manifests advance to shared-t59-s21-p1. Lib source/test,
VM/Compat/MVDM and user configuration/media remain unchanged. Existing build
trees and test logs remain; generated types-layout fixtures are removed.
All six ledger members are verified or explicitly retained as designed.
Executor P1 delivery is ready for separate actual-commit coordinator review.
