library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity b84 is
    Port ( a : in  STD_LOGIC_VECTOR (7 downto 0);
           q1 : out  STD_LOGIC;
           q2 : out  STD_LOGIC;
           q3 : out  STD_LOGIC;
           q4 : out  STD_LOGIC);
end b84;

architecture Behavioral of b84 is

begin

q1<=a(0);
q2<=a(1);
q3<=a(2);
q4<=a(3);


end Behavioral;

