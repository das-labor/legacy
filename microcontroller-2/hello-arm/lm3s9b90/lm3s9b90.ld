/* lm3s9b90.ld */
/*
    This file is part of the OpenARMWare.
    Copyright (C) 2010  Daniel Otte (daniel.otte@rub.de)

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

MEMORY {
	flash (rx): ORIGIN = 0x00000000, LENGTH = 256K
	ram  (rwx): ORIGIN = 0x20000000, LENGTH =  96K
}

SECTIONS {
	.text : {
		_text = .;
		KEEP(*(.isr_vectors))
		*(.text*)
		*(.rodata*)
	} > flash
     /* .ARM.exidx is sorted, so has to go in its own output section.
*/
    .ARM.exidx : {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > flash

  . = ALIGN(4);
    .ARM.extab : {
        __extab_start = .;
        *(.ARM.extab* .gnu.linkonce.armextab.*)
        __extab_end = .;
    } > flash

  . = ALIGN(4);
	
	_text_end = .;
	.data : AT ( ADDR(.ARM.extab) + SIZEOF(.ARM.extab) ) {
		_data = .;
		*(.data*)
		_data_end = .;
	} > ram
	
	.bss : AT ( ADDR(.data) + SIZEOF(.data) ) {
		_bss = .;
		*(.bss*)
		*(COMMON)
		_bss_end = .;
	} > ram	
    end = .;
}
