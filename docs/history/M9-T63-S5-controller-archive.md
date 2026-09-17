# M9 T63 S5: Controller and Queue Archive

S5 is closed after the pushed implementation range `0cf40a9..c3cba33` and an
independent actual-change review.  It adds only private VM/Compat archive state:
fixed-width PIC, PIT/RTC, DMA, FDC, HDD and q/tic representations plus semantic
callback IDs.  It exposes no Common or Lib API, App command, file format or
media path.

Queue restoration validates and builds both replacement queues before either
live queue is replaced.  Unknown callback IDs, active PIC callbacks, and HDD
continuations without a fixed semantic identity reject capture.  No callback
address, native handle, host stack, `jmp_buf`, controller struct byte dump or
ordinary enqueue operation crosses the archive boundary.

Focused x64/x86 checkpoint and source-boundary tests pass.  Final sequential
full CTest passes 103/103 on each width.  The P2 production package code is in
`assets/binary/softpc32.exe` and `assets/binary/softpc64.exe`; P3/P4 add only
test/evidence coverage.  The detailed implementation and receiver disposition
are retained in [the T63 record](M9-T63-machine-snapshots.md).
