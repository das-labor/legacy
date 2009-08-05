OUTPUT_FORMAT("elf32-lm32")
ENTRY(_start)

__DYNAMIC = 0;


_BRAM_START = 0x00000000;
_BRAM_SIZE  = 0x1000;
_BRAM_END   = _BRAM_START + _BRAM_SIZE;

MEMORY {
  bram    : ORIGIN = 0x00000000, LENGTH = 0x1000          /*  4k */
}

SECTIONS
{
  .text : 
  {
    _ftext = .;
    *(.text .stub .text.* .gnu.linkonce.t.*)
    _etext = .;
  } > bram

  .rodata :
  {
	. = ALIGN(4);
	_frodata = .;
    *(.rodata .rodata.* .gnu.linkonce.r.*)
    *(.rodata1)
	_erodata = .;
  } > bram

  .data : 
  {
	. = ALIGN(4);
	_fdata = .;
    *(.data .data.* .gnu.linkonce.d.*)
    *(.data1)
    _gp = ALIGN(16);
    *(.sdata .sdata.* .gnu.linkonce.s.*)
    _edata = .;
  } > bram

  .bss :
  {
	. = ALIGN(4);
    _fbss = .;
    *(.dynsbss)
    *(.sbss .sbss.* .gnu.linkonce.sb.*)
    *(.scommon)
    *(.dynbss)
    *(.bss .bss.* .gnu.linkonce.b.*)
    *(COMMON)
    _ebss = .;
    _end = .;
  } > bram
}

PROVIDE(_fstack = ORIGIN(bram) + LENGTH(bram) - 4);
