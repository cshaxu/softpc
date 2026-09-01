# Documentation Guide

This is the sole documentation entry point.

## Task Reading Set

Before starting or resuming an implementation subtask, every participant reads:

1. this guide;
2. the active [Current](states/CURRENT.md) packet and its subtask brief;
3. [Execution Rules](rules/EXECUTION.md); and
4. [Contributing](../CONTRIBUTING.md).

Then apply these triggers:

- Planning, admitting, or closing a subtask also reads [Queue](states/QUEUE.md)
  and [Roadmap](design/ROADMAP.md). Closing also reads
  [Documentation Rules](rules/DOCUMENT.md).
- Code or build work reads [System Architecture](design/ARCHITECTURE.md),
  [Source Layout](design/CODING.md), [Architecture Rules](rules/ARCHITECTURE.md),
  and [Coding Rules](rules/CODING.md).
- A UX change also reads [Product UX](design/UI.md). Documentation or governance
  work reads [Documentation Rules](rules/DOCUMENT.md) and every authority it
  directly changes.

## Authority Map

- `design/` defines the product, architecture, source layout, UX, and roadmap.
- `rules/` defines enforceable process, architecture, coding, and documentation
  constraints.
- `states/CURRENT.md` is the only active work packet and current baseline.
- `states/QUEUE.md` holds ordered, unnumbered candidate links.
- `states/TODO.md` holds deferred debt only.
- `proposals/` contains the detail for queued candidates.
- `history/` will retain closed numbered implementation-task records.
- `etc/` contains indexed supporting evidence only.

## Daily Operation

[Current](states/CURRENT.md) is the only active-task and technical-baseline
authority. [Queue](states/QUEUE.md) holds ordered, unnumbered candidates. A
candidate receives a numeric task only when the owner admits it. Deferred debt
that is not a planned task belongs in [TODO](states/TODO.md).
