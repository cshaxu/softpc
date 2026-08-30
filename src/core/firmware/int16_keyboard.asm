; Fixed-machine INT 16h keyboard service.
; It consumes scan codes from the standalone 8042 model at ports 64h/60h.

BITS 16
ORG 0x0300

int16_keyboard:
    push bx
    push dx
    push bp
    mov bp, sp
    cmp ah, 0x00                ; read key (wait until one is available)
    je read_key
    cmp ah, 0x01                ; test key
    jne done
test_key:
    in al, 0x64
    test al, 0x01
    jz no_key
    and word [bp+10], 0xffbf    ; clear saved ZF for IRET
    jmp short done
no_key:
    or word [bp+10], 0x0040     ; set saved ZF for IRET
    jmp short done
read_key:
    in al, 0x64
    test al, 0x01
    jz read_key
    in al, 0x60
    test al, 0x80                ; discard break codes
    jnz read_key
    cmp al, 0xe0                 ; discard extended prefix
    je read_key
    mov ah, al
    xor bx, bx
    mov bl, al
    mov al, [cs:ascii_table+bx]
done:
    pop bp
    pop dx
    pop bx
    iret

; Unshifted set-1 scan-code translation for console input. Modifier handling
; remains guest firmware territory; this table supplies normal command-line
; typing, control keys, and punctuation.
ascii_table:
    db 0, 27, '1234567890-=', 8, 9
    db 'qwertyuiop[]', 13, 0, 'asdfghjkl;`', 0, '\zxcvbnm,./', 0
    times 128 - ($ - ascii_table) db 0
