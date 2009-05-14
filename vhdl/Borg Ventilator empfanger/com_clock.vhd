library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity com_clock is
    Port ( com_clk     : in  STD_LOGIC;
           MHZ_200     : in  STD_LOGIC;
           com_clk_2x  : out STD_LOGIC;
			  diag_cf1,diag_cf2: out STD_LOGIC_VECTOR (6 downto 0):="0000000";
			  diag_ff1,diag_ff2,diag_res1,diag_res2: out STD_LOGIC);
end com_clock;

architecture Behavioral of com_clock is

signal NQ_INT1,Q_INT1,NQ_INT3,Q_INT3,res1,res2,cci: std_logic;
signal cf1,cf2: std_logic_vector (6 downto 0);
constant sl : integer := 7;

begin

--------------------------------------------------------------------
cci <= com_clk;

process (MHZ_200)-- Zählen wie lange die High Flanke schon anhält. 
begin
   if rising_edge (MHZ_200) then
      if cci='1' then cf1 <= cf1 + 1; end if;
		if cci='0' then cf1 <= (others => '0'); end if;
   end if;
end process;

process (MHZ_200)-- Zählen wie lange die Low Flanke schon anhält.
begin
   if rising_edge (MHZ_200) then
      if cci='0' then cf2 <= cf2 + 1; end if;
		if cci='1' then cf2 <= (others => '0'); end if;
   end if;
end process;

process (MHZ_200)-- Reset für High flanke generieren 
begin
   if rising_edge (MHZ_200) then
      if cf1>3 then res1 <='1'; else res1 <='0'; end if;
   end if;
end process;

process (MHZ_200)-- Reset für Low Flanke generieren
begin
   if rising_edge (MHZ_200) then
      if cf2>3 then res2 <='1'; else res2 <='0'; end if;
   end if;
end process;

 
-----------------------------------------------------------------------



com_clk_2x <= q_int1 or q_int3;

-- Für Die Fehlersuche:
diag_cf1 <=cf1;
diag_cf2 <=cf2;
diag_ff1 <=q_int1;
diag_ff2 <=q_int3;
diag_res1 <=res1;
diag_res2 <=res2;

-----KOMBINATORISCHE-FF---(UNGETAKTET)-------------------------------------

--Jedes FF wird Sofort bei einem Flankenwechsel gesetzt. Das vermeidet Jitter
nq_int1 <= cci nor  q_int1;
 q_int1 <= res1 nor nq_int1;


nq_int3 <= (not cci) nor  q_int3;
 q_int3 <=  res2 nor nq_int3;

-----------------------------------------------------------------------------------------------
end Behavioral;

