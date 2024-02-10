
extern _low_memory
_low_memory: dw 0

extern _used_memory
_used_memory: dw 0

global _get_low_memory
_get_low_memory:
    ; Clear carry flag
    clc
 
    ; Switch to the BIOS (= request low memory size)
    int 0x12
 
    ; The carry flag is set if it failed
    jc .error
 
    ; AX = amount of continuous memory in KB starting from 0.

    mov [_low_memory], byte ax
    ret

    .error:
        ret

global _get_used_memory
_get_used_memory:
    ; Needs verification

    mov ah, 0x8A
    int 0x15

    mov [_used_memory], dx

    ret
