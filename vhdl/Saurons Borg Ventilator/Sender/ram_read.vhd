--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
-- Hackerspace:		Das Labor    www.das-labor.org											--
-- Hacker: 				Sauron																			--
-- 																											--
-- Datum:    			30.06.2009        															--
-- Projekt: 			Der Borg Ventilator															--
-- Modul Name:     	ram_read.vhd     																--
-- Beschreibung: 		Ich berechne die Adresse zum lesen aus dem Arbeitsspeicher     --
--                   dann gehe ich alle LED's der reihe nach durch und berechne     --
--                   für die aktuelle led und den aktuellen winkel die Adresse      --
--							im Speicher. Die Leds sind zueinander im 90 grad winkel        --
--                   angeordnet. Eine neue Adresse wird nur generiert, wenn nicht   --
--                   geschrieben wird.                                              --
-- Pipelining:       																		         --
-- Latenz:           6 clk                                                          --
--																												--
-- Dependencies: 		xy_polar.vhd  (sinus.xco, multiplier_xy.xco)      					--
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

entity ram_read is
    Port (       clk : in    STD_LOGIC;
	           winkel : in    STD_LOGIC_VECTOR( 9 downto 0);
		  write_enable : in    STD_LOGIC;
		       ram_adr :out    STD_LOGIC_vector(17 downto 0);
				  led_nr :out    STD_LOGIC_VECTOR( 7 downto 0);
			winkel_diag :out    STD_LOGIC_VECTOR( 9 downto 0));
end ram_read;

architecture Behavioral of ram_read is

component xy_polar
	port(
		clk      : IN  std_logic;
		winkel   : IN  std_logic_vector( 9 downto 0);
		led      : IN  std_logic_vector( 7 downto 0);          
		mem_adr  : OUT std_logic_vector(17 downto 0);
		led_v    : OUT std_logic_vector( 7 downto 0));
end component;
	-- stufe 1
	signal fluegel_winkel_1 : std_logic_vector( 9 downto 0):= (others => '0');
	signal led_nr_1         : std_logic_vector( 7 downto 0):= (others => '0');
	signal write_enable_1   : std_logic;
	--stufe 2
	signal led_nr_2			: std_logic_vector( 7 downto 0);
	signal led_winkel_2		: std_logic_vector( 9 downto 0);
	--stufe 6

begin -- und ab gehts
 
-- winkel in ein Register übernehmen,
-- damit alle leds für den selben winkel berechnet werden
process (clk) begin -- #u1#
 if rising_edge (clk) and led_nr_1 = 255 then
   fluegel_winkel_1 <= winkel;
  end if;
end process;

	winkel_diag      <= fluegel_winkel_1;

  
-- leds von innen nach aussen durchgehen und den 90 grad
-- versatz zueinander beachten	
process (clk) begin--#U1#
	If rising_edge (clk) then
		if write_enable = '0' then
			led_nr_1 <= led_nr_1 + 1;
		end if;
			write_enable_1 <= write_enable;
	end if;	
end process;	 

process (clk) begin-- #12#	 
if rising_edge (clk) then
	if write_enable_1 = '0' then	 
		if    led_nr_1(1 downto 0) = "00" then
				led_winkel_2 <= fluegel_winkel_1;
		elsif led_nr_1(1 downto 0) = "01" then  
				led_winkel_2 <= fluegel_winkel_1 + 256;
		elsif led_nr_1(1 downto 0) = "10" then  
				led_winkel_2 <= fluegel_winkel_1 + 512;
		else  
				led_winkel_2 <= fluegel_winkel_1 + 768;
		end if;
	end if;
				led_nr_2 <= led_nr_1;
  end if;
end process;

-- winkel und led_nr in eine Speicheradresse umrechnen
-- xy_polar hat allerdings eine latenz von 4 takten

	U1: xy_polar PORT MAP(--#26#
		   clk => clk,
		winkel => led_winkel_2,
		   led => led_nr_2,
	  mem_adr => ram_adr,
--	 winkel_v => winkel_diag,
   	 led_v => led_nr

		 );

end Behavioral;

