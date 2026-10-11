# M9 T88 S2: V7 packed scanout correction

The owner accepts S2 and requests a separate S3 for colour/resolution regression
coverage. S2 is closed; T88 remains open. The previous Compat repair brief is
renumbered S4, not merged into the test task.

Accepted implementation `911c4749` modifies three original display predicates:
V7 packed painter selection, interleaved row-byte stride, and pointer-coordinate
conversion. CPU bank/read/write paths remain unchanged. Production +8/-3,
net +5; existing App VGA smoke +101/-0. No new state/interface/renderer or
Lib/Emulator/Compat change exists. Configuration, media and snapshots are untouched.

Both Release packages build; focused tests pass on x86/x64. The new receiver
fails with the retained S1 behaviour. Shared 84/84 then App/checks 47/47 pass
per width, excluding five desktop tests. Original-mode, pixel, dirty, CPU
address, pointer, snapshot and Win3.1 roundtrip evidence and artifact hashes
are retained in the [proposal](../proposals/m9-win31-file-manager-vga-width-transition.md).
This is bounded S2 acceptance, not a claim that the separate incomplete-generation
publication defect or every arbitrary VGA register combination is solved.
