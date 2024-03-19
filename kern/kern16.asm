
bits 16

section _ENTRY class=CODE

extern _kstart_
extern _set_ivt
extern _gdt_info
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

    ;
    ;   Big unreal mode
    ;
    ;   64KiB limit still there
    ;   but if we make this work
    ;   we can modify it to be
    ;   huge unreal mode (which
    ;   will break the code limit
    ;   of 64KiB)
    ;
    cli
    push    ds

    lgdt    [_gdt_info]

    mov     eax,    cr0
    or      al,     1
    mov     cr0,    eax

    mov     bx,     0x08
    mov     ds,     bx

    and     al,     0xFE
    mov     cr0,    eax

    pop     ds
    sti

    xor     dh, dh
    push    dx
    call    _kstart_

    cli
    hlt
