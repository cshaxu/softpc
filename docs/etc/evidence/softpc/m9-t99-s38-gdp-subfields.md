# T99 S38 GDP subfields and Unchained DWORD

Baseline: 0d0ef251; fixed XP SP1 a2f6d7c93aa4f11efc51147566d5961c59a8ef89.
Owner approves the two preceding audit findings. This is not a foreign patch
import, public-component revision or bulk-path activation.

## Finite coverage ledger

| Item | Existing owner and repair | Proof |
| --- | --- | --- |
| 1378/1379 | Low WORD/BYTE of native colour_comp; group with 1376 | Identity/value checks with nonuniform data; actual CHAIN2 BYTE and CHAIN4 WORD reads |
| 1382/1383 | Low WORD/BYTE of native dont_care; group with 1380 | Same fixture, independently varying compare/care masks |
| 1407 | Low BYTE of native rotate; group with 1404 | Original setter then alias read for all eight rotate amounts |
| S_2097 | Second guest WORD address changes +8 to +2 | Four planes, five offsets, independent physical bytes and complete DWORD assertions |
| Bulk DWORD fill | Original linear SAS scalar loop remains | Source/caller investigation; no activation, deletion or performance claim |

## Evidence and minimum implementation

Prior isolated x64 rule execution showed all 16 tested Unchained plane/address
pairs returned AA992211 instead of 44332211. Changing only the +8 constant to
+2 corrected all 16; the existing save and high-WORD assembly are correct and
remain unchanged. A preliminary interpretation of a partial source excerpt
as missing high-WORD storage was disproved by the complete body and execution.
Permanent coverage extends this to twenty cases including a 0x3fff boundary.

The existing GNU fixture first fails at the missing 1378 alias on unchanged
production. Prior isolated before/after probes also show CHAIN2 BYTE reads
returning 00 rather than FF, CHAIN4 WORD reads returning 0000 rather than FFFF,
and rotate field=3 while its alias reads 0. Grouping the five offsets with
existing native fields corrects those probes. Permanent tests additionally use
nonuniform 12345678/87654321 values, zero/all-one masks and rotate amounts 0-7.
They avoid the endian-blind all-zero/all-one-only oracle.

The original generated offsets identify low-order subfields, not native +2/+3
byte positions. The selected Win32 x86/x64 native field storage is little-endian.
Compatibility maps these accesses to the same native field storage; generated
consumers retain their original byte/WORD types. No duplicate storage, copy,
consumer compensation, new state or interface is added.

Source sweep across all sevid C rules found nonaligned constants within the
VGA GDP range: 1378/1379/1382/1383/1407 and 1429/1430/1431. The latter three
already map to actual byte members dither/wrmode/chain. The five former
accesses are the proven omissions. This inventory does not claim arbitrary
computed addresses or every historical generated rule has been exhaustively
validated.

Changes: production +8/-1, net +7: gdp_state.c +7/-0 and sevid000.c +1/-1.
Existing App test +63/-0; no new target, framework, public ABI or source tree.
Normalized-LF sevid000.c comparison against pinned XP, using SequenceMatcher
with autojunk=False, changes from +513/-526 to +514/-527. Only one mirror line
changes; its original CRLF, condition structure and diagnostics are retained.

## Bulk-fill contract and recommendation

The fixed XP CCPU c_sas_fillsdw implementation already loops c_sas_storedw.
That entry checks the page boundary, bios_write_double performs translation/
permissions or original ROM fallback, and phy_w32/phy_w16 perform the physical
memory-type/video dispatch. The C-VID fill_dword_ev_glue instead subtracts the
current video aperture base and invokes c_ev_write_ptr.d_fill; it does not
perform the SAS linear-memory contract itself. Both the wrapper and registered
rule remain compiled, but the selected CCPU fill path does not call them.

The bulk rule can be useful for already valid, contiguous video spans; its mere
existence does not prove that any SAS linear fill can safely be redirected.
Connecting it would require bounded spans preserving mapping, permissions,
fault/order behavior and RAM/ROM/video transitions, and a measured benefit.
Keep it for this correctness task, neither delete it as dead text nor activate
it merely to exercise an existing API. At P1 delivery the odd Copy DWORD-fill defect remained in TODO; the
owner-admitted P2 below repairs it before any consumer is enabled. No new
fast/slow path is introduced here, and no speedup is claimed.

## Verification and limits

The current fixture tests original rules and
Compat field identity, not a complete DOS/Win31/Win95 guest scenario. In
particular it does not establish a causal link to any historical screenshot,
prove all Video7 banking combinations or demonstrate a bulk-fill speedup.

Initial complete qualification: x64 background 187/187 (88.69 seconds), x86
187/187 (78.98 seconds), native x64 8/8 (24.51 seconds), native x86 8/8
(24.67 seconds). The expected unchanged-production alias assertion failure
precedes the repair and is not counted as a passing run.

Self-review found trailing whitespace after the new mirror reason comment;
only that whitespace was removed. Final cached builds and five relevant
VGA/documentation/mirror checks per architecture pass. Relinking changed the
EXE hashes; full qualification is therefore repeated on those final artifacts.
No further source changes are made during this final qualification.

Final published SHA-256:

| Artifact | SHA-256 |
| --- | --- |
| softpc32.exe | de8e3bd125e1fd1d95d68dd90f76bb1399ccc739e26d16830b5ce720bdd00319 |
| softpc64.exe | 3205f18b8d6dab27fbd36e870a2f9b9d8e0de5345660f8de9ed07f342813640e |

Actual-diff review: five case labels rejoin three native fields; all generated
readers/writers use the same existing mapping. The Unchained callee still owns
plane scaling and the caller still owns two-WORD composition. There is no
new state, consumer-specific compensation or duplicate implementation. Shared
eight-tree paths, root CMake, guest media, snapshots and INI are unchanged.
No non-mirror build warnings were observed; existing mirror warnings remain.

Final-artifact qualification completes without failures or skips: x64 background
187/187 (84.36 seconds), x86 background 187/187 (80.50 seconds), x64 native
8/8 (24.23 seconds), x86 native 8/8 (24.52 seconds). Thus all 390 distinct
architecture/test cases pass against the final builds. Both EXE hashes remain
as listed above. Documentation, 475-body mirror, whitespace and newline gates
pass. No new non-mirror warnings are observed. Task scratch is retired after
promotion of these results; only reusable caches remain. Await owner acceptance.

## P2: retained Copy fill correction

Baseline a43c6d24. The owner's additional request repairs the bulk entry itself
and asks how a future consumer should use it; it does not enable that consumer.
Frozen scope is S_2703 Copy DWORD fill and the adjacent S_2700 empty-fill
boundary identified by the mandatory same-class sweep, in sevid019.c only.

For odd addresses, the old DWORD algorithm writes a first BYTE, forms its
middle WORDs using incomplete portions of the pattern, then writes one extra
BYTE at the end. With pattern 44332211, the desired memory sequence is
11 22 33 44 repeated, regardless of start parity. Keep the head and common
loop: rotate all 32 pattern bits by 8/24 into the existing two WORD locals;
on the final odd-start iteration write only the low BYTE of the second WORD.
Zero count exits through the existing local-storage cleanup before the head.
Selected dirty dispatch, plane strides, original labels and ABI remain intact.

The unchanged-production fixture passes only 16/96 independent DWORD cases;
all 80 odd-start cases fail. With the correction, the same 96 cases pass.
They cover six start addresses (even, odd and near a plane boundary), four
patterns including nonuniform values, and counts 0/1/2/7. The oracle builds
expected repeated guest bytes in independently selected physical plane cells
and compares the entire plane allocation, including untouched planes and tail.
It neither replays the implementation as its expected result nor merely tests
continuation survival. Existing nonempty BYTE/WORD controls remain.

Adjacent-source sweep: BYTE Copy fill checks its count before its first store.
WORD Copy fill writes an odd head before decrementing count; zero underflows
to FFFFFFFF. Add the same early cleanup exit there. This second finding is
source-proven; the unguarded zero-WORD loop was not deliberately run until
memory corruption. Four additional even/odd BYTE/WORD empty-fill cases check
that all planes stay unchanged after the repair. ModeX compound replay tests
remain lifecycle checks, not a claim of complete mode-specific value coverage.
The current CPU still uses scalar stores, so these observations do not establish
a DOS/Win31/Win95 symptom or a runtime speed improvement.

### Concrete future integration boundary

1. First measure actual existing SAS callers. The whole-source scan finds BIOS
   clear/scroll uses of sas_fills/sas_fillsw in video.c, ega_vide.c, v7_video.c
   and cga.c, and ROM initialization uses sas_fills. It finds no direct DWORD
   fill call beyond declarations, definition and the SAS function table.
   CCPU c_main.c executes STOSB/STOSW/STOSD through its per-element operand
   decode/check/commit path, not through c_sas_fillsdw. Merely connecting the
   latter therefore does not accelerate CPU REP STOSD.
2. Retain one SAS linear-memory entry. Translate and validate only the next
   bounded span, respecting the current page, permissions, wrap, physical
   memory type, aperture/bank and fault ordering. Do not prevalidate the entire
   operation and suppress writes that should precede a later fault. An empty
   operation returns before backend dispatch; the physical span is nonempty.
3. Dispatch through the existing memory-type handler ownership. VIDEO calls
   the original selected C-VID fill glue, which subtracts gvi_pc_low_regen and
   invokes the current mode's handler. RAM/ROM and boundary effects retain
   their existing semantics. No Machine, Session, public Lib or Compat video
   special case, and no duplicate video algorithm is appropriate.
4. MEM_HANDLERS/gmi_define_mem currently register BYTE/WORD fills only. Start
   with measured BYTE/WORD callers if a bulk optimization is warranted. A real
   DWORD consumer would require a coherent extension of that same physical
   dispatch contract and all its providers, not a direct hard-coded call to
   fill_dword_ev_glue from a linear SAS or product path. Do not add that field
   without a real beneficial caller. Integrating REP itself is a distinct CPU
   semantic task: preserve address direction, segment/page faults and completed
   element/register progress; it is not part of this repair.
5. Before enabling any bulk dispatch, compare against the current scalar path
   for modes actually selectable by that consumer, masks/latches, odd addresses,
   direction, page/type/bank boundaries, partial faults and dirty effects on
   both architectures. Existing WORD/DWORD rules are not assumed universally
   equivalent from this Copy-only result. Measure total workload improvement,
   including original generated-rule allocation and GDP lookup overhead.

Recommendation: keep the now-correct retained Copy DWORD capability, but do not
wire an unused entry merely to give it a consumer. Investigate a measured BIOS
BYTE/WORD bulk opportunity first; add DWORD dispatch only with an identified
consumer. This describes a future admission boundary, not shipped optimization.

### P2 actual-diff review and accounting

Relative to a43c6d24, production sevid019.c is +16/-7 (net +9); the existing
chain2_rule_smoke.c fixture is +36/-2 (net +34), including removal of its
obsolete deferred-DWORD comment. No new target or fixture framework is added.
Against the pinned raw XP file, normalized-LF SequenceMatcher with
autojunk=False changes from +710/-1113 to +726/-1120: 23 more mirror-diff
lines, not 23 net production lines. CRLF remains intact in the mirror;
non-mirror changed text remains LF. Only the local manifest hash changes;
raw/subset hashes and condition structure are unchanged.

The executor's actual-source review confirms that the full-width rotations
use unsigned 32-bit operations with fixed legal shifts, both empty exits reach
existing cleanup, and the final odd byte cannot overwrite the next guest
address. Original BYTE and nonempty WORD code, plane mapping, callbacks and
selected dirty dispatch are preserved. No new owner, interface, state or
consumer appears. Public eight trees, root build infrastructure, guest media,
snapshots and user configuration remain unchanged.

The first full run (before the additional WORD empty guard) passed all 390
cases: background x64 187/187 (87.29 s), x86 187/187 (81.38 s); native x64
8/8 (25.23 s), x86 8/8 (25.17 s). Final-source qualification is recorded below
separately; the earlier run does not certify the later change.

### P2 final qualification

Final cached x64/x86 builds publish both EXEs. Background x64 passes 187/187
(87.27 s), x86 passes 187/187 (85.07 s); native x64 passes 8/8 (24.90 s),
x86 passes 8/8 (24.40 s). All 390 distinct architecture/test cases pass,
without failure or skip. Both fixtures execute 96/96 DWORD pattern cases and
the four additional empty BYTE/WORD controls. The DWORD before/after mismatch
is observed dynamically; the unguarded WORD underflow is source proof only.
No fresh manual DOS/Win31/Win95 reproduction is claimed.

Documentation, 475-body XP manifest and whitespace/newline checks pass.
The only observed final-build warning is the existing mirror switch-unreachable
statement in sevid019.c; there are no new non-mirror warnings. Task processes
finish before scratch retirement. Final published SHA-256:

| Artifact | SHA-256 |
| --- | --- |
| softpc32.exe | d6be3942694f0ca2ec26b0c4057fdf1eb8f2d0e08a63498011c1177f8aea68e6 |
| softpc64.exe | 8906b7f0f38d547679329776c8e273b45cfcadedf2ea0ac6019b5c3425e4ec13 |
