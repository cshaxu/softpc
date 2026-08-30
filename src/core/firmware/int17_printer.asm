; Fixed-machine INT 17h printer BIOS service.
; The profile presents a selected, ready printer sink and discards output.

BITS 16
ORG 0x0a00

    mov ah, 0x90
    clc
    iret
