# x86 corpus

This package selects C11 without extensions in standalone and embedded builds.
GNU/Clang builds enable -Wall -Wextra -Wpedantic -Werror in this package only.

Architecture-specific chips and Core. Products may use this corpus, but Lib,
Common and Product itself never depend on it.
Core owns guest execution; there is no host worker, Console, host input loop
or product state machine here. SoftPC keeps its existing original executor.

| Component | Responsibility | Public interface |
| --- | --- | --- |
| core | CPU/FPU execution, guest time, memory/port routes and bounded inspection | machine_interface.h and adjacent *_interface.h |
| chips | CPU and individual device register/state mechanisms; board wiring is external | each chip's *_interface.h |

## Build and verification

Keep src/x86, src/common and src/lib as sibling corpora. For example:

```text
cmake -S src/x86 -B build/x86-corpus -DCMAKE_BUILD_TYPE=Release
cmake --build build/x86-corpus
cmake --build build/x86-corpus --target x86-verify
```

MANIFEST.sha256 covers every file with exact LF-normalized SHA-256 values.
x86-verify reuses Common's manifest checker with this corpus root; the x86-owned
source/build gate checks allowed edges and private/platform boundaries. It needs
no importing-product paths. Tests and x86 negative probes live in test/x86.
Each test suite builds independently with its own fixtures and shared tools
directly in test/. No receiving emulator is implemented here.

The complete retained corpus contains chips/{cpu,pit825x,rtc146818,pic8259,
dma8237,fdc8272,hdc,fpu,video,kbc8042,keyboard,ps2mouse,ppi8255,xtkeyboard} and
core. Chips expose copied values and borrowed bus callbacks; Core owns guest
execution/time, not a host worker or product presentation. PC composition is
outside this package. SoftPC does not connect this additional CPU/Core to its
existing executor. The two retained CPU instruction files keep NXVM's inherited
GNU -w setting; other sources select C11 and the strict shared warning profile.
