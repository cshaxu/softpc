# M9 T56 S5: ownership convergence and audio lifecycle

## Owner requirement

Audit post-extraction app/host direct library calls. Remove duplicated or
bypassing ownership only; retain legitimate product configuration, media,
trace and compatibility work without a common wrapper layer.

## Delivery

The audit retained these single-owner direct calls:

- app configuration read and prompt trace writer use storage;
- app keyboard is the SoftPC guest-input adapter over copied KVM events;
- host machine preflight plus FDD/HDD image providers own their product media;
- host platform pacing owns its clock/sync mechanics.

None accesses a broker, KVM leaf, session queue or executor owned by common.
The only duplicate lifecycle was the optional speaker worker. It was created
lazily by the first tone callback and destroyed as a side effect of the
original reset timer teardown. The app now creates it explicitly during
entity assembly and joins it before machine destruction. The callback only
updates tone state and wakes that already-owned worker; reset has no thread
creation/destruction side effect.

`softpc-audio-lifecycle-smoke` proves start idempotence and both shutdown /
recreate cycles without emitting a tone. This preserves the original sound
device state machine while giving the product one auditable worker owner.

## Verification

- x64 full CTest: 60/60 passed.
- x86 full CTest: 60/60 passed.
- strict standalone lib CTest: 8/8 passed.
- Documentation/DAG gates and `git diff --check` passed.
- Package SHA-256: `softpc32.exe`
  `58D3B35C91B07FF3A3E2C4E0A343BDF235B0C018BCEE7BD603AB800A57282230`;
  `softpc64.exe`
  `01DF2F0754A8B89944A26F871085C0C79B62BA42C9D33E5A8B436C4B0D352E5E`.
  User INI and media were unchanged.

## Reviewer closure

The retained direct calls are responsibility-owned and do not duplicate a
common component. The audio worker now has one explicit construction and
destruction route. S5 is closed; S6 may import the approved xasm32 corpus.
