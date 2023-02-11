
bits     16
section _TEXT class=CODE

global  _keyboard_asm
_keyboard_asm:
    
    mov     ah, 0x00
    int         0x16

    mov     ah, 0x0e
    int         0x10

    jmp     _keyboard_asm

    ret

global  _clear_screen
_clear_screen:
    
    mov     ax, 0x03
    int         0x10

    ;   Stop blinking
    mov     ax, 1003h
    mov     bx, 0
    int         0x10

    mov     ah, 09h
    mov     cx, 2000d
    mov     al, 20h
    mov     bl, 0xE0
    int         0x10

    ret
