------------------------------------------------------------------------------------
------------------------------------------------------------------------------------
-- Hackerspace:		Das Labor    www.das-labor.org						      	    --
-- Hacker: 				Sauron																		 --
-- 																										 --
-- Datum:    			02.04.2009 													             --
-- Geändert:         10.04.2009                                                   --
-- Projekt: 			Der Borg Ventilator														 --
-- Modul Name:     	display2.vhd 																 --
-- Beschreibung: 		Ich enthalte ein interface und kann daten als Bitmuster oder --
--                   als Hex wert oder altenativ als Dezimal Zahl anzeigen.       --
--							Ziffern werden gemultiplext,     									 --
--																											 --
-- Dependencies: 		keine                													 --
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


entity display2 is
    Port ( clk  : in STD_LOGIC;
			  wr_str   : in STD_LOGIC;
			  p_id     : in STD_LOGIC_VECTOR (7 downto 0);
			  out_p    : in STD_LOGIC_VECTOR (7 downto 0);
			  Display: out  STD_LOGIC_VECTOR (11 downto 0));
                                                         			  
end display2;


architecture Behavioral of display2 is

constant adr    : integer := 20; -- Startadresse für Interface

signal segment            : STD_LOGIC_VECTOR (1 downto 0):="00";
signal seg_dauer          : STD_LOGIC_VECTOR (7 downto 0):=(others => '0');
signal seg_tmp            : STD_LOGIC_VECTOR (7 downto 0):=(others => '0'); 
signal int_cnt,bcd_cnt    : STD_LOGIC_VECTOR (15 downto 0):=(others => '0');
signal bcd,bcd_int,bcd_ext: STD_LOGIC_VECTOR (3 downto 0);
signal seven_seg          : STD_LOGIC_VECTOR (6 downto 0);
signal bin                : STD_LOGIC_VECTOR (7 downto 0); 
signal int_hi,int_lo      : STD_LOGIC_VECTOR (7 downto 0); 
signal bin0,bin1,bin2,bin3: STD_LOGIC_VECTOR (7 downto 0); 
signal hell0,hell1,hell2,hell3: STD_LOGIC_VECTOR (7 downto 0):="01111111"; 
signal bcd_gewandelt      : std_logic_vector (15 downto 0);
signal seg0,seg1,seg2,seg3: std_logic;
signal hex,binaer,punkt   : std_logic;
signal punkte             : std_logic_vector (3 downto 0);

begin

----------------INTERFACE-----------------------------------------------------------

--  process (clk) begin 
--    if rising_edge (clk) then 
--      if p_id = adr + 0 and wr_str = '1' then  
--	     hex0 <= out_p (3 downto 0); 
--	   end if;
--    end if; 
--  end process;

-- UND-DAS-SELBE-IN-KURZ

process (clk) begin if rising_edge (clk) then if p_id = adr + 4  and wr_str = '1' then  int_hi <= out_p   ; end if;end if; end process;
process (clk) begin if rising_edge (clk) then if p_id = adr + 5  and wr_str = '1' then  int_lo <= out_p   ; end if;end if; end process;
process (clk) begin if rising_edge (clk) then if p_id = adr + 6  and wr_str = '1' then  bin0   <= out_p   ; end if;end if; end process;
process (clk) begin if rising_edge (clk) then if p_id = adr + 7  and wr_str = '1' then  bin1   <= out_p   ; end if;end if; end process;
process (clk) begin if rising_edge (clk) then if p_id = adr + 8  and wr_str = '1' then  bin2   <= out_p   ; end if;end if; end process;
process (clk) begin if rising_edge (clk) then if p_id = adr + 9  and wr_str = '1' then  bin3   <= out_p   ; end if;end if; end process;
process (clk) begin if rising_edge (clk) then if p_id = adr + 10 and wr_str = '1' then  hell0  <= out_p   ; end if;end if; end process;
process (clk) begin if rising_edge (clk) then if p_id = adr + 11 and wr_str = '1' then  hell1  <= out_p   ; end if;end if; end process;
process (clk) begin if rising_edge (clk) then if p_id = adr + 12 and wr_str = '1' then  hell2  <= out_p   ; end if;end if; end process;
process (clk) begin if rising_edge (clk) then if p_id = adr + 13 and wr_str = '1' then  hell3  <= out_p   ; end if;end if; end process;
process (clk) begin if rising_edge (clk) then if p_id = adr + 14 and wr_str = '1' then  hex    <= out_p(0); end if;end if; end process;
process (clk) begin if rising_edge (clk) then if p_id = adr + 15 and wr_str = '1' then  binaer <= out_p(0); end if;end if; end process;
process (clk) begin if rising_edge (clk) then if p_id = adr + 16 and wr_str = '1' then  punkte <= out_p (3 downto 0); end if;end if; end process;


-----------MULTIPLEXER-FÜR-WECHSEL-DER-STELLE---------------------------------------
process (clk)
begin
if rising_edge (clk) then
  seg_tmp <= seg_tmp+1;
end if;
end process;

process (clk)
begin
if rising_edge (clk)and seg_tmp = 0 then
  seg_dauer <= seg_dauer+1;
  if seg_dauer=0 then 
    segment <= segment +1;
  end if;	 
end if;
end process;



process (clk)--  HELLIGKEIT FÜR DIE EINZELNEN SEGMENTE (PWM)	
begin
if rising_edge (clk) then
  if (seg_dauer < hell0) and segment=0 then seg0 <= '0'; else seg0 <= '1'; end if;	 
end if;
end process;

process (clk)--  HELLIGKEIT FÜR DIE EINZELNEN SEGMENTE (PWM)	
begin
if rising_edge (clk) then
  if (seg_dauer < hell1) and segment=1 then seg1 <= '0'; else seg1 <= '1'; end if;	 
end if;
end process;

process (clk)--  HELLIGKEIT FÜR DIE EINZELNEN SEGMENTE (PWM)	
begin
if rising_edge (clk) then
  if (seg_dauer < hell2) and segment=2 then seg2 <= '0'; else seg2 <= '1'; end if;	 
end if;
end process;

process (clk)--  HELLIGKEIT FÜR DIE EINZELNEN SEGMENTE (PWM)	
begin
if rising_edge (clk) then
  if (seg_dauer < hell3) and segment=3 then seg3 <= '0'; else seg3 <= '1'; end if;	 
end if;
end process;

----------UMWANDLUNG-VON-INTEGER-NACH-BCD------------------------------------------
--  2 Zähler , einer Zählt in INT, der andere in BCD
--  wenn der Eingang gleich dem int Zähler ist, wird der BCD Zähler ausgegeben

process (clk)

variable bcd_valid : std_logic;

begin

if rising_edge (clk) then
    
  bcd_cnt <= bcd_cnt + 1;
	 
	 if int_cnt = 10000 then--         Überlauf ? dann bei 0 Anfangen
	   bcd_cnt <= (others => '0');
	   int_cnt <= (others => '0');
	 end if;

--   ist die Aktuelle BCD Zahl gülig ?
	 if bcd_cnt (3 downto 0) < 10 and bcd_cnt (7 downto 4) < 10 and 
	   bcd_cnt (11 downto 8) < 10 and bcd_cnt (15 downto 12) < 10 then
	   int_cnt <= int_cnt + 1;
		bcd_valid := '1'; else bcd_valid := '0';
	 end if;
	 
	 if (int_hi & int_lo) = int_cnt and bcd_valid = '1' then 
--	 bcd_out <= bcd_cnt;--diagnose
	 bcd_gewandelt <= bcd_cnt;
	 end if;
	 
	 
end if;
end process;
------------------ZIFFER-AUSSUCHEN--------------------------------------------------
    with segment select--  Dezimale Ziffer
	 bcd_int <= bcd_gewandelt (15 downto 12) when "00",
	            bcd_gewandelt (11 downto  8) when "01",
	            bcd_gewandelt ( 7 downto  4) when "10",
	            bcd_gewandelt ( 3 downto  0) when others;

    with segment select-- Hexadezimale Ziffer
	 bcd_ext <= int_hi (7 downto 4) when "00",
	            int_hi (3 downto 0) when "01",
	            int_lo (7 downto 4) when "10",
	            int_lo (3 downto 0) when others;
									
									
    with segment select-- Bitmuster
	  bin  <=   bin0 when "00",
	            bin1 when "01",
	            bin2 when "10",
	            bin3 when others;
					
					
									
----------------------------UMWANDLUNG-BCD-7SEGMENT--------------------------------
   
    with bcd select
   seven_seg <= "1111001" when "0001",   --1
                "0100100" when "0010",   --2
                "0110000" when "0011",   --3
                "0011001" when "0100",   --4
                "0010010" when "0101",   --5
                "0000010" when "0110",   --6
                "1111000" when "0111",   --7
                "0000000" when "1000",   --8
                "0010000" when "1001",   --9
                "0001000" when "1010",   --A
                "0000011" when "1011",   --b
                "1000110" when "1100",   --C
                "0100001" when "1101",   --d
                "0000110" when "1110",   --E
                "0001110" when "1111",   --F
                "1000000" when others;   --0
					 
-------------------------------PUNKTE-IM-7-SEGMENT-DISPLAY---------------------------
process (clk) begin
if rising_edge (clk) then
 if (segment=3 and punkte(0) = '1') or 
    (segment=2 and punkte(1) = '1') or
    (segment=1 and punkte(2) = '1') or
    (segment=0 and punkte(3) = '1') then
	 punkt <= '0';
  else
    punkt <= '1';
  end if;
end if;  
end process;
------------------------------------AUSGABE-----------------------------------------
  
process (clk)
begin
if rising_edge (clk) then
  if binaer = '1' then--         Directe binäre Ansteuerung der Leucht-segmente
  display <= seg3 & seg2 & seg1 & seg0 & not bin;
  else--                         Ausgabe von Hex oder Int Ziffer
  display <= seg3 & seg2 & seg1 & seg0 & punkt & seven_seg  ;
  end if;
end if;
end process;

process (clk)
begin
if rising_edge (clk) then
  if hex = '0' then bcd <= bcd_int; else bcd <= bcd_ext; end if;
end if;
end process;

end Behavioral;
