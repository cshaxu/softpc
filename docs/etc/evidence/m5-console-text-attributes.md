# M5 Console Original Text Attributes

## Defect

The standalone console copied character bytes from the original `nt_cga`
text surface but wrote them through `WriteConsoleOutputCharacterA`.  That API
does not carry the original PC foreground/background attribute, so every
guest text screen inherited the host console's default colours.  This made
Setup and firmware text appear as a flattened black-background display even
when the original surface contained colour attributes.

## Change

The console endpoint now writes `CHAR_INFO` rows with both the character and
the original attribute through `WriteConsoleOutputA`.  It remembers both
character and attribute snapshots, so an attribute-only update is also
painted.  The window renderer already used those attributes and is unchanged.

This is presentation-only: original `nt_cga` still creates the 4-byte text
cells, runtime still owns the copied snapshot, and no guest RAM, VGA,
controller, BIOS, ROM or BOP behavior is altered.

## Verification

- x64 MinGW serial CTest: `17/17` passed.
- x86 Clang serial CTest: `17/17` passed.
- Both launchers were relinked into `build/output`.

The configured real-media Setup probe reaches the original text-surface
`Welcome to Setup.` stage.  A dedicated diagnostic established that this
particular stage is text mode, not evidence of a missing VGA graphics frame.
