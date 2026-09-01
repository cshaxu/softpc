# M3 Host Runner Pacing Removal

The removed `runner_pacer` waited after every 5,000-instruction frontend
slice.  For a fast CCPU return this became a Windows scheduler sleep, imposing
host latency on every console input poll and window worker quantum.

The console still checks its input queue before every bounded machine slice.
The window still has a dedicated UI thread, queued input, and its existing
50-ms presentation cadence.  SoftPC CPU execution, PIT/quick events, BIOS,
BOP, ROM, media, and guest time are unchanged.

Verification: x64 rebuilt and `softpc-bop-smoke` passed; x86 rebuilt and
direct `softpc-bop-smoke.exe` returned zero.

The rebuilt `build/output/softpc64.exe` and `softpc32.exe` each loaded their
adjacent fixed `softpc.ini`, entered the monitor, and exited zero after the
`exit` command.
