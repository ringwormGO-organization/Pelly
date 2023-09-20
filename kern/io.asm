
bits     16
section _TEXT class=CODE

extern _ascii_code
_ascii_code db 0

extern _row
_row db 0

extern _column
_column db 0

;
; _init_keyboard -> Do init stuff
;
global _init_keyboard
_init_keyboard:
    mov ah, 0x01
    mov cx, 0x10
    int 0x10

    mov cl, 13
    mov bl, 40

;
;   _c_keyboard   ->  get user input (int 0x16) cmp. with characters,
;                       return char to C,
;                         if backspace handle here,
;                         if cursor keys are pressed, assign special value to them and return new value to C
;
global _c_keyboard
_c_keyboard:

    ; Fix this
    mov ah, 0x02,
    mov dh, cl,
    mov dl, bl,
    int 0x10

    mov ah, 0x00
    int 0x16

    mov [_ascii_code], byte al

    cmp al, 0x08
    je  .backspace

    cmp ah, 0x48
    je .up_cursor

    cmp ah, 0x50
    je .down_cursor

    cmp ah, 0x4B
    je .left_cursor

    cmp ah, 0x4D
    je .right_cursor

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

    .up_cursor:
        sub cl, 0x01

        mov ah, 0x02,
        mov dh, cl,
        mov dl, bl,

        int 0x10
        ret

    .down_cursor:
        dec di
        mov byte [di], 1
        dec cl

        mov ah, 0x0e
        mov al, 0x0a
        int     0x10

        ret

    .left_cursor:
        dec di
        mov byte [di], 0
        dec cl

        mov ah, 0x0e
        mov al, 0x08
        int     0x10

        ret

    .right_cursor:
        ; TODO: Fix memory leak

        dec di
        mov byte [di], 0
        dec cl

        mov ah, 0x0e
        mov al, 0x00
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
