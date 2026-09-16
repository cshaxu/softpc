# DOS DEBUG memory-command semantics

## Admission

M9 T59 S22 continues from f7110d9. Original request: 准入一个S任务用于在当前代码风格和结构的基础上，修复和原始DOS DEBUG工具的语义差距；X系列32位命令继续保持现状，除了需要仿照16位DEBUG命令语义修复的改动除外。请开始，完成后编译测试提交推送。
Owner clarification: 批准。交互式E和DOS环境行为不需要改变。

The read-only MS-DOS 6.0 cmd/debug research establishes neutral requirements;
no external code is copied, translated, built or redistributed. Retain existing
Common command functions, Lib Types vocabulary, paused machine access and
sequential output. Do not change interactive E's line input, host-file N/L/W,
machine execution plans, DOS process/EMS services or X command namespace.
This is memory-command compatibility, not complete DOS DEBUG emulation.

## Finite ledger and implementation

| Member | Required proof |
| --- | --- |
| Parsing | Preserve quoted case, spaces and doubled quotes; reject malformed addresses/bytes before access. One list parser shared by E/F/S and XE/XF/XS. |
| Ranges | C/D/F/M/S/U share end-inclusive or L-count parsing; default D=128, U=32, DS ranges=128; reject reversed/overflowing explicit ranges, permit full segment. |
| E/F and XE/XF | Parse entire byte list before writes; malformed tails make no changes. Retain per-byte machine errors, no rollback transaction. |
| C/M | Bound source range, compare/copy including last byte; directional overlap move and original destination offset wrapping. |
| S/XS | Complete patterns only, include overlapping matches, finite full-segment search. Short range yields no match rather than replicating original arithmetic underflow. |
| D/U | Correct segment defaults, segment-end progression and no invented 1 MiB clamp; retain machine access policy. U emits whole instructions and terminates failed decode. |
| Retained boundary | Interactive E/XE, execution plans, host files and extended counts remain unchanged; no Lib/App/VM/Compat/MVDM or public ABI changes. |
| Delivery | Focused fake-memory tests, existing debugger tests, x86/x64 full build/tests, manifests/docs gates, P1 push then actual-commit review and P2 push. |

Stop for a required public input/machine ABI change rather than creating a
second command path. Tests use bounded copied fake memory, no new media or trace.
Count production/test C/H and test registration against f7110d9; record all
similar parser/range hits and dispositions. T59 remains open for owner testing.

## Implementation and similar-issue sweep

The existing command.c keeps one dispatch and the same command functions.
scanrange replaces six separate real-range interpretations; scanlist replaces
six byte-list loops. The existing tokenizer now preserves quoted contents and
doubled quotes rather than lowercasing/splitting literal bytes. Address parsing
rejects missing or repeated colon fields rather than silently changing meaning.
No library, public interface, machine adapter, thread or input owner changes.

C/D/F/M/S/U use a widened count for 64 KiB, validate explicit range before
access, and accept L with attached or separated length. L0 at offset zero
means 64 KiB; a nonzero start cannot fit that range and is rejected. This does
not reproduce the original counter-underflow/wrap anomalies. S visits every
candidate once including overlapping matches; a short range returns no matches.
E/F/XE/XF validate even an unused invalid pattern tail before their first write;
hardware access failure still terminates through the existing status path,
without rolling back successful writes. Original destination offset wrap is
retained for C/M and E's sequential writes. Normal overlap uses directional copy.

Real U defaults to CS for every explicit range and decodes as 16-bit independent
of the current code-segment attribute. Its existing R/trace display helper uses
the same real read. A split read at the segment end provides the decoder's fixed
15-byte buffer with wrapped real offsets, allowing a whole instruction to finish.
XU/XR retain their linear/current-mode decoder. D/U no longer clamp at 1 MiB;
the machine adapter still applies its existing memory policy. Failed decode stops
without a busy loop; this task does not replace xasm32's instruction table with
the historical DOS decoder or add P/DOS execution services.

Sweep: rg -n 'scanrange|scanlist|scannubit8|scannubit16|STD_STRTOK'
src/common/debug/command.c. All real range and six list consumers are covered.
Scalar register/port/arithmetic/trace arguments retain their distinct numeric
parsers; E/XE interactive scalar responses remain line-based. XA/XC/XD/XM/XU
count/address boundaries from S21 are unchanged. N/L/W remain host-file commands;
no guest PSP, command-tail, disk-sector or EMS service is introduced. The owner
approved these environment and interactive exceptions, not a claim of complete
DOS DEBUG replacement. No external source implementation was imported.

Read-only research identity: the local MS-DOS 6.0 README and cmd/debug files;
debcom1.asm SHA256 2A5C53AB19D26C9299771802B4D0DB35742965524AD0F6872073772C18242649;
debuasm.asm SHA256 550A49E20889F4121EB07C70F619994990FB6541F769A9488438C228DD07C1C2.
Only independently written requirements/tests and existing C code changes ship.

## Focused proof

The existing debug_linear fixture also covers the real counterparts, using the
public submit path over bounded copied fake memory. It verifies invalid range
and address rejection, full-segment F/S/M/C, both move directions, destination
wrap, E wrap, quoted case/space/escaped quotes, E/F/XE/XF no writes on bad tails,
overlapping and short S/XS, D/U defaults and high real addresses, CS versus DS,
and whole-instruction U offset wrap. Existing XU count/overflow/decode and XM/XS
tests remain. The threaded fixture now expects real rather than linear reads
for the 16-bit R instruction display; XR assertions remain unchanged.

The first targeted run exposed only that obsolete R routing assertion; it was
updated to explicitly require one real read and zero linear reads. Final focused
debug_output/debug_linear/common_machine pass at both widths. Changed command.c
and the memory fixture pass C17 -Wall -Wextra -Wpedantic -Werror syntax checking
with both GNU compilers. No manual DOS binary comparison or Linux runtime test
is claimed.

## Executor delivery

Both tests-x64/tests-x86 builds complete. Full test-x64 passes 97/97 in 52.63 s;
test-x86 passes 97/97 in 54.09 s, including compact Console on the first run at
both widths. The earlier S21 intermittent fixture observation remains separate
TODO debt; these passes do not establish its cause or repair. Common/Lib source
and test manifests, corpus/DAG gates, documentation gate and diff whitespace pass.
Generated types-layout fixture directories were removed after test completion;
existing build trees/logs, user INI and guest media remain untouched.

Reproducible git diff --numstat f7110d9 -- src/common/debug/command.c
test/common/common_machine_smoke.c test/common/debug_linear_smoke.c:
production +223/-354 (net -131); tests +67/-7 (net +60); combined net -71.
No new test target, production file or object was introduced. Manifests,
documentation and binaries are excluded from these counts.

Fixed softpc32.exe: 3,487,581 bytes, SHA256
7D2D65CB7E8AB1E0E7CAC48AB05C289C1350C0321ED9C70D0D291A74FA55E8A2.
Fixed softpc64.exe: 2,847,702 bytes, SHA256
45CAB2236EB33F8FA39EF2482E6066C4FC0E4F7E931122B08332F97DFD35AEE4.
All ledger members have focused proof or the approved retained boundary.
P1 is ready for actual-commit coordinator review; T59 stays open.

## Coordinator actual-commit review

Reviewed pushed b332b94 against f7110d9, the original request and the owner's
explicit E/environment exclusions. Actual production changes are confined to
command.c; shared private parsers replace duplicated interpretation rather
than creating another command engine. Byte-list capacity is bounded by the
existing 255-byte input limit; expanded strings cannot exceed their input.
Range arithmetic uses widened counts before any 16-bit offset conversion.
Short-pattern subtraction and complete-segment termination are guarded.
Parsing errors survive dispatch rather than being cleared by exec.

Actual tests prove both updated real semantics and retained extended behavior;
R's changed real-read assertion does not weaken XR's linear-read assertion.
Repeated focused debugger/threaded/corpus/manifest tests pass 6/6 at each width
after the pushed commit. Full-suite evidence remains first-run 97/97 at both
widths. P1 left a clean worktree. This P2 updates review/status only; S22 is
delivered for owner testing, without claiming DOS environment, interactive E,
historical decoder instruction-table or complete command-set equivalence.
