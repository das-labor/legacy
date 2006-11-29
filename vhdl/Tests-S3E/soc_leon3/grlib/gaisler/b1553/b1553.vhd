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
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
library grlib;
use grlib.amba.all;

package b1553 is
  
  type b1553_in_type is record
      busainp   :  std_logic;
      busainn   :  std_logic;
      busbinp   :  std_logic;
      busbinn   :  std_logic;
  end record;

  type b1553_out_type is record
      busainen  :  std_logic;
      busaoutin :  std_logic;
      busaoutp  :  std_logic;
      busaoutn  :  std_logic;
      busbinen  :  std_logic;
      busboutin :  std_logic;
      busboutp  :  std_logic;
      busboutn  :  std_logic;
  end record;

  type rt1553_in_type is record
       useextok  : std_ulogic;  
       cmdok     : std_ulogic;
  end record;

  type rt1553_out_type is record
      msgstart  : std_ulogic;
      cmdsync   : std_ulogic;
      syncnow   : std_ulogic;
      busreset  : std_ulogic;
      
      cmdval    : std_logic_vector(11 downto 0);
      cmdokout  : std_ulogic;
      cmdstb    : std_ulogic;
      addrlat   : std_ulogic;
      intlat    : std_ulogic;
  end record;
  
  type brm1553_in_type is record
       cmdok     : std_ulogic;
  end record;

  type brm1553_out_type is record
      msgstart  : std_ulogic;
      cmdsync   : std_ulogic;
      syncnow   : std_ulogic;
      busreset  : std_ulogic;
      opmode    : std_logic_vector(1 downto 0);
      cmdval    : std_logic_vector(11 downto 0);
      cmdokout  : std_ulogic;
      cmdstb    : std_ulogic;
  end record;
  
  component b1553bc is
    generic (
      hindex  : integer := 0;
      pindex  : integer := 0;
      paddr   : integer := 0;
      pmask   : integer := 16#fff#;
      pirq    : integer := 0
      );
    port (
      rstn    : in  std_ulogic;
      clk     : in  std_ulogic;
      b1553i  : in  b1553_in_type;
      b1553o  : out b1553_out_type;
      apbi    : in  apb_slv_in_type;
      apbo    : out apb_slv_out_type;
      ahbi    : in  ahb_mst_in_type;
      ahbo    : out ahb_mst_out_type
      );
  end component;

  component b1553rt is
    generic (
        hindex    : integer := 0;
        pindex    : integer := 0;
        paddr     : integer := 0;
        pmask     : integer := 16#fff#;
        pirq      : integer := 0;
        ahbaddr   : integer range 0 to 16#FFFFF# := 0;    -- Address register reset value
        clkspd    : integer range 0 to 3 := 1;            -- Clock speed.
        rtaddr    : integer range 0 to 31 := 0;           -- RT address
        rtaddrp   : integer range 0 to 1 := 1;            -- RT address parity bit (set to achieve odd parity)
        wrtcmd    : integer range 0 to 1 := 1;            -- Write CW to memory
        wrttsw    : integer range 0 to 1 := 1;            -- Write TSW to memory
        extmdata  : integer range 0 to 1 := 0;            -- Read/write mode code data from/to memory
        intenbbr  : integer range 0 to 1 := 0;            -- Generate interrupts on bad messages
        bcasten   : integer range 0 to 1 := 1;            -- Enable broadcast
        sa30loop  : integer range 0 to 1 := 0             -- Enable loopback on sub address 30
        );
    port (
      rstn      : in    std_ulogic;
      clk       : in    std_ulogic;
      b1553i    : in    b1553_in_type;
      b1553o    : out   b1553_out_type;
      rti       : in    rt1553_in_type;
      rto       : out   rt1553_out_type;
      apbi      : in    apb_slv_in_type;
      apbo      : out   apb_slv_out_type;
      ahbi      : in    ahb_mst_in_type;
      ahbo      : out   ahb_mst_out_type
      );
  end component;
  
  component b1553brm is 
     generic (
       hindex       : integer := 0;
       pindex       : integer := 0;
       paddr        : integer := 0;
       pmask        : integer := 16#ff0#;
       pirq         : integer := 0;
       ahbaddr      : integer range 0 to 16#FFFFF# := 0;    -- Address register reset value
       abits       : integer range 12 to 17 := 16;          -- Number of address bits needed to address the allocated memory
       rtaddr       : integer range 0 to 31 := 0;           -- RT address
       rtaddrp      : integer range 0 to 1  := 1;           -- RT address parity bit (set to achieve odd parity)
       lockn        : integer range 0 to 1  := 1;           -- Prevent registers from overriding generic conf
       mselin       : integer range 0 to 3  := 1;           -- Mode select
       abstdin      : integer range 0 to 1  := 0;           -- Set bus standard
    
       bcenable     : integer range 0 to 1  := 1;           -- These generics only
       rtenable     : integer range 0 to 1  := 1;           -- affect the RTL version
       mtenable     : integer range 0 to 1  := 1;           -- of the core. 
       legregs      : integer range 0 to 4  := 1;
       enhanced     : integer range 0 to 1  := 1;
       initfreq     : integer range 12 to 24:= 20;
       betiming     : integer range 0 to 1  := 1;
       syncrst      : integer range 0 to 1  := 0
       );
     port (
       rstn      : in    std_ulogic;
       rstoutn   : out   std_ulogic;
       clk       : in    std_ulogic;
       tclk      : in    std_ulogic;
       brmi      : in    brm1553_in_type;
       brmo      : out   brm1553_out_type;
       b1553i    : in    b1553_in_type;
       b1553o    : out   b1553_out_type;
       apbi      : in    apb_slv_in_type;
       apbo      : out   apb_slv_out_type;
       ahbi      : in    ahb_mst_in_type;
       ahbo      : out   ahb_mst_out_type
       );
  end component;
  
component b1553brm_async is 
 generic (
   hindex       : integer := 0;
   slvndx       : integer := 0;
   ioaddr       : integer := 16#000#;
   iomask       : integer := 16#FF0#;
   irq          : integer := 0;
   ahbaddr      : integer range 0 to 16#FFFFF# := 0;    -- Address register reset value
   abits        : integer range 12 to 17 := 16;         -- Number of address bits needed to address the allocated memory
   rtaddr       : integer range 0 to 31 := 0;           -- RT address
   rtaddrp      : integer range 0 to 1  := 1;           -- RT address parity bit (set to achieve odd parity)
   lockn        : integer range 0 to 1  := 1;           -- Prevent registers from overriding generic conf
   mselin       : integer range 0 to 3  := 1;           -- Mode select
   abstdin      : integer range 0 to 1  := 0;           -- Set bus standard
   clkbuftype  : integer range 0 to 2  := 0;
   bcenable     : integer range 0 to 1  := 1;           -- These generics only
   rtenable     : integer range 0 to 1  := 1;           -- affect the RTL version
   mtenable     : integer range 0 to 1  := 1;           -- of the core. 
   legregs      : integer range 0 to 4  := 1;
   enhanced     : integer range 0 to 1  := 1;
   initfreq     : integer range 12 to 24:= 20;
   betiming     : integer range 0 to 1  := 1;
   syncrst      : integer range 0 to 1  := 0
   );
 port (
   rstn      : in    std_ulogic;
   rstoutn   : out   std_ulogic;
   sys_clk   : in    std_ulogic;
   brm_clk1  : in    std_ulogic;
   brm_clk2  : in    std_ulogic;
   tclk      : in    std_ulogic;
   brmi      : in    brm1553_in_type;
   brmo      : out   brm1553_out_type;
   b1553i    : in    b1553_in_type;
   b1553o    : out   b1553_out_type;
   ahbsi     : in    ahb_slv_in_type;
   ahbso     : out   ahb_slv_out_type;
   ahbmi     : in    ahb_mst_in_type;
   ahbmo     : out   ahb_mst_out_type
   );
end component;      

end package;
