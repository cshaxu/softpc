# M9 T98 S6 - Text underline and split cursor

Owner accepted and closed S6 on 2026-10-21. Executor delivery is 8e66d797;
actual-change review is 1b95cfd7. T98 remains open.

Window renders explicit underline scanlines and the normalized union of two
cursor spans through the existing pixel/overlay paths. Console intentionally
approximates these with native underline and topmost-span cursor height.
Compat consumes original calculated video state; the former percentage
roundtrip is removed. Four-slot ownership and blink ownership are unchanged.

Production C/H +153/-114, net +39; test C/H +261/-116, net +145. The sole
mirror change is vga_prts.c +4/-0, requesting the existing refresh after an
underline-position change. Shared ABI/source migration is explicit.

Both cached architectures built and both EXEs were published. Each architecture
has 189 distinct final-code passing test records, including targeted supplements:
184 background and five desktop. This was not a single all-green run. Package
attach/prompt intermittency remains in TODO; subsequent passes are not a fix.
Three unchanged native-audio integrations were not rerun. Original cursor-off
bit handling is separately deferred rather than hidden by presentation code.

Published SHA-256:
- softpc32: EA6786D56FC58D8BD873BAFC5E93C7D51F11BD9D12AF583585488373FD53C824
- softpc64: 7D137F481626F4904AD33F634328E006D2C9E6662D4BACBD8AA34FF6EFBA6568

Task scratch was retired after all owned jobs exited; build residue check
passed. User INI/media and untracked snapshot.debug were untouched.
Detailed evidence remains in the T98 proposal's S6 sections.
