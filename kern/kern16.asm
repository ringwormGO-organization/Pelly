
bits 16

section _ENTRY class=CODE
stack segment STACK 'STACK'

extern _kstart_
global entry

entry:
    cli

    mov     ax, ds
    mov     ss, ax
    mov     sp, 0
    mov     bp, sp
    sti

    xor     dh, dh
    push    dx
    call    _kstart_

    cli
    hlt
