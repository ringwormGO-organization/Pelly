
extern _low_memory
_low_memory: dw 0

extern _used_memory
_used_memory: dw 20

extern _entry_count
_entry_count: dw 0

extern _segment
_segment: dw 0

extern _offset
_offset: dw 0

; Convert linear address to segment:offset address
; Args:
;    1 - linear address
;    2 - (out) target segment (e.g. es)
;    3 - target 32-bit register to use (e.g. eax)
;    4 - target lower 16-bit half of #3 (e.g. ax)
%macro LinearToSegOffset 4

    mov %3, %1      ; linear address to eax
    shr %3, 4
    mov %2, %4
    mov %3, %1      ; linear address to eax
    and %3, 0xf

%endmacro

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

;
;
;
global _get_used_memory
_get_used_memory:
    ; Needs verification

    clc

    mov ah, 0x8A
    int 0x15

    mov [_used_memory], dx
    ret

    .error:
        mov ax, 0x0e
        mov ah, 'E'

        int 0x10
        ret

global _e820
_e820:
	; make new call frame
    push bp             ; save old call frame
    mov bp, sp          ; initialize new call frame

	mov di, [bp + 4]
    mov ax, [bp + 6]

	mov es, ax

	pusha
	call _do_e820

	mov [_entry_count], bp

	popa

	; restore old call frame
    mov sp, bp
    pop bp

	ret

; use the INT 0x15, eax= 0xE820 BIOS function to get a memory map
; note: initially di is 0, be sure to set it to a value so that the BIOS code will not be overwritten. 
;       The consequence of overwriting the BIOS code will lead to problems like getting stuck in `int 0x15`
; inputs: es:di -> destination buffer for 24 byte entries
; outputs: bp = entry count, trashes all registers except esi
mmap_ent equ 0x8000             ; the number of entries will be stored at 0x8000
_do_e820:
	mov di, 0x8004          ; Set di to 0x8004. Otherwise this code will get stuck in `int 0x15` after some entries are fetched 
	xor ebx, ebx		; ebx must be 0 to start
	xor bp, bp		; keep an entry count in bp
	mov edx, 0x0534D4150	; Place "SMAP" into edx
	mov eax, 0xe820
	mov [es:di + 20], dword 1	; force a valid ACPI 3.X entry
	mov ecx, 24		; ask for 24 bytes
	mov [_segment], es
	mov [_offset], di
	int 0x15
	jc short .failed	; carry set on first call means "unsupported function"
	mov edx, 0x0534D4150	; Some BIOSes apparently trash this register?
	cmp eax, edx		; on success, eax must have been reset to "SMAP"
	jne short .failed
	test ebx, ebx		; ebx = 0 implies list is only 1 entry long (worthless)
	je short .failed
	jmp short .jmpin
.e820lp:
	mov eax, 0xe820		; eax, ecx get trashed on every int 0x15 call
	mov [es:di + 20], dword 1	; force a valid ACPI 3.X entry
	mov ecx, 24		; ask for 24 bytes again
	int 0x15
	jc short .e820f		; carry set means "end of list already reached"
	mov edx, 0x0534D4150	; repair potentially trashed register
.jmpin:
	jcxz .skipent		; skip any 0 length entries
	cmp cl, 20		; got a 24 byte ACPI 3.X response?
	jbe short .notext
	test byte [es:di + 20], 1	; if so: is the "ignore this data" bit clear?
	je short .skipent
.notext:
	mov ecx, [es:di + 8]	; get lower uint32_t of memory region length
	or ecx, [es:di + 12]	; "or" it with upper uint32_t to test for zero
	jz .skipent		; if length uint64_t is 0, skip entry
	inc bp			; got a good entry: ++count, move to next storage spot
	add di, 24
.skipent:
	test ebx, ebx		; if ebx resets to 0, list is complete
	jne short .e820lp
.e820f:
	mov [mmap_ent], bp	; store the entry count
	mov [_entry_count], bp

	; mov [_segment], es
	; mov [_offset], di

	clc			; there is "jc" on end of list to this point, so the carry must be cleared
	ret
.failed:
	stc			; "function unsupported" error exit
	ret
;
;   __E820GetNextBlock -> Get E820 memory map
;   void _cdecl E820GetNextBlock(E820MemoryBlock* block, uint32_t* continuationId);
;
E820Signature   equ 0x534D4150

global _E820GetNextBlock
_E820GetNextBlock:
	clc

    ; make new call frame
    push bp             ; save old call frame
    mov bp, sp          ; initialize new call frame

    ; setup params
    LinearToSegOffset [bp + 8], es, edi, di     ; es:di pointer to structure
    
    LinearToSegOffset [bp + 12], ds, esi, si    ; ebx - pointer to continuationId
    mov ebx, ds:[si]

    mov eax, 0xE820                             ; eax - function
    mov edx, E820Signature                      ; edx - signature
    mov ecx, 24                                 ; ecx - size of structure

    ; call interrupt
    int 0x15

	jc .Error

    ; test results
    cmp eax, E820Signature
    jne .Error

    .IfSuccedeed:
        mov eax, ecx            ; return size
        mov ds:[si], ebx        ; fill continuation parameter
        jmp .EndIf

    .Error:
        mov eax, -1

    .EndIf:

    ; restore old call frame
    mov sp, bp
    pop bp
    ret
