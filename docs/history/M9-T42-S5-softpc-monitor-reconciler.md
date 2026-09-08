# M9 T42 S5: SoftPC Monitor And Derived-State Reconciler

## Closure

S5 is closed by owner direction after the SoftPC-side control path was
implemented and its two package widths were rebuilt.  It gives `src/app/` the
sole product-policy role: monitor lines, copied UX events, runtime state/frame
completion, component completion, and broker completion enter one control FIFO.
The reconciler alone derives lifecycle requests, Window/VM-Console existence,
and the Current Console transition from completed facts.

The generic library remains policy-free.  MVDM and the original-host ABI were
not changed.

## Delivered Facts

- Runtime snapshots use two buffers and publish only complete snapshots.  A
  graphics no-dirty callback leaves the last published frame untouched.
- The control path observes and forwards only a new committed frame sequence.
  Text-mode snapshots now publish only when their actual text/cursor/font/
  palette state changed; an executor heartbeat cannot flood the control FIFO
  and starve raw Console input.
- Lifecycle, component, and broker requests are completion-gated.  The staged
  host replacement keeps an old Current Console active until a replacement
  reader/mode is prepared.
- `assets/binary/softpc64.exe` and `softpc32.exe` were rebuilt.  The adjacent
  user-owned `softpc.ini` and guest media were not modified.

## Verification

- GCC x64 configure/build and full CTest: 26/26 passed.
- MSYS2 MinGW32 configure/build and full CTest: 26/26 passed.
- Focused runtime regression proves an unchanged text screen preserves its
  published sequence, alongside runtime, reconciler, broker, source-boundary,
  documentation, manifest, and package tests.

The final package-refresh commits are `1e1666d` (x64 and text-frame gate) and
`8cd95ed` (x86).

## Follow-on Boundary

The owner reports residual manual interaction edge cases to revisit later.
They are not silently claimed as S5 runtime acceptance.  T42 S6–S8 remain
unadmitted: real-thread integration acceptance, owner matrix acceptance, and
NXVM byte-identical adoption/re-import are separate work.
