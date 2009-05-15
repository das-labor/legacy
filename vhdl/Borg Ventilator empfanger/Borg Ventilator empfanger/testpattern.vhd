
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity testpattern is
    Port ( Pattern : in  STD_LOGIC_VECTOR (7 downto 0);
  			  clk20   : in  STD_LOGIC;
			  cnt: out STD_LOGIC_VECTOR (1 downto 0);
			  q1 : out  STD_LOGIC;
           q2 : out  STD_LOGIC;
           q3 : out  STD_LOGIC;
           q4,trig : out  STD_LOGIC);
end testpattern;

architecture Behavioral of testpattern is

signal bitcnt : STD_LOGIC_VECTOR (1 downto 0):="00";
begin

process (clk20)
begin
  if rising_edge (clk20) then
    bitcnt <= bitcnt +1;
  end if;
end process;
cnt <= bitcnt;

process (clk20)
begin
  if rising_edge (clk20) then
    if bitcnt = 3 then       -- Daten vom Letzten Takt
	    q1 <= Pattern (7) ; q2 <= Pattern (5) ; q3 <= Pattern (3) ; q4 <= Pattern (1);
	    trig <= '0'; 
	 end if;
    if bitcnt = 0 then       --  aktuelle Daten
	    q1 <= Pattern (6) ; q2 <= Pattern (4) ; q3 <= Pattern (2) ; q4 <= Pattern (0);
	    trig <= '1'; -- Triggerung für Scope
	 end if;
    if bitcnt = 1 then       --  Daten vom Letzten Takt invertiert
	    q1 <= not Pattern (7) ; q2 <= not Pattern (5) ; q3 <= not Pattern (3) ; q4 <= not Pattern (1);
	    trig <= '0';
	 end if;
    if bitcnt = 2 then       -- aktuelle Daten invertiert
	    q1 <= not Pattern (6) ; q2 <= not Pattern (4) ; q3 <= not Pattern (2) ; q4 <= not Pattern (0);
	    trig <= '0';
	 end if;
  end if;
end process;

end Behavioral;

