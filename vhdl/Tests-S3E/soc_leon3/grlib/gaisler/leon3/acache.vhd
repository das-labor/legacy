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
-- Entity:      acache
-- File:        acache.vhd
-- Author:      Jiri Gaisler - Gaisler Research
-- Description: Interface module between I/D cache controllers and Amba AHB
------------------------------------------------------------------------------  

library ieee;
use ieee.std_logic_1164.all;
library grlib;
use grlib.amba.all;
use grlib.stdlib.all;
use grlib.devices.all;
library gaisler;
use gaisler.libiu.all;
use gaisler.libcache.all;
use gaisler.leon3.all;

entity acache is
  generic (
    hindex : integer range 0 to NAHBMST-1  := 0;
    ilinesize : integer range 4 to 8 := 4;
    cached : integer := 0;
    clk2x     : integer range 0 to 1 := 0);  
  port (
    rst    : in  std_ulogic;
    clk    : in  std_ulogic;
    mcii   : in  memory_ic_in_type;
    mcio   : out memory_ic_out_type;
    mcdi   : in  memory_dc_in_type;
    mcdo   : out memory_dc_out_type;
    ahbi   : in  ahb_mst_in_type;
    ahbo   : out ahb_mst_out_type;
    ahbso  : in  ahb_slv_out_vector;
    hclk   : in std_ulogic
  );
end;

architecture rtl of acache is

-- cache control register type

type reg_type is record
  bg    : std_ulogic; 	-- bus grant
  bo    : std_ulogic; 	-- bus owner
  ba    : std_ulogic;	-- bus active
  lb    : std_ulogic;	-- last burst cycle
  retry : std_ulogic;	-- retry/split pending
  werr  : std_ulogic;	-- write error
  lock  : std_ulogic;  
  hcache :  std_ulogic;
  iacc   : std_ulogic;
  dannul : std_ulogic;
  reqmask: std_logic_vector(1 downto 0);
end record;

constant hconfig : ahb_config_type := (
  0 => ahb_device_reg ( VENDOR_GAISLER, GAISLER_LEON3, 0, LEON3_VERSION, 0),
  others => zero32);

signal r, rin : reg_type;
begin

  comb : process(ahbi, r, rst, mcii, mcdi, hclk, ahbso)

  variable v : reg_type;
  variable haddr   : std_logic_vector(31 downto 0);   -- address bus
  variable htrans  : std_logic_vector(1 downto 0);    -- transfer type 
  variable hwrite  : std_ulogic;  		      -- read/write
  variable hlock   : std_ulogic;  		      -- bus lock
  variable hsize   : std_logic_vector(2 downto 0);    -- transfer size
  variable hburst  : std_logic_vector(2 downto 0);    -- burst type
  variable hwdata  : std_logic_vector(31 downto 0);   -- write data
  variable hbusreq : std_ulogic;   -- bus request
  variable iready, dready : std_ulogic;   
  variable igrant, dgrant : std_ulogic;   
  variable iretry, dretry : std_ulogic;   
  variable ihcache, dhcache, dec_hcache : std_ulogic;   
  variable imexc, dmexc, nbo, ireq, dreq : std_ulogic;   
  variable su, nb : std_ulogic;   
  variable vhclk : std_ulogic;
  
  begin

-- initialisation

    htrans := HTRANS_IDLE;
    if (clk2x /= 0) then vhclk := hclk; else vhclk := '0'; end if;
    v := r; iready := '0'; v.werr := '0';
    dready := '0'; igrant := '0'; dgrant := '0'; 
    imexc := '0'; dmexc := '0'; hlock := '0'; iretry := '0'; dretry := '0';
    ihcache := '0'; dhcache := '0'; su := '0'; --hcache := ahbi.hcache;
    if (ahbi.hready and not vhclk) = '1' then v.lb := '0'; end if;

-- generate AHB signals

    ireq := mcii.req and r.reqmask(1);
    dreq := mcdi.req and r.reqmask(0) and (r.lock or not mcdi.lock or not r.bg) and not r.dannul;
    hbusreq := ireq or dreq;
    if hbusreq = '1' then htrans := HTRANS_NONSEQ; end if;
    hwdata := mcdi.data;
    nbo := (dreq and not (r.ba and mcii.req and not r.bo));
    dec_hcache := ahb_slv_dec_cache(mcdi.address, ahbso, cached); 
    if nbo = '0' then
      haddr := mcii.address; hwrite := '0'; hsize := HSIZE_WORD; hlock := '0';
      su := mcii.su;
      if (ireq and r.ba and not r.bo and not r.retry) = '1' then
        htrans := HTRANS_SEQ; haddr(4 downto 2) := haddr(4 downto 2) +1;
        if (((ilinesize = 4) and haddr(3 downto 2) = "10")
          or ((ilinesize = 8) and haddr(4 downto 2) = "110")) and ((ahbi.hready and not vhclk) = '1')
        then v.lb := '1'; end if;
      end if;
      if mcii.burst = '1' then 
        hburst := HBURST_INCR;
      else hburst := HBURST_SINGLE; end if;      
      if (ireq and r.bg and ahbi.hready and not vhclk and not r.retry) = '1' 
      then igrant := '1'; v.hcache := ahbi.hcache; end if; 
    else
      haddr := mcdi.address; hwrite := not mcdi.read; hsize := '0' & mcdi.size;
      hlock := mcdi.lock;
      if mcdi.asi /= "1010" then su := '1'; else su := '0'; end if;
      if mcdi.burst = '1' then hburst := HBURST_INCR; 
      else hburst := HBURST_SINGLE; end if;
      if (dreq and r.ba and r.bo and not r.retry) = '1' then 
        htrans := HTRANS_SEQ; haddr(4 downto 2) := haddr(4 downto 2) +1;
        hburst := HBURST_INCR;
      end if;
      if (dreq and r.bg and ahbi.hready and not vhclk and not r.retry) = '1' 
      then dgrant := '1'; v.hcache := dec_hcache; end if; 
    end if;
    if ireq = '0' then hlock := mcdi.lock; end if;

    if (vhclk = '0') then 
      if (r.ba = '1') and ((ahbi.hresp = HRESP_RETRY) or (ahbi.hresp = HRESP_SPLIT))
      then v.retry := not ahbi.hready; else v.retry := '0'; end if;
    end if;
      
    if r.retry = '1' then htrans := HTRANS_IDLE; end if;

    if r.bo = '0' then
      if r.ba = '1' then
	ihcache := r.hcache;
        if (ahbi.hready and not vhclk) = '1' then
	  case ahbi.hresp is
	  when HRESP_OKAY => iready := '1'; 
	  when HRESP_RETRY | HRESP_SPLIT=> iretry := '1'; 
	  when others => iready := '1'; imexc := '1';
	  end case;
        end if;
      end if;
    else
      if r.ba = '1' then
	dhcache := r.hcache;
        if (ahbi.hready and not vhclk) = '1' then
	  case ahbi.hresp is
	  when HRESP_OKAY => dready := '1'; 
	  when HRESP_RETRY | HRESP_SPLIT=> dretry := '1'; 
	  when others => dready := '1'; dmexc := '1'; v.werr := not mcdi.read;
	  end case;
        end if;
      end if;
      hlock := mcdi.lock;
    end if;

    -- decode cacheability

    if (nbo = '1') and ((hsize = "011") or ((dec_hcache and mcdi.read and mcdi.cache) = '1')) then 
      hsize := "010"; haddr(1 downto 0) := "00";
    end if;

    if (ahbi.hready and not vhclk) = '1' then
      v.iacc := r.bg and igrant;
      if r.iacc = '1' then v.dannul := iretry;
      elsif r.bg = '1' then v.dannul := '0'; end if;                           
      v.bo := nbo; v.bg := ahbi.hgrant(hindex);
      if (htrans = HTRANS_NONSEQ) or (htrans = HTRANS_SEQ) then
	v.ba := r.bg;
      else v.ba := '0'; end if;
    end if;

    if hburst = HBURST_SINGLE then nb := '1'; else nb := '0'; end if;

    v.lock := mcdi.lock and not (htrans(1) and r.bg and not hlock);
    
    if (clk2x /= 0) then
      if (vhclk = '0') then v.reqmask := "11";
      else v.reqmask := mcii.req & mcdi.req; end if;
    end if;    

    
-- reset operation

    if rst = '0' then
      v.bg := '0'; v.bo := '0'; v.ba := '0'; v.retry := '0'; v.werr := '0'; v.lb := '0';
    end if;

-- drive ports

    ahbo.haddr   <= haddr ;
    ahbo.htrans  <= htrans;
    ahbo.hbusreq <= hbusreq and not r.lb and not (((r.bo and r.ba) or nb) and r.bg and nbo);    
    ahbo.hwdata  <= hwdata;
    ahbo.hlock   <= hlock;
    ahbo.hwrite  <= hwrite;
    ahbo.hsize   <= hsize;
    ahbo.hburst  <= hburst;
    ahbo.hprot   <= "11" & su & nbo;
    ahbo.hindex  <= hindex;
    mcio.grant   <= igrant;
    mcio.ready   <= iready;
    mcio.mexc    <= imexc;
    mcio.retry   <= iretry;
    mcio.cache   <= ihcache;
    mcdo.grant   <= dgrant;
    mcdo.ready   <= dready;
    mcdo.mexc    <= dmexc;
    mcdo.retry   <= dretry;
    mcdo.werr    <= r.werr;
    mcdo.cache   <= dhcache;
    mcdo.ba      <= r.ba;
    mcdo.bg      <= r.bg;

    rin <= v;

  end process;

  mcio.data <= ahbi.hrdata; mcdo.data <= ahbi.hrdata;
  ahbo.hirq    <= (others => '0');
  ahbo.hconfig <= hconfig;

  reg : process(clk)
  begin
    if rising_edge(clk) then r <= rin; end if;
    if (clk2x = 0) then r.reqmask <= "11"; end if;
  end process;


end;

