
bits 16

section _ENTRY class=CODE

extern _kstart_
extern _load_interrupt
global entry

entry:
    cli

    mov     ax, ds
    mov     ss, ax
    mov     sp, 0
    mov     bp, sp
    sti

    mov     ax, 0xFFFE

    call    _load_interrupt

    int     0x69

    xor     dh, dh
    push    dx
    call    _kstart_

    cli
    hlt
