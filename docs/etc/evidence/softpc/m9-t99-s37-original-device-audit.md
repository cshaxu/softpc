# T99 S37 original-device audit

Baseline: SoftPC 2b7a097a; original source XP SP1
a2f6d7c93aa4f11efc51147566d5961c59a8ef89. NTVDM64's reports are leads,
not imported patches or acceptance dependencies. No guest binary was changed.

## Finite applicability ledger

1. **CCPU BIOS access: confirmed and repaired.** Both supported builds compile
   ccpusas4.c with CCPU/CPU_40_STYLE/CPU_486, without SPC486. c_sas_hw_at,
   c_sas_w_at, c_sas_dw_at and stores reach the six BIOS helpers. PG_U is 2;
   the selected c_tlb.c access_check distinguishes supervisor/user requests.
   The six inverted CPL conditions are present in pinned XP too. The unchanged
   xtrn2phy mapper is observed by a test-local linker wrapper, never replaced.
   Matrix: CPL0/1/2/3 and V86 CPL3, supervisor/user and RO/RW pages, reads and
   writes of 1/2/4 bytes. Old requests fail 120/120 checks of permission bits;
   this does not mean every access returned the wrong data. Corrected x64/x86
   pass 120/120. V86's accessed/dirty strictness remains. The nested guest
   handler is substituted solely to observe fallback selection; protected-mode
   tests do not install fallback selectors. Neither full guest fault handling,
   486 CR0.WP policy, cross-page accesses nor all V86 cases are claimed.

2. **Slave BIOS EOI: confirmed and repaired.** CMake compiles slave_bs.c into
   machine devices; Compat's BIOS BOP table routes unused slave interrupts to
   D11_int. Old slave IRQ11 case leaves ISR 08 and overwrites mask with 20.
   One port constant now targets the command port; the prior mask write remains.
   Actual PIC tests cover slave IRQ, master IRQ, no hardware IRQ and spurious
   cascade; verify both ISRs, both masks and BIOS flag. An initial fixture
   expectation for the spurious cascade flag was corrected to FF after tracing
   intack: it has no in-service owner. This is separate from the proven EOI bug.

3. **Virtual CRTC: confirmed and repaired.** SPC386 without GISP_CPU enables
   support/ios.c virtual I/O; c_IOVirtualised executes ROM I/O in current V86
   context. ega_char_gen's original font operations call recalc_text. On an
   existing Win31 snapshot, the task probe starts a DOS VM through normal
   keyboard input, windows it with Alt+Enter, then uses the existing per-instruction
   observer to stop in real V86 state (EFLAGS 23046, CR0 8000001b). It invokes
   the original BIOS font entry with 8/16-dot font requests. Virtual CRTC is
   400 lines, physical calculation 481; original BIOS records 60/30 rows instead
   of 50/25. This is actual VDD execution, with a probe-issued BIOS request,
   not proof that a particular unmodified guest application naturally made it.
   A test-local instruction executor additionally checks real recalc_text and
   SAS virtual-I/O wiring: two VDE values exercising both overflow bits, three
   font heights, both existing V7 multipliers, VGA V86 versus native VGA and EGA
   (36 combinations). Index restoration precedes existing CRTC writes; row count,
   font height and page length are asserted. Original code fails this fixture;
   repaired x64/x86 pass. The extra original +1 rounding is deliberately retained.
   The same Win31 probe after repair records 50/25 rows and exits successfully.

4. **PIC reinitialization: no matching repair justified.** Only Sound Blaster's
   AutoInitEoiHook is registered (start and snapshot restore); there is no timer
   or keyboard EoiPending callback. Negative count deliberately does not advance
   DMA. PIC mutation, EOI dispatch and quick-event callbacks run on the single
   executor; host timer wakeups do not concurrently mutate PIC state. ICW1 clears
   old IRR/ISR/count, and the delayed callback merely unmasks and scans remaining
   IRR, so it does not recreate discarded requests. Tests cover both chips,
   normal/auto EOI, pending/in-service/empty and repeated initialization, then
   successful fresh IRQs. Actual Sound Blaster tests prove negative notification
   does not start a block, reset discards the old IRQ, and an explicit guest
   transfer restart works. A guest discarding its completion IRQ must restart
   the transfer; adding the foreign negative notification would not change that.
   No reset callback is added, hence no new reentrant requeue/lock path exists.
   Existing normal EOI tests cover the real callback's next-block queueing.

5. **Keyboard early EOI: not reproduced; existing ownership is sufficient.**
   Selected non-NTVDM keyba.c owns output_full. Only the port-60 consumption path
   releases it; port 64 and PIC EOI do not. continue_output/codes_to_translate
   cannot replace an unread slot. Existing IRQ fixture now checks make/break
   sequences with EOI-before-read and read-before-EOI, repeated port-64 reads and
   queue advancement. Existing controller reply/reset/cancel coverage remains.
   No NTVDM KbdEOIHook or second latch is introduced.

## Similar-issue sweep and limits

Enumerated PG_U requests and CPL checks in CCPU, EOI writes in BIOS and all
RegisterEOIHook production call sites. Only the six helpers have this inverted
condition; other permission logic remains unchanged. BIOS re_direct/int_287
already use slave command port. Found a separate IU8 truncation in WORD/DWORD
fallback return values; P1 transferred it to TODO. Owner-admitted P2 below
repairs it and removes that resolved entry.

The fixes affect DOS/Win31/Win95 through shared original BIOS/CCPU paths.
The Win31 evidence is specifically the real V86 font case above. Full tests
do not imply exhaustive Win95 sessions, every paging configuration, every
font/mode or all possible device reinitialization sequences.

## Diff and review

Relative to 2b7a097a, production is three mirror files, +21/-7, net +14.
Tests: four App machine files, +303/-1, net +302 (including the new 110-line
SAS executable). Root CMake is +9/-2, net +7. No public-eight-tree, Compat,
App product, guest, configuration, disk or snapshot changes. Test growth is
bounded by the five requested behavioral proofs; existing fixtures are reused.

Normalized-LF original XP comparisons, using difflib.SequenceMatcher with
`autojunk=False`, show the accumulated mirror differences below. These include
older retained patches/excluded branches, not just this task's functional edits.

| Original path | Before added/deleted | After added/deleted |
| --- | --- | --- |
| base/ccpu386/ccpusas4.c | 82/50 | 89/56 |
| base/bios/slave_bs.c | 5/7 | 6/8 |
| base/video/ega_vide.c | 226/671 | 239/671 |

Thus this task adds 28 changed mirror lines while net production size grows by
14. Original/subset manifest hashes stay fixed; only three local hashes change.
Mirror CRLF and autonomous LF are preserved. No original conditional structure
is removed or generalized, and no new alternative device implementation exists.

Executor and separate coordinator-role review inspect actual source/test/CMake
changes, not only summaries: correct U/S polarity, command/data distinction,
CRTC high-bit assembly/index restoration, unchanged multiplier/rounding, fixture
ownership and actual call paths. Non-audio tests retain existing memory audio
sinks. No new test-only production API, recovery logic, polling or second state
owner. GNU link wrapping is confined to the new App test and delegates to the
unchanged mapper; both supported MinGW architectures exercise it.

## Delivery qualification

Both cached Ninja/ccache builds publish the EXEs, with the temporary guest target
removed before full build. Original mirror compiler/header warnings remain;
no new autonomous/public-code warning or build error was observed. This is an
incremental qualification, not a clean recompilation of every unchanged file.

- x64 complete background suite: 187/187, 89.64 seconds.
- x86 complete background suite: 187/187, 79.80 seconds.
- x64 native desktop/audio/package: 8/8, 81.85 seconds.
- x86 native suite: initial 7/8 in 68.25 seconds. Compact-console package child
  failed during startup (stage 1, error 6, exit C0000142), before guest execution.
  Desktop remained active; no disconnect cause is asserted. The unchanged
  artifact passes the isolated rerun 1/1 in 8.17 seconds. This intermittent
  startup observation is not hidden or claimed fixed by the device changes.
- Final qualified universe: 390 distinct architecture/test cases, all have a
  passing result. Documentation and 475-entry mirror gates also pass after
  evidence updates. Actual-diff review and source/EXE probe-exclusion check pass.
- DOS boot/restart and product integration run in the suites. No manual complete
  Win95 session or exhaustive Windows driver/font suite is claimed.

Published SHA-256:

| Artifact | SHA-256 |
| --- | --- |
| softpc32.exe | 6c2bb71a2d7929af0a73d1f49df165d829f418ce074961bbde4f6c9f4dfdfa86 |
| softpc64.exe | 56eb0ec61e0273deb3fd3f6c3f1120f8cfbbc0577d0357da0aa34b52079c890c |

Temporary traces, guest probe, its private cached target artifacts and task
scratch are removed after promotion of these concise results. Owner acceptance
is still required; T99 is not closed by this S delivery.

## P2: full-width original ROM fallback

Owner additionally admits the adjacent WORD/DWORD truncation on top of P1
7a77c818. The four return casts in bios_read_word/bios_read_double used IU8,
although biosDoInst captures the full EAX result. They now use IU16/IU32.
No new path, state, interface or guest instruction is introduced.

The existing BIOS access fixture retains its 120 permission cases and adds
12 original-ROM cases: V86 and protected mode, BYTE/WORD/DWORD, values
0x12345678 and 0xfedcba98. Its existing linker wrapper forces only the first
outer translation to miss; the original CPU then executes the embedded ROM
against valid mapped memory. Protected mode uses the original registered
virtual selectors with a valid LDT. No guest binary is edited or rebuilt.
The test asserts full results and restoration of CS, DS, EIP, EAX, EDX and EBP.

Before the production change, all eight WORD/DWORD cases return only 0x78 or
0x98; four BYTE cases pass, and the original 120 permission cases still pass.
An initial fixture attempt omitted a valid LDTR selector and was corrected
before this complete before/after comparison; it was not a product defect.
This proves original ROM fallback return widths, not an OS page-fault handler's
policy, exhaustive paging/V86 behavior, or a particular Windows symptom.

Similar-issue sweep: searching all biosDoInst returns in ccpusas4.c finds the
four defective casts and two valid BYTE IU8 casts. Write argument widening and
c_IOVirtualised's full-width result need no change. The resolved TODO entry is
removed; the three unrelated existing findings remain deferred.

P2 production: +4/-4, net zero, one mirror file. Test: +61/-3, net +58 in the
existing fixture; no new target or framework. The accumulated normalized-LF
ccpusas4.c difference from pinned XP changes from +89/-56 to +93/-60: eight
additional changed mirror lines, no added production lines. Mirror CRLF,
autonomous LF and all original conditional structure remain unchanged.

P2 qualification: cached x64/x86 builds completed and published
both EXEs. Background suites pass 187/187 each (85.38/80.29 seconds).
No non-mirror compiler warnings were observed; existing mirror warnings remain.
Documentation, all 475 mirror hashes and changed-file newline checks pass.
Native suites each pass the three Window/Console cases but fail the three
native audio/MIDI and two package cases while quser reports the session Disc.
PCM/MIDI open fails; package cannot compose its machine or reach the prompt.
After the desktop became Active, both unchanged-artifact native suites pass
8/8 (x64 24.76 seconds; x86 24.39 seconds). The first-run failures are retained
here rather than relabelled as passes; all 390 distinct architecture/test cases
now have passing results. Final verbose focused runs confirm 120 permission
cases plus 12 original-ROM width cases on each architecture, zero failures.
No specific Windows symptom is attributed to the four-cast repair.

Published P2 SHA-256:

| Artifact | SHA-256 |
| --- | --- |
| softpc32.exe | 730b985ad48a013d631b4a756c8833c274ef1f302cb95166bd8d65eb52ca5738 |
| softpc64.exe | 3dae6c5c2453b22a101aa8794defc14cecefdf10a1c9da04aedf417916dcd306 |

Executor actual-diff review confirms four width substitutions, the unchanged
BYTE path, real-ROM execution and saved-register assertions. No public tree,
CMake, product configuration, media or snapshot changes. No new production
interface, state or alternative path. Task scratch is removed after promotion
of these results; persistent Ninja/ccache trees remain. Await owner acceptance.
