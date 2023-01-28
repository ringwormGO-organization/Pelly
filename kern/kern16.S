    [org       0x0]
    [bits       16]

    jmp     _kern16

    %include "lib/asm/puts16.S"

    _kern16:
        mov     si, msgk
        call    puts16

        cli
        hlt

    ;   Strings
    msgk:    db  "===> Pelly Operating System 0.0.3", 0
