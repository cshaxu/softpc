; Fixed-machine INT 10h/AH=0Eh teletype service.
; This is guest firmware: it writes directly to B800 text memory and never
; exits into a host callback.

BITS 16
ORG 0x0200

int10_teletype:
    push ax
    push bx
    push dx
    push di
    push es
    cmp ah, 0x0e
    jne done
    mov bx, 0xb800
    mov es, bx
    mov di, [cs:cursor]
    cmp al, 0x0d
    je done
    cmp al, 0x0a
    je done
    mov ah, 0x07
    stosw
    add word [cs:cursor], 2
    cmp word [cs:cursor], 4000
    jb done
    mov word [cs:cursor], 0
done:
    pop es
    pop di
    pop dx
    pop bx
    pop ax
    iret
cursor:
    dw 0
