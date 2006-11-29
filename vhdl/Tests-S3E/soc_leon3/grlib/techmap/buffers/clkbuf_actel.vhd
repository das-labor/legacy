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
-- Entity: 	clkbuf_actel
-- File:	clkbuf_actel.vhd
-- Author:	Marko Isomaki - Gaisler Research
-- Description:	Clock buffer generator for Actel devices
------------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
-- pragma translate_off
library axcelerator;
use axcelerator.hclkint;
use axcelerator.clkint;
-- pragma translate_on

entity clkbuf_actel is
  generic(
    buftype :  integer range 0 to 2 := 0);
  port(
    i       :  in  std_ulogic;
    o       :  out std_ulogic
  );
end entity;

architecture rtl of clkbuf_actel is
  component hclkint port(a : in std_ulogic; y : out std_ulogic); end component;
  component clkint port(a : in std_ulogic; y : out std_ulogic); end component;
begin
  buf0 : if buftype = 0 generate 
    o <= i;
  end generate;
  buf1 : if buftype = 1 generate 
    buf : hclkint port map(A => i, Y => o);
  end generate;
  buf2 : if buftype = 2 generate 
    buf : clkint port map(A => i, Y => o);
  end generate;
end architecture;
