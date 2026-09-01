# M4 Frontend Mailbox Conversion And Responsiveness Proof

## Objective

Replace transitional console/window loops with mailbox consumers over the M3
runtime and prove direct-launch use with real local media.

## Acceptance

Console and Win32 window read only published frames; RDP keyboard/mouse input
has bounded enqueue-to-executor latency; idle guest execution does not busy
spin; original graphics and Windows Setup reach their documented checkpoints.

## Verification

Run x64 and x86 repository suites, source-pristine/overlay static checks, and
owner-local A:-only, C:-only, dual-media, graphics-mode, and Windows Setup
probes using non-mutating media mode.

## Stop Condition

Stop if a performance fix requires replacing a SoftPC controller, renderer,
BIOS/ROM, or BOP semantic rather than changing the runtime/frontend boundary.
