# Existing FPU accuracy assessment

T60 S7 follows the [admitted plan](../history/M9-T60-reference-repairs-proposal.md).
The owner requires retained compatibility and minimum necessary OpenNT diff;
reference behavior is not permission to replace existing contracts.

Assess the selected fpu.c/zfrsrvd.c executor and existing x87 layout tests.
Finite coverage: finite arithmetic, rounding modes and integer conversion,
range/extended precision, zero/infinity/NaN, and the existing save/load layouts.
Separate concrete reproducible faults from the existing double-precision
representation limits. Use product-owned bounded probes and selected-build
evidence; do not claim exhaustive x87 conformance from a small matrix.

No zfrsrvdx86 import, new backend, firmware/media change, asymmetric x86/x64
implementation or Lib/Common/shared-test change. External NTVDMx64 and OpenNT
remain read-only comparison material. Do not implement a compatibility-breaking
substitution or request another exception to the owner's rejection.

For a justified narrow fix: record exact original diff, focused regression,
both fixed EXEs, serial full suites, commit/push and actual-commit review.
For retained behavior: record the tested boundary and reason; do not relabel
a known representation limit as repaired. Whole T60 closure separately checks
S1-S7 and all added original-source divergence.

## Initial coverage audit

CMake selects fpu.c plus zfrsrvd.c at both widths. host/inc/cfpu_def.h defines
FPH as double, with separate FP80 layout/tag storage; this is not evidence of
full extended-precision arithmetic. The existing x87_layout_smoke checks byte
layouts, not instruction behavior.

Product command_provider_smoke.c already has x87_values: ten real-executor
load/store cases (signed single, double, extended, integer and BCD) and four
exact finite arithmetic cases using 6 and 2. Reuse this execution harness;
do not create another CPU or change Common to assess FPU behavior.

The next bounded matrix must cover all four rounding modes on positive and
negative ties, integer conversion boundaries, signed zero/infinity/NaN and
retained extended load/store versus arithmetic precision. Existing passing
tests do not cover those claims. npx_rint implements explicit control-word
rounding; FIST has separate 16/32/64-bit paths, so all need independent results.
No defect is declared from suspicious casts alone, and no backend replacement
is admitted. This audit is incomplete pending those probes.

## Rounding probe and bounded repair in progress

The real instruction matrix reproduced FISTP m64 nearest-even conversion of
3.5 returning integer indefinite instead of 4. The M64I branch converted the
unrounded input, then compared it to npx_rint and classified the discrepancy
as overflow. M16I/M32I already round first. The local correction passes a rounded
value through the existing converter; no alternate FPU or conversion path.

That first correction exposed the converter's double-to-FP64 pointer aliasing:
the optimized old large-integer test failed, and adding diagnostic output
changed the result. Copying the representation to FPHOST with memcpy removes
the invalid alias. Diagnostics were removed. Both widths passed the initial
48-case matrix and prior large-integer probes. The production diff so far is
fpu.c +10/-5; it preserves the original algorithm and overflow owner.

The matrix now includes half/quarter values around zero (96 cases) plus five
double load/store cases for signed zero, signed infinity and quiet NaN. x64
passed on rerun. Its first run failed before x87_values at an existing debugger
expected-IP assertion (line 189), so that failed run is retained as a separate
verification concern, not counted as a clean pass or attributed to FPU without
evidence. Range/precision assessment and full dual-width proof remain pending.

## Bounded matrix coverage

The final integer matrix has 19 inputs x four rounding modes x three widths
(228 cases). It checks stored bytes and FNSTSW invalid-operation status,
including signed 16/32-bit limits, +/-2^63 and 2^64. Seventeen format roundtrips
include signed zero/infinity, quiet NaN, extended 1+2^-63 and 2^1024. Four exact
arithmetic results remain, plus (2^53+1)-2^53 before the final store: it returns
zero, demonstrating the retained double intermediate precision limit rather
than merely double store rounding. Raw extended preservation does not imply
extended arithmetic. x64 focused proof passes; both full builds succeeded.

CVTFPHI64 callers were enumerated: integer helper arithmetic, BCD conversion,
FIST and remainder paths share its bit decoding. The change there only replaces
pointer punning with a representation copy; shifts and caller policy remain.
Only FIST M64I now rounds before conversion, matching its M16I/M32I neighbors.
No general host-FP exception, transcendental or complete x87 conformance claim
is made. Those would exceed this finite accuracy assessment and require a
different backend scope. Protected shared roots retain zero T60 diff.

Current production diff +10/-5 in fpu.c; product test +90/-7, combined net +88.
Full serial suites passed: x64 97/97 (98.60 s), x86 97/97 (75.64 s).
Afterwards command-provider repeated five times at each width, all passing
(x64 2.79 s, x86 2.88 s). This does not erase the earlier expected-IP failure
or prove that unrelated debugger timing cannot fail again; no shared-code fix
is claimed. The FIST failure was independently reproduced and corrected.

Both fixed EXEs were rebuilt. SHA256:

- x86: 4795C30678FCB092E728179D828B9A5A3B737A16E04AB160D19153A3C28874C9
- x64: 02A828C3CD9D99EDE9A1186C9B1B9F1A0B32AB1A36C8B9DD9B9BE8828437CB14

This is a locally reproduced repair during reference-guided assessment, not
an imported NTVDMx64 patch. Its alternative native FPU backend remains excluded.
No D6 decoding, media, INI or protected shared corpus changed. Unattributed
trailing-blank-line edits in 18 comms/video mirror files remain outside this
delivery; they are not necessary FPU changes and are not staged or discarded.
Executor evidence is complete; actual-commit review is required for closure.

## Coordinator actual-commit review

Reviewed pushed 64d26a0, the two FPU hunks, real instruction probes and both
package hashes. Original conversion shifts, overflow handling, instruction
dispatch and backend remain; no new production state or alternate path exists.
The FPU file versus original OpenNT is +12/-6 (line endings ignored), including
pre-T60 differences; S7 itself is +10/-5. Product test is +90/-7.
Post-commit command-provider/layout/package checks pass 3/3 at both widths
(x64 5.51 s, x86 5.64 s). Full results and bounded limitations are above.
S7 meets its assessment/repair exit criteria and closes.

Owner subsequently requested "请一起提交": the 18 previously unattributed
comms/video files are included in the closure commit. Their entire difference
is deletion of 21 trailing blank lines, confirmed by git diff
--ignore-blank-lines --exit-code. They are owner-approved whitespace changes,
not necessary correctness fixes or evidence of smaller original-source diff.
Disposable types-layout test fixtures were removed; build trees, INI and media
were preserved. T60 remains open for owner acceptance of the delivered builds.
