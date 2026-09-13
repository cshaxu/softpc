# M9 T55 S19: narrow lib simplification sweep

This evidence records the owner-approved follow-on cleanup to S18 P1. It keeps
the S18 failure and ownership contract intact while correcting three narrow
implementation seams: stale public create prose, storage status collapsing, and
repeatable startup notifier selection.

The associated admitted design is
[`m9-t55-s19-lib-simplification-sweep.md`](../../../proposals/m9-t55-s19-lib-simplification-sweep.md).

Verification records the focused notifier rejection proof, x64/x86 builds and
full CTest, strict-library CTest, and manifest, dependency and documentation
gates.

## Results

- `ui_window_capture_contract_smoke` proves the first notifier selection
  succeeds and a second selection is rejected without changing the selected
  notifier.
- Fresh x64 and x86 package builds completed; each full CTest run passed
  56/56 tests.
- Strict-library CTest passed 3/3. The manifest, component dependency,
  standalone-source boundary and documentation-governance gates passed.

## Post-P1 independent review

- `git diff --check` passed. The P1 diff has no MVDM, package INI or guest
  media change; only the two explicitly rebuilt package executables changed.
- No `ui_component_mailboxes_set_notify` caller remains. The sole selected
  notifier is Window startup, and the focused smoke proves repeated selection
  is rejected.
- The medium result paths no longer turn a concrete seek/read/write/flush
  failure into a generic I/O status. Remaining platform file I/O returns are
  direct native operation results, not chained result collapse.
