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
-- Entity: 	Various
-- File:	atmel_simprims.vhd
-- Author:	Jiri Gaisler - Gaisler Research
-- Description:	ATMEL ATC18 behavioural models
--		Modelled after IO33/PCILIB data sheets
------------------------------------------------------------------------------

-- pragma translate_off
-- input pad

library ieee;
use ieee.std_logic_1164.all;

entity pc33d00 is port (pad : in std_logic; cin : out std_logic); end; 
architecture rtl of pc33d00 is begin cin <= to_x01(pad) after 1 ns; end;

-- input pad with pull-up

library ieee;
use ieee.std_logic_1164.all;

entity pc33d00u is port (pad : inout std_logic; cin : out std_logic); end; 
architecture rtl of pc33d00u is 
begin cin <= to_x01(pad) after 1 ns; pad <= 'H'; end;

-- input schmitt pad

library ieee;
use ieee.std_logic_1164.all;

entity pc33d20 is port (pad : in std_logic; cin : out std_logic); end; 
architecture rtl of pc33d20 is begin cin <= to_x01(pad) after 1 ns; end;

-- input schmitt pad with pull-up

library ieee;
use ieee.std_logic_1164.all;

entity pc33d20u is port (pad : inout std_logic; cin : out std_logic); end; 
architecture rtl of pc33d20u is 
begin cin <= to_x01(pad) after 1 ns; pad <= 'H'; end;

-- output pads

library ieee; use ieee.std_logic_1164.all;

entity pt33o01 is port (i : in  std_logic; pad : out  std_logic); end; 
architecture rtl of pt33o01 is begin pad <= to_x01(i) after 2 ns; end;

library ieee; use ieee.std_logic_1164.all;

entity pt33o02 is port (i : in  std_logic; pad : out  std_logic); end; 
architecture rtl of pt33o02 is begin pad <= to_x01(i) after 2 ns; end;

library ieee; use ieee.std_logic_1164.all;

entity pt33o03 is port (i : in  std_logic; pad : out  std_logic); end; 
architecture rtl of pt33o03 is begin pad <= to_x01(i) after 2 ns; end;

library ieee; use ieee.std_logic_1164.all;

entity pt33o04 is port (i : in  std_logic; pad : out  std_logic); end; 
architecture rtl of pt33o04 is begin pad <= to_x01(i) after 2 ns; end;

-- output tri-state pads

library ieee; use ieee.std_logic_1164.all;

entity pt33t01 is port (i, oen : in  std_logic; pad : out  std_logic); end; 
architecture rtl of pt33t01 is
begin pad <= to_x01(i) after 2 ns when oen = '0' else 'H' after 2 ns; end; 

library ieee; use ieee.std_logic_1164.all;

entity pt33t02 is port (i, oen : in  std_logic; pad : out  std_logic); end; 
architecture rtl of pt33t02 is
begin pad <= to_x01(i) after 2 ns when oen = '0' else 'H' after 2 ns; end; 

library ieee; use ieee.std_logic_1164.all;

entity pt33t03 is port (i, oen : in  std_logic; pad : out  std_logic); end; 
architecture rtl of pt33t03 is
begin pad <= to_x01(i) after 2 ns when oen = '0' else 'H' after 2 ns; end; 

library ieee; use ieee.std_logic_1164.all;

entity pt33t04 is port (i, oen : in  std_logic; pad : out  std_logic); end; 
architecture rtl of pt33t04 is
begin pad <= to_x01(i) after 2 ns when oen = '0' else 'H' after 2 ns; end; 

-- output tri-state pads with pull-up

library ieee; use ieee.std_logic_1164.all;

entity pt33t01u is port (i, oen : in  std_logic; pad : out  std_logic); end; 
architecture rtl of pt33t01u is
begin pad <= to_x01(i) after 2 ns when oen = '0' else 'H' after 2 ns; end; 

library ieee; use ieee.std_logic_1164.all;

entity pt33t02u is port (i, oen : in  std_logic; pad : out  std_logic); end; 
architecture rtl of pt33t02u is
begin pad <= to_x01(i) after 2 ns when oen = '0' else 'H' after 2 ns; end; 

library ieee; use ieee.std_logic_1164.all;

entity pt33t03u is port (i, oen : in  std_logic; pad : out  std_logic); end; 
architecture rtl of pt33t03u is
begin pad <= to_x01(i) after 2 ns when oen = '0' else 'H' after 2 ns; end; 

-- bidirectional pad

library ieee; use ieee.std_logic_1164.all;

entity pt33b01 is
  port ( i, oen : in std_logic; cin : out std_logic; pad : inout std_logic);
end; 
architecture rtl of pt33b01 is
begin 
  pad <= to_x01(i) after 2 ns when oen = '0' else 'Z' after 2 ns;
  cin <= to_x01(pad) after 1 ns;
end;

library ieee; use ieee.std_logic_1164.all;
entity pt33b02 is
  port ( i, oen : in std_logic; cin : out std_logic; pad : inout std_logic);
end; 
architecture rtl of pt33b02 is
begin 
  pad <= to_x01(i) after 2 ns when oen = '0' else 'Z' after 2 ns;
  cin <= to_x01(pad) after 1 ns;
end;

library ieee; use ieee.std_logic_1164.all;

entity pt33b03 is
  port ( i, oen : in std_logic; cin : out std_logic; pad : inout std_logic);
end; 
architecture rtl of pt33b03 is
begin 
  pad <= to_x01(i) after 2 ns when oen = '0' else 'Z' after 2 ns;
  cin <= to_x01(pad) after 1 ns;
end;

library ieee; use ieee.std_logic_1164.all;

entity pt33b04 is
  port ( i, oen : in std_logic; cin : out std_logic; pad : inout std_logic);
end; 
architecture rtl of pt33b04 is
begin 
  pad <= to_x01(i) after 2 ns when oen = '0' else 'Z' after 2 ns;
  cin <= to_x01(pad) after 1 ns;
end;

-- bidirectional pads with pull-up

library ieee;
use ieee.std_logic_1164.all;

entity pt33b01u is
  port ( i, oen : in std_logic; cin : out std_logic; pad : inout std_logic);
end; 

architecture rtl of pt33b01u is
begin 
  pad <= to_x01(i) after 2 ns when oen = '0' else 'H' after 2 ns;
  cin <= to_x01(pad) after 1 ns;
end;

library ieee;
use ieee.std_logic_1164.all;

entity pt33b02u is
  port ( i, oen : in std_logic; cin : out std_logic; pad : inout std_logic);
end; 
architecture rtl of pt33b02u is
begin 
  pad <= to_x01(i) after 2 ns when oen = '0' else 'H' after 2 ns;
  cin <= to_x01(pad) after 1 ns;
end;

library ieee;
use ieee.std_logic_1164.all;

entity pt33b03u is
  port ( i, oen : in std_logic; cin : out std_logic; pad : inout std_logic);
end; 
architecture rtl of pt33b03u is
begin 
  pad <= to_x01(i) after 2 ns when oen = '0' else 'H' after 2 ns;
  cin <= to_x01(pad) after 1 ns;
end;

library ieee;
use ieee.std_logic_1164.all;

entity pt33b04u is
  port ( i, oen : in std_logic; cin : out std_logic; pad : inout std_logic);
end; 
architecture rtl of pt33b04u is
begin 
  pad <= to_x01(i) after 2 ns when oen = '0' else 'H' after 2 ns;
  cin <= to_x01(pad) after 1 ns;
end;

-- PCI output pad

library ieee; use ieee.std_logic_1164.all;
entity pp33o01 is port (i : in  std_logic; pad : out  std_logic); end; 
architecture rtl of pp33o01 is begin pad <= to_x01(i) after 2 ns; end;

-- PCI bidirectional pad

library ieee; use ieee.std_logic_1164.all;
entity pp33b015vt is
  port ( i, oen : in std_logic; cin : out std_logic; pad : inout std_logic);
end; 
architecture rtl of pp33b015vt is
begin 
  pad <= to_x01(i) after 2 ns when oen = '0' else 'Z' after 2 ns;
  cin <= to_x01(pad) after 1 ns;
end;

-- PCI output tri-state pad

library ieee; use ieee.std_logic_1164.all;
entity pp33t015vt is port (i, oen : in  std_logic; pad : out  std_logic); end; 
architecture rtl of pp33t015vt is
begin pad <= to_x01(i) after 2 ns when oen = '0' else 'Z' after 2 ns; end; 
-- pragma translate_on
