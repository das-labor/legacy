LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.numeric_std.ALL;
use IEEE.MATH_REAL.all;

use work.sim_bmppack.all;



ENTITY vtb_ram_control2_vhd IS
END vtb_ram_control2_vhd;

ARCHITECTURE behavior OF vtb_ram_control2_vhd IS 

	-- Component Declaration for the Unit Under Test (UUT)
	COMPONENT ram_control
	PORT(
		clk50 : IN std_logic;
		clk100 : IN std_logic;
		winkel : IN std_logic_vector(9 downto 0);
		ad_adr : IN std_logic_vector(17 downto 0);
		ad_dat : IN std_logic_vector(15 downto 0);
		ad_wr : IN std_logic;
		cpu_dat_x : IN std_logic_vector(7 downto 0);
		cpu_dat_y : IN std_logic_vector(7 downto 0);
		cpu_adr_hi_lo : IN std_logic_vector(7 downto 0);
		cpu_adr_hi_hi : IN std_logic;
		cpu_adr_lo_lo : IN std_logic_vector(7 downto 0);
		cpu_adr_lo_hi : IN std_logic;
		cpu_write : IN std_logic;
		cpu_enable : IN std_logic;    
		sram_1_io : INOUT std_logic_vector(15 downto 0);
		sram_2_io : INOUT std_logic_vector(15 downto 0);      
--		write_adr_d : OUT std_logic_vector(17 downto 0);
--		write_dat_d : OUT std_logic_vector(15 downto 0);
--		write_wrt_d : OUT std_logic;
--		write_sel_d : OUT std_logic;
--		read_adr_d : OUT std_logic_vector(17 downto 0);
--		led_nr_d : OUT std_logic_vector(7 downto 0);
--		winkel_diag_d : OUT std_logic_vector(9 downto 0);
		sram_adr : OUT std_logic_vector(17 downto 0);
		sram_oe : OUT std_logic;
		sram_we : OUT std_logic;
		sram_1_ce : OUT std_logic;
		sram_1_ub : OUT std_logic;
		sram_1_lb : OUT std_logic;
		sram_2_ce : OUT std_logic;
		sram_2_ub : OUT std_logic;
		sram_2_lb : OUT std_logic;
		sram_read : OUT std_logic_vector(15 downto 0);
 	   sram_pos  : out std_logic_vector ( 7 downto 0)

		);
	END COMPONENT;

	--Inputs
	SIGNAL clk50 :  std_logic := '0';
	SIGNAL clk100 :  std_logic := '0';
	SIGNAL ad_wr :  std_logic := '0';
	SIGNAL cpu_adr_hi_hi :  std_logic := '0';
	SIGNAL cpu_adr_lo_hi :  std_logic := '0';
	SIGNAL cpu_write :  std_logic := '0';
	SIGNAL cpu_enable :  std_logic := '0';
	SIGNAL winkel :  std_logic_vector(9 downto 0) := (others=>'0');
	SIGNAL ad_adr :  std_logic_vector(17 downto 0) := (others=>'0');
	SIGNAL ad_dat :  std_logic_vector(15 downto 0) := (others=>'0');
	SIGNAL cpu_dat_x :  std_logic_vector(7 downto 0) := (others=>'0');
	SIGNAL cpu_dat_y :  std_logic_vector(7 downto 0) := (others=>'0');
	SIGNAL cpu_adr_hi_lo :  std_logic_vector(7 downto 0) := (others=>'0');
	SIGNAL cpu_adr_lo_lo :  std_logic_vector(7 downto 0) := (others=>'0');

	--BiDirs
	SIGNAL sram_1_io :  std_logic_vector(15 downto 0);
	SIGNAL sram_2_io :  std_logic_vector(15 downto 0);

	--Outputs
--	SIGNAL write_adr_d :  std_logic_vector(17 downto 0);
--	SIGNAL write_dat_d :  std_logic_vector(15 downto 0);
--	SIGNAL write_wrt_d :  std_logic;
--	SIGNAL write_sel_d :  std_logic;
--	SIGNAL read_adr_d :  std_logic_vector(17 downto 0);
--	SIGNAL led_nr_d :  std_logic_vector(7 downto 0);
--	SIGNAL winkel_diag_d :  std_logic_vector(9 downto 0);
	SIGNAL sram_adr :  std_logic_vector(17 downto 0);
	SIGNAL sram_oe :  std_logic;
	SIGNAL sram_we :  std_logic;
	SIGNAL sram_1_ce :  std_logic;
	SIGNAL sram_1_ub :  std_logic;
	SIGNAL sram_1_lb :  std_logic;
	SIGNAL sram_2_ce :  std_logic;
	SIGNAL sram_2_ub :  std_logic;
	SIGNAL sram_2_lb :  std_logic;
	SIGNAL sram_read :  std_logic_vector(15 downto 0);
	SIGNAL sram_pos  :  std_logic_vector ( 7 downto 0);

	
		signal count_i: std_logic_vector(17 downto 0):= (others =>'0');
		signal read_help : std_logic_vector(15 downto 0):= (others =>'0');
		signal colordata : std_logic_vector(23 downto 0) := x"ffffff";
	

BEGIN

	-- Instantiate the Unit Under Test (UUT)
	uut: ram_control PORT MAP(
		clk50 => clk50,
		clk100 => clk100,
		winkel => winkel,
		ad_adr => ad_adr,
		ad_dat => ad_dat,
		ad_wr => ad_wr,
		cpu_dat_x => cpu_dat_x,
		cpu_dat_y => cpu_dat_y,
		cpu_adr_hi_lo => cpu_adr_hi_lo,
		cpu_adr_hi_hi => cpu_adr_hi_hi,
		cpu_adr_lo_lo => cpu_adr_lo_lo,
		cpu_adr_lo_hi => cpu_adr_lo_hi,
		cpu_write => cpu_write,
		cpu_enable => cpu_enable,
--		write_adr_d => write_adr_d,
--		write_dat_d => write_dat_d,
--		write_wrt_d => write_wrt_d,
--		write_sel_d => write_sel_d,
--		read_adr_d => read_adr_d,
--		led_nr_d => led_nr_d,
--		winkel_diag_d => winkel_diag_d,
		sram_adr => sram_adr,
		sram_oe => sram_oe,
		sram_we => sram_we,
		sram_1_ce => sram_1_ce,
		sram_1_ub => sram_1_ub,
		sram_1_lb => sram_1_lb,
		sram_1_io => sram_1_io,
		sram_2_ce => sram_2_ce,
		sram_2_ub => sram_2_ub,
		sram_2_lb => sram_2_lb,
		sram_2_io => sram_2_io,
		sram_read => sram_read,
		sram_pos  => sram_pos
	);

-- generiere 50 Mhz Takt
process begin
		clk50 <= '1';
		wait for 10 ns;
		clk50 <= '0';
		wait for 10 ns; 
end process;

-- generiere 100 Mhz Takt
process begin
		clk100 <= '1';
		wait for 5 ns;
		clk100 <= '0';
		wait for 5 ns; 
end process;


-- erzeuge ein Testmuster für den ad_wandler eingang
process (clk50) begin
	if rising_edge (clk50) then
		if ad_wr = '0' then ad_wr <= '1';
		else ad_wr <= '0';
		end if;
		
		if ad_wr = '0' then
		count_i <= count_i + 1;
		end if;
		
		ad_dat <= count_i (17 downto 10) & count_i (8 downto 1);
		ad_adr <= count_i;
	end if;
end process;

-- testpattern für leseoperationen
--process (clk100) begin
--	if rising_edge (clk100) then
--		sram_1_io <= read_help;
--		sram_2_io <= read_help;
--		read_help <= read_help + 1;
--		if read_help = "1111111111111111" then read_help <= (others => '0');end if;
--	end if;
--end process;


-- RAM Lesezugriffe auf ein reales Bild umleiten 
-- Das Bild muss 512 x 512 pixel gross sein
process begin
    ReadFile("lena.bmp");
	wait;
end process;


process (clk50)
variable read_x : integer; 
variable read_y : integer; 
  begin

  if rising_edge (clk50) then
	 
	if sram_we = '1' and sram_oe = '0' then 
	
	read_x := CONV_INTEGER(sram_adr( 8 downto 0));
	read_y := CONV_INTEGER(sram_adr(17 downto 9));
	
		
		getpixel(read_x,read_y,colordata);
		
							
		-- Farbauflösung von 16M auf 64k reduzieren
		sram_1_io <= colordata (23 downto 19) & colordata (15 downto 10) & colordata (7 downto 3);
		
	else 
		sram_1_io <= (others => 'Z');

	end if;
  end if;

end process;
END;
