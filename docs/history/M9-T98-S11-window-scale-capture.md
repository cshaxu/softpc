# M9 T98 S11 - Window scale and capture

Owner closes S11 on 2026-10-22 and explicitly transfers the reproduced
snap/drag-off exit into new S12. T98 stays open.

Accepted implementation: 1aab6116; actual-diff review: 64706f69. Initial fit,
retained frame-transition scale and actual clipped capture bounds are verified.
Production +40/-3; tests +123/-3. Both architectures have passing evidence for
all 192 registrations; x86 compact package failed once before an unchanged
desktop repeat passed. This is not a claim that its existing intermittency is fixed.

Follow-up owner-run x86 diagnostic proves WM_SIZING with wParam=9 immediately
reaches the geometry default IO_ERROR and component failure. Snap had successfully
changed client 320x536 to 320x200; drag-off restored 304x190 before this failure.
The observed exit is terminal error propagation, not evidence of a scale rejection
or an access violation. The rejection branch predates S11 (07b149ec4).

Temporary tracing and diagnostic EXEs are not a release baseline. S12 removes
them and publishes normal binaries. The still-needed diagnostic scratch is
transferred to S12 until concise evidence promotion and cleanup. Configuration,
guest media and the owner's untracked snapshot remain untouched.
