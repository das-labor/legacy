library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity f00_8 is
    Port ( q0,q1,q2,q4,q8,q16,q32,q64,q128,q255 : out std_logic_vector (7 downto 0));
end f00_8;

architecture Behavioral of f00_8 is

begin
q0    <=  "00000000";
q1    <=  "00000001";
q2    <=  "00000010";
q4    <=  "00000100";
q8    <=  "00001000";
q16   <=  "00010000";
q32   <=  "00100000";
q64   <=  "01000000";
q128  <=  "10000000";
q255  <=  "11111111";


end Behavioral;

