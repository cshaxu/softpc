; Fixed 80x25 color-text INT 10h services. This is guest firmware only:
; it touches B800 text memory and holds state in its ROM data.

BITS 16
ORG 0x0200

int10:
    push bx
    push dx
    push di
    push es
    cmp ah, 0x0e
    je teletype
    cmp ah, 0x0f
    je get_mode
    cmp ah, 0x02
    je set_cursor
    cmp ah, 0x03
    je get_cursor
    jmp done

get_mode:
    mov ax, 0x5003                 ; mode 3, eighty text columns
    xor bh, bh
    jmp done

set_cursor:
    xor ax, ax
    mov al, dh
    mov bl, 80
    mul bl
    xor bx, bx
    mov bl, dl
    add ax, bx
    shl ax, 1
    mov [cs:cursor], ax
    jmp done

get_cursor:
    mov ax, [cs:cursor]
    shr ax, 1
    mov bl, 80
    div bl
    mov dh, al
    mov dl, ah
    xor bh, bh
    jmp done

teletype:
    mov bx, 0xb800
    mov es, bx
    mov di, [cs:cursor]
    cmp al, 0x0d
    je carriage_return
    cmp al, 0x0a
    je line_feed
    mov ah, 0x07
    stosw
    add word [cs:cursor], 2
    jmp constrain_cursor

carriage_return:
    mov ax, [cs:cursor]
    xor dx, dx
    mov bx, 160
    div bx
    mul bx
    mov [cs:cursor], ax
    jmp done

line_feed:
    add word [cs:cursor], 160

constrain_cursor:
    cmp word [cs:cursor], 4000
    jb done
    mov word [cs:cursor], 0
done:
    pop es
    pop di
    pop dx
    pop bx
    iret
cursor:
    dw 0
