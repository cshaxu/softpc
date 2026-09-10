# M9 T49 S4 — Shared-library neutral language

## Objective

Remove SoftPC product-model terminology from the reusable library corpus so
the same source describes application/content/input behavior for every adopter.

## Boundaries

- Only `src/lib` C/H prose and private implementation identifiers may change.
- Public ABI names, event values, semantics, application code, and MVDM do not
  change.

## Required work

1. Replace `VM`, `guest`, and `machine` wording in public and private C/H
   comments with neutral application/content/input/raw-Console language.
2. Rename private helpers such as guest-input acceptance predicates to neutral
   content/input names where necessary; no public identifier changes.
3. Preserve exact capture, freeze, raw-input, and copied-frame behavior.

## Verification and exit

A C/H corpus search finds no non-historical product-model wording; strict
build, focused behavior tests, manifest verification, and dual-width regression
show no behavioral or ABI change.

## Completion evidence

- The library C/H corpus has no remaining `VM`, `guest`, `machine`, or
  `guest_input` wording. The private Window acceptance helper now names
  content input without changing its freeze/capture behavior.
- Standalone strict-lib verification and root x64/x86 CTest each passed 34/34.
  `c90c900` contains the implementation and refreshed package pair.
