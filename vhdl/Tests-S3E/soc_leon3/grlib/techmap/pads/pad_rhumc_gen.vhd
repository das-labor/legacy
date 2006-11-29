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
-- Package: 	umcpads_gen
-- File:	umcpads_gen.vhd
-- Author:	Jiri Gaisler - Gaisler Research
-- Description:	UMC pad wrappers
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
package umcpads is
  -- input pad

  component ICMT3V port( A : in std_logic; Z : out std_logic); end component;

  -- input pad with pull-up

  component ICMT3VPU port( A : in std_logic; Z : out std_logic); end component;

  -- input pad with pull-down

  component ICMT3VPD port( A : in std_logic; Z : out std_logic); end component;

  -- schmitt input pad

  component ISTRT3V port( A : in std_logic; Z : out std_logic); end component;

  -- output pads

  component OCM3V4 port( Z : out std_logic; A : in std_logic); end component;
  component OCM3V12 port( Z : out std_logic; A : in std_logic); end component;
  component OCM3V24 port( Z : out std_logic; A : in std_logic); end component;


  -- tri-state output pads

  component OCMTR4 port( EN : in std_logic; A : in std_logic; Z : out std_logic); end component;
  component OCMTR12 port( EN : in std_logic; A : in std_logic; Z : out std_logic); end component;
  component OCMTR24 port( EN : in std_logic; A : in std_logic; Z : out std_logic); end component;

  -- bidirectional pads

  component BICM3V4 port( IO : inout std_logic; EN : in std_logic; A : in std_logic; Z : out std_logic); end component;
  component BICM3V12 port( IO : inout std_logic; EN : in std_logic; A : in std_logic; Z : out std_logic); end component;
  component BICM3V24 port( IO : inout std_logic; EN : in std_logic; A : in std_logic; Z : out std_logic); end component;

end;

library ieee;
use ieee.std_logic_1164.all;
library techmap;
use techmap.gencomp.all;
-- pragma translate_off
library umc;
use umc.ICMT3V;
-- pragma translate_on

entity rhumc_inpad is
  generic (level : integer := 0; voltage : integer := 0);
  port (pad : in std_logic; o : out std_logic);
end; 
architecture rtl of rhumc_inpad is
  component ICMT3V port( A : in std_logic; Z : out std_logic); end component;
begin
  ip : ICMT3V port map (a => pad, z => o);
end;

library ieee;
use ieee.std_logic_1164.all;
library techmap;
use techmap.gencomp.all;
-- pragma translate_off
library umc;
use umc.BICM3V4;
use umc.BICM3V12;
use umc.BICM3V24;
-- pragma translate_on

entity rhumc_iopad  is
  generic (level : integer := 0; slew : integer := 0;
	   voltage : integer := 0; strength : integer := 0);
  port (pad : inout std_logic; i, en : in std_logic; o : out std_logic);
end ;
architecture rtl of rhumc_iopad is
  component BICM3V4 port( IO : inout std_logic; EN : in std_logic; A : in std_logic; Z : out std_logic); end component;
  component BICM3V12 port( IO : inout std_logic; EN : in std_logic; A : in std_logic; Z : out std_logic); end component;
  component BICM3V24 port( IO : inout std_logic; EN : in std_logic; A : in std_logic; Z : out std_logic); end component;
begin
  f4 : if (strength <= 4)  generate
      op : BICM3V4 port map (a => i, en => en, io => pad, z => o);
  end generate;
  f12 : if (strength > 4)  and (strength <= 12)  generate
      op : BICM3V12 port map (a => i, en => en, io => pad, z => o);
  end generate;
  f24 : if (strength > 16)  generate
      op : BICM3V24 port map (a => i, en => en, io => pad, z => o);
  end generate;
end;

library ieee;
use ieee.std_logic_1164.all;
library techmap;
use techmap.gencomp.all;
-- pragma translate_off
library umc;
use umc.OCM3V4;
use umc.OCM3V12;
use umc.OCM3V24;
-- pragma translate_on

entity rhumc_outpad  is
  generic (level : integer := 0; slew : integer := 0;
	   voltage : integer := 0; strength : integer := 0);
  port (pad : out std_logic; i : in std_logic);
end ;
architecture rtl of rhumc_outpad is
  component OCM3V4 port( Z : out std_logic; A : in std_logic); end component;
  component OCM3V12 port( Z : out std_logic; A : in std_logic); end component;
  component OCM3V24 port( Z : out std_logic; A : in std_logic); end component;
begin
  f4 : if (strength <= 4)  generate
      op : OCM3V4 port map (a => i, z => pad);
  end generate;
  f12 : if (strength > 4) and (strength <= 12)  generate
      op : OCM3V12 port map (a => i, z => pad);
  end generate;
  f24 : if (strength > 12) generate
      op : OCM3V24 port map (a => i, z => pad);
  end generate;
end;

library ieee;
use ieee.std_logic_1164.all;
library techmap;
use techmap.gencomp.all;
-- pragma translate_off
library umc;
use umc.OCMTR4;
use umc.OCMTR12;
use umc.OCMTR24;
-- pragma translate_on

entity rhumc_toutpad  is
  generic (level : integer := 0; slew : integer := 0;
	   voltage : integer := 0; strength : integer := 0);
  port (pad : out std_logic; i, en : in std_logic);
end ;
architecture rtl of rhumc_toutpad is
  component OCMTR4 port( EN : in std_logic; A : in std_logic; Z : out std_logic); end component;
  component OCMTR12 port( EN : in std_logic; A : in std_logic; Z : out std_logic); end component;
  component OCMTR24 port( EN : in std_logic; A : in std_logic; Z : out std_logic); end component;
begin
  f4 : if (strength <= 4)  generate
      op : OCMTR4 port map (a => i, en => en, z => pad);
  end generate;
  f12  : if (strength > 4) and (strength <= 12)  generate
      op : OCMTR12 port map (a => i, en => en, z => pad);
  end generate;
  f24 : if (strength > 12) generate
      op : OCMTR24 port map (a => i, en => en, z => pad);
  end generate;
end;
