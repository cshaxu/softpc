# M2 Original Host-Contract Compatibility Layer

## Objective

Implement a standalone substitute for the finite original host callbacks
needed by the selected machine.

## Scope

Timer notification, raw-image media, hardware/firmware BOP service dispatch,
keyboard/mouse ingress, audio sink, and original host globals/contracts. The
layer links no NTVDM, DOS/WOW, VDD, CSR, or BaseSrv code.

## Acceptance

Original CPU/device/BIOS source invokes the compatibility host through its
historical contracts; A: and C: boot paths and firmware BOPs work without a
product selector service.

## Stop Condition

Stop if a demanded callback is product semantic rather than a finite machine
or host capability; record it as unavailable instead of emulating NTVDM.
