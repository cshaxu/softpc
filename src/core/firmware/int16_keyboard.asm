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
    in al, 0x60
    mov ah, al                  ; scan code; ASCII translation is guest-owned
    xor al, al
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
    mov ah, al
    xor al, al
done:
    pop bp
    pop dx
    pop bx
    iret
