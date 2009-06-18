library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity memory_control is
    Port (       clk : in    STD_LOGIC;
	           clk100 : in    STD_LOGIC;
	         sram_adr : out   STD_LOGIC_VECTOR (17 downto 0);
           sram_1_io : inout STD_LOGIC_VECTOR (15 downto 0) :="0000000000000000";
           sram_2_io : inout STD_LOGIC_VECTOR (15 downto 0) :="0000000000000000";
             sram_oe : out   STD_LOGIC;
             sram_we : out   STD_LOGIC := '0';
           sram_1_ce : out   STD_LOGIC := '0';
           sram_1_ub : out   STD_LOGIC;
           sram_1_lb : out   STD_LOGIC;
           sram_2_ce : out   STD_LOGIC := '0';
           sram_2_ub : out   STD_LOGIC;
           sram_2_lb : out   STD_LOGIC;
			     ad_adr : in    STD_LOGIC_VECTOR (17 downto 0);
              ad_dat : in    STD_LOGIC_VECTOR (15 downto 0);
				   ad_wr : in    STD_LOGIC;
				 out_adr : in    STD_LOGIC_VECTOR (17 downto 0);
             out_dat : out   STD_LOGIC_VECTOR (15 downto 0);
				  ad_end : in    STD_LOGIC;
				  ad_new : in    STD_LOGIC;
             out_new : out   STD_LOGIC;
             out_rdy : out	  STD_LOGIC;	
              winkel : in    STD_LOGIC_VECTOR ( 9 downto 0);				 
			 data_count : out   STD_LOGIC_VECTOR ( 5 downto 0);
	   	   led_nr_d : out   STD_LOGIC_VECTOR ( 7 downto 0);
          led_nr_v_d : out   STD_LOGIC_VECTOR ( 7 downto 0);
           mem_adr_d : out   STD_LOGIC_VECTOR (17 downto 0);
         write_mem_d : out   STD_LOGIC;
    fluegel_winkel_d : out   STD_LOGIC_VECTOR ( 9 downto 0);			 
        led_winkel_d : out   STD_LOGIC_VECTOR ( 9 downto 0);
     x_pos_d,y_pos_d : out   STD_LOGIC_VECTOR ( 8 downto 0);	
              dout_d	: out   STD_LOGIC_VECTOR (33 downto 0);
        polar_data_d	: out   STD_LOGIC_VECTOR (15 downto 0):="0000000000000000"

 
			  );	  
				  
end memory_control;

architecture Behavioral of memory_control is

signal din,dout      : std_logic_VECTOR(33 downto 0);
signal rd_en         : std_logic;
signal prog_full     : std_logic;
signal prog_empty    : std_logic;
signal write_mem_2   : std_logic := '0';
signal write_mem_3   : std_logic;
signal write_mem_4   : std_logic;
signal write_mem_5   : std_logic;
signal write_mem_6   : std_logic;
signal write_mem_7   : std_logic;
signal sram_we_i     : std_logic := '0';
signal help_we_i     : std_logic := '0';
signal chip_1        : std_logic := '0';
signal chip_2        : std_logic;
signal chip_3        : std_logic;
signal chip_4        : std_logic;
signal chip_5        : std_logic;
signal chip_6        : std_logic;
signal led_nr_1      : std_logic_vector( 7 downto 0) := "00000000";
signal led_nr_2      : std_logic_vector( 7 downto 0) := "00000000";
signal led_winkel    : std_logic_vector( 9 downto 0) := "0000000000";
signal fluegel_winkel: std_logic_vector( 9 downto 0) := "0000000000";
signal mem_adr       : std_logic_vector(17 downto 0);
signal led_nr_v      : std_logic_vector( 7 downto 0);
signal x_pos         : std_logic_vector( 8 downto 0);
signal y_pos         : std_logic_vector( 8 downto 0);
signal ad_wrt        : std_logic;
signal polar_data    : std_logic_vector(15 downto 0);



--signal sram_adr_en   : std_logic := '1';
--signal sram_we_cnt   : std_logic_vector( 7 downto 0):="00000100";

component fifo
	port(
		clk       : IN  std_logic;
		din       : IN  std_logic_VECTOR(33 downto 0);
		rd_en     : IN  std_logic;
		wr_en     : IN  std_logic;
		data_count: OUT std_logic_VECTOR( 5 downto 0);
		dout      : OUT std_logic_VECTOR(33 downto 0);
		empty     : OUT std_logic;
		full      : OUT std_logic;
		prog_empty: OUT std_logic;
		prog_full : OUT std_logic
		);
end component;

component xy_polar
	port(
		clk      : IN  std_logic;
		winkel   : IN  std_logic_vector( 9 downto 0);
		led      : IN  std_logic_vector( 7 downto 0);          
		mem_adr  : OUT std_logic_vector(17 downto 0);
		x_pos    : OUT std_logic_vector( 8 downto 0);
		y_pos    : OUT std_logic_vector( 8 downto 0);
		winkel_v : OUT std_logic_vector( 9 downto 0);
		led_v    : OUT std_logic_vector( 7 downto 0);
		rdy      : OUT std_logic
		);
end component;

begin

sram_1_ub <= '1'; -- sram upper und lower byte enable
sram_1_lb <= '1';
sram_2_ub <= '1';
sram_2_lb <= '1';
sram_oe   <= '1'; -- Sram output enable




--Dateneingang mit fifo puffern

process (clk) begin
  if rising_edge (clk) then
    din <= ad_adr & ad_dat; -- speicheradresse und daten zusammenfassen
	 ad_wrt<= ad_wr; -- wird ebenfalls gepuffert um synchron zu ad_adr/ad_dat zu bleiben
  end if;
end process;
  
U0 : fifo
		port map (
			clk => clk,
			din => din,
			rd_en => write_mem_5,
			wr_en => ad_wrt,
	      data_count => data_count,
      	dout => dout,
			prog_empty => prog_empty,
			prog_full => prog_full);



-- Umschaltung zwischen lesen und schreiben

process (clk) begin -- #12#
  if rising_edge (clk) then
    if prog_full = '1' then
	     write_mem_2 <= '1';
	 elsif prog_empty = '1' then
        write_mem_2 <= '0';
	 end if;	  
  end if;
end process;


-- write_mem um weitere 4 takte verzögern
process (clk) begin if rising_edge (clk) then write_mem_3 <= write_mem_2;end if;end process;
process (clk) begin if rising_edge (clk) then write_mem_4 <= write_mem_3;end if;end process;
process (clk) begin if rising_edge (clk) then write_mem_5 <= write_mem_4;end if;end process;
process (clk) begin if rising_edge (clk) then write_mem_6 <= write_mem_5;end if;end process;
process (clk) begin if rising_edge (clk) then write_mem_7 <= write_mem_6;end if;end process;


-- Umschaltung zwischen Bildspeicher 1 und 2

process (clk) begin --#U1#
  if rising_edge (clk) then
    if ad_new = '1' then
	   if chip_1 = '0' then
		  chip_1 <= '1';
		else
        chip_1 <= '0';
      end if;
    end if;
  end if;
end process;

process (clk) begin if rising_edge (clk) then chip_2 <= chip_1;end if;end process;
process (clk) begin if rising_edge (clk) then chip_3 <= chip_2;end if;end process;
process (clk) begin if rising_edge (clk) then chip_4 <= chip_3;end if;end process;
process (clk) begin if rising_edge (clk) then chip_5 <= chip_4;end if;end process;
process (clk) begin if rising_edge (clk) then chip_6 <= chip_5;end if;end process;




----------------------------------------------------
--  Adresse für Leseoperation aus SRAM bestimmen  --
----------------------------------------------------

-- winkel in ein Register übernehmen,
-- damit alle leds für den selben winkel berechnet werden
process (clk) begin -- #u1#
 if rising_edge (clk) and led_nr_1 = 0 then
   fluegel_winkel <= winkel;
  end if;
end process;
  
-- leds von innen nach aussen durchgehen und den 90 grad
-- versatz zueinander beachten	
process (clk) --#12# 
variable write_mem_1:std_logic; 

begin
  if rising_edge (clk) then
    if prog_full = '1' then
	     write_mem_1 := '1';
	 elsif prog_empty = '1' then
        write_mem_1 := '0';
	 end if;	 
    
	 if write_mem_1 = '0' then
	       led_nr_1 <= led_nr_1 + 1;
    end if;
	 
    if write_mem_2 = '0' then	 
	   if    led_nr_1(1 downto 0) = "00" then
		  led_winkel <= fluegel_winkel;
		elsif led_nr_1(1 downto 0) = "01" then  
        led_winkel <= fluegel_winkel + 256;
		elsif led_nr_1(1 downto 0) = "10" then  
        led_winkel <= fluegel_winkel + 512;
		else  
        led_winkel <= fluegel_winkel + 768;
      end if;
	 end if;	
  end if;
end process;

process (clk) begin if rising_edge (clk) then led_nr_2 <= led_nr_1;end if;end process;


-- winkel und led_nr in eine Speicheradresse umrechnen
-- xy_polar hat allerdings eine latenz von 4 takten

	U1: xy_polar PORT MAP(
		clk => clk,
		winkel => led_winkel,
		led => led_nr_2,
		mem_adr => mem_adr,
		x_pos => x_pos,
		y_pos => y_pos,
--		winkel_v => ,
		led_v => led_nr_v
--		rdy => 
	);


----------------------------------------------------
--------  Zugriff auf das SRAM  --------------------
----------------------------------------------------


process (clk) begin  -- #67#
  if rising_edge (clk) then
--                          -----  Daten aus SRAM lesen  -----
    if write_mem_6 = '0' then                                        
        rd_en <= '0';        -- nicht aus fifo lesen
--        sram_we <= '0';      -- nicht in sram schreiben
	     sram_adr <= mem_adr; -- leseadresse von modul xy_polar
      if chip_6 = '0' then   --- Lesen aus chip 1                  
        sram_1_ce <= '1';    -- chip 1 enable                      
        sram_2_ce <= '0';    -- chip 2 disable
--        polar_data <= sram_1_io;      -- daten ausgeben
        sram_2_io <= (others => '0'); -- ist ja disabled 
	   else		            --- lesen aus chip 2
        sram_1_ce <= '0';   -- chip 1 disable
        sram_2_ce <= '1';   -- chip 2 enable
        sram_1_io <= (others => '0');  -- ist ja disabled
--        polar_data <= sram_2_io;       -- daten ausgeben
      end if;		 
--                          -----  Daten ins SRAM schreiben ----
    else
 	     rd_en <= '1';        -- lesen aus fifo  
--        sram_we <= '1';      -- schreiben in sram
	     sram_adr <= dout (33 downto 16); -- in fifo liegen nebeneinander adresse und daten
      if chip_6 = '0' then
        sram_1_ce <= '1';
        sram_2_ce <= '0';
--        sram_1_io <= dout (15 downto 0); -- fifo daten ins sram chip 1 schieben
--        sram_2_io <= (others => '0');
  	   else		
        sram_1_ce <= '0';
        sram_2_ce <= '1';
--        sram_1_io <= (others => '0'); 
--        sram_2_io <= dout (15 downto 0); -- fifo daten ins sram chip 2 schieben
      end if;	-- chip select	 
	 end if;  -- lesen oder schreiben 
  end if;  -- keine flanke
end process;

-- impuls zum  schreiben erzeugen
process (clk100) begin
  if rising_edge (clk100) then
    if write_mem_7 = '1' then
	   if sram_we_i='0' then sram_we_i <= '1';
      else sram_we_i <= '0';
		end if;
    else sram_we_i <= '0';	
    end if;	 
  end if;
end process;  

sram_we<= sram_we_i;

-- Daten von SRAM um 10 ns verzögert einlesen
process (clk100) begin
  if rising_edge (clk100) then
    if write_mem_7 = '0' then
	   if help_we_i = '1' then-- jetzt lesen
        if chip_6 = '0' then
          polar_data <= sram_1_io;  
		  else
          polar_data <= sram_2_io;
		  end if;	 
		    if help_we_i='0' then help_we_i <= '1';-- einen merker setzern
          else help_we_i <= '0';          -- ob wir in der 1. hälfte des clk sind, oder nicht
		    end if;
      end if;
	 else help_we_i <= '0';	
    end if; -- nicht lesen	 
  end if;  --keine flanke
end process;  

--diagnose
led_nr_d<=led_nr_2;
led_nr_v_d<=led_nr_v;
mem_adr_d<=mem_adr;
write_mem_d<=write_mem_6;
fluegel_winkel_d<=fluegel_winkel;
led_winkel_d<=led_winkel;
x_pos_d<=x_pos;
y_pos_d<=y_pos;
dout_d <= dout;
polar_data_d <= polar_data;  

  
end Behavioral;