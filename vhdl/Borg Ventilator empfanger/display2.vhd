library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity display2 is
    Port ( clk,hex,binaer    : in STD_LOGIC;
           bcd01  : in  STD_LOGIC_VECTOR (7 downto 0);
           bcd23  : in  STD_LOGIC_VECTOR (7 downto 0);
           int_hi : in  STD_LOGIC_VECTOR (7 downto 0);
           int_lo : in  STD_LOGIC_VECTOR (7 downto 0);
           bin0  : in  STD_LOGIC_VECTOR (7 downto 0);
           bin1  : in  STD_LOGIC_VECTOR (7 downto 0);
           bin2  : in  STD_LOGIC_VECTOR (7 downto 0);
           bin3  : in  STD_LOGIC_VECTOR (7 downto 0);
           hell0 : in  STD_LOGIC_VECTOR (7 downto 0);
           hell1 : in  STD_LOGIC_VECTOR (7 downto 0);
           hell2 : in  STD_LOGIC_VECTOR (7 downto 0);
           hell3 : in  STD_LOGIC_VECTOR (7 downto 0);
			  Display : out  STD_LOGIC_VECTOR (11 downto 0);
			  seg_dauer_diag : out STD_LOGIC_VECTOR (16 downto 0);
			  seg0_diag,seg1_diag,seg2_diag,seg3_diag : out STD_LOGIC;
			  segment_diag : out  STD_LOGIC_VECTOR (1 downto 0);
			  int_cnt_diag : out  STD_LOGIC_VECTOR (15 downto 0);
			  bcd_cnt_diag : out  STD_LOGIC_VECTOR (15 downto 0);
--			  int_in       : in   STD_LOGIC_VECTOR (15 downto 0);
			  bcd_out      : out  STD_LOGIC_VECTOR (15 downto 0));
			  
end display2;

architecture Behavioral of display2 is

signal segment  : STD_LOGIC_VECTOR (1 downto 0):="00";
signal seg_dauer: STD_LOGIC_VECTOR (16 downto 0):="00000000000000000";
signal seg0, seg1, seg2, seg3 , res_ib: std_logic;
signal int_cnt,bcd_cnt: STD_LOGIC_VECTOR (15 downto 0):=(others => '0');
signal bcd,bcd_int,bcd_ext: STD_LOGIC_VECTOR (3 downto 0);
signal seven_seg: STD_LOGIC_VECTOR (6 downto 0);
signal bin: STD_LOGIC_VECTOR (7 downto 0);




begin
-----------MULTIPLEXER-FÜR-WECHSEL-DER-STELLE----------------------------------------------------
process (clk)
begin
if rising_edge (clk) then
  seg_dauer <= seg_dauer+1;
  if seg_dauer=0 then 
    segment <= segment +1;
  end if;	 
end if;
end process;


process (clk)--  HELLIGKEIT FÜR DIE EINZELNEN SEGMENTE (PWM)	
begin
if rising_edge (clk) then
  if (seg_dauer (16 downto 9) < hell0) and segment=0 then seg0 <= '0'; else seg0 <= '1'; end if;	 
end if;

if rising_edge (clk) then
  if (seg_dauer (16 downto 9) < hell1) and segment=1 then seg1 <= '0'; else seg1 <= '1'; end if;	 
end if;

if rising_edge (clk) then
  if (seg_dauer (16 downto 9) < hell2) and segment=2 then seg2 <= '0'; else seg2 <= '1'; end if;	 
end if;

if rising_edge (clk) then
  if (seg_dauer (16 downto 9) < hell3) and segment=3 then seg3 <= '0'; else seg3 <= '1'; end if;	 
end if;
end process;

----------UMWANDLUNG-VON-INTEGER-NACH-BCD------------------------------------------
--  2 Zähler , einer Zählt in INT, der andere in BCD
--  wenn der Eingang gleich dem int Zähler ist, wird der BCD Zähler ausgegeben
--
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
	 bcd_out <= bcd_cnt;
	 end if;
	 
	 
end if;
end process;
--------------------------------------------------------------------------------
    with segment select
	 bcd_int <= bcd_cnt (15 downto 12) when "11",
	            bcd_cnt (11 downto  8) when "10",
	            bcd_cnt ( 7 downto  4) when "01",
	            bcd_cnt ( 3 downto  0) when "00",
	                            "0000" when others;

    with segment select
	 bcd_ext <= bcd01 (7 downto 4) when "00",
	            bcd01 (3 downto 0) when "01",
	            bcd23 (7 downto 4) when "10",
	            bcd23 (3 downto 0) when "11",
	                        "0000" when others;
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
----------------------------------------------------------
--					 
--					 
---------------------------------AUSGABE----------------------------------

process (clk)
begin
if rising_edge (clk) then
  if binaer = '1' then
  display <= bin & seg0 & seg1 & seg2 & seg3;
  else
  display <= seven_seg & '0' & seg0 & seg1 & seg2 & seg3;
  end if;
end if;
end process;

process (clk)
begin
if rising_edge (clk) then
  if hex = '0' then bcd <= bcd_int; else bcd <= bcd_ext; end if;
end if;
end process;

-------------------------------DIAGNOSE----------------------------------
segment_diag <= segment;
seg_dauer_diag <= seg_dauer;				
seg0_diag <= seg0;
seg1_diag <= seg1;
seg2_diag <= seg2;
seg3_diag <= seg3;
bcd_cnt_diag <= bcd_cnt;
int_cnt_diag <= int_cnt;

end Behavioral;
