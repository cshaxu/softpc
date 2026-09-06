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

No frontend may take a lock around a call into SoftPC and then paint or wait on
the Windows message queue. No host callback may create a second device-state
owner. New code names its ownership path (`softpc_host_*`, `softpc_vm_*`, or
`softpc_frontend_*`).

Structural relocation uses `git mv`; formatting-only and behavioral changes
remain distinguishable in review. Tests preserve their declared unit or
integration input boundary.
