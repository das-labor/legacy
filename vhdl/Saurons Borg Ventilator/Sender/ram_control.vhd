
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

		cpu_dat_x		: IN 	std_logic_vector(7 downto 0);
		cpu_dat_y 		: IN 	std_logic_vector(7 downto 0);
		cpu_adr_hi_lo 	: IN 	std_logic_vector(7 downto 0);
		cpu_adr_hi_hi 	: IN 	std_logic;
		cpu_adr_lo_lo 	: IN 	std_logic_vector(7 downto 0);
		cpu_adr_lo_hi 	: IN 	std_logic;
		cpu_write 		: IN 	std_logic;
		cpu_enable 		: IN 	std_logic;

	 
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
			  
			  sram_read : out std_logic_vector (15 downto 0);
			  sram_pos  : out std_logic_vector ( 7 downto 0);
			  write_sel_diag  : out std_logic
			  
														);
end ram_control;

architecture Behavioral of ram_control is

	
	COMPONENT ram_write
	PORT(
		clk 				: IN 	std_logic;
		ad_adr 			: IN 	std_logic_vector(17 downto 0);
		ad_dat 			: IN 	std_logic_vector(15 downto 0);
		ad_wr 			: IN 	std_logic;
		cpu_dat_x 		: IN 	std_logic_vector(7 downto 0);
		cpu_dat_y 		: IN 	std_logic_vector(7 downto 0);
		cpu_adr_hi_lo 	: IN 	std_logic_vector(7 downto 0);
		cpu_adr_hi_hi 	: IN 	std_logic;
		cpu_adr_lo_lo 	: IN 	std_logic_vector(7 downto 0);
		cpu_adr_lo_hi 	: IN 	std_logic;
		cpu_write 		: IN 	std_logic;
		cpu_enable 		: IN 	std_logic;          
		sram_adr 		: OUT std_logic_vector(17 downto 0);
		sram_dat 		: OUT std_logic_vector(15 downto 0);
		sram_wrt 		: OUT std_logic;
		sram_sel 		: OUT std_logic;
		write_enable 	: OUT std_logic
		);
	END COMPONENT;
	
	COMPONENT ram_read
	PORT(
		clk 				: IN std_logic;
		winkel 			: IN std_logic_vector(9 downto 0);
		write_enable 	: IN std_logic;          
		ram_adr 			: OUT std_logic_vector(17 downto 0);
		led_nr 			: OUT std_logic_vector(7 downto 0);
		winkel_diag 	: OUT std_logic_vector(9 downto 0)
		);
	END COMPONENT;

signal write_enable	: std_logic;
signal write_help		: std_logic;
signal read_help		: std_logic:='0';
signal sram_read_3a  : std_logic_vector (15 downto 0);
signal sram_read_4   : std_logic_vector (15 downto 0);
signal led_nr_2      : std_logic_vector ( 7 downto 0);
signal led_nr_3      : std_logic_vector ( 7 downto 0);
signal led_nr_4      : std_logic_vector ( 7 downto 0);

-- stufe u
signal write_adr_u	: std_logic_vector (17 downto 0);
signal write_dat_u   : std_logic_vector (15 downto 0);
signal write_wrt_u   : std_logic;
signal write_sel_u   : std_logic;
signal read_adr_u    : std_logic_vector (17 downto 0);
signal led_nr_u      : std_logic_vector ( 7 downto 0);
signal winkel_diag_u : std_logic_vector ( 9 downto 0);

-- stufe 1
signal write_adr_1	: std_logic_vector (17 downto 0);
signal write_dat_1   : std_logic_vector (15 downto 0);
signal write_wrt_1   : std_logic;
signal write_sel_1   : std_logic;
signal read_adr_1    : std_logic_vector (17 downto 0);
signal led_nr_1      : std_logic_vector ( 7 downto 0);
signal winkel_diag_1 : std_logic_vector ( 9 downto 0);

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
		cpu_dat_x 		=> cpu_dat_x,
		cpu_dat_y 		=> cpu_dat_y,
		cpu_adr_hi_lo 	=> cpu_adr_hi_lo,
		cpu_adr_hi_hi 	=> cpu_adr_hi_hi,
		cpu_adr_lo_lo 	=> cpu_adr_lo_lo,
		cpu_adr_lo_hi 	=> cpu_adr_lo_hi,
		cpu_write 		=> cpu_write,
		cpu_enable 		=> cpu_enable
	);

	u1: ram_read PORT MAP(
		clk => clk50,
		winkel => winkel,
		write_enable => write_enable ,
		ram_adr => read_adr_u,
		led_nr => led_nr_u,
		winkel_diag => winkel_diag_u
	);


-- synchronisieren zum 100 mhz clk
process (clk100) begin
	if rising_edge (clk100) then
		write_adr_1	<= write_adr_u;
		write_dat_1	<= write_dat_u;
		write_wrt_1	<= write_wrt_u;
		write_sel_1	<= write_sel_u;
		 read_adr_1 <= read_adr_u;
		   led_nr_1 <= led_nr_u;
	 winkel_diag_1 <= winkel_diag_u;
	end if;
end process;


process (clk100) begin
	if rising_edge (clk100) then
		if write_wrt_1 = '1' then			-- in den Speicher schreiben
				sram_adr <= write_adr_1;				-- Adresse zum schreiben
				sram_1_io  <= write_dat_1;				-- Daten schreiben 
				sram_2_io  <= write_dat_1;
				sram_oe <= '1';				-- schreiben beschleunigen durch
			if write_sel_1 = '0' then		-- abschalten der sram ausg�nge
				sram_1_ce <= '0';				-- schreiben in
				sram_2_ce <= '1';				-- sram 1
			else
				sram_1_ce <= '1';				-- Sram 2 
				sram_2_ce <= '0';
			end if;	
		
		else --                        Aus speicher lesen
				sram_adr <= read_adr_1;					-- Adresse zum Lesen
				sram_1_io  <= (others => 'Z');		-- IO auf Treestate
				sram_2_io  <= (others => 'Z');
				sram_oe <= '0';
			
			if write_sel_1 = '1' then		-- 
				sram_1_ce <= '0';				-- lesen von 
				sram_2_ce <= '1';				-- sram 1
			else
				sram_1_ce <= '1';				-- Sram 2 
				sram_2_ce <= '0';
			end if;	
	
		end if;
		
			sram_1_ub <= '0';							-- Alle Bytes ansprechen
			sram_1_lb <= '0';
			sram_2_ub <= '0';
			sram_2_lb <= '0';
			
			
		if write_wrt_1 = '1' then					-- Signal zum schreiben erzeugen
			write_help <= not write_help;
			sram_we <= write_wrt_1 and write_help;
		else
			write_help <='0';
			sram_we <= '1';
		end if;
							

														-- Aus sram lesen
		if write_wrt_1 = '0' then
			read_help <= not read_help;
			
			if read_help = '1' then					-- flanke zum lesen
				if write_sel_1 = '0'  then
					sram_read_3a <= sram_1_io;
				else 
					sram_read_3a <= sram_2_io;
				end if;
			end if;
			else
					read_help <= '0';
					sram_read_3a <= (others => '0');
		end if;
	end if;
end process;

process (clk50) begin
if rising_edge (clk50) then
	sram_read_4 <= sram_read_3a;
	sram_read   <= sram_read_4;
	led_nr_2		<= led_nr_1;
	led_nr_3		<= led_nr_2;
	led_nr_4		<= led_nr_3;
	sram_pos		<= led_nr_4;
end if;
end process;	

write_sel_diag <= write_sel_1;

end Behavioral;

