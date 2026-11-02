# M9 T99 S26: seventh-depth exclusion

Baseline is S25 closure `2bfd64ac`, fixed XP SP1 subset and read-only OpenNT
for diagnostic mapping. The six current candidates and six XP direct-child
candidates correspond. Three existing arms have a single `#error`; three
selected arms were observed in each of six real preprocessing contexts
(three translation units per architecture). No seventh-depth excluded body
required a production change. No eighth-depth contents are assessed here.

Both architecture builds, mirror/documentation gates and full tests passed:
186 non-native and eight serial native tests per architecture, no skips.
Both existing published binaries match the S25 hashes:

- softpc32.exe: `be3c1f1d26766762254947f70fad76180ffb772f5a3b7b7ebeac2549df6584f4`
- softpc64.exe: `1dca80444393d841ccb77849efd32691aa9fc78f423df920c58999916f1b7763`

Production and test source diff: zero. The result covers the current supported
configurations; the remaining selected-frame audit follows this delivery.
