LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.numeric_std.ALL;

ENTITY vtb_ram_write_vhd IS
END vtb_ram_write_vhd;

ARCHITECTURE behavior OF vtb_ram_write_vhd IS 

	-- Component Declaration for the Unit Under Test (UUT)
	COMPONENT ram_write
	PORT(
		clk : IN std_logic;
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
		sram_adr : OUT std_logic_vector(17 downto 0);
		sram_dat : OUT std_logic_vector(15 downto 0);
		sram_wrt : OUT std_logic;
		sram_sel : OUT std_logic;
      read_enable: out   STD_LOGIC

		);
	END COMPONENT;

	--Inputs
	SIGNAL clk :  std_logic := '0';
	SIGNAL ad_wr :  std_logic := '0';
	SIGNAL cpu_adr_hi_hi :  std_logic := '0';
	SIGNAL cpu_adr_lo_hi :  std_logic := '0';
	SIGNAL cpu_write :  std_logic := '0';
	SIGNAL cpu_enable :  std_logic := '0';
	SIGNAL ad_adr :  std_logic_vector(17 downto 0) := (others=>'0');
	SIGNAL ad_dat :  std_logic_vector(15 downto 0) := (others=>'0');
	SIGNAL cpu_dat_x :  std_logic_vector(7 downto 0) := (others=>'0');
	SIGNAL cpu_dat_y :  std_logic_vector(7 downto 0) := (others=>'0');
	SIGNAL cpu_adr_hi_lo :  std_logic_vector(7 downto 0) := (others=>'0');
	SIGNAL cpu_adr_lo_lo :  std_logic_vector(7 downto 0) := (others=>'0');

	--Outputs
	SIGNAL sram_adr :  std_logic_vector(17 downto 0);
	SIGNAL sram_dat :  std_logic_vector(15 downto 0);
	SIGNAL sram_wrt :  std_logic;
	SIGNAL sram_sel :  std_logic;
	signal read_enable: std_logic;
	
	signal sram_adr_i: std_logic_vector(17 downto 0):= (others =>'0');

BEGIN

	-- Instantiate the Unit Under Test (UUT)
	uut: ram_write PORT MAP(
		clk => clk,
		sram_adr => sram_adr,
		sram_dat => sram_dat,
		sram_wrt => sram_wrt,
		sram_sel => sram_sel,
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
		read_enable => read_enable,
		cpu_enable => cpu_enable
	);

process begin
		clk <= '0';
		wait for 10 ns;
		clk <= '1';
		wait for 10 ns; 
end process;

process (clk) begin
	if rising_edge (clk) then
		if ad_wr = '0' then ad_wr <= '1';
		else ad_wr <= '0';
		end if;
		
		if ad_wr = '0' then
		sram_adr_i <= sram_adr_i + 1;
		end if;
		
		ad_dat <= sram_adr_i (17 downto 10) & sram_adr_i (8 downto 1);
		ad_adr <= sram_adr_i;
	end if;
end process;

END;
