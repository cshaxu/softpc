# M9 T99 S23: fourth-depth reconciliation

This records the final S23 state after exclusion delivery `df6c079d`. S22
closure `83caa3f9` is the comparison baseline; the fixed XP SP1 source subset
and read-only OpenNT comparison remain the mirror authorities. No public
component, test source, INI, guest media or snapshot was changed.

Fourth-depth current-tree accounting covers 327 arms: 123 single-error arms,
191 retained arms observed on both architectures in actual preprocessing
contexts, and 13 restored original selected frames. Five previously direct
selected arms moved to depth 5 after original parent frames were restored.
A structural scan found no direct error arm with additional body content.
The retained review also confirms one existing error frame and two existing
selected else frames. Original selected source and local hardware fixes remain.

The fifth-depth queues contain 94 present arms under retained parents and 97 XP
direct-child candidates under 203 mapped parents; these counts overlap. One
local selected frame has no one-to-one XP guard and is recorded in the parent
map for fifth-depth scope review. Queue entries are candidates, not fifth-depth
dispositions; no fifth-depth body was processed here.

Both x86 and x64 final builds and mirror/documentation gates passed. Each
architecture passed all 186 non-native tests and eight serial native desktop
and audio integration tests at `-j4`. The 13 restored frames were checked
against their original conditions and matching post-boundary XP line controls.

Relative to S22, mirror production changes cover eight files, +124/-1 lines
(net +123); test source changes are zero. The retained phase alone covers
four files, +65/-0 lines. The changes restore conditional scaffolding, replace
excluded bodies and preserve boundary line controls. Existing selected runtime
paths and accepted local hardware fixes are preserved. Builds and tests prove
exercised supported configurations, not unsupported guest configurations.

Final published assets:

- softpc32.exe SHA-256 `69112D55DD3EFC11713EA6CE965B1E09264AC938DF7B6F4D5F48743ECDF623DB`
- softpc64.exe SHA-256 `6C2ED83D4748EE76CC6ECF56C819099499EFC50892C76879F4FB7640AC179A0D`

Actual-diff review confirms retained source and local fixes were preserved and
fourth-depth direct error bodies contain no nested code. Mirror manifest,
selected-frame and queue source hashes, and `git diff --check`, passed.
