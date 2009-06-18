--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
-- Hackerspace:		Das Labor    www.das-labor.org											--
-- Hacker: 				Sauron																			--
-- 																											--
-- Datum:    			09.06.2009        															--
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
-- Dependencies: 		sinus.xco                                       					--
--																												--
-- Version:          V9.6.1       																	--
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
             x_pos : out  STD_LOGIC_VECTOR ( 8 downto 0);
             y_pos : out  STD_LOGIC_VECTOR ( 8 downto 0);
          winkel_v : out  STD_LOGIC_VECTOR ( 9 downto 0);
             led_v : out  STD_LOGIC_VECTOR ( 7 downto 0);
				   rdy : out  STD_LOGIC
--			 sine_8_d : out  std_logic_VECTOR ( 8 downto 0);
--			  led_b_d : out  STD_LOGIC_VECTOR ( 7 downto 0);
--			x_pos_8_d : out  std_logic_VECTOR ( 8 downto 0);
--        x_id,y_id: out  STD_LOGIC_VECTOR ( 8 downto 0)
				 );
end xy_polar;

architecture Behavioral of xy_polar is

signal sine_8,cosine_8    :std_logic_VECTOR( 8 downto 0);
signal x_pos_8,y_pos_8    :std_logic_VECTOR( 8 downto 0);
signal mem_adr_a          :std_logic_VECTOR(17 downto 0);
signal x_i,y_i            :std_logic_VECTOR( 8 downto 0);
signal winkel_1,winkel_2  :std_logic_VECTOR( 9 downto 0);
signal led_1,led_2        :std_logic_VECTOR( 7 downto 0);
signal winkel_3,winkel_4  :std_logic_VECTOR( 9 downto 0);
signal led_3,led_4        :std_logic_VECTOR( 7 downto 0);
signal led_a              :std_logic_VECTOR( 7 downto 0);

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
port map
(
       THETA => winkel,
		   clk => clk,
        SINE => sine_8,
      COSINE => cosine_8
);

-- Ergebnisse des sinus/cosinus haben eine Latenze von 1 clk, da eingang registered ist
-- Deshalb muss LED auch um 1 clk verzögert werden, der Ausgang ist nicht registered, da
-- der nachfolgende eingang registered ist

process (clk) begin  -- #23#
  if rising_edge (clk) then 
    led_a <= led;   
  end if; 
end process;  
 
x : multiplier_xy
port map -- #35#
(
			clk => clk,
			a => led_a,
			b => sine_8,
			p => x_pos_8
);



y : multiplier_xy
port map -- #35#
(
			clk => clk,
			a => led_a,
			b => cosine_8,
			p => y_pos_8
);

-- Jeder Multiplier hat eine Latenz von 2



x_i <= x_pos_8 + 256;-- Koordinatenursprung verlegen
y_i <= y_pos_8 + 256;

mem_adr_a <= y_i & x_i;-- y_pos * 512 (durch verschieben) + x_pos

-- Ergebniss der Speicherberechnung bis zum Nächsten Takt verzögern
-- (Pipeline Stufe 4)

process (clk) begin -- #56#
  if rising_edge (clk) then
    mem_adr <= mem_adr_a;
  end if;
end process; 
	 
process (clk) begin -- #56#
  if rising_edge (clk) then
      x_pos <= x_i;
      y_pos <= y_i;
  end if;
end process; 
  
-- Winkel um 4 Takte verzögert ausgeben
process (clk) begin  if rising_edge (clk) then winkel_1 <= winkel;   end if; end process;  
process (clk) begin  if rising_edge (clk) then winkel_2 <= winkel_1; end if; end process;  
process (clk) begin  if rising_edge (clk) then winkel_3 <= winkel_2; end if; end process;
process (clk) begin  if rising_edge (clk) then winkel_4 <= winkel_3; end if; end process;
  

 

-- LED um 4 Takte verzögert ausgeben
process (clk) begin  if rising_edge (clk) then led_1 <= led;   end if; end process;  
process (clk) begin  if rising_edge (clk) then led_2 <= led_1; end if; end process;  
process (clk) begin  if rising_edge (clk) then led_3 <= led_2; end if; end process;  
process (clk) begin  if rising_edge (clk) then led_4 <= led_3; end if; end process;  

led_v <= led_4;
winkel_v <= winkel_4;

process (led,led_4,winkel,winkel_4) begin
  if (led=led_4) and (winkel=winkel_4) 
    then
  rdy <= '1';
    else
  rdy <= '0';	 
  end if;
end process;

-- diagnose
--sine_8_d<=sine_8;
--x_pos_8_d<=x_pos_8;
--led_b_d<=led_a;
--x_id <= x_i;
--y_id <= y_i;

end Behavioral;

