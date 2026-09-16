# M9 T61 S4 — One original keyboard table owner

## Executor delivery

Owner requested equivalent minimization of the standalone keyboard branch,
not new input semantics. P1 `82fb169` removes the duplicate prefix from
nt_keycd.c; the four original tables and KeyMsgToKeyCode remain unchanged.
NT-only includes and BIOS conversion remain in the original non-standalone
path. The existing Scan-1 entry shares the original table, without new state,
translation or interface. VM/Compat callers are unchanged.

Production one file +11/-447, net -436; product test one file +34/-0.
Original content difference falls from +449/-0 to +13/-0. Three local hunks
replace two large branch hunks. Checkout line endings are separately recorded
in the [ledger](../etc/evidence/softpc/pristine-divergence-current.md).
Both selected preprocessed token streams match their prior SHA256 exactly.
All 217 table entries are covered by fingerprints and modifier/make-break
checks. Both builds pass and both full suites pass 97/97. Fixed EXEs refreshed.

## Coordinator review

Reviewed actual pushed 82fb169 paths and source diff against the S4 packet:
no original algorithm change, no accidental NT service activation, no second
table/mapper, no shared corpus, INI or media changes. Four table definitions
are each original-identical and unique. Existing full-suite input/hotkey tests
remain green; no new manual RDP claim. Post-commit IRQ/keycode/BOP each pass
3/3 on x64 and x86. Documentation and whitespace gates pass; temporary probe
removed. Reviewed worktree clean and implementation pushed to main.

The pre-existing original equal-size index defect is explicitly transferred
to TODO for separate input-safety admission; it is not silently repaired in
this token-equivalent cleanup or asserted safe. S4's duplication candidates
are all disposed. S4 closes; S5 is admitted for sound-branch assessment.
T61 remains open through the final S6 audit.
