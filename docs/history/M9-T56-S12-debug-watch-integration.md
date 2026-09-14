# M9 T56 S12: debugger watch and command integration

## Admission And Review

The owner admitted complete debug/xasm32 integration and explicitly approved
narrow necessary MVDM edits. Executor delivery `edc7065` was committed and
pushed before coordinator review. The single-agent coordinator reviewed that
commit's complete production/test diff, packet, proposal ledger and final
test logs; S12 is accepted. This is not the separate T56 closure audit.

## Delivered Contract

- Original CCPU operand access notifications only; no instruction, translation,
  guest TF/DR or device algorithm changes. Product driver owns watch/plan state.
- Read/write/execute watch registration, overlap matching, result queries,
  cancellation, reset and command output share the existing executor rendezvous
  and PAUSED event. Debugger inspection does not trigger operand watches.
- Copied bounded access observations support XT; overflow is explicit and does
  not suppress matching. No dormant observe_instruction sink or write-only
  awaiting_pause state remains.
- Real command matrix caught and corrected inline XE's stale continuation;
  malformed G/T/XW parameters no longer mutate their execution state first.
- A/U and XA/XU use the actual imported xasm32; no alternate decoder/executor.

## Evidence And Accounting

Final fixed builds passed x64 63/63 (43.42s), x86 63/63 (44.52s), strict lib
8/8 and documentation/diff gates. Real CPU tests cover scalar/stack/block/REP
access, exception isolation, watches, trace, repeated breakpoints and command
consumers. Shipping package tests exercise Console/Window G/T/XW and then
cold-start DOS. This does not claim manual Win3.1 acceptance.

Against `53c6625`, tracked production (`git diff --numstat -- src`) is eight
files +176/-86, net +90; tests are two files +268/-10, net +258. MVDM changes
in this stage are only `c_page.c` +21/-6, net +15. Shared lib, media and user
INI are unchanged. The finite operation/command ledger and individual boundary
dispositions remain in the [proposal](../proposals/m9-common-corpus-convergence.md).

SHA-256:

- softpc32.exe: `67BAFC9EE9A5BD6552FC423ED09FD47A51A8652BC02297D637F5E145607E4ED0`
- softpc64.exe: `BB3886C042DBF4A6B01DE9EAF2A10C8C707373878D62E40B271662407E249D4F`

Short ignored build/test logs are retained, disposable debugger files were
removed by tests, and no owned test process remains. Architectural CR1/CR4 are
explicitly unsupported. The separate original-x87 arithmetic observation is
recorded in TODO with a baseline-comparison admission condition, not silently
changed or presented as a debugger fix.
