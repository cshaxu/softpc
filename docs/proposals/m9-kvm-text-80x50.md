# Bounded 80x50 KVM text frames

## Request and admitted design

Owner: "收口T83，准入T84进行kvm-*组件的文本帧容量升级 80x50".
The earlier request explicitly includes Lib, Common and App-SoftPC end-to-end
delivery. The active packet is in [Current](../states/CURRENT.md).

Support active extents of 1..80 columns and 1..50 rows, including 80x22,
80x25, 80x43 and 80x50. Storage remains bounded with an 80-cell row stride.
Keep complete copied frames and opaque latest-wins mailboxes. Cells retain
their existing glyph/bank/colour contract; Window owns font bitmaps and
Console owns character mappings. Fonts and graphics capacities are unchanged.
This changes shared structure sizes: consumers must rebuild together.

Separate capacity from default layout. The common status page remains 80x25.
Window derives pixels from active rows and font height. In particular, 50 rows
at eight scanlines are 400 pixels, while 50 at sixteen are 800 pixels; text
surface allocation must be verified independently of the graphics height cap.

Logical Console and the native broker must carry the same bounded capacity.
The broker establishes a surface for the active extent, preserves its existing
no-shrink policy, and clears formerly occupied cells when a smaller frame
replaces a larger one. Successful native output must cover the requested
rectangle before its completed-frame cache is updated. A host that cannot
provide the requested surface returns an explicit failure, not clipped success.
Cooked screen ownership/restoration remains with the existing broker.

App-SoftPC copies committed painter dimensions and its existing 80x50 Compat
surface. It must not infer active dimensions from capacity or transient device
registers. Out-of-range dimensions remain unsupported; zero dimensions remain
invalid. Invalid submissions preserve the previous mailbox publication.

## Ordered S plan

S1 is admitted: implement the connected capacity change in one coherent source
delivery, including Lib/Console/broker, Common consumers, App adapter, boundary
and transition tests, manifests, dual builds and background regression.
Keeping the producer and consumers in one S prevents a partially enlarged ABI.
Preliminary estimate: 10--18 production files, +80--180/-50--140 lines;
8--14 test/build files, +180--350/-40--100 lines. These are estimates and will
be replaced with Git counts; mechanical renames are counted explicitly.

S2 is planned: after owner testing, perform the whole-task actual-change and
requirement audit, dispose of task-owned scratch, and close T84 with owner
approval. No speculative second implementation or new state machine is planned.

## Frozen coverage ledger and exit evidence

Each member below must finish with a source disposition and focused test proof:

1. Base text cell storage, extent validation and fixed stride.
2. Window text copy, fonts, rendered pixels, surface allocation and cursor/dirty
   rectangles, including same-pixel-size 25x16 to 50x8 transitions.
3. KVM Console mapping and logical Console validation/storage.
4. Native broker buffer/viewport, completed cache and shrinking active coverage;
   partial writes and host geometry failure must not acknowledge success.
5. Common frame comparison/forwarding and independent 80x25 status layout.
6. App-SoftPC surface-to-frame adapter, last row and actual geometry reporting.
7. Shared tests, product integration tests, manifests and documentation.

Test 22/25/43/50 rows, smallest extents, last cell/cursor, 50-to-25-to-50,
font heights 8/14/16, zero dimensions, 81 columns and 51 rows. Assert no
out-of-bounds access, no stale lower rows and no invalid-frame publication.
Use native API fakes for geometry failures and background-compatible tests;
do not disturb the user's desktop without arranging the native test session.
Run both Release builds, required background suites, shared manifest/DAG gates
and documentation governance. Deliver both EXEs with actual added/deleted/net
counts and disclosed exclusions. Preserve INI, snapshots and guest disks.

Search all text constants and numeric capacity uses across src/lib, src/common,
src/app-softpc and corresponding test roots; classify each as capacity,
active geometry, fixed default, font/pixel calculation or fixture. The original
mirror is read-only in this task. A required mirror algorithm or snapshot
format change stops implementation for a separate scope decision.
