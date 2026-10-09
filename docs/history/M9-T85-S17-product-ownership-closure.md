# M9 T85 S17 — Product ownership closure

The owner accepts the Product establishment and explicitly directs admission
of the following Ninja build-efficiency S.

## Delivered boundary

- Product owns the moved debug, xasm32 and surface implementations and their
  tests.
- The retired `src/x86`, `src/ibmpc`, `test/x86` and `test/ibmpc` component
  trees, their build entries and obsolete firmware helpers are removed.
- Product's corpus checks retain the one-way `lib < common < product` graph.

## Delivered commit and verification

`288d9319` removes the retired roots.  The focused Product suite passes on
x64 and x86, as recorded in the admitted S17 delivery.  This closure does not
claim a package refresh, runtime behavior change, configuration/media change
or a rename of Common; those are outside S17.
