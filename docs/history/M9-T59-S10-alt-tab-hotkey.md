# Customer Alt+Tab hotkey

## Original request and boundary

“下一S任务：增加 Ctrl+Alt+T热键，负责往客户机送 Alt+Tab;”

T59 S10 starts from 52dbc21. App keyboard owns registration and interpretation:
register T with Ctrl+Alt as send-alt-tab, then use the existing synthetic input
path to send Alt+Tab while running. Share the Alt+key sequence with CAF; preserve
its initial Ctrl/Alt release and make/break order. Common and Lib stay unchanged.
Cooked monitor has no KVM hotkeys; paused/inactive states inject no guest keys.

## Finite convergence ledger

1. The existing App registry passed to both KVM leaves includes CAT exactly once.
2. Running CAT emits Ctrl-up, Alt-up, Alt-down, Tab-down, Tab-up, Alt-up;
   no T reaches the guest and sink failure stops the sequence. CAF stays intact.
3. Non-running CAT is consumed without input or lifecycle action. The single
   shared help constant documents it for monitor and raw graphical status.
4. Tests cover actual matcher/dispatch, sequence and non-running/failure paths;
   build/package both widths, full regressions, documentation gate and clean push.

No new state, queue, matcher or VM API. Stop if shared or preserved-machine
changes are needed. Search all product registry, dispatcher and help references
for duplicate paths; retain existing lifecycle/mouse hotkeys unchanged.
Completion requires complete executor P then independent review P, both pushed.
T59 remains open and owner manual acceptance is not inferred.

## Implementation and verification record

App adds exactly one registry entry and one running-only dispatch branch. CAF
and CAT share the existing Alt sequence, now a private parameterized helper;
the unused cross-file Alt+Enter declaration is removed, not retained as a wrapper.
The help constant remains shared by monitor help and the raw graphical status.
Composition still passes the one registry through Common UI to both KVM leaves.

The new product-only keyboard test links the real App dispatcher and Lib matcher
against a recording machine-input boundary. It proves one complete CAT press is
consumed without ordinary T/Control/Alt leakage, produces the expected six input
records, and behaves identically in independent matcher instances. It checks
inactive/paused suppression, unchanged CAF output and refusal at every sequence
position without continued delivery. Existing provider/help tests also cover CAT.
No shared source or shared test corpus was modified.

The first test included a repeated T make but expected only one hotkey; the
existing matcher deliberately delivers repeated matched makes. Corrected the
fixture to one physical press, preserving existing repeat behavior rather than
changing Lib. Final focused x64/x86 tests each pass 3/3. This is source/automated
proof, not a claim of interactive Window/raw Console acceptance.

Similar-issue scan: app_keyboard_hotkeys is the only product registry, consumed
by composition; app_keyboard_handle_hotkey remains the only identifier policy;
HELP_HOTKEYS is the only help text. No additional input or dispatch route was added.
Both full builds succeed and all four unchanged corpus manifests verify.

Fixed packages:

- softpc32.exe: 3,489,534 bytes; SHA256
  F29170E7E722451FD321ABC2C0FFD7AAFAB97639749DA001C7AFB944BF3ABBA0.
- softpc64.exe: 2,848,250 bytes; SHA256
  03569B90AE0FD236F919DC9CF10C625079302EA26B26CF6E237729C62DE7FC5E.

Changed-path accounting against 52dbc21, excluding documentation/packages:
three production C/H files add 13/delete 7 lines (net +6); three product test C
files add 108/delete 0. Root CMake adds 10 lines for the isolated keyboard unit
target, which needs no firmware or real machine. Reused existing build trees;
no new recordings or disposable media. User INI remains untouched.

## P1 delivery

Final full x64 91/91 passed in 81.42 seconds; x86 91/91 in 82.38 seconds.
Focused tests are 3/3 each; documentation governance, whitespace and all four
corpus manifest checks pass. Executor reread the original request and verified
all four ledger items. Ready for complete push and independent coordinator
review. S9's reviewed record was retained intact in history, not relabeled as
owner-tested. T59 remains open for owner testing.

## P2 coordinator review

After 8f60ef6 was pushed, switched roles and reviewed the committed App/API and
test/build changes against the original request. T uses the existing registry,
running guard and machine enqueue sink. Alt-key helper preserves CAF's release/
press order, uses scan 0x0f for Tab, stops on a failed sink, and adds no state.
No native focus, lifecycle, shared library or machine implementation changed.
The new test isolates only machine enqueue, not the registry/matcher/dispatcher;
provider and help coverage use their existing production paths. Both 91/91 suites
and unchanged corpus checks cover the bounded delivery. HEAD matched origin/main
and worktree was clean before this review. Accepted for owner testing; T stays open.
