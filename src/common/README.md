# Common corpus

`src/common` is the product-neutral virtual-machine coordination corpus.  It
uses public `lib` contracts and accepts product behavior only through copied
options and injected callbacks.  It never includes app, host, or MVDM source.

| Component | One responsibility | Public contract |
| --- | --- | --- |
| `machine` | executor, lifecycle/input queues, frame publication, optional paused debug adapter | `machine_interface.h` |
| `session` | one control queue, completed-fact reduction and dispatch | `session_interface.h` |
| interaction owner | monitor logical Console, broker and KVM composition | its root public contract |
| `xasm32` | copied byte/text assembly and disassembly | `xasm32_interface.h` |
| `debug` | generic debug command engine using the optional machine adapter | `debug_interface.h` |

`session` calls `machine` and `ui`.  `debug` calls `machine` and `xasm32`.
No other common component edge is permitted.  Application and host code may
compose only the root `*_interface.h` contracts; implementation headers and
source files remain component-local.  `xasm32` and `debug` are dormant generic
capabilities: importing products decide whether to inject an adapter or expose
a command, and neither component creates a product execution or Console path.
