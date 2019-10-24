; Our lower memory looks like this
; [ 0x0     - 0x3FF   ] IVT (Interrupt Vector Table)
; [ 0x400   - 0x4FF   ] BDA (BIOS Data Area)
; [ 0x500   - 0x7BFF  ] Free Memory (almost 30 KiB)
; [ 0x7C00  - 0x7DFF  ] Our BIOS Bootsector
; [ 0x7E00  - 0x7FFF  ] Free Meory (480.5 KiB)
; [ 0x80000 - 0x9FFFF ] EBDA (Extended Bios Data Area)
; [ 0xA0000 - 0xFFFFF ] Video memory, ROM (unusable)

; When we write data, it's nice to know how long it is.
; We will write out strings for example to be null terminating,
; which means we always declate the last byte of strings as 0.
boot_msg:
	db 'Booting yuliia...', 0

some_function:
	pusha
	.. shit
	popa
	ret
