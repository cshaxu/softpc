# M9 T93 S2 — PCM structural relocation

## Request and boundary

The owner's continued goal is to build shared Lib Audio src/test and then
integrate XP NTVDM-parity audio in standalone SoftPC. S2 is the approved
behavior-preserving move; S3 owns PCM capability additions. Baseline 14092722.

## Executor delivery

P1 7f642848 is pushed to main. Five PCM files (793 lines) move under
src/lib/audio/pcm, and two tests under test/lib/audio/pcm. Public/private
names, guards, includes and the audio-pcm build target follow that owner.
Compat and its existing four App tests use the renamed contract. No alias,
forwarder, second backend, device behavior or XP mirror change is introduced.

The ownership verifier resolves the nested leaf before applying the same
direct-edge and private/platform-header rules. Its full edge matrix remains;
new negative/positive cases protect the nested platform boundary.

Accounting with git diff -M20% --numstat 14092722 7f642848:

| Role | Files | Added | Removed | Net |
| --- | ---: | ---: | ---: | ---: |
| Production C/H | 6 | 166 | 166 | 0 |
| Test C | 6 | 198 | 198 | 0 |
| Production build and verifiers | 4 | 30 | 21 | +9 |
| Test build and verifier cases | 2 | 35 | 9 | +26 |

Docs, manifests and EXEs are excluded from those counts. Test symbol edits
exceed the original estimate, not the scope. All twelve changed C/H bodies
match the baseline after the declared spelling substitutions. SDK aliases
remain unchanged. Stale-path/name sweep finds no old public PCM API/target.

## Verification

- x64/x86 Release builds succeed and publish both package EXEs.
- Each architecture: shared background 90/90, other background 55/55,
  native desktop 5/5; total 150/150. Desktop work used native tests, not
  Computer Use. The owner permits future scheduling without repeated asks.
- Standalone layout self-test, manifests, documentation and diff checks pass.
- x64 build used 45 steps. x86 Ninja recovered an incomplete cache-file ending
  and rebuilt 584 steps; no manual cache reset or duplicate job was performed.
- Initial short layout invocation used a relative root and failed before
  checking sources; corrected absolute-root invocation passed. The first
  sandbox background launcher disappeared before producing a stage log; its
  absence was verified before launching the persistent hidden job.
- No new manual audibility claim; these are structural regression proofs.

Published SHA-256:

- softpc64.exe: 45CAA737DFAC408EE694768DDCB301CEF5A7A77CA3E33A66A9E7DE14D168CF29.
- softpc32.exe: 34A4EB80F0138160E556AD6A25DAA0DFE6CB644DB564C45D7945C4CBA060B96D.

## Coordinator actual-change review

Reviewed the pushed 7f642848 diff, file/rename inventory, public interface,
build registration, ownership-verifier changes, test evidence and original
scope. Production changes are naming-only, tests retain their cases, and the
new verifier handles the admitted nested owner without relaxing boundaries.
The source mirror, Emulator, shared X86, user INI/media/snapshots are unchanged.
HEAD and origin/main matched and the worktree was clean after P1.

S2 is accepted under the owner's sequential execution authorization. S3 may
begin; T93 remains open for the full audio integration and owner T acceptance.
