
    global _gdt_info
    _gdt_info:
        dw _gdt_end - _gdt - 1
        dd _gdt
    
    _gdt:
        dd 0, 0

    _flat_desc:
        db  0xff, 0xff, 0, 0, 0, 10010010b, 11001111b, 0

    _gdt_end:
