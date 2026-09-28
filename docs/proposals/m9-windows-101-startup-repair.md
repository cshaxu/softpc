# Windows 1.01 startup diagnosis and repair

## Product observation

With `O:\assets\softpc-win95\softpc.ini` and its selected `hdd.img`, a
fresh SoftPC start reaches DOS, but entering `win` does not show the Windows
1.01 startup screen or reach an interactive desktop.

## Objective

Find the first observable machine/host contract violation on this reproducible
Windows 1.01 route, repair it at its actual owner, and prove that `win` reaches
an interactive Windows 1.01 session without regressing existing background
tests or the supplied Windows 95 configuration.

## First implementation-step boundary

S1 first establishes a non-mutating reproduction and a finite execution/video
trace around `win`.  It then admits only the smallest causal repair supported
by that evidence.  If the failure needs a broader machine/device semantic
change, S1 records the evidence and stops for an owner-approved follow-on S
rather than guessing at a guest-version workaround.

## Ownership constraints

- Preserve the recovered `src/app-softpc/softpc.new` source baseline.  A direct
  mirror edit is permitted only for a narrow, source-visible mechanical/ABI
  correction under the coding rule; functional host behavior belongs in
  `app-softpc/compat` or `app-softpc/machine`.
- Do not add a Windows-1.01-specific product branch.  The repaired contract
  must be generally valid for the original controller/renderer path.
- Lib/Common/x86 remain unchanged unless evidence shows their public contract
  itself is the failed boundary; such expansion stops S1 for approval.
- The supplied `O:\assets\softpc-win95\softpc.ini`, `hdd.img`, snapshot and
  other media are read-only test inputs.  Disposable copies/overlays belong
  under the task build tree.

## Delivery convention

Every repaired build refreshes `assets/binary/softpc32.exe` and
`assets/binary/softpc64.exe`, and copies the latter to
`O:\assets\softpc-win95\softpc64.exe`.  The extra copy is a test deliverable,
not a tracked repository asset; no supplied INI or media is rewritten.

## Verification

- Reproduce from a byte-for-byte disposable copy of the supplied INI/HDD.
- Capture enough bounded evidence to identify the failing owner before editing.
- Add focused regression proof at the owning product boundary when a causal
  repair is made.
- Build Release x64/x86; run relevant focused tests and both standard serial
  background CTest presets.  Report desktop/manual verification separately.
- Verify `git diff --check`, documentation governance, and every applicable
  component/mirror gate.

## Completion condition

The original repro reaches an observable interactive Windows 1.01 state using
the supplied configuration, the repair has one owner and no guest-version
special case, package/copy deliverables are refreshed, and all admitted
verification evidence is recorded for owner acceptance.
