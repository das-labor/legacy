library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity roh_verzoegerung is
    Port ( clk200 : in  STD_LOGIC;
           a : in  STD_LOGIC;
           q : out  STD_LOGIC);
end roh_verzoegerung;

architecture Behavioral of roh_verzoegerung is

CONSTANT gv : integer := 8; -- generelle verzögerung

signal schieb   : STD_LOGIC_VECTOR (gv downto 0);

begin
schieb1:process (clk200)
begin
  if rising_edge (clk200) then
	  schieb <= schieb ((gv-1)   downto 0) & a;
	  end if;	  
end process schieb1;

q <= schieb(gv);

end Behavioral;

