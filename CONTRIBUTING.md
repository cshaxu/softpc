# Contributing

Read [the documentation guide](docs/README.md), the active packet, and the
execution rules before proposing or changing work. The design and rules linked
there are the project authorities. Apply the linked shared governance skills
when their scope applies; the repository documents remain the local authority.

Every change records its ownership boundary, user-visible impact, verification,
and any deferred work. Use the task identifier and commit conventions in the
execution rules. A local executable is developer evidence, not a release.

The complete `build/` tree is ignored and reserved for native configurations,
generated sources, tests, logs, captures, and disposable media. Use `build/`
and subdirectories such as `build/x86/` for additional toolchains. The only
user-facing deliverables are `artifacts/binary/softpc32.exe`,
`artifacts/binary/softpc64.exe`, and their adjacent `softpc.ini`. Reusable
guest media belongs in `artifacts/media/`. The fixed original ROMs are embedded
from `src/mvdm/softpc.new/roms/`; any future external-ROM contract belongs in
`artifacts/roms/`.

`artifacts/binary/softpc.ini` is user-owned configuration and is never an
agent build or packaging output; see the binding rule in
[Execution Rules](docs/rules/EXECUTION.md#user-package-configuration).
