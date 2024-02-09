
;
;   TODO: add option for arguments, instead of preset interrupt & handler
;

bits     16
section _TEXT class=CODE

global _load_interrupt
_load_interrupt:

    xor     ax, ax
    mov     es, ax

    mov     al, 0x69
    mov     bl, 0x4
    mul     bl
    mov     bx, ax

    mov     word [es:bx], handler

    add     bx, 2

    mov     word [es:bx], cs

    ret

handler:
    mov     ah, 0x0e
    mov     al, 'A'
    int     0x10

    iret
