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
-- Package: 	pad_xilinx_gen
-- File:	pad_xilinx_gen.vhd
-- Author:	Jiri Gaisler - Gaisler Research
-- Description:	Xilinx pads wrappers
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
library techmap;
use techmap.gencomp.all;
-- pragma translate_off
library unisim;
use unisim.IBUF;
-- pragma translate_on

entity virtex_inpad is
  generic (level : integer := 0; voltage : integer := x33v);
  port (pad : in std_ulogic; o : out std_ulogic);
end; 
architecture rtl of virtex_inpad is
  component IBUF generic(
      CAPACITANCE : string := "DONT_CARE"; IOSTANDARD : string := "LVCMOS25");
    port (O : out std_ulogic; I : in std_ulogic); end component;
begin
  pci0 : if level = pci33 generate
    pci_5 : if voltage = x50v generate
      ip : IBUF generic map (IOSTANDARD => "PCI33_5") port map (O => o, I => pad);
    end generate;
    pci_3 : if voltage /= x50v generate
      ip : IBUF generic map (IOSTANDARD => "PCI33_3") port map (O => o, I => pad);
    end generate;
  end generate;
  ttl0 : if level = ttl generate
    ip : IBUF generic map (IOSTANDARD => "LVTTL") port map (O => o, I => pad);
  end generate;
  cmos0 : if level = cmos generate
    cmos_33 : if voltage = x33v generate
      ip : IBUF generic map (IOSTANDARD => "LVCMOS33") port map (O => o, I => pad);
    end generate;
    cmos_25 : if voltage /= x33v generate
      ip : IBUF generic map (IOSTANDARD => "LVCMOS25") port map (O => o, I => pad);
    end generate;
  end generate;
  sstl2x : if level = sstl2_i generate
    ip : IBUF generic map (IOSTANDARD => "SSTL2_I") port map (O => o, I => pad);
  end generate;
  sstl2y : if level = sstl2_ii generate
    ip : IBUF generic map (IOSTANDARD => "SSTL2_II") port map (O => o, I => pad);
  end generate;
  gen0 : if (level /= pci33) and (level /= ttl) and (level /= cmos) 
	and (level /= sstl2_i)and (level /= sstl2_ii) generate
    ip : IBUF port map (O => o, I => pad);
  end generate;
end;

library ieee;
use ieee.std_logic_1164.all;
library techmap;
use techmap.gencomp.all;
-- pragma translate_off
library unisim;
use unisim.IOBUF;
-- pragma translate_on

entity virtex_iopad  is
  generic (level : integer := 0; slew : integer := 0;
	   voltage : integer := x33v; strength : integer := 12);
  port (pad : inout std_ulogic; i, en : in std_ulogic; o : out std_ulogic);
end ;
architecture rtl of virtex_iopad is
  component IOBUF generic (
      CAPACITANCE : string := "DONT_CARE"; DRIVE : integer := 12;
      IOSTANDARD  : string := "LVCMOS25"; SLEW : string := "SLOW");
    port (O : out std_ulogic; IO : inout std_logic; I, T : in std_ulogic); end component;
begin
  pci0 : if level = pci33 generate
    pci_5 : if voltage = x50v generate
      op : IOBUF generic map (IOSTANDARD => "PCI33_5")
                 port map (O => o, IO => pad, I => i, T => en);
    end generate;
    pci_3 : if voltage /= x50v generate
      op : IOBUF generic map (IOSTANDARD => "PCI33_3")
                 port map (O => o, IO => pad, I => i, T => en);
    end generate;
  end generate;
  ttl0 : if level = ttl generate
    slow0 : if slew = 0 generate
      op : IOBUF generic map (drive => strength, IOSTANDARD => "LVTTL")
                 port map (O => o, IO => pad, I => i, T => en);
    end generate;
    fast0 : if slew /= 0 generate
      op : IOBUF generic map (drive => strength, IOSTANDARD => "LVTTL", SLEW => "FAST")
                 port map (O => o, IO => pad, I => i, T => en);
    end generate;
  end generate;
  cmos0 : if level = cmos generate
    slow0 : if slew = 0 generate
      op : IOBUF generic map (drive => strength, IOSTANDARD => "LVCMOS33")
                 port map (O => o, IO => pad, I => i, T => en);
    end generate;
    fast0 : if slew /= 0 generate
      op : IOBUF generic map (drive => strength, IOSTANDARD => "LVCMOS33", SLEW => "FAST")
                 port map (O => o, IO => pad, I => i, T => en);
    end generate;
  end generate;
  sstl2x : if level = sstl2_i generate
    op : IOBUF generic map (drive => strength, IOSTANDARD => "SSTL2_I")
                 port map (O => o, IO => pad, I => i, T => en);
  end generate;
  sstl2y : if level = sstl2_ii generate
    op : IOBUF generic map (drive => strength, IOSTANDARD => "SSTL2_II")
                 port map (O => o, IO => pad, I => i, T => en);
  end generate;
  gen0 : if (level /= pci33) and (level /= ttl) and (level /= cmos) and 
	(level /= sstl2_i) and (level /= sstl2_ii) generate
    op : IOBUF port map (O => o, IO => pad, I => i, T => en);
  end generate;
end;

library ieee;
use ieee.std_logic_1164.all;
library techmap;
use techmap.gencomp.all;
-- pragma translate_off
library unisim;
use unisim.OBUF;
-- pragma translate_on

entity virtex_outpad  is
  generic (level : integer := 0; slew : integer := 0;
	   voltage : integer := 0; strength : integer := 12);
  port (pad : out std_ulogic; i : in std_ulogic);
end ;
architecture rtl of virtex_outpad is
  component OBUF generic (
      CAPACITANCE : string := "DONT_CARE"; DRIVE : integer := 12;
      IOSTANDARD  : string := "LVCMOS25"; SLEW : string := "SLOW");
    port (O : out std_ulogic; I : in std_ulogic); end component;
begin
  pci0 : if level = pci33 generate
    pci_5 : if voltage = x50v generate
      op : OBUF generic map (drive => strength, IOSTANDARD => "PCI33_5")
                port map (O => pad, I => i);
    end generate;
    pci_3 : if voltage /= x50v generate
      op : OBUF generic map (drive => strength, IOSTANDARD => "PCI33_3")
                port map (O => pad, I => i);
    end generate;
  end generate;
  ttl0 : if level = ttl generate
    slow0 : if slew = 0 generate
      op : OBUF generic map (drive => strength, IOSTANDARD => "LVTTL")
                port map (O => pad, I => i);
    end generate;
    fast0 : if slew /= 0 generate
      op : OBUF generic map (drive => strength, IOSTANDARD => "LVTTL", SLEW => "FAST")
                port map (O => pad, I => i);
    end generate;
  end generate;
  cmos0 : if level = cmos generate
    slow0 : if slew = 0 generate
      op : OBUF generic map (drive => strength, IOSTANDARD => "LVCMOS33")
                port map (O => pad, I => i);
    end generate;
    fast0 : if slew /= 0 generate
      op : OBUF generic map (drive => strength, IOSTANDARD => "LVCMOS33", SLEW => "FAST")
                port map (O => pad, I => i);
    end generate;
  end generate;
  sstl2x : if level = sstl2_i generate
      op : OBUF generic map (drive => strength, IOSTANDARD => "SSTL2_I")
                port map (O => pad, I => i);
  end generate;
  sstl2y : if level = sstl2_ii generate
      op : OBUF generic map (drive => strength, IOSTANDARD => "SSTL2_II")
                port map (O => pad, I => i);
  end generate;
  gen0 : if (level /= pci33) and (level /= ttl) and (level /= cmos) and 
	(level /= sstl2_i) and (level /= sstl2_ii) generate
      op : OBUF port map (O => pad, I => i);
  end generate;
end;

library ieee;
use ieee.std_logic_1164.all;
library techmap;
use techmap.gencomp.all;
-- pragma translate_off
library unisim;
use unisim.OBUFT;
-- pragma translate_on

entity virtex_toutpad  is
  generic (level : integer := 0; slew : integer := 0;
	   voltage : integer := 0; strength : integer := 12);
  port (pad : out std_ulogic; i, en : in std_ulogic);
end ;
architecture rtl of virtex_toutpad is
  component OBUFT generic (
      CAPACITANCE : string := "DONT_CARE"; DRIVE : integer := 12;
      IOSTANDARD  : string := "LVCMOS25"; SLEW : string := "SLOW");
    port (O : out std_ulogic; I, T : in std_ulogic); end component;
begin
  pci0 : if level = pci33 generate
    pci_5 : if voltage = x50v generate
      op : OBUFT generic map (drive => strength, IOSTANDARD => "PCI33_5")
                 port map (O => pad, I => i, T => en);
    end generate;
    pci_3 : if voltage /= x50v generate
      op : OBUFT generic map (drive => strength, IOSTANDARD => "PCI33_3")
                 port map (O => pad, I => i, T => en);
    end generate;
  end generate;
  ttl0 : if level = ttl generate
    slow0 : if slew = 0 generate
      op : OBUFT generic map (drive => strength, IOSTANDARD => "LVTTL")
                 port map (O => pad, I => i, T => en);
    end generate;
    fast0 : if slew /= 0 generate
      op : OBUFT generic map (drive => strength, IOSTANDARD => "LVTTL", SLEW => "FAST")
                 port map (O => pad, I => i, T => en);
    end generate;
  end generate;
  cmos0 : if level = cmos generate
    slow0 : if slew = 0 generate
      op : OBUFT generic map (drive => strength, IOSTANDARD => "LVCMOS33")
                 port map (O => pad, I => i, T => en);
    end generate;
    fast0 : if slew /= 0 generate
      op : OBUFT generic map (drive => strength, IOSTANDARD => "LVCMOS33", SLEW => "FAST")
                 port map (O => pad, I => i, T => en);
    end generate;
  end generate;
  gen0 : if (level /= pci33) and (level /= ttl) and (level /= cmos) generate
    op : OBUFT port map (O => pad, I => i, T => en);
  end generate;
end;

library ieee;
use ieee.std_logic_1164.all;
library techmap;
use techmap.gencomp.all;
use techmap.allpads.all;
-- pragma translate_off
library unisim;
-- pragma translate_on

entity virtex_odpad  is
  generic (level : integer := 0; slew : integer := 0;
	   voltage : integer := 0; strength : integer := 12);
  port (pad : out std_ulogic; i : in std_ulogic);
end ;
architecture rtl of virtex_odpad is
signal gnd : std_ulogic;
begin
  gnd <= '0';
  p : virtex_toutpad generic map (level, slew, voltage, strength)
    port map (pad, gnd, i); 
end;

library ieee;
use ieee.std_logic_1164.all;
library techmap;
use techmap.gencomp.all;
-- pragma translate_off
library unisim;
use unisim.IBUFG;
use unisim.IBUF;
use unisim.BUFGMUX;
use unisim.BUFG;
-- pragma translate_on

entity virtex_clkpad is
  generic (level : integer := 0; voltage : integer := x33v; arch : integer := 0);
  port (pad : in std_ulogic; o : out std_ulogic);
end; 
architecture rtl of virtex_clkpad is
  component IBUFG  generic(
      CAPACITANCE : string := "DONT_CARE"; IOSTANDARD : string := "LVCMOS25");
    port (O : out std_logic; I : in std_logic); end component;
  component IBUF generic(
      CAPACITANCE : string := "DONT_CARE"; IOSTANDARD : string := "LVCMOS25");
    port (O : out std_ulogic; I : in std_ulogic); end component;
  component BUFGMUX port (O : out std_logic; I0, I1, S : in std_logic); end component;
  component BUFG port (O : out std_logic; I : in std_logic); end component;
signal gnd, ol : std_ulogic;
begin
  gnd <= '0';
  g0 : if arch = 0 generate
    pci0 : if level = pci33 generate
      pci_5 : if voltage = x50v generate
        ip : IBUFG generic map (IOSTANDARD => "PCI33_5") port map (O => o, I => pad);
      end generate;
      pci_3 : if voltage /= x50v generate
        ip : IBUFG generic map (IOSTANDARD => "PCI33_3") port map (O => o, I => pad);
      end generate;
    end generate;
    ttl0 : if level = ttl generate
      ip : IBUFG generic map (IOSTANDARD => "LVTTL") port map (O => o, I => pad);
    end generate;
    cmos0 : if level = cmos generate
      ip : IBUFG generic map (IOSTANDARD => "LVCMOS33") port map (O => o, I => pad);
    end generate;
    gen0 : if (level /= pci33) and (level /= ttl) and (level /= cmos) generate
      ip : IBUFG port map (O => o, I => pad);
    end generate;
  end generate;
  g1 : if arch = 1 generate
    pci0 : if level = pci33 generate
      pci_5 : if voltage = x50v generate
        ip : IBUF generic map (IOSTANDARD => "PCI33_5") port map (O => ol, I => pad);
        bf : BUFGMUX port map (O => o, I0 => ol, I1 => gnd, S => gnd);
      end generate;
      pci_3 : if voltage /= x50v generate
        ip : IBUF generic map (IOSTANDARD => "PCI33_3") port map (O => ol, I => pad);
        bf : BUFGMUX port map (O => o, I0 => ol, I1 => gnd, S => gnd);
      end generate;
    end generate;
    ttl0 : if level = ttl generate
      ip : IBUF generic map (IOSTANDARD => "LVTTL") port map (O => ol, I => pad);
      bf : BUFGMUX port map (O => o, I0 => ol, I1 => gnd, S => gnd);
    end generate;
    cmos0 : if level = cmos generate
      ip : IBUF generic map (IOSTANDARD => "LVCMOS33") port map (O => ol, I => pad);
      bf : BUFGMUX port map (O => o, I0 => ol, I1 => gnd, S => gnd);
    end generate;
    gen0 : if (level /= pci33) and (level /= ttl) and (level /= cmos) generate
      ip : IBUF port map (O => ol, I => pad);
      bf : BUFGMUX port map (O => o, I0 => ol, I1 => gnd, S => gnd);
    end generate;
  end generate;
  g2 : if arch = 2 generate
    pci0 : if level = pci33 generate
      pci_5 : if voltage = x50v generate
        ip : IBUFG generic map (IOSTANDARD => "PCI33_5") port map (O => ol, I => pad);
        bf : BUFG port map (O => o, I => ol);
      end generate;
      pci_3 : if voltage /= x50v generate
        ip : IBUFG generic map (IOSTANDARD => "PCI33_3") port map (O => ol, I => pad);
        bf : BUFG port map (O => o, I => ol);
      end generate;
    end generate;
    ttl0 : if level = ttl generate
      ip : IBUFG generic map (IOSTANDARD => "LVTTL") port map (O => ol, I => pad);
      bf : BUFG port map (O => o, I => ol);
    end generate;
    cmos0 : if level = cmos generate
      ip : IBUFG generic map (IOSTANDARD => "LVCMOS33") port map (O => ol, I => pad);
      bf : BUFG port map (O => o, I => ol);
    end generate;
    gen0 : if (level /= pci33) and (level /= ttl) and (level /= cmos) generate
      ip : IBUFG port map (O => ol, I => pad);
      bf : BUFG port map (O => o, I => ol);
    end generate;
  end generate;
end; 

library ieee;
use ieee.std_logic_1164.all;
library techmap;
use techmap.gencomp.all;
-- pragma translate_off
library unisim;
use unisim.OBUFDS_LVDS_25;
use unisim.OBUFDS_LVDS_33;
-- pragma translate_on

entity virtex_outpad_ds  is
  generic (level : integer := lvds; voltage : integer := x33v);
  port (padp, padn : out std_ulogic; i : in std_ulogic);
end ;
architecture rtl of virtex_outpad_ds is
  component OBUFDS_LVDS_25
     port ( O : out std_ulogic; OB : out std_ulogic; I : in std_ulogic);
  end component;
  component OBUFDS_LVDS_33
     port ( O : out std_ulogic; OB : out std_ulogic; I : in std_ulogic);
  end component;
begin
  xlvds : if level = lvds generate
    lvds_33 : if voltage = x33v generate
      op : OBUFDS_LVDS_33 port map (O => padp, OB => padn, I => i);
    end generate;
    lvds_25 : if voltage /= x33v generate
      op : OBUFDS_LVDS_25 port map (O => padp, OB => padn, I => i);
    end generate;
  end generate;
end;

library ieee;
use ieee.std_logic_1164.all;
library techmap;
use techmap.gencomp.all;
-- pragma translate_off
library unisim;
use unisim.IBUFDS_LVDS_25;
use unisim.IBUFDS_LVDS_33;
-- pragma translate_on

entity virtex_inpad_ds is
  generic (level : integer := lvds; voltage : integer := x33v);
  port (padp, padn : in std_ulogic; o : out std_ulogic);
end; 

architecture rtl of virtex_inpad_ds is
  component IBUFDS_LVDS_25
     port ( O : out std_ulogic; I : in std_ulogic; IB : in std_ulogic);
  end component;
  component IBUFDS_LVDS_33
     port ( O : out std_ulogic; I : in std_ulogic; IB : in std_ulogic);
  end component;
begin
  xlvds : if level = lvds generate
    lvds_33 : if voltage = x33v generate
      ip : IBUFDS_LVDS_33 port map (O => o, I => padp, IB => padn);
    end generate;
    lvds_25 : if voltage /= x33v generate
      ip : IBUFDS_LVDS_25 port map (O => o, I => padp, IB => padn);
    end generate;
  end generate;
  beh : if level /= lvds generate
    o <= padp after 1 ns;
  end generate;
end;

