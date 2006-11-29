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
-- Entity: 	cache
-- File:	cache.vhd
-- Author:	Jiri Gaisler
-- Description:	Cache controllers and AHB interface
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
library grlib;
use grlib.amba.all;
library gaisler;
use gaisler.libiu.all;
use gaisler.libcache.all;

entity cache is
  generic (
    hindex    : integer              := 0;
    dsu       : integer range 0 to 1 := 0;
    icen      : integer range 0 to 1 := 0;
    irepl     : integer range 0 to 2 := 0;
    isets     : integer range 1 to 4 := 1;
    ilinesize : integer range 4 to 8 := 4;
    isetsize  : integer range 1 to 256 := 1;
    isetlock  : integer range 0 to 1 := 0;
    dcen      : integer range 0 to 1 := 0;
    drepl     : integer range 0 to 2 := 0;
    dsets     : integer range 1 to 4 := 1;
    dlinesize : integer range 4 to 8 := 4;
    dsetsize  : integer range 1 to 256 := 1;
    dsetlock  : integer range 0 to 1 := 0;
    dsnoop    : integer range 0 to 2 := 0;
    ilram      : integer range 0 to 1 := 0;
    ilramsize  : integer range 1 to 512 := 1;        
    ilramstart : integer range 0 to 255 := 16#8e#;
    dlram      : integer range 0 to 1 := 0;
    dlramsize  : integer range 1 to 512 := 1;        
    dlramstart : integer range 0 to 255 := 16#8f#;
    cached     : integer := 0;
    clk2x     : integer range 0 to 1 := 0);          
  port (
    rst   : in  std_ulogic;
    clk   : in  std_ulogic;
    ici   : in  icache_in_type;
    ico   : out icache_out_type;
    dci   : in  dcache_in_type;
    dco   : out dcache_out_type;
    ahbi  : in  ahb_mst_in_type;
    ahbo  : out ahb_mst_out_type;
    ahbsi : in  ahb_slv_in_type;
    ahbso  : in  ahb_slv_out_vector;        
    crami : out cram_in_type;
    cramo : in  cram_out_type;
    fpuholdn : in  std_ulogic;
    hclk    : in std_ulogic
  );
end; 

architecture rtl of cache is

signal icol  : icache_out_type;
signal dcol  : dcache_out_type;
signal mcii : memory_ic_in_type;
signal mcio : memory_ic_out_type;
signal mcdi : memory_dc_in_type;
signal mcdo : memory_dc_out_type;

begin

     icache0 : icache 
       generic map (icen, irepl, isets, ilinesize, isetsize, isetlock, ilram,
                    ilramsize, ilramstart)
       port map ( rst, clk, ici, icol, dci, dcol, mcii, mcio, 
    		 crami.icramin, cramo.icramo, fpuholdn);
     dcache0 : dcache 
       generic map (dsu, dcen, drepl, dsets, dlinesize, dsetsize,  dsetlock, dsnoop,
 		    dlram, dlramsize, dlramstart, ilram, ilramstart, clk2x)
       port map ( rst, clk, dci, dcol, icol, mcdi, mcdo, ahbsi,
 		 crami.dcramin, cramo.dcramo, fpuholdn, hclk);
     a0 : acache 
       generic map (hindex, ilinesize, cached, clk2x)
       port map (rst, clk, mcii, mcio, mcdi, mcdo, ahbi, ahbo, ahbso, hclk);
    
  ico <= icol;
  dco <= dcol;

end ;

