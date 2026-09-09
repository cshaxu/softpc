# M9 T47 S3 — Neutral Console broker API

## Outcome

`host_console_broker` is now the public generic host Console interface. Its
caller owns all logical Console objects and supplies an expected Current
Console for replace and cooked-line operations. Host owns only native I/O,
mode binding, transactional replacement, and its existing fail-closed
recovery; it exposes neither monitor nor product-state queries.

The legacy `host_console_cooked` adapter is intentionally retained as a
temporary source-compatible bridge so this independently pushed S3 part stays
runnable. Its one caller is SoftPC's monitor. S4 will migrate that caller to
the public broker and delete the adapter.

## Evidence

- `softpc-host-console-broker-smoke` now includes public expected-current and
  cooked-line assertions and retains coverage of all raw/cooked replacement
  pairs plus failure recovery.
- `src/lib/verify_manifest.cmake` passed.
- Fresh x64 and x86 package builds each passed full CTest, 32/32.

## Closure

This changes no machine source, monitor command semantics, Current-Console
product policy, guest media, or user-owned package configuration. T47 S4 is
active for the one SoftPC monitor migration and final adapter deletion.
