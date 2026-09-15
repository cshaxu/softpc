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
