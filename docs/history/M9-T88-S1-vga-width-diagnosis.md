# M9 T88 S1: VGA width-transition diagnosis

The owner accepts diagnostic S1 and admits separate original-mirror repair S2.
S1 is closed as diagnosis, not as a repaired-product acceptance.

## Evidence and attribution

Both x86 and x64 received bounded snapshot/boot observation attempts. The
owner subsequently reproduced native Window width changes with the diagnostic
x64 package. The captured producer-to-consumer ledger contains complete
1280x480 frames followed by 640x480 frames; Machine, UI and Window preserve
those dimensions. A later port-only diagnostic reproduces V7 640-mode
doubling without running Win3.1 by toggling CPU SR4 chain4 alone.

The visible scanout configuration remains unchanged during the captured
failure. Original Host selects a planar painter when CPU chain4 is disabled;
the packed painter's 640-byte visible row is replaced by the planar width
formula yielding 1280. Original derived source-row units change at the same
time. The V7 manual separates CPU chain4 from CRTC accesses. This establishes
an inherited display-path interpretation defect, not a KVM geometry fault.

Compat has a separate incomplete-generation publication gap: hardware-pointer
damage may qualify a newly cleared surface before body reconstruction. A
cleared mode-change flag alone is not sufficient proof of completeness.

## Scope and disposition

No production source or public ABI was changed in S1. Temporary source probe
inserts were removed after creating the diagnostic. The package x64 executable
is diagnostic evidence, not a repaired release; x86 remains unchanged until
S2 refreshes both release executables. Configuration and guest media remain
untouched. No Computer Use was used. One further owner localization opportunity
was consumed; attribution needs no further manual diagnostic run.

The exact original/derived state, limitations, similar-issue disposition and
separate S2/S3 briefs remain in the
[task proposal](../proposals/m9-win31-file-manager-vga-width-transition.md).
Missing FC recalculation and direct CRTC start selection were not established
as causes of this captured failure and are not speculative repair targets.

Documentation governance and diff-whitespace checks pass. This does not claim
S2 pixel/dirty/pointer correctness, production full regression or disappearance
of the owner's intermittent flash; those remain implementation acceptance.
