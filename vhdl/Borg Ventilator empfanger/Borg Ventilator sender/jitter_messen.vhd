library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity jitter_messen is
    Port ( clk20 : in  STD_LOGIC;
           clk200 : in  STD_LOGIC;
           daten : in  STD_LOGIC;
           jitter : out  STD_LOGIC_VECTOR (2 downto 0);
           code : out  STD_LOGIC_VECTOR (7 downto 0));
end jitter_messen;

architecture Behavioral of jitter_messen is

begin



end Behavioral;

