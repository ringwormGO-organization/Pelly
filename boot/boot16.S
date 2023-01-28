
    [org    0x7C00]
    [bits       16]
    jmp     _start
    nop

        %include "lib/asm/puts16.S"

        _start:

            mov     ax, 0
            mov     ds, ax
            mov     es, ax

            mov     ss, ax
            mov     sp, 0x7C00

            mov     si, msg
            call    puts16
            
            hlt

    times   510-($-$$) db 0
    dw      0xaa55
