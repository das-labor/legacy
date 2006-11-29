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
-- Package: 	components
-- File:	components.vhd
-- Author:	Jiri Gaisler, Gaisler Research
-- Description:	Actel proasic3 RAM component declarations
-----------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;

package components is

-- Proasic3 rams

  component RAM4K9
    generic (abits : integer range 9 to 12 := 9);
    port(
	ADDRA0, ADDRA1, ADDRA2, ADDRA3, ADDRA4, ADDRA5, ADDRA6, ADDRA7,
	ADDRA8, ADDRA9, ADDRA10, ADDRA11 : in std_logic;
	ADDRB0, ADDRB1, ADDRB2, ADDRB3, ADDRB4, ADDRB5, ADDRB6, ADDRB7,
	ADDRB8, ADDRB9, ADDRB10, ADDRB11 : in std_logic;
	BLKA, WENA, PIPEA, WMODEA, WIDTHA0, WIDTHA1, WENB, BLKB,
	PIPEB, WMODEB, WIDTHB1, WIDTHB0 : in std_logic;
	DINA0, DINA1, DINA2, DINA3, DINA4, DINA5, DINA6, DINA7, DINA8 : in std_logic;
	DINB0, DINB1, DINB2, DINB3, DINB4, DINB5, DINB6, DINB7, DINB8 : in std_logic;
	RESET, CLKA, CLKB : in std_logic; 
	DOUTA0, DOUTA1, DOUTA2, DOUTA3, DOUTA4, DOUTA5, DOUTA6, DOUTA7, DOUTA8 : out std_logic;
	DOUTB0, DOUTB1, DOUTB2, DOUTB3, DOUTB4, DOUTB5, DOUTB6, DOUTB7, DOUTB8 : out std_logic
    );
  end component;

  component RAM512X18
    port(
      RADDR8, RADDR7, RADDR6, RADDR5, RADDR4, RADDR3, RADDR2, RADDR1, RADDR0 : in std_logic;
      WADDR8, WADDR7, WADDR6, WADDR5, WADDR4, WADDR3, WADDR2, WADDR1, WADDR0 : in std_logic;
      WD17, WD16, WD15, WD14, WD13, WD12, WD11, WD10, WD9, 
      WD8, WD7, WD6, WD5, WD4, WD3, WD2, WD1, WD0 : in std_logic;
      REN, WEN, RESET, RW0, RW1, WW1, WW0, PIPE, RCLK, WCLK : in std_logic;
      RD17, RD16, RD15, RD14, RD13, RD12, RD11, RD10, RD9, 
      RD8, RD7, RD6, RD5, RD4, RD3, RD2, RD1, RD0 : out std_logic
    );
  end component;

  component CLKBUF port(Y : out std_logic; PAD : in std_logic); end component;
  component CLKBUF_LVCMOS18 port(Y : out std_logic; PAD : in std_logic); end component;
  component CLKBUF_LVCMOS25 port(Y : out std_logic; PAD : in std_logic); end component;
  component CLKBUF_LVCMOS33 port(Y : out std_logic; PAD : in std_logic); end component;
  component CLKBUF_LVCMOS5 port(Y : out std_logic; PAD : in std_logic); end component;
  component CLKBUF_PCI port(Y : out std_logic; PAD : in std_logic); end component;

end;

library ieee;
use ieee.std_logic_1164.all;
entity CLKBUF is port(Y : out std_logic; PAD : in std_logic); end;
architecture rtl of CLKBUF is begin Y <= PAD; end;

library ieee;
use ieee.std_logic_1164.all;
entity CLKBUF_PCI is port(Y : out std_logic; PAD : in std_logic); end;
architecture rtl of CLKBUF_PCI is begin Y <= PAD; end;

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity RAM4K9 is generic (abits : integer range 9 to 12 := 9);
    port(
	ADDRA0, ADDRA1, ADDRA2, ADDRA3, ADDRA4, ADDRA5, ADDRA6, ADDRA7,
	ADDRA8, ADDRA9, ADDRA10, ADDRA11 : in std_logic;
	ADDRB0, ADDRB1, ADDRB2, ADDRB3, ADDRB4, ADDRB5, ADDRB6, ADDRB7,
	ADDRB8, ADDRB9, ADDRB10, ADDRB11 : in std_logic;
	BLKA, WENA, PIPEA, WMODEA, WIDTHA0, WIDTHA1, WENB, BLKB,
	PIPEB, WMODEB, WIDTHB1, WIDTHB0 : in std_logic;
	DINA0, DINA1, DINA2, DINA3, DINA4, DINA5, DINA6, DINA7, DINA8 : in std_logic;
	DINB0, DINB1, DINB2, DINB3, DINB4, DINB5, DINB6, DINB7, DINB8 : in std_logic;
	RESET, CLKA, CLKB : in std_logic; 
	DOUTA0, DOUTA1, DOUTA2, DOUTA3, DOUTA4, DOUTA5, DOUTA6, DOUTA7, DOUTA8 : out std_logic;
	DOUTB0, DOUTB1, DOUTB2, DOUTB3, DOUTB4, DOUTB5, DOUTB6, DOUTB7, DOUTB8 : out std_logic
    );
end ;

architecture sim of RAM4K9 is
  type dwarrtype is array (9 to 12) of integer;
  constant dwarr : dwarrtype := (9, 4, 2, 1);
  constant dwidth : integer := dwarr(abits);
  subtype memword is std_logic_vector(dwidth-1 downto 0);
  type memtype is array (0 to 2**abits-1) of memword;
begin
  p1 : process(CLKA, CLKB, RESET)
  variable mem : memtype;
  variable ra, rb : std_logic_vector(11 downto 0);
  variable da, db : std_logic_vector(8 downto 0);
  variable qa, qb : std_logic_vector(8 downto 0);
  variable qal, qbl : std_logic_vector(8 downto 0);
  variable qao, qbo : std_logic_vector(8 downto 0);
  begin
   if rising_edge(CLKA) then
     ra := ADDRA11 & ADDRA10 & ADDRA9 & ADDRA8 & ADDRA7 & ADDRA6 & ADDRA5 &
	ADDRA4 & ADDRA3 & ADDRA2 & ADDRA1 & ADDRA0;
     da := DINA8 & DINA7 & DINA6 & DINA5 & DINA4 & DINA3 & DINA2 & 
	DINA1 & DINA0;
      if BLKA = '0' then 
        if not (is_x (ra(abits-1 downto 0))) then 
          qa(dwidth-1 downto 0) := mem(to_integer(unsigned(ra(abits-1 downto 0))));
        else qa := (others => 'X'); end if;
	if WENA = '0' and not (is_x (ra(abits-1 downto 0))) then
	  mem(to_integer(unsigned(ra(abits-1 downto 0)))) := da(dwidth-1 downto 0);
	  if WMODEA = '1' then qa := da(dwidth-1 downto 0); end if;
        end if;
      elsif is_x(BLKA) then qa := (others => 'X'); end if;
      if PIPEA = '1' then qao := qal; else qao := qa; end if;
      qal := qa;
   end if;
   if reset = '0' then qao := (others => '0'); end if;
   (DOUTA8, DOUTA7, DOUTA6, DOUTA5, DOUTA4, DOUTA3, DOUTA2, 
	DOUTA1, DOUTA0) <= qao;
   if rising_edge(CLKB) then
     rb := ADDRB11 & ADDRB10 & ADDRB9 & ADDRB8 & ADDRB7 & ADDRB6 & ADDRB5 &
	ADDRB4 & ADDRB3 & ADDRB2 & ADDRB1 & ADDRB0;
     db := DINB8 & DINB7 & DINB6 & DINB5 & DINB4 & DINB3 & DINB2 & 
	DINB1 & DINB0;
      if BLKB = '0' then 
        if not (is_x (rb(abits-1 downto 0))) then 
          qb(dwidth-1 downto 0) := mem(to_integer(unsigned(rb(abits-1 downto 0))));
        else qb := (others => 'X'); end if;
	if WENB = '0' and not (is_x (rb(abits-1 downto 0))) then
	  mem(to_integer(unsigned(rb(abits-1 downto 0)))) := db(dwidth-1 downto 0);
	  if WMODEB = '1' then qb := db(dwidth-1 downto 0); end if;
        end if;
      elsif is_x(BLKB) then qb := (others => 'X'); end if;
      if PIPEB = '1' then qbo := qbl; else qbo := qb; end if;
      qbl := qb;
   end if;
   if reset = '0' then qbo := (others => '0'); end if;
   (DOUTB8, DOUTB7, DOUTB6, DOUTB5, DOUTB4, DOUTB3, DOUTB2, 
	DOUTB1, DOUTB0) <= qbo;
  end process;
end;

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity RAM512X18 is 
    port(
      RADDR8, RADDR7, RADDR6, RADDR5, RADDR4, RADDR3, RADDR2, RADDR1, RADDR0 : in std_logic;
      WADDR8, WADDR7, WADDR6, WADDR5, WADDR4, WADDR3, WADDR2, WADDR1, WADDR0 : in std_logic;
      WD17, WD16, WD15, WD14, WD13, WD12, WD11, WD10, WD9, 
      WD8, WD7, WD6, WD5, WD4, WD3, WD2, WD1, WD0 : in std_logic;
      REN, WEN, RESET, RW0, RW1, WW1, WW0, PIPE, RCLK, WCLK : in std_logic;
      RD17, RD16, RD15, RD14, RD13, RD12, RD11, RD10, RD9, 
      RD8, RD7, RD6, RD5, RD4, RD3, RD2, RD1, RD0 : out std_logic
    );
end ;

architecture sim of RAM512X18 is
  constant abits : integer := 8;
  constant dwidth : integer := 18;
  subtype memword is std_logic_vector(dwidth-1 downto 0);
  type memtype is array (0 to 2**abits-1) of memword;
begin
  p1 : process(RCLK, WCLK, RESET)
  variable mem : memtype;
  variable ra, rb  : std_logic_vector(8 downto 0);
  variable da  : std_logic_vector(17 downto 0);
  variable qb  : std_logic_vector(17 downto 0);
  variable qbl : std_logic_vector(17 downto 0);
  variable qbo : std_logic_vector(17 downto 0);
  begin
   if rising_edge(WCLK) then
     ra :=  '0' & WADDR7 & WADDR6 & WADDR5 &
	WADDR4 & WADDR3 & WADDR2 & WADDR1 & WADDR0;
     da := WD17 & WD16 & WD15 & WD14 & WD13 & WD12 & WD11 & 
	Wd10 & WD9 & WD8 & WD7 & WD6 & WD5 & WD4 & WD3 & WD2 & 
	WD1 & WD0;
     if WEN = '0' and not (is_x (ra(abits-1 downto 0))) then
       mem(to_integer(unsigned(ra(abits-1 downto 0)))) := da(dwidth-1 downto 0);
     end if;
   end if;
   if rising_edge(RCLK) then
     rb :=  '0' & RADDR7 & RADDR6 & RADDR5 &
	RADDR4 & RADDR3 & RADDR2 & RADDR1 & RADDR0;
      if REN = '0' then 
        if not (is_x (rb(abits-1 downto 0))) then 
          qb := mem(to_integer(unsigned(rb(abits-1 downto 0))));
        else qb := (others => 'X'); end if;
      elsif is_x(REN) then qb := (others => 'X'); end if;
      if PIPE = '1' then qbo := qbl; else qbo := qb; end if;
      qbl := qb;
   end if;
   if RESET = '0' then qbo := (others => '0'); end if;
   (RD17, RD16, RD15, RD14, RD13, RD12, RD11, RD10, RD9,
      RD8, RD7, RD6, RD5, RD4, RD3, RD2, RD1, RD0) <= qbo;
  end process;
end;
