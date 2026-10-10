# M9 T86 S1 delivery record: Console handoff

T86 S1 delivered the raw-Console to cooked-monitor handoff work through P10.
Its implementation and review history are retained in the active-task proposal
[Raw Console to cooked monitor handoff repair](../proposals/m9-raw-console-cooked-handoff.md).

The delivery restores the saved cooked Console surface cursor only while that
surface is selected, verifies its backing buffer after extended display
metadata, and leaves reader creation responsible only for the one native
reader.  The same delivery records the later generic startup-preamble ownership
correction and shared test-identity import.

This is a delivery record, not a task-level closure: T86 remains open for its
next admitted subtask.  P10 is `00905b3d`.
