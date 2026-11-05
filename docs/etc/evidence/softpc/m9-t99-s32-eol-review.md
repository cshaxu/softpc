# T99 S32 tracked-text line-ending verification

The frozen universe is `git ls-files --eol` at dff86c90 plus this task's
new text records. Git stores tracked text as LF. Worktree attributes require
CRLF for the mirror and the user-owned package INI, LF for other text; binary
files are excluded. The INI and binary/media/snapshot data were not normalized.

Eight tracked worktree files had nonconforming endings and were normalized
without changing their Git-normalized content:

- Mirror: `host/inc/nt_sb.h`, `host/src/nt_sb.c`, `nt_sbmid.c`, `nt_sbmxr.c`,
  `base/ccpu386/c_debug.c`, and `base/cvidc/evidgen.h`.
- Outside mirror: `docs/etc/evidence/softpc/m9-t99-s30-p20-unique-branch-proof.txt`
  and `docs/history/M9-T99-S16-public-audit.md`.

After normalization: 471 mirror text paths are CRLF; 1,517 other tracked text
paths are LF; the package INI is CRLF; 14 binary paths are excluded. The
normalized files have no Git content diff. New S32 text records use LF.

Both ccache-enabled Ninja trees built all targets successfully. The package
EXEs were refreshed: x86 SHA-256
`7d1245f261bf86272e966eeb9759aa18d6cf4eeec80a6514e5f2622bd7ae517d`,
x64 SHA-256
`9f1212f8b26a5e98bb06b5fec601a6ff7cf6c499f16991dd68d62fb81ab75c3e`.

Background CTest: 186 tests per architecture. The first x86 run had four
failures: build-cleanup selftest could not create a junction in the sandbox,
and the Types layout, Emulator negative verifier, and X86 Debug negative
corpus tests timed out. The first x64 run had three failures: the same junction
restriction and the Types and Emulator timeouts. All affected tests passed on
serial rerun outside the sandbox: x86 cleanup, Emulator and X86 Debug passed
in the first rerun, Types timed out once more and then passed in 109.72 seconds;
x64 cleanup, Types and Emulator passed (Types 91.87 seconds). Another
project's concurrent high-parallel build was observed during the initial runs;
it had ended before the successful final x86 rerun. This observation does not
prove the sole cause of the timeouts. The original per-test budgets were not
changed.

Native integration CTest outside the sandbox passed 8/8 per architecture,
including package, compact Console, Window and native audio/MIDI. XP mirror,
documentation-governance and build-residue gates were also run; no S32 source
or test semantic change was made. Raw task logs are disposable after delivery.
