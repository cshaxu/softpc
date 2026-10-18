# Windows 95 Volume Control And Restart Investigation

## Status

Owner admits T97 S1 on 2026-10-17 after T96 acceptance: "works! 好了收口本S和本T，准入下一个T任务".
CURRENT owns its active packet; this proposal retains the investigation plan.
Baseline is 5ea4e55e. S1 findings below distinguish observed guest policy from
an emulator defect; no production repair has been made.
Owner subsequently directs: "本项和队列剩余一项合并调查吧". The queued
safe-shutdown restart investigation is merged into T97 S1, without allocating
another T/S. Combined investigation does not presume a shared cause.

## Owner observation

Startup sound works but Multimedia Properties' playback volume slider and
taskbar volume-control checkbox are disabled. This became the next Queue head
after the previously admitted playback and frame work.

Separately, choosing Yes to restart after sound-driver installation reaches
the safe-to-turn-off screen and stays there. This was originally the third
queued candidate and is now part of the same investigation.

## Investigation and repair boundary

The model reports SB 2.0/DSP 2.01 and contains original CT1335 mixer registers.
PCM playback does not prove driver volume-control exposure. Query installed
driver capabilities and trace mixer exchanges to distinguish an authentic
model/driver limitation from a hardware contract defect. No dynamic causal
proof currently exists. Do not spoof SB16 identity or force UI flags; host
PCM gain alone cannot establish guest mixer capability.

For restart, that screen proves restart did not complete, not which boundary
failed. Existing keyboard-controller/CPU reset paths and T94's successful
observed guest restart do not establish this dialog's behavior. Trace guest
restart intent, reset commands, CPU reset entry and BIOS progress. Compare
normal and installation-requested restart using disposable media/checkpoints.
No automatic reboot on screen detection, Windows-specific branch or assumed
APM diagnosis. Preserve distinct normal-shutdown semantics and check sound
rebinding after a genuine restart.

## Exit evidence

Provide a reproducible capability explanation and minimal repair if defective,
with deterministic regressions and actual guest verification. If new hardware
capability is required, seek approval rather than claim a fix. Use disposable
media and bounded probes, not Computer Use. Dual builds/publication, prescribed
regression, diff review and commit/push precede owner acceptance.

## Admitted investigation plan

- S1: investigate volume controls and restart together, with separate evidence
  and causal conclusions. Distinguish driver/software/model limits from device
  defects, and guest shutdown intent from a failed reset. Return minimal repair
  designs with per-file diff estimates or evidenced limitations requiring direction.
- Subsequent S tasks, subject to findings and admission: implement approved
  corrections separately by responsible boundary unless a common cause is proved;
  focused mocked tests and actual guest verification, cached dual
  builds/publication and required full regression; actual-diff review and push.

S1 does not authorize spoofing SB16, a new mixer model, forcing guest UI flags,
or modifying the user's disk/INI/snapshot. Preserve the existing PCM path.
Native host gain is not proof of a guest-exposed mixer capability. Use
task-owned overlays and bounded probes without Computer Use. Long diagnostics
need owned hidden jobs/logs/exits; observation timeout is not job failure.

## Finite S1 investigation ledger

1. Reproduce disabled controls and working PCM in the same guest state.
2. Identify installed driver/model and advertised volume/mixer capabilities.
3. Trace guest requests through original DSP/mixer registers and Compat.
4. Check volume-control application installation and guest configuration.
5. Reproduce both ordinary restart and installation-requested restart; retain
   normal shutdown as the control case.
6. Trace guest intent, controller reset requests, CPU reset and BIOS progress;
   identify the first divergent stage without inferring it from the screen.
7. Verify subsequent audio rebind separately from the restart result.
8. Classify each symptom's first divergence, responsible owner and minimal change;
   state whether a common cause has evidence or the causes are independent.
9. Specify focused proof, full verification, compatibility effects and diff
   for each proposed correction, including mirror delta when relevant.

Each member needs direct evidence or an explicit limitation. Neither issue is
complete merely because the other is resolved. Scope amendment changes documentation only; no
production/test diff or EXE rebuild is claimed before S1 findings.

## S1 investigated causes

### Volume controls

The restored Win95 guest reproduces the disabled playback volume slider and
taskbar checkbox. Running its installed Sndvol32 reports no active mixer
device and exits. The executable is present: this is not a missing-volume-
application diagnosis. The device property page identifies Microsoft
MSSBLST.DRV/MSSBLST.VXD, version 4.0.950, with the Creative Labs Sound Blaster
device operating normally.

Inspection of those actual installed binaries establishes the capability
predicate, rather than guessing from the friendly device name. The VxD sends
DSP E1, assembles the two returned version bytes, and supplies that version
to the 16-bit driver's device instance. MXDMESSAGE/GETNUMDEVS returns zero
when that version is below 0300h. WODMESSAGE's GETVOLUME and SETVOLUME likewise
return unsupported below 0300h. SoftPC's original sndblst.h deliberately
reports 0201h; nt_sbdsp.c returns those bytes through its existing command
path. The installed driver's predicates therefore reject mixer/volume
capabilities while retaining PCM playback.

The original CT1335 register implementation in nt_sbmxr.c does not change
this driver predicate or constitute a complete SB Pro device. Changing only
the reported DSP version would activate additional driver assumptions; that
would be a hardware-model spoof, not a correct narrow repair. Host PCM gain
cannot independently make the guest enumerate a mixer.

Minimal disposition: no production patch for this symptom under the current
SB2.0 model. Providing this driver's mixer UI would require a separately
approved, complete compatible hardware capability or a demonstrably compatible
guest driver, not a forced checkbox or public Lib change.

### Installation confirmation versus restart

The same guest's Device Manager Change Driver path was exercised with the
existing Creative Labs Sound Blaster selection, without changing hardware
identity. On accepting device properties, Windows displays a hardware-change
notice explicitly asking whether to shut down now. The Chinese prompt says
"要立即关闭系统吗？"; it instructs the user to exit Windows before adjusting
hardware settings. Selecting Yes produces the reported safe-to-turn-off
screen. The trace does not enter the BIOS reboot callbacks in that run.

The ordinary Shut Down dialog's distinct Restart selection is the control:
the trace enters original BIOS reset, completes the existing audio termination
and reinitialization callbacks, and returns to the Win95 desktop. A second
ordinary restart also returns to the desktop and the PCM enqueue probe records
139373 accepted frames after the BIOS reinitialization marker. This proves
post-reset audio submission, not subjective audibility or every sample's
eventual playback. The subsequent ordinary Shutdown selection reaches the
same safe-to-turn-off screen without another BIOS reboot. Thus the
observed hardware-change confirmation is a shutdown request, not a restart
request misinterpreted by SoftPC. Do not add an automatic reset at the
safe-to-turn-off screen: that would destroy legitimate shutdown semantics.

This reproduces the reported end screen through the same installed driver's
hardware-change flow. It is not a recording of the owner's original click,
nor proof that every possible installer dialog uses this wording. A different
dialog explicitly requesting restart would need its own intent/reset trace;
no such failing restart has been established here.

### Evidence scope and proposed change size

Evidence is task-owned x64 snapshot replay, guest key enqueue and direct frame
capture, installed-driver inspection, and bounded linker observation of the
existing BIOS/controller/audio paths. No Computer Use, guest UI bypass,
original media/configuration change, production instrumentation or public
component edit was used. The diagnostic overlay and snapshots are disposable.
The two causes are independent: driver capability policy and explicit guest
shutdown intent. Neither is evidence of a new SoftPC regression.

Proposed production/test/mirror delta for the demonstrated symptoms is
**+0/-0, net 0**. No runtime repair, ABI change or EXE refresh is justified by
these findings. No broad x86/full-suite qualification is claimed for this
read-only investigation; the accepted T96 runtime remains the package baseline.
Any follow-up hardware upgrade needs its own design/admission, not an implicit
S2 implementation based on this report.

## S1 nine-item evidence reconciliation

1. Disabled controls reproduced in the owner's restored guest; startup and
   shutdown PCM submissions observed in the same overlay lineage. The owner
   already reported audible startup. No new listening claim is made.
2. The selected Sound Blaster device and installed driver files were observed
   in Device Manager. Actual binary inspection supplies the volume predicate.
3. The VxD's E1 version exchange maps directly to original DSP_GET_VERSION;
   mixer enumeration and wave volume reject 0201h before usable mixer control.
   No claim that every mixer I/O was traced, or that CT1335 has full SB Pro
   compatibility, is needed or made.
4. Sndvol32 is installed and reports no active mixer; forcing its tray flag
   cannot create the device it needs.
5. Normal restart completed twice. The driver hardware-change confirmation
   and ordinary shutdown both reached safe-to-turn-off. First-time installation
   itself was not repeated; the observed driver-change confirmation is the
   reproducible case, with the original-click limitation stated above.
6. Explicitly selected Restart enters BIOS; the explicitly worded hardware-
   change Shutdown does not. The first divergence is guest requested action,
   not a demonstrated reset/BIOS failure. The initial noisy A20 trace hit its
   budget and was replaced with reset-only tracing; its missing later I/O is
   not used as negative proof.
7. Second restart verifies post-reset PCM admission. Temporary FIFO-full
   returns accepting zero frames are backpressure, not native device-open
   failures; no unsupported audio capability was reclassified as success.
8. Independent guest driver capability and shutdown policies explain the
   observed cases. There is no supported narrow production fix to apply.
9. Production, tests and source-mirror changes are zero. Documentation checks
   and actual diff review qualify this S1 report, not a new runtime release.
   A future complete mixer-capable model would require separate design,
   approval, deterministic device tests and full x86/x64 guest qualification;
   its scope is not disguised as a one-line version change.

Task-local evidence is retained under build/t97/s1: volume frame-23;
restart frame-9/frame-13; install frame-118/frame-121; final frame-11/frame-30/
frame-36/frame-39 and their bounded logs. Diagnostic jobs ended normally with
shutdown/destroy/vm_destroy and exit code zero. Original binaries, media,
configuration and snapshots remain unchanged. Generated guest binaries and
captures are ignored evidence, not committed fixtures.

Installed binary SHA-256 anchors for reproducibility:
- MSSBLST.DRV: 1DBAD58BC086E5A77BE7D5D26E22327B43C94DA85AB471CAF23F1947C120D35E.
- MSSBLST.VXD: A643A88B1591F2ADF63711C353FA4458A2D4502B3878FF1043265AEA52A125E3.

Relevant binary offsets (file offsets, not runtime addresses): VxD 2c80h
reads DSP version, 2b44h/2b84h forwards it, 2877h stores it; DRV 1e98h copies
that value into instance+6. MXDMESSAGE at 50fah checks it at 51cdh;
WODMESSAGE volume checks are at 81ech and 8228h. These inspected installed
binaries are evidence, not new project source or redistributed assets.
