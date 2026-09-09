# M9 T43 S3 — Fail-closed UX control capacity

## Outcome

The shared component mailbox now accepts an atomic control batch. The fixed
ordinary FIFO holds 32 records; capacity failure returns
`LIB_STATUS_LIMIT_EXCEEDED`, reports it through the component failure sink,
and leaves all queued/requested controls unchanged. `ux-window` uses this for
the two-record disable-mouse then release sequence, preventing a partial
state change.

STOP remains a separate reserved terminal record. It is FIFO, idempotent, and
does not overwrite ordinary control; the reserve prevents a full normal queue
from blocking synchronous component destruction indefinitely.

## Proof

The shared component test covers capacity preservation, failure reporting,
STOP ordering, and atomic two-record rejection. A leaf-level test fills a
Window through `ux_window_set_title` and a separate Console mailbox, proving
their capacities are independent. Fresh x64/x86 package builds each passed
full CTest, 31/31.
