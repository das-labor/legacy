----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    20:22:40 01/08/2007 
-- Design Name: 
-- Module Name:    PSEUDO_HADAMARD_TRANSFORMATION - Behavioral 
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

entity PSEUDO_HADAMARD_TRANSFORMATION is	Port( in_left 		: in std_logic_vector(7 downto 0);			in_right 	: in std_logic_vector(7 downto 0);			out_left 	: out std_logic_vector(7 downto 0);
			out_right 	: out std_logic_vector(7 downto 0));
end PSEUDO_HADAMARD_TRANSFORMATION;

architecture Behavioral of PSEUDO_HADAMARD_TRANSFORMATION is

	begin
		out_left <= ( in_left(6 downto 0) & '0' )  + in_right;
		out_right <= in_left + in_right;

end Behavioral;
