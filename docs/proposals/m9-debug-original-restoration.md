# Original debugger restoration

Owner request: 请你对照原始代码，按照原始代码的原始风格恢复所有功能；不要自己发明新的结构和风格

T59 S7 baseline: SoftPC 0299b93. Reference is read-only NXVM 9bd08dd8
src/nxvm-baseline/debug.c and device/vcpu.c, plus extraction 57c1d6d7.
Original source is comparison material, not a build dependency.

## Design and convergence ledger

Restore lost behavior in existing functions using existing machine access and
copied result output. Preserve original formatting, register order and flags.
Do not restore global CPU access, polling, native Console ownership or supersede
the approved paused executor/continuation contract. No new public API or layer.

Coverage universe is XA/XC/XD/XE/XF/XG/XM/XR/XS/XT/XU/XW,
XREG/XSREG/XCREG and extended help. Each receives comparison, disposition and
test evidence. Completion requires all entries accounted for, dual-width full
tests/packages, manifests, executor push and separate coordinator review.

- XR/XREG: extraction 57c1d6d7 replaced original devicePrintCpuReg with
  rprintregs, losing 32-bit registers/flags and adding real-mode disassembly.
  Restore the original print sequence in xrprintreg, then its one linear decode.
  XT/XG/watch completion already use that function and share the repair.
- XA/XC/XD/XE/XF/XM/XS/XU/XW: compare original algorithms and current adapter;
  do not relabel inherited edge defects as extraction regressions.
- XSREG/XCREG: preserve existing effective snapshot adapter and architecturally
  unavailable CR1/CR4 handling; no invented CPU fields.
- Existing input continuations and execution completion replace old blocking
  reads/polling deliberately. They must remain single-path and be tested.

Verification adds exact nonzero high-register/flags output and one linear
disassembly, plus original x-command coverage through existing test fixtures.
No changes to configuration, media or MVDM are admitted. Owner additionally
confirms the existing output path must use Lib Types' function vocabulary:
“对啊！就是那样。” Add only the header alias lib_c_snprintf in types/file.h;
route every debugger snprintf site through it, including existing error and
append formatting. No new variadic wrapper or runtime implementation is needed.

Owner further requests: “类似问题一并审计解决”. Extend the naming-only sweep to
all five Common components, including xasm32 and retained STD_* macros. Existing
memory/text/allocation wrappers are reused; only missing one-to-one C aliases
are added to Types. No algorithm, parser, ownership, function signature or
return-value semantics change in this mechanical portion. Add a Common corpus
gate and negative fixture to prevent raw CRT calls/macro aliases returning.

## Comparison dispositions

Read original debug.c's extended command block and device/vcpu.c's print
functions at 9bd08dd8, the pre-57c1d6d7 Core callback chain and the extraction
patch. Existing T56 S7 provenance remains unchanged; this restores its lost
print contract, not a new dependency or licensing assertion.

| Commands | Disposition and proof |
| --- | --- |
| XR, XREG | Restore eight GPRs, EIP/EFL, original flag spelling/order and one linear decode. Shared fake checks exact high-bit values, both flag polarities, compact/spaced aliases and all ten 32-bit register edit continuations. |
| XSREG, XCREG | Existing snapshot-backed output is connected. Restore read-only data attribute `rw` (original), not `Rw`. Keep approved LDTR selector/base/limit-only capability and unavailable CR1/CR4. Product matrix covers both displays; shared fixture checks attribute spelling. |
| XT, XG | Original trace threshold (256) and repeated breakpoint-count loop are already represented by existing completion continuations. Both now use restored XR output. Shared fake and real CPU provider tests cover completion; product test repeats T/XT and XG hits. No polling restored. |
| XA, XU | Existing Common xasm32 byte/text boundary replaces original assembler calls; code size and address progression retained. Product matrix plus both xasm32 suites cover roundtrip. Original XU already narrows count to 8 bits and has different no-argument/address-only defaults; unchanged, recorded as inherited debt. |
| XC, XD, XE, XF, XM, XS | Original per-byte algorithms, count/address parsing and adapter paths remain. Existing product matrix executes edit/fill/copy/compare/search/dump against actual machine memory. XS legacy addrparse and XM forward overlap behavior are present in 9bd08dd8; not invented by Common and not silently redefined here. |
| XW, X? | Original identifier dispatch/help and watch set/clear/display remain; real CPU watch tests cover read/write/execute and watch-completion output. Help's XG count wording is inherited, not evidence of instruction-limit semantics. |

Common's bounded copied output and the approved command continuations are
retained integration contracts, not a claim of byte-for-byte terminal behavior
for unbounded commands. Silent long-output truncation and inherited invalid-input
edge cases are explicitly recorded in TODO; this S does not claim to repair
every defect in the original debugger.

CRT sweep: all Common C/H, including retained STD_* definitions, now use
Types' existing memory/text/allocation wrappers. Only snprintf/strcmp/strchr/
strtok need header-only aliases. xasm32 changes are token substitutions, not
parser edits. The corpus gate checks the audited CRT vocabulary in calls and
macro definitions, with both forms covered by negative fixtures.

## Verification discoveries

The first full x64 run exposed two fixture problems, not accepted passes:
composition counted raw memset and therefore missed the renamed production
call; keyboard adapter equivalence compared whole structs containing padding
and inactive union bytes. Change the former interception to lib_memory_set and
all six latter comparisons to active key/text fields, retaining count/type/source
and every meaningful payload assertion. No input implementation changes result.
The first x86 run overlapped these fixture edits and failed manifest/composition
checks; it is not final evidence. Rebuild and rerun both complete suites after
all edits/manifests are stable. The direct x86 incremental invocation lacked
the preset's compiler environment; only the successful preset rebuild counts.

Mechanical proof: normalize 0299b93's nine non-debug changed C/H files by the
explicit CRT identifier map and removed unused includes; each equals the final
file exactly. Thus assembly tables, parsing, queue ownership and control flow
are unchanged by vocabulary adoption. Source review finds no edits in App, VM,
Compat, MVDM or the user's INI.

## Executor delivery

Final stable-tree verification on 2026-09-14:

- Both package/test presets built successfully. Full test-x64: 89/89,
  90.46 seconds; full test-x86: 89/89, 85.68 seconds.
- Composition and keyboard lifetime each pass 20 repetitions at both widths.
  Exact XR, all ten edit continuations, flags, T/XT/XG, real machine command
  matrix, shared xasm32 and all four corpus manifests pass.
- Common CRT gate rejects both direct calls and legacy macro aliases for all
  14 audited function names. Documentation governance and diff whitespace pass.
- The initial full rebuild reports existing preserved MVDM endif-label warnings
  and Compat platform.c's callback-pointer warning; none originate in changed
  Common/Types code. No claim of a warning-free full baseline is made.
- Accounting against 0299b93 via git diff --numstat -- src test, excluding
  manifests: 14 production C/H/CMake paths +388/-364 (net +24); five test
  C/CMake paths +147/-28 (net +119). Nine production files are proven purely
  mechanical; no new runtime state, ownership or output path.
- x86 EXE: 3,507,176 bytes, SHA256
  A1E484FD3BDE566AD2387AF9E1A0E8206D0F6E5536BC398DE390EA3E3495A284.
  x64 EXE: 2,854,084 bytes, SHA256
  DCA0D619CA7253219EDD309CBC8FF62893BFA82EDBF6935DF187F470EE0D4CE0.

Original request reread: preserve source style and restore lost functions,
then route CRT through Types and audit similar cases. These are implemented
and verified within the ledger, with original edge defects explicitly outside
restoration. Owner manual testing is still pending; T59 is not closed.
Owned bounded build/test logs are removed after recording this evidence.

## Coordinator review

After executor push 8a851e9, reviewed its actual 0299b93..8a851e9 changed-path
set and source/test patch against the three owner instructions. The original
print sequence is restored in the existing function, while Types supplies only
definitions. Shared and real-machine tests assert high register bits, not the
ambiguous AX substring alone. Other Common production changes match the recorded
token-only proof. The fixture corrections keep semantic field checks and do not
change keyboard production code. Four manifests verify again after commit;
main and origin/main match. P2 records this review only, not owner acceptance
or T59 closure. Both fixed EXEs are ready for manual testing.

## Owner-admitted CLI continuation and output restoration

Original requests: “没错，我要求完全100%恢复原版用户体验”;
“批准调整, 开始执行，完成后核对源代码进行语义审计，编译测试提交推送。”

Baseline for this repair is 0faaf45. The S7 XA disposition above was too broad:
single-line roundtrip did not prove interactive address advancement. The
following expanded finite ledger replaces that claim, not the preserved source.

1. Parse/dispatch: empty and whitespace-only Enter do not repeat; compact and
   spaced syntax, help, invalid command and q retain original handling.
2. All seven read continuations (A/E/R/V/XA/XE/XR): use the original printed
   suffix as the next prompt, consume a supplied line without reprinting its
   prefix, preserve errors/empty-line exit and advance assembly addresses.
3. D/U/XD/XU defaults and G/T/XG/XT completions: compare original progression
   and output order, retaining the sole paused executor and approved hotkeys.
4. Every PRINTF site: retain original command-level formatting and Types
   vocabulary. Debug owns a growable output buffer, exposed as a borrowed
   string until the next producing call/open/destroy. Session accepts a borrowed
   additional text span and writes it through the existing monitor transaction,
   normalizing LF to CRLF in bounded chunks. No new Console owner or input loop.
   Allocation/formatting failure is explicit, not successful truncation.
5. App forwards the borrowed text without copying into its fixed command result;
   completion text is consumed before another debug-producing call. New commands
   invalidate an unconsumed completion. Closing q carries no borrowed output.

Focused proof uses shared fake memory/registers for transcripts, compact syntax,
multiple assembly lines, register/byte continuation and >8192 output; product
tests verify the same binding against the real CPU. Session tests prove long
text, CRLF boundaries, one cancellation, no extra prompt and output failure.
Full dual-width tests and package refresh remain required. Original algorithmic
edge defects remain original behavior, not a license to redesign commands.

## P3 source-semantic audit

Compared the original 9bd08dd8 debug.c command bodies and vcpu register aliases
with the final Common implementation, using 57c1d6d7 to identify extraction
changes. Parsing an empty line returns without repeating a command. All seven
read continuations now retain original prompts and reentry; XA advances its
existing address across separate submissions. R word writes, L length registers
and G/T IP assignments preserve upper register halves, matching original aliases.
Plain G prints registers on completion; T/XT preserve inter-step blank lines and
zero counts do not run; XG zero hits likewise does not run. Both disassemblers
retain the final bytes of a 15-byte instruction. Extended print routines retain
their original sequential PRINTF style through Types definitions.

The H/help/Xhelp, D/XD, compare/fill/move/search and U/XU command bodies were
reviewed against original formatting, defaults and loops. No new command
algorithm or parser was substituted. Exact transcript tests cover continuation
prompts, memory edits, defaults, disassembly and a 256-row dump exceeding the old
8192-byte output ceiling. Fault tests cover initial allocation, later growth and
native output failure. Session owns no new output object: it consumes the borrowed
debug string synchronously through its existing monitor transaction.

This is original CLI restoration within the approved machine/Console boundary,
not restoration of original global CPU access or a blocking input loop. Existing
prevalidation, guarded decode/wrap behavior and the corrected 16-bit W offset
(the original cast wrapped at eight bits) are retained; known original XS/XM/XU
edge debts remain in TODO. These are not claimed byte-for-byte invalid-input
equivalence. VM, Compat, MVDM, INI and media are unchanged.

The first full test pass exposed two shipping-package assertions waiting for
the removed migration-only `Debugger:` banner. They now inspect the actual
current `-` prompt at the Console cursor, rather than accepting an old screen
substring. No production workaround restores the unwanted banner. Final rebuilt
full-suite results below supersede those initial runs.

## P3 final delivery evidence

Both tests-x64/tests-x86 builds succeeded. Final stable-source test-x64 passed
90/90 in 124.92 seconds; test-x86 passed 90/90 in 107.60 seconds, including
shipping Console integration, shared transcript tests, failure injection,
component/CRT negative gates and all four manifests. Documentation governance
and diff whitespace checks pass. No temporary research files were created;
build-owned test logs remain in their normal build trees.

Against 0faaf45, excluding manifests/docs/binaries: six production C/H paths
add 186/delete 101 lines (net +85); six test C/CMake paths including the new
debug_output test add 269/delete 25 (net +244). The added output owner is the
existing debug object, not a new component/thread. There is one monitor write
transaction and no second command/input path.

- softpc32.exe: 3,490,068 bytes; SHA256
  079EC8210F54C14910AB18A347BD5C69BDEF5C033CBA94F71ACADDCC53E3CA8E.
- softpc64.exe: 2,848,783 bytes; SHA256
  52EC0AF81272D64F720A10A1B3228221E5FA900DAD5E85EB24B2F25ABE970C2D.

Original request reread and executor self-review complete. This delivery awaits
post-push coordinator review and owner testing, not whole-T closure.
