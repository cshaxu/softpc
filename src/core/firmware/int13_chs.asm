; Fixed-machine INT 13h CHS service. A: is DL=00h (2 heads, 18 sectors)
; and C: is DL=80h (16 heads, 63 sectors). The ATA drive bit selects the
; corresponding raw-media backend; no product callback is used.

BITS 16
ORG 0x0100

int13:
    push ax
    push bx
    push cx
    push dx
    push si
    push di
    push bp
    push ds
    push ax
    cmp ah, 0x00
    je reset
    cmp ah, 0x08
    je parameters
    mov di, bx
    cmp ah, 0x02
    je transfer
    cmp ah, 0x03
    jne fail
transfer:
    or al, al
    jz fail
    cmp al, 128
    ja fail
    mov ax, cx
    xchg al, ah
    and ah, 0xc0
    shr ah, 6
    mov bx, dx
    mov bl, bh
    xor bh, bh
    mov si, dx
    and si, 0x00ff
    cmp si, 0x80
    je hard_geometry
    or si, si
    jne fail
    mov si, 2
    mov bp, 18
    jmp short geometry_ready
hard_geometry:
    mov si, 16
    mov bp, 63
geometry_ready:
    mul si
    add ax, bx
    mul bp
    mov bx, cx
    and bl, 0x3f
    dec bx
    add ax, bx
    mov bx, ax
    mov dx, 0x1f2
    pop cx
    mov ax, cx
    out dx, al
    inc dx
    mov ax, bx
    out dx, al
    inc dx
    mov al, ah
    out dx, al
    inc dx
    xor al, al
    out dx, al
    inc dx
    mov ax, bp
    cmp al, 63
    jne floppy_select
    mov al, 0xf0
    jmp short drive_ready
floppy_select:
    mov al, 0xe0
drive_ready:
    out dx, al
    inc dx
    cmp ch, 0x03
    je write_command
    mov al, 0x20
    out dx, al
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
    out dx, al
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
fail:
    pop ax
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
reset:
    pop ax
    jmp short success
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
    cmp dl, 0x80
    je hard_parameters
    or dl, dl
    jne fail_result
    mov ch, 79
    mov cl, 18
    mov dh, 1
    mov dl, 1
    jmp short parameters_ready
hard_parameters:
    mov ch, 0xff
    mov cl, 63
    mov dh, 15
    mov dl, 1
parameters_ready:
    xor ah, ah
    clc
    iret
fail_result:
    mov ah, 0x01
    stc
    iret
