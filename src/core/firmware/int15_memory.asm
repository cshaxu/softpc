; Fixed-machine INT 15h/AH=88h extended-memory query.
; The machine has 16 MiB total RAM, so 15 MiB is available above 1 MiB.

BITS 16
ORG 0x0800

    cmp ah, 0x88
    jne unsupported
    mov ax, 0x3c00
    clc
    iret

unsupported:
    mov ah, 0x86
    stc
    iret
