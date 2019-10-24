; IVT (Interrupt Vector Table)
; 0x10 ; Video Services (expects function in AH)
; 0x13 ; Low Level Disk Services

; Video Services
; 0x0E ; Write Character in TTY Mode

mov ah, 0x0e ; function 0x0E
mov al, '!' ; character to write
int 0x10 ; call Video Services interrupt

; Low Level Disk Services
; 0x02 ; Read Sectors

mov ah, 0x02
mov dl, 0 ; Read drive 0
mov ch, 3 ; Select cylinder 3
mov dh, 1 ; Select track 2 (0 indexed)
mov cl, 4 ; Select 4th sector (1 indexed)
mov al, 5 ; Read 5 sectors from the start point

mov bx, 0xa000
mov es, bx
mov bx, 0x1234

int 0x13

jc disk_error

cmp al, <number of sectors expected>
jne disk_error

disk_error:
	mov bx, DISK_ERROR_MSG
	call print
	jmp $

DISK_ERROR_MSG:
	db "Disk read error!", 0