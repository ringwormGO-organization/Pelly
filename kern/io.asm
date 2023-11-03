
bits     16
section _TEXT class=CODE

extern _ascii_code
_ascii_code:    dw  0

;
;   _move_cursor -> set dh & dl to corresponding position
;
global _move_keyboard
_move_keyboard:
    ; make new call frame
    push bp             ; save old call frame
    mov bp, sp          ; initialize new call frame

    ; save bx
    push bx

    ; Set variables to corresponding position

    ; [bp + 0] - old call frame
    ; [bp + 2] - return address (small memory model => 2 bytes)
    ; [bp + 4] - first argument (row)
    ; [bp + 6] - second argument (column)
    
    mov dl, [bp + 4]
    mov dh, [bp + 6]

    ; Move cursor to corresponding starting position
    mov ah, 0x02
    mov bh, 0x00
    int 0x10

    ; restore bx
    pop bx

    ; restore old call frame
    mov sp, bp
    pop bp

    ret

;
;   _init_keyboard -> set dh & dl to corresponding starting position
;
global _init_keyboard
_init_keyboard:

    ; make new call frame
    push bp             ; save old call frame
    mov bp, sp          ; initialize new call frame

    ; save bx
    push bx

    ; Set cursor type to block
    mov ah, 0x01
    mov cx, 0x10
    int 0x10

    ; Set variables to corresponding starting position
    
    ; [bp + 0] - old call frame
    ; [bp + 2] - return address (small memory model => 2 bytes)
    ; [bp + 4] - first argument (row)
    ; [bp + 6] - second argument (column)

    mov dl, [bp + 4]
    mov dh, [bp + 6]

    ; Move cursor to corresponding starting position
    mov ah, 0x02
    mov bh, 0x00
    int 0x10

    ; restore bx
    pop bx

    ; restore old call frame
    mov sp, bp
    pop bp

    ret

;
;   _asm_keyboard_loop -> get user input (int 0x16),
;                         send ASCII code to C,
;                         and handle there
;
global _asm_keyboard_loop
_asm_keyboard_loop:

    ; Get user input (int 0x16)
    mov ah, 0x00
    int 0x16

    mov [_ascii_code], byte al

    ; Check if input is backspace, enter or cursor
    ; Handle backspace & enter here, if cursor assign special numbers, else just return already assigned ASCII code

    cmp al, 0x08
    je  .backspace

    cmp al, 0x0D
    je  .enterk

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
        ; Go left (0x08)
        mov ah, 0x0e
        mov al, 0x08
        int     0x10

        ; Print space (delete character)
        mov al, ' '
        int     0x10

        ret

    .enterk:
        ; Go to END OF TEXT
        mov ah, 0x0e
        mov al, 0x0a
        int 0x10

        ; Go to CARRIAGE RETURN
        mov ah, 0x0e
        mov al, 0x0D
        int 0x10

        ret

    ; Assign special values to cursor keys
    ; 0 - up
    ; 1 - down
    ; 2 - left
    ; 3 - right

    .up_cursor:
        mov [_ascii_code], byte 0
        ret

    .down_cursor:
        mov [_ascii_code], byte 1
        ret

    .left_cursor:
        mov [_ascii_code], byte 2
        ret

    .right_cursor:
        mov [_ascii_code], byte 3
        ret

;
;   _up_cursor -> move cursor up
;
global _up_cursor
_up_cursor:
    ; Check if current row (top) is 0
    cmp dh, 0
    je  .return

    ; Decrement row, aka move row above
    mov ah, 0x02
    mov bh, 0
    sub dh, 1
    int 0x10

    ret

    .return:
        ret

;
;   _down_cursor -> move cursor down
;
global _down_cursor
_down_cursor:
    ; Check if row (top bottom) is 24
    cmp dh, 24
    je  .return

    ; Increment row, aka move row below
    mov ah, 0x02
    mov bh, 0
    add dh, 1
    int 0x10
    
    ret

    .return:
        ret

;
;   _left_cursor -> move cursor left
;
global _left_cursor
_left_cursor:
    ; Check if current column (top left) is 0
    cmp dl, 0
    je  .return

    ; Decrement columns, aka move left
    mov ah, 0x02
    mov bh, 0
    sub dl, 1
    int 0x10

    ret

    .return:
        ret

;
;   _right_cursor -> move cursor right
;
global _right_cursor
_right_cursor:
    ; Check if current column (top right) is 79
    cmp dl, 79
    je  .return

    ; Increment columns, aka move right
    mov ah, 0x02
    mov bh, 0
    add dl, 1
    int 0x10

    ret

    .return:
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
