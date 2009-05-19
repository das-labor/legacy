library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity b83 is
    Port ( a : in  STD_LOGIC_VECTOR (7 downto 0);
           q : out  STD_LOGIC_VECTOR (2 downto 0));
end b83;

architecture Behavioral of b83 is

begin

q(0)<=a(0); 
q(1)<=a(1); 
q(2)<=a(2); 

end Behavioral;

