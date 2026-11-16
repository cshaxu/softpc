# README refresh

Owner requests updating the outdated root README. M9 Td S24 is documentation
only; T100 remains open and S5 remains closed. Verify runtime/audio/snapshot,
configuration and build/test statements against current in-tree sources.
Preserve screenshots, user configuration and binaries. Use one cached Ninja
build route, distinguish background from native integration tests, and link
canonical design/process documentation instead of duplicating governance.
Exit: checked local links/preset names, documentation gate and clean diff;
no runtime rebuild for prose-only changes.

## Completion

Baseline22113cd0. README now names the XP SP1 subset, current PCM/MIDI
routes and snapshot/input commands. It removes old beep/idle-only endpoint
claims and the claim that overlay eagerly copies whole media at startup.
The default INI remains untouched. Cached Build.cmd/preset instructions are
consolidated; background and native desktop/audio suites are clearly separated.
Existing screenshots remain unchanged, without claiming they show every guest.

Evidence sources: tools/Build.cmd, CMakePresets.json, assets/binary/softpc.ini,
product/extensions.c, x86-ibmpc/product/keyboard.c, Lib medium.c, and current
architecture/source-layout/UI authorities. Checked13 local links/anchors,
all literal preset names, LF, whitespace and documentation governance.
No source/binary/test/configuration/guest change; no new runtime pass claimed.
Build residue check passes. Executor and coordinator reviews agree the README
is a user-facing overview, with detailed design/process linked to its owners.
T100 remains open; this documentation request does not reopen S5 or admit S6.

## Retained request packet

## M9 Td S24 Packet

| Field | Required record |
| --- | --- |
| Identifier Mode | Governance |
| Admission And Approval | Owner requests updating outdated README.md. |
| Objective | Accurate concise user overview, run/build/test and feature pointers. |
| Non-goals | No source, binary, media, configuration or product behavior changes. |
| Reference Baseline | 22113cd0. |
| Candidate Proposal | [README refresh](M9-Td-S24-readme-refresh.md). |
| Files And ABI Surface | Root README and governance records; no ABI change. |
| Applicable Rules | docs/README.md reading set, EXECUTION and DOCUMENT. |
| Verification | Compare current source/presets/INI; check links, gate and whitespace. |
| Expected Markers | No stale beep/idle-only claims or misleading full-test command. |
| Asset Needs | Existing screenshots read-only; no build scratch. |
| Reporting Requirements | Summarize corrected statements and actual checks. |
| Stop Conditions | Do not invent unsupported capabilities or external validation. |
| Exit Criteria | Accurate README, passing documentation checks, committed/pushed. |
| Original Owner Request | readme.md 有点过时了，你帮我更新一下。 |
| Similar-Issue Sweep | Review every README section against its current owner. |
