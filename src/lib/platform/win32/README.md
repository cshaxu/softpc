# Shared Win32 Presentation Component

This directory is a synchronized source component for SoftPC, NXVM, and
NTVDM64.  It is copied and hash-checked by source; it is not a repository,
build, runtime, or acceptance dependency between those projects.

The component owns copied-frame representation, latest-frame mailbox/event,
generic input-event queue/wake, RDP-safe Win32 input normalization, registered
host actions, geometry, explicit mouse capture, the complete reusable Win32
window and private-console presenters, and the common `WINDOW`/`CONSOLE`
display router.  A presenter consumes only `win32_presentation_binding`:
opaque copied-frame storage plus callbacks for generic events, actions, state,
and title text.  It cannot name a product runtime or machine.
`win32_presentation_run()` owns the generic router-to-presenter dispatch;
products provide a policy and binding, never their own console/window loop.
A product binding owns machine snapshot production, guest input encoding,
action meaning, configuration syntax, and lifecycle.
`MANIFEST.sha256` normalizes CRLF to LF and is verified by
`test/support/win32_presentation_manifest.cmake`.
