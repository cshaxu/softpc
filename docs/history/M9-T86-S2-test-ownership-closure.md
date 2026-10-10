# M9 T86 S2 closure: App-test ownership normalization

The owner accepts S2 and admits S3 on 2026-10-10.

S2 removed duplicated Emulator FIFO assertions and direct Emulator Session
implementation compilation from the SoftPC runtime test. It moved the shared
App-test cleanup helper to `test/app-softpc/`, removed stale include paths,
gave each non-desktop test a private ignored work directory, and made App
CTest identities/labels reflect their actual owner and execution class.

Tracked test/build code changed by `+154/-220` (net `-66`): root registration
`+132/-164`, App test source `+19/-55`, static ownership gate `+3/-1`.
Production source, public APIs, package EXEs, INI, media and snapshots did not
change. x64 and x86 non-desktop suites each passed 129/129; the focused
ownership, documentation, runtime and cross-process snapshot checks passed
4/4 on both widths. Executor deliveries are `e36b3e67`, `dd536315`, and
`a987bb5e`.

The subsequent audit found a separate public-corpus label defect. It is not
retroactively folded into S2: T86 S3 owns that shared registration work.
