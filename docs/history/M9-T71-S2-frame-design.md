# T71 S2 Design Review

Owner: "好的，接下来准入修复，开始执行。"
Reviewed design commits d1afdbd5 through 85940465 against the final request:
shared text base; leaf-specific resources; Window graphics; transparent FIFO
and latest-wins; VM-owned CP437; S5 fixed limits and explicit failure results.
Superseded per-cell restructuring remains separately queued, not duplicated.
The proposal records S3--S6 bounds, finite coverage and provisional estimates;
S4 preflight must freeze exact upstream/frame layout before implementation.

Actual review: only proposals and Current changed; no code or executable is
claimed. Documentation and whitespace gates pass. S2 design delivery is closed;
S3 begins under Current, and T71 remains open. Existing binaries retain S1
verification (109/109 per width); this is not new runtime testing.
