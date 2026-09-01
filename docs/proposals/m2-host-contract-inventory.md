# M2 Host-Contract Inventory And Minimal Compatibility Surface

## Objective

Turn every externally resolved symbol used by the recovered machine into a
named host contract with a source owner, capability category, and disposition.

## Scope

Inventory original host callbacks, globals, headers, and link inputs reached
by the selected original CCPU, controllers, BIOS, BOP paths, and renderers.
Classify each as timer, media, finite firmware/hardware BOP, surface, input,
audio, unavailable product semantic, or unresolved.

## Acceptance

There is one generated or reviewable ledger for all selected link-time host
dependencies. The public standalone host header contains only opaque handles
and copied values; no guest state or NTVDM product type crosses it.

## Stop Condition

Stop if a callback cannot be categorized without inferring undocumented DOS,
WOW, or NTVDM behavior.
