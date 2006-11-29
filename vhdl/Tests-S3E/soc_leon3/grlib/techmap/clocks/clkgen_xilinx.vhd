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
-- Entity: 	various
-- File:	clkgen_xilinx.vhd
-- Author:	Jiri Gaisler, Gaisler Research
-- Author:	Richard Pender, Pender Electronic Design
-- Description:	Clock generators for Virtex and Virtex-2 fpgas
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
-- pragma translate_off
library grlib;
use grlib.stdlib.all;
library unisim;
use unisim.BUFG;
use unisim.DCM;
use unisim.BUFGDLL;
-- pragma translate_on
library techmap;
use techmap.gencomp.all;
use techmap.libclk.all;


------------------------------------------------------------------
-- Virtex2 clock generator ---------------------------------------
------------------------------------------------------------------

entity clkgen_virtex2 is
  generic (
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
    clk2x   : out std_logic;			-- double clock
    sdclk   : out std_logic;			-- SDRAM clock
    pciclk  : out std_logic;			-- PCI clock
    cgi     : in clkgen_in_type;
    cgo     : out clkgen_out_type);
end; 

architecture struct of clkgen_virtex2 is 

  component BUFG port (O : out std_logic; I : in std_logic); end component;

  component DCM
    generic (
      CLKFX_MULTIPLY : integer := 1 ;
      CLKFX_DIVIDE : integer := 1
    );
    port (
      CLKFB    : in  std_logic;
      CLKIN    : in  std_logic;
      DSSEN    : in  std_logic;
      PSCLK    : in  std_logic;
      PSEN     : in  std_logic;
      PSINCDEC : in  std_logic;
      RST      : in  std_logic;
      CLK0     : out std_logic;
      CLK90    : out std_logic;
      CLK180   : out std_logic;
      CLK270   : out std_logic;
      CLK2X    : out std_logic;
      CLK2X180 : out std_logic;
      CLKDV    : out std_logic;
      CLKFX    : out std_logic;
      CLKFX180 : out std_logic;
      LOCKED   : out std_logic;
      PSDONE   : out std_logic;
      STATUS   : out std_logic_vector (7 downto 0));
  end component;
  component BUFGDLL port (O : out std_logic; I : in std_logic); end component;

constant VERSION : integer := 1;
constant CLKIN_PERIOD_ST : string := "25";
attribute CLKIN_PERIOD : string;
attribute CLKIN_PERIOD of dll0: label is CLKIN_PERIOD_ST;
signal gnd, Clk_i, Clk_j, Clk_k, Clk_l, clk_m, clk_n, clk_o, clk_p, clk_i2, clk_sd, clk_r, dll0rst, dll0lock, dll1lock, dll2xlock : std_logic;
signal dll1rst, dll2xrst : std_logic_vector(0 to 3);
signal Clk0B, Clkint, pciclkint : std_logic;

begin

  gnd <= '0';
  clk <= clk_i when (CLK2XEN = 0) else clk_p;
  clkn <= clk_m; clk2x <= clk_i2;

  c0 : if (PCISYSCLK = 0) or (PCIEN = 0) generate
    clkint <= clkin;
  end generate;

  c2 : if PCIEN /= 0 generate
    pciclkint <= pciclkin;
    p3 : if PCISYSCLK = 1 generate clkint <= pciclkint; end generate;
    p0 : if PCIDLL = 1 generate
      u1 : BUFGDLL port map (I => pciclkint, O => pciclk);
    end generate;
    p1 : if PCIDLL = 0 generate 
      u1 : BUFG port map (I => pciclkint, O => pciclk);
    end generate;
  end generate;

  c3 : if PCIEN = 0 generate 
    pciclk <= '0';
  end generate;

  bufg0 : BUFG port map (I => Clk0B, O => Clk_i);
  bufg1 : BUFG port map (I => Clk_j, O => Clk_k);
  bufg2 : BUFG port map (I => Clk_l, O => Clk_m);
  buf34gen : if (CLK2XEN /= 0) generate
    bufg3 : BUFG port map (I => Clk_n, O => Clk_i2);
    bufg4 : BUFG port map (I => clk_o, O => clk_p);
  end generate;
  dll0rst <= not cgi.pllrst;
  dll0 : DCM 
    generic map (CLKFX_MULTIPLY => clk_mul, CLKFX_DIVIDE => clk_div)
    port map ( CLKIN => clkint, CLKFB => Clk_k, DSSEN => gnd, PSCLK => gnd,
    PSEN => gnd, PSINCDEC => gnd, RST => dll0rst, CLK0 => Clk_j,
    CLKFX => Clk0B, CLKFX180 => Clk_l, LOCKED => dll0lock);


  clk2xgen : if (CLK2XEN /= 0) generate
    dll2x : DCM generic map (CLKFX_MULTIPLY => 2, CLKFX_DIVIDE => 2)
      port map ( CLKIN => Clk_i, CLKFB => clk_p, DSSEN => gnd, PSCLK => gnd,
      PSEN => gnd, PSINCDEC => gnd, RST => dll2xrst(0), CLK0 => clk_o,
                 CLK2X => clk_n,  LOCKED => dll2xlock);
    rstdel2x : process (Clk_i, dll0lock)
    begin
      if dll0lock = '0' then dll2xrst <= (others => '1');
      elsif rising_edge(Clk_i) then
	dll2xrst <= dll2xrst(1 to 3) & '0';
      end if;
    end process;      
  end generate;

  clk_sd1 : if (CLK2XEN = 0) generate
    clk_i2 <= clk_i; dll2xlock <= dll0lock;
    clk_sd <= clk_i;
  end generate;

  clk_sd2 : if (CLK2XEN = 1) generate clk_sd <= clk_p; end generate;  
  clk_sd3 : if (CLK2XEN = 2) generate clk_sd <= clk_i2; end generate;

  
  sd0 : if (SDRAMEN /= 0) and (SDINVCLK=0) generate
    cgo.clklock <= dll1lock;
    dll1 : DCM generic map (CLKFX_MULTIPLY => 2, CLKFX_DIVIDE => 2)
      port map ( CLKIN => clk_sd, CLKFB => cgi.pllref, DSSEN => gnd, PSCLK => gnd,
      PSEN => gnd, PSINCDEC => gnd, RST => dll1rst(0), CLK0 => sdclk, --CLK2X => clk2x, 
      LOCKED => dll1lock);
    rstdel : process (clk_sd, dll2xlock)
    begin
      if dll2xlock = '0' then dll1rst <= (others => '1');
      elsif rising_edge(clk_sd) then
	dll1rst <= dll1rst(1 to 3) & '0';
      end if;
    end process;
  end generate;

  sd1 : if ((SDRAMEN = 0) or (SDINVCLK = 1)) and (CLK2XEN /= 2) generate
    sdclk <= clk_i when (SDINVCLK = 0) else not clk_i;     
    cgo.clklock <= dll0lock when (CLK2XEN = 0) else dll2xlock;
  end generate;

  sd1_2x : if ((SDRAMEN = 0) or (SDINVCLK = 1)) and (CLK2XEN = 2) generate
    sdclk <= clk_i2 when (SDINVCLK = 0) else not clk_i2;
    cgo.clklock <= dll2xlock;
  end generate;  

  
  cgo.pcilock <= '1';

-- pragma translate_off
  bootmsg : report_version 
  generic map (
    "clkgen_virtex2" & ": virtex-2 sdram/pci clock generator, version " & tost(VERSION),
    "clkgen_virtex2" & ": Frequency " &  tost(freq) & " KHz, DCM divisor " & tost(clk_mul) & "/" & tost(clk_div));
-- pragma translate_on


end;

library ieee;
use ieee.std_logic_1164.all;
-- pragma translate_off
library grlib;
use grlib.stdlib.all;
library unisim;
use unisim.BUFG;
use unisim.CLKDLL;
use unisim.BUFGDLL;
-- pragma translate_on
library techmap;
use techmap.gencomp.all;
use techmap.libclk.all;

entity clkgen_virtex is
  generic (
    clk_mul  : integer := 1; 
    clk_div  : integer := 1;
    sdramen  : integer := 0;
    sdinvclk : integer := 0;
    pcien    : integer := 0;
    pcidll   : integer := 0;
    pcisysclk: integer := 0);
port (
    clkin   : in  std_logic;
    pciclkin: in  std_logic;
    clk     : out std_logic;			-- main clock
    clkn    : out std_logic;			-- inverted main clock
    clk2x   : out std_logic;			-- double clock
    sdclk   : out std_logic;			-- SDRAM clock
    pciclk  : out std_logic;			-- PCI clock
    cgi     : in clkgen_in_type;
    cgo     : out clkgen_out_type
);
end;

architecture rtl of clkgen_virtex is

  component BUFG port (O : out std_logic; I : in std_logic); end component;
  component CLKDLL
    port (
      CLK0    : out std_ulogic;
      CLK180  : out std_ulogic;
      CLK270  : out std_ulogic;
      CLK2X   : out std_ulogic;
      CLK90   : out std_ulogic;
      CLKDV   : out std_ulogic;
      LOCKED  : out std_ulogic;
      CLKFB   : in  std_ulogic;
      CLKIN   : in  std_ulogic;
      RST     : in  std_ulogic);
  end component;
  component BUFGDLL port (O : out std_logic; I : in std_logic); end component;

signal gnd, Clk_i, Clk_j, Clk_k, dll0rst, dll0lock, dll1lock : std_logic;
signal dll1rst : std_logic_vector(0 to 3);
signal Clk0B, Clkint, CLK2XL, CLKDV, CLK180, pciclkint : std_logic;

begin

  gnd <= '0'; clk <= clk_i; clkn <= not clk_i;

  c0 : if (PCISYSCLK = 0) or (PCIEN = 0) generate
    clkint <= clkin;
  end generate;

  c2 : if PCIEN /= 0 generate
    pciclkint <= pciclkin;
    p3 : if PCISYSCLK = 1 generate clkint <= pciclkint; end generate;
    p0 : if PCIDLL = 1 generate
      u1 : BUFGDLL port map (I => pciclkint, O => pciclk);
    end generate;
    p1 : if PCIDLL = 0 generate
      u1 : BUFG port map (I => pciclkint, O => pciclk);
    end generate;
  end generate;

  c3 : if PCIEN = 0 generate
    pciclk <= '0';
  end generate;

  bufg0 : BUFG port map (I => Clk0B, O => Clk_i);
  bufg1 : BUFG port map (I => Clk_j, O => Clk_k);
  dll0rst <= not cgi.pllrst;
  dll0 : CLKDLL 
    port map (CLKIN => Clkint, CLKFB => Clk_k, CLK0 => Clk_j, CLK180 => CLK180,
    CLK2X => CLK2XL, CLKDV => CLKDV, LOCKED => dll0lock, RST => dll0rst);
   
  Clk0B <= CLK2XL when clk_mul/clk_div = 2 
	else CLKDV when clk_div/clk_mul = 2 else Clk_j;

  sd0 : if (SDRAMEN /= 0) and (SDINVCLK = 0) generate
    cgo.clklock <= dll1lock;
    dll1 : CLKDLL 
      port map (CLKIN => Clk_i, CLKFB => cgi.pllref, RST => dll1rst(0), CLK0 => sdclk,
	CLK2X => clk2x, LOCKED => dll1lock);
    rstdel : process (clk_i)
    begin
      if dll0lock = '0' then dll1rst <= (others => '1');
      elsif rising_edge(clk_i) then
	dll1rst <= dll1rst(1 to 3) & '0';
      end if;
    end process;
  end generate;

  sd1 : if not ((SDRAMEN /= 0) and (SDINVCLK = 0)) generate
    sdclk <= not clk_i; cgo.clklock <= dll0lock;
  end generate;

  cgo.pcilock <= '1';

end;

library ieee;
use ieee.std_logic_1164.all;
-- pragma translate_off
library grlib;
use grlib.stdlib.all;
library unisim;
use unisim.BUFG;
use unisim.DCM;
-- pragma translate_on
library techmap;
use techmap.gencomp.all;
use techmap.libclk.all;


entity clkmul_virtex2 is
  generic ( clk_mul : integer := 1 ; clk_div : integer := 1);
  port (
    resetn  : in  std_logic;
    clkin   : in  std_logic;
    clk     : out std_logic
  );
end;

architecture struct of clkmul_virtex2 is

--  attribute CLKFX_MULTIPLY : string;
--  attribute CLKFX_DIVIDE : string;
  attribute CLKIN_PERIOD : string;
--
--  attribute CLKFX_MULTIPLY of dll0: label is "5";
--  attribute CLKFX_DIVIDE of dll0: label is "4";
  attribute CLKIN_PERIOD of dll0: label is "25";
--
--  attribute CLKFX_MULTIPLY of dll1: label is "4";
--  attribute CLKFX_DIVIDE of dll1: label is "4";
--  attribute CLKIN_PERIOD of dll1: label is "25";
--

component DCM
  generic (
    CLKFX_MULTIPLY : integer := 1 ;
    CLKFX_DIVIDE : integer := 1
  );
  port (
    CLKFB    : in  std_logic;
    CLKIN    : in  std_logic;
    DSSEN    : in  std_logic;
    PSCLK    : in  std_logic;
    PSEN     : in  std_logic;
    PSINCDEC : in  std_logic;
    RST      : in  std_logic;
    CLK0     : out std_logic;
    CLK90    : out std_logic;
    CLK180   : out std_logic;
    CLK270   : out std_logic;
    CLK2X    : out std_logic;
    CLK2X180 : out std_logic;
    CLKDV    : out std_logic;
    CLKFX    : out std_logic;
    CLKFX180 : out std_logic;
    LOCKED   : out std_logic;
    PSDONE   : out std_logic;
    STATUS   : out std_logic_vector (7 downto 0));
end component;

component BUFG port ( O : out std_logic; I : in std_logic); end component;

signal gnd, Clk_i, Clk_j, Clk_k, Clk_l  : std_logic;
signal Clk0B, Clk_FB, dll0rst : std_logic;

begin

  gnd <= '0'; clk <= clk_i;
  dll0rst <= not resetn;

  bufg0 : BUFG port map (I => Clk0B, O => Clk_i);
  bufg1 : BUFG port map (I => Clk_j, O => Clk_k);

  dll0 : DCM
    generic map (CLKFX_MULTIPLY => clk_mul, CLKFX_DIVIDE => clk_div)
    port map ( CLKIN => clkin, CLKFB => Clk_k, DSSEN => gnd, PSCLK => gnd,
    PSEN => gnd, PSINCDEC => gnd, RST => dll0rst, CLK0 => Clk_j,
    CLKFX => Clk0B );

end;

