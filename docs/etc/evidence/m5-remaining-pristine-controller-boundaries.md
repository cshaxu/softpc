# M5 Remaining Pristine Controller Boundaries

## Fresh audit

`scripts/audit_pristine_divergence.ps1` against
`O:\repos.external\opennt-src-2\nt\private\mvdm\softpc.new` reports 60
canonical-text C/H divergences.  Thirty-five are named port-ABI overlays,
nineteen are original host algorithms with independent endpoints, and six
remain `restore-pristine` controller/firmware sources:

- `base/bios/reset.c`
- `base/keymouse/keyba.c`
- `base/keymouse/ppi.c`
- `base/system/cmos.c`
- `base/system/timer.c`
- `base/system/illegalp.c`

## PPI extraction result

The direct restoration candidate in `ppi.c` was rejected by evidence.  Its
standalone `HostPpiState` call is not a redundant NTVDM branch: removing it
causes the x86 original sound smoke to fail its full PPI/Timer-2 state
assertion.  The original non-NT enable/disable callbacks encode a narrower
signal.  The next valid cut is therefore an independent full-PPI sound host
contract, followed by restoration of the controller source.

## Keyboard extraction result

The original `CPU_40_STYLE` keyboard-reset path calls
`host_cpu_interrupt()`.  The historical NT implementation is an empty host
stub, and the standalone host does not yet expose a named CPU-interrupt
contract.  Restoring its call before that contract exists would add an
unproved CPU path; it remains pending extraction.

No machine source was retained or changed by these probes.  The x86 sound
smoke passed again after restoring the prior PPI source.
