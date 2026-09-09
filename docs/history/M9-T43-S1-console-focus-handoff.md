# M9 T43 S1 — Native Console focus handoff

## Outcome

The generic Current Console broker now requests native focus only after a
binding has been bound and its raw/cooked reader activated. The Win32 native
Console leaf obtains the process Console window and makes the same best-effort
foreground/focus request already used by Window creation. The product still
requests only a logical Console replacement and contains no Win32 focus call.

## Focused proof

The broker fake records native focus requests. It proves exactly one request
for initial cooked activation, raw replacement, and a successful old-binding
restoration after a failed next activation. It proves no request for prepare
failure or for a next activation that never becomes active.

## Regression

Fresh x64 and x86 package builds each passed full CTest, 30/30. The shared
library manifest was regenerated and verified for the T43 S1 corpus.
