# Execution Rules

Only the owner admits, reorders, or closes implementation tasks. Queue entries
are ordered but unnumbered. Admission creates one numeric `T` and one active
subtask `S` in `states/CURRENT.md`; completed task evidence moves to `history/`.

Use `M<milestone> T<task> S<subtask> P<part>: summary` for admitted
implementation commits. Standalone documentation/governance work uses
`M<milestone> Td S<subtask> P<part>: summary` and never allocates a numeric T.

Every implementation subtask states its owner, input/output boundary, focused
verification, full regression requirement, similar-issue sweep, stop
condition, and exit criteria. A runnable-path change requires x64 and x86
build/test evidence. Real media remains local evidence and must never be
mutated unless the active packet explicitly permits its configured mode.

An executor reports a discovered scope conflict rather than silently expanding
an admitted task. Passing one smoke test never closes a task.
