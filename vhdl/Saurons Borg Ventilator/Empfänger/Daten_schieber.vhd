library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Daten_schieber is
    Port ( clk20    : in  STD_LOGIC;
           clk200   : in  STD_LOGIC;
           data1_in : in  STD_LOGIC;
           data2_in : in  STD_LOGIC;
           data3_in : in  STD_LOGIC;
           data4_in : in  STD_LOGIC;
           data_out1: out STD_LOGIC;
           switch   : in  STD_LOGIC_VECTOR (2 downto 0);
			  pattern_o: out STD_LOGIC_VECTOR (7 downto 0);
			  cnt      : out STD_LOGIC_VECTOR (7 downto 0));
end Daten_schieber;

architecture Behavioral of Daten_schieber is

CONSTANT gv : integer := 4; -- generelle verzögerung

signal clk1,clk2,resclk,do1 : STD_LOGIC;
signal data1_old : STD_LOGIC;
signal data2_old : STD_LOGIC;
signal data3_old : STD_LOGIC;
signal data4_old : STD_LOGIC;
signal s1a       : STD_LOGIC_VECTOR (gv+1 downto 0);
signal s1b       : STD_LOGIC_VECTOR (gv+2 downto 0);
signal s1c       : STD_LOGIC_VECTOR (gv+3 downto 0);
signal s1d       : STD_LOGIC_VECTOR (gv+4 downto 0);
signal s1e       : STD_LOGIC_VECTOR (gv+5 downto 0);
signal s1f       : STD_LOGIC_VECTOR (gv+6 downto 0);
signal s1g       : STD_LOGIC_VECTOR (gv+7 downto 0);
signal s1h       : STD_LOGIC_VECTOR (gv+8 downto 0);
signal clkcnt    : STD_LOGIC_VECTOR (7 downto 0);
signal pattern   : STD_LOGIC_VECTOR (7 downto 0);
signal switch_sync : STD_LOGIC_VECTOR (2 downto 0);

------------------------DIE-SCHIEBEREGISTER----------------------------------------------------------
begin

schieb1:process (clk200)
begin
  if rising_edge (clk200) then
	  s1a <= s1a (gv   downto 0) & data1_in;
	  s1b <= s1b (gv+1 downto 0) & data1_in;
	  s1c <= s1c (gv+2 downto 0) & data1_in;
	  s1d <= s1d (gv+3 downto 0) & data1_in;
	  s1e <= s1e (gv+4 downto 0) & data1_in;
	  s1f <= s1f (gv+5 downto 0) & data1_in;
	  s1g <= s1g (gv+6 downto 0) & data1_in;
     s1h <= s1h (gv+7 downto 0) & data1_in;
	  end if;	  
end process schieb1;

--------------------ZÄHLEN-VOM-BEGINN-DER-TAKTFLANKE------------------------------------------------
process (clk200)
begin
  if rising_edge (clk200) then
    if resclk = '1' then clkcnt <= (others => '0');
	 else clkcnt <= clkcnt + 1;
	 end if;
  end if;
end process;

process(clk200)-- Steigende Flange auf clk Suchen ( zum Löschen des Zählers)
 begin
   if rising_edge(clk200) then
     clk1 <= clk20;
     clk2 <= clk1;
   end if;
 end process;
resclk <= clk1 and not clk2; 

----------------------DATENANALYSE------------------------------------------------------------------
process(clk20)-- alte Daten Zwischenspeichern
 begin
   if rising_edge(clk20) then
     data1_old <= data1_in;
     data2_old <= data2_in;
     data3_old <= data3_in;
     data4_old <= data4_in;
   end if;
end process;

pattern <= data1_old & data1_in & data2_old & data2_in & data3_old & data3_in & data4_old & data4_in;

-- mit pattern in eine Tabelle schauen (bram) und dort einen Korrekturwert abholen

--  with pattern SELect
--   LED<= 3 when "01010101",   
--         3 when "10101010",   
--         3 when others;   
-- 
    

---------------------------MULTIPLEXER---------------------------------------------------------------
process (switch_sync,s1a(gv+1),s1b(gv+2),s1c(gv+3),s1d(gv+4),s1e(gv+5),s1f(gv+6),s1g(gv+7),s1h(gv+8))
begin
   case switch_sync is
      when "000" => do1 <= s1a(gv+1);
      when "001" => do1 <= s1b(gv+2);
      when "010" => do1 <= s1c(gv+3);
      when "011" => do1 <= s1d(gv+4);
      when "100" => do1 <= s1e(gv+5);
      when "101" => do1 <= s1f(gv+6);
      when "110" => do1 <= s1g(gv+7);
      when "111" => do1 <= s1h(gv+8);
      when others => do1 <= s1d(gv+4);
   end case;
end process;

process(clk200)-- Genauen Zeitpunkt zum Muxer Umstellen Festlegen
 begin
   if rising_edge(clk200) and clkcnt = 3 then
     switch_sync <= switch;
   end if;
end process;


-----------------------------------------------------------------------------------------------------
--d1o <= data1_old;
--d1a <= data1_in;
cnt       <= clkcnt;
data_out1 <= do1;
pattern_o <= pattern;
end Behavioral;

