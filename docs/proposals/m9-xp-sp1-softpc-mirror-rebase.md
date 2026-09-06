# M9: XP SP1 SoftPC Mirror Rebase

## Purpose

Make `winxpscodes/Source/XPSP1/NT/base/mvdm/softpc.new`, frozen at
`a2f6d7c93`, the comparison baseline for the repository-owned selected
SoftPC mirror.  The result is an auditable ported mirror whose ordinary review
diff is `XP SP1 -> src/mvdm/softpc.new`; OpenNT remains a historical lineage
reference only.

This is not an import of the XP NTVDM product.  The selected machine remains
the normal i386 AT/V7 configuration (`CPU_40_STYLE`, `CCPU`, `C_VID`,
`CPU_486`, `V7VGA`, `PROD`).  PC-98, Japan-only, non-x86 and product-host
routes are deliberately outside the mirror selection.

## Baseline And Boundary

The audited roots are:

```text
OpenNT: O:/repos.external/opennt-src-2/nt/private/mvdm/softpc.new @ 5e4619ab6
XP SP1: O:/repos.external/winxpscodes/Source/XPSP1/NT/base/mvdm/softpc.new @ a2f6d7c93
Local:  src/mvdm/softpc.new
```

The frozen pre-T40 comparison normalizes CRLF/LF and terminal blank lines for
C/H ownership only.  It found 898 OpenNT C/H files, 793 XP C/H files and 526
local C/H files.  OpenNT to XP has 145 changed same-path files, 126
OpenNT-only files (principally retired non-x86 routes), and 21 XP-only files
(principally PC-98 and SoundBlaster routes).  Neither one-sided inventory is
permission to add a source to the selected standalone build.

M9 T40 subsequently removed 33 unselected local headers.  The current local
inventory is therefore 493 C/H files.  Local now lacks all 126 OpenNT-only
paths: 96 had already been absent and T40 retired the final 30 MIPS/PPC
headers.  Its other three header removals (`get_env.h`, `gfisflop.h`, and
`monsim32.h`) are present in both reference trees, so they do not alter the
OpenNT-only or XP-only counts.  None of the 33 was an A/B content-difference
path; the 58 A and 28 B semantic ledger remains intact.

For the 526 local paths with peers in both references: 344 are identical in
all three trees; 66 are existing standalone/port-ABI differences while the
two references agree and are explicitly out of scope; 58 are type A and 28
are type B below.  No local path already equals XP while differing from
OpenNT.  Thus an XP mirror claim must account for every A/B row rather than
asserting that XP fixes were previously adopted.

The frozen standalone boundary remains outside the mirror.  In particular no
XP `nt_bop.c`, `nt_vdm.c`, `nt_vdd.c`, `nt_pif.c`, `nt_inthk.c`, `nt_sas.c`,
`nt_mem.c`, `nt_umb.c`, `nt_emm.c`, or `x86_emm.c` may enter the target.
`src/host/compat/` continues to own host state, resource ownership, lifecycle,
media topology, presentation, and policy.

## Per-Path Audit

`Raw` means the XP hunk may be copied verbatim only after the stated focused
proof; it does not authorize a bulk copy.  `Filtered` means retain only
normal-i386/AT/V7 machine hunks after excluding the named product or alternate
configuration branch.  `Keep` means retain the local compatibility owner and
record the intentional XP difference.  `Block` requires owner direction.

### Type A — Local Equals OpenNT; XP Differs

| Path | XP change audit | Verdict / required local diff |
| --- | --- | --- |
| `base/bios/cmos_bis.c` | Explicit byte narrowing at CMOS I/O call sites; no product owner. | **Raw**, retaining local x64 scalar/media adaptations elsewhere; CMOS/media/reset proof. |
| `base/comms/printer.c` | XP delta includes alternate-machine/product conditional work. | **Filtered** only if an unguarded AT printer fix is isolated; serial/parallel proof. |
| `base/comms/printer_.c` | XP delta is alternate-machine conditional maintenance. | **Keep**; no selected-machine hunk identified. |
| `base/comms/rs232_io.c` | XP delta crosses NTVDM/alternate serial endpoint conditionals. | **Keep**; standalone endpoint remains external. |
| `base/disks/diskbios.c` | XP changes BIOS disk integration, including host assumptions. | **Filtered**; retain standalone image ownership and prove A:/C:/dual media. |
| `base/disks/fdisk.c` | XP changes fixed-disk controller/BIOS interaction. | **Filtered**; do not replace local media topology calls. |
| `base/disks/fla.c` | XP changes floppy attachment/control paths. | **Filtered**; retain standalone media endpoint and prove floppy-only boot. |
| `base/disks/floppy.c` | XP updates controller behavior plus non-selected conditionals. | **Filtered**; controller regression required. |
| `base/disks/floppy_i.c` | XP updates floppy internal/controller paths. | **Filtered**; controller regression required. |
| `base/disks/gfi.c` | XP alters GFI host bridge assumptions. | **Keep** local image backend unless a hunk is proved machine-only; direct/readonly/overlay proof. |
| `base/disks/gfi_mpty.c` | XP alters empty-image/host bridge assumptions. | **Keep** local image backend unless a hunk is proved machine-only; media proof. |
| `base/inc/base_def.h` | Shared build/config declarations include alternate-platform evolution. | **Filtered** declarations only; compile both widths first. |
| `base/inc/bios.h` | XP expands BIOS declarations for broader product configurations. | **Filtered** declarations required by accepted machine hunks only. |
| `base/inc/config.h` | XP configuration surface includes unavailable product/alternate options. | **Keep** selected standalone configuration. |
| `base/inc/dma.h` | XP expands DMA declarations for alternate machine support. | **Keep** unless an accepted AT controller hunk needs a declaration. |
| `base/inc/egagraph.h` | XP graphics declarations accompany alternate display work. | **Filtered** with corresponding renderer batch only. |
| `base/inc/egamode.h` | XP mode declarations accompany alternate display work. | **Filtered** with corresponding renderer batch only. |
| `base/inc/egavideo.h` | XP video declarations accompany alternate display work. | **Filtered** with corresponding renderer batch only. |
| `base/inc/floppy.h` | XP floppy declarations follow controller changes. | **Filtered** with floppy batch only. |
| `base/inc/gfi.h` | XP GFI declarations expose host product contracts. | **Keep** standalone media ABI; no raw import. |
| `base/inc/gfx_upd.h` | XP graphics-update declarations pair with renderer changes. | **Filtered** with graphics proof. |
| `base/inc/gvi.h` | XP graphics/video interface declarations pair with video changes. | **Filtered** with graphics proof. |
| `base/inc/host_lpt.h` | XP declares NT/product LPT endpoint details. | **Keep** external standalone endpoint. |
| `base/inc/mouse.h` | XP declaration changes accompany alternate/input product behavior. | **Filtered** only after input behavior review. |
| `base/inc/printer.h` | XP declaration changes accompany printer endpoint work. | **Filtered** with serial/parallel batch only. |
| `base/inc/rs232.h` | XP declaration changes cross serial host boundary. | **Keep** external standalone endpoint. |
| `base/inc/tape_io.h` | XP declaration changes accompany Japan/NT memory behavior. | **Keep** selected BIOS contract. |
| `base/keymouse/ppi.c` | XP adds PC-98 controller initialization behind alternate-machine guards. | **Keep** normal AT PPI source. |
| `base/support/time_day.c` | XP changes host-time presentation/format assumptions. | **Keep** unless a machine-only hunk is proved; clock regression. |
| `base/system/at_dma.c` | XP adds PC-98/SoundBlaster-related conditional paths. | **Keep** selected AT DMA form. |
| `base/system/illegalp.c` | XP changes product/CPU exception integration conditionals. | **Keep** current standalone trap boundary. |
| `base/system/timestrb.c` | XP adjusts host timing-strobe conditional code. | **Filtered** only with timer proof; no product scheduler import. |
| `base/system/unexp_nt.c` | XP extends unexpected NT/product handling. | **Keep** standalone failure path. |
| `base/video/cga.c` | XP includes wider machine/display configuration work. | **Filtered** normal CGA hunks only; frame tests. |
| `base/video/ega_mode.c` | XP adds alternate display/mode conditionals. | **Filtered** normal EGA hunks only; frame tests. |
| `base/video/ega_prts.c` | XP adds alternate display port conditionals. | **Filtered** normal EGA hunks only; frame tests. |
| `base/video/ega_read.c` | XP changes video read path with alternate configuration code. | **Filtered** normal EGA hunks only; frame tests. |
| `base/video/ega_vide.c` | XP changes video dispatcher with alternate configuration code. | **Filtered** normal EGA hunks only; frame tests. |
| `base/video/ega_writ.c` | XP changes video write path with alternate configuration code. | **Filtered** normal EGA hunks only; frame tests. |
| `base/video/egawrtm0.c` | XP changes write-mode implementation with alternate configuration code. | **Filtered** normal EGA hunks only; frame tests. |
| `base/video/egwrtm12.c` | XP changes write-mode implementation with alternate configuration code. | **Filtered** normal EGA hunks only; frame tests. |
| `base/video/gvi.c` | XP changes graphics-video interface for broader product builds. | **Filtered** with the video batch. |
| `base/video/v7_ports.c` | XP changes V7 port code with configuration conditionals. | **Filtered** normal V7 hunks only; V7 frame/cursor proof. |
| `base/video/v7_video.c` | XP changes V7 renderer integration with configuration conditionals. | **Filtered** normal V7 hunks only; V7 frame/cursor proof. |
| `base/video/vga_mode.c` | XP changes VGA mode paths with alternate configuration code. | **Filtered** normal VGA hunks only; frame tests. |
| `base/video/vga_prts.c` | XP changes VGA port paths with alternate configuration code. | **Filtered** normal VGA hunks only; frame tests. |
| `base/video/vga_vide.c` | XP changes VGA dispatcher with alternate configuration code. | **Filtered** normal VGA hunks only; frame tests. |
| `base/video/video.c` | XP changes top-level video selection for broader NTVDM configurations. | **Filtered** normal AT/V7 dispatcher hunks only. |
| `base/video/video_io.c` | XP changes video I/O routing with alternate configuration code. | **Filtered** normal path only; frame tests. |
| `host/inc/host_gen.h` | XP broadens original NT host declarations. | **Keep** unless an accepted external compatibility adapter needs a declaration. |
| `host/inc/host_inc.h` | XP broadens original NT host declarations. | **Keep** external ABI. |
| `host/inc/host_rrr.h` | XP broadens original NT host declarations. | **Keep** external ABI. |
| `host/inc/monregs.h` | XP monitor/product register declarations are not standalone policy. | **Keep**; no raw import. |
| `host/inc/nt_cga.h` | XP host renderer declaration evolution. | **Filtered** only alongside an external renderer adapter change. |
| `host/inc/nt_fulsc.h` | XP full-screen product-host interface. | **Keep**; standalone presentation owns this concern. |
| `host/inc/nt_pif.h` | XP PIF product-host interface. | **Keep**; excluded. |
| `host/inc/nt_reset.h` | XP NT reset host contract. | **Keep** external reset topology. |
| `host/inc/nt_uis.h` | XP user-interface/product-host contract. | **Keep**; excluded. |

### Type B — Local, OpenNT, And XP All Differ

| Path | Three-way audit | Verdict / required local diff |
| --- | --- | --- |
| `base/bios/reset.c` | Local has direct host-representation corrections; XP changes reset-era behavior/host branching. | **Block** raw import; three-way merge only after reset topology and guest-boot proof. |
| `base/bios/tape_io.c` | XP adds `JAPAN && i386` NT virtual-memory/XMS behavior; local has a separately blocked caller-context behavior change. | **Keep + Block**: exclude XP product/Japan code; owner must decide local behavior separately. |
| `base/ccpu386/fpu.c` | XP’s visible delta is a nonfunctional comment cleanup; local carries the required CCPU ABI correction. | **Keep** local code; optional comment only, no behavior work. |
| `base/comms/com.c` | XP adds PC-98 serial implementation; local differs for standalone compatibility. | **Keep** local normal-AT/host ABI; exclude XP PC-98 branch. |
| `base/inc/egacpu.h` | XP declaration evolution intersects local C-VID/CPU representation. | **Keep** local ABI; three-way declaration merge only with C-VID proof. |
| `base/inc/ica.h` | XP PIC declarations intersect local host ABI. | **Keep** local ABI; merge only with `ica.c` controller proof. |
| `base/inc/ios.h` | XP I/O declarations intersect local standalone endpoint declarations. | **Keep** external ABI; no raw import. |
| `base/inc/sas.h` | XP SAS declarations intersect local pointer-width adaptation. | **Keep** local ABI; merge only with CCPU/SAS dual-width proof. |
| `base/inc/video.h` | XP video declarations intersect local rendering compatibility. | **Keep** local ABI; merge only with accepted renderer hunks. |
| `base/keymouse/keyba.c` | XP input evolution overlaps local keyboard standalone adaptation. | **Block** raw import; three-way merge after keyboard regression. |
| `base/keymouse/keybd_io.c` | XP input evolution overlaps local keyboard I/O adaptation. | **Block** raw import; three-way merge after keyboard regression. |
| `base/keymouse/mouse.c` | XP input evolution overlaps local Windows 3.1 InPort diagnostic behavior, already a behavior blocker. | **Block** pending owner direction. |
| `base/keymouse/mouse_io.c` | XP input evolution overlaps local mouse I/O adaptation. | **Block** raw import; mouse/capture regression required. |
| `base/support/ios.c` | XP support I/O differs while local is the external compatibility owner. | **Keep** external compatibility; no machine-source import. |
| `base/system/ica.c` | XP PIC behavior overlaps local standalone IRQ/typing adaptation. | **Block** raw import; controller-only three-way merge and IRQ proof. |
| `base/system/idetect.c` | XP private MVDM include/layout assumptions overlap local ABI adaptation. | **Keep** local declaration adaptation; no product include import. |
| `base/system/rom.c` | XP ROM/config behavior overlaps local selected ROM/media adaptation. | **Block** raw import; ROM hash plus boot/media proof. |
| `base/system/timer.c` | XP NTVDM timing path conflicts with local zero-delay quick-event protection. | **Keep + Block**: retain local protection; only a proved machine fix may be extracted with timer regression. |
| `base/video/gfx_updt.c` | XP graphics update changes overlap local standalone rendering adjustment. | **Block** raw import; three-way merge with CGA/EGA/VGA/V7 frame proof. |
| `host/inc/host_def.h` | XP host definitions conflict with local pointer-width/external host representation. | **Keep** local external ABI. |
| `host/inc/insignia.h` | XP original host definitions conflict with local external host representation. | **Keep** local external ABI. |
| `host/inc/nt_graph.h` | XP renderer host contract conflicts with standalone DIB ownership. | **Keep** standalone contract; no raw import. |
| `host/src/nt_cga.c` | XP NT renderer evolves while local binds a standalone surface. | **Keep** external adapter; selectively port only proved renderer algorithm hunks. |
| `host/src/nt_ega.c` | XP NT renderer evolves while local binds a standalone surface. | **Keep** external adapter; selectively port only proved renderer algorithm hunks. |
| `host/src/nt_graph.c` | XP graphics product-host implementation conflicts with local DIB/presentation ownership. | **Keep** external adapter; no raw import. |
| `host/src/nt_keycd.c` | XP keyboard product-host implementation conflicts with local input ownership. | **Keep** external adapter; no raw import. |
| `host/src/nt_sound.c` | XP sound product-host implementation conflicts with local audio endpoint. | **Keep** external adapter; no raw import. |
| `host/src/nt_vga.c` | XP NT renderer evolves while local binds a standalone surface. | **Keep** external adapter; selectively port only proved renderer algorithm hunks. |

## Normal AT/V7 Profile Classes

> **Withdrawn preliminary classification.** This file-level grouping did not
> establish the semantic effect of every XP hunk under the actual standalone
> defines and is not an admission ledger.  It must be replaced by the
> hunk-level Ax/Bx audit before this proposal can be admitted.

The owner-requested per-file Chinese semantic audit is
[XP SP1 SoftPC 逐文件语义审计](../etc/evidence/softpc/xp-sp1-hunk-semantic-audit-zh.md).
It supersedes this withdrawn preliminary grouping for the A/B evidence.

The A/B labels above only describe the three-way text relationship.  The
following classes describe the XP hunk's relevance to the selected normal
AT/V7 machine profile.  They prevent a large XP diff which is mostly an
`#if NEC_98` wrapper from being mistaken for a large standalone upgrade.

### A1 — Mechanical selected-profile narrowing

These are unguarded normal-AT changes that make an existing byte-sized
machine value explicit.  They add no guest capability and do not replace a
host endpoint.  They are the only first-batch raw-import candidates, subject
to the stated focused tests.

- `base/bios/cmos_bis.c`: cast CMOS index/shutdown expressions to `IU8` at
  `outb`; the 8-bit port value is unchanged.
- `base/comms/printer.c`: cast status/control bit operations to `IU8` and use
  an 8-bit state value in the normal printer path; the large surrounding delta
  is PC-98-only and remains excluded.
- `base/disks/gfi.c`: narrow FDC status/device-number return and activation
  expressions to the declared `SHORT`/`UTINY` contract.
- `base/disks/gfi_mpty.c`: narrow the empty-GFI status return to `SHORT`.
- `base/disks/floppy.c`: where the normal path has a byte-store narrowing,
  take only that isolated cast, never the PC-98 media-discovery code.

### A2 — Selected-profile declarations and configuration plumbing

These files do not by themselves implement a guest feature.  They can change
only with the corresponding accepted A1/A3 hunk, and must be three-way
checked against the current x86/x64 declaration ABI:

`base/inc/base_def.h`, `bios.h`, `config.h`, `dma.h`, `egagraph.h`,
`egamode.h`, `egavideo.h`, `floppy.h`, `gfi.h`, `gfx_upd.h`, `gvi.h`,
`host_lpt.h`, `mouse.h`, `printer.h`, `rs232.h`, `tape_io.h`, and the video
and serial declarations under `host/inc/`.

### A3 — Same normal-machine subsystem; possible XP algorithm change

These retain a normal AT/V7 code path, but the current audit has not yet
proved that an unguarded XP hunk is independent of XP's media or presentation
host.  They are the genuine *machine-profile investigation* set, not raw
import candidates:

- Disk/FDC/BIOS: `diskbios.c`, `fdisk.c`, `fla.c`, `floppy_i.c`.
- Normal controller timing: `time_day.c`, `timestrb.c`.
- Normal graphics algorithms: `cga.c`, `ega_mode.c`, `ega_prts.c`,
  `ega_read.c`, `ega_vide.c`, `ega_writ.c`, `egawrtm0.c`, `egwrtm12.c`,
  `gvi.c`, `v7_ports.c`, `v7_video.c`, `vga_mode.c`, `vga_prts.c`,
  `vga_vide.c`, `video.c`, and `video_io.c`.

An A3 hunk is an improvement for the current profile only if it changes code
compiled with the selected defines and passes the named disk/controller or
frame regression; a change solely inside an alternate guard is recorded but
not copied.

### A4 — No selected-profile upgrade

These XP deltas are solely alternate-machine or product-host support, or are
wrappers needed to hold such support: `printer_.c`, `rs232_io.c`, `ppi.c`,
`at_dma.c`, `illegalp.c`, `unexp_nt.c`, `host_gen.h`, `host_inc.h`,
`host_rrr.h`, `monregs.h`, `nt_cga.h`, `nt_fulsc.h`, `nt_pif.h`, `nt_reset.h`,
and `nt_uis.h`.  They remain intentional XP-to-local differences.  `gfi.c`
and `gfi_mpty.c` are exceptions only for their A1 casts; their host bridge is
not adopted.

### B1 — XP change is outside the selected profile; local port remains

No three-way code merge is needed for the selected machine in these files:

- `base/ccpu386/fpu.c`: XP changes a comment; retain local CCPU ABI code.
- `base/comms/com.c`: XP adds the PC-98 8251 path; retain local normal UART
  and standalone host ABI.
- `base/bios/tape_io.c`: XP adds Japan/NT virtual-memory behavior; retain the
  selected BIOS contract.  Its separate local behavior blocker is not solved
  by this classification.

### B2 — Current-profile machine behavior with a real three-way question

These are the B files where XP may contain a normal-machine fix, but local
standalone work also changes the same behavior.  They need a hunk-level
three-way merge, never a whole-file replacement:

`base/bios/reset.c`, `base/keymouse/keyba.c`, `keybd_io.c`, `mouse.c`,
`mouse_io.c`, `base/system/ica.c`, `rom.c`, `timer.c`, and
`base/video/gfx_updt.c`.

The current standalone constraints are part of the acceptance test: machine
reset topology, Windows 3.1 InPort behavior, a zero-delay quick-event guard,
fixed ROM selection, image media topology, IRQ delivery, and copied-frame
rendering must survive any XP-derived hunk.

### B3 — Current-profile ABI/declaration merge only

`base/inc/egacpu.h`, `ica.h`, `ios.h`, `sas.h`, `video.h`, and
`base/system/idetect.c` may be touched only as declarations required by an
approved B2/A3 machine hunk.  The local x86/x64 ABI representation is the
starting point; XP declarations are not a replacement host ABI.

### B4 — Same original feature, different host implementation

`base/support/ios.c`, `host/inc/host_def.h`, `insignia.h`, `nt_graph.h`, and
`host/src/nt_cga.c`, `nt_ega.c`, `nt_graph.c`, `nt_keycd.c`, `nt_sound.c`,
and `nt_vga.c` implement or declare original NT host endpoints.  XP's version
is not a new guest capability; it is a different NTVDM product-host
implementation of the same display/input/audio/I/O service.  Keep the
standalone adapter.  At most, port a self-contained renderer algorithm into
the external adapter with focused proof.

## Proposed Serial Work

### S1 — XP baseline admission and fail-closed ledger

Replace the OpenNT-only comparison contract in the divergence audit with a
parameterized, frozen-baseline manifest.  Publish the OpenNT-to-XP lineage
map and XP-to-local ledger, both with exact revision/root/hash metadata.  The
XP-to-local ledger must contain all 152 material local differences: 66
unchanged port/host differences plus the 58 A and 28 B rows above.  A new or
changed local C/H path must fail the audit until it has an explicit
disposition.  This subtask changes no machine source.

### S2 — Mechanical and selected-machine XP adoption

Apply only S1-approved `Raw` rows and individually approved normal-i386
`Filtered` hunks.  Every local source diff retains its narrow reason marker;
no compatibility state enters the mirror.  Begin with the CMOS narrowing and
only then controller/media/video candidates whose focused proof is available.

### S3 — Three-way controller, timing, input, ROM, and renderer review

Resolve B rows only after their named behavior decision and focused test.  A
blocked row may stay as an intentional XP difference; it must never be hidden
by a baseline switch.  Original `nt_*` adapter work belongs in the external
host compatibility owner.

### S4 — Freeze XP mirror proof

Make the XP manifest and source-boundary check the routine review gate.
OpenNT comparison remains reproducible lineage evidence, not the default
acceptance diff.  Document the exact process for a future XP reference change
as a new admitted audit.

### S5 — Optional AT ISA Sound Blaster device (separate capability track)

Treat XP SP1's seven Sound Blaster-only paths as behavioral reference material,
not source-import candidates: `base/inc/sndblst.h`, `host/inc/nt_sb.h`, and
`host/src/nt_sb.c`, `nt_sbdsp.c`, `nt_sbfm.c`, `nt_sbmid.c`, and `nt_sbmxr.c`.
Sound Blaster is an ISA/AT expansion capability rather than a PC-98 feature,
but XP implements it through the NTVDM/VDD product boundary (`NtVdmControl`,
VDD DMA, and NTVDM port hooks).  Those sources must not enter the standalone
mirror or compatibility layer.

If admitted, this is a separately scoped post-baseline machine-capability
task.  It starts from the XP-visible device behavior and designs standalone
ownership for DSP, FM/AdLib, mixer, ISA port decode, DMA channel, IRQ delivery,
and audio-output endpoint.  It must preserve the one standalone host owner and
prove IRQ/DMA/audio behavior on both x64 and x86.  It neither blocks S1--S4 nor
turns the XP mirror rebase into an NTVDM feature import.

## Verification

Every source-changing subtask requires: source-selection proof that no
excluded XP product source is compiled; focused subsystem tests named above;
clean GCC x64 and i686 x86 builds with full CTest; both package smokes; ROM
hash checks; and no modification to user-owned `assets/binary/softpc.ini` or
guest media.  S1 additionally verifies that the audit fails on an injected,
unclassified path without retaining that injection.

## Stop Conditions

Stop for owner direction if a candidate changes guest-visible controller,
firmware, ROM, BOP, timer, renderer, or media semantics; if it needs NTVDM,
DOS/WOW, VDD, CSR, PIF, product host state, or a second host owner; or if a
three-way merge cannot preserve one x86/x64 selected-machine source model.
