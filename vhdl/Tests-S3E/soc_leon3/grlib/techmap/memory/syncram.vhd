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
-- Entity: 	syncram
-- File:	syncram.vhd
-- Author:	Jiri Gaisler - Gaisler Research
-- Description:	syncronous 1-port ram with tech selection
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
library techmap;
use techmap.gencomp.all;
  
entity syncram is
  generic (tech : integer := 0; abits : integer := 6; dbits : integer := 8 );
  port (
    clk      : in std_ulogic;
    address  : in std_logic_vector((abits -1) downto 0);
    datain   : in std_logic_vector((dbits -1) downto 0);
    dataout  : out std_logic_vector((dbits -1) downto 0);
    enable   : in std_ulogic;
    write    : in std_ulogic); 
end;

architecture rtl of syncram is
  component altera_syncram
  generic ( abits : integer := 10; dbits : integer := 8 );
  port (
    clk      : in std_ulogic;
    address  : in std_logic_vector((abits -1) downto 0);
    datain   : in std_logic_vector((dbits -1) downto 0);
    dataout  : out std_logic_vector((dbits -1) downto 0);
    enable   : in std_ulogic;
    write    : in std_ulogic); 
  end component;
  component axcel_syncram
  generic ( abits : integer := 10; dbits : integer := 8 );
  port (
    clk      : in std_ulogic;
    address  : in std_logic_vector((abits -1) downto 0);
    datain   : in std_logic_vector((dbits -1) downto 0);
    dataout  : out std_logic_vector((dbits -1) downto 0);
    enable   : in std_ulogic;
    write    : in std_ulogic); 
  end component;
  component generic_syncram
  generic (abits : integer := 10; dbits : integer := 8 );
  port (
    clk      : in std_ulogic;
    address  : in std_logic_vector((abits -1) downto 0);
    datain   : in std_logic_vector((dbits -1) downto 0);
    dataout  : out std_logic_vector((dbits -1) downto 0);
    write    : in std_ulogic); 
  end component;
  component ihp25_syncram
    generic ( abits : integer := 10; dbits : integer := 8 );
    port (
      clk      : in std_logic;
      address  : in std_logic_vector(abits -1 downto 0);
      datain   : in std_logic_vector(dbits -1 downto 0);
      dataout  : out std_logic_vector(dbits -1 downto 0);
      enable   : in std_logic;
      write    : in std_logic);
  end component;
  component lattice_syncram
  generic ( abits : integer := 10; dbits : integer := 8 );
  port (
    clk      : in std_ulogic;
    address  : in std_logic_vector((abits -1) downto 0);
    datain   : in std_logic_vector((dbits -1) downto 0);
    dataout  : out std_logic_vector((dbits -1) downto 0);
    enable   : in std_ulogic;
    write    : in std_ulogic); 
  end component;
  component proasic_syncram
  generic ( abits : integer := 10; dbits : integer := 8 );
  port (
    clk      : in std_ulogic;
    address  : in std_logic_vector((abits -1) downto 0);
    datain   : in std_logic_vector((dbits -1) downto 0);
    dataout  : out std_logic_vector((dbits -1) downto 0);
    enable   : in std_ulogic;
    write    : in std_ulogic); 
  end component;
  component proasic3_syncram
  generic ( abits : integer := 10; dbits : integer := 8 );
  port (
    clk      : in std_ulogic;
    address  : in std_logic_vector((abits -1) downto 0);
    datain   : in std_logic_vector((dbits -1) downto 0);
    dataout  : out std_logic_vector((dbits -1) downto 0);
    enable   : in std_ulogic;
    write    : in std_ulogic); 
  end component;
  component rh_lib18t_syncram_2p
  generic ( abits : integer := 6; dbits : integer := 8);
  port (
    rclk     : in std_ulogic;
    renable  : in std_ulogic;
    raddress : in std_logic_vector((abits -1) downto 0);
    dataout  : out std_logic_vector((dbits -1) downto 0);
    write    : in std_ulogic;
    waddress : in std_logic_vector((abits -1) downto 0);
    datain   : in std_logic_vector((dbits -1) downto 0);
    diagin   : in std_logic_vector(3 downto 0));
  end component;
  component rhumc_syncram
  generic ( abits : integer := 10; dbits : integer := 8 );
  port (
    clk      : in std_ulogic;
    address  : in std_logic_vector(abits -1 downto 0);
    datain   : in std_logic_vector(dbits -1 downto 0);
    dataout  : out std_logic_vector(dbits -1 downto 0);
    enable   : in std_ulogic;
    write    : in std_ulogic);
  end component;
  component virage_syncram
  generic ( abits : integer := 10; dbits : integer := 8 );
  port (
    clk      : in std_ulogic;
    address  : in std_logic_vector(abits -1 downto 0);
    datain   : in std_logic_vector(dbits -1 downto 0);
    dataout  : out std_logic_vector(dbits -1 downto 0);
    enable   : in std_ulogic;
    write    : in std_ulogic);
  end component;
  component virtex_syncram
  generic ( abits : integer := 6; dbits : integer := 8 );
  port (
    clk      : in std_ulogic;
    address  : in std_logic_vector((abits -1) downto 0);
    datain   : in std_logic_vector((dbits -1) downto 0);
    dataout  : out std_logic_vector((dbits -1) downto 0);
    enable   : in std_ulogic;
    write    : in std_ulogic); 
  end component;
  component virtex2_syncram
  generic ( abits : integer := 10; dbits : integer := 8 );
  port (
    clk      : in std_ulogic;
    address  : in std_logic_vector((abits -1) downto 0);
    datain   : in std_logic_vector((dbits -1) downto 0);
    dataout  : out std_logic_vector((dbits -1) downto 0);
    enable   : in std_ulogic;
    write    : in std_ulogic); 
  end component;

  signal gnd4 : std_logic_vector(3 downto 0);

begin

  inf : if tech = inferred generate 
    u0 : generic_syncram generic map (abits, dbits)
         port map (clk, address, datain, dataout, write);
  end generate;

  xcv : if tech = virtex generate 
    u0 : virtex_syncram generic map (abits, dbits)
         port map (clk, address, datain, dataout, enable, write);
  end generate;

  xc2v : if (tech = virtex2) or (tech = spartan3) or (tech = virtex4) generate 
    u0 : virtex2_syncram generic map (abits, dbits)
         port map (clk, address, datain, dataout, enable, write);
  end generate;

  vir  : if tech = memvirage generate 
    u0 : virage_syncram generic map (abits, dbits)
         port map (clk, address, datain, dataout, enable, write);
  end generate;

  axc  : if tech = axcel generate 
    u0 : axcel_syncram generic map (abits, dbits)
         port map (clk, address, datain, dataout, enable, write);
  end generate;

  proa : if tech = proasic generate 
    u0 : proasic_syncram generic map (abits, dbits)
         port map (clk, address, datain, dataout, enable, write);
  end generate;

  rhu  : if tech = rhumc generate 
    u0 : rhumc_syncram generic map (abits, dbits)
         port map (clk, address, datain, dataout, enable, write);
  end generate;

  proa3 : if tech = proasic3 generate 
    u0 : proasic3_syncram generic map (abits, dbits)
         port map (clk, address, datain, dataout, enable, write);
  end generate;

  ihp : if tech = ihp25 generate
    u0 : ihp25_syncram generic map(abits, dbits)
         port map(clk, address, datain, dataout, enable, write);
  end generate;

  alt : if tech = altera generate
    u0 : altera_syncram generic map(abits, dbits)
         port map(clk, address, datain, dataout, enable, write);
  end generate;

  rht : if tech = rhlib18t generate
    u0 : rh_lib18t_syncram_2p generic map(abits, dbits)
         port map(clk, enable, address, dataout, write, address, 
	          datain, gnd4);
  end generate;

  lat : if tech = lattice generate
    u0 : lattice_syncram generic map(abits, dbits)
         port map(clk, address, datain, dataout, enable, write);
  end generate;

  gnd4 <= "0000";

-- pragma translate_off
  noram : if has_sram(tech) = 0 generate
    x : process
    begin 
      assert false report "synram: technology " & tech_table(tech) &
	" not supported"
      severity failure;
      wait;
    end process;
  end generate; 
-- pragma translate_on
end;

