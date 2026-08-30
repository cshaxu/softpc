; Fixed-machine INT 14h serial BIOS service.
; Initialization, transmit, and status are immediately ready; receive times
; out because this fixed profile has no serial input backend.

BITS 16
ORG 0x0900

    cmp ah, 0x02
    je timeout
    cmp ah, 0x00
    je ready
    cmp ah, 0x01
    je ready
    cmp ah, 0x03
    jne timeout
ready:
    mov ah, 0x60
    clc
    iret
timeout:
    mov ah, 0x80
    stc
    iret
