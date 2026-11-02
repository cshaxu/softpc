# M9 T99 S22: third-depth reconciliation

This records the final S22 state after exclusion delivery `23926edf`. S21
closure `97cb254b` is the comparison baseline; the fixed XP SP1 source subset
and read-only OpenNT comparison remain the mirror authorities. No public
component, test source, INI, guest media or snapshot was changed.

Third-depth current-tree accounting covers 600 arms: 256 single-error arms,
322 retained arms observed on both architectures in actual preprocessing
contexts, and 22 restored original selected frames. Seven previously direct
selected arms moved to depth 4 after their original parent frames were restored.
A structural scan found no direct error arm with additional body content.
The retained review also confirms two existing error frames and one existing
local NTVDM-or-standalone condition extension. In `gfx_updt.c`, a NEC condition
previously spanned two functions; the original separate frame boundaries were
restored without changing the selected function code.

The fourth-depth queues contain 309 present arms under retained parents and
326 XP direct-child candidates under 343 mapped parents; these counts overlap.
One local selected frame has no one-to-one XP guard and is recorded in the
parent map for fourth-depth scope review. Queue entries are candidates, not
fourth-depth dispositions; no fourth-depth body was processed here.

Both x86 and x64 final builds and mirror/documentation gates passed. Each
architecture passed all 186 non-native tests and eight serial native desktop
and audio integration tests at `-j4`. Earlier exclusion qualification had one
x64 `emulator.verifier-negative` timeout at its existing 60-second budget; the
isolated retry passed in 18.54 seconds, the full retry passed, and final full
qualification passed. Host contention is a possible explanation, not a proven
root cause. No test or product timeout was changed.

Relative to S21, mirror production changes cover 19 files, +413/-139 lines
(net +274); test source changes are zero. The retained phase alone covers
four files, +112/-1 lines (net +111). They restore conditional scaffolding,
replace excluded bodies and correct boundary line controls. Existing selected
runtime paths and accepted local hardware fixes are preserved. Builds and
tests prove exercised supported configurations; they do not establish guest
behavior for unsupported configurations.

Final published assets:

- softpc32.exe SHA-256 `3D98BEAE70E338A70CAE0DC48972785BD8EC241D1B3BE1923B5A0CF52A28107D`
- softpc64.exe SHA-256 `334C7270BA064AA474AEF9BF9EB72CB9391D9AE4C1268CD4E064BEE7F5EDE553`

Actual-diff review confirms selected source content and required local fixes
were preserved, the split frame follows XP boundaries, and third-depth direct
error bodies contain no nested code. Mirror manifest, selected-frame and
queue source hashes and `git diff --check` pass.
