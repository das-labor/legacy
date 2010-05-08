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
		*(.init*)
		*(.fini*)
		*(.text*)
		*(.rodata*)
		_text_end = .;
	} > flash
	
	.data : AT ( ADDR(.text) + SIZEOF(.text) ) {
		_data = .;
		*(.data*)
		_data_end = .;
	} > ram
	
	.bss : AT ( ADDR(.data) + SIZEOF(.data) ) {
		_bss = .;
		__bss_start__ = .;
		*(.bss*)
		*(COMMON)
		_bss_end = .;
		__bss_end__ = .;
	} > ram	
    end = .;
}
