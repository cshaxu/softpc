# M4 Window Frame Consumer And RDP Input Proof

## Objective

Make the Win32 window a frame-mailbox consumer and prove responsive local and
RDP input through the same normalized ingress.

## Scope

Replace direct VRAM reads, UI-thread machine calls, and periodic full-screen
polling with consumption of original-renderer frames published by M3. Normalize
window and RDP keyboard/mouse events before queueing them to the executor.

## Acceptance

The window only paints copied surfaces, the original renderer retains text and
graphics ownership, and a bounded input-latency probe shows no typing slowdown
caused by the UI thread or frame refresh loop.

## Stop Condition

Stop if the proposed fix changes original renderer, controller, BIOS/ROM, or
BOP behavior rather than the frontend/runtime boundary.
