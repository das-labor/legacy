--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
-- Hackerspace:		Das Labor    www.das-labor.org											--
-- Hacker: 				Sauron																			--
-- 																											--
-- Datum:    			28.06.2009        															--
-- Projekt: 			Der Borg Ventilator															--
-- Modul Name:     	ram_write.vhd     															--
-- Beschreibung: 		Daten in den Arbeitsspeicher schieben                          --
--                   Ich nehmen Daten entgegen entweder von einer CPU oder          --
--                   von einem AD wandler. Anschliessend geht es in einen FIFO      --
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


entity ram_write is
    Port (       clk : in    STD_LOGIC;

	         sram_adr : out   STD_LOGIC_VECTOR (17 downto 0);
            sram_dat : out   STD_LOGIC_VECTOR (15 downto 0) :="0000000000000000";
            sram_wrt : out   STD_LOGIC := '0';
            sram_sel : out   STD_LOGIC := '0';
			  
	           ad_adr : in    STD_LOGIC_VECTOR (17 downto 0);
              ad_dat : in    STD_LOGIC_VECTOR (15 downto 0);
	            ad_wr : in    STD_LOGIC;

           cpu_dat_x : in    STD_LOGIC_VECTOR ( 7 downto 0);
           cpu_dat_y : in    STD_LOGIC_VECTOR ( 7 downto 0);
       cpu_adr_hi_lo : in    STD_LOGIC_VECTOR ( 7 downto 0);
		 cpu_adr_hi_hi : in    STD_LOGIC;
       cpu_adr_lo_lo : in    STD_LOGIC_VECTOR ( 7 downto 0);
		 cpu_adr_lo_hi : in    STD_LOGIC;	 
           cpu_write : in    STD_LOGIC;			 
          cpu_enable : in    STD_LOGIC:='0'
			 
			  );	  
end ram_write;

architecture Behavioral of ram_write is

signal cpu_write_last     : std_logic;
--signale für die pipeline stufe 1
signal fifo_dat_1         : std_logic_vector (15 downto 0);
signal fifo_adr_1         : std_logic_vector (17 downto 0);
signal fifo_wr_1          : std_logic;
--signale für die pipeline stufe 2
signal din_2              : std_logic_vector (33 downto 0);
signal fifo_wr_2          : std_logic;
--signale für die pipeline stufe 3
signal prog_full_3        : std_logic;
signal prog_empty_3       : std_logic;
--signale für die pipeline stufe 4
signal writemem_4         : std_logic;
--signale für die pipeline stufe 5
signal dout_5             : std_logic_vector (33 downto 0);
signal writemem_5         : std_logic;
--signale für die pipeline stufe 6
signal chip_6             : std_logic :='0';

component fifo
	port(
		clk       : IN  std_logic;
		din       : IN  std_logic_VECTOR(33 downto 0);
		rd_en     : IN  std_logic;
		wr_en     : IN  std_logic;
		dout      : OUT std_logic_VECTOR(33 downto 0);
		empty     : OUT std_logic;
		full      : OUT std_logic;
		prog_empty: OUT std_logic;
		prog_full : OUT std_logic
		);
end component;

attribute box_type : string; 
attribute box_type of fifo : component is "black_box";


begin ----------   und los gehts --------------

-- Datenquelle für den Fifo auswählen (cpu oder ad-wandler)

process (clk) begin-- #U1#
  if rising_edge (clk) then
    if cpu_enable = '1' then--  fifo wird von cpu gefüttert
       fifo_dat_1 <= cpu_dat_y  & cpu_dat_x;
       fifo_adr_1 <= cpu_adr_hi_hi &  cpu_adr_hi_lo &  cpu_adr_lo_hi &  cpu_adr_lo_lo;
       fifo_wr_1  <= (not cpu_write_last) and cpu_write; -- Flankenerkennung
    else -- fifo wird von ad_wandler gefüttert
       fifo_dat_1 <= ad_dat;	 
		 fifo_adr_1 <= ad_adr;
		 fifo_wr_1  <= ad_wr;
    end if;
       cpu_write_last	 <= cpu_write;-- für die Flankenerkennung
  end if;
end process;


--Dateneingang mit fifo puffern

process (clk) begin-- #12#
  if rising_edge (clk) then
    din_2 <= fifo_adr_1 & fifo_dat_1; -- speicheradresse und daten zusammenfassen
	 fifo_wr_2<= fifo_wr_1; -- wird ebenfalls gepuffert um synchron zu ad_adr/ad_dat zu bleiben
  end if;
end process;
  
U0 : fifo
		port map (
			clk => clk,
			din => din_2,
			rd_en => writemem_4,
			wr_en => fifo_wr_2,
      	dout => dout_5,
			prog_empty => prog_empty_3,
			prog_full => prog_full_3);

-- Umschaltung zwischen lesen und schreiben

process (clk) begin -- #34#
  if rising_edge (clk) then
    if prog_full_3 = '1' then
	     writemem_4 <= '1';
	 elsif prog_empty_3 = '1' then
        writemem_4 <= '0';
	 end if;	  
  end if;
end process;

process (clk) begin -- #45#
  if rising_edge (clk) then
    writemem_5 <= writemem_4;
  end if;
end process;


process (clk) begin -- #56#
  if rising_edge (clk) then
    if writemem_5 = '1' then-- bei einem schreibvorgang adresse und daten ausgeben
        sram_adr <= dout_5 (33 downto 16);
	     sram_dat <= dout_5 (15 downto  0);
        sram_wrt <= '1';
	 else -- es gibt nix zu schreiben, also nix ausgeben.
        sram_adr <= (others => '0');	 
	     sram_dat <= (others => '0');
		  sram_wrt <= '0';
	 end if;
-- bei dout = 1111... ist das aktuelle Bild komplett, und es wird ab sofort 
-- in den anderen Speicher chip geschrieben.		  
    if dout_5 = "0000000000000000000000000000000000"  and writemem_5 = '1' then
		if chip_6 = '0' then
			chip_6 <= '1';
		else
			chip_6 <= '0';
      end if;
    end if;
  end if;
end process;

-- chip select ausgeben
sram_sel <= chip_6;

end Behavioral;
