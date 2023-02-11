
bits     16
section _TEXT class=CODE

HW_EQUIP_PS2     equ 4          ; PS2 mouse installed?
MOUSE_PKT_BYTES  equ 3          ; Number of bytes in mouse packet
MOUSE_RESOLUTION equ 3          ; Mouse resolution 8 counts/mm

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

global _mouse_initialize
_mouse_initialize:
    push es
    push bx

    int 0x11                    ; Get equipment list
    test ax, HW_EQUIP_PS2       ; Is a PS2 mouse installed?
    jz ._no_mouse               ; if not print error and end

    mov ax, 0xC205              ; Initialize mouse
    mov bh, MOUSE_PKT_BYTES     ; 3 byte packets
    int 0x15                    ; Call BIOS to initialize
    jc ._no_mouse               ; If not successful assume no mouse
    
    mov ax, 0xC203              ; Set resolution
    mov bh, MOUSE_RESOLUTION    ; 8 counts / mm
    int 0x15                    ; Call BIOS to set resolution
    jc ._no_mouse               ; If not successful assume no mouse

    push cs
    push es                     ; ES = segment where code and mouse handler reside
    
    mov bx, _mouse_callback_dummy
    mov ax, 0xC207              ; Install a default null handler (ES:BX)
    int 0x15                    ; Call BIOS to set callback
    jc ._no_mouse               ; If not successful assume no mouse

    clc                         ; CF = 0 is success
    jmp ._finished
    
._no_mouse:
    stc                         ; CF = 1 is error
    mov     si, noMouseMsg
    call    _mouse_print_string

._finished:
    pop bx
    pop es
    ret

; Function: mouse_enable
;           Enable the mouse
;
; Inputs:   None
; Returns:  None
; Clobbers: AX

global _mouse_enable
_mouse_enable:
    push es
    push bx

    call _mouse_disable          ; Disable mouse before enabling

    push cs
    pop es
    mov bx, _mouse_callback
    mov ax, 0xC207              ; Set mouse callback function (ES:BX)
    int 0x15                    ; Call BIOS to set callback

    mov ax, 0xC200              ; Enable/Disable mouse
    mov bh, 1                   ; BH = Enable = 1
    int 0x15                    ; Call BIOS to disable mouse

    pop bx
    pop es
    ret

; Function: mouse_disable
;           Disable the mouse
;
; Inputs:   None
; Returns:  None
; Clobbers: AX

global _mouse_disable
_mouse_disable:
    push es
    push bx

    mov ax, 0xC200              ; Enable/Disable mouse
    xor bx, bx                  ; BH = Disable = 0
    int 0x15                    ; Call BIOS to disable mouse

    mov es, bx
    mov ax, 0xC207              ; Clear callback function (ES:BX=0:0)
    int 0x15                    ; Call BIOS to set callback

    pop bx
    pop es
    ret

; Function: mouse_callback (FAR)
;           called by the interrupt handler to process a mouse data packet
;           All registers that are modified must be saved and restored
;           Since we are polling manually this handler does nothing
;
; Inputs:   SP+4  = Unused (0)
;           SP+6  = MovementY
;           SP+8  = MovementX
;           SP+10 = Mouse Status
;
; Returns:  None
; Clobbers: None

ARG_OFFSETS      equ 6          ; Offset of args from BP

_mouse_callback:
    push bp                     ; Function prologue
    mov bp, sp
    push ds                     ; Save registers we modify
    push ax
    push bx
    push cx
    push dx

    push cs
    pop ds                      ; DS = CS, CS = where our variables are stored

    mov al,[bp+ARG_OFFSETS+6]
    mov bl, al                  ; BX = copy of status byte
    mov cl, 3                   ; Shift signY (bit 5) left 3 bits
    shl al, cl                  ; CF = signY
                                ; Sign bit of AL = SignX
    sbb dh, dh                  ; CH = SignY value set in all bits
    cbw                         ; AH = SignX value set in all bits
    mov dl, [bp+ARG_OFFSETS+2]  ; CX = movementY
    mov al, [bp+ARG_OFFSETS+4]  ; AX = movementX

    ; new mouse X_coord = X_Coord + movementX
    ; new mouse Y_coord = Y_Coord + (-movementY)
    neg dx
    mov cx, [mouseY]
    add dx, cx                  ; DX = new mouse Y_coord
    mov cx, [mouseX]
    add ax, cx                  ; AX = new mouse X_coord

    ; Status
    mov [curStatus], bl         ; Update the current status with the new bits
    mov [mouseX], ax            ; Update current virtual mouseX coord
    mov [mouseY], dx            ; Update current virtual mouseY coord

    pop dx                      ; Restore all modified registers
    pop cx
    pop bx
    pop ax
    pop ds
    pop bp                      ; Function epilogue

_mouse_callback_dummy:
    retf                        ; This routine was reached via FAR CALL. Need a FAR RET

; Function: poll_mouse
;           Poll the mouse state and display the X and Y coordinates and the status byte
;
; Inputs:   None
; Returns:  None
; Clobbers: None

global _poll_mouse
_poll_mouse:
    push ax
    push bx
    push dx

    mov bx, 0x1164              ; Set display page to 0 (BH) and color red (BL)

    cli
    mov ax, [mouseX]            ; Retrieve current mouse coordinates. Disable interrupts
    mov dx, [mouseY]            ; So that these two variables are read atomically
    sti

    call _mouse_print_word_hex         ; Print the mouseX coordinate
    mov si, delimCommaSpc
    call _mouse_print_string

    mov ax, dx
    call _mouse_print_word_hex         ; Print the mouseY coordinate
    mov si, delimCommaSpc
    call _mouse_print_string

    mov al, [curStatus]
    call _mouse_print_byte_hex         ; Print the last read mouse state byte

    mov al, 0x0d
    call _mouse_print_char             ; Print carriage return to return to beginning of line

    pop dx
    pop bx
    pop ax

    ret
    
; Function: print_string
;           Display a string to the console on the specified page and in a
;           specified color if running in a graphics mode
;
; Inputs:   SI = Offset of address to print
;           BH = Page number
;           BL = foreground color (graphics modes only)
; Clobbers: SI

_mouse_print_string:
    push ax
    mov ah, 0x0e                ; BIOS TTY Print
    jmp .getch
.repeat:
    int 0x10                    ; print character
.getch:
    lodsb                       ; Get character from string
    test al,al                  ; Have we reached end of string?
    jnz .repeat                 ;     if not process next character
.end:
    pop ax
    ret

; Function: print_char
;           Print character on specified page and in a specified color
;           if running in a graphics mode
;
; Inputs:   AL = Character to print
;           BH = Page number
;           BL = foreground color (graphics modes only)
; Returns:  None
; Clobbers: AX

_mouse_print_char:
    mov ah, 0x0e                ; TTY function to display character in AL
    int 0x10                    ; Make BIOS call
    ret

; Function: print_word_hex
;           Print a 16-bit unsigned integer in hexadecimal on specified
;           page and in a specified color if running in a graphics mode
;
; Inputs:   AX = Unsigned 16-bit integer to print
;           BH = Page number
;           BL = foreground color (graphics modes only)
; Returns:  None
; Clobbers: Mone

_mouse_print_word_hex:
    xchg al, ah                 ; Print the high byte first
    call _mouse_print_byte_hex
    xchg al, ah                 ; Print the low byte second
    call _mouse_print_byte_hex
    ret

; Function: print_byte_hex
;           Print a 8-bit unsigned integer in hexadecimal on specified
;           page and in a specified color if running in a graphics mode
;
; Inputs:   AL = Unsigned 8-bit integer to print
;           BH = Page number
;           BL = foreground color (graphics modes only)
; Returns:  None
; Clobbers: Mone

_mouse_print_byte_hex:
    push ax
    push cx
    push bx

    lea bx, [.table]            ; Get translation table address

    ; Translate each nibble to its ASCII equivalent
    mov ah, al                  ; Make copy of byte to print
    and al, 0x0f                ; Isolate lower nibble in AL
    mov cl, 4
    shr ah, cl                  ; Isolate the upper nibble in AH
    xlat                        ; Translate lower nibble to ASCII
    xchg ah, al
    xlat                        ; Translate upper nibble to ASCII

    pop bx                      ; Restore attribute and page
    mov ch, ah                  ; Make copy of lower nibble
    mov ah, 0x0e
    int 0x10                    ; Print the high nibble
    mov al, ch
    int 0x10                    ; Print the low nibble

    pop cx
    pop ax
    
    ret

.table: db "0123456789ABCDEF", 0

align 2
mouseX:       dw 0              ; Current mouse X coordinate
mouseY:       dw 0              ; Current mouse Y coordinate
curStatus:    db 0              ; Current mouse status
noMouseMsg:   db "Error setting up & initializing mouse", 0x0d, 0x0a, 0
tests:        db "Teststrings", 0x0d, 0x0a, 0
delimCommaSpc:db ", ", 0
