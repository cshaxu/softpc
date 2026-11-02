# M9 T99 S22: third-depth exclusion delivery

This is an intermediate delivery, not S22 closure. S21 closure `97cb254b`
is the source baseline; the XP SP1 revision remains fixed in the mirror
manifest. At this depth, 49 original conditional groups were restored and
seven previously retained bodies were replaced with a single `#error`. Original
selected siblings, including existing standalone hardware fixes, remain in
place. Original guarded local reset-equipment logic was retained.

The current-tree depth-3 scan finds 588 arms: 259 single-error arms and 329
retained arms; no direct error arm has other body content. A probe of 558
actual x86/x64 preprocessing contexts completed with zero failures: 329
retained arms were observed on both architectures. Seven initially unobserved
arms were separately examined in 18 actual contexts. Their parents were
reached and the arms were unselected on both widths; only then were their
bodies replaced. Probe hashes record the source at probe time. All seven
replacement boundaries and retained XP diagnostic line positions were checked.
Six removed bodies matched XP text exactly; the VGA status branch differed
only by prior omission of nested original guards/native sections, and the
whole unselected branch is now one error. No active-path behavior claim is
made for untested configurations.

Both cached architecture builds and mirror/documentation gates passed.
The first x64 full test run passed 185/186; `emulator.verifier-negative`
timed out at its unchanged 60-second budget. It passed its isolated retry in
18.54 seconds. A subsequent complete `-j4` rerun passed all 186 non-native
and eight serial native desktop/audio integration tests per architecture.
Concurrent host load is a possible explanation, not a demonstrated root cause.
No product or test timeout was changed.

Compared with S21, mirror production changes cover 18 files, +301/-138 lines
(net +163); test source changes are zero. The selected paths, local fixes,
public components, configuration, guest media and snapshots are unchanged.
Actual-diff and `git diff --check` review passed.

Published assets:

- softpc32.exe SHA-256 `DE5C715041A83478B26D993B4823D4511CE5D5F06DA419792EEFC7004DE4F8C4`
- softpc64.exe SHA-256 `0CBCF2081CD7344AB1ED7BDD21AB28C02498C9E754498986CC8D598E9CE63061`

Twenty-five of the original 98 unmatched XP child records remain to be
reviewed in the retained phase; this count includes already-existing error
arms and one local condition extension. It is not a count of missing files.
After this delivery is pushed, the retained phase must verify those records,
audit all retained arms, and enqueue only direct depth-4 children before
final S22 build, tests, delivery and closure.
