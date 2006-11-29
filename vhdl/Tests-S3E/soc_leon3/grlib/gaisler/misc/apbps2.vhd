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
-- Entity:      apbps2
-- File:        apbps2.vhd
-- Author:      Marcus Hellqvist, Jiri Gaisler
-- Description: PS/2 keyboard interface
-----------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;

library grlib;
use grlib.stdlib.all;
use grlib.amba.all;
use grlib.devices.all;
library gaisler;
use gaisler.misc.all;

entity apbps2 is
  generic(
    pindex      : integer := 0;
    paddr       : integer := 0;
    pmask       : integer := 16#fff#;
    pirq        : integer := 0;
    fKHz        : integer := 50000;
    fixed       : integer := 1
    );
  port(
    rst         : in std_ulogic;        -- Global asynchronous reset
    clk         : in std_ulogic;        -- Global clock
    apbi        : in apb_slv_in_type;
    apbo        : out apb_slv_out_type;
    ps2i        : in ps2_in_type;
    ps2o        : out ps2_out_type
    );
end;

architecture rtl of apbps2 is

constant fifosize       : integer := 16;
type rxstates is (idle,start,data,parity,stop);
type txstates is (idle,waitrequest,start,data,parity,stop,ack);
type fifotype is array(0 to fifosize-1) of std_logic_vector(7 downto 0);

type ps2_regs is record

-- status reg
 data_ready     : std_ulogic;                                   -- data ready
 parity_error   : std_ulogic;                                   -- parity carry out/ error bit
 frame_error    : std_ulogic;                                   -- frame error when receiving
 kb_inh         : std_ulogic;                                   -- keyboard inhibit
 obf            : std_ulogic;                                   -- output buffer full
 ibf            : std_ulogic;                                   -- input buffer full
 rcnt           : std_logic_vector(log2x(fifosize) downto 0);   -- fifo counter
 tcnt           : std_logic_vector(log2x(fifosize) downto 0);   -- fifo counter

-- control reg
 rx_en          : std_ulogic;                                   -- receive enable
 tx_en          : std_ulogic;                                   -- transmit enable
 rx_irq         : std_ulogic;                                   -- keyboard interrupt
 tx_irq         : std_ulogic;                                   -- transmit interrupt

-- others
 rxfifo         : fifotype;                                     -- fifo with 16 bytes
 rraddr         : std_logic_vector(log2x(fifosize)-1 downto 0); -- fifo read address
 rwaddr         : std_logic_vector(log2x(fifosize)-1 downto 0); -- fifo write address
 rxstate        : rxstates;
 txfifo         : fifotype;                                     -- fifo with 16 bytes
 traddr         : std_logic_vector(log2x(fifosize)-1 downto 0); -- fifo read address
 twaddr         : std_logic_vector(log2x(fifosize)-1 downto 0); -- fifo write address
 txstate        : txstates;
 ps2_clk_syn    : std_ulogic;                                   -- ps2 clock synchronized
 ps2_data_syn   : std_ulogic;                                   -- ps2 data synchronized
 ps2_clk_fall   : std_ulogic;                                   -- ps2 clock falling edge detector
 rshift         : std_logic_vector(7 downto 0);                 -- shift register
 rpar           : std_ulogic;                                   -- parity check bit
 tshift         : std_logic_vector(9 downto 0);                 -- shift register
 tpar           : std_ulogic;                                   -- transmit parity bit
 ps2clk         : std_ulogic;                                   -- ps2 clock
 ps2data        : std_ulogic;                                   -- ps2 data
 ps2clkoe       : std_ulogic;                                   -- ps2 clock output enable
 ps2dataoe      : std_ulogic;                                   -- ps2 data output enable
 timer          : std_logic_vector(13 downto 0);                -- timer
 reload         : std_logic_vector(13 downto 0);                -- reload register
end record;

 constant rcntzero      : std_logic_vector(log2x(fifosize) downto 0) := (others => '0');
 constant REVISION      : integer := 1;
 constant pconfig       : apb_config_type := (
                        0 => ahb_device_reg ( VENDOR_GAISLER, GAISLER_APBPS2, 0, REVISION, pirq),
                        1 => apb_iobar(paddr, pmask));

 signal r, rin          : ps2_regs;
 signal ps2_clk, ps2_data : std_ulogic;

begin
  ps2_op : process(r, rst, ps2_clk, ps2_data,apbi)
    variable v  : ps2_regs;
    variable rdata : std_logic_vector(31 downto 0);
    variable irq : std_logic_vector(NAHBIRQ-1 downto 0);
  begin
    v := r;
    rdata := (others => '0'); v.data_ready := '0'; irq := (others => '0'); irq(pirq) := r.rx_irq or r.tx_irq;
    v.rx_irq := '0'; v.tx_irq := '0'; v.obf := r.rcnt(log2x(fifosize)); v.ibf := r.tcnt(log2x(fifosize));

    if r.rcnt /= rcntzero then v.data_ready := '1'; end if;

    -- Synchronizing ps2 input
    v.ps2_clk_syn := ps2_clk; v.ps2_data_syn := ps2_data;

    -- read registers
    case apbi.paddr(3 downto 2) is
      when "00" =>
	rdata(7 downto 0) := r.rxfifo(conv_integer(r.rraddr));
        if (apbi.psel(pindex) and apbi.penable and (not apbi.pwrite)) = '1' then
          if r.rcnt /= rcntzero then
            v.rxfifo(conv_integer(r.rraddr)) := (others => '0');
            v.rraddr := r.rraddr +1; v.rcnt := r.rcnt -1;
          end if;
        end if;
      when "01" =>
	rdata(27 + log2x(fifosize) downto 27) := r.rcnt;
        rdata(22 + log2x(fifosize) downto 22) := r.tcnt;
        rdata(5 downto 0) := r.ibf & r.obf & r.kb_inh & r.frame_error & r.parity_error & r.data_ready;
      when "10" =>
	rdata(3 downto 0) := r.tx_irq & r.rx_irq & r.tx_en & r.rx_en;
      when others =>
	if fixed = 0 then rdata(13 downto 0) := r.reload; end if;
    end case;

    -- write registers
    if (apbi.psel(pindex) and apbi.penable and apbi.pwrite) = '1' then
      case apbi.paddr(3 downto 2) is
        when "00" =>    v.txfifo(conv_integer(r.twaddr)) := apbi.pwdata(7 downto 0);
                        v.twaddr := r.twaddr +1; v.tcnt := r.tcnt +1;
        when "01" =>    v.obf := apbi.pwdata(4);
                        v.kb_inh := apbi.pwdata(3);
                        v.frame_error := apbi.pwdata(2);
                        v.parity_error := apbi.pwdata(1);

        when "10" =>    v.tx_irq := apbi.pwdata(3);
                        v.rx_irq := apbi.pwdata(2);
                        v.tx_en := apbi.pwdata(1);
                        v.rx_en := apbi.pwdata(0);
        when others =>
          v.reload := apbi.pwdata(13 downto 0);
      end case;
    end if;

    if v.tx_en = '1' then
      v.rxstate := idle;
    end if;

    if r.ps2_clk_fall = '0' then
      if v.ps2_clk_syn /= r.ps2_clk_syn then
        if v.ps2_clk_syn = '0' and (v.rx_en = '1' or v.tx_en = '1') then
          v.ps2_clk_fall := '1';
        end if;
      end if;
    else
      v.ps2_clk_fall := '0';
    end if;

    if v.tx_en = '0' then
    -- receiver state machine
      case r.rxstate is
      when idle =>
        if v.rx_en = '1' and v.ps2_data_syn = '0' then
          v.rshift := (others => '1'); v.rxstate := start;
        end if;
      when start =>
        if v.ps2_clk_fall = '1' then
          if v.ps2_data_syn = '0' then
            v.rshift := v.ps2_data_syn & r.rshift(7 downto 1);
            v.rxstate := data; v.rpar := '0';
            v.parity_error := '0'; v.frame_error := '0';
          else v.rxstate := idle; end if;
        end if;
      when data =>
        if v.ps2_clk_fall = '1' then
          v.rshift := v.ps2_data_syn & r.rshift(7 downto 1);
          v.rpar := r.rpar xor v.ps2_data_syn;
          if r.rshift(0) = '0' then v.rxstate := parity; end if;
        end if;
      when parity =>
        if v.ps2_clk_fall = '1' then
          v.parity_error := r.rpar xor (not v.ps2_data_syn);
          v.rxstate := stop;
        end if;
      when stop =>
        if v.ps2_clk_fall = '1' then
          if v.ps2_data_syn = '1' then
            v.rx_irq := '1'; v.rxstate := idle;
            if (not v.obf) = '1' and r.parity_error = '0' then
              v.rxfifo(conv_integer(r.rwaddr)) := v.rshift(7 downto 0);
              v.rwaddr := r.rwaddr +1; v.rcnt :=r.rcnt +1;
            end if;
          else v.frame_error := '1'; v.rxstate := idle; end if;
        end if;
      end case;
    end if;

    -- keyboard inhibit / high impedance
    if v.tx_en /= '1' then
      if r.obf = '1' then
        v.kb_inh := '1'; v.ps2clk := '0'; v.ps2data := '1';
        v.ps2dataoe := '0'; v.ps2clkoe := '0';
      else
        v.ps2clk := '1'; v.ps2data := '1'; v.ps2dataoe := '1';
        v.ps2clkoe := '1';
      end if;
    end if;

    if r.tx_irq = '1' then
      v.tx_en := '0'; v.rx_en := '1';
    end if;

    case r.txstate is
    when idle =>
      if v.tx_en = '1' and v.traddr /= v.twaddr then
        v.ps2clk := '0'; v.ps2clkoe := '0';
        v.ps2data := '1'; v.ps2dataoe := '0'; v.txstate := waitrequest;
      end if;
    when waitrequest =>
      v.timer := r.timer - 1;
      if (v.timer(13) and not r.timer(13)) = '1' then
        if fixed = 1 then v.timer := conv_std_logic_vector(fKHz/10,14);
        else v.timer := r.reload; end if;
        v.ps2clk := '1'; v.ps2data := '0'; v.txstate := start;
      end if;
    when start  =>
      v.ps2clkoe := '1';
      v.tshift := "10" & r.txfifo(conv_integer(r.traddr));
      v.traddr := r.traddr +1;v.tcnt := r.tcnt -1;
      v.tpar := '1';
      v.txstate := data;
    when data =>
      if v.ps2_clk_fall = '1' then
        v.ps2data := r.tshift(0);
        v.tpar := r.tpar xor r.tshift(0);
        v.tshift := '1' & r.tshift(9 downto 1);
        if v.tshift = "1111111110" then v.txstate := parity; end if;
      end if;
    when parity =>
      if v.ps2_clk_fall = '1' then
        v.ps2data := r.tpar; v.txstate := stop;
      end if;
    when stop =>
      if v.ps2_clk_fall = '1' then
        v.ps2data := '1'; v.txstate := ack;
      end if;
    when ack =>
      v.ps2dataoe := '1';
      if v.ps2_clk_fall = '1' and v.ps2_data_syn = '0'then
        v.ps2data := '1'; v.ps2dataoe := '0'; v.tx_irq := '1';
        v.txstate := idle;
      end if;
    end case;

    -- reset operations
    if rst = '0' then
      v.data_ready := '0'; v.kb_inh := '0'; v.parity_error := '0';
      v.frame_error := '0'; v.obf := '0'; v.rx_en := '0';
      v.tx_en := '0'; v.rx_irq := '0'; v.tx_irq := '0';
      v.ps2_clk_fall  := '0'; v.ps2_clk_syn  := '0'; v.ps2_data_syn := '0';
      v.rshift := (others => '0'); v.rxstate := idle; v.txstate := idle;
      v.rraddr := (others => '0'); v.rwaddr := (others => '0');
      v.rcnt := (others => '0'); v.traddr := (others => '0');
      v.twaddr := (others => '0'); v.tcnt := (others => '0');
      v.tshift := (others => '0'); v.tpar := '0';
      v.timer := conv_std_logic_vector(fKHz/10,14);
    end if;

    -- update registers
    rin <= v;

    -- drive outputs
    apbo.prdata <= rdata; apbo.pirq <= irq;
    apbo.pindex <= pindex;
    ps2o.ps2_clk_o <= r.ps2clk;
    ps2o.ps2_clk_oe <= r.ps2clkoe;
    ps2o.ps2_data_o <= r.ps2data;
    ps2o.ps2_data_oe <= r.ps2dataoe;
  end process;

  apbo.pconfig <= pconfig;

  regs : process(clk)
  begin
    if rising_edge(clk) then
      r <= rin;
      ps2_data <= to_x01(ps2i.ps2_data_i);
      ps2_clk <= to_x01(ps2i.ps2_clk_i);
    end if;
  end process;

-- pragma translate_off
    bootmsg : report_version
    generic map ("apbps2_" & tost(pindex) & ": APB PS2 interface rev 0, irq "
    & tost(pirq));
-- pragma translate_on

end;
