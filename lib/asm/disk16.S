
    ; lba_to_chs -> converts LBA addrs. to CHS addrs.
    lba_to_chs:
        push    ax
        push    dx

        inc     dx
        mov     cx, dx

        xor     dx, dx
        div     word [bdb_heads]

        mov     dh, dl
        mov     ch, al
        shl     ah, 6
        or      cl, ah

        pop     ax
        mov     dl, al
        pop     ax
        ret

    ; read_disk -> reads sectors from a disk
    read_disk:
        push    ax
        push    bx
        push    cx
        push    dx
        push    di

        push    cx
        call    lba_to_chs
        pop     ax

        mov     ah, 02h
        mov     di, 3

        .retry:
            pusha
            stc
            int     0x13
            jnc     .done

            popa
            call    reset_disk

            dec     di
            test    di, di
            jnz     .retry

        .fail:
            jmp flp_err

        .done:
            popa

            pop     di
            pop     dx
            pop     cx
            pop     bx
            pop     ax

            ret

    ; reset_disk -> resets the disk contrl
    reset_disk:
        pusha
        
        mov     ah, 0
        stc
        int     0x13

        jc  flp_err

        popa
        ret

    ;   Errors
    flp_err:
        mov     si, msg0
        call    puts16

        jmp     .wkey

        .wkey:
            mov ah, 0
            int 16h
            jmp 0FFFFh:0

        .halt:
            cli
            hlt
