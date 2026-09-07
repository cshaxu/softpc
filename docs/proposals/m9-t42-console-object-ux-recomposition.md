# M9 T42: Console-object And UX Recomposition

## Purpose

Implement the owner-approved Console-object architecture after T41's completed
NXVM-library import/binding baseline. The work replaces the unreliable
SoftPC-specific Console/frontend handoff with one generic `base`/`host`/`ux`
candidate, composed only by SoftPC. NXVM must adopt the resulting generic
library corpus byte-identically; SoftPC then imports that adopted corpus
byte-identically before T42 closes.

Current product behavior is owned by [Product UX](../design/UI.md); component
boundaries, lifecycle ordering, generations, and threading are owned by
[System Architecture](../design/ARCHITECTURE.md). The retained [T41 proposal](m9-nxvm-library-adoption-and-softpc-binding.md)
and [decision record](../etc/evidence/softpc/m9-t41-s5-console-design-review-record-zh.md)
are background, not competing authority.

## Implementation Scope

1. Add the generic logical `lib_console` object to `base` and the one-current-
   object native Console broker to `host`, including raw/cooked modes,
   transactional replacement, output ownership, and stale-event suppression.
2. Recompose `ux` as independently enableable Window and Console presenters,
   with separate frame mailboxes, completed presenter-set facts, an optional
   UX-owned Console object, Window-close request delivery, and no host or
   SoftPC dependency.
3. Rework only non-MVDM SoftPC code into the sole policy/reconciler owner:
   derive presenter set, current Console object, and VM lifecycle from
   configuration, completed facts, frame route, and FIFO intent; own monitor
   parsing, `console_control`, DISPLAY policy, CAP semantics, and X policy.
4. Prove the state and ordering matrix with deterministic completion-gated
   fakes before real-thread boundary tests. Build/test both package widths and
   obtain owner runtime acceptance.
5. Submit the generic `src/lib` candidate to NXVM, require its byte-identical
   adoption, then import and manifest-verify the adopted corpus in SoftPC.

## Non-goals

- No modification to `src/mvdm/softpc.new/`, its Win32 calls, or WinNT-derived
  implementation.
- No SoftPC lifecycle, DISPLAY TYPE, `console_control`, monitor syntax, or
  guest hotkey in shared lib.
- No permanent SoftPC-only fork of `src/lib`; no NXVM runtime/build dependency.
- No package configuration or guest-media modification.

## Exit

The derived-state and transition tests cover every approved Console/Window/
monitor lifecycle combination; host proves exactly one active Console object;
UX and host concurrency boundaries are deterministic; x64 and x86 builds/tests
pass; the owner accepts runtime behavior; NXVM has adopted the generic corpus
unchanged and SoftPC has re-imported and verified it.
