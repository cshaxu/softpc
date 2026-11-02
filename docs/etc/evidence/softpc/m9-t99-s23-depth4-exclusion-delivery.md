# M9 T99 S23: fourth-depth exclusion delivery

This is an intermediate delivery, not S23 closure. S22 closure `83caa3f9`
is the comparison baseline; the pinned XP SP1 subset is the mirror baseline.
Nine original conditional groups with false supported-context arms were restored
with single `#error` bodies, including the two-arm JAPAN/KOREA group. Existing
selected A20, font and video code was preserved. No test source, public
component, user configuration, media or snapshot was modified.

Current-tree scan finds 324 fourth-depth arms: 128 single-error and 196 retained;
no direct error arm contains additional body content. A probe of 160 actual
x86/x64 preprocessing contexts completed without failure. It initially observed
195 of the 196 retained arms on both architectures; the remaining injected
marker fell inside a multiline comment. A corrected two-context probe observed
both its parent and the selected arm on both architectures. Source hashes at
probe time and per-arm observations are in the sibling probe TSV.

Both x86 and x64 builds and mirror/documentation gates passed. Each width passed
186 non-native tests at `-j4` and eight serial native desktop/audio integration
tests. Relative to S22, mirror production changes cover four files, +59/-1
lines (net +58); test-source changes are zero. The selected execution path is
unchanged in supported configurations. `git diff --check` passed.

Published assets:

- softpc32.exe SHA-256 `C31458014885D24E569AE2FF4616BBFBB238FCA5773604C5FBB14130D38894A3`
- softpc64.exe SHA-256 `88EBFF1451944BCF23C3BAFA428A7CDA9E1699241756DA4090216C61F6D5D847`

Sixteen of 33 original unresolved XP child records remain for retained review;
this count includes existing frames and selected candidates. After this phase is
pushed, S23 must review them and the retained arms, enqueue direct depth-5
children, and complete final build, tests, delivery and closure.
