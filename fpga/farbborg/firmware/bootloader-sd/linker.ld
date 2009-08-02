OUTPUT_FORMAT("elf32-lm32")
ENTRY(_start)

__DYNAMIC = 0;


_BRAM_START = 0x00000000;
_BRAM_SIZE  = 0x2000;
_BRAM_END   = _BRAM_START + _BRAM_SIZE;

MEMORY {
/*  bram    : ORIGIN = 0x00000000, LENGTH = 0x2000 */         /*  8k */
  sram    : ORIGIN = 0x40000000 + 1024*(512-16), LENGTH = 0x4000          /*  16 k */
  
}

SECTIONS
{
  .text : 
  { 
	  . = ALIGN(4);
    _ftext = .;
    *(.text .stub .text.* .gnu.linkonce.t.*)
    _etext = .;
  } > sram

  .rodata :
  {
	. = ALIGN(4);
	_frodata = .;
    *(.rodata .rodata.* .gnu.linkonce.r.*)
    *(.rodata1)
	_erodata = .;
  } > sram

  .data : 
  {
	. = ALIGN(4);
	_fdata = .;
    *(.data .data.* .gnu.linkonce.d.*)
    *(.data1)
    _gp = ALIGN(16);
    *(.sdata .sdata.* .gnu.linkonce.s.*)
    _edata = .;
  } > sram

  .bss :
  {
    _fbss = .; 
    *(.dynsbss)
    *(.sbss .sbss.* .gnu.linkonce.sb.*)
    *(.scommon)
    *(.dynbss)
    *(.bss .bss.* .gnu.linkonce.b.*)
    *(COMMON)
    _ebss = .;
    _end = .;
  } >sram
}

PROVIDE(_fstack = ORIGIN(sram) + LENGTH(sram) - 4);
