--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
-- Hackerspace:      Das Labor    www.das-labor.org                                 --
-- Hacker:           Sauron                                                         --
--                                                                                  --
-- Datum:            19.07.2009                                                     --
-- Projekt:          Der Borg Ventilator                                            --
-- Modul Name:       ram_control.vhd                                                --
-- Beschreibung: 	   Ich Koordiniere die Lese und Schreibzugriffe auf den           --
--                   Hauptspeicher, gespeichert werden 2 Vollständige Bilder.       --
--                   Gelesen wird immer aus dem Vollständigen Bild, während das     --
--	                  2. Bild aufbegaut wird                                         --
--                   Lesezugriffe erfolgen Passend zur Geometrie der                --
--                   Ventilator Blätter.                                            --
-- Pipelining:                                                                      --
-- Latenz:           11 clk                                                         --
--	                                                                                 --
-- Dependencies: 	   ram_write.vhd / ram_read.vhd                                   --
--	                                                                                 --
-- Version:          V9.7.5                                                         --
--                                                                                  --
-- Additional Comments: SRAM_2_IO und SRAM_x_UB/LB können auf einer eigenen         --
--	                     Platine Eingespart werden (20 Pins weniger ! )              --
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity ram_control is Port ( 

	  clk50 			   : IN 	std_logic;
	  clk100			   : IN	std_logic;
	  winkel 			: IN 	std_logic_vector( 9 downto 0);
	
	  ad_adr 			: IN 	std_logic_vector(17 downto 0);
	  ad_dat 			: IN 	std_logic_vector(15 downto 0);
	  ad_wr 			   : IN 	std_logic;
		
	  wr_str          : IN 	std_logic;
	  p_id            : IN 	std_logic_vector( 7 downto 0);
	  out_p           : IN 	std_logic_vector( 7 downto 0);

	  sram_adr	      : out std_logic_vector(17 downto 0);
	  sram_oe         : out std_logic;
	  sram_we         : out std_logic;

	  sram_1_ce       : out std_logic;
	  sram_1_ub       : out std_logic;
	  sram_1_lb       : out std_logic;
	  sram_1_io       : inout std_logic_vector(15 downto 0);

	  sram_2_ce       : out std_logic;
	  sram_2_ub       : out std_logic;
	  sram_2_lb       : out std_logic;
	  sram_2_io       : inout std_logic_vector(15 downto 0);
			  
	  sram_read       : out std_logic_vector(15 downto 0);
	  sram_pos        : out std_logic_vector( 7 downto 0);
	  winkel_diag     : out std_logic_vector( 9 downto 0);
	  write_lesen_diag: out std_logic
														);
end ram_control;


architecture Behavioral of ram_control is

	COMPONENT ram_write
	PORT(
		clk 				: IN 	std_logic;
		ad_adr 			: IN 	std_logic_vector(17 downto 0);
		ad_dat 			: IN 	std_logic_vector(15 downto 0);
		ad_wr 			: IN 	std_logic;
      cpu_rot        : IN  std_logic_vector( 7 downto 0);
      cpu_grun       : IN  std_logic_vector( 7 downto 0);
      cpu_blau       : IN  std_logic_vector( 7 downto 0);
      cpu_x_lo       : IN  std_logic_vector( 7 downto 0);
      cpu_x_hi       : IN  std_logic_vector( 7 downto 0);
      cpu_y_lo       : IN  std_logic_vector( 7 downto 0);
      cpu_y_hi	      : IN  std_logic_vector( 7 downto 0);
		cpu_write 		: IN 	std_logic;
		cpu_enable 		: IN  std_logic;          
		sram_adr 		: OUT std_logic_vector(17 downto 0);
		sram_dat 		: OUT std_logic_vector(15 downto 0);
		sram_wrt 		: OUT std_logic;
		sram_sel 		: OUT std_logic;
		write_enable 	: OUT std_logic;
		totes_lesen    : OUT std_logic
		);
	END COMPONENT;
	
	COMPONENT ram_read
	PORT(
		clk 				: IN std_logic;
		winkel 			: IN std_logic_vector(9 downto 0);
		write_enable 	: IN std_logic;          
		ram_adr 			: OUT std_logic_vector(17 downto 0);
		led_nr 			: OUT std_logic_vector( 7 downto 0);
		winkel_diag    : OUT std_logic_vector( 9 downto 0)
		);
	END COMPONENT;


-------------------------
-- Signal Definitionen --
-------------------------

-- inteface für picoblace	
constant adr    : integer := 0; -- Startadresse für Interface
signal cpu_rot       : std_logic_vector ( 7 downto 0);
signal cpu_grun      : std_logic_vector ( 7 downto 0);
signal cpu_blau      : std_logic_vector ( 7 downto 0);
signal cpu_x_lo      : std_logic_vector ( 7 downto 0);
signal cpu_x_hi      : std_logic_vector ( 7 downto 0);
signal cpu_y_lo      : std_logic_vector ( 7 downto 0);
signal cpu_y_hi	   : std_logic_vector ( 7 downto 0);
signal cpu_write 		: std_logic_vector ( 7 downto 0);
signal cpu_enable		: std_logic_vector ( 7 downto 0);

-- stufe u
signal write_adr_u	: std_logic_vector (17 downto 0);
signal write_dat_u   : std_logic_vector (15 downto 0);
signal write_wrt_u   : std_logic;
signal write_sel_u   : std_logic;
signal write_enable_u: std_logic;
signal totes_lesen_u : std_logic;
signal read_adr_u    : std_logic_vector (17 downto 0);

-- stufe 1
signal write_adr_1	: std_logic_vector (17 downto 0);
signal write_dat_1   : std_logic_vector (15 downto 0);
signal write_wrt_1   : std_logic;
signal write_sel_1   : std_logic;
signal write_enable_1: std_logic;
signal totes_lesen_1 : std_logic;
signal read_adr_1    : std_logic_vector (17 downto 0);

-- stufe 2
signal write_adr_2	: std_logic_vector (17 downto 0);
signal write_dat_2   : std_logic_vector (15 downto 0);
signal write_wrt_2   : std_logic;
signal write_sel_2   : std_logic;
signal write_enable_2: std_logic;
signal totes_lesen_2 : std_logic;
signal read_adr_2    : std_logic_vector (17 downto 0);

-- stufe 3
signal write_adr_3	: std_logic_vector (17 downto 0);
signal write_dat_3   : std_logic_vector (15 downto 0);
signal write_wrt_3   : std_logic;
signal write_sel_3   : std_logic;
signal write_enable_3: std_logic;
signal totes_lesen_3 : std_logic;
signal read_adr_3    : std_logic_vector (17 downto 0);

-- led_nr
signal led_nr_a: std_logic_vector (7 downto 0);
signal led_nr_b: std_logic_vector (7 downto 0);
signal led_nr_c: std_logic_vector (7 downto 0);
signal led_nr_d: std_logic_vector (7 downto 0);
signal led_nr_e: std_logic_vector (7 downto 0);

-- Read / Write
signal write_wrt_4: std_logic;
signal write_wrt_5: std_logic;
signal write_dat_4: std_logic_vector (15 downto 0);
signal sram_read_a: std_logic_vector (15 downto 0);
signal sram_read_b: std_logic_vector (15 downto 0);

-- Winkel
signal winkel_diag_u : std_logic_vector ( 9 downto 0);
signal winkel_diag_1 : std_logic_vector ( 9 downto 0);
signal winkel_diag_2 : std_logic_vector ( 9 downto 0);
signal winkel_diag_3 : std_logic_vector ( 9 downto 0);
signal winkel_diag_4 : std_logic_vector ( 9 downto 0);
signal winkel_diag_5 : std_logic_vector ( 9 downto 0);
signal winkel_diag_6 : std_logic_vector ( 9 downto 0);
signal winkel_diag_7 : std_logic_vector ( 9 downto 0);
signal winkel_diag_8 : std_logic_vector ( 9 downto 0);
signal winkel_diag_9 : std_logic_vector ( 9 downto 0);

-- sonstiges
signal write_help		: std_logic;
signal read_help		: std_logic:='0';
signal tristate      : std_logic;
signal totes_lesen_4 : std_logic;



begin

			sram_1_ub <= '0';							-- Alle Bytes ansprechen
			sram_1_lb <= '0';							-- aub beiden	
			sram_2_ub <= '0';							-- Speicher-
			sram_2_lb <= '0';                   -- bausteinen


---------------------------
-----    Port Maps    -----
---------------------------

	u0: ram_write PORT MAP(

		clk 				=> clk50,
		sram_adr 		=> write_adr_u,
		sram_dat 		=> write_dat_u,
		sram_wrt 		=> write_wrt_u,
		sram_sel 		=> write_sel_u,
		write_enable 	=> write_enable_u,
		totes_lesen    => totes_lesen_u,
		
		ad_adr 			=> ad_adr,
		ad_dat 			=> ad_dat,
		ad_wr 			=> ad_wr,
		
		cpu_rot 		   => cpu_rot,
		cpu_grun 		=> cpu_grun,
		cpu_blau 	   => cpu_blau,
		cpu_x_hi    	=> cpu_x_hi,
		cpu_x_lo 	   => cpu_x_lo,
		cpu_y_hi    	=> cpu_y_hi,
		cpu_y_lo 	   => cpu_y_lo,
		cpu_write 		=> cpu_write(0),
		cpu_enable 		=> cpu_enable(0)
	);

-------------------------------------------------------


	u1: ram_read PORT MAP(

		clk            => clk50,
		winkel         => winkel,
	   write_enable   => write_enable_u ,
		ram_adr        => read_adr_u,
		led_nr         => led_nr_a,
	   winkel_diag    => winkel_diag_u
	);


-----------------------------
-- Interface für Picoblaze --
-----------------------------

--process (clk50) begin if rising_edge (clk50) then if p_id = adr + 0  and wr_str = '1' then  cpu_rot    <= out_p   ; end if;end if; end process;
--process (clk50) begin if rising_edge (clk50) then if p_id = adr + 1  and wr_str = '1' then  cpu_grun   <= out_p   ; end if;end if; end process;
--process (clk50) begin if rising_edge (clk50) then if p_id = adr + 2  and wr_str = '1' then  cpu_blau   <= out_p   ; end if;end if; end process;
--process (clk50) begin if rising_edge (clk50) then if p_id = adr + 3  and wr_str = '1' then  cpu_x_hi   <= out_p   ; end if;end if; end process;
--process (clk50) begin if rising_edge (clk50) then if p_id = adr + 4  and wr_str = '1' then  cpu_x_lo   <= out_p   ; end if;end if; end process;
--process (clk50) begin if rising_edge (clk50) then if p_id = adr + 5  and wr_str = '1' then  cpu_y_hi   <= out_p   ; end if;end if; end process;
--process (clk50) begin if rising_edge (clk50) then if p_id = adr + 6  and wr_str = '1' then  cpu_y_lo   <= out_p   ; end if;end if; end process;
--process (clk50) begin if rising_edge (clk50) then if p_id = adr + 7  and wr_str = '1' then  cpu_write  <= out_p   ; end if;end if; end process;
--process (clk50) begin if rising_edge (clk50) then if p_id = adr + 8  and wr_str = '1' then  cpu_enable <= out_p   ; end if;end if; end process;

	
-------------------------------------
-- synchronisieren zum 100 mhz clk --
-------------------------------------

process (clk100) begin
	if rising_edge (clk100) then
	
		write_adr_1 	<= write_adr_u;
		write_dat_1 	<= write_dat_u;
		write_wrt_1 	<= write_wrt_u;
		write_sel_1 	<= write_sel_u;
		write_enable_1 <= write_enable_u;
		totes_lesen_1  <= totes_lesen_u;
		read_adr_1     <= read_adr_u;
		
--------------------------------------------------

		write_adr_2 	<= write_adr_1;
		write_dat_2 	<= write_dat_1;
		write_wrt_2 	<= write_wrt_1;
		write_sel_2 	<= write_sel_1;
		write_enable_2 <= write_enable_1;
		totes_lesen_2  <= totes_lesen_1;
		read_adr_2     <= read_adr_1;

--------------------------------------------------

		write_adr_3 	<= write_adr_2;
		write_dat_3 	<= write_dat_2;
		write_wrt_3 	<= write_wrt_2;
		write_sel_3 	<= write_sel_2;
		write_enable_3 <= write_enable_2;
		totes_lesen_3  <= totes_lesen_2;
		read_adr_3     <= read_adr_2;

	end if;
end process;


-- Sonstige Verzögerungen

process (clk100) begin
	if rising_edge (clk100) then

		totes_lesen_4 <= totes_lesen_3; 			-- für sram_oe & sram_ce

		write_wrt_3 <= write_wrt_2;				--
      write_wrt_4 <= write_wrt_3;				-- für sram_read
      write_wrt_5 <= write_wrt_4;				-- 

		write_dat_4 <= write_dat_3;				-- für sram_io (schreiben)
	end if;
end process;


---------------------
-- Speicherzugriff --
---------------------


-- sram Addresse bestimmen

process (clk100) begin
	if rising_edge (clk100) then
			if write_wrt_3 = '1' then					-- Gibt die Leseadresse
																-- oder die Schreib Adresse
				if totes_lesen_3='0' then				-- auf den SRAM Adressbus
					sram_adr <= write_adr_3;			-- 
				else											-- Zwischen Lesen und schreiben
					sram_adr <= (others => '0');		-- gibt es ein Byte (totes lesen)
				end if;										-- an dem keine Adresse Ausgegeben wird
			else 												
				if totes_lesen_3='0' then
					sram_adr <= read_adr_3;				
				else
					sram_adr <= (others => '0');
				end if;	
			
			end if;
	end if;
end process;


-- sram output enable

process (clk100) begin
	if rising_edge (clk100) then
		if write_wrt_3 = '1' then						-- SRAM Output enable
				sram_oe <= '1';							-- setzen
		else --                        				-- Beim Schreiben sind die
				sram_oe <= '0';							-- Ausgänge Abgeschaltet

			if totes_lesen_4='0' then					-- einen Halben Takt vor schreiben
					sram_oe <= '0';						-- die Ausgänge setzen 
			else
				sram_oe <= '1';			 
			end if;	

		end if;
	end if;
end process;


-- sram Chip enable

process (clk100) begin
	if rising_edge (clk100) then
		if write_wrt_3 = '1' or totes_lesen_4 = '1' then	-- Die Beiden Chip Enables
			if write_sel_3 = '0' then								-- werden in abhängigkeit 
				sram_1_ce <= '0';										-- von Lesen und Schreiben
				sram_2_ce <= '1';										-- gesetzt 
			else															-- wichtig ist auch,
				sram_1_ce <= '1';										-- in welchen chip geschrieben /
				sram_2_ce <= '0';										-- oder gelesen wird
			end if;	
		else 
			if write_sel_3 = '1' then		 
				sram_1_ce <= '0';				
				sram_2_ce <= '1';				
			else	
				sram_1_ce <= '1';				 
				sram_2_ce <= '0';
			end if;
		end if;
	end if;
end process;


-- Tristate treiber

process (clk100) begin
  if rising_edge (clk100) then
		tristate <= not write_wrt_3;
	end if;
end process;

sram_1_io  <= write_dat_4 when tristate='0' else (others => 'Z');    
sram_2_io  <= write_dat_4 when tristate='0' else (others => 'Z');


-- Write Enable erzeugen			

process (clk100) begin
  if rising_edge (clk100) then
		if write_wrt_2 = '1' then					-- Signal zum schreiben erzeugen
			write_help <= not write_help;
			sram_we <= write_wrt_2 and write_help;
		else
			write_help <='1';
			sram_we <= '1';
		end if;
	end if;
end process;


-- Daten aus speicher lesen

process (clk100) 
variable a,b,lesen : std_logic;
begin

  if rising_edge (clk100) then				

			a := not write_wrt_3;
			b := not write_wrt_5;
			lesen := a and b;
														-- Die Daten zum Lesen aus dem Speicher
		if lesen = '1' then						-- stehen erst einen Halben Takt
			read_help <= not read_help;		-- Nach Auflegen der Adresse Bereit
			if read_help = '1' then				
			
--			sram_read_a <= sram_1_io;			-- Lesen mit einem SRAM_IO
														-- (für bords mit nur einem Datenbus)

				if write_sel_2 = '0'  then		--
					sram_read_a <= sram_2_io;	-- Lesen mit
				else 									-- 2 Datenbussen
					sram_read_a <= sram_1_io;	--
				end if;								--
			end if;
			else    read_help <= '1';
		end if;
	end if;
end process;


----------------------------
-- gelesene Daten mit     --
-- 50 MHt synchronisieren --
----------------------------

process (clk50) begin
	if rising_edge (clk50) then
      led_nr_b <= led_nr_a;
      led_nr_c <= led_nr_b;
      led_nr_d <= led_nr_c;
      led_nr_e <= led_nr_d;
		sram_pos <= led_nr_d;
		
	sram_read_b <= sram_read_a;	
	sram_read   <= sram_read_b;	

	end if;
end Process;


-- Winkel in den richtigen Zeitlichen bezug bringen

process (clk50) begin
if rising_edge (clk50) then
	winkel_diag_1 <= winkel_diag_u;
	winkel_diag_2 <= winkel_diag_1;
	winkel_diag_3 <= winkel_diag_2;
	winkel_diag_4 <= winkel_diag_3;
	winkel_diag_5 <= winkel_diag_4;
	winkel_diag_6 <= winkel_diag_5;
	winkel_diag_7 <= winkel_diag_6;
	winkel_diag_8 <= winkel_diag_7;
	winkel_diag_9 <= winkel_diag_8;
	winkel_diag   <= winkel_diag_9;
end if;
end process;	



-- Diagnose

write_lesen_diag <= totes_lesen_3;

end Behavioral;

