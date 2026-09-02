# M5 Remaining Pristine Controller Boundaries

## Fresh audit

The pre-restoration run of `scripts/audit_pristine_divergence.ps1` against
`O:\repos.external\opennt-src-2\nt\private\mvdm\softpc.new` reported 60
canonical-text C/H divergences.  Thirty-five were named port-ABI overlays,
nineteen were original host algorithms with independent endpoints, and six
were `restore-pristine` controller/firmware sources. PPI, the original
illegal-driver BOP path, CPU_40 keyboard reset path and CMOS controller are
now restored, leaving these two routes:

- `base/bios/reset.c`
- `base/system/timer.c`

## PPI restoration result

`base/keymouse/ppi.c` now compares equal to the original source after line-end
normalization, including its inactive historical `#ifdef NTVDM` branch.  The
standalone build selects the original non-NTVDM enable/disable callbacks.
The standalone host owns only the endpoint composition: it records timer-2
gate state when the original timer calls its gate function, and combines that
state with the original PPI callbacks in `nt_sound.c`.  No PPI controller
logic remains standalone-specific.

`softpc-sound-smoke` exercises original port `0x61` through `ppi.c`, invokes
the original PIT-to-host waveform callback, and verifies both enable and
disable state.  It passed cleanly for x86 and x64.  The whole-tree audit script
has a pre-existing empty-source-file handling defect, so this extraction uses
the direct source comparison plus dual-width regression as its evidence;
repairing that independent audit utility remains separate work.

## Keyboard restoration result

`base/keymouse/keyba.c` now compares equal to the original source after
line-end normalization. Its CPU_40 reset bit is exposed only to that original
compilation unit by a generated CCPU port-ABI accessor, and the corresponding
historical host notification is the original no-op endpoint. The original
8042 reset smoke passes on both widths; detailed proof is in
`m5-keyboard-reset-restoration.md`.

## CMOS restoration result

`base/system/cmos.c` now compares equal to the original source after
line-ending normalization. A reproducible build-time overlay supplies the two
standalone host ABI differences: pointer-width-safe graphics configuration and
the concrete attached fixed-media topology. The controller still owns CMOS
values and POST comparison; the host reports only whether the existing fixed
image backend attached C: and/or D:.

The original fixed-product assumption that C: always exists is no longer in
the controller source. `softpc-fdc-smoke` proves a floppy-only machine reports
no fixed disk in CMOS, while `softpc-dual-media-smoke` proves the configured
A:+C: route reports the original C: type. Both focused tests pass on x86 and
x64; detailed proof is in `m5-cmos-media-port-restoration.md`.

No additional machine source was retained or changed by these probes.
