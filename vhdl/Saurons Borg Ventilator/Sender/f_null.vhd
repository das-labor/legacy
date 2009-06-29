library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;



entity f_null is
    Port ( fo : out  STD_LOGIC);
end f_null;

architecture Behavioral of f_null is

begin

fo<='0';

end Behavioral;

