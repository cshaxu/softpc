# M9 T83 S2 Closure Audit

S2 relocates the product tree to `src/app-softpc` and product tests to
`test/app-softpc`; product static gates now live in `tools/checks`.

The audit found four in-scope migration omissions: direct-test include roots,
three `cleanup.h` relative includes, the product-boundary negative fixture, and
the prior S1 history record required by Continuation governance. All were
repaired before final verification; no corrective P is required.

The recovered `softpc.new` corpus is relocation-only. Both Release widths
build, and x64/x86 background CTest plus the product-boundary, source-boundary
and documentation gates pass. Owner INI and media are unchanged.
