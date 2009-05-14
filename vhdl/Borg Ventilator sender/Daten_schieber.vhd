------------------------------------------------------------------------------------
------------------------------------------------------------------------------------
-- Hackerspace:		Das Labor    www.das-labor.org                               --
-- Hacker: 				Sauron                                                       --
--                                                                                --
-- Datum:    			03.04.2009 									                         --
-- Änderung:         10.04.2009                                                   --
-- Projekt: 			Der Borg Ventilator													    --
-- Modul Name:     	daten_schieber.vhd                                           --
-- Beschreibung: 		meine Aufgabe ist es, die Datensignale so zu versögern, dass --
--                   sie gleichzeitig beim Empfänger ankommen. Die nötigen        --
--							verzögerungszeiten erhalte ich aus einem block ram           --
--							verschoben wird in schritten von 5 ns bereich -15 bis +20ns  --
--																											 --
-- Dependencies: 		KCPSM3, pb_rom       													 --
--																											 --
-- Version:          V9.4.1																		 --
-- 																										 --
-- Additional Comments: Wiederstand ist Zwecklos											 --
--																											 --
------------------------------------------------------------------------------------
------------------------------------------------------------------------------------

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
           data_out2: out STD_LOGIC;
           data_out3: out STD_LOGIC;
           data_out4: out STD_LOGIC;
			  wr_str   : in STD_LOGIC;
			  p_id     : in STD_LOGIC_VECTOR (7 downto 0);
			  out_p    : in STD_LOGIC_VECTOR (7 downto 0);
--           wenb1    : in  STD_LOGIC_VECTOR (7 downto 0);
--           wenb2    : in  STD_LOGIC_VECTOR (7 downto 0);
--           wenb3    : in  STD_LOGIC_VECTOR (7 downto 0);
--           wenb4    : in  STD_LOGIC_VECTOR (7 downto 0);
--			  inb1     : in  STD_LOGIC_VECTOR (7 downto 0);
--			  inb2     : in  STD_LOGIC_VECTOR (7 downto 0);
--			  inb3     : in  STD_LOGIC_VECTOR (7 downto 0);
--			  inb4     : in  STD_LOGIC_VECTOR (7 downto 0);
--			  addrb1   : in  STD_LOGIC_VECTOR (7 downto 0);
--			  addrb2   : in  STD_LOGIC_VECTOR (7 downto 0);
--			  addrb3   : in  STD_LOGIC_VECTOR (7 downto 0);
--			  addrb4   : in  STD_LOGIC_VECTOR (7 downto 0);
			  outb1    : out STD_LOGIC_VECTOR (7 downto 0);
           outb2    : out STD_LOGIC_VECTOR (7 downto 0);
           outb3    : out STD_LOGIC_VECTOR (7 downto 0);
           outb4    : out STD_LOGIC_VECTOR (7 downto 0));

end Daten_schieber;

architecture Behavioral of Daten_schieber is

CONSTANT gv : integer := 4; -- generelle verzögerung

signal clk1,clk2,resclk: STD_LOGIC;
signal data1_old  : STD_LOGIC;
signal data2_old  : STD_LOGIC;
signal data3_old  : STD_LOGIC;
signal data4_old  : STD_LOGIC;
signal do1,do2    : STD_LOGIC;
signal do3,do4    : STD_LOGIC;
signal s1a,s2a,s3a,s4a: STD_LOGIC_VECTOR (gv+1 downto 0);
signal s1b,s2b,s3b,s4b: STD_LOGIC_VECTOR (gv+2 downto 0);
signal s1c,s2c,s3c,s4c: STD_LOGIC_VECTOR (gv+3 downto 0);
signal s1d,s2d,s3d,s4d: STD_LOGIC_VECTOR (gv+4 downto 0);
signal s1e,s2e,s3e,s4e: STD_LOGIC_VECTOR (gv+5 downto 0);
signal s1f,s2f,s3f,s4f: STD_LOGIC_VECTOR (gv+6 downto 0);
signal s1g,s2g,s3g,s4g: STD_LOGIC_VECTOR (gv+7 downto 0);
signal s1h,s2h,s3h,s4h: STD_LOGIC_VECTOR (gv+8 downto 0);
signal clkcnt      : STD_LOGIC_VECTOR (7 downto 0);
signal pattern     : STD_LOGIC_VECTOR (7 downto 0);
signal dinb1,doutb1: STD_LOGIC_VECTOR (2 downto 0);
signal dinb2,doutb2: STD_LOGIC_VECTOR (2 downto 0);
signal dinb3,doutb3: STD_LOGIC_VECTOR (2 downto 0);
signal dinb4,doutb4: STD_LOGIC_VECTOR (2 downto 0);
signal switch_sync1: STD_LOGIC_VECTOR (2 downto 0);
signal switch_sync2: STD_LOGIC_VECTOR (2 downto 0);
signal switch_sync3: STD_LOGIC_VECTOR (2 downto 0);
signal switch_sync4: STD_LOGIC_VECTOR (2 downto 0);
signal switch1     : STD_LOGIC_VECTOR (2 downto 0);
signal switch2     : STD_LOGIC_VECTOR (2 downto 0);
signal switch3     : STD_LOGIC_VECTOR (2 downto 0);
signal switch4     : STD_LOGIC_VECTOR (2 downto 0);
signal wenb1       : STD_LOGIC_VECTOR (7 downto 0);
signal wenb2       : STD_LOGIC_VECTOR (7 downto 0);
signal wenb3       : STD_LOGIC_VECTOR (7 downto 0);
signal wenb4       : STD_LOGIC_VECTOR (7 downto 0);
signal inb1        : STD_LOGIC_VECTOR (7 downto 0);
signal inb2        : STD_LOGIC_VECTOR (7 downto 0);
signal inb3        : STD_LOGIC_VECTOR (7 downto 0);
signal inb4        : STD_LOGIC_VECTOR (7 downto 0);
signal addrb1      : STD_LOGIC_VECTOR (7 downto 0);
signal addrb2      : STD_LOGIC_VECTOR (7 downto 0);
signal addrb3      : STD_LOGIC_VECTOR (7 downto 0);
signal addrb4      : STD_LOGIC_VECTOR (7 downto 0);

component korrekturdaten
	port (
	addra: IN std_logic_VECTOR(7 downto 0);
	addrb: IN std_logic_VECTOR(7 downto 0);
	clka: IN std_logic;
	clkb: IN std_logic;
	dinb: IN std_logic_VECTOR(2 downto 0);
	douta: OUT std_logic_VECTOR(2 downto 0);
	doutb: OUT std_logic_VECTOR(2 downto 0);
	web: IN std_logic);
end component;


------------------------DIE-SCHIEBEREGISTER----------------------------------------------------------
begin
-- Jede der 8 möglichkeiten erhätt ein eigenes schieberegister, Die synthese macht die nötigen optimierungen
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

schieb2:process (clk200)
begin
  if rising_edge (clk200) then
	  s2a <= s2a (gv   downto 0) & data2_in;
	  s2b <= s2b (gv+1 downto 0) & data2_in;
	  s2c <= s2c (gv+2 downto 0) & data2_in;
	  s2d <= s2d (gv+3 downto 0) & data2_in;
	  s2e <= s2e (gv+4 downto 0) & data2_in;
	  s2f <= s2f (gv+5 downto 0) & data2_in;
	  s2g <= s2g (gv+6 downto 0) & data2_in;
     s2h <= s2h (gv+7 downto 0) & data2_in;
	  end if;	  
end process schieb2;

schieb3:process (clk200)
begin
  if rising_edge (clk200) then
	  s3a <= s3a (gv   downto 0) & data3_in;
	  s3b <= s3b (gv+1 downto 0) & data3_in;
	  s3c <= s3c (gv+2 downto 0) & data3_in;
	  s3d <= s3d (gv+3 downto 0) & data3_in;
	  s3e <= s3e (gv+4 downto 0) & data3_in;
	  s3f <= s3f (gv+5 downto 0) & data3_in;
	  s3g <= s3g (gv+6 downto 0) & data3_in;
     s3h <= s3h (gv+7 downto 0) & data3_in;
	  end if;	  
end process schieb3;

schieb4:process (clk200)
begin
  if rising_edge (clk200) then
	  s4a <= s4a (gv   downto 0) & data4_in;
	  s4b <= s4b (gv+1 downto 0) & data4_in;
	  s4c <= s4c (gv+2 downto 0) & data4_in;
	  s4d <= s4d (gv+3 downto 0) & data4_in;
	  s4e <= s4e (gv+4 downto 0) & data4_in;
	  s4f <= s4f (gv+5 downto 0) & data4_in;
	  s4g <= s4g (gv+6 downto 0) & data4_in;
     s4h <= s4h (gv+7 downto 0) & data4_in;
	  end if;	  
end process schieb4;

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

   

---------------------------MULTIPLEXER---------------------------------------------------------------
-- kanal 1
process (switch_sync1,s1a(gv+1),s1b(gv+2),s1c(gv+3),s1d(gv+4),s1e(gv+5),s1f(gv+6),s1g(gv+7),s1h(gv+8))
begin
   case switch_sync1 is
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

-- kanal 2
process (switch_sync2,s2a(gv+1),s2b(gv+2),s2c(gv+3),s2d(gv+4),s2e(gv+5),s2f(gv+6),s2g(gv+7),s2h(gv+8))
begin
   case switch_sync2 is
      when "000" => do2 <= s2a(gv+1);
      when "001" => do2 <= s2b(gv+2);
      when "010" => do2 <= s2c(gv+3);
      when "011" => do2 <= s2d(gv+4);
      when "100" => do2 <= s2e(gv+5);
      when "101" => do2 <= s2f(gv+6);
      when "110" => do2 <= s2g(gv+7);
      when "111" => do2 <= s2h(gv+8);
      when others => do2 <= s2d(gv+4);
   end case;
end process;

-- kanal 3
process (switch_sync3,s3a(gv+1),s3b(gv+2),s3c(gv+3),s3d(gv+4),s3e(gv+5),s3f(gv+6),s3g(gv+7),s3h(gv+8))
begin
   case switch_sync3 is
      when "000" => do3 <= s3a(gv+1);
      when "001" => do3 <= s3b(gv+2);
      when "010" => do3 <= s3c(gv+3);
      when "011" => do3 <= s3d(gv+4);
      when "100" => do3 <= s3e(gv+5);
      when "101" => do3 <= s3f(gv+6);
      when "110" => do3 <= s3g(gv+7);
      when "111" => do3 <= s3h(gv+8);
      when others => do3 <= s3d(gv+4);
   end case;
end process;

-- kanal 4
process (switch_sync4,s4a(gv+1),s4b(gv+2),s4c(gv+3),s4d(gv+4),s4e(gv+5),s4f(gv+6),s4g(gv+7),s4h(gv+8))
begin
   case switch_sync4 is
      when "000" => do4 <= s4a(gv+1);
      when "001" => do4 <= s4b(gv+2);
      when "010" => do4 <= s4c(gv+3);
      when "011" => do4 <= s4d(gv+4);
      when "100" => do4 <= s4e(gv+5);
      when "101" => do4 <= s4f(gv+6);
      when "110" => do4 <= s4g(gv+7);
      when "111" => do4 <= s4h(gv+8);
      when others => do4 <= s4d(gv+4);
   end case;
end process;

process(clk200)-- Genauen Zeitpunkt zum Muxer Umstellen Festlegen
 begin
   if rising_edge(clk200) and clkcnt = 3 then
     switch_sync1 <= switch1;
     switch_sync2 <= switch2;
     switch_sync3 <= switch3;
     switch_sync4 <= switch4;
   end if;
end process;


-----------------------------------------------------------------------------------------------------
data_out1 <= do1;
data_out2 <= do2;
data_out3 <= do3;
data_out4 <= do4;
---------------------------Memory-------------------------------------------------------------------
-- Anbindung Block Ram für Kanal 1
k1 :korrekturdaten
port map (     addra=>pattern,
	            addrb=>addrb1,
	             clka=>clk200,
            	 clkb=>clk200,
	             dinb=>dinb1,
	            douta=>switch1,
	            doutb=>doutb1,
	              web=>wenb1 (0) 
					);

dinb1 <=inb1 (2 downto 0);
outb1 <= "00000" & doutb1;

-- Anbindung Block Ram für Kanal 2
k2 :korrekturdaten
port map (     addra=>pattern,
	            addrb=>addrb2,
	             clka=>clk200,
            	 clkb=>clk200,
	             dinb=>dinb2,
	            douta=>switch2,
	            doutb=>doutb2,
	              web=>wenb2 (0) 
					);

dinb2 <=inb2 (2 downto 0);
outb2 <= "00000" & doutb2;

-- Anbindung Block Ram für Kanal 3
k3 :korrekturdaten
port map (     addra=>pattern,
	            addrb=>addrb3,
	             clka=>clk200,
            	 clkb=>clk200,
	             dinb=>dinb3,
	            douta=>switch3,
	            doutb=>doutb3,
	              web=>wenb3 (0) 
					);

dinb3 <=inb3 (2 downto 0);
outb3 <= "00000" & doutb3;

-- Anbindung Block Ram für Kanal 4
k4 :korrekturdaten
port map (     addra=>pattern,
	            addrb=>addrb4,
	             clka=>clk200,
            	 clkb=>clk200,
	             dinb=>dinb4,
	            douta=>switch4,
	            doutb=>doutb4,
	              web=>wenb4 (0) 
					);

dinb4 <=inb4 (2 downto 0);
outb4 <= "00000" & doutb4;

-- INTERFACE FÜR ANBINDUND DER BLOCK RAM AN DEN PICOBLAZE

process (clk200) begin if rising_edge (clk200) then if p_id = 60  and wr_str = '1' then  wenb1 <= out_p   ; end if;end if; end process;
process (clk200) begin if rising_edge (clk200) then if p_id = 61  and wr_str = '1' then  wenb2 <= out_p   ; end if;end if; end process;
process (clk200) begin if rising_edge (clk200) then if p_id = 62  and wr_str = '1' then  wenb3 <= out_p   ; end if;end if; end process;
process (clk200) begin if rising_edge (clk200) then if p_id = 63  and wr_str = '1' then  wenb4 <= out_p   ; end if;end if; end process;
process (clk200) begin if rising_edge (clk200) then if p_id = 64  and wr_str = '1' then   inb1 <= out_p   ; end if;end if; end process;
process (clk200) begin if rising_edge (clk200) then if p_id = 65  and wr_str = '1' then   inb2 <= out_p   ; end if;end if; end process;
process (clk200) begin if rising_edge (clk200) then if p_id = 66  and wr_str = '1' then   inb3 <= out_p   ; end if;end if; end process;
process (clk200) begin if rising_edge (clk200) then if p_id = 67  and wr_str = '1' then   inb4 <= out_p   ; end if;end if; end process;
process (clk200) begin if rising_edge (clk200) then if p_id = 68  and wr_str = '1' then addrb1 <= out_p   ; end if;end if; end process;
process (clk200) begin if rising_edge (clk200) then if p_id = 69  and wr_str = '1' then addrb2 <= out_p   ; end if;end if; end process;
process (clk200) begin if rising_edge (clk200) then if p_id = 70  and wr_str = '1' then addrb3 <= out_p   ; end if;end if; end process;
process (clk200) begin if rising_edge (clk200) then if p_id = 71  and wr_str = '1' then addrb4 <= out_p   ; end if;end if; end process;

end Behavioral;

