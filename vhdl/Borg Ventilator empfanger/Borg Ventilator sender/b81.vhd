library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity b81 is
    Port ( a : in  STD_LOGIC_VECTOR (7 downto 0);
           q : out  STD_LOGIC);
end b81;

architecture Behavioral of b81 is

begin

q<=a(0); 


end Behavioral;

