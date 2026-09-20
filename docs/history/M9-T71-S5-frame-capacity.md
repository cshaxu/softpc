# T71 S5 Fixed Frame Capacity And Failure Results

## Request And Review

Owner required fixed capacity, complete validation, explicit unsupported output,
no silent producer crop, and component-owned validators. Owner then approved
the necessary Compat query changes: "批准修改compat". The
[proposal](../proposals/m9-kvm-text-frame-contract.md) retains preflight,
finite coverage and focused evidence. Baseline 0623dc5e; delivery 193ff7f0 is
pushed. This record closes S5, not T71.

The coordinator reviewed the actual production/test diff against the packet:
Base validates only common text dimensions; Window owns fonts/graphics; Console
owns BMP maps. Logical Console independently validates its explicit output cells.
Rejection precedes mailbox mutation/wake or output callbacks. No old boolean
validator alias remains. Common's driver returns status; its existing valid
field represents no-frame. Failure unwinds the existing executor and reports
ERROR after cleanup. No new thread, state flag, queue or frame storage was added.

Compat reports original controller text geometry separately from backing storage
and preserves font metadata without out-of-range copying. VM no longer clips
text extents or treats unsupported output as no-frame. The actual producer test
proves 80x25, oversized dimensions/fonts and uninitialized geometry. All direct
callbacks and fakes migrated; run-qualified published-frame readers retain their
distinct boolean contract. MVDM, snapshots, guest media and INI are unchanged.

## Accounting And Verification

Reproduce with `git diff --numstat 0623dc5e 193ff7f0`, classified by path/suffix.

| Category | Added | Removed | Net |
| --- | ---: | ---: | ---: |
| Production C/H | 137 | 59 | +78 |
| Test C/H | 139 | 17 | +122 |
| Four manifests | 23 | 23 | 0 |
| Task documents | 108 | 4 | +104 |
| Build/gates | 0 | 0 | 0 |

Both Release builds pass. Final x86 full suite: 110/110, 100.87 seconds;
subsequent isolated x64: 110/110, 71.06 seconds. These include snapshot,
restart, package, copied input, four manifests and dependency checks.
Documentation governance and whitespace checks pass. No fresh manual Windows
guest acceptance or native Linux presenter validation is claimed.

EXE sizes: x86 3655213 bytes (+1095); x64 3058756 (+70). Hashes and detailed
test dispositions are retained in the proposal. S4 frame layouts/storage counts
are unchanged. No disposable diagnostic directory was created for S5.

S6 is automatically admitted under the owner's serial plan. Existing unrelated
Queue and cell-attribute proposal worktree edits are preserved, not staged in
this delivery; the S5 implementation paths match the pushed commit.
