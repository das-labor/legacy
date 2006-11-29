-----------------------------------------------------------------------------
-- LEON3 Demonstration design test bench configuration
-- Copyright (C) 2004 Jiri Gaisler, Gaisler Research
--
-- This program is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation; either version 2 of the License, or
-- (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
-- GNU General Public License for more details.
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
library techmap;
use techmap.gencomp.all;

package config is
-- Technology and synthesis options
  constant CFG_FABTECH : integer := spartan3;
  constant CFG_MEMTECH : integer := inferred;
  constant CFG_PADTECH : integer := inferred;
  constant CFG_NOASYNC : integer := 1;
-- Clock generator
  constant CFG_CLKTECH : integer := virtex2;
  constant CFG_CLKMUL : integer := (2);
  constant CFG_CLKDIV : integer := (2);
  constant CFG_PCIDLL : integer := 0;
  constant CFG_PCISYSCLK : integer := 0;
-- LEON3 processor core
  constant CFG_NCPU : integer := (1);
  constant CFG_NWIN : integer := (8);
  constant CFG_V8 : integer := 0;
  constant CFG_MAC : integer := 0;
  constant CFG_SVT : integer := 1;
  constant CFG_RSTADDR : integer := 16#00000#;
  constant CFG_LDDEL : integer := (1);
  constant CFG_NWP : integer := (0);
  constant CFG_PWD : integer := 0*2;
  constant CFG_FPU : integer := 0;
  constant CFG_ICEN : integer := 1;
  constant CFG_ISETS : integer := 1;
  constant CFG_ISETSZ : integer := 2;
  constant CFG_ILINE : integer := 8;
  constant CFG_IREPL : integer := 0;
  constant CFG_ILOCK : integer := 0;
  constant CFG_ILRAMEN : integer := 0;
  constant CFG_ILRAMADDR: integer := 16#8E#;
  constant CFG_ILRAMSZ : integer := 1;
  constant CFG_DCEN : integer := 0;
  constant CFG_DSETS : integer := 1;
  constant CFG_DSETSZ : integer := 1;
  constant CFG_DLINE : integer := 8;
  constant CFG_DREPL : integer := 0;
  constant CFG_DLOCK : integer := 0;
  constant CFG_DSNOOP : integer := 0 + 0;
  constant CFG_DFIXED : integer := 16#0#;
  constant CFG_DLRAMEN : integer := 0;
  constant CFG_DLRAMADDR: integer := 16#8F#;
  constant CFG_DLRAMSZ : integer := 1;
  constant CFG_MMUEN : integer := 0;
  constant CFG_ITLBNUM : integer := 2;
  constant CFG_DTLBNUM : integer := 2;
  constant CFG_TLB_TYPE : integer := 1;
  constant CFG_TLB_REP : integer := 1;
  constant CFG_DSU : integer := 0;
  constant CFG_ITBSZ : integer := 0;
  constant CFG_ATBSZ : integer := 0;
  constant CFG_DISAS : integer := 0 + 0;
  constant CFG_PCLOW : integer := 2;
-- AMBA settings
  constant CFG_DEFMST : integer := (0);
  constant CFG_RROBIN : integer := 1;
  constant CFG_SPLIT : integer := 0;
  constant CFG_AHBIO : integer := 16#FFF#;
  constant CFG_APBADDR : integer := 16#800#;
-- DSU UART
  constant CFG_AHB_UART : integer := 0;
-- JTAG based DSU interface
  constant CFG_AHB_JTAG : integer := 0;
-- LEON2 memory controller
  constant CFG_MCTRL_LEON2 : integer := 0;
  constant CFG_MCTRL_RAM8BIT : integer := 0;
  constant CFG_MCTRL_RAM16BIT : integer := 0;
  constant CFG_MCTRL_5CS : integer := 0;
  constant CFG_MCTRL_SDEN : integer := 0;
  constant CFG_MCTRL_SEPBUS : integer := 0;
  constant CFG_MCTRL_INVCLK : integer := 0;
  constant CFG_MCTRL_SD64 : integer := 0;
-- DDRMP controller
  constant CFG_DDRMP_EN : integer := 0;
  constant CFG_DDRMP_EN2P : integer := 0;
  constant CFG_DDRMP_NCS : integer := 1;
  constant CFG_DDRMP_NDEV : integer := 2;
  constant CFG_DDRMP_NBITS : integer := 16;
  constant CFG_DDRMP_MBITS : integer := 64;
  constant CFG_DDRMP_PERIOD : integer := 1000/100;
-- AHB status register
  constant CFG_AHBSTAT : integer := 0;
  constant CFG_AHBSTATN : integer := 1;
-- AHB RAM
  constant CFG_AHBRAMEN : integer := 1;
  constant CFG_AHBRSZ : integer := 8;
  constant CFG_AHBRADDR : integer := 16#A00#;
-- PCI interface
  constant CFG_PCI : integer := 0;
  constant CFG_PCIVID : integer := 0;
  constant CFG_PCIDID : integer := 0;
  constant CFG_PCIDEPTH : integer := 8;
  constant CFG_PCI_MTF : integer := 1;
-- PCI trace buffer
  constant CFG_PCITBUFEN: integer := 0;
  constant CFG_PCITBUF : integer := 256;
-- UART 1
  constant CFG_UART1_ENABLE : integer := 0;
  constant CFG_UART1_FIFO : integer := 1;
-- LEON3 interrupt controller
  constant CFG_IRQ3_ENABLE : integer := 0;
-- Modular timer
  constant CFG_GPT_ENABLE : integer := 0;
  constant CFG_GPT_NTIM : integer := 1;
  constant CFG_GPT_SW : integer := 8;
  constant CFG_GPT_TW : integer := 8;
  constant CFG_GPT_IRQ : integer := 8;
  constant CFG_GPT_SEPIRQ : integer := 0;
  constant CFG_GPT_WDOGEN : integer := 0;
  constant CFG_GPT_WDOG : integer := 16#0#;
-- GPIO port
  constant CFG_GRGPIO_ENABLE : integer := 0;
  constant CFG_GRGPIO_IMASK : integer := 16#0000#;
  constant CFG_GRGPIO_WIDTH : integer := 1;
-- GRLIB debugging
  constant CFG_DUART : integer := 0;

end;
