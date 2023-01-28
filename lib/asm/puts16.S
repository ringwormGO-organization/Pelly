
    ;   puts16 -> print string function (16-bit)
    puts16:
        push    si
        push    ax

        .loop:

            lodsb
            or  al, al
            jz  .done

            mov ah, 0x0e
            mov bh, 0
            int     0x10

            jmp .loop

        .done:
            pop ax
            pop si
            ret

    ;   Strings
    msg:    db  "===> Pelly Operating System 0.0.1", 0
