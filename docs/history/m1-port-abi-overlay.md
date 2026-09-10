# M1 Port-ABI Overlay Extraction

## Objective

Move unavoidable x64/x86 CCPU and C-VID representation adaptations out of
pristine source into reproducible port-ABI inputs.

## Scope And Guardrails

Cover pointer-width GDP state, generated C-VID rules, and compiler-visible
declaration corrections only. Do not add scheduler policy, device behavior,
BOP services, BIOS changes, or frontend logic.

## Acceptance

Both host widths build from the same pristine source manifest; every generated
or overlay transformation has input, output, rationale, and focused proof.

## Stop Condition

Stop for owner direction if a required adaptation changes guest-visible
machine behavior rather than host representation.
