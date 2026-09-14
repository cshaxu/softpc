# M9 T56 S7: debug source provenance

## Frozen source

`common/debug` is a byte-for-byte source import from NXVM commit
`e894ef8949a0d92719678f9ebd6cec2793256ac0`, directory `src/common/debug`.

| File | SHA-256 |
| --- | --- |
| `command_runtime.h` | `3E5A477574F8DEBB2B3ADB3760242A4E65C78AF41508A67EFD41C869D6ACEA86` |
| `command.c` | `396BD482A4BE01FB8D6EF5C687036505EB06D8DD2F82E4C696C762BAFF54D05B` |
| `command.h` | `8A3D36E9C94F0D8B31144D40D7B29A09DA3AC97B02637E57EB1D0C50A26204DD` |
| `debug_interface.h` | `1FB9BD90BD37D80260B2B9036165279612D4764B243C51C81D451AB228BE9695` |
| `debug.c` | `C1272ABCF69B905EE7F5B3B542692FA44EF4A42249BC7F062FB53F7932037B10` |

## Common boundary and closure

`common/machine` now owns a small typed paused-state debug lease. A lease is
invalidated by resume, accepted reset/stop, cold run completion, and destroy.
It forwards only to an optional injected product adapter; it creates no
executor or CPU path. The SoftPC driver deliberately does not inject one, so
its debug capability is explicitly unsupported and no monitor CLI route was
added.

The common-machine fake proves lease acquisition, copied debug result,
original debug help execution, and stale-lease rejection after resume. Exact
source equality was checked against the frozen source. x86/x64 full CTest both
passed 62/62; strict lib CTest remained 8/8. Package SHA-256: `softpc32.exe`
`0A83FB27FAB87E2CD6DD456111446C4FAE0E8E165A0A3DCC411DC446CE91DCDA`;
`softpc64.exe` `B7A6AB5F6B9C3EF915C295BA6A2018A72790DE8D9D39543A303BC79CFC1F76CB`.

S7 is closed. A future product-debug task must add a SoftPC adapter only with
an admitted MVDM-safe-point contract; it must not bypass common/machine.
