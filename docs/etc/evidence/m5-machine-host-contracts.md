# M5 Machine/Host Contract Extraction

## Result

The six remaining machine-source rows have a bounded independent-host route.
The declaration boundary is capability-only: it does not let the runtime
inspect guest RAM, device state, CCPU state, ROM bytes, or BOP selectors.

| Restoration row | Required host capability | Existing owner | Source disposition |
| --- | --- | --- | --- |
| `base/bios/reset.c` | fixed machine configuration: graphics, memory and attached media names | `src/core/softpc_standalone_platform.c`: `config_inquire` | restore POST after configuration calls are carried by the host configuration contract |
| `base/keymouse/keyba.c` | host key event -> original SoftPC key number; executor wake is already owned by runtime | original `host/src/nt_keycd.c`: `softpc_host_scan1_to_key`; standalone platform ingress | restored table ownership to the original host algorithm; retain the CPU_40 generated interrupt call as a port-ABI overlay |
| `base/keymouse/ppi.c` | Timer-2 PPI state notification | original `host/src/nt_sound.c`: `HostPpiState`, `PlayContinuousTone` | retain original PPI logic; replace only the selected compile-time host endpoint |
| `base/system/cmos.c` | presence of configured A:, C: and D: media | standalone `config_inquire` backed by the fixed image attachment | replace standalone CMOS condition with a media-presence host query before restoring source |
| `base/system/timer.c` | wall-clock sample, one executor-owned heartbeat and quick-event delay semantics | standalone `host_gettimeofday`, `host_timer_init`, `host_timer_event`, `host_delays` | recover and prove the original quick-event timing invariant before removing the zero-delay guard |
| `base/system/illegalp.c` | explicit unavailable-product error result | standalone `host_error` | host reports unavailable; it must not emulate a product driver or alter the BOP selector |

## Keyboard Contract

`softpc_host_scan1_to_key(unsigned int)` is declared in
`src/host/softpc_compat/softpc_host_input.h` and implemented beside the
original `Scan1ToKeynum` table in `host/src/nt_keycd.c`. The standalone
platform converts a non-extended Set-1 make/break byte through that endpoint,
then invokes the original `host_key_down`/`host_key_up` controller ingress.
It owns no keyboard-controller table.

The original `CPU_40_STYLE` keyboard-reset code writes a private CCPU
interrupt-map representation that is not declared to the machine device
target. The selected CCPU exposes the equivalent mechanical request through
`cpu_interrupt(CPU_HW_RESET, 0)`. This one-line difference remains a
reproducible **port-ABI overlay**, rather than inventing a host CPU-reset
protocol or allowing the host to write CCPU state.

## Proof

On the x64 MinGW and established x86 Clang CMake/Ninja builds, the three
affected source units compiled; each focused `softpc-machine-smoke` passed,
and each real-media `--windows-setup` probe passed with configured overlay
media. The smoke reaches repeated bounded machine execution, therefore
exercising the restored CPU_40 reset request path without a separate
guest-state owner.

This declaration cut changes no ROM byte, BOP selector, controller protocol,
firmware behavior, guest image, CCPU execution semantics, or renderer path.
