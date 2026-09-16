# MVDM reference-guided correctness repairs

## Owner request and admission

Original request: “观察ntvdmx64项目对mvdm做的补丁 跟本项目的代码进行核对看是否有可修复的问题”.
After the comparison and FPU discussion: “好的，请你将以上建议引入的修复内容加入一个T任务的proposal队列，然后准入开始执行。”
Additional hard boundary: “预计本项目不对 lib和common进行任何改动；如果需要改动，必须要我批准”.

This candidate was selected ahead of the three existing candidates and
immediately admitted as M9 T60; CURRENT owns the active S. Queue retains the
other candidates in their original order. SoftPC baseline is 549ed44. The
read-only NTVDMx64 reference is 84a13d2 in the owner-provided checkout.
Its common/minnt patches are research leads, not imported payloads or build
dependencies. No external code, notices or new licensing grant is imported.
Repairs are narrow changes to existing project code, proved by project tests.

## Boundary and approach

Preserve the original CPU/controller/renderer ownership. These are explicitly
admitted machine-correctness repairs, not standalone-only branches or general
permission to rewrite MVDM. Each stage records its source-mirror divergence,
reason and focused proof here. No second emulator, renderer, dispatch route,
source generation or product policy is introduced.

src/lib, src/common, test/lib, test/common and their manifests are protected:
zero task diff unless the owner separately approves a specific change. Tests
belong to the product test/unit boundary. App/VM/Compat changes require a
concrete necessity within the active stage; no opportunistic cleanup.
Preserve INI, guest media, ROMs and existing UI behavior.

## Finite ledger and serial stages

| Stage | Scope / retained owner | Proof and exit |
| --- | --- | --- |
| S1 | Rejected PIC acknowledgement must not become vector 65535; retain original CPU interrupt dispatch. | Reproduce rejection with original PIC/CPU; rejected dispatch preserves CS:IP, stack and flags; valid IRQ still delivered. Both widths and full suites. |
| S2 | EGA/VGA painter bounds, zero/negative sizes and coordinate units. | Boundary/guard-buffer cases for the existing selected painters; empty rectangles do no work, valid pixels unchanged. No blanket upstream clipping copy. |
| S3 | Preserve low address bits in EGA dirty updates. | Partial versus full rendering equality for offsets modulo four, split and wrapped display. No global temporary alignment owner. |
| S4 | Horizontal pel-panning state, invalidation and rendering. | Register-only changes update pixels; zero pan unchanged; split-region semantics verified before closure. |
| S5 | C-VID byte/word writer declaration consistency. | Check actual generated-rule calling conventions as well as tables; width/address boundary proof at both widths, no unsupported claim of an already-reproduced corruption. |
| S6 | D6/SALC versus retained BOP contract assessment. | Account for selected firmware and BOP FE before proposing a change. Do not replace decoding automatically; present conflicting semantics for owner decision if needed. |
| S7 | FPU accuracy assessment beyond the repaired T59 layouts. | Compare finite/range/rounding/special-value behavior using the existing executor. Distinguish confirmed faults from double-precision design limits. No zfrsrvdx86 import, new backend or asymmetric x86/x64 implementation without owner approval. |

Allowed dispositions: verified repair, proved inapplicable/already correct,
or an explicit owner decision for a boundary-changing issue. Every ledger row
needs its own evidence; passing boot is not whole-task proof. S6/S7 assessment
cannot claim complete x87 equivalence or silently expand into backend work.

## Excluded reference patches

The NTVDM-only timer branch, native zfrsrvdx86 context swap, WOW64/WOW/DPMI,
HAXM/CVIDCNEW and bulk-copy optimization repairs are not imported. Current
SAS copies already retain per-element accesses. Printer ACK toggling and
segment-permission workarounds are not adopted without a separate reproducer
and justified hardware contract. T59's x87 layout correction remains intact.

## Delivery and verification

Each implemented S supplies both fixed EXEs, focused regression, serial x64/x86
full suites, source-boundary/documentation gates and an actual-diff review.
Use existing build presets. Owned disposable fixtures stay under build; no
new trace is needed for S1. Remove only newly owned disposable evidence.
Commit/push each complete executor P, then review the actual commit before
the governance P. Report production/test net lines and protected-path diff.
T60 remains open until all rows have explicit dispositions and a whole-task
audit; no current TODO is recreated merely for historical observations.

## S1 source-mirror disposition

base/ccpu386/c_intr.c: sentinel rejection before interrupt side effects.
This fixes the existing PIC-to-CPU contract once at the shared interrupt entry;
it does not change valid vectors, exception handling, PIC state or host policy.
All do_intrupt callers and both hardware-interrupt paths are included in the
same-class sweep. The original rejected acknowledgement is -1; conversion to
the existing IU16 ABI produces 65535, outside the valid vector range.

Same-class sweep: rg for do_intrupt, ica_intack and ICA_INTACK_REJECT in
base/ccpu386 and base/system/ica.c. Four PIC rejection exits share -1.
Both c_main hardware-acknowledgement branches and zfrsrvd's immediate NPX
dispatch use the corrected entry. The separate SFELLOW NPX vector, byte-sized
c_do_interrupt, INT/INTO and c_xcptn exception vectors remain valid and unchanged.
No duplicated caller guard, new state or external patch implementation is added.

The existing product IRQ smoke first proves normal PIT delivery, then masks
both PICs and obtains the original -1 result. Direct dispatch must preserve
CS:EIP, SP, flags and the six stack bytes. A stale host hardware request is
also exercised through the original instruction loop. The unmodified CPU
failed the new CS:EIP assertion; the corrected CPU passes on x64. This is a
product test, not a change to either shared test corpus.

## S1 executor evidence

Both tests-x64/tests-x86 builds refresh the fixed packages. Serial full suites
pass 97/97 on x64 (81.46 s) and x86 (69.38 s), including the new IRQ proof,
valid PIT delivery, restart/BOP/x87 tests and shared-corpus verification.
Documentation governance and diff whitespace checks pass. No manual product
test or protected-mode-specific rejection probe is claimed.

Against 549ed44, git diff --numstat -- src test reports production +4/-0
(c_intr.c), tests +43/-0 (irq_smoke.c), combined net +47. No new production
object, state, API or test target. The four protected shared roots have zero
diff; INI, guest media, ROMs, App, VM and Compat remain unchanged.

Fixed x86 executable SHA256:
D8C0BEE496EAD154BDECA06288D65616CF0A62A298978D2571B8057A9EDC12C8.
Fixed x64 executable SHA256:
6043C1F27A0B4F0CD013D86342E47667F46B9355E9C2B8AE4CA9EA1924803967.
Executor P1 fa27786 passed [actual-commit review](M9-T60-S1-pic-rejection.md).
S1 is closed; T60's remaining stages are not closed.
