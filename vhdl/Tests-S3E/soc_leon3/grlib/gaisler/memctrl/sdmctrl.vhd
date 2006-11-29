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
-- Entity: 	sdmctrl
-- File:	sdmctrl.vhd
-- Author:	Jiri Gaisler - Gaisler Research
-- Description:	SDRAM memory controller to fit with LEON2 memory controller.
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
library grlib;
use grlib.amba.all;
use grlib.stdlib.all;
library gaisler;
use gaisler.memctrl.all;

entity sdmctrl is
  generic (
    pindex  : integer := 0;
    invclk  : integer := 0;
    fast    : integer := 0;
    wprot   : integer := 0;
    sdbits  : integer := 32
  );
  port (
    rst    : in  std_ulogic;
    clk    : in  std_ulogic;
    sdi    : in  sdram_in_type;
    sdo    : out sdram_out_type;
    apbi   : in  apb_slv_in_type;
    apbo   : out apb_slv_out_type;
    wpo    : in  wprot_out_type;
    sdmo   : out sdram_mctrl_out_type
  );
end; 

architecture rtl of sdmctrl is

constant WPROTEN  : boolean := (wprot /= 0);
constant SDINVCLK : boolean := (invclk /= 0);
constant BUS64    : boolean := (sdbits = 64);

type mcycletype is (midle, active, leadout);
type sdcycletype is (act1, act2, act3, rd1, rd2, rd3, rd4, rd5, rd6, rd7, rd8,
		     wr1, wr2, wr3, wr4, wr5, sidle);
type icycletype is (iidle, pre, ref, lmode, finish);

-- sdram configuration register

type sdram_cfg_type is record
  command          : std_logic_vector(1 downto 0);
  csize            : std_logic_vector(1 downto 0);
  bsize            : std_logic_vector(2 downto 0);
  casdel           : std_ulogic;  -- CAS to data delay: 2/3 clock cycles
  trfc             : std_logic_vector(2 downto 0);
  trp              : std_ulogic;  -- precharge to activate: 2/3 clock cycles
  refresh          : std_logic_vector(14 downto 0);
  renable          : std_ulogic;
end record;

-- local registers

type reg_type is record
  hready        : std_ulogic;
  hsel          : std_ulogic;
  bdrive        : std_ulogic;
  burst         : std_ulogic;
  busy          : std_ulogic;
  bdelay        : std_ulogic;
  wprothit      : std_ulogic;
  startsd       : std_ulogic;
  aload         : std_ulogic;

  mstate	: mcycletype;
  sdstate	: sdcycletype;
  cmstate	: mcycletype;
  istate	: icycletype;
  icnt          : std_logic_vector(2 downto 0);

  cfg           : sdram_cfg_type;
  trfc          : std_logic_vector(2 downto 0);
  refresh       : std_logic_vector(14 downto 0);
  sdcsn  	: std_logic_vector(1  downto 0);
  sdwen  	: std_ulogic;
  rasn 		: std_ulogic;
  casn 		: std_ulogic;
  dqm  		: std_logic_vector(7 downto 0);
  bsel 		: std_ulogic;
  -- only needed to keep address lines from switch too much
  address  	: std_logic_vector(16 downto 2);  -- memory address
end record;


signal r, ri : reg_type;

begin

  ctrl : process(rst, apbi, sdi, wpo, r)
  variable v : reg_type;		-- local variables for registers
  variable startsd : std_ulogic;
  variable dataout : std_logic_vector(31 downto 0); -- data from memory
  variable haddr   : std_logic_vector(31 downto 0);
  variable regsd : std_logic_vector(31 downto 0);   -- data from registers
  variable dqm      : std_logic_vector(7 downto 0);
  variable raddr    : std_logic_vector(12 downto 0);
  variable adec     : std_ulogic;
  variable busy     : std_ulogic;
  variable aload    : std_ulogic;
  variable rams     : std_logic_vector(1 downto 0);
  variable hresp    : std_logic_vector(1 downto 0);
  variable ba       : std_logic_vector(1 downto 0);

  begin

-- Variable default settings to avoid latches

    v := r; startsd := '0'; v.busy := '0'; hresp := HRESP_OKAY;

    if sdi.hready = '1' then v.hsel := sdi.hsel; end if;
    if (sdi.hready and sdi.hsel ) = '1' then
      if sdi.htrans(1) = '1' then v.hready := '0'; end if;
    end if;

    if fast = 1 then haddr := sdi.rhaddr; else haddr := sdi.haddr; end if;

-- main state

    case sdi.hsize is
    when "00" =>
      case sdi.rhaddr(1 downto 0) is
      when "00" => dqm := "11110111";
      when "01" => dqm := "11111011";
      when "10" => dqm := "11111101";
      when others => dqm := "11111110";
      end case;
    when "01" =>
      if sdi.rhaddr(1) = '0' then dqm := "11110011"; else  dqm := "11111100"; end if;
    when others => dqm := "11110000";
    end case;

    if BUS64 and (r.bsel = '1') then
      dqm := dqm(3 downto 0) & "1111";
    end if;

-- main FSM

    case r.mstate is
    when midle =>
      if (v.hsel and sdi.nhtrans(1)) = '1' then
	if (r.sdstate = sidle) and (r.cfg.command = "00") and 
	   (r.cmstate = midle) and (sdi.idle = '1')
        then 
	  if fast = 1 then v.startsd := '1'; else startsd := '1'; end if;
	  v.mstate := active;
	end if;
      end if;
    when others => null;
    end case;
      
    startsd := r.startsd or startsd;

-- generate row and column address size

    case r.cfg.csize is
    when "00" => raddr := haddr(22 downto 10); 
    when "01" => raddr := haddr(23 downto 11);
    when "10" => raddr := haddr(24 downto 12);
    when others => 
      if r.cfg.bsize = "111" then raddr := haddr(26 downto 14);
      else raddr := haddr(25 downto 13); end if;
    end case;

-- generate bank address

    ba := genmux(r.cfg.bsize, haddr(28 downto 21)) &
          genmux(r.cfg.bsize, haddr(27 downto 20));

-- generate chip select

    if BUS64 then
      adec := genmux(r.cfg.bsize, haddr(30 downto 23));
      v.bsel := genmux(r.cfg.bsize, sdi.rhaddr(29 downto 22));
    else
      adec := genmux(r.cfg.bsize, haddr(29 downto 22)); v.bsel := '0';
    end if;
    if (sdi.srdis = '0') and (r.cfg.bsize = "111") then adec := not adec; end if;
    rams := adec & not adec;

    if r.trfc /= "000" then v.trfc := r.trfc - 1; end if;

-- sdram access FSM

    case r.sdstate is
    when sidle =>
      v.bdelay := '0';
      if (startsd = '1') and (r.cfg.command = "00") and (r.cmstate = midle) then
        v.address(16 downto 2) := ba & raddr;
	v.sdcsn := not rams(1 downto 0); v.rasn := '0'; v.sdstate := act1; 
	v.startsd := '0';
      end if;
    when act1 =>
	v.rasn := '1'; v.trfc := r.cfg.trfc;
	if r.cfg.casdel = '1' then v.sdstate := act2; else
	  v.sdstate := act3;
          v.hready := sdi.hwrite and sdi.htrans(0) and sdi.htrans(1);
	end if;
        if WPROTEN then 
	  v.wprothit := wpo.wprothit;
	  if wpo.wprothit = '1' then hresp := HRESP_ERROR; end if;
	end if;
    when act2 =>
	v.sdstate := act3; 
        v.hready := sdi.hwrite and sdi.htrans(0) and sdi.htrans(1);
        if WPROTEN and (r.wprothit = '1') then 
	  hresp := HRESP_ERROR; v.hready := '0'; 
	end if;
    when act3 =>
      v.casn := '0'; 
      v.address(14 downto 2) := sdi.rhaddr(13 downto 12) & '0' & sdi.rhaddr(11 downto 2);
      v.dqm := dqm; v.burst := r.hready;

      if sdi.hwrite = '1' then

	v.sdstate := wr1; v.sdwen := '0'; v.bdrive := '1';
        if sdi.htrans = "11" or (r.hready = '0') then v.hready := '1'; end if;
        if WPROTEN and (r.wprothit = '1') then
	  hresp := HRESP_ERROR; v.hready := '1'; 
	  v.sdstate := wr1; v.sdwen := '1'; v.bdrive := '0'; v.casn := '1';
	end if;
      else v.sdstate := rd1; end if;
    when wr1 =>
      v.address(14 downto 2) := sdi.rhaddr(13 downto 12) & '0' & sdi.rhaddr(11 downto 2);
      if (((r.burst and r.hready) = '1') and (sdi.rhtrans = "11"))
      and not (WPROTEN and (r.wprothit = '1'))
      then 
	v.hready := sdi.htrans(0) and sdi.htrans(1) and r.hready;
--	v.hready := sdi.htrans(0) and r.hready;
      else
        v.sdstate := wr2; v.bdrive := '0'; v.casn := '1'; v.sdwen := '1';
	v.dqm := (others => '1');
      end if;
    when wr2 =>
      if (r.trfc(2 downto 1) = "00") then
        if (r.cfg.trp = '0') then v.rasn := '0'; v.sdwen := '0'; end if;
        v.sdstate := wr3;
      end if;
    when wr3 =>
      if (r.cfg.trp = '1') then 
	v.rasn := '0'; v.sdwen := '0'; v.sdstate := wr4;
      else 
        v.sdcsn := "11"; v.rasn := '1'; v.sdwen := '1';
        if r.trfc = "000" then v.sdstate := sidle; end if;
      end if;
    when wr4 =>
      v.sdcsn := "11"; v.rasn := '1'; v.sdwen := '1'; 
      if (r.cfg.trp = '1') then v.sdstate := wr5;
      else 
	if r.trfc = "000" then v.sdstate := sidle; end if;
      end if;
    when wr5 =>
      if r.trfc = "000" then v.sdstate := sidle; end if;
    when rd1 =>
      v.casn := '1'; v.sdstate := rd7;
    when rd7 =>
      if r.cfg.casdel = '1' then v.sdstate := rd2;
      else
        v.sdstate := rd3;
        if (r.trfc(2 downto 1) = "00") then
          if sdi.htrans /= "11" then v.rasn := '0'; v.sdwen := '0'; end if;
        end if;
      end if;
    when rd2 =>
      v.sdstate := rd3;
      if sdi.htrans /= "11" then v.rasn := '0'; v.sdwen := '0'; end if;
      if v.sdwen = '0' then v.dqm := (others => '1'); end if;
    when rd3 =>
      v.sdstate := rd4; v.hready := '1';
      if r.sdwen = '0' then
	v.rasn := '1'; v.sdwen := '1'; v.sdcsn := "11"; v.dqm := (others => '1');
      end if;

    when rd4 =>
      v.hready := '1';

      if sdi.htrans /= "11" or (r.sdcsn = "11") then

        v.hready := '0'; v.dqm := (others => '1');
        if (r.sdcsn /= "11") then
	  v.rasn := '0'; v.sdwen := '0'; v.sdstate := rd5;
	else
          if r.cfg.trp = '1' then v.sdstate := rd6; 
	  else v.sdstate := sidle; end if;
        end if;
      end if;
    when rd5 =>
--      if (r.cfg.trp or (r.hsel and not sdi.rhaddr(30)))= '1' then 
      if r.cfg.trp = '1' then 
	v.sdstate := rd6; 
      else v.sdstate := sidle; end if;
      v.sdcsn := (others => '1'); v.rasn := '1'; v.sdwen := '1'; v.dqm := (others => '1');
    when rd6 =>
      v.sdstate := sidle; v.dqm := (others => '1');

      v.sdcsn := (others => '1'); v.rasn := '1'; v.sdwen := '1';

    when others =>
	v.sdstate := sidle;
    end case;

-- sdram commands

    case r.cmstate is
    when midle =>
      if r.sdstate = sidle then
        case r.cfg.command is
        when "01" => -- precharge
          if (sdi.idle = '1') then
	    v.busy := '1';
	    v.sdcsn := (others => '0'); v.rasn := '0'; v.sdwen := '0';
	    v.address(12) := '1'; v.cmstate := active;
	  end if;
        when "10" => -- auto-refresh
	  v.sdcsn := (others => '0'); v.rasn := '0'; v.casn := '0';
          v.cmstate := active;
        when "11" =>
          if (sdi.idle = '1') then
	    v.busy := '1';
	    v.sdcsn := (others => '0'); v.rasn := '0'; v.casn := '0'; 
	    v.sdwen := '0'; v.cmstate := active;
	    v.address(15 downto 2) := "000010001" & r.cfg.casdel & "0111";
	  end if;
        when others => null;
        end case;
      end if;
    when active =>
      v.sdcsn := (others => '1'); v.rasn := '1'; v.casn := '1'; 
      v.sdwen := '1'; v.cfg.command := "00";
      v.cmstate := leadout; v.trfc := r.cfg.trfc;
    when leadout =>
      if r.trfc = "000" then v.cmstate := midle; end if;

    end case;

-- sdram init

    case r.istate is
    when iidle =>
      if (sdi.idle and sdi.enable) = '1' then
        v.cfg.command := "01"; v.istate := pre;
      end if;
    when pre =>
      if r.cfg.command = "00" then
        v.cfg.command := "10"; v.istate := ref; v.icnt := "111"; 
      end if;
    when ref =>
      if r.cfg.command = "00" then
        v.cfg.command := "10"; v.icnt := r.icnt - 1;
	if r.icnt = "000" then v.istate := lmode; v.cfg.command := "11"; end if; 
      end if;
    when lmode =>
      if r.cfg.command = "00" then
        v.istate := finish;
      end if;
    when others =>
      if sdi.enable = '0' then
        v.istate := iidle;
      end if;
    end case;

    if (sdi.hready and sdi.hsel ) = '1' then
      if sdi.htrans(1) = '0' then v.hready := '1'; end if;
    end if;

-- second part of main fsm

    case r.mstate is
    when active =>
      if v.hready = '1' then
	v.mstate := midle;
      end if;
    when others => null;
    end case;

-- sdram refresh counter

      if (r.cfg.renable = '1') and (r.istate = finish) then 
	v.refresh := r.refresh - 1;
        if (v.refresh(14) and not r.refresh(14))  = '1' then 
	  v.refresh := r.cfg.refresh;
	  v.cfg.command := "10";
	end if;
      end if;

-- APB register access


    if (apbi.psel(pindex) and apbi.penable and apbi.pwrite) = '1' then
      case apbi.paddr(3 downto 2) is
      when "01" =>
        if sdi.enable = '1' then
          v.cfg.command     :=  apbi.pwdata(20 downto 19); 
        end if;
        v.cfg.csize       :=  apbi.pwdata(22 downto 21); 
        v.cfg.bsize       :=  apbi.pwdata(25 downto 23); 
        v.cfg.casdel      :=  apbi.pwdata(26); 
        v.cfg.trfc        :=  apbi.pwdata(29 downto 27); 
        v.cfg.trp         :=  apbi.pwdata(30); 
        v.cfg.renable     :=  apbi.pwdata(31); 
      when "10" =>
        v.cfg.refresh     :=  apbi.pwdata(26 downto 12); 
        v.refresh         :=  (others => '0');
      when others =>
      end case;
    end if;

    regsd := (others => '0');
    case apbi.paddr(3 downto 2) is
    when "01" => 
      regsd(31 downto 19) := r.cfg.renable & r.cfg.trp & r.cfg.trfc &
	 r.cfg.casdel & r.cfg.bsize & r.cfg.csize & r.cfg.command; 
    when others => 
      regsd(26 downto 12) := r.cfg.refresh; 
    end case;
    apbo.prdata <= regsd;

-- synchronise with sram/prom controller

    if fast = 0 then
      if (r.sdstate < wr4) or (v.hsel = '1') then v.busy := '1';end if;
    else
      if (r.sdstate < wr4) or (r.startsd = '1') then v.busy := '1';end if;
    end if;
    v.busy := v.busy or r.bdelay;
    busy := v.busy or r.busy;
    v.aload := r.busy and not v.busy;
    aload := v.aload;

-- generate memory address

    sdmo.address <= v.address;

-- reset

    if rst = '0' then
      v.sdstate	      := sidle; 
      v.mstate	      := midle; 
      v.istate	      := iidle; 
      v.cmstate	      := midle; 
      v.hsel	      := '0';
      v.cfg.command   := "00";
      v.cfg.csize     := "10";
      v.cfg.bsize     := "000";
      v.cfg.casdel    :=  '1';
      v.cfg.trfc      := "111";
      v.cfg.renable   :=  '0';
      v.cfg.trp       :=  '1';
      v.dqm	      := (others => '1');
      v.sdwen	      := '1';
      v.rasn	      := '1';
      v.casn	      := '1';
      v.hready	      := '1';
      v.startsd       := '0';
    end if;

    ri <= v; 

    sdmo.bdrive  <= v.bdrive;

    sdo.sdcke    <= (others => '1');
    sdo.sdcsn    <= r.sdcsn;
    sdo.sdwen    <= r.sdwen;
    sdo.dqm      <= r.dqm;
    sdo.rasn     <= r.rasn;
    sdo.casn     <= r.casn;
    sdmo.busy    <= busy;
    sdmo.aload   <= aload;

    sdmo.hready  <= r.hready;

    sdmo.hresp   <= hresp;
    sdmo.hsel    <= r.hsel;
    sdmo.bsel    <= r.bsel;

  end process;


    regs : process(clk,rst)
    begin 

      if rising_edge(clk) then
        r <= ri;
        if rst = '0' then
          r.icnt <= (others => '0');
        end if; 
      end if;

      if rst = '0' then
        r.bdrive <= '0';
        r.sdcsn  <= (others => '1');
      end if;
    end process;

end;

