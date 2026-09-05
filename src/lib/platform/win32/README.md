# Shared Win32 Presentation Component

This directory is a synchronized source component for SoftPC, NXVM, and
NTVDM64.  It is copied and hash-checked by source; it is not a repository,
build, runtime, or acceptance dependency between those projects.

The component owns copied-frame representation and host-side Win32 input,
geometry, and explicit mouse-capture mechanics only.  A project binding owns
guest input encoding, frame publication/lifetimes, product hotkeys,
presentation routing, and lifecycle.
`MANIFEST.sha256` normalizes CRLF to LF and is verified by
`test/support/win32_presentation_manifest.cmake`.
