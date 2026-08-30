bits 16
org 0

; Fixed-profile INT 13h / AH=02 service for the standalone ROM.  It transfers
; one CHS sector through the primary ATA PIO task file into ES:BX.  DL 00h
; uses the 1.44MB geometry (2 heads, 18 sectors); DL 80h uses the fixed-disk
; geometry (16 heads, 63 sectors).  The core device owns the image; there is
; no BOP or host callback.
hdd_int13_read:
    push ax
    push bx
    push cx
    push dx
    push di
    push bp
    cmp ah, 0x02
    jne .error
    cmp al, 1
    jne .error
    mov di, bx

    ; LBA = ((cylinder * heads + head) * sectors) + sector - 1.
    mov ax, cx
    xchg al, ah
    and ah, 0xc0
    shr ah, 6
    mov bx, dx
    mov bl, bh
    xor bh, bh
    mov bp, dx
    and bp, 0xff
    cmp bp, 0x80
    jb .floppy_geometry
    mov si, 16
    jmp short .geometry_ready
.floppy_geometry:
    mov si, 2
.geometry_ready:
    mul si
    add ax, bx
    cmp bp, 0x80
    jb .floppy_sectors
    mov si, 63
    jmp short .sectors_ready
.floppy_sectors:
    mov si, 18
.sectors_ready:
    mul si
    mov bx, cx
    and bl, 0x3f
    dec bx
    add ax, bx
    mov bx, ax

    mov dx, 0x1f2
    mov al, 1
    out dx, al
    inc dx
    ; ATA LBA28 low and middle bytes.  The standalone fixed profile does not
    ; expose images large enough to need the upper bytes.
    mov ax, bx
    out dx, al
    inc dx
    mov al, ah
    out dx, al
    inc dx
    xor al, al
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

    pop bp
    pop di
    pop dx
    pop cx
    pop bx
    pop ax
    xor ah, ah
    clc
    iret

.error:
    pop bp
    pop di
    pop dx
    pop cx
    pop bx
    pop ax
    mov ah, 1
    stc
    iret
