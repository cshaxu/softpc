# M9 T62 S6: pressed event ledger

Request: [follow-up proposal](../proposals/m9-common-lib-followup-simplification.md).
Baseline 37d4973; implementation db6d534 pushed.

Coordinator inspected actual diff: one source-local copied event replaces the
duplicate source wrapper. Identity comparison, repeat replacement, 256-key
capacity, swap removal and failed-sink return are unchanged. No new matcher,
queue or public API. src/common and src/lib sweep finds no remaining duplicate
pressed-key wrapper; Lib itself did not change.

Production one C path +12/-20 = -8; test one C path +30/-0, from git diff
--numstat 37d4973 db6d534, excluding manifests/docs/artifacts.
Both builds and full suites passed: x64 101/101 (60.24s), x86 101/101 (61.08s).
Post-commit source identity/retirement/failure/manifest/corpus checks 5/5 each.
Both EXEs refreshed; INI/media untouched. S6 closes and S7 begins; T62 open.
