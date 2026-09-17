# M9 T63 S3: existing Storage and media decision

S3 closes with no production source change. The initial claim that snapshot
loading required a new streaming reader was incorrect: an existing readonly
Storage medium already has bounded byte count and exact `read_at` operations.
The App will own its temporary readonly lease while it supplies bounded source
chunks to the later Common machine read/write operation.

The owner also accepts ordinary truncate publication. Saving will use the
existing binary Storage writer directly; if a write fails, the destination can
be partial and any earlier same-path snapshot can be lost. This deliberately
removes the proposed temporary-file/commit API and keeps Lib/test-lib exactly
unchanged.

Media ownership is also fixed without adding a second overlay model:

- Compat's HDD and GFI/FDD adapters exclusively retain the live medium leases.
- VM retains the configured paths and shared media mode.
- A later Compat-private export scans an overlay's *effective* bytes through
  its current lease and a readonly base lease. It records only differing
  4-KiB ranges; direct and readonly record metadata/fingerprint only.
- Loading validates paths/fingerprints and prepares replacement leases before
  it changes the live media owner. No Lib page-list access or App native I/O
  is permitted.

This completes S3's file/media design decision. It does not claim a user
command, a container, or any machine-state serializer. The next active slice
is S4 CPU/SAS/RAM state representation.
