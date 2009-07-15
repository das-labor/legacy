--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
-- Hackerspace:		Das Labor    www.das-labor.org											--
-- Hacker: 				Sauron																			--
-- 																											--
-- Datum:    			27.06.2009        															--
-- Projekt: 			Der Borg Ventilator															--
-- Modul Name:     	xy_polar.vhd     																--
-- Beschreibung: 		Umrechnung polar nach xy + speicheradresse                     --
--                   Ich berechne anhand des winkels zuerst den Sin/cos(-256 - +255)--
--                   Danach Skaliere ich mit Multiplikation / Division auf den      --
--							passenden Abstand vom Mittelpunkt                              --
--                   Anschliessend wird der Koordinaten-nullpunkt nach Links-unten  --
--                   verschoben (0-511)                                             --
--							Danach wird mit den xy-koordinaten die Adresse im Speicher     --
--                   bestimmt                                                       --
-- Pipelining:       | sin/cos 2 | multiplikation | Addressberechnung |             --
-- Latenz:           4 clk                                                          --
--																												--
-- Dependencies: 		sinus.xco, multiplier_xy.xco                      					--
--																												--
-- Version:          V9.6.2       																	--
-- 																											--
-- Additional Comments: Wiederstand ist Zwecklos		         							--
--																												--
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

library UNISIM;
use UNISIM.VComponents.all;

entity xy_polar is
    Port (     clk : in   STD_LOGIC;
            winkel : in   STD_LOGIC_VECTOR ( 9 downto 0);
               led : in   STD_LOGIC_VECTOR ( 7 downto 0);
           mem_adr : out  STD_LOGIC_VECTOR (17 downto 0);
--          winkel_v : out  STD_LOGIC_VECTOR ( 9 downto 0);
             led_v : out  STD_LOGIC_VECTOR ( 7 downto 0)
			
				 );
end xy_polar;

architecture Behavioral of xy_polar is

signal sine_3,cosine_3    :std_logic_VECTOR( 8 downto 0);
signal x_signed_5         :std_logic_VECTOR( 8 downto 0);
signal y_signed_5         :std_logic_VECTOR( 8 downto 0);
signal x_5,y_5            :std_logic_VECTOR( 8 downto 0);
--signal winkel_3,winkel_4  :std_logic_VECTOR( 9 downto 0);
--signal winkel_5           :std_logic_VECTOR( 9 downto 0);
signal led_3,led_4        :std_logic_VECTOR( 7 downto 0);
signal led_5              :std_logic_VECTOR( 7 downto 0);

component sinus
	port (
	 THETA: IN  std_logic_VECTOR(9 downto 0);
	   clk: IN  std_logic;
	  SINE: OUT std_logic_VECTOR(8 downto 0);
	COSINE: OUT std_logic_VECTOR(8 downto 0));
end component;

attribute box_type : string; 
attribute box_type of sinus : component is "black_box";

component multiplier_xy
	port (
	clk: IN  std_logic;
	  a: IN  std_logic_VECTOR(7 downto 0);
	  b: IN  std_logic_VECTOR(8 downto 0);
  	  p: OUT std_logic_VECTOR(8 downto 0));
end component;

attribute box_type of multiplier_xy : component is "black_box";

begin

sin0:sinus--   Sinus und Cosinus funktion einbinden
port map --#23#
(
       THETA => winkel,
		   clk => clk,
        SINE => sine_3,
      COSINE => cosine_3
);
-- (co)sinus sind im bereich -512 bis +511
-- Ergebnisse des sinus/cosinus haben eine Latenze von 1 clk, da eingang registered ist
-- Deshalb muss LED auch um 1 clk verzögert werden, der Ausgang ist nicht registered, da
-- der nachfolgende eingang registered ist


x : multiplier_xy
port map -- #35#
(
			clk => clk,
			a => led_3,
			b => sine_3,
			p => x_signed_5
);


y : multiplier_xy
port map -- #35#
(
			clk => clk,
			a => led_3,
			b => cosine_3,
			p => y_signed_5
);

-- (co)sinus * Led (0-255) / 256  (Division durch links schift)

-- Jeder Multiplier hat eine Latenz von 2 takten


x_5 <= x_signed_5 + 256;-- Typkonvertierung von Signed nach unsigned
y_5 <= y_signed_5 + 256;


-- Ergebniss der Speicherberechnung bis zum Nächsten Takt verzögern
-- (Pipeline Stufe 4)

process (clk) begin -- #56#
  if rising_edge (clk) then
    mem_adr <= y_5 & x_5;
	   end if;
end process; 
	 
  
-- Winkel um 4 Takte verzögert Ausgeben
--process (clk) begin  if rising_edge (clk) then winkel_3 <= winkel;   end if; end process;  
--process (clk) begin  if rising_edge (clk) then winkel_4 <= winkel_3; end if; end process;  
--process (clk) begin  if rising_edge (clk) then winkel_5 <= winkel_4; end if; end process;
--process (clk) begin  if rising_edge (clk) then winkel_v <= winkel_5; end if; end process;
  

-- LED um 4 Takte verzögert Ausgeben
process (clk) begin  if rising_edge (clk) then led_3 <= led;  end if; end process;  
process (clk) begin  if rising_edge (clk) then led_4 <= led_3; end if; end process;  
process (clk) begin  if rising_edge (clk) then led_5 <= led_4; end if; end process;  
process (clk) begin  if rising_edge (clk) then led_v <= led_5; end if; end process;  

end Behavioral;

