# Win3.1 File Manager VGA width-transition repair

## Observed contract failure

With Window presentation, starting Windows 3.1 File Manager can make the
host KVM Window briefly become roughly twice its normal width and then shrink
again.  This is visibly a presentation-size transition, not an accepted
application resize policy.  Similar historical transitions around Win3.1 DOS
Prompt make it especially important not to solve this by identifying a guest
program or applying a fixed host width.

## Investigation first

1. Reproduce with the documented Win3.1/File Manager path on both x86 and
   x64, recording each published graphics/text frame's dimensions, stride,
   sequence, and display-enable transition without changing guest timing.
2. Trace the producing VGA state through the selected original renderer and
   Compat frame conversion.  Compare the relevant state transition to the
   read-only OpenNT and NTVDM64 mirrors only as source evidence.
3. Establish whether the fault is an original-host renderer defect, a current
   Compat conversion mismatch, or an App/VM frame publication of a transient
   mode.  The answer determines the sole repair owner; no consumer-side
   size-filter, debounce, delay, or File Manager/Win3.1 special case is
   permitted.

## Repair constraints and acceptance

If the source renderer computes an invalid visible geometry, prefer the
smallest source-local correction consistent with original VGA register
semantics and record its pristine-mirror delta.  If Compat exports an
incomplete transition, correct the producer's publish readiness rather than
having KVM Window guess.  Lib and Emulator may be changed only if evidence
shows their documented copied-frame contract itself is wrong.

Acceptance requires stable host Window geometry during File Manager startup,
unchanged legitimate guest mode changes, dual-width targeted proof and manual
Win3.1 verification.  The task also rechecks the existing Win3.1 DOS Prompt
width cases so a shared mode-transition correction does not regress them.
