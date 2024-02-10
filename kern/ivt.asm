
bits     16
section _TEXT class=CODE

global _set_ivt
_set_ivt:
    mov     al, 0x69
    mov     si, _int_0x69_handler
    call    _load_interrupt

    ret

;
; _load_interrupt:
;   al - int. number
;   si - handler
;
global _load_interrupt
_load_interrupt:
    push    ax

    xor     ax, ax
    mov     es, ax

    pop     ax

    mov     bl, 0x4
    mul     bl
    mov     bx, ax

    mov     word [es:bx], si

    add     bx, 2

    mov     word [es:bx], cs

    ret

;
;   Int. 0x69
;
_int_0x69_handler:
    mov     ah, 0x0e
    mov     al, 'A'
    int     0x10

    iret
