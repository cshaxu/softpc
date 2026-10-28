# M9 T99 S15: V7 Pointer Update

The owner accepts and closes S15 and admits S16. Executor deliveries 81a7a096
and 099e2104 and actual-commit reviews dd10399b and 2faad954 were pushed.
The original V7 pointer/background handoff and monochrome palette-only
invalidation now share the existing DIB update and damage owners. The owner
manually verified the initial trail repair; the approved follow-up adds pixel
regression for palette and invalidation. No public component was changed.

Final follow-up builds publish both EXEs and pass 193/193 tests per width,
combining background and reconnected desktop/native groups; two earlier
disconnected-session package failures are explicitly retained in the
[proposal](../proposals/m9-public-handoff-simplification.md#s15-admission-v7-pointerbody-handoff).
The earlier stage-5 package failure and real-guest proof limits also remain
documented there. Task scratch was retired; INI and media remain unchanged.
This closes S15 only; T99 remains open.
