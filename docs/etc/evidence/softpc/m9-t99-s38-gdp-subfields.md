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
it merely to exercise an existing API. The known odd Copy DWORD-fill defect
remains in TODO and must be resolved before that consumer is enabled. No new
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
