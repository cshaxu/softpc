# T93 S11 — UTF-8 paths and native directory rules

## Admission and closure

Original owner request: "验收通过，准入执行S11到收口". This accepts S10
and authorizes S11 through verified closure, not automatic S12 admission.
Baseline: 45d6f860. Executor 33a9c9f7 is committed and pushed; subsequent
coordinator review inspected the actual committed changes and evidence.
S11 is closed on 2026-10-16. T93 remains open without an active S.
The first closure document used an equivalent idle sentence instead of the
gate's exact idle marker; its documentation check failed. A follow-up governance
correction adds the required idle/open-task markers and reruns that check.
No production, test implementation or executable changes are involved.

## Result and ownership

- Base returns UTF-8 executable directories, with native root/separator rules
  in each platform leaf and unchanged caller output on failure.
- Storage reads, owned bytes, medium opens and binary truncate/append writers
  interpret the same UTF-8 paths. Existing sharing, locking and stream lifetime
  remain; reader failures preserve their precise status.
- Windows uses strict native wide conversion. POSIX preserves literal
  backslashes and rejects non-UTF-8 path bytes. Base owns the small stateless
  validator; Types owns only native declarations. The inward Storage/Base
  edge is explicit in the build, design and exhaustive dependency test.
- No App, Emulator, X86, original mirror, INI, media, snapshot or audio code
  changed. No new worker, recovery state, fallback or path framework exists.

## Proof and accounting

Both cached Release builds publish both EXEs. Each architecture passes
96 shared + 65 other unit/integration + five native desktop tests: 166/166,
zero failures, disabled or skipped cases. All ten desktop cases ran without
Computer Use. Native Unicode file names are checked independently through
the wide OS boundary. Linux implementation tests use controlled native calls;
this is not a Linux-host product qualification claim.

Production C/H: 16 Lib paths, +199/-45, net +154. Test C/H: three Lib paths,
+160/-39, net +121. Source build/gates: +8/-3; test build/gates: +20/-2.
The [finite ledger and delivery](../proposals/m9-xp-sound-blaster-standalone.md#s11-executor-qualification)
record full accounting, hashes, compatible behavior, preliminary/final evidence
and separate actual-change review. Ninja's existing x86 log-recovery warning
caused extra rebuilding; it is not claimed repaired.

ASCII behavior remains; non-ASCII paths previously encoded in a native ANSI
code page must now be UTF-8. POSIX non-UTF-8 filenames remain unsupported.
Console text/title encoding is planned S12, not silently included here.
