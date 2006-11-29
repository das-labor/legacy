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
-- Entity:      mmu_acache
-- File:        mmu_acache.vhd
-- Author:      Jiri Gaisler - Gaisler Research, Konrad Eisele <eiselekd@web.de>
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
use gaisler.mmuconfig.all;
use gaisler.mmuiface.all;

 
entity mmu_acache is
  generic (
    hindex : integer range 0 to NAHBMST-1  := 0;
    ilinesize : integer range 4 to 8 := 4;
    cached : integer := 0;
    clk2x  : integer range 0 to 1 := 0);
  port (
    rst    : in  std_logic;
    clk    : in  std_logic;
    mcii   : in  memory_ic_in_type;
    mcio   : out memory_ic_out_type;
    mcdi   : in  memory_dc_in_type;
    mcdo   : out memory_dc_out_type;
    mcmmi  : in  memory_mm_in_type;
    mcmmo  : out memory_mm_out_type;
    ahbi   : in  ahb_mst_in_type;
    ahbo   : out ahb_mst_out_type;
    ahbso  : in  ahb_slv_out_vector;
    hclk   : in  std_ulogic
  );
end;

architecture rtl of mmu_acache is

-- cache control register type

type cctrltype is record
  ib     : std_logic;				-- icache burst enable
  dfrz   : std_logic;				-- dcache freeze enable
  ifrz   : std_logic;				-- icache freeze enable
  dsnoop : std_logic;				-- data cache snooping
  dcs    : std_logic_vector(1 downto 0);	-- dcache state
  ics    : std_logic_vector(1 downto 0);	-- icache state
end record;

type astates is (idle, dcache, icache, mmu);
type reg_type is record
  bg     : std_logic; 	-- bus grant
  bo     : std_logic_vector(1 downto 0); 	-- bus owner
  ba     : std_logic;	-- bus active
  lb    : std_ulogic;	-- last burst cycle
  retry  : std_logic;	-- retry/split pending
  werr   : std_logic;	-- write error
  lock   : std_ulogic;
  hcache : std_logic;	-- cacheable access
  astate : astates;
  iacc   : std_ulogic;
  dannul : std_ulogic;
  reqmask: std_logic_vector(2 downto 0);  
end record;

constant hconfig : ahb_config_type := (
  0 => ahb_device_reg ( VENDOR_GAISLER, GAISLER_LEON3, 0, LEON3_VERSION, 0),
  others => zero32);

signal r, rin : reg_type;
begin

  comb : process(ahbi, r, rst, mcii, mcdi, mcmmi, ahbso, hclk)

  variable v : reg_type;
  variable haddr   : std_logic_vector(31 downto 0);   -- address bus
  variable htrans  : std_logic_vector(1 downto 0);    -- transfer type 
  variable hwrite  : std_logic;  		      -- read/write
  variable hlock   : std_logic;  		      -- bus lock
  variable hsize   : std_logic_vector(2 downto 0);    -- transfer size
  variable hburst  : std_logic_vector(2 downto 0);    -- burst type
  variable hwdata  : std_logic_vector(31 downto 0);   -- write data
  variable hbusreq : std_logic;   -- bus request
  variable iflush, dflush, mmflush : std_logic;   
  variable iready, dready, mmready : std_logic;   
  variable igrant, dgrant, mmgrant : std_logic;   
  variable iretry, dretry, mmretry : std_logic;   
  variable ihcache, dhcache, mmhcache, hcache, dec_hcache : std_logic;   
  variable imexc, dmexc, mmmexc : std_logic;
  variable dreq : std_logic;
  variable nbo : std_logic_vector(1 downto 0);
  variable su, nb : std_logic;   
  variable vhclk : std_ulogic;
  
  begin

    -- initialisation

    htrans := HTRANS_IDLE;
    if (clk2x /= 0) then vhclk := hclk; else vhclk := '0'; end if;    
    v := r;  v.werr := '0';
    iready := '0'; dready := '0'; mmready := '0';
    igrant := '0'; dgrant := '0'; mmgrant := '0'; 
    imexc := '0'; dmexc := '0'; mmmexc := '0'; hlock := '0';
    iretry := '0'; dretry := '0'; mmretry := '0';
    ihcache := '0'; dhcache := '0'; mmhcache := '0';
    iflush := '0'; dflush := '0'; mmflush := '0'; su := '0';
    hcache := ahbi.hcache;
    
    haddr := (others => '0');
    hwrite := '0';
    hsize := (others => '0');
    hlock := '0'; 
    hburst := (others => '0'); 
    if (ahbi.hready and not vhclk) = '1' then v.lb := '0'; end if;
    
    -- generate AHB signals

    dreq := mcdi.req and (r.lock or not mcdi.lock or not r.bg) and not r.dannul;--and not r.pwd;
    hwdata := mcdi.data;
    hbusreq := '0';

    if (mcii.req = '1') and ((clk2x = 0) or (r.reqmask(2) = '1')) and 
      not (( ((r.ba and dreq) = '1') and (r.bo = "01")) or
           ( ((r.ba and mcmmi.req) = '1') and (r.bo = "10"))) then
      nbo := "00";
      hbusreq := '1';
      htrans := HTRANS_NONSEQ;
    elsif (dreq = '1') and ((clk2x = 0) or (r.reqmask(1) = '1')) and 
      not (( ((r.ba and mcii.req) = '1') and (r.bo = "00")) or
           ( ((r.ba and mcmmi.req) = '1') and (r.bo = "10"))) then
      nbo := "01";
      hbusreq := '1';
      htrans := HTRANS_NONSEQ;
    elsif (mcmmi.req = '1') and ((clk2x = 0) or (r.reqmask(0) = '1')) and 
      not (( ((r.ba and mcii.req) = '1') and (r.bo = "00")) or
           ( ((r.ba and dreq) = '1') and (r.bo = "01"))) then
      nbo := "10";
      hbusreq := '1';
      htrans := HTRANS_NONSEQ;
    else
      nbo := "11";
    end if;
    dec_hcache := ahb_slv_dec_cache(mcdi.address, ahbso, cached);
    
    if nbo = "10" then
      haddr := mcmmi.address; hwrite := not mcmmi.read; hsize := '0' & mcmmi.size;
      hlock := mcmmi.lock;
      htrans := HTRANS_NONSEQ; hburst := HBURST_SINGLE; 
      if (mcmmi.req and r.bg and ahbi.hready and not r.retry and not vhclk) = '1' 
      then mmgrant := '1'; v.hcache := ahbi.hcache; end if; 
    elsif nbo = "00" then
      haddr := mcii.address; hwrite := '0'; hsize := HSIZE_WORD; hlock := '0';
      su := mcii.su;
      if ((mcii.req and r.ba) = '1')  and (r.bo = "00") and ((not r.retry) = '1') then
        htrans := HTRANS_SEQ; haddr(4 downto 2) := haddr(4 downto 2) +1;
        if (((ilinesize = 4) and haddr(3 downto 2) = "10")
          or ((ilinesize = 8) and haddr(4 downto 2) = "110")) and ((ahbi.hready and not vhclk) = '1')
        then v.lb := '1'; end if;
      end if;
      if mcii.burst = '1' then hburst := HBURST_INCR; 
      else hburst := HBURST_SINGLE; end if;
      if (mcii.req and r.bg and ahbi.hready and not r.retry and not vhclk) = '1' 
      then igrant := '1'; v.hcache := ahbi.hcache; end if; 
    elsif nbo = "01" then
      haddr := mcdi.address; hwrite := not mcdi.read; hsize := '0' & mcdi.size;
      hlock := mcdi.lock; 
      if mcdi.asi /= "1010" then su := '1'; else su := '0'; end if;  --ASI_UDATA
      if mcdi.burst = '1' then hburst := HBURST_INCR; 
      else hburst := HBURST_SINGLE; end if;
      if ((dreq and r.ba) = '1') and (r.bo = "01") and ((not r.retry) = '1') then 
        htrans := HTRANS_SEQ; haddr(4 downto 2) := haddr(4 downto 2) +1;
        hburst := HBURST_INCR; 
      end if;
      if (dreq and r.bg and ahbi.hready and not r.retry and not vhclk) = '1' 
      then dgrant := '1'; v.hcache := dec_hcache; end if;
    end if;
    
    if mcii.req = '0' then hlock := mcdi.lock; end if;

    if (vhclk = '0') then
      if (r.ba = '1') and 
        ((ahbi.hresp = HRESP_RETRY) or (ahbi.hresp = HRESP_SPLIT))
      then v.retry := not ahbi.hready; else v.retry := '0'; end if;
    end if;
      
    if r.retry = '1' then htrans := HTRANS_IDLE; end if;

    if r.bo = "10" then
      hwdata := mcmmi.data;
      if r.ba = '1' then
        mmhcache := r.hcache;
        if (ahbi.hready and not vhclk) = '1' then
	  case ahbi.hresp is
	  when HRESP_OKAY => mmready := '1'; 
	  when HRESP_RETRY | HRESP_SPLIT=> mmretry := '1'; 
	  when others => mmready := '1'; mmmexc := '1'; v.werr := not mcmmi.read;
	  end case;
        end if;
      end if;
    elsif r.bo = "00" then
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
    elsif r.bo = "01" then
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

    if (nbo = "01") and ((hsize = "011") or ((dec_hcache and mcdi.read and mcdi.cache) = '1')) then 
      hsize := "010"; haddr(1 downto 0) := "00";
    end if;

    if (ahbi.hready and not vhclk) = '1' then
      v.iacc := r.bg and igrant;
      if r.iacc = '1' then v.dannul := iretry;
      elsif r.bg = '1' then v.dannul := '0'; end if;                                 
      if r.retry = '0' then v.bo := nbo; end if;
      v.bg := ahbi.hgrant(hindex);
      if (htrans = HTRANS_NONSEQ) or (htrans = HTRANS_SEQ) then
	v.ba := r.bg;
      else v.ba := '0'; end if;
    end if;

    if hburst = HBURST_SINGLE then nb := '1'; else nb := '0'; end if;

    v.lock := mcdi.lock and not (htrans(1) and r.bg and not hlock);
    
    if (clk2x /= 0) then
      if (vhclk = '0') then v.reqmask := "111";
      else v.reqmask := mcii.req & mcdi.req & mcmmi.req; end if;
    end if;
                                                
    
    -- reset operation

    if rst = '0' then
      v.bg := '0'; v.bo := "00"; v.ba := '0'; v.retry := '0'; v.werr := '0'; v.lb := '0';
      v.hcache := '0';
      v.astate := idle;
    end if;

    -- drive ports
    
    ahbo.haddr   <= haddr ;
    ahbo.htrans  <= htrans;
    ahbo.hbusreq <= hbusreq and not r.lb and not (((r.bo(1) and r.ba) or nb) and r.bg);
    ahbo.hwdata  <= hwdata;
    ahbo.hlock   <= hlock;
    ahbo.hwrite  <= hwrite;
    ahbo.hsize   <= hsize;
    ahbo.hburst  <= hburst;
    ahbo.hindex  <= hindex;
    if nbo = "00" then ahbo.hprot   <= "11" & su & '0';
    else ahbo.hprot   <= "11" & su & '1'; end if;
    
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

    mcmmo.grant   <= mmgrant;
    mcmmo.ready   <= mmready;
    mcmmo.mexc    <= mmmexc;
    mcmmo.retry   <= mmretry;
    mcmmo.werr    <= r.werr;
    mcmmo.cache   <= mmhcache;
 
    rin <= v;

  end process;

  mcio.data  <= ahbi.hrdata;
  mcdo.data  <= ahbi.hrdata;
  mcmmo.data <= ahbi.hrdata;
  ahbo.hirq    <= (others => '0');
  ahbo.hconfig <= hconfig;

  reg : process(clk)
  begin
    if rising_edge(clk) then r <= rin; end if;
    if (clk2x = 0) then r.reqmask <= "111"; end if;    
  end process;


end;














