----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    22:45:37 05/23/2009 
-- Design Name: 
-- Module Name:    com_20_status - Behavioral 
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

entity com_20_status is
    Port ( a : in  STD_LOGIC;
           b : in  STD_LOGIC_VECTOR (7 downto 0);
           q : out  STD_LOGIC_VECTOR (7 downto 0));
end com_20_status;

architecture Behavioral of com_20_status is

begin
q(0)<=not a;  --1= not locked
q(1)<=b(0);   --1= Phase shift overflow
q(2)<=b(1);   --1= loos of input clock
q(3)<='0';    
q(4)<='0';
q(5)<='0';
q(6)<='0';
q(7)<='0';






end Behavioral;

