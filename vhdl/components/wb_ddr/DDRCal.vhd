-----------------------------------------------------------------------------
-- Wishbone DDR controller component
-- (c) 2007 Joerg Bornschein (jb@capsec.org)
--
-- This file is part of wb_ddr and should not be used directly
-----------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

-----------------------------------------------------------------------------
-- DDRCal -------------------------------------------------------------------
entity DDRCal is
	port (
		clk       : in  std_logic;
		reset     : in  std_logic;
		-- Calibration
		cal_xing  : out std_logic_vector(5 downto 0);
		cal_inc   : out std_logic;
		cal_dec   : out std_logic;
		cal_done  : in  std_logic;
		cal_ovf   : in  std_logic;
		-- Status & Buttons
		match_led : in  std_logic_vector(7 downto 0);		
		rotary    : in  std_logic_vector(2 downto 0);
		led       : out std_logic_vector(7 downto 0) );
end DDRCal;

-----------------------------------------------------------------------------
-- Implementation -----------------------------------------------------------
architecture rtl of DDRCal is

signal clkxing_int : std_logic_vector(7 downto 0);

signal rot_q1      : std_logic;
signal rot_q1_2    : std_logic;
signal rot_q2      : std_logic;

signal rot_event  : std_logic;
signal rot_left   : std_logic;

begin

led <= clkxing_int     when rotary(2)='1' else
       match_led       when rotary(2)='0' and cal_ovf='0' else
       (others => '1') when cal_ovf='1';
		 
cal_xing <= clkxing_int(5 downto 0);

-----------------------------------------------------------------------------
-- Rotary encoder
procrot: process(clk)
begin
	if reset='1' then
		clkxing_int <= "00000100";
		cal_inc <= '0';
		cal_dec <= '0';		
   elsif clk'event and clk='1' then
		-- Filter 
		case rotary(1 downto 0) is
		when "00" =>   rot_q1 <= '0';
  					      rot_q2 <= rot_q2;
		when "01" =>   rot_q1 <= rot_q1;
                     rot_q2 <= '0';
		when "10" =>   rot_q1 <= rot_q1;
                     rot_q2 <= '1';
		when "11" =>   rot_q1 <= '1';
                     rot_q2 <= rot_q2;
		when others => rot_q1 <= rot_q1;
                     rot_q2 <= rot_q2;
		end case;
	  
		-- Direction
		rot_q1_2 <= rot_q1;
		if rot_q1='1' and rot_q1_2='0' then
			rot_event <= '1';
			rot_left  <= rot_q2;
		else
			rot_event <= '0';
			rot_left  <= rot_left;
		end if;
		
		-- Phase shifing
		if rot_event='1' and rotary(2)='0' then
			cal_inc <= not rot_left;
			cal_dec <= rot_left;
		else 
			cal_inc <= '0';
			cal_dec <= '0';
		end if;
		
		-- Clock region crossing
		if rot_event='1' and rotary(2)='1' then
			if rot_left='1' then
				clkxing_int(7 downto 1) <= clkxing_int(6 downto 0);
				clkxing_int(0) <= clkxing_int(7);
			else
				clkxing_int(6 downto 0) <= clkxing_int(7 downto 1);
				clkxing_int(7) <= clkxing_int(0);
			end if;
		end if;
   end if;
end process;

end rtl;

