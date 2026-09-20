# Common corpus

`src/common` contains shared virtual-machine coordination and explicitly named
x86 debugger/assembler components. It
uses public `lib` contracts and accepts product behavior only through copied
options and injected callbacks.  It never includes app, host, or MVDM source.
Common has no platform directories, platform types, native calls or OS-selected
implementations. Base owns blocking mutex/event/task/wait; Types owns the atomic
vocabulary. Common retains queue, lifecycle and frame-publication ownership.

Machine frame snapshots combine a publication sequence, a typed Window frame
and text-only Console character maps. Copying transfers the active Window
extent and, for text only, both character maps. They specify the expected run
and leave the destination unchanged on rejection. UI publishes the Window
value and builds the small Console text value; it owns neither character
encoding nor a duplicate graphics buffer. Machine compares both maps and
font banks when detecting text changes.
Session treats frame events as publication hints and
derives content, sequence and graphics/text routing from that one snapshot,
which may be newer than the notification. UI owns the shared action vocabulary;
session does not maintain a second action enum or mapping.
UI APIs have one control-thread caller. Immutable creation options are shared
with input callbacks; their run-generation tag uses a Types atomic. Frame
deduplication and object ownership remain control-thread-local. Native producers
must be destroyed before the event sink/context is released.
UI destruction first stops the broker reader and removes its output binding,
then joins the KVM producers. Any failure retains UI and remaining callback
dependencies; the application must not continue freeing them. Constructors
clear a valid output pointer before validating other arguments.

The debug command state is the public opaque debug object itself, with no
separately allocated forwarding owner. Session uses one internal control state
for completed facts and presentation actions; pure derivation stays separate.
Debug's fixed argument pointer table is embedded, with no open/close allocation.
Session forwards TEXT through the same running-only input sink as MOUSE,
without a held-key entry; character support belongs to the machine adapter.
UI submits the fixed graphical Console explanation only when entering that
content kind or creating a fresh Console. Text still follows frame sequences;
only successful submissions update these control-thread-local markers.

Session owns one pending cooked line until its normal/rejected event is consumed
or the broker confirms cancellation/handoff. Frame events never request input.
Provider prompt readiness is level-triggered, not consumed by notification;
text, explicit requests and prompt admission use one result outlet. While editing,
notification text first cancels and joins the reader, discarding the partial line.
A completed queued line remains pending and is still consumed. UI only forwards
that cancellation to its broker; it owns no second reader state or editor.

Debugger output is a growable, object-owned string, borrowed until the next
submit/observe/open/destroy. Its result prompt carries the original input suffix
(address, byte value, flags or colon), not an additional generic continuation
label. Session command results may borrow additional text until the next provider
call. Session consumes it synchronously through its existing monitor transaction,
normalizing LF/CRLF in bounded chunks. No large result is silently truncated;
allocation/formatting and native write failures remain explicit. Providers must
not retain a debug result across a producing call without copying its text.

Linear debug byte ranges must fit the 32-bit address space before access.
XM copies in address-safe direction for overlapping ranges. XS reports only
patterns wholly inside its byte count and accepts linear addresses only.
XU retains its full 32-bit instruction count and stops on decode failure or
address exhaustion, without wrapping the saved next address. XA likewise ends
its input continuation when the last address is consumed. XE/XF retain original
incremental validation: an invalid later byte does not undo earlier writes.

| Component | One responsibility | Public contract |
| --- | --- | --- |
| `machine` | executor, lifecycle/input queues, frame publication, optional paused debug adapter | `machine_interface.h` |
| `session` | one control queue, completed-fact reduction and dispatch | `session_interface.h` |
| interaction owner | monitor logical Console, broker and KVM composition | its root public contract |
| `x86-xasm32` | x86 copied byte/text assembly and disassembly | `xasm32_interface.h` |
| `x86-debug` | x86 debug commands and copied CPU protocol | `debug_interface.h`, `protocol_interface.h` |

`session` calls `machine` and `ui`. `x86-debug` calls `machine` and `x86-xasm32`.
No other common component edge is permitted.  Application and host code may
compose only the root `*_interface.h` contracts; implementation headers and
source files remain component-local. Importing products decide whether to inject
an adapter or expose a command. SoftPC now selects debug through its app CLI
binding; neither component creates a product execution or Console path.
The machine debug contract is synchronous to the control-thread caller and
serviced by the existing paused executor. Disassembly reports instruction byte
count separately from text length; callers must not use text length as a PC step.
Machine copies opaque pointer-free request/response bytes with explicit lengths,
bounded by 128/1536 bytes in its single request slot. It does not interpret CPU
operations. x86-debug's independent protocol header owns the register/address/
operation vocabulary; frontend and product adapter use aligned typed copies.
Driver execution validates protocol-specific sizes and access constraints.
Failures return zero response length without changing caller output bytes.
An unsuccessful completion wait is not proof of quiescence: shut down before
reusing the slot. Existing cancellation clears product plans, not in-flight
requests. The current build still includes x86 components; optional build/test
selection is separate from this neutral Machine ABI.

Machine shutdown synchronously joins its worker and all callbacks without
freeing the machine object. The serialized owner may then release callback
targets and finally destroy the machine/driver; destroy reuses shutdown.
Both return status. Failed shutdown/destroy retains ownership; callback targets
and the driver must remain alive until shutdown succeeds.
Shutdown is permanent and idempotent, unlike the restartable product stop.

## Independent verification

Transfer src/common, src/lib, test/common and test/lib unchanged. No importing
product sources, configuration or resources are needed:

```text
cmake -S test/common -B build/common
cmake --build build/common
cmake --build build/common --target common-verify
ctest --test-dir build/common --output-on-failure
```

Production sources/verification and the external test suite have separate
LF-normalized MANIFEST.sha256 files. Verification rejects missing, extra,
changed or duplicate entries. The source/build DAG gate rejects platform
bypasses and sibling private includes; its negative tests live in test/common.
The source-only build still accepts an explicit COMMON_LIB_ROOT when configured
directly. It does not register or carry test code.
Common synchronization tests run against actual Base primitives without sleeps.
Platform backend availability does not imply complete native display parity.
