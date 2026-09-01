# M2 Normalized Keyboard, Mouse, And Audio Host Contracts

## Objective

Restore original host-facing input and sound contracts behind a narrow,
portable standalone boundary.

## Scope

Preserve original SoftPC key/scancode, mouse, and sound contract semantics.
Replace only console focus, VDM suspension, Windows handle, and wave-device
endpoints with normalized host callbacks.

## Acceptance

Original device code receives keyboard and mouse ingress through its historical
contracts, including key-up behavior. The audio contract consumes host output
without taking machine ownership. The implementation has no RDP-special CPU or
renderer path.

## Stop Condition

Stop if a required input or sound operation is a product session service rather
than a finite host endpoint.
