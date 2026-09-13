# Long-Term Review Ledger

- TODO(High): shared-library lifecycle owner — T55 S17 whole-lib audit identifies asynchronous fault visibility, failed-start ownership, checked broker/task disposal, synchronization and native-side-effect completion gaps. Admit the unified callback/ownership contract in [S17 evidence](../etc/evidence/m9-t55-s17-cursor-global-audit.md) before implementation; include Windows/Linux implementations and all application consumers, not another Window-only patch. G6 phase/text-unit consistency needs its complete renderer proof as recorded there.

- TODO(Medium): package-test owner — T55 S17 both package smokes fail stage 5 with the owner's display=window INI because they unconditionally inspect Console for the DOS prompt. Admit configuration-aware native-package verification without modifying owner configuration or weakening the raw-Console restart assertion; current 55/56 results are not full package acceptance.

- TODO(High): SoftPC presentation/compatibility owner — T55 S11 owner narrowed the display failure to Win3.1 MS-DOS prompt fullscreen -> CAF to Window -> Alt+Enter back to fullscreen -> cls leaves stale content. Ordinary DOS cls passes both owner and native-package checks. Owner requests separate handling; admit a mode-roundtrip investigation with this exact sequence, distinguish stale source/frame/host caches, and do not patch CLS or MVDM speculatively.

- TODO(Medium): package acceptance owner — T55 S7 observed one x64 stage-7 CAP-to-monitor timeout after DOS; direct rerun, a full rerun and five consecutive package runs passed. Root cause is not established. Admit a baseline-comparison investigation if it recurs or the owner requests it; preserve the current assertion and do not label the symptom fixed.

- TODO(Low): storage owner — overlay page lookup is O(n) per page. Owner explicitly deferred algorithm changes in T55 S4; admit a measured page-index optimization only when a large dirty-page workload demonstrates a useful gain. Preserve current medium semantics.
