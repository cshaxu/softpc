# M9 T99 S21: depth-2 exclusion delivery

This is an intermediate delivery, not S21 closure. The XP SP1 revision and
S20 baseline are recorded in CURRENT. This batch restores 116 direct groups,
replacing conclusively excluded arms with a single `#error` and preserving the
selected code and its diagnostic line controls. Nine prior queue entries now
belong to depth 3 after their parent frames were restored. Of 140 provisional
XP candidates, 99 are represented by these restorations, four have separately
reviewed dispositions, and 37 remain for the retained-arm phase.

A structural scan found 807 direct depth-2 `#error` arms with no other body
content. This is a structure check, not proof of complete XP frame coverage.
Final-source probes observed 128 source arms and three header arms selected in
both architecture contexts; 40 source and 68 header preprocessing contexts
succeeded. Detailed records are in the sibling S21 TSV files.

OpenNT is read-only comparison material; the active mirror baseline is pinned
XP SP1. A preliminary exact alignment of the 116 XP directive locations found
55 matching lines, 60 without exact alignment and one missing OpenNT file.
These are alignment results, not evidence that the remaining groups are
incorrect or have been individually compared.

After final source changes, both x64 and x86 built and passed mirror and
documentation gates. Each architecture passed 186 non-native tests and eight
serial native desktop/audio integration tests. The initial restricted run
failed the cleanup selftest because directory junction creation was denied;
the full unrestricted rerun passed. The two published binaries were:

- softpc32.exe: 49D3BD98D768A50E9DD87F77C1DCF2F10FB525EFC33843C3577AA9419964C765
- softpc64.exe: 01FA6CE27BE28C7BF28F8A43AC9EB80442879F5C99464C21277EE1C037EA3643

The audit found no test or public component changes. Mirror source diff against
S20 is 33 files, +645/-64 lines, net +581. Remaining selected frames, original
coverage and the next-depth queue must be audited before S21 can close.
