
extern _entry_count
_entry_count: dw 0

extern _segment
_segment: dw 0

extern _offset
_offset: dw 0

;
;	_e820 -> call _do_e820 function from assembly to not get random garbage
;	Thanks to Nanobyte for help
;
global _e820
_e820:
	; make new call frame
	push bp             ; save old call frame
	mov bp, sp          ; initialize new call frame

	; set the offset of a destination buffer, _do_e820() function's argument
	mov di, [bp + 4]

	; set the segment of a destination buffer, _do_e820() function's argument
	mov ax, [bp + 6]
	mov es, ax

	; get memory map
	pusha
	call _do_e820
	popa

	; restore old call frame
	mov sp, bp
	pop bp

	ret

; use the INT 0x15, eax= 0xE820 BIOS function to get a memory map
; note: initially di is 0, be sure to set it to a value so that the BIOS code will not be overwritten. 
;		The consequence of overwriting the BIOS code will lead to problems like getting stuck in `int 0x15`
; inputs: es:di -> destination buffer for 24 byte entries
; outputs: bp = entry count, trashes all registers except esi
mmap_ent equ 0x8000				; the number of entries will be stored at 0x8000
_do_e820:
	mov di, 0x8004				; Set di to 0x8004. Otherwise this code will get stuck in `int 0x15` after some entries are fetched 
	xor ebx, ebx				; ebx must be 0 to start
	xor bp, bp					; keep an entry count in bp
	mov edx, 0x0534D4150		; Place "SMAP" into edx
	mov eax, 0xe820
	mov [es:di + 20], dword 1	; force a valid ACPI 3.X entry
	mov ecx, 24					; ask for 24 bytes
	int 0x15
	jc short .failed			; carry set on first call means "unsupported function"
	mov edx, 0x0534D4150		; Some BIOSes apparently trash this register?
	cmp eax, edx				; on success, eax must have been reset to "SMAP"
	jne short .failed
	test ebx, ebx				; ebx = 0 implies list is only 1 entry long (worthless)
	mov [_offset], di			; copy offset of the returned destination buffer to extern `offset` variable
	mov [_segment], es			; copy segment of the returned destination buffer to extern `segment` variable
	je short .failed
	jmp short .jmpin
.e820lp:
	mov eax, 0xe820				; eax, ecx get trashed on every int 0x15 call
	mov [es:di + 20], dword 1	; force a valid ACPI 3.X entry
	mov ecx, 24					; ask for 24 bytes again
	int 0x15
	jc short .e820f				; carry set means "end of list already reached"
	mov edx, 0x0534D4150		; repair potentially trashed register
.jmpin:
	jcxz .skipent				; skip any 0 length entries
	cmp cl, 20					; got a 24 byte ACPI 3.X response?
	jbe short .notext
	test byte [es:di + 20], 1	; if so: is the "ignore this data" bit clear?
	je short .skipent
.notext:
	mov ecx, [es:di + 8]		; get lower uint32_t of memory region length
	or ecx, [es:di + 12]		; "or" it with upper uint32_t to test for zero
	jz .skipent					; if length uint64_t is 0, skip entry
	inc bp						; got a good entry: ++count, move to next storage spot
	add di, 24
.skipent:
	test ebx, ebx				; if ebx resets to 0, list is complete
	jne short .e820lp
.e820f:
	mov [mmap_ent], bp			; store the entry count
	mov [_entry_count], bp		; copy entry count to the extern `entry_count` variable
	
	clc							; there is "jc" on end of list to this point, so the carry must be cleared
	ret
.failed:
	stc							; "function unsupported" error exit
	ret
