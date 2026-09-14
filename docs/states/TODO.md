# Long-Term Review Ledger

- TODO(Medium): SoftPC CPU owner — T56 S12 disposable FNINIT/FILD qword/FISTP qword probe observed an indefinite integer write after a finite read. The debug observation matches actual stored bytes; arithmetic correctness and pre-hook baseline equivalence were not this debug-ABI task's scope. Admit a separate original-x87 baseline comparison if requested; preserve media and do not alter CPU arithmetic in a debugger adapter.

- TODO(High): SoftPC presentation/compatibility owner — T55 S11 owner narrowed the display failure to Win3.1 MS-DOS prompt fullscreen -> CAF to Window -> Alt+Enter back to fullscreen -> cls leaves stale content. Ordinary DOS cls passes both owner and native-package checks. Owner requests separate handling; admit a mode-roundtrip investigation with this exact sequence, distinguish stale source/frame/host caches, and do not patch CLS or MVDM speculatively.

- TODO(Medium): package acceptance owner — T55 S7 observed one x64 stage-7 CAP-to-monitor timeout after DOS; direct rerun, a full rerun and five consecutive package runs passed. Root cause is not established. Admit a baseline-comparison investigation if it recurs or the owner requests it; preserve the current assertion and do not label the symptom fixed.

- TODO(Low): storage owner — overlay page lookup is O(n) per page. Owner explicitly deferred algorithm changes in T55 S4; admit a measured page-index optimization only when a large dirty-page workload demonstrates a useful gain. Preserve current medium semantics.
