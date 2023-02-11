
bits 16

section _ENTRY class=CODE

extern _kstart_
global entry

entry:
    cli

    mov     ax, ds
    mov     ss, ax
    mov     sp, 0
    mov     bp, sp
    sti

    mov     ax, 0xFFFE

    xor     dh, dh
    push    dx
    call    _kstart_

    cli
    hlt
