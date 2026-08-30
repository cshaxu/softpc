; Fixed-machine INT 1Ah/AH=00h time-of-day tick service.
; The fixed PIT maintains the 32-bit tick counter at BDA 0040:006c.

BITS 16
ORG 0x0600

    push ds
    cmp ah, 0x00
    jne unsupported
    xor ax, ax
    mov ds, ax
    mov dx, [0x046c]
    mov cx, [0x046e]
    xor al, al
    clc
    pop ds
    iret

unsupported:
    mov ah, 0x86
    stc
    pop ds
    iret
