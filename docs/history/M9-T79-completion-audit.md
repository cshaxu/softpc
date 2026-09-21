# T79 Completion Audit

Owner: “测试通过 可以收口t任务 准入下一个t”。S2 and T79 are closed.

## Scope and convergence

The original request was to repair Win95 A: classification without guest-label
or Windows-specific hacks. Frozen universe: the seven S2 path rows in the
[archived proposal](M9-T79-floppy-drive-identification-proposal.md).

| Requirement | Evidence/disposition |
| --- | --- |
| S1 identify first wrong contract | f33d49c2/f6dadddd and S1 record: GFI conflated hardware with media, both-width probes. |
| S2 initial/insert/eject/reset/destroy | cba189b5: GFI physical type persists independently; five profiles/three modes and empty-command tests. |
| BIOS identity | HDD-only INT11/BDA boot proof; default A: INT13 type/count; no BIOS patch. |
| Snapshot identity | Eight canonical bytes, absent/present-media and both cross-width directions pass; no old-format compatibility promised. |
| Integration | Release both widths; focused 7/7 each; background x64 110/110 (178.87s), x86 110/110 (163.39s). |
| Actual-change review | b9413d65 reviewed the 14 P1 paths against the packet; owner subsequently reports manual testing passed. |
| Boundaries/assets | No shared corpus, Core mirror, INI or guest-media changes. Task scratch removed; no T79 debt transfer required. |

Owner acceptance completes the pending manual criterion. It does not imply
every desktop test or Linux runtime was exercised: five desktop cases per
width were excluded, and low-density BIOS guest-detection coverage remains
as explicitly bounded in the proposal. No additional platform claim is made.

## Endpoint accounting

Rechecked `git diff --numstat 8f59c184..b9413d65 -- src test`:
production +56/-23, net +33; tests/scripts +187/-37, net +150.
Three production and five test/script paths; no second controller, executor,
thread or lifecycle owner. The closure/admission adds no code or binary changes.

Accepted package hashes rechecked at closure:

- softpc32.exe: `A0C1B40B05897F2C14E555808134F1CA1733320693D89618CA912C91019AA829`.
- softpc64.exe: `23B2DA9CF6BA9F8C1904B4F329F9AA76E388079ED373470A938F747B559394CF`.

The proposal is archived, direct links repaired and TODO remains empty.
The already owner-ordered shared-corpus candidate is admitted as T80 S1;
XP rebase remains queued. Previously uncommitted queue/proposal material is
incorporated by this explicit admission, not deleted or silently discarded.
