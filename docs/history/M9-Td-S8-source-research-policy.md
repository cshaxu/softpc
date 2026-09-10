# M9 Td S8 — Source and research policy

## Objective

Make SoftPC's source provenance, third-party material, historical research,
and binary/firmware boundaries explicit and enforceable at the same governance
level as NXVM, without asserting rights that this repository has not proved.

## Scope

- Add a single supporting source/research policy, indexed by `docs/etc/`.
- Add the concise enforceable boundary to the architecture rules and make the
  documentation gate require the policy's stable structure.
- State the separately owned status of the preserved SoftPC mirror, project
  code, exact shared-library adoption, and read-only comparison trees.
- State the required task evidence for any future external source import,
  research result, binary, firmware, or license decision.

## Non-goals

- No source import, license grant, copyright rewrite, or distribution change.
- No modification to `src/mvdm/softpc.new/`, `src/lib/`, package media, or
  executable artifacts.
- No claim that historical SoftPC source, OpenNT, NXVM, Microsoft binaries,
  or third-party firmware is redistributable under a license not recorded by
  the owner.

## Exit criteria

- One indexed policy distinguishes provenance facts from unproven legal
  conclusions and gives a clear stop condition for each external-material
  class.
- The current architecture rule links to that policy and the documentation
  verifier rejects a missing or structurally incomplete policy.
- Real gate, accepted/rejected verifier fixtures, and diff checks pass; the
  work is committed and pushed with a clean worktree.

## Completion evidence

- The indexed policy distinguishes preserved source mirror, project code,
  exact shared-corpus adoption, and read-only comparison trees.
- It records existing ROM/media inputs as legacy material, not a license grant,
  and bars new external source, binary, firmware, media, or redistribution
  claims without a dedicated owner/legal task.
- The architecture rule links the policy; the documentation verifier rejects a
  missing policy and its accepted/rejected fixture suite passes.
- Fresh x64 and x86 CTest each passed 36/36 after the policy change. No source,
  binary, package, or media asset changed.
