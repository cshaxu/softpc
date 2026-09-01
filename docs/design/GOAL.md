# Product Goal

SoftPC is a direct-launch, single-machine PC virtual machine built around the
recovered original SoftPC machine. It is not an NTVDM host and does not expose
DOS, WOW, BaseSrv, VDD, CSR, session, or product-service behavior.

The executable has no command-line configuration surface. It loads the fixed
`softpc.ini` next to itself, provides one fixed machine configuration, and
offers an NXVM-style monitor console plus an optional Win32 display window.
The configuration selects memory, floppy image, fixed-disk image, their access
mode, and console/window presentation; it does not select machine profiles or
create multiple sessions.

The product succeeds when a user can start, pause, reset, stop, and swap the
configured floppy through the monitor; boot A:, C:, or both safely; use guest
keyboard and mouse input responsively through console, window, or RDP; and
run the selected original BIOS, VGA ROM, controllers, CPU, C-VID, and BOP
firmware boundary on both x86 and x64 Windows hosts.

The product does not succeed by replacing a SoftPC controller, renderer,
BIOS/ROM, or BOP mechanism with a new machine implementation merely to make a
guest boot. New code is confined to portability adaptation, host contracts,
runtime orchestration, and presentation.
