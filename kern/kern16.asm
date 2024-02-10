
bits 16

section _ENTRY class=CODE

extern _kstart_
extern _set_ivt
global entry

entry:
    cli

    mov     ax, ds
    mov     ss, ax
    mov     sp, 0
    mov     bp, sp
    sti

    mov     ax, 0xFFFE

    call    _set_ivt

    int     0x69

    xor     dh, dh
    push    dx
    call    _kstart_

    cli
    hlt
