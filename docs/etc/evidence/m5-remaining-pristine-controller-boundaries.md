# M5 Remaining Pristine Controller Boundaries

## Fresh audit

The pre-restoration run of `scripts/audit_pristine_divergence.ps1` against
`O:\repos.external\opennt-src-2\nt\private\mvdm\softpc.new` reported 60
canonical-text C/H divergences.  Thirty-five were named port-ABI overlays,
nineteen were original host algorithms with independent endpoints, and six
were `restore-pristine` controller/firmware sources.  PPI is now restored,
leaving these five controller/firmware routes:

- `base/bios/reset.c`
- `base/keymouse/keyba.c`
- `base/system/cmos.c`
- `base/system/timer.c`
- `base/system/illegalp.c`

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

## Keyboard extraction result

The original `CPU_40_STYLE` keyboard-reset path calls
`host_cpu_interrupt()`.  The historical NT implementation is an empty host
stub, and the standalone host does not yet expose a named CPU-interrupt
contract.  Restoring its call before that contract exists would add an
unproved CPU path; it remains pending extraction.

No additional machine source was retained or changed by these probes.
