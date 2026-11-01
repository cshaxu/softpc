# Coding Rules

Apply the shared [coding-governance skill](https://github.com/cshaxu/skills/blob/main/coding-governance/SKILL.md).
The concrete SoftPC source layout is [Source Layout](../design/CODING.md).

Keep original SoftPC source formatting and behavior intact. New standalone
code is C17 and belongs outside `src/mvdm/softpc.new/`. A narrow source-level
portability adjustment may be made directly at its affected point only for a
mechanical compiler, declaration, calling-ABI, or pointer-representation
correction. It requires a local reason marker, x86/x64 proof, and an explicit
disposition in the pristine ledger. It must not change guest or machine
behavior or add host policy, lifecycle, ownership, capability, or state;
those larger functional differences belong in external host compatibility
code. Build-time source transformation and generated C/H build inputs are not
permitted for such portability corrections.

## Selected mirror conditional branches

For every retained `src/app-softpc/softpc.new/` source subset, preserve the
original conditional-compilation structure, including the applicable `#if`,
`#ifdef`, `#ifndef`, `#elif`, `#else`, and `#endif` directives. Restore any
such structure previously removed from a retained file when reconciling that
subset with its pinned original. Preserve selected bodies and necessary local
repairs; do not convert a supported or shared branch into a failure.

When a branch body is deliberately excluded from the retained subset, replace
its excluded body with a single `#error` directive that identifies the
unsupported mirror configuration. Do not use an explanatory comment as the
replacement; existing comment-only exclusion placeholders must be converted
to `#error`. Preserve the original nested condition structure where applicable.
Retain or restore original `#line` directives needed for correct diagnostics
and source locations; where exclusion shifts retained code, use correctly
verified line-control directives at the applicable boundary. `#line` is line
control, not a substitute for `#error` or an explanatory placeholder.
This rule concerns excluded branch bodies, not ordinary original comments,
include guards, runtime diagnostics, or whole files excluded from the project.

Classify each exclusion against the pinned original, all supported production
and test configurations and the local patch ledger before changing it. Keep
raw original, selected-subset and local differences separately auditable;
record restored directives in provenance and retain each `#error` as a visible
local difference rather than concealing it in the subset baseline. Validate that
supported x86/x64 paths still compile and retain their behavior, and that
deliberately enabling an excluded configuration fails at its `#error`. Verify relevant diagnostic
file/line mappings when line-control directives are restored or added. Do not
generate transformed source as a build input.

No frontend may take a lock around a call into SoftPC and then paint or wait on
the Windows message queue. No host callback may create a second device-state
owner. New code names its ownership path (`softpc_host_*`, `softpc_vm_*`, or
`softpc_frontend_*`).

Structural relocation uses `git mv`; formatting-only and behavioral changes
remain distinguishable in review. Tests preserve their declared unit or
integration input boundary.
