
    [org    0x7c00]
    [bits       16]
    jmp short _preb
    nop

        %include "lib/asm/bsec16.S"

        _preb:
            jmp     _start
            %include "lib/asm/puts16.S"
            %include "lib/asm/disk16.S"

        _start:

            mov     ax, 0
            mov     ds, ax
            mov     es, ax

            mov     ss, ax
            mov     sp, 0x7c00

            mov     si, msg
            call    puts16
            
            mov     ax, 1
            mov     cl, 1
            mov     bx, 0x7e00
            call    read_disk

            cli
            hlt

        ;   Strings
        msg:        db  "===> Pelly Operating System 0.0.2", 13, 10, 0
        msg0:       db  "==> DE-1 | Reboot...", 13, 10, 0

    times   510-($-$$) db 0
    dw      0xaa55
