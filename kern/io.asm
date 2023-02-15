
bits     16
section _TEXT class=CODE

;
;   _keyboard_asm   -> get user input (int 0x16) cmp. with characters,
;                      print the character.
;
global  _keyboard_asm
_keyboard_asm:

    xor     cl, cl
    
    ._keyboard_loop:
        mov ah, 0x00
        int     0x16

        cmp al, 0x08
        je  .backspace
        
        cmp al, 0x0D
        je  ._keyboard_done

        cmp cl, 0x3F
        je  ._keyboard_loop

        mov ah, 0x0e
        int     0x10

        stosb
        inc cl
        jmp ._keyboard_loop

    .backspace:
        cmp cl, 0
        je  ._keyboard_loop

        dec di
        mov byte [di], 0
        dec cl

        mov ah, 0x0e
        mov al, 0x08
        int     0x10

        mov al, ' '
        int     0x10

        mov al, 0x08
        int     0x10

        jmp ._keyboard_loop
        
    ._keyboard_done:
        mov al, 0
        stosb

        mov ah, 0x0e
        mov al, 0x0D
        int     0x10
        
        mov al, 0x0A
        int     0x10
        
        jmp ._keyboard_loop

;
;   _clear_screen -> clears the screen (80x25 res.), disable blinking
;                    sets the color of the background & foreground
;                    https://en.wikipedia.org/wiki/BIOS_color_attributes
;
global  _clear_screen
_clear_screen:
    
    mov     ax, 0x03
    int         0x10

    ;   Stop blinking (for full 16-colors)
    mov     ax, 1003h
    mov     bx, 0
    int         0x10

    mov     ah, 09h
    mov     cx, 2000d
    mov     al, 20h
    mov     bl, 0x1F
    int         0x10

    ret

;   *****************************************   ;
;               Mouse cursor code               ;
;                                               ;
;   Arrow keys = move                           ;
;   *****************************************   ;
global _keyb_mouse
_keyb_mouse:

    clc
    cli

    mov     ah, 01h
    mov     cx, 07h
    int         10h

    mov     dl, 5d
    mov     dh, 5d

    jmp     _mouse

    _mouse:

        mov ah, 02h
        mov dl, bl
        mov dh, cl
        int     10h

        mov ah, 00h
        int     16h

        cmp ah, 0x48
        je      _up

        cmp ah, 0x50
        je      _down

        cmp ah, 0x4B
        je      _left

        cmp ah, 0x4D
        je      _right

        jmp _mouse

        _up:
            cmp cl, 0h
            je  _mouse

            sub cl, 1
            jmp _mouse

        _down:
            cmp cl, 24d
            je  _mouse

            add cl, 1
            jmp _mouse

        _left:
            cmp bl, 0h
            je  _mouse

            sub bl, 1
            jmp _mouse

        _right:
            cmp bl, 79d
            je  _mouse

            add bl, 1
            jmp _mouse

        jmp _mouse
