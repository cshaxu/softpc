# T99 S33: Windowed DOS Text Addressing Investigation

Investigation baseline: SoftPC 9250732e. The owner subsequently approved
implementation; qualification results are recorded below.

## Dynamic evidence

The owner-provided state was restored through the real Machine driver with
overlay media and fixture audio. A native probe captured the published graphics
frame without constructing a KVM Window. The 640 by 480 output reproduced the
reported corruption, excluding host-window scaling as its origin.

The probe then resumed the machine, used guest keyboard events to return to
full-screen DOS, cleared the screen, requested fresh version output and returned
to windowed DOS. Full-screen video memory contained intact text. After the
transition, the guest's saved character/attribute buffer already contained
every-other-character loss and two source rows packed into one destination row.
The fault therefore precedes Windows drawing the DOS window.

Instrumentation of the actual VGA port and video-read paths established:

- The VDD switches from CHAIN2 to unchained access and selects character plane 0.
- A guest REP MOVSB copies sequential plane bytes into its saved text buffer.
- It selects attribute plane 1 and copies every second plane byte into odd
  destination bytes, interleaving attributes with characters.
- SoftPC's compact CHAIN2 storage has adjacent characters at adjacent plane
  offsets. Attribute interleaving consequently replaces every other character.

For an A/B diagnostic only, the probe expanded the character and attribute plane
spacing in volatile video memory before the identical window transition. Font
planes were preserved. Both the resulting saved text and rendered window were
correct. This intervention was not written to guest media or the supplied
snapshot and is not proposed as a production mode-switch repacking path.

The probes completed Machine and VM destruction successfully. This is x64
dynamic evidence, not x86 or full graphics-mode qualification. NTVDM64 was not
executed; a similar symptom there is not proof of an identical cause.

## Reverse transition follow-up

The owner additionally reported corruption when a windowed DOS Prompt becomes
full-screen. A separate probe restored the supplied state, cleared the DOS
window and generated fresh version output while still windowed. The captured
graphics frame showed intact text before Alt+Enter. The resulting full-screen
text frame contained 1,000 glyphs with value 0x07: attribute bytes were being
displayed as characters between the real characters, with doubled row spacing.

A diagnostic-only compaction of character/attribute plane spacing after this
transition restored the complete text and reduced those glyphs to zero. The
probe rebuilt the existing presentation and completed normal teardown. No
production or on-disk guest state was changed. This is the reverse manifestation
of the same layout mismatch: restoring hardware-plane data and displaying it
with compact CHAIN2 assumptions disagree.

This experiment proves the windowed-to-full-screen transition with freshly
generated windowed content. It did not independently cold-boot and launch a new
initially-windowed DOS task. The proposed repair must qualify both directions
and repeated roundtrips; neither diagnostic memory rearrangement is a proposed
production repair or evidence that a production fix has already shipped.

## Source explanation

The selected CVID CHAIN2 implementation packs pairs of CPU addresses into one
four-plane backing group. In simplified units, the byte backing offset is
`4 * (address >> 1) + (address & 1)` for the first plane pair. Unchained access
uses the plane address directly. The compact representation works with the
existing compact text painter, but is observable when the guest changes access
mode to save raw VGA planes.

Relevant owners:

- `base/cvidc/sevid000.c`: CHAIN2 and unchained reads.
- `base/cvidc/sevid001.c`: backward string reads.
- `base/cvidc/sevid016.c` through `sevid020.c`: CHAIN2 write/fill/move paths.
- `base/cvidc/ev_glue.c`: selected handlers and dirty-address units.
- `base/video/gfx_updt.c`: text and CHAIN2 display-address consumers.
- `host/src/nt_cga.c`: compact text-cell transfer to the host adapter.

All paths above are relative to the preserved `softpc.new` tree. The compact
CHAIN2 read calculation was also inspected in fixed XP SP1 commit
`a2f6d7c93aa4f11efc51147566d5961c59a8ef89`. This is inherited source behavior;
the investigation does not identify a local introduction commit.

The IBM VGA/XGA Technical Reference, May 1992, explains sequencer odd/even
addressing (section 2-54), graphics-controller plane access (2-83 through 2-86),
and CRTC word addressing (2-74 through 2-75). CPU addressing and display
addressing must agree on one physical-plane representation.

Reference: https://www.ardent-tool.com/video/IBM_VGA_XGA_Technical_Reference_Manual_May92.pdf

## Recommended repair boundary

Retain the existing handlers and update their address arithmetic, traversal
increments and corresponding dirty/display units to share hardware-consistent
plane addresses. Do not introduce a second backing store, switch-time repacking,
Win3.x detection, or a special plane-reader for this guest routine. Do not change
the public frame format, Machine/Session handoff or Window renderer.

The initial review surface is ten mirror files listed above, plus the existing
video snapshot format owner and focused tests. This is a candidate surface,
not a claim that every file requires changes. Exact changed-line counts require
enumerating each generated read/write/fill/move entry and its internal calling
units; this investigation has not produced a verified production diff. In
particular, replacing one read address with address/2 is not an accepted repair.

The existing `evidfunc.c` tables provide a finite access-path review universe:
10 CHAIN2 read entries (two read modes, each with byte/word/dword and forward/
backward string reads) and 60 write entries (five selected sets, each with
12 write/fill/forward-move/backward-move operations). There are 118 generated
CHAIN2 routine bodies including their internal wrappers: 8 in sevid000, 2 in
sevid001, 6 in sevid016, 32 each in sevid017/018/019 and 6 in sevid020. These
are coverage counts, not patch-site counts. Preserve wrappers that only delegate;
review arithmetic at its existing owner rather than rewriting all entries.

For the reproduced first-plane-pair addressing case, the intended distinction
can be expressed without generated-code syntax:

```
compact backing:  4 * (cpu_address >> 1) + (cpu_address & 1)
plane backing:    4 * (cpu_address & ~1) + (cpu_address & 1)
```

This is not a global search-and-replace recipe: existing plane selection,
banking, aperture limits and CRTC byte/word address selection remain authoritative.
The matching display advance in this word-addressed text mode changes from one
plane group to two. A repair must retain the original handler dispatch and
prove these units at the access/display boundaries, not add a transition hook.

Because raw video planes are serialized, a changed backing representation must
have an explicit snapshot format boundary. Previously corrupted guest buffers
cannot be reconstructed simply by loading them with corrected video handlers.
Do not silently reinterpret old snapshots or promise their automatic recovery.

Required focused proof before delivery:

1. Byte/word/string read, write, fill and move, including direction and plane
   selection, preserve the same data across CHAIN2/unchained transitions.
2. Text scanout, cursor, font access, dirty updates and bank boundaries agree
   with the corrected address units; ordinary DOS output remains correct.
3. The fresh full-screen DOS to windowed DOS roundtrip succeeds without probe
   memory intervention; repeated roundtrips preserve characters and attributes.
4. Existing CGA/EGA/VGA/V7 graphics tests and snapshot roundtrips pass on both
   architectures, followed by the repository's required product regressions.

## Review disposition

Confirmed: a cross-mode video-memory address-layout mismatch causes this
reproduction. Confirmed: repairing plane spacing in an isolated diagnostic
eliminates it. At that investigation stage, production-path and cross-mode qualification
were still pending; the results below supersede that intermediate status.
The owner approved that repair scope; the qualification section records its
subsequent implementation and remaining coverage limits.

## Implementation qualification

The owner approved implementation after the investigation. The worktree now
updates C-VID CHAIN2 physical-address shifts and strides, the existing selected
dirty marker units, original text/CGA scanout and original BIOS scalar/pixel
writes. Public Lib/Emulator/X86 frames and rendering are unchanged. The existing
nt_text painter gathers cells into its existing host buffer; it does not repack
guest VRAM on mode changes. Snapshot serialization adds a format identity to
reject old raw video layouts rather than silently reinterpret them. Original
owner snapshots and media remain untouched.

A fresh x64 cold boot using the repaired objects reached DOS, started Win3.1,
opened COMMAND through keyboard input, switched fullscreen -> window ->
fullscreen, and captured complete text in both modes. This run used no old
snapshot and no VRAM intervention. The earlier snapshot-based diagnostic also
proved the reverse direction after fresh guest CLS/VER output. Probe captures
are disposable; this record preserves the observation, not a promise of raw
trace retention.

Focused VGA (including BIOS CGA pixels and BIOS/CPU text-plane handoff), cold
boot and snapshot tests pass. The first parallel qualification rounds exposed
the old BIOS layout (repaired), incorrect manifest normalization (corrected),
stale negative-test fixtures, and static-gate timeouts under critically low host
memory. Later jobs were externally interrupted with exit 1073807364. Those
runs are retained here as failure history, not represented as passing results.
The final cached dual-architecture qualification passes every registered test:

| Architecture | Build/publish | Background unit/gate/integration | Desktop/native audio |
| --- | --- | --- | --- |
| x64 | exit 0 | 186/186; 177.24 s | 8/8; 36.18 s |
| x86 | exit 0 | 186/186; 143.73 s | 8/8; 26.23 s |

Both package variants and native MIDI/PCM passed. Build warnings are confined
to preserved mirror sources. No public test timeout or product behavior was
relaxed to obtain these results.

The old CHAIN2 compound-entry replay test proves continuation safety, not full
value semantics. The scan found legacy ModeX word/dword decomposition and bulk
read/fill arithmetic that require a separate explicit disposition before a
complete access-path coverage claim. Current CCPU SAS string operations use
scalar byte/word accesses (ccpusas4.c); this does not make all retained C-VID
bulk entry points semantically verified.

### Compound-entry sweep disposition

The independent string-reader oracle covers mode 0/1, both address parities
and both traversal directions against scalar reads. It exposed the old reverse
mode-0 start address and the omitted start-address parity in all four string
readers; these now use the same plane spacing and final-latch alignment.
The focused VGA executable passed after these corrections. Byte/word Copy
fills have an independent physical-plane oracle. Existing compound replay
coverage remains a continuation/lifetime check, not complete value coverage.

An additional probe using nonuniform Copy dword fills exposed pre-existing
odd-address value/tail assembly errors. These are explicitly deferred in TODO.
ccpusas4.c c_sas_fillsdw iterates c_sas_storedw; the selected CPU does not call
that C-VID dword-fill entry. Its byte/word bulk function arrays are assigned but
have no reader use beyond macro definitions in that file. Do not claim the
retained compound-entry table is entirely correct or use it as a future bulk
CPU backend without resolving this debt. The two corrected physical increments
in its even dword-fill loop concern the admitted layout, not value assembly.

The final qualification rerun follows externally interrupted prior jobs
(exit 1073807364); those incomplete runs are not acceptance evidence. The
first rounds also ran with approximately 100 MiB free host RAM and timed out
in unrelated static gates. No test budget was increased.

## P1 actual-change review and delivery accounting

The executor and coordinator roles reviewed the actual diff against 9250732e.
One physical plane layout remains authoritative. No public component, guest
code, user INI or medium changed; no mode-switch repacking, new thread, clock,
state machine or renderer path was introduced. Original selected-marker and
scanout paths remain in use. Snapshot format identity rejects old serialized
raw planes; old snapshots must be recreated and are not rewritten.

Production: 15 files, +196/-220, net -24. Of these, 14 mirror files account
for +187/-219, net -32; snapshot serialization accounts for +9/-1.
Tests: 3 files, +147/-22, net +125. Counted with git diff --numstat against
9250732e, excluding documentation and EXEs.

For the same 14 mirror files compared with their verified pinned XP originals,
prior local differences were +5027/-14070 and final differences +5195/-14270.
Thus mirror difference volume increases by 368 lines although production source
net decreases. Original bytes were verified against the manifest's XP raw hash;
comparisons normalize only CRLF to LF. Existing subset exclusions remain part
of that raw comparison, not hidden as new functional changes.

| Mirror file | Prior + | Prior - | Final + | Final - |
| --- | ---: | ---: | ---: | ---: |
| base/cvidc/ev_glue.c | 55 | 10 | 55 | 10 |
| base/cvidc/sevid000.c | 482 | 487 | 501 | 506 |
| base/cvidc/sevid001.c | 455 | 458 | 467 | 470 |
| base/cvidc/sevid016.c | 654 | 715 | 656 | 717 |
| base/cvidc/sevid017.c | 475 | 648 | 479 | 652 |
| base/cvidc/sevid018.c | 501 | 668 | 503 | 670 |
| base/cvidc/sevid019.c | 680 | 1083 | 710 | 1113 |
| base/cvidc/sevid020.c | 638 | 690 | 648 | 700 |
| base/inc/gvi.h | 29 | 352 | 32 | 352 |
| base/video/egawrtm0.c | 28 | 77 | 32 | 81 |
| base/video/egwrtm12.c | 22 | 73 | 32 | 83 |
| base/video/gfx_updt.c | 567 | 3504 | 629 | 3566 |
| base/video/video.c | 355 | 3004 | 357 | 3006 |
| host/src/nt_cga.c | 86 | 2301 | 94 | 2344 |

Published SHA-256:

- softpc32.exe: `44aba28c2666688e5cd8c41cbebb8fa714bd1077a5257080d11483c68ba8b0e2`.
- softpc64.exe: `2208621a9e32bc57374035108b5df4e941b7946627163511058f66b331242c6d`.

Final review reran the VGA fixture on both architectures after removing an
unreachable test branch: both exit 0. A fresh cold DOS -> Win3.1 -> COMMAND ->
windowed COMMAND -> full-screen COMMAND run using the final objects also exits
0, including Machine and VM destruction. The published windowed graphics frame
was inspected: complete instruction text, MS-DOS 6.22 version/copyright and
prompt, without alternating-character loss; returned full-screen text contains
the same complete strings. This is native-probe evidence, not Computer Use or
owner acceptance. Test probes and raw captures are retired after delivery.

## P2 necessity audit

Owner requires keeping both the original repair and newly discovered error
corrections; only proven unnecessary edits may be restored. The frozen audit
universe is P1 versus 9250732e: 15 production files and 169 zero-context Git
hunks. The audit covered their arithmetic, actual owner and consumer, rather
than treating lack of a direct CCPU caller as permission to revert a fix.

| Change group | Disposition |
| --- | --- |
| C-VID byte/word address shifts, odd-pair advance | Keep: the actual physical layout repair. |
| String/fill/move address and traversal changes | Keep: the retained entries must use the same layout, even where the selected CPU delegates to scalar stores. |
| Newly found string start/parity/latch and even dword-fill stride errors | Keep: actual corrections, not cosmetic edits. The separately recorded odd dword-fill value defect remains deferred. |
| BIOS scalar writes and CGA pixel writes | Keep: independent original writers must agree with CPU reads. |
| Dirty indices and nonzero text/CGA scanout, scroll and cursor offsets | Keep: address units changed; restoring these would read/mark the wrong locations. |
| nt_text gather loop and removal of contiguous-copy branches | Keep: source cells are eight bytes apart; neither old contiguous-copy branch can represent the repaired layout. |
| Snapshot format identity and original line-control markers | Keep: explicit compatibility boundary and correct diagnostics. |
| Nine literal-zero display pointer replacements | Restore: both macros return precisely PCDisplay.screen_ptr, with no side effects. |

Only gfx_updt.c changed in P2: nine get_chain2_screen_ptr(0) calls returned to
the original get_screen_ptr(0). Actual variable-offset mappings and every
stride correction remain. No correct fix or new error correction was reverted.
No dead-path exemption or second representation was introduced.

Both architectures were rebuilt and published from the persistent cached Ninja
trees. The gfx_updt object is byte-identical to P1 on both architectures.
The entire EXEs are byte-identical after masking only PE TimeDateStamp and
CheckSum; all section contents, sizes and remaining bytes are identical.
This proves the narrowing has no executable behavior change. P1's full 388
passing results remain applicable; the focused VGA/mirror/documentation checks
were also rerun for this delivery. No needless full-suite repetition is claimed.

Against the original pre-S33 source baseline, production now totals +187/-211
(net -24), mirror code +178/-210 (net -32); test changes remain +147/-22.
Against pinned XP, these mirror files now total +5186/-14261: the difference
volume increase is 350 rather than 368, a reduction of 18 lines. This is removal
of nine no-op substitutions, not a redesign or an assertion that the remaining
patch is the mathematical minimum.

P2 focused results: x64 5/5 and x86 5/5, all exit 0. Published SHA-256:

- softpc32.exe: `95e00841c76c89007a09b5bf809bb1a57881fb9b05bbd9ba44d27e428f81936a`.
- softpc64.exe: `e019f3760344f3c8c6259083b2d2444cb643ee70601f66e9dd7874e7bb36857d`.
