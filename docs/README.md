# Documentation Guide

This is the sole documentation entry point.

## Task Reading Set

Before changing work for a subtask, read:

1. this guide;
2. the active [Current](states/CURRENT.md) packet and its brief;
3. [Execution Rules](rules/EXECUTION.md); and
4. [Contributing](../CONTRIBUTING.md).

Then apply the change triggers:

- Planning, admission, or closure also reads [Queue](states/QUEUE.md),
  [TODO](states/TODO.md), and [Roadmap](design/ROADMAP.md).
- Code or build work reads [System Architecture](design/ARCHITECTURE.md),
  [Source Layout](design/CODING.md), [Architecture Rules](rules/ARCHITECTURE.md),
  and [Coding Rules](rules/CODING.md).
- Documentation or governance work reads [Documentation Rules](rules/DOCUMENT.md)
  and every authority it changes. UX work reads [Product UX](design/UI.md).

Read the fixed set on first entry to a subtask and first resume; re-read an
authority when its requirements or the affected surface changes.

## Authority Map

- `design/` owns current product design, source layout, UI, and milestones.
- `rules/` owns enforceable repository process, architecture, coding, and
  documentation constraints.
- `states/CURRENT.md` is the sole active-work and technical-baseline authority.
- `states/QUEUE.md` holds ordered unnumbered candidate links; `states/TODO.md`
  holds deferred debt only.
- `proposals/` holds unnumbered candidate detail; `history/` holds closed
  implementation-task records and retained proposals.
- `etc/` holds only indexed supporting material; see [its index](etc/README.md).

## Daily Operation

One topic has one current authority. Link across boundaries instead of copying
rules, design, status, or plans. Queue is not an active plan and history never
overrides current design or rules.
