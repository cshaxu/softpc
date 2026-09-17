# M9 T63 S4: CPU/SAS/RAM private archive

S4 adds the first internal state round-trip slice.  It is not a user snapshot
feature: there is no command, file container, media section, Common ABI, or
new Lib API.

## Ownership and format boundary

`compat/ccpu/snapshot.h` defines a fixed-width port ABI.  It contains copied
scalars and bounded byte ranges only: no pointer, native handle, function
address, `jmp_buf`, or C-stack content crosses it.  `compat/ccpu/archive.*`
owns temporary heap buffers for SAS RAM, SAS page types, and the FAST-TLB page
index.  `vm/snapshot_image.c` owns the VM-private aggregate and preserves the
already-audited fresh CCPU entry phase.  None of these types cross Common.

The selected original owners export their own semantic state rather than a raw
struct image:

- `c_reg.c`: visible registers, control registers, descriptor tables and six
  segment registers including hidden base/limit/access state;
- `c_main.c`: debug/test registers, interrupt map and copied reentry phase;
- `c_debug.c`: actual instruction/data breakpoint tables and counts;
- `c_tlb.c`: all 4×8 hardware-TLB entries, replacement positions, FAST-TLB
  index and physical address cache;
- `ccpusas4.c`: SAS bytes, page-type map, A20 wrapping and selector state;
- `fpu.c` / `zfrsrvd.c`: exact eight-slot value/tag stack, control/status/TOS,
  NPX instruction fields and deferred exception state.

Process bindings and instruction-local continuation remain deliberately absent.
Restore uses the existing later fresh CCPU entry, not a captured host stack.
Video planes/latches, keyboard/mouse, PIC/PIT/DMA, quick/tick queues, storage
controllers and media are still unrepresented; S4 therefore cannot be exposed
as a save/load product operation.

## Verification and review

`checkpoint_smoke` now captures a real archive, mutates CPU registers,
breakpoints, FPU control/status, RAM and a live cached translation, restores,
and proves all are recovered.  After restore it edits the restored PTE without
flushing: translation remains at the archived physical page, proving the TLB
was restored rather than rebuilt.  The same test continues to cover natural
nested-BOP return, HLT reentry, interrupt shadow, timeout and event-queue
contracts.

Both package widths were rebuilt.  `ctest --preset test-x64 -j 4` and
`ctest --preset test-x86 -j 4` each pass 103/103.  The first x64 parallel run
reported the pre-existing flaky `library.types-layout-selftest`; a focused
rerun and the final full x64 run pass.  The failure was in Lib governance
fixture isolation and no Lib path changed in S4.

Tracked production C/H/CMake changes are +646/-1 across 13 paths; tests are
+59/-0 in one path; the preserved original mirror is +366/-0 across seven
narrow state-owner hooks.  The additional `-1` is the previous single-line
CMake source-list replacement.  No duplicate CPU-state owner or alternate
restore route was introduced.

The executor/coordinator review verified that all newly retained paths have
one purpose: state-port ABI, VM-private aggregate, or one original state owner.
The aggregate does not claim disk/video/device completeness and has no product
caller.  S4 closes; T63's later device, presentation, Common and App stages
remain outside this packet.
