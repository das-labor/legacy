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
-- Entity:      can_mod
-- File:        can_mod.vhd
-- Author:      Jiri Gaisler - Gaisler Research
-- Description: OpenCores CAN MAC with FIFO RAM
------------------------------------------------------------------------------ 

library ieee;
use ieee.std_logic_1164.all;
library techmap;
use techmap.gencomp.all;
library opencores;
use opencores.cancomp.all;

entity can_mod is
   generic ( memtech   : integer := DEFMEMTECH);
   port (                          
      reset  : in  std_logic;        
      clk     : in  std_logic;        
      cs      : in  std_logic;        
      we      : in  std_logic;        
      addr    : in  std_logic_vector(7 downto 0);   
      data_in : in  std_logic_vector(7 downto 0);   
      data_out: out std_logic_vector(7 downto 0);   
      irq     : out std_logic;      
      rxi     : in  std_logic;      
      txo     : out std_logic    
   );                           
end;                               

architecture rtl of can_mod is 

-- // port connections for Ram
--//64x8
signal q_dp_64x8	: std_logic_vector(7 downto 0);
signal data_64x8	: std_logic_vector(7 downto 0);
signal wren_64x8	: std_logic;
signal rden_64x8	: std_logic;
signal wraddress_64x8	: std_logic_vector(5 downto 0);
signal rdaddress_64x8	: std_logic_vector(5 downto 0);
--//64x4
signal q_dp_64x4	: std_logic_vector(3 downto 0);
signal data_64x4	: std_logic_vector(3 downto 0);
signal wren_64x4x1	: std_logic;
signal wraddress_64x4x1 : std_logic_vector(5 downto 0);
signal rdaddress_64x4x1	: std_logic_vector(5 downto 0);
--//64x1
signal q_dp_64x1	: std_logic;
signal data_64x1	: std_logic;
signal vcc, gnd : std_ulogic;

begin

  gnd <= '0'; vcc <= '1';
  
  can : can_top port map ( rst => reset, addr => addr, data_in => data_in, 
	data_out => data_out, cs => cs, we => we, clk_i => clk, 
    	tx_o => txo, rx_i => rxi, bus_off_on => open,  irq_on => irq,
        clkout_o => open,
    	q_dp_64x8 => q_dp_64x8, data_64x8 => data_64x8, wren_64x8 => wren_64x8,
    	rden_64x8 => rden_64x8, wraddress_64x8 => wraddress_64x8,
    	rdaddress_64x8 => rdaddress_64x8, q_dp_64x4 => q_dp_64x4,
    	data_64x4 => data_64x4, wren_64x4x1 => wren_64x4x1,
    	wraddress_64x4x1 => wraddress_64x4x1,
	rdaddress_64x4x1 => rdaddress_64x4x1,
    	q_dp_64x1 => q_dp_64x1, data_64x1 => data_64x1);

  fifo : syncram_2p generic map(memtech,6,8,0)
  port map(rclk => clk, renable => rden_64x8, wclk => clk,
	raddress => rdaddress_64x8, waddress => wraddress_64x8,
	datain => data_64x8, write => wren_64x8, dataout => q_dp_64x8);

  info_fifo : syncram_2p generic map(memtech,6,4,0)
  port map(rclk => clk, wclk => clk, raddress => rdaddress_64x4x1,
	waddress => wraddress_64x4x1, datain => data_64x4,
     	write => wren_64x4x1, dataout => q_dp_64x4, renable =>vcc);
	
  overrun_fifo : syncram_2p generic map(0,6,1,0) 
  port map(rclk => clk, wclk => clk, raddress => rdaddress_64x4x1,
	waddress => wraddress_64x4x1, datain(0) => data_64x1,
     	write  => wren_64x4x1, dataout(0) => q_dp_64x1, renable => vcc);
end;
