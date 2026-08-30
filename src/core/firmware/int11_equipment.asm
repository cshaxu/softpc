; Fixed-machine INT 11h equipment list.
; The immediate is patched at reset: 0022h for the color/FPU base machine,
; plus bit 0 when a floppy is attached.

BITS 16
ORG 0x0700

    mov ax, 0x0022
    iret
