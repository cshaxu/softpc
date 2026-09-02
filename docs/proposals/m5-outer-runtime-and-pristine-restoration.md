# M5 Outer Runtime And Pristine Restoration

## Objective

Replace the current transitional runtime shell with an NXVM-style outer host
while returning all standalone-only machine changes to a reproducibly audited
original SoftPC baseline.  SoftPC remains one fixed `CPU_40_STYLE` machine;
this proposal neither adds a profile selector nor a session service.

The target is a direct launcher with the existing fixed `softpc.ini`, monitor
commands, optional Win32 window, raw-image media ports, and one executor.  It
must preserve the original CCPU, C-VID, controllers, BIOS, ROMs, VGA ROM and
BOP routing.  The host supplies only platform capabilities.

## Audit Baseline

The current read-only divergence command reports 60 actionable C/C-header
rows against the selected OpenNT `softpc.new` tree.  The local generated
manifest is evidence only; its machine-side rows include BIOS reset, keyboard,
PPI, CMOS, timer, ROM, video, C-VID and CCPU declarations.  Every such row
must receive a disposition before a runtime repair depends on it:

| Class | Required disposition |
| --- | --- |
| Original controller, firmware, BIOS, ROM, CCPU or C-VID logic | restore pristine, then use a generated port-ABI overlay only when width/toolchain proof requires it |
| Original `nt_*` renderer/input/audio algorithm | retain through `host/softpc-compat`, replacing only NTVDM service endpoints |
| Host media, clock, window, monitor or input collection | standalone compatibility host or VM runtime |
| Duplicate controller, renderer or scheduler behavior | remove after the original path is usable |

No local media path, guest byte, or generated build output is a repository
input or acceptance dependency.

## Target Execution Shape

```text
monitor / Win32 window
  enqueue normalized commands and input; consume copied frames
                         |
vm/runtime
  one executor; bounded queues; lifecycle publication; no guest-state access
                         |
host/softpc-compat
  original timer, media, DIB, input, audio and finite firmware-BOP contracts
                         |
port-ABI generator
  reproducible x86/x64 representation adaptation only
                         |
pristine SoftPC
  CCPU/C-VID/controllers/BIOS/ROM/VGA ROM/BOP
```

The executor may not use a nominal-MHz instruction clock, frontend polling, a
second guest timer, thread suspension, or an artificial BOP return to obtain
responsiveness.  A yield is permitted only at an evidenced original CCPU safe
point; HLT may wait on a host event without advancing guest time.  All timer,
quick-event, IRQ and controller work remains on the executor and follows the
recovered original host callback order.

## Ordered Implementation Candidates

1. **M5 pristine-source and host-contract ledger closure.** Re-run the
   divergence manifest; move or replace each machine-side standalone branch
   with a pristine source plus a named overlay/compatibility owner.  Exit only
   when no controller, BIOS, ROM, BOP or renderer duplicate remains hidden in
   `src/core/softpc`.
2. **M5 original timer, quick-event and 8042 contract recovery.** Recover the
   exact non-product host callback order required by `quick_ev`, PIT/PIC,
   keyboard refill, FDC and original host clocks.  Add an executor-owned
   multi-byte keyboard proof before touching window UX.  This is the first
   implementation candidate because the present runtime accepts only the
   first byte of a consecutive key sequence.
3. **M5 CCPU safe-boundary executor adapter.** Prove and implement a generated
   port-ABI return/wait boundary from original CCPU control flow.  Replace the
   unused `instruction_budget` facade with no API until it has a real original
   safe-point implementation.  The VM loop owns yielding; no frontend or host
   thread writes CCPU interrupt state.
4. **M5 original presenter and frame publication recovery.** Feed monitor text
   and Win32 DIB display from original `nt_cga`/`nt_ega`/`nt_vga`/`nt_graph`
   surfaces, including original text geometry, LUT, dirty-rectangle and V7
   paths.  The window only paints published snapshots.
5. **M5 real-media Windows Setup closure.** Validate A:, C:, and A:+C: boot;
   responsive console/window/RDP input; and a usable Windows Setup welcome
   screen using configured overlay media.  Diagnose any remaining guest stop
   through original SoftPC debug seams, not replacement devices.
6. **M5 dual-width release proof.** Build and execute the above path on x86
   and x64, then publish fixed `softpc32.exe`, `softpc64.exe` and example
   `softpc.ini` together without altering user configuration or guest media.

## Common Verification

Every admitted candidate records its input/output boundary, machine-source
disposition sweep, x86/x64 build, focused test, full applicable regression,
and real-media evidence.  The Windows Setup test is a milestone acceptance
test, not a substitute for controller and host-contract proofs.

## Stop Conditions

Stop and report for approval if recovery requires a DOS/WOW/NTVDM/CSR/VDD
service, modifies a BOP selector or ROM byte, introduces a second device
state owner, or lacks an evidenced original CCPU safe point.
