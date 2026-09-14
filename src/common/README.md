# Common corpus

`src/common` is the product-neutral virtual-machine coordination corpus.  It
uses public `lib` contracts and accepts product behavior only through copied
options and injected callbacks.  It never includes app, host, or MVDM source.
Common has no platform directories, platform types, native calls or OS-selected
implementations. Host owns blocking mutex/event/task/wait; Types owns the atomic
vocabulary. Common retains queue, lifecycle and frame-publication ownership.

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
source files remain component-local. Importing products decide whether to inject
an adapter or expose a command. SoftPC now selects debug through its app CLI
binding; neither component creates a product execution or Console path.
The machine debug contract is synchronous to the control-thread caller and
serviced by the existing paused executor. Disassembly reports instruction byte
count separately from text length; callers must not use text length as a PC step.

## Independent verification

Provide the Lib corpus explicitly; no importing product sources/tests are needed:

```text
cmake -S path/to/common -B build/common -DCOMMON_LIB_ROOT=path/to/lib
cmake --build build/common
cmake --build build/common --target common-verify
ctest --test-dir build/common --output-on-failure
```

The complete LF-normalized corpus (including verification and tests) is covered
by MANIFEST.sha256. The verifier rejects missing, extra, changed or duplicate
entries. The source/build DAG gate rejects platform bypasses and sibling private
includes; its negative tests travel inside this corpus. Lib's own independent
verification remains its owner's responsibility, not a product test dependency.
Common synchronization tests run against actual Host primitives without sleeps.
Platform backend availability does not imply complete native display parity.
