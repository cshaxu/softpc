# M9 Td S19: XP Baseline Proposal Refresh

## Owner Request And Scope

The owner directed a Td governance refresh after reviewing the current project
rules and T83 layout: the queued XP task still aims to move the `softpc.new`
comparison baseline from OpenNT to XP SP1. Update the proposal accordingly.

This governance delivery changes proposal, Queue and compact Current-state
records only. It neither admits the numeric XP task nor changes source, build
configuration, tests, executable artifacts, user-owned INI or guest media.

## Decision Record

The revised queued proposal establishes these owner-directed points:

- XP SP1, not OpenNT, is the intended final routine mirror baseline and daily
  comparison target.
- OpenNT remains frozen lineage/migration evidence.
- IBM PC/AT hardware semantics are XP-first candidates. The final file shape
  may be XP unchanged, XP plus an explicit local standalone/x64 patch, an
  explicitly selected XP hunk, or no introduction for an excluded route.
- PC-98, Japan, non-x86 and NTVDM-product routes have explicit dispositions;
  a disabled conditional is not a silent adoption decision.
- XP-only ISA Sound Blaster is a later XP-mirror capability batch: its device
  model may be imported from XP, while VDD/NTVDM host attachment is excluded
  and replaced at the standalone boundary.

The older Chinese A/B audit remains supporting historical evidence. Its
pre-T83 inventory and old whole-file destination ledger cannot serve as an
admission ledger until S1 recomputes the current path/hash/profile universe.

## Verification And Closure

Finite review scope: the XP proposal, Queue link/text, Current governance
summary, source/research policy, current source-layout authority and the
historical A/B audit index. The revised proposal links existing evidence,
does not duplicate current rules, identifies current `src/app-softpc/softpc.new`
and names the current dual-width background CTest presets.

Run documentation governance and `git diff --check`. No build or runtime test
is required or claimed because no runnable path changed. Td S19 closes when
those documentation checks pass and changed-path review confirms that only the
four listed documentation records changed.
