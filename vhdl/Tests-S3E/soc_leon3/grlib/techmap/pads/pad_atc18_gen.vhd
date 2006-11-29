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
-- Package: 	atcpads_gen
-- File:	atcpads_gen.vhd
-- Author:	Jiri Gaisler - Gaisler Research
-- Description:	Atmel ATC18 pad wrappers
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
package atcpads is
  -- input pad
  component pc33d00 port (pad : in std_logic; cin : out std_logic); end component; 
  -- input pad with pull-up
  component pc33d00u port (pad : in std_logic; cin : out std_logic); end component; 
  -- schmitt input pad
  component pc33d20 port (pad : in std_logic; cin : out std_logic); end component; 
  -- schmitt input pad with pull-up
  component pt33d20u port (pad : inout std_logic; cin : out std_logic); end component; 
  -- output pads
  component pt33o01 port (i : in std_logic; pad : out std_logic); end component; 
  component pt33o02 port (i : in std_logic; pad : out std_logic); end component; 
  component pt33o03 port (i : in std_logic; pad : out std_logic); end component; 
  component pt33o04 port (i : in std_logic; pad : out std_logic); end component; 
  -- tri-state output pads
  component pt33t01 port (i, oen : in std_logic; pad : out std_logic); end component; 
  component pt33t02 port (i, oen : in std_logic; pad : out std_logic); end component; 
  component pt33t03 port (i, oen : in std_logic; pad : out std_logic); end component; 
  component pt33t04 port (i, oen : in std_logic; pad : out std_logic); end component; 
  -- tri-state output pads with pull-up
  component pt33t01u port (i, oen : in std_logic; pad : out std_logic); end component; 
  component pt33t02u port (i, oen : in std_logic; pad : out std_logic); end component; 
  component pt33t03u port (i, oen : in std_logic; pad : out std_logic); end component; 
  component pt33t04u port (i, oen : in std_logic; pad : out std_logic); end component; 
  -- bidirectional pads
  component pt33b01
    port (i, oen : in std_logic; cin : out std_logic; pad : inout std_logic);
  end component; 
  component pt33b02
    port (i, oen : in std_logic; cin : out std_logic; pad : inout std_logic);
  end component; 
  component pt33b03
    port (i, oen : in std_logic; cin : out std_logic; pad : inout std_logic);
  end component; 
  component pt33b04
    port (i, oen : in std_logic; cin : out std_logic; pad : inout std_logic);
  end component; 
  -- bidirectional pads with pull-up
  component pt33b01u
    port (i, oen : in std_logic; cin : out std_logic; pad : inout std_logic);
  end component; 
  component pt33b02u
    port (i, oen : in std_logic; cin : out std_logic; pad : inout std_logic);
  end component; 
  component pt33b03u
    port (i, oen : in std_logic; cin : out std_logic; pad : inout std_logic);
  end component; 
  component pt33b04u
    port (i, oen : in std_logic; cin : out std_logic; pad : inout std_logic);
  end component; 
--PCI pads
  component pp33o01 
    port (i : in  std_logic; pad : out  std_logic);
  end component; 
  component pp33b015vt 
    port ( i, oen : in std_logic; cin : out std_logic; pad : inout std_logic);
  end component; 
  component pp33t015vt 
    port (i, oen : in  std_logic; pad : out  std_logic);
  end component;
end;

library ieee;
library techmap;
use ieee.std_logic_1164.all;
use techmap.gencomp.all;
-- pragma translate_off
library atmel;
use atmel.pc33d00;
-- pragma translate_on

entity atc18_inpad is
  generic (level : integer := 0; voltage : integer := 0);
  port (pad : in std_logic; o : out std_logic);
end; 
architecture rtl of atc18_inpad is
  component pc33d00 port (pad : in std_logic; cin : out std_logic); end component; 
begin
  pci0 : if level = pci33 generate
    ip : pc33d00 port map (pad => pad, cin => o);
  end generate;
  gen0 : if level /= pci33 generate
    ip : pc33d00 port map (pad => pad, cin => o);
  end generate;
end;

library ieee;
use ieee.std_logic_1164.all;
library techmap;
use techmap.gencomp.all;
-- pragma translate_off
library atmel;
use atmel.pp33b015vt;
use atmel.pt33b01;
use atmel.pt33b02;
use atmel.pt33b03;
use atmel.pt33b04;
-- pragma translate_on

entity atc18_iopad  is
  generic (level : integer := 0; slew : integer := 0;
	   voltage : integer := 0; strength : integer := 0);
  port (pad : inout std_logic; i, en : in std_logic; o : out std_logic);
end ;
architecture rtl of atc18_iopad is
  component pp33b015vt 
    port ( i, oen : in std_logic; cin : out std_logic; pad : inout std_logic);
  end component; 
  component pt33b01
    port (i, oen : in std_logic; cin : out std_logic; pad : inout std_logic);
  end component; 
  component pt33b02
    port (i, oen : in std_logic; cin : out std_logic; pad : inout std_logic);
  end component; 
  component pt33b03
    port (i, oen : in std_logic; cin : out std_logic; pad : inout std_logic);
  end component; 
  component pt33b04
    port (i, oen : in std_logic; cin : out std_logic; pad : inout std_logic);
  end component; 

begin
  pci0 : if level = pci33 generate
    op : pp33b015vt port map (i => i, oen => en, pad => pad, cin => o);
  end generate;
  gen0 : if level /= pci33 generate
    f1 : if (strength <= 4)  generate
      op : pt33b01 port map (i => i, oen => en, pad => pad, cin => o);
    end generate;
    f2 : if (strength > 4)  and (strength <= 8)  generate
      op : pt33b02 port map (i => i, oen => en, pad => pad, cin => o);
    end generate;
    f3 : if (strength > 8)  and (strength <= 16)  generate
      op : pt33b03 port map (i => i, oen => en, pad => pad, cin => o);
    end generate;
    f4 : if (strength > 16)  generate
      op : pt33b04 port map (i => i, oen => en, pad => pad, cin => o);
    end generate;
  end generate;
end;

library ieee;
use ieee.std_logic_1164.all;
library techmap;
use techmap.gencomp.all;
-- pragma translate_off
library atmel;
-- pragma translate_on
use techmap.allpads.all;

entity atc18_iodpad  is
  generic (level : integer := 0; slew : integer := 0;
	   voltage : integer := 0; strength : integer := 0);
  port (pad : inout std_logic; i, en : in std_logic; o : out std_logic);
end ;
architecture rtl of atc18_iodpad is
signal gnd : std_ulogic;
begin
  gnd <= '0';
  p0 : atc18_iopad generic map (level, slew, voltage, strength)
  port map (pad => pad, i => gnd, en => i, o => o);
end;

library ieee;
use ieee.std_logic_1164.all;
library techmap;
use techmap.gencomp.all;
-- pragma translate_off
library atmel;
use atmel.pp33t015vt;
use atmel.pt33o01;
use atmel.pt33o02;
use atmel.pt33o03;
use atmel.pt33o04;
-- pragma translate_on

entity atc18_outpad  is
  generic (level : integer := 0; slew : integer := 0;
	   voltage : integer := 0; strength : integer := 0);
  port (pad : out std_logic; i : in std_logic);
end ;
architecture rtl of atc18_outpad is
  component pp33t015vt 
    port (i, oen : in  std_logic; pad : out  std_logic);
  end component;
  component pt33o01 port (i : in std_logic; pad : out std_logic); end component; 
  component pt33o02 port (i : in std_logic; pad : out std_logic); end component; 
  component pt33o03 port (i : in std_logic; pad : out std_logic); end component; 
  component pt33o04 port (i : in std_logic; pad : out std_logic); end component; 
signal gnd : std_logic;
begin

  gnd <= '0';

  pci0 : if level = pci33 generate
    op : pp33t015vt port map (i => i, oen => gnd, pad => pad);
  end generate;
  gen0 : if level /= pci33 generate
    f4 : if (strength <= 4)  generate
      op : pt33o01 port map (i => i, pad => pad);
    end generate;
    f8  : if (strength > 4) and (strength <= 8)  generate
      op : pt33o02 port map (i => i, pad => pad);
    end generate;
    f16 : if (strength > 8) and (strength <= 16)  generate
      op : pt33o03 port map (i => i, pad => pad);
    end generate;
    f32 : if (strength > 16) generate
      op : pt33o02 port map (i => i, pad => pad);
    end generate;
  end generate;
end;

library ieee;
use ieee.std_logic_1164.all;
library techmap;
use techmap.gencomp.all;
-- pragma translate_off
library atmel;
use atmel.pp33t015vt;
use atmel.pt33t01;
use atmel.pt33t02;
use atmel.pt33t03;
use atmel.pt33t04;
-- pragma translate_on

entity atc18_toutpad  is
  generic (level : integer := 0; slew : integer := 0;
	   voltage : integer := 0; strength : integer := 0);
  port (pad : out std_logic; i, en : in std_logic);
end ;
architecture rtl of atc18_toutpad is
  component pp33t015vt 
    port (i, oen : in  std_logic; pad : out  std_logic);
  end component;
  component pt33t01 port (i, oen : in std_logic; pad : out std_logic); end component; 
  component pt33t02 port (i, oen : in std_logic; pad : out std_logic); end component; 
  component pt33t03 port (i, oen : in std_logic; pad : out std_logic); end component; 
  component pt33t04 port (i, oen : in std_logic; pad : out std_logic); end component; 
begin

  pci0 : if level = pci33 generate
    op : pp33t015vt port map (i => i, oen => en, pad => pad);
  end generate;
  gen0 : if level /= pci33 generate
    f4 : if (strength <= 4)  generate
      op : pt33t01 port map (i => i, oen => en, pad => pad);
    end generate;
    f8  : if (strength > 4) and (strength <= 8)  generate
      op : pt33t02 port map (i => i, oen => en, pad => pad);
    end generate;
    f16 : if (strength > 8) and (strength <= 16)  generate
      op : pt33t03 port map (i => i, oen => en, pad => pad);
    end generate;
    f32 : if (strength > 16) generate
      op : pt33t02 port map (i => i, oen => en, pad => pad);
    end generate;
  end generate;

end;

library ieee;
use ieee.std_logic_1164.all;
library techmap;
use techmap.gencomp.all;
-- pragma translate_off
library atmel;
-- pragma translate_on
use techmap.allpads.all;

entity atc18_odpad  is
  generic (level : integer := 0; slew : integer := 0;
	   voltage : integer := 0; strength : integer := 0);
  port (pad : out std_logic; i : in std_logic);
end ;
architecture rtl of atc18_odpad is
signal gnd : std_ulogic;
begin
  gnd <= '0';
  p0 : atc18_toutpad generic map (level, slew, voltage, strength)
  port map (pad => pad, i => gnd, en => i);
end;

library ieee;
use ieee.std_logic_1164.all;

entity atc18_clkpad is
  generic (level : integer := 0; voltage : integer := 0);
  port (pad : in std_logic; o : out std_logic);
end; 
architecture rtl of atc18_clkpad is
begin
  o <= pad;
end;
