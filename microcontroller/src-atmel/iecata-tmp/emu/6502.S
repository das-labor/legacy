.arch atmega162


rom_offset = 0xc000
ram_offset = 0xb800



/* 6502 registers */
x = 2
y = 3

pcl = 18
pch = 19

a = 16
b = 17

opcode = 18
zero = 1


.global exec_6502
;
exec_6502:
	clr  r1
	call mem_r
	mov  opcode,b
	andi b,0x0f
	lsl  b
	lsl  b
	ldi  r31,hi8(pm(jmptbl))
	ldi  r30,lo8(pm(jmptbl))
	add  r30,b
	adc  r31,zero
	ijmp

jmptbl:
	jmp 
	jmp blah


	ret

;is called to read the memory the 6502 sees
;address is in r25:r24 return in b register
mem_r:
	cpi  r25,0x80
	brcc read_rom
	andi r25,0x1f
	cpi  r25,0x08
	brcc read_via

	;read ram
	ldi  r27,hi8(ram_offset)
	add	 r27,r25
	mov  r26,r24
	ld   b,Z
	ret
	
read_rom:
	andi r25,0x3f
	ldi  r27,hi8(rom_offset)
	add  r27,r25
	ld   b,Z
	ret

read_via:
	ret

