------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity tgb is
    Port ( clk : in  STD_LOGIC;
           a,b,c,d     : in  STD_LOGIC_VECTOR (7 downto 0);
           q1,q2,q3,q4 : out  STD_LOGIC_VECTOR (7 downto 0);
           en1,en2,en3,en4: in STD_LOGIC
			 
			 );
end tgb;

architecture Behavioral of tgb is

signal c1,c2,c3,c4 : STD_LOGIC_VECTOR (7 downto 0);
signal q1i,q2i,q3i,q4i: STD_LOGIC_VECTOR (7 downto 0);


begin



process (clk) begin
if rising_edge (clk)and en1='1' then
  if a=255 then 
    q1i <= q1i +1;
  else
    q1i <=a;
  end if;	 
end if;
end process;  

process (clk) begin
if rising_edge (clk)and en2='1' then
  if b=255 then 
    q2i <= q2i -7;
  else
    q2i <=b;
  end if;
end if;
end process;  

process (clk) begin
if rising_edge (clk)and en3='1' then
  if c=255 then 
    q3i <= q3i +3;
  else
    q3i <=c;
  end if;
end if;
end process;  

process (clk) begin
if rising_edge (clk)and en4='1' then
  if d=255 then 
    q4i <= q4i -5;
  else
    q4i <=d;
  end if;
end if;
end process;  

q1 <= q1i;
q2 <= q2i;
q3 <= q3i;
q4 <= q4i;


end Behavioral;
