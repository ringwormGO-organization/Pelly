
bits     16
section _TEXT class=CODE

extern _ascii_code
_ascii_code db 0

;
;   _keyboard_asm   -> get user input (int 0x16) cmp. with characters,
;                      return char to C, if backspace handle here.
;
global _c_keyboard
_c_keyboard:
    mov ah, 0x00
    int 0x16

    mov [_ascii_code], byte al

    cmp al, 0x08
    je  .backspace

    ret

    .backspace:
        dec di
        mov byte [di], 0
        dec cl

        mov ah, 0x0e
        mov al, 0x08
        int     0x10

        mov al, ' '
        int     0x10

        ret

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
