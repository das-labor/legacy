----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    18:40:56 06/15/2006 
-- Design Name: 
-- Module Name:    Main - Behavioral 
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

entity Main is
    Port ( LED : out  STD_LOGIC_VECTOR (7 downto 0);
           SEGMENT : out  STD_LOGIC_VECTOR (7 downto 0);
           DIGIT : out  STD_LOGIC_VECTOR (3 downto 0);
           UART_TXD : out  STD_LOGIC;
           UART_RXD : in  STD_LOGIC;
           BTN : in  STD_LOGIC_VECTOR (3 downto 0);
           SW : in  STD_LOGIC_VECTOR (7 downto 0);
           CLK : in  STD_LOGIC);
end Main;

architecture Behavioral of Main is
	signal RESET : STD_LOGIC;
begin
	

RESETPROC: process (RESET)
	begin
	end process;

SWPROC: process(SW)
	begin
		SEGMENT <= SW;
	end process;

-----------------------------------------------------------------------------
-- default NULL
NIX: process (BTN, SW)
	begin
	end process;

	RESET    <= BTN(3);
	LED      <= (others => '1');
	DIGIT    <= (others => '0');
	UART_TXD <= UART_RXD;
end Behavioral;

