# M9 T55 S1 — Exact NXVM Library Refresh Closure

## Closure decision

S1 is accepted by the owner and closed. SoftPC now carries the NXVM
`64d211c9` library corpus exactly: its 74 relative paths and SHA-256 values
match NXVM with no local library variant.

The only integration changes outside that corpus are the explicit byte-count
prompt-trace writer adaptation and the standalone cold-run keyboard boundary.
The latter clears a completed run's IRQ1 and invalid 8042 output byte, so a
new cold boot cannot consume an old scan record.

## Verification

- Library manifest and a complete relative-path SHA-256 comparison passed:
  74 SoftPC paths, 74 NXVM paths, zero one-sided paths, zero differing hashes.
- The exact Ctrl-break cross-run smoke passed five consecutive times at x64
  and x86.
- Complete CTest passed 37/37 at x64 and 37/37 at x86; both package targets
  rebuilt the agent-owned `softpc32.exe` and `softpc64.exe`.
- Documentation governance and `git diff --check` passed. No MVDM source,
  guest media, or user-owned `assets/binary/softpc.ini` path changed.

## Delivery

Executor delivery: `aa90f1e M9 T55 S1 P1: refresh canonical NXVM library`.

