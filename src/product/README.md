# Product

Product owns shared user-facing command policy, debugger/assembly tools and
session/UI composition.  It depends on Lib and Common (renamed Emulator by the
following task), never on the retained x86 CPU or IBM PC hardware components.

The IBM PC machine may consume Product Debug's protocol as an external
integration contract. Product remains the sole owner of the command
implementation, hotkey policy and surface composition.
