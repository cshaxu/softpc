bits 16
org 0

; Minimal fixed-disk INT 13h / AH=02 read service for the standalone ROM.
; It transfers one CHS sector through the primary ATA PIO task file into
; ES:BX. The core device owns the image; there is no BOP or host callback.
hdd_int13_read:
    push ax
    push bx
    push cx
    push dx
    push di
    mov di, bx

    mov dx, 0x1f2
    mov al, 1
    out dx, al
    inc dx
    mov al, cl
    and al, 0x3f
    dec al
    out dx, al
    inc dx
    xor al, al
    out dx, al
    inc dx
    out dx, al
    inc dx
    mov al, 0xe0
    out dx, al
    inc dx
    mov al, 0x20
    out dx, al

    mov dx, 0x1f0
    mov cx, 256
    cld
.read_word:
    in ax, dx
    stosw
    loop .read_word

    pop di
    pop dx
    pop cx
    pop bx
    pop ax
    xor ah, ah
    clc
    iret
