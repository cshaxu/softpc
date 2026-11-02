# M9 T99 S25: sixth-depth reconciliation

Baseline: S24 closure `3ae9618d`, pinned XP SP1 subset and read-only OpenNT
for diagnostic line comparison. Exclusion delivery `3e7d69fd` was pushed
after dual-architecture builds and all 194 tests per architecture passed.
The [exclusion report](m9-t99-s25-depth6-excluded.md) records the guard
checks, published asset hashes and the correction to its initial prose count.

Sixth depth contains 14 arms: seven direct single-error and seven selected
in both supported architectures. All seven selected parent directives match
the corresponding XP candidates. No further sixth-depth frames needed
restoration, and production source was unchanged after exclusion delivery.
The original code and selected conditional frames remain intact.

The [current seventh-depth queue](m9-t99-s25-depth7-provisional-queue.tsv)
and [XP direct-child queue](m9-t99-s25-depth7-xp-direct-queue.tsv) each
contain six corresponding arms. These are overlapping candidates, not 12
issues; seventh-depth contents have not been assessed.

The complete source diff relative to S24 is one mirror file +2/-1, net +1;
test source is unchanged. No public API, user configuration, media or snapshot
changed. Since this retained review changed only documentation and queue
records, the exclusion delivery's builds, published binaries and full test
results remain the final production qualification for S25. Supported-build
checks do not prove deliberately excluded configurations.
