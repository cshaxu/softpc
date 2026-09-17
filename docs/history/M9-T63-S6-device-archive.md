# M9 T63 S6: private device archive closure

## Scope

S6 completed the frozen private receiver ledger for the later snapshot
transaction. It did not add a public state API, a snapshot file, an App
command, or a Lib change.

## Receiver result

| Receiver family | Final disposition |
| --- | --- |
| PPI/speaker | Fixed guest latch and edge-baseline payload; host audio rebuilds. |
| InPort mouse, 8042 and DOS INT 33h mouse driver | Fixed semantic controller/driver payload; host resources rebuild. |
| Video planes, fonts, DAC, controller registers and C-VID latches | Fixed semantic payload; GDP, DIB and rendering resources rebuild. |
| Serial and parallel virtual endpoints | Fixed controller/carrier payload with semantic delayed-callback IDs. |
| Configured serial/printer output file | Explicit capture rejection; no path, file, handle or external effect is archived. |

The archive never contains a raw legacy structure, native pointer, callback
address, host handle, host pixel surface, UI mailbox, or host-resource object.
Delayed work is restored only through existing semantic queue identifiers.

## Delivery evidence

P2--P9 added focused private archive round trips for every admitted payload
family, including destructive mutation followed by restore. The final P9
delivery is `b88c5c0` and reports sequential full CTest `103/103` at x64 and
x86, plus both package-smoke variants at each width. Fixed packages were
refreshed by P9; their hashes are recorded in the active baseline at that
delivery.

S6 does not claim end-user snapshot functionality. The next bounded step is
S7: Common's two opaque machine-state operations and one VM-owned,
executor-bound safe-boundary transaction.
