# M9 T99 S21: second-depth reconciliation

This records the final S21 source state following exclusion delivery
`960b019e`. The active source reference is the pinned XP SP1 subset identified
in CURRENT; OpenNT is read-only comparison material. No public component,
test source, INI, guest media or snapshot was changed.

The current second depth has 1,549 arms. The structural scan reports 796
single-`#error` arms and zero such arms with additional body content. The
other 753 arms are accounted in `m9-t99-s21-depth2-accounting.tsv`: 663 map
to previously recorded or refreshed supported-context observations; 39 are
restored selected frames; 51 are retained siblings of excluded groups. The
40-frame selected ledger includes 39 restorations and one existing frame.
This is current-tree accounting; it does not assert functionality in an
unsupported configuration.

Comparison against the XP subset mapped 746 selected parents to an original
second-depth arm: 673 exact, 40 explicitly anchored, 31 by original condition
group line, and two existing local condition extensions. Seven selected
local frames have no one-to-one original guard. Their entries remain in the
XP-parent map for local-scope review during depth 3. The resulting depth-3
queues contain 518 present arms and 596 XP direct-child arms; the two counts
are not additive, and no depth-3 branch has been judged here. All 518 present
arms have a retained parent in the S21 accounting.

Both x86 and x64 final builds and mirror/documentation gates passed. On each
architecture, all 186 non-native and eight serial native desktop/audio
integration tests passed. The initial restricted test environment denied
junction creation in the cleanup selftest; the complete unrestricted rerun
passed. After the last line-control corrections the full suite was rerun
again. Final-source probes passed 40 source contexts and 68 header contexts;
128 source and three header arms were observed selected in both architectures.

Relative to S20 (`36be4a7a`), mirror production changes total 35 files,
+832/-70 lines (net +762); test source changes are zero. The earlier exclusion
P contains 33 mirror files +645/-64; the retained phase is 17 files +187/-6.
These are conditional scaffolding, excluded-body replacement and diagnostic
line controls; selected executable behavior is intended to remain unchanged.
Dual-architecture builds/tests prove the exercised paths; no claim is made
about guest behavior for unsupported preprocessor configurations.

Published assets after the final build:

- softpc32.exe SHA-256 `23144A44B2E5FF007069E17DDCC4CFC8FCD25DCD45D7A30F47264729E54A8998`
- softpc64.exe SHA-256 `7496204B37D12444D8B57486F32C878085007512E12FDDE620A7316335380B94`

Actual-diff review: the retained paths retain their original selected code,
including the previously accepted InPort, video dirty and timer fixes. Nested
conditions are not processed in this depth; the next-depth queues own them.
Line controls added at the restored boundaries were checked against matching
XP lines. `git diff --check` and the mirror manifest gate pass.
