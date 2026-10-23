# M9 T99 S12: Ninja Incremental-Build Integrity

Original request: 准入S12，老规矩. Owner accepts the bounded research result
and closes S12: 批准收口，请你把外部重命名变化一起提交推送。
T99 remains open for a separate completion audit; no new S is admitted.

Research 5d44c2cf and actual-commit review 0108e47e cover historical symptom,
tool/configuration identity, untouched metadata, writers, repeated no-op and
unchanged configure/build behavior. Six initial no-op builds per architecture
take about 0.4..0.5s with zero compile/link edges. A metadata-only timestamp
change to one test source produces exactly its object/link; the relevant test
passes on both widths, and restored full builds again have no work.

Current dependency databases have valid records and exact EOF. Historical
premature-end recovery belongs to .ninja_deps, not the text .ninja_log version.
The damaged historical bytes and writer timeline are unavailable. Owner accepts
no speculative code/tooling fix; this is not a claim that historical corruption
has been causally repaired. The [research report](../proposals/m9-public-handoff-simplification.md#s12-research-and-minimum-disposition)
retains the finite ledger, source references, exact hashes and limitations.

Production, test and build-script diff is zero. Both EXEs retain S11 identities;
no new full runtime qualification is claimed. Documentation, mirror, whitespace
and build-residue gates pass; task scratch is retired and both caches remain.

The owner also explicitly approves the external rename from
assets/binary/snapshot.midi to assets/binary/snapshot-midi. Before staging,
the destination is 18,648,397 bytes and has the same Git blob as the original:
ebb8a85534b6136675e13fbc65bd4f4d8dacdcee. This is a byte-identical rename,
not a newly generated or modified guest payload. No configuration reference
or other medium is changed.
