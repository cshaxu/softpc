# M9 T99 S25: sixth-depth exclusion

Baseline is S24 closure `3ae9618d`, pinned XP SP1 subset and read-only OpenNT
for diagnostic line comparison. The [starting queue](m9-t99-s24-depth6-provisional-queue.tsv)
and corresponding XP queue have 14 entries each. [Accounting](m9-t99-s25-depth6-excluded-accounting.tsv)
records seven single-error and seven selected arms; no seventh-depth contents
are assessed by this record.

Actual x86 and x64 preprocessing of the four affected translation units
completed successfully. The seven retained arms were observed in both
architectures. The newly excluded `EGA_DUMP || EGA_STAT` arm in
`base/video/ega_writ.c` was not observed; its selected sibling established
that its parent was reached. Defining each macro independently caused the
new guard error in each architecture. Its existing selected sibling and nested
conditions are preserved; `#line 463` restores its prior logical line.

Relative to `3ae9618d`, production is one mirror file +2/-1, net +1; test
source is unchanged. Identity and exception ledgers are updated. These checks
prove only the supported build contexts and explicit negative guard cases.

Both x86 and x64 builds and mirror/documentation gates passed. Each
architecture passed all 186 non-native and eight serial native desktop/audio
tests, without skips. Published asset SHA-256 values:

- softpc32.exe: `be3c1f1d26766762254947f70fad76180ffb772f5a3b7b7ebeac2549df6584f4`
- softpc64.exe: `1dca80444393d841ccb77849efd32691aa9fc78f423df920c58999916f1b7763`

Count correction: the initially committed prose transposed the counts as
six excluded and eight selected; the per-arm accounting always contained
seven of each. No branch disposition or test result changed.
