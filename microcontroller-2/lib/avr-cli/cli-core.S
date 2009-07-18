/* cli-core.S */
/*
    This file is part of the AVR-Huffman.
    Copyright (C) 2009  Daniel Otte (daniel.otte@rub.de)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "avr-asm-macros.S"

#define CMDLIST_ENTRY_SIZE 8

/******************************************************************************/
/* uint16_t max_cmd_length(PGM_VOID_P cmdlist){
	uint16_t t,ret=0;
	char* str;
	for(;;){
		str = (char*)pgm_read_word(cmdlist);
		cmdlist = (uint8_t*)cmdlist + CMDLIST_ENTRY_SIZE;
		if(str==NULL)
			return ret;
		t = strlen_P(str);
		if(t>ret)
			ret=t;
	}
}
 *    param cmdlist:  r24:r25
 */
TMP_CNT_0 = 26
TMP_CNT_1 = 27 
.global max_cmd_length
max_cmd_length:
	movw r30, r24
	clr r24
	clr r25
1:
	lpm r26, Z+
	lpm r27, Z+
	movw r20, r30
	adiw r26, 0
	brne 3f
	ret
3:
	movw r30, r26
	clr TMP_CNT_0
	clr TMP_CNT_1
4:
	lpm r1, Z+
	adiw TMP_CNT_0, 1
	tst r1
	brne 4b
	sbiw TMP_CNT_0, 1
	movw r30, r20
	adiw r30, CMDLIST_ENTRY_SIZE-2
	cp  r24, TMP_CNT_0
	cpc r25, TMP_CNT_1
	brge 1b
	movw r24, TMP_CNT_0
	rjmp 1b
		 
/******************************************************************************/
.global echo_ctrl
echo_ctrl: 
	call strstrip
	tst r24
	brne 3f
	tst r25
	breq print_echo_status
3:
	call strlwr
	movw r26, r24
	ld r20, X+
	tst r20
	breq print_echo_status
	cpi r20, '1'
	breq exit_1
	cpi r20, '0'
	breq exit_0
	cpi r20, 't'
	breq exit_1
	cpi r20, 'f'
	breq exit_0
	ld r20, X+
	cpi r20, 'n'
	breq exit_1
	cpi r20, 'f'
	breq exit_0	
	ret
exit_1: 	
	ldi r20, 1
	rjmp exit
exit_0:
	ldi r20, 0
exit:	
	sts cli_echo, r20
	ret
print_echo_status:
	ldi r24, lo8(pecho)
	ldi r25, hi8(pecho)
	rcall cli_putstr_P
	lds r20, cli_echo
	tst r20
	breq 3f
	clr r25
	ldi r24, 'n'
	rcall cli_putc
	rjmp 4f
3:	clr r25
	ldi r24, 'f'
	rcall cli_putc
	clr r25
	ldi r24, 'f'
	rcall cli_putc
4:	clr r25
	ldi r24, '\r'
	rcall cli_putc
	clr r25
	ldi r24, '\n'
	rcall cli_putc
	ret		
pecho:
.asciz "\r\necho is o"

/******************************************************************************/
/* int8_t search_and_call(char* cmd, uint16_t maxcmdlength, PGM_VOID_P cmdlist)	
 *    param cmd:          r24:r25
 *    param maxcmdlength: r22:r23
 *    param cmdlist:      r20:r21
 */
/* 
.global search_and_call
search_and_call:
	adiw r24, 0
	breq return_1
	movw r26, r24
	ld r18, X
	cpi r18, 0
	breq return_1
	cpi r18, '#'
	breq return_1
	cpi r18, '?'
	breq auto_help
	
auto_help:
	movw r24, r22
	movw r22, r20
	rcall cli_auto_help	
return_1:
	clr r25
	ldi r24, 1
	ret	 
*/
