# M9 T63: machine snapshots; S1 design review

Original owner requests and the nine-stage plan are preserved in the
[proposal](M9-T63-machine-snapshots-proposal.md). T62 closed and pushed in
54b2009 before T63 was admitted. Design delivery cf2d979 is pushed.

## S1 request-to-evidence review

| Request | Design and source evidence |
| --- | --- |
| Save a paused installation and load/resume in another process | Explicit paused command matrix; CCPU simulation stack and HLT paths identified as first proof gate, not assumed portable. |
| All runtime state consistent | Proposal enumerates CPU/FPU/SAS/video/input/controllers/q-tic/timer/media; S2 must expand to field-level coverage before implementation can claim completeness. |
| One binary file | One versioned section container, no state sidecars; no raw pointer/struct dump. |
| Direct/readonly omit disk contents | Store references/fingerprints; changed/missing backing files reject load. No silent disk rollback or full-disk copy. |
| Save floppy and hard-disk overlays | Both media owners identified; existing Lib read_at cannot enumerate dirty pages, so S3 needs narrowly scoped copied-page support. |
| Preserve current architecture/experience | Sole executor, VM-owned format, Common-neutral operation/completion, original device state owners; KVM unchanged; load finishes paused with full frame. |
| Product/engineering design and admission | Product limits, failure states, ownership, S1-S9 estimates/proofs and stop conditions recorded; S2 admitted next, not all code claimed done. |

Coordinator reviewed actual cf2d979 changes and source locations: Common
executor/debug rendezvous, c_main NEXT_INST/HLT/simulate, FPU setters, SAS
physical bus access, quick-event pointer queues, GDP slots, timer pending
mailbox and media interfaces. Notably setNpxTagwordReg does not restore tags;
ordinary PAUSED retains a native stack and does not prove timer quiescence.
These are unresolved engineering inputs, not patched defects or successful
restore evidence. Changed DIRECT content cannot be reconstructed without
payload; installation rollback therefore requires OVERLAY.

## Owner boundary revision after S1

Owner subsequently required no Lib changes and only two Common machine-state
read/write operations. This supersedes S1's proposed copied-page Lib support
and broader Common snapshot completion contract. Effective overlay differences
can be computed using existing read_at and restored through open/write_at;
the serialized result need not preserve Lib's internal dirty-page membership.
App owns file I/O, VM owns the state format; Common does not accept paths or
add snapshot events. File publication safety remains an explicit proof gate,
not grounds for quietly expanding Lib. The proposal and active S2 packet now
carry these narrower constraints; no production code was changed.

S1 changes documentation only: production/test C/H delta 0; no EXE rebuild,
media access or user configuration edit. Documentation governance passed
before and after the pushed design delivery; git diff check passed. No runtime
tests or owner snapshot tests are claimed. S1 closes as the bounded design
audit, not as field-complete state enumeration. T63 remains open; S2 is next.
