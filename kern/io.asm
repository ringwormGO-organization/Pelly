
bits     16
section _TEXT class=CODE

extern _ascii_code
_ascii_code db 0

extern _pos_x
_pos_x db 0

;
;   _keyboard_asm   -> get user input (int 0x16) cmp. with characters,
;                      return char to C,
;                           if backspace handle here,
;                           if cursor keys are pressed, assign special value to them and return new value to C
;
global _c_keyboard
_c_keyboard:

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

    ; we are changing value of 'al' register because otherwise value is same for all pressed cursor keys
    .up_cursor:
        mov al, 0
        mov [_ascii_code], byte al

        ret

    .down_cursor:
        mov al, 2
        mov [_ascii_code], byte al

        ret

    .left_cursor:
        mov al, 3
        mov [_ascii_code], byte al

        ret

    .right_cursor:
        mov al, 4
        mov [_ascii_code], byte al
        
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

global _move_cursor_up
_move_cursor_up:
    mov  dl, 0
    mov  dh, 0

    mov  bh, 0
    mov  ah, 02h

    int  0x10
    ret

global _move_cursor_down
_move_cursor_down:
    dec di
    mov byte [di], 1
    dec cl

    mov ah, 0x0e
    mov al, 0x0a
    int     0x10

    ret

global _move_cursor_left
_move_cursor_left:
    dec di
    mov byte [di], 0
    dec cl

    mov ah, 0x0e
    mov al, 0x08
    int     0x10

    ret

global _move_cursor_right
_move_cursor_right:
    mov  dl, 0
    mov  dh, 0

    mov  bh, 0
    mov  ah, 02h
    
    int  0x10
    ret
