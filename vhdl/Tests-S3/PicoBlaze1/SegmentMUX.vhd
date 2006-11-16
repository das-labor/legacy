----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    04:28:17 06/17/2006 
-- Design Name: 
-- Module Name:    SegmentMUX - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity SegmentMUX is
    Port ( digit0 : in  STD_LOGIC_VECTOR (3 downto 0);
           digit1 : in  STD_LOGIC_VECTOR (3 downto 0);
           digit2 : in  STD_LOGIC_VECTOR (3 downto 0);
           digit3 : in  STD_LOGIC_VECTOR (3 downto 0);
           segments : out  STD_LOGIC_VECTOR (7 downto 0);
           mux : out  STD_LOGIC_VECTOR (3 downto 0);
           clk : in  STD_LOGIC);
end SegmentMUX;

architecture Behavioral of SegmentMUX is

signal segmentcount : STD_LOGIC_VECTOR(14 downto 0);
signal nibble       : STD_LOGIC_VECTOR(3 downto 0);

begin

MUXERPROC: process(clk)
begin
   if clk='1' and clk'event then
      segmentcount <= segmentcount + 1;
   end if;
end process;

with segmentcount(14 downto 13) select
   mux   <= "0111" when "00",
            "1011" when "01",
            "1101" when "10",
            "1110" when "11",
            "----" when others;

with segmentcount(14 downto 13) select 
   nibble <= digit0 when "00",
             digit1 when "01",
             digit2 when "10",
             digit3 when "11",
             "----" when others;

with nibble select 
   segments <= "11000000" when "0000",   
               "11111001" when "0001",   
               "10100100" when "0010",
               "10110000" when "0011",   
               "10011001" when "0100",
               "10010010" when "0101",
               "10000010" when "0110",   
               "11111000" when "0111",   
               "10000000" when "1000",
               "10010000" when "1001",
               "10001000" when "1010",
               "10000011" when "1011",
               "11000110" when "1100",       
               "10100001" when "1101",
               "10000110" when "1110",
               "10001110" when "1111",
               "01111111" when others;
end Behavioral;

