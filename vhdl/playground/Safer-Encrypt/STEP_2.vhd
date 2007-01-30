----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    21:28:37 01/26/2007 
-- Design Name: 
-- Module Name:    STEP_2 - Behavioral 
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

entity STEP_2 is
    Port ( input 	: in  STD_LOGIC_VECTOR (63 downto 0);
           key 	: in  STD_LOGIC_VECTOR (63 downto 0);
           output : out  STD_LOGIC_VECTOR (63 downto 0));
end STEP_2;

architecture Behavioral of STEP_2 is

begin

		output(  7 downto  0) <= key(  7 downto  0) +   input(  7 downto  0);
		output( 15 downto  8) <= key( 15 downto  8) xor input( 15 downto  8);
		output( 23 downto 16) <= key( 23 downto 16) xor input( 23 downto 16);
		output( 31 downto 24) <= key( 31 downto 24) +   input( 31 downto 24);
		output( 39 downto 32) <= key( 39 downto 32) +   input( 39 downto 32);
		output( 47 downto 40) <= key( 47 downto 40) xor input( 47 downto 40);
		output( 55 downto 48) <= key( 55 downto 48) xor input( 55 downto 48);
		output( 63 downto 56) <= key( 63 downto 56) +   input( 63 downto 56);

end Behavioral;

