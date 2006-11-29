------------------------------------------------------------------------------
--  This file is a part of the GRLIB VHDL IP LIBRARY
--  Copyright (C) 2003, Gaisler Research
--
--  This program is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License as published by
--  the Free Software Foundation; either version 2 of the License, or
--  (at your option) any later version.
--
--  This program is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU General Public License for more details.
--
--  You should have received a copy of the GNU General Public License
--  along with this program; if not, write to the Free Software
--  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
-----------------------------------------------------------------------------
-- Entity: 	clkgen
-- File:	clkgen.vhd
-- Author:	Jiri Gaisler Gaisler Research
-- Description:	Clock generator with tech selection
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
library techmap;
use techmap.gencomp.all;
use techmap.libclk.all;

entity clkgen is
  generic (
    tech     : integer := DEFFABTECH; 
    clk_mul  : integer := 1; 
    clk_div  : integer := 1;
    sdramen  : integer := 0;
    sdinvclk : integer := 0;
    pcien    : integer := 0;
    pcidll   : integer := 0;
    pcisysclk: integer := 0;
    freq     : integer := 25000;	-- clock frequency in KHz
    clk2xen  : integer := 0);    
  port (
    clkin   : in  std_logic;
    pciclkin: in  std_logic;
    clk     : out std_logic;			-- main clock
    clkn    : out std_logic;			-- inverted main clock
    clk2x   : out std_logic;			-- 2x clock
    sdclk   : out std_logic;			-- SDRAM clock
    pciclk  : out std_logic;			-- PCI clock
    cgi     : in clkgen_in_type;
    cgo     : out clkgen_out_type;
    clk4x   : out std_logic);			-- 4x clock
end; 

architecture struct of clkgen is 
signal intclk : std_ulogic;
begin
  gen : if (tech /= virtex) and (tech /= virtex2) and (tech /= virtex4) and (tech /= altera) and
	(tech /= spartan3) and not ((tech = axcel) or (tech = proasic3) or (tech = proasic)) and
	(tech /= rhlib18t)
  generate
    intclk <= pciclkin when PCISYSCLK = 1 else clkin;
    sdclk <= not intclk when SDINVCLK = 1 else intclk;
    pciclk <= pciclkin; clk <= intclk; clkn <= not intclk;
    cgo.clklock <= '1'; cgo.pcilock <= '1'; clk2x <= '0';
  end generate;
  xcv : if tech = virtex generate
    v : clkgen_virtex 
    generic map (clk_mul, clk_div, sdramen, sdinvclk, pcien, pcidll, pcisysclk)
    port map (clkin, pciclkin, clk, clkn, clk2x, sdclk, pciclk, cgi, cgo);
  end generate;
  xc2v : if (tech = virtex2) or (tech = spartan3) or (tech = virtex4) generate
    v : clkgen_virtex2 
    generic map (clk_mul, clk_div, sdramen, sdinvclk, pcien, pcidll, pcisysclk, freq, clk2xen)
    port map (clkin, pciclkin, clk, clkn, clk2x, sdclk, pciclk, cgi, cgo);
  end generate;
  alt : if tech = altera generate
   v : clkgen_altera 
   generic map (clk_mul, clk_div, sdramen, sdinvclk, pcien, pcidll, pcisysclk, freq, clk2xen)
   port map (clkin, pciclkin, clk, clkn, clk2x, sdclk, pciclk, cgi, cgo);
  end generate;
  act : if (tech = axcel) or (tech = proasic3) or (tech = proasic) generate
    intclk <= pciclkin when PCISYSCLK = 1 else clkin;
    sdclk <= '0'; pciclk <= pciclkin; clk <= intclk; clkn <= '0';
    cgo.clklock <= '1'; cgo.pcilock <= '1'; clk2x <= '0';
  end generate;
  lib18t : if (tech = rhlib18t) generate
    v : clkgen_rh_lib18t 
    generic map (clk_mul, clk_div)
    port map (cgi.pllrst, clkin, clk, sdclk, clk2x, clk4x);
    intclk <= pciclkin when PCISYSCLK = 1 else clkin;
    pciclk <= pciclkin; clkn <= '0';
    cgo.clklock <= '1'; cgo.pcilock <= '1';
  end generate;
end;
