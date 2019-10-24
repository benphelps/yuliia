; x86 has 4 (2 byte, 16 bit) registers
; a, b, c, d

; Access to each 2 byte register is done using
; ax, bx, cx, dx

mov ax, 'BP' ; places 2 bytes into ax
mov ax, 'V' ; places 2 bytes into ax, nasm pads with 0x0

; Acces to each bit is done using h/l (high/low)
; ah/al, bh/bl, ch/cl, dh/dl

mov ah, 'B'
mov al, 'P'
cmp ax, 'BP'


; x86 offers special segment registers
; cx, ds, ss, es

; x86 has to special registers that maintain the
; addresses to the top and bottom of the stack
; sp = stack top
; bp = stack bottom

; idk yet, haha