----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    14:17:01 12/19/2006 
-- Design Name: 
-- Module Name:    pwm_testbench - Behavioral 
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

---- Uncomment the following library declaration if instantiating
---- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity pwm_testbench is
end pwm_testbench;

architecture Behavioral of pwm_testbench is

signal reset : std_logic;
signal clk : std_logic;

begin

reset <= '1', '0' after 100ns;

process
begin
	clk <= '0', '1' after 20ns;
	wait for 40ns;
end process;

pwm: entity work.pwm
    Port map( reset => reset,
           cp2 => clk,
           clk => clk,
			  addr => "00000000000",
			  din => "00000000",
			  we => '0'
           --lsr_clr : out  STD_LOGIC;
           --lsr_d : out  STD_LOGIC;
           --lsr_c : out  STD_LOGIC;
           --latch_data : out  STD_LOGIC_VECTOR (7 downto 0));
		);	  

end Behavioral;

