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
-- Entity: 	inpad
-- File:	inpad.vhd
-- Author:	Jiri Gaisler - Gaisler Research
-- Description:	input pad with technology wrapper
------------------------------------------------------------------------------

library techmap;
library ieee;
use ieee.std_logic_1164.all;
use techmap.gencomp.all;
use techmap.allpads.all;

entity inpad is
  generic (tech : integer := 0; level : integer := 0; 
	voltage : integer := x33v; filter : integer := 0);
  port (pad : in std_ulogic; o : out std_ulogic);
end; 

architecture rtl of inpad is
begin
  gen0 : if has_pads(tech) = 0 generate
    o <= to_X01(pad) after 1 ns;
  end generate;
  xcv : if (tech = virtex) or (tech = virtex2) or (tech = spartan3) or (tech = virtex4) generate
    u0 : virtex_inpad generic map (level, voltage) port map (pad, o);
  end generate;
  axc : if (tech = axcel) or (tech = proasic) or (tech = proasic3) generate
    u0 : axcel_inpad generic map (level, voltage) port map (pad, o);
  end generate;
  atc : if (tech = atc18) generate
    u0 : atc18_inpad generic map (level, voltage) port map (pad, o);
  end generate;
  rhu : if (tech = rhumc) generate
    u0 : rhumc_inpad generic map (level, voltage) port map (pad, o);
  end generate;
  ihp : if (tech = ihp25) generate
    u0 : ihp25_inpad generic map(level, voltage) port map(pad, o);
  end generate; 
  rh18t : if (tech = rhlib18t) generate
    u0 : rh_lib18t_inpad generic map (voltage, filter) port map(pad, o);
  end generate; 
end;

library techmap;
library ieee;
use ieee.std_logic_1164.all;
use techmap.gencomp.all;

entity inpadv is
  generic (tech : integer := 0; level : integer := 0; 
	   voltage : integer := 0; width : integer := 1);
  port (
    pad : in  std_logic_vector(width-1 downto 0); 
    o   : out std_logic_vector(width-1 downto 0));
end; 
architecture rtl of inpadv is
begin
  v : for i in width-1 downto 0 generate
    u0 : inpad generic map (tech, level, voltage) port map (pad(i), o(i));
  end generate;
end;
