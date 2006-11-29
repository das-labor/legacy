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
----------------------------------------------------------------------------
-- Package: 	allpads
-- File:	allpads.vhd
-- Author:	Jiri Gaisler - Gaisler Research
-- Description:	All tech pads
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
library techmap;
use techmap.gencomp.all;

package allpads is

component apa3_clkpad 
  generic (level : integer := 0; voltage : integer := 0);
  port (pad : in std_ulogic; o : out std_ulogic);
end component; 

component axcel_inpad 
  generic (level : integer := 0; voltage : integer := 0);
  port (pad : in std_ulogic; o : out std_ulogic);
end component; 

component axcel_iopad 
  generic (level : integer := 0; slew : integer := 0;
	   voltage : integer := 0; strength : integer := 0);
  port (pad : inout std_ulogic; i, en : in std_ulogic; o : out std_ulogic);
end component;

component axcel_outpad 
  generic (level : integer := 0; slew : integer := 0;
	   voltage : integer := 0; strength : integer := 0);
  port (pad : out std_ulogic; i : in std_ulogic);
end component;

component axcel_odpad 
  generic (level : integer := 0; slew : integer := 0;
	   voltage : integer := 0; strength : integer := 0);
  port (pad : out std_ulogic; i : in std_ulogic);
end component;

component axcel_toutpad 
  generic (level : integer := 0; slew : integer := 0;
	   voltage : integer := 0; strength : integer := 0);
  port (pad : out std_ulogic; i, en : in std_ulogic);
end component;

component axcel_clkpad 
  generic (level : integer := 0; voltage : integer := 0);
  port (pad : in std_ulogic; o : out std_ulogic);
end component; 

component axcel_inpad_ds
  generic (level : integer := lvds; voltage : integer := x33v);
  port (padp, padn : in std_ulogic; o : out std_ulogic);
end component; 

component axcel_outpad_ds
  generic (level : integer := lvds; voltage : integer := x33v);
  port (padp, padn : out std_ulogic; i : in std_ulogic);
end component;

component atc18_inpad 
  generic (level : integer := 0; voltage : integer := 0);
  port (pad : in std_logic; o : out std_logic);
end component; 

component atc18_iopad 
  generic (level : integer := 0; slew : integer := 0;
	   voltage : integer := 0; strength : integer := 0);
  port (pad : inout std_logic; i, en : in std_logic; o : out std_logic);
end component;

component atc18_outpad 
  generic (level : integer := 0; slew : integer := 0;
	   voltage : integer := 0; strength : integer := 0);
  port (pad : out std_logic; i : in std_logic);
end component;

component atc18_odpad 
  generic (level : integer := 0; slew : integer := 0;
	   voltage : integer := 0; strength : integer := 0);
  port (pad : out std_logic; i : in std_logic);
end component;

component atc18_toutpad 
  generic (level : integer := 0; slew : integer := 0;
	   voltage : integer := 0; strength : integer := 0);
  port (pad : out std_logic; i, en : in std_logic);
end component;

component atc18_clkpad 
  generic (level : integer := 0; voltage : integer := 0);
  port (pad : in std_logic; o : out std_logic);
end component; 

  component ihp25_inpad
    generic(level : integer := 0; voltage : integer := 0);
    port (pad : in std_logic; o : out std_logic);
  end component; 

  component ihp25_iopad
    generic (level : integer := 0; slew : integer := 0;
             voltage : integer := 0; strength : integer := 0);
    port (pad : inout std_logic; i, en : in std_logic; o : out std_logic);
  end component;
  
  component ihp25_outpad
    generic (level : integer := 0; slew : integer := 0;
             voltage : integer := 0; strength : integer := 0);
    port (pad : out  std_logic; i : in  std_logic);
  end component; 

  component ihp25_toutpad 
    generic (level : integer := 0; slew : integer := 0;
	     voltage : integer := 0; strength : integer := 0);
    port (pad : out std_ulogic; i, en : in std_logic);
  end component;

  component ihp25_clkpad 
    generic (level : integer := 0; voltage : integer := 0);
    port (pad : in std_ulogic; o : out std_ulogic);
  end component; 

component rhumc_inpad 
  generic (level : integer := 0; voltage : integer := 0);
  port (pad : in std_logic; o : out std_logic);
end component; 

component rhumc_iopad 
  generic (level : integer := 0; slew : integer := 0;
	   voltage : integer := 0; strength : integer := 0);
  port (pad : inout std_logic; i, en : in std_logic; o : out std_logic);
end component;

component rhumc_outpad 
  generic (level : integer := 0; slew : integer := 0;
	   voltage : integer := 0; strength : integer := 0);
  port (pad : out std_logic; i : in std_logic);
end component;

component rhumc_toutpad 
  generic (level : integer := 0; slew : integer := 0;
	   voltage : integer := 0; strength : integer := 0);
  port (pad : out std_logic; i, en : in std_logic);
end component;

component virtex_inpad 
  generic (level : integer := 0; voltage : integer := x33v);
  port (pad : in std_ulogic; o : out std_ulogic);
end component; 

component virtex_iopad 
  generic (level : integer := 0; slew : integer := 0;
	   voltage : integer := x33v; strength : integer := 12);
  port (pad : inout std_ulogic; i, en : in std_ulogic; o : out std_ulogic);
end component;

component virtex_outpad 
  generic (level : integer := 0; slew : integer := 0;
	   voltage : integer := x33v; strength : integer := 12);
  port (pad : out std_ulogic; i : in std_ulogic);
end component;

component virtex_odpad 
  generic (level : integer := 0; slew : integer := 0;
	   voltage : integer := x33v; strength : integer := 12);
  port (pad : out std_ulogic; i : in std_ulogic);
end component;

component virtex_toutpad 
  generic (level : integer := 0; slew : integer := 0;
	   voltage : integer := x33v; strength : integer := 12);
  port (pad : out std_ulogic; i, en : in std_ulogic);
end component;

component virtex_clkpad 
  generic (level : integer := 0; voltage : integer := x33v; arch : integer := 0);
  port (pad : in std_ulogic; o : out std_ulogic);
end component; 

component virtex_inpad_ds
  generic (level : integer := lvds; voltage : integer := x33v);
  port (padp, padn : in std_ulogic; o : out std_ulogic);
end component; 

component virtex_outpad_ds
  generic (level : integer := lvds; voltage : integer := x33v);
  port (padp, padn : out std_ulogic; i : in std_ulogic);
end component;

component rh_lib18t_inpad
  generic ( voltage : integer := 0; filter : integer := 0);
  port (pad : in std_logic; o : out std_logic);
end component; 

component rh_lib18t_iopad
  generic ( strength : integer := 0);
  port (pad : inout std_logic; i, en : in std_logic; o : out std_logic);
end component;

component rh_lib18t_inpad_ds is
  port (padp, padn : in std_ulogic; o : out std_ulogic; en : in std_ulogic);
end component; 

component rh_lib18t_outpad_ds is
  port (padp, padn : out std_ulogic; i, en : in std_ulogic);
end component; 

end;
