; Fixed-machine INT 12h service: conventional RAM is 640 KiB.
; This remains ordinary guest firmware, with no host callback.

BITS 16
ORG 0x0500

    mov ax, 0x0280
    iret
