; Fixed-machine INT 13h CHS read/write service.
; The ROM is assembled to a byte array embedded by softpc_machine.c.  It is
; deliberately firmware code, not a host callback or service escape.

BITS 16
ORG 0x0100

int13_read:
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    push bp
    push ds
    push ax                       ; retain AH=function and AL=count
    cmp ah, 0x00                  ; controller reset
    je reset
    cmp ah, 0x08                  ; drive parameters
    je parameters
    mov di, bx                    ; ES:DI destination / source offset
    cmp ah, 0x02                  ; read sectors
    je transfer
    cmp ah, 0x03                  ; write sectors
    jne fail
transfer:
    or al, al
    jz fail
    cmp al, 128                   ; standalone ATA PIO transfer limit
    ja fail

    mov ax, cx                    ; CH + CL[7:6] form the cylinder
    xchg al, ah
    and ah, 0xc0
    shr ah, 6
    mov bx, dx
    mov bl, bh                    ; head
    xor bh, bh
    mov si, [cs:heads]
    mov bp, [cs:sectors_per_track]
    mul si                        ; cylinder * heads
    add ax, bx
    mul bp                        ; (cylinder * heads + head) * sectors
    mov bx, cx
    and bl, 0x3f
    dec bx
    add ax, bx                    ; LBA in AX (fixed profile stays < 64K)
    mov bx, ax

    mov dx, 0x1f2
    pop cx
    mov ax, cx
    out dx, al                    ; ATA sector count
    inc dx
    mov ax, bx
    out dx, al                    ; LBA[7:0]
    inc dx
    mov al, ah
    out dx, al                    ; LBA[15:8]
    inc dx
    xor al, al
    out dx, al                    ; LBA[23:16]
    inc dx
    mov al, 0xe0
    out dx, al                    ; master, LBA mode
    inc dx
    cmp ch, 0x03
    je write_command
    mov al, 0x20
    out dx, al                    ; read sectors

    mov dx, 0x1f0
    xor ch, ch
read_sector:
    push cx
    mov cx, 0x100
    cld
read_word:
    in ax, dx
    stosw
    loop read_word
    pop cx
    loop read_sector
    jmp success

write_command:
    mov al, 0x30
    out dx, al                    ; write sectors

    mov dx, 0x1f0
    xor ch, ch
    push es
    pop ds
    mov si, di
write_sector:
    push cx
    mov cx, 0x100
    cld
    rep outsw
    pop cx
    loop write_sector

success:
    pop ds
    pop bp
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    xor ah, ah
    clc
    iret
reset:
    pop ax
    pop ds
    pop bp
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    xor ah, ah
    clc
    iret
parameters:
    pop ax
    pop ds
    pop bp
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    mov ch, 0xff
    mov cl, [cs:sectors_per_track]
    mov dh, [cs:heads]
    dec dh
    mov dl, 1
    xor ah, ah
    clc
    iret
fail:
    pop cx
    pop ds
    pop bp
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    mov ah, 0x01
    stc
    iret

heads:
    dw 16
sectors_per_track:
    dw 63
