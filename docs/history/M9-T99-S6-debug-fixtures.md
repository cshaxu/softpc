# M9 T99 S6: Private debug and boot fixtures

The owner closes S6 by immediate advancement to S7, without another acceptance
round. T99 remains open. Delivery is 8566eb42, actual-commit coordinator review
is 819ff0cc. This closure introduces no source, test or artifact change.

S6 does contain code changes: production +4/-0 and tests +20/-0. The existing
debug refetch hook clears quick eligibility; command-provider owns its test
loop and restores ejected media before reset; runtime restores its boot media
before restart. Original HLT, BIOS and BOP semantics remain unchanged. Only
c_main.c's local normalized mirror hash changes; the XP subset is unchanged.

Cached dual builds publish both EXEs. Fresh qualification passes 192/192 per
architecture, focused uninstrumented checks total 120 passes and additional
desktop package checks total 24 passes. These are distinct runs, not a claim
of several full qualifications. The TF correction has source and historical
observation evidence, not a new deterministic failing-before reproduction.

The historical native pre-main Console initialization failure was not
reproduced or repaired. The owner explicitly accepts current passing
qualification without carrying it as active/deferred work. Research remains
in the proposal; no new TODO or Queue blocker is created.

Published SHA-256 identities:

- softpc32.exe: 0EA1A248BC280F9CA04B52005EE2594C5D9568AFCF07311DD81262B37E08F6BF
- softpc64.exe: 337EBE62DA50318FE53C82B4275C0F73CDBA2389EDFEE8BE29261208ECBF6077

Documentation, all 505 XP mirror bodies, whitespace and build-residue checks
pass. Owned ETW sessions and temporary probes are removed; all jobs have
exited and scratch is retired. Public trees, INI, snapshots and guest media
are unchanged. S7 owns the preplanned App test-contract audit and repair.
