# M9 T99 S24: fifth-depth exclusion delivery

Baseline: S23 closure `5b7a5bee`, pinned XP SP1 source subset
`a2f6d7c93aa4f11efc51147566d5961c59a8ef89`, with OpenNT used to verify
existing diagnostic line mappings. The starting current queue has 94 arms.
The [accounting](m9-t99-s24-depth5-excluded-accounting.tsv) records 40
single-error arms and 54 retained arms; this is not a disposition of the XP
queue or of any sixth-depth descendants.

The only newly excluded body is `base/video/ega_read.c`'s
`EGA_DUMP || EGA_STAT`: neither macro is defined in the supported build
contexts, and its immediately following selected sibling proves the enclosing
parent is reached in both architectures. The original condition remains; its
body is a single error, and `#line 952` restores the OpenNT diagnostic
position after the original closing directive. Existing selected behavior and
other local fixes remain unchanged.

The instrumented preprocessing probe completed 24 actual source compile
contexts and 110 header dependency contexts with no errors. Both architectures
selected all 54 retained arms. A structural scan found exactly one direct
#error and no other content in each of the 40 excluded bodies. Separately,
forcing each of `EGA_DUMP` and `EGA_STAT` reached the intended error on x86
and x64. These are supported-build and explicit guard checks, not proof of
all hypothetical configurations.

Production diff relative to `5b7a5bee`: one mirror source, +2/-1 (net +1);
no test-source change. The mirror identity manifest and exception ledger are
updated; no public interfaces, user INI, guest media or snapshots changed.

The six unmatched XP direct-child candidates remain for the subsequent
retained-frame review, after this exclusion delivery is committed and pushed.

Both architecture builds, XP mirror and documentation gates passed. Full
qualification passed on each architecture: 186 non-native tests at `-j4`
and eight native desktop/audio tests serially, with zero failures. Published
asset SHA-256 values:

- softpc32.exe: `26c4e9d36106e62f2ad9dbdd61d17862f48314884dbd63df12588a9b35f17f65`
- softpc64.exe: `3e211d83b59be86268426387e9a5aba85fb7b44e4f5659b66b6ab096bf8761bc`
