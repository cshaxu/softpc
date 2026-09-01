# Coding Rules

Keep original SoftPC source formatting and behavior intact. New standalone
code is C17 and belongs outside `src/core/softpc/`. A source-level portability
adjustment to original code requires a named port-ABI overlay, a reproducible
generation/patch rule, x86/x64 proof, and an explicit disposition in the
pristine ledger.

No frontend may take a lock around a call into SoftPC and then paint or wait on
the Windows message queue. No host callback may create a second device-state
owner. New code names its ownership path (`softpc_host_*`, `softpc_vm_*`, or
`softpc_frontend_*`).
