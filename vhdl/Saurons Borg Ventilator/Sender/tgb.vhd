--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
-- Hackerspace:		Das Labor    www.das-labor.org											--
-- Hacker: 				Sauron																			--
-- 																											--
-- Datum:    			09.03.2009        															--
-- Projekt: 			Der Borg Ventilator															--
-- Modul Name:     	tgb.vhd     																	--
-- Beschreibung: 		Ich Erstelle verschiedene Arten von Testmustern um die Daten-  --
--                   übertragung auch ohne reale Daten testen zu können.            --
--							                                											--
--																												--
-- Dependencies: 		Packet_write                                    					--
--																												--
-- Version:          V9.4.1       																	--
-- 																											--
-- Additional Comments: Wiederstand ist Zwecklos												--
--																												--
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

ENTITY tgb is
PORT(
	clk20          : IN  STD_LOGIC;
	a,b,c,d        : IN  STD_LOGIC_VECTOR (7 downto 0);
	pattern        : IN  STD_LOGIC_VECTOR (7 downto 0);
	trig           : OUT std_logic;
	mux            : IN  STD_LOGIC_VECTOR (7 downto 0);
	q1,q2,q3,q4    : OUT STD_LOGIC);
END tgb;

architecture Behavioral of tgb is

-- Komponente packet_write bekanntmachen
COMPONENT packet_write
PORT(
	b8_code      : IN std_logic_vector(7 downto 0);
	clk          : IN std_logic;          
	q1           : OUT std_logic;
	rdy          : OUT std_logic;
	counter      : OUT std_logic_vector(9 downto 0)
	);
END COMPONENT;

 -- Signaldefinitionen

signal c1,c2,c3,c4    : STD_LOGIC;
signal d1,d2,d3,d4    : STD_LOGIC;
signal e1,e2,e3,e4    : STD_LOGIC;
signal en1,en2,en3,en4: STD_LOGIC;
signal trig_i         : STD_LOGIC;
signal q1i,q2i,q3i,q4i: STD_LOGIC_VECTOR (7 downto 0);
signal bitcnt : STD_LOGIC_VECTOR (1 downto 0):="00";


begin


-- Wenn als Soll-Daten der Wert 255 vorgegeben wird, werden sich wechselnde Daten ausgegeben
-- Ansonsten werden die Soll Daten ausgegeben
-- Datengenerierung Kanal 1
process (clk20) begin
if rising_edge (clk20)and en1='1' then
  if a=255 then 
    q1i <= q1i +1;
  else
    q1i <=a;
  end if;	 
end if;
end process;  

	packet_write_1: packet_write PORT MAP(
		b8_code => q1i,
		    clk => clk20,
		     q1 => c1,
	   	 rdy => en1);


-- Datengenerierung Kanal 2
process (clk20) begin
if rising_edge (clk20)and en2='1' then
  if b=255 then 
    q2i <= q2i -7;
  else
    q2i <=b;
  end if;
end if;
end process;  

	packet_write_2: packet_write PORT MAP(
		b8_code => q2i,
		    clk => clk20,
		     q1 => c2,
	   	 rdy => en2);


-- Datengenerierung Kanal 3
process (clk20) begin
if rising_edge (clk20)and en3='1' then
  if c=255 then 
    q3i <= q3i +3;
  else
    q3i <=c;
  end if;
end if;
end process;  

	packet_write_3: packet_write PORT MAP(
		b8_code => q3i,
		    clk => clk20,
		     q1 => c3,
	   	 rdy => en3);


-- Datengenerierung Kanal 4
process (clk20) begin
if rising_edge (clk20)and en4='1' then
  if d=255 then 
    q4i <= q4i -5;
  else
    q4i <=d;
  end if;
end if;
end process;  

	packet_write_4: packet_write PORT MAP(
		b8_code => q4i,
		    clk => clk20,
		     q1 => c4,
	   	 rdy => en4);

--================== ALTERNATIVE: TESTPATTERN ZUM EINSTELLEN DER VERZÖGERUNG ===============================

-- Zähler, der immern von 0 bis 3 Zählt
process (clk20)
begin
  if rising_edge (clk20) then
    bitcnt <= bitcnt +1;
  end if;
end process;

process (clk20)
begin
  if rising_edge (clk20) then
    if bitcnt = 3 then       -- Daten vom Letzten Takt
	    e1 <= Pattern (7) ; e2 <= Pattern (5) ; e3 <= Pattern (3) ; e4 <= Pattern (1);
	    trig_i <= '0'; 
	 end if;
    if bitcnt = 0 then       --  aktuelle Daten
	    e1 <= Pattern (6) ; e2 <= Pattern (4) ; e3 <= Pattern (2) ; e4 <= Pattern (0);
	    trig_i <= '1'; -- Triggerung für Scope
	 end if;
    if bitcnt = 1 then       --  Daten vom Letzten Takt invertiert
	    e1 <= not Pattern (7) ; e2 <= not Pattern (5) ; e3 <= not Pattern (3) ; e4 <= not Pattern (1);
	    trig_i <= '0';
	 end if;
    if bitcnt = 2 then       -- aktuelle Daten invertiert
	    e1 <= not Pattern (6) ; e2 <= not Pattern (4) ; e3 <= not Pattern (2) ; e4 <= not Pattern (0);
	    trig_i <= '0';
	 end if;
  end if;
end process;

-- Daten Ausgeben

-- wenn x=0 dann Kanal ausschalten
d1 <= '0' when a=0 else c1;
d2 <= '0' when b=0 else c2;
d3 <= '0' when c=0 else c3;
d4 <= '0' when d=0 else c4;

-- wenn mux = 0 dann Testdaten ausgeben, bei mux = 1 Pattern ausgeben
q1 <= e1 when mux (0) = '1' else d1 ;
q2 <= e2 when mux (1) = '1' else d2 ;
q3 <= e3 when mux (2) = '1' else d3 ;
q4 <= e4 when mux (3) = '1' else d4 ;

--Trigger passend setzen
trig <= trig_i when mux (0) = '1' else en1;

end Behavioral;
