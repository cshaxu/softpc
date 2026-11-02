# M9 T99 S28: ninth-depth exclusion audit

Baseline is S27 closure `0fdf5f0a` and fixed XP SP1 subset, with read-only
OpenNT for diagnostic comparisons. The two current and corresponding XP
ninth-depth arms are `#ifdef A3CPU` and its `#else` in
`base/video/ega_writ.c`. The first already contains exactly one error; the
else was observed in actual x86 and x64 preprocessing. No excluded body or
production source required alteration. No tenth-depth body is assessed here.

Both architecture builds, mirror/documentation gates and full tests passed:
186 non-native and eight serial native tests per architecture, no skips.
Both published binaries remain unchanged from S25, SHA-256:

- softpc32.exe: `be3c1f1d26766762254947f70fad76180ffb772f5a3b7b7ebeac2549df6584f4`
- softpc64.exe: `1dca80444393d841ccb77849efd32691aa9fc78f423df920c58999916f1b7763`

Production and test-source diff are zero. Selected-frame audit follows this
delivery; checks establish only the supported configuration behavior.
