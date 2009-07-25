--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
-- Hackerspace:		Das Labor    www.das-labor.org											--
-- Hacker: 				Sauron																			--
-- 																											--
-- Datum:    			30.06.2009        															--
-- Projekt: 			Der Borg Ventilator															--
-- Modul Name:     	ram_write.vhd     															--
-- Beschreibung: 		Daten in den Arbeitsspeicher schieben                          --
--                   Ich nehmen Daten entgegen entweder von einer CPU oder          --
--                   von einem AD wandler. Anschliessend geht es in einen FIFO      --
--							wenn der fifo voll ist, wird in das ram geschrieben.           --
--                   wenn ein vollständiges bild geschrieben wurde, wird            --
--                   zum anderen speicher chip gewechselt                           --
-- Pipelining:                                                                      --
-- Latenz:           10 clk                                                          --
--																												--
-- Dependencies: 		fifo.xco                                        					--
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


entity ram_write is
    Port (       clk : in    STD_LOGIC;

	         sram_adr : out   STD_LOGIC_VECTOR (17 downto 0);
            sram_dat : out   STD_LOGIC_VECTOR (15 downto 0) :="0000000000000000";
            sram_wrt : out   STD_LOGIC := '0';
            sram_sel : out   STD_LOGIC := '0';
			write_enable: out   STD_LOGIC;
			totes_lesen : out   STD_logic := '0';
			  
	           ad_adr : in    STD_LOGIC_VECTOR (17 downto 0);
              ad_dat : in    STD_LOGIC_VECTOR (15 downto 0);
	            ad_wr : in    STD_LOGIC;

             cpu_rot : in    STD_LOGIC_VECTOR ( 7 downto 0);
            cpu_grun : in    STD_LOGIC_VECTOR ( 7 downto 0);
            cpu_blau : in    STD_LOGIC_VECTOR ( 7 downto 0);
            cpu_x_lo : in    STD_LOGIC_VECTOR ( 7 downto 0);
            cpu_x_hi : in    STD_LOGIC_VECTOR ( 7 downto 0);
            cpu_y_lo : in    STD_LOGIC_VECTOR ( 7 downto 0);
            cpu_y_hi	: in    STD_LOGIC_VECTOR ( 7 downto 0);

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
signal prog_empty_4       : std_logic;
--signale für die pipeline stufe 5
signal dout_5             : std_logic_vector (33 downto 0);
signal writemem_5         : std_logic;
--signale für die pipeline stufe 6
signal chip_6             : std_logic :='0';
signal sram_adr_6         : STD_LOGIC_VECTOR (17 downto 0);
signal sram_dat_6         : STD_LOGIC_VECTOR (15 downto 0);
signal sram_wrt_6         : STD_LOGIC ;
--signale für die pipeline stufe 7
signal sram_adr_7         : STD_LOGIC_VECTOR (17 downto 0);
signal sram_dat_7         : STD_LOGIC_VECTOR (15 downto 0);
signal sram_wrt_7         : STD_LOGIC ;
signal sram_sel_7         : STD_LOGIC ;
--signale für die pipeline stufe 8
signal sram_adr_8         : STD_LOGIC_VECTOR (17 downto 0);
signal sram_dat_8         : STD_LOGIC_VECTOR (15 downto 0);
signal sram_wrt_8         : STD_LOGIC ;
signal sram_sel_8         : STD_LOGIC ;
--signale für die pipeline stufe 9
signal sram_adr_9         : STD_LOGIC_VECTOR (17 downto 0);
signal sram_dat_9         : STD_LOGIC_VECTOR (15 downto 0);
signal sram_wrt_9         : STD_LOGIC ;
signal sram_sel_9         : STD_LOGIC ;
--signale für die pipeline stufe 10
signal sram_adr_10        : STD_LOGIC_VECTOR (17 downto 0);
signal sram_dat_10        : STD_LOGIC_VECTOR (15 downto 0);
signal sram_wrt_10        : STD_LOGIC ;
signal sram_sel_10        : STD_LOGIC ;

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
       fifo_dat_1 <= cpu_blau (7 downto 3) & cpu_grun (7 downto 2) & cpu_rot (7 downto 3);
       fifo_adr_1 <= cpu_y_hi(0) &  cpu_y_lo &  cpu_x_hi(0) &  cpu_x_lo;
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

-- Signal für ram_read generieren
process (clk) begin -- #34#
  if rising_edge (clk) then
    if prog_full_3 = '1' then
		  write_enable <= '1';
	 elsif prog_empty_4 = '1' then
		  write_enable <= '0';
	 end if;	  
  end if;
end process;

process (clk) begin -- #34#
  if rising_edge (clk) then
	 prog_empty_4 <= prog_empty_3;
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
        sram_adr_6 <= dout_5 (33 downto 16);
	     sram_dat_6 <= dout_5 (15 downto  0);
        sram_wrt_6 <= '1';
	 else -- es gibt nix zu schreiben, also nix ausgeben.
        sram_adr_6 <= (others => '0');	 
	     sram_dat_6 <= (others => '0');
		  sram_wrt_6 <= '0';
	 end if;
-- bei dout = 1111... ist das aktuelle Bild komplett, und es wird ab sofort 
-- in den anderen Speicher chip geschrieben.		  
    if dout_5(33 downto 16)="111111111111111111"  then
		if chip_6 = '0' then
			chip_6 <= '1';
		else
			chip_6 <= '0';
      end if;
    end if;
  end if;
end process;



--Zeitliches Synchronisieren mit Ram_read
process (clk) begin
	if rising_edge (clk) then
	sram_adr_7 <= sram_adr_6;	sram_dat_7 <= sram_dat_6;
	sram_wrt_7 <= sram_wrt_6;	sram_sel_7 <= chip_6;

	sram_adr_8 <= sram_adr_7;	sram_dat_8 <= sram_dat_7;
	sram_wrt_8 <= sram_wrt_7;	sram_sel_8 <= sram_sel_7;

	sram_adr_9 <= sram_adr_8;	sram_dat_9 <= sram_dat_8;
	sram_wrt_9 <= sram_wrt_8;	sram_sel_9 <= sram_sel_8;

	sram_adr_10 <= sram_adr_9;	sram_dat_10 <= sram_dat_9;
	sram_wrt_10 <= sram_wrt_9;	sram_sel_10 <= sram_sel_9;

	sram_adr <= sram_adr_10;	sram_dat <= sram_dat_10;
	sram_wrt <= sram_wrt_10;	sram_sel <= sram_sel_10;


	end if;
end process;


-- Totes Lesen (Dem SRAM_IO zeit geben, zum umschalten auf schreiben)
-- Dadurch entfällt ein Datenbus
process (clk) begin
   if rising_edge (clk) then
      if sram_wrt_9 = '1' and sram_wrt_10 = '0' 
		then totes_lesen <= '1';
		elsif sram_wrt_9 = '1' and sram_wrt_10 = '1'
		then totes_lesen <= '0';
		end if;
	end if;
end process;



end Behavioral;
