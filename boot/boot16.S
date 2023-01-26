
    [org    0x7c00]
    [bits       16]
    jmp     _start
    nop

        _start:
            mov     ah, 0x0e
            mov     al, 65
            int         0x10
            
            jmp     $

    times   510-($-$$) db 0
    dw      0xaa55
