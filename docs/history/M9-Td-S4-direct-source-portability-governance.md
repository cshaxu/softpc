# M9 Td S4: Direct-Source Portability Governance

## Outcome

Closed as the governance admission for the direct-source-diff portability
model. Architecture, source-layout, coding rules, roadmap, current baseline,
and the public source-layout description now agree on these boundaries:

- `src/mvdm/softpc.new/` may carry only narrow, locally explained mechanical
  compiler, declaration, calling-ABI, or pointer-representation corrections;
- host state, ownership, lifecycle, capability, resource, and policy work
  belongs outside that tree in the host compatibility owner; and
- build-time source transforms and generated C/H build inputs are not an
  admissible final portability mechanism.

No machine source, CMake source selection, scripts, package artifact, media,
or user configuration changed in this governance task. Documentation
governance validation passed. M9 T38 is the implementation task that applies
the audited direct corrections, removes the transform rules, and proves both
package widths.
