--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
-- Hackerspace:      Das Labor    www.das-labor.org                                 --
-- Hacker:           Sauron                                                         --
--                                                                                  --
-- Datum:            14.07.2009                                                     --
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
-- Version:          V9.7.2                                                         --
--                                                                                  --
-- Additional Comments: Widerstand ist Zwecklos                                     --
--	                                                                                 --
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity ram_control is Port ( 

		clk50 			: IN 	std_logic;
		clk100			: IN	std_logic;
		winkel 			: IN 	std_logic_vector (9 downto 0);
	
		ad_adr 			: IN 	std_logic_vector(17 downto 0);
		ad_dat 			: IN 	std_logic_vector(15 downto 0);
		ad_wr 			: IN 	std_logic;
		
	   wr_str         : IN 	std_logic;
	   p_id           : IN 	std_logic_vector (7 downto 0);
		out_p          : IN 	std_logic_vector (7 downto 0);


--      cpu_rot        : IN  std_logic_vector ( 7 downto 0);
--      cpu_grun       : IN  std_logic_vector ( 7 downto 0);
--      cpu_blau       : IN  std_logic_vector ( 7 downto 0);
--      cpu_x_lo       : IN  std_logic_vector ( 7 downto 0);
--      cpu_x_hi       : IN  std_logic_vector ( 7 downto 0);
--      cpu_y_lo       : IN  std_logic_vector ( 7 downto 0);
--      cpu_y_hi	      : IN  std_logic_vector ( 7 downto 0);
--		cpu_write 		: IN 	std_logic_vector ( 7 downto 0);
--		cpu_enable 		: IN 	std_logic_vector ( 7 downto 0);

	 
	        sram_adr	: out std_logic_vector (17 downto 0);
			  sram_oe   : out std_logic;
			  sram_we   : out std_logic;

	        sram_1_ce : out std_logic;
	        sram_1_ub : out std_logic;
	        sram_1_lb : out std_logic;
	        sram_1_io : inout std_logic_vector (15 downto 0);

	        sram_2_ce : out std_logic;
	        sram_2_ub : out std_logic;
	        sram_2_lb : out std_logic;
	        sram_2_io : inout std_logic_vector (15 downto 0);
			  
		sram_read      : out std_logic_vector (15 downto 0);
		sram_pos       : out std_logic_vector ( 7 downto 0);
		winkel_diag    : out std_logic_vector ( 9 downto 0);
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
      cpu_rot        : IN  std_logic_vector ( 7 downto 0);
      cpu_grun       : IN  std_logic_vector ( 7 downto 0);
      cpu_blau       : IN  std_logic_vector ( 7 downto 0);
      cpu_x_lo       : IN  std_logic_vector ( 7 downto 0);
      cpu_x_hi       : IN  std_logic_vector ( 7 downto 0);
      cpu_y_lo       : IN  std_logic_vector ( 7 downto 0);
      cpu_y_hi	      : IN  std_logic_vector ( 7 downto 0);
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

signal write_enable	: std_logic;
signal write_help		: std_logic;
signal read_help		: std_logic:='0';
signal sram_read_2   : std_logic_vector (15 downto 0);
signal led_nr_2      : std_logic_vector ( 7 downto 0);
signal led_nr_3      : std_logic_vector ( 7 downto 0);
signal write_dat     : std_logic_vector (15 downto 0);

-- stufe u
signal write_adr_u	: std_logic_vector (17 downto 0);
signal write_dat_u   : std_logic_vector (15 downto 0);
signal write_wrt_u   : std_logic;
signal write_wrt_b   : std_logic;
signal write_wrt   : std_logic;
signal write_sel_u   : std_logic;
signal write_sel_b   : std_logic;
signal write_sel   : std_logic;
signal read_adr_u    : std_logic_vector (17 downto 0);
signal led_nr_u      : std_logic_vector ( 7 downto 0);

-- stufe 1
signal write_wrt_1   : std_logic;
signal write_sel_1   : std_logic;
signal led_nr_1      : std_logic_vector ( 7 downto 0);

-- stufe a
signal sram_adr_a    : std_logic_vector(17 downto 0);
signal sram_oe_a     : std_logic;
signal sram_we_a     : std_logic;
signal sram_1_ce_a   : std_logic;
signal sram_2_ce_a   : std_logic;
signal write_dat_a   : std_logic_vector (15 downto 0);
-- stufe b
signal sram_adr_b    : std_logic_vector(17 downto 0);
signal sram_oe_b     : std_logic;
signal sram_we_b     : std_logic;
signal sram_1_ce_b   : std_logic;
signal sram_2_ce_b   : std_logic;
signal write_dat_b   : std_logic_vector (15 downto 0);


signal winkel_diag1  : std_logic_vector (9 downto 0);
signal winkel_diag2  : std_logic_vector (9 downto 0);
signal winkel_diag3  : std_logic_vector (9 downto 0);
signal winkel_diag4  : std_logic_vector (9 downto 0);
signal winkel_diag5  : std_logic_vector (9 downto 0);
signal winkel_diag6  : std_logic_vector (9 downto 0);
signal winkel_diag7  : std_logic_vector (9 downto 0);
signal winkel_diag8  : std_logic_vector (9 downto 0);
signal winkel_diag9  : std_logic_vector (9 downto 0);

signal totes_lesen   : std_logic;
signal write_lesen   : std_logic;
signal write_lesen_a   : std_logic;
signal write_lesen_b   : std_logic;
signal write_l1 : std_logic;


begin

	u0: ram_write PORT MAP(
		clk 				=> clk50,
		sram_adr 		=> write_adr_u,
		sram_dat 		=> write_dat_u,
		sram_wrt 		=> write_wrt_u,
		sram_sel 		=> write_sel_u,
		write_enable 	=> write_enable,
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
		cpu_enable 		=> cpu_enable(0),
		totes_lesen    => totes_lesen
	);

	u1: ram_read PORT MAP(
		clk       => clk50,
		winkel    => winkel,
	write_enable => write_enable ,
		ram_adr   => read_adr_u,
		led_nr    => led_nr_u,
	winkel_diag  => winkel_diag1
	);

-- Interface
process (clk50) begin if rising_edge (clk50) then if p_id = adr + 0  and wr_str = '1' then  cpu_rot    <= out_p   ; end if;end if; end process;
process (clk50) begin if rising_edge (clk50) then if p_id = adr + 1  and wr_str = '1' then  cpu_grun   <= out_p   ; end if;end if; end process;
process (clk50) begin if rising_edge (clk50) then if p_id = adr + 2  and wr_str = '1' then  cpu_blau   <= out_p   ; end if;end if; end process;
process (clk50) begin if rising_edge (clk50) then if p_id = adr + 3  and wr_str = '1' then  cpu_x_hi   <= out_p   ; end if;end if; end process;
process (clk50) begin if rising_edge (clk50) then if p_id = adr + 4  and wr_str = '1' then  cpu_x_lo   <= out_p   ; end if;end if; end process;
process (clk50) begin if rising_edge (clk50) then if p_id = adr + 5  and wr_str = '1' then  cpu_y_hi   <= out_p   ; end if;end if; end process;
process (clk50) begin if rising_edge (clk50) then if p_id = adr + 6  and wr_str = '1' then  cpu_y_lo   <= out_p   ; end if;end if; end process;
process (clk50) begin if rising_edge (clk50) then if p_id = adr + 7  and wr_str = '1' then  cpu_write  <= out_p   ; end if;end if; end process;
process (clk50) begin if rising_edge (clk50) then if p_id = adr + 8  and wr_str = '1' then  cpu_enable <= out_p   ; end if;end if; end process;


process (clk50) begin
	if rising_edge (clk50) then
		if write_wrt_u = '1' then			-- in den Speicher schreiben
				sram_adr_a <= write_adr_u;				-- Adresse zum schreiben
				sram_oe_a <= '1';				-- schreiben beschleunigen durch
			if write_sel_u = '0' then		-- abschalten der sram ausgänge
				sram_1_ce_a <= '0';				-- schreiben in
				sram_2_ce_a <= '1';				-- sram 1 oder sram 2
			else
				sram_1_ce_a <= '1';				-- Sram 2 
				sram_2_ce_a <= '0';
			end if;	
		
		else --                        Aus speicher lesen
			if totes_lesen='0' then
				sram_adr_a <= read_adr_u;					-- Adresse zum Lesen
				sram_oe_a <= '0';
			else
				sram_adr_a <= (others => '0');
				sram_oe_a <= '1';
			end if;	
			
			if write_sel_u = '1' then		 
				sram_1_ce_a <= '0';				-- lesen von 
				sram_2_ce_a <= '1';				-- sram 1
			else	

				sram_1_ce_a <= '1';				-- Sram 2 
				sram_2_ce_a <= '0';
			end if;		end if;
	end if;
end process;


-- synchronisieren zum 100 mhz clk
process (clk100) begin
	if rising_edge (clk100) then
		sram_adr_b  <= sram_adr_a  ;
		sram_oe_b   <= sram_oe_a   ;
		sram_we_b   <= sram_we_a   ;
		sram_1_ce_b <= sram_1_ce_a ;
		sram_2_ce_b <= sram_2_ce_a ;
		write_dat_b <= write_dat_a ;
		write_sel_b <= write_sel_1 ;
		write_wrt_b <= write_wrt_1 ;
	 write_lesen_b <= write_lesen_a;
	end if;
end process;

process (clk100) begin
	if rising_edge (clk100) then
		sram_adr  <= sram_adr_b  ;
		sram_oe   <= sram_oe_b   ;
		sram_we   <= sram_we_b   ;
		sram_1_ce <= sram_1_ce_b ;
		sram_2_ce <= sram_2_ce_b ;
		write_dat <= write_dat_b ;
		write_sel <= write_sel_b ;
		write_wrt <= write_wrt_b ;
	 write_lesen <= write_lesen_b ;
	end if;
end process;

-- Entweder Daten ausgeben, oder Tristate
sram_1_io  <= write_dat when write_wrt='1' else (others => 'Z');    
sram_2_io  <= write_dat when write_wrt='1' else (others => 'Z');

		
			sram_1_ub <= '0';							-- Alle Bytes ansprechen
			sram_1_lb <= '0';
			sram_2_ub <= '0';
			sram_2_lb <= '0';

-- Write Enable erzeugen			
process (clk100) begin
  if rising_edge (clk100) then
		if write_wrt_u = '1' then					-- Signal zum schreiben erzeugen
			write_help <= not write_help;
			sram_we_a <= write_wrt_u and write_help;
		else
			write_help <='1';
			sram_we_a <= '1';
		end if;
	end if;
end process;
							
-- Zum richtigen Zeitpunkt ein Byte aus dem Speicher Lesen
process (clk100) begin
  if rising_edge (clk100) then
														-- Aus sram lesen
		if write_lesen = '1' then

			read_help <= not read_help;		--jeden 2. takt (100 mhz)
			if read_help = '1' then					-- flanke zum lesen
			
			sram_read_2 <= sram_1_io;
				if write_sel_u = '0'  then
					sram_read_2 <= sram_2_io;
				else 
					sram_read_2 <= sram_1_io;
				end if;
			end if;
			
			else
					read_help <= '1';
		end if;
	end if;
end process;



process (clk50) 
variable a,b : std_logic;
begin
if rising_edge (clk50) then
a := not write_l1; -- ende des schreibens
b := not write_wrt_u; -- beginn des schreibens
	write_lesen_a <= (a and b);
end if;
end process;	

write_lesen_diag <= write_lesen;

process (clk50) begin
if rising_edge (clk50) then
	sram_read   <= sram_read_2;
	led_nr_1    <= led_nr_u;
	led_nr_2		<= led_nr_1;
	led_nr_3		<= led_nr_2;
--	led_nr_4		<= led_nr_3;
	sram_pos		<= led_nr_3;
	write_l1    <= write_wrt_u;
	write_dat_a	<= write_dat_u;
	write_wrt_1	<= write_wrt_u;
	write_sel_1	<= write_sel_u;
end if;
end process;	

-- Diagnose

process (clk50) begin
if rising_edge (clk50) then
	winkel_diag2 <= winkel_diag1;
	winkel_diag3 <= winkel_diag2;
	winkel_diag4 <= winkel_diag3;
	winkel_diag5 <= winkel_diag4;
	winkel_diag6 <= winkel_diag5;
	winkel_diag7 <= winkel_diag6;
	winkel_diag8 <= winkel_diag7;
	winkel_diag9 <= winkel_diag8;
	winkel_diag  <= winkel_diag9;
end if;
end process;	

-- warnungen reduzieren

end Behavioral;

