# Win95 A: Floppy Drive Identification Repair

## Request And Admission Boundary

Owner: "队列第二位追加一个T任务，用于修复win95把软驱A盘识别为
“可移动磁盘”而不是软驱的故障，Td治理。"

Unnumbered candidate, second in Queue. No numeric T, implementation or runtime
investigation is admitted by this document. The reported symptom is not yet
independently reproduced; root cause remains unknown.

## Product Goal

Windows 95 should identify the emulated A: device as the configured floppy
drive, not present a generic removable-disk identity in its place. Establish
whether the difference is device classification, driver selection or only
shell description/icon before deciding a repair. Do not infer a hardware fault
solely from the generic word "removable", and do not patch guest labels.

Preserve DOS/Win3.1 access, empty-drive behavior, insertion/ejection, readonly,
direct and overlay policies, reset retention and snapshot media restoration.

## Investigation And Engineering Plan

1. Freeze x86/x64 baseline and reproduce with disposable media/overlay. Record
   actual Win95 drive properties, driver/device enumeration and A: access with
   media present and absent. Separate cold boot from snapshot restoration.
2. Trace the selected original floppy device, CMOS/BIOS equipment and disk
   service responses, controller behavior and VM/Compat media adaptation.
   Compare with original source and a known-good floppy classification where
   available. Identify the first incorrect contract; hypotheses are not fixes.
3. Repair at that owner with minimal scope. Prefer a general device-contract
   correction over Windows-version tests, shell workarounds or a second device
   implementation. VM/Compat own product/host adaptation; original device logic
   stays MVDM. Any proposed Lib/Common change requires explicit scope review.
4. Add focused regressions for the observed contract and analogous paths.
   Build both EXEs, run full background regressions, then obtain owner Win95
   verification. Reserve desktop interaction explicitly when required.

These are proposed S boundaries (investigation, evidenced repair, acceptance),
not allocated S identifiers. Before each admitted S, report affected files,
estimated additions/removals, original-mirror diff and acceptance evidence;
afterward report actual counts and package links.

## Acceptance And Non-goals

- Proven floppy identification on both widths, with evidence distinguishing it
  from mere wording; successful A: access, no-media handling and media changes.
- Readonly/direct/overlay and snapshot reload retain their established behavior;
  new tests create small disposable images outside committed assets.
- No modifications to owner INI, guest installation or original disk images.
  No speculative capability expansion, guest-specific branch or unrelated
  storage redesign. Native desktop tests must not interrupt owner activity.
- Stop for design review if evidence points to a guest-only configuration issue
  or an unapproved shared ABI change; do not manufacture a host fix.

Reference: [current baseline](../states/CURRENT.md),
[T71 closure](../history/M9-T71-completion-audit.md).
