LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.numeric_std.ALL;

ENTITY vtb_ram_read_vhd IS
END vtb_ram_read_vhd;

ARCHITECTURE behavior OF vtb_ram_read_vhd IS 

	-- Component Declaration for the Unit Under Test (UUT)
	COMPONENT ram_read
	PORT(
		clk : IN std_logic;
		winkel : IN std_logic_vector(9 downto 0);
		write_enable : IN std_logic;          
		ram_adr : OUT std_logic_vector(17 downto 0);
		led_nr : OUT std_logic_vector(7 downto 0);
		winkel_diag : OUT std_logic_vector(9 downto 0)
		);
	END COMPONENT;

	--Inputs
	SIGNAL clk :  std_logic := '0';
	SIGNAL write_enable :  std_logic := '0';
	SIGNAL winkel :  std_logic_vector(9 downto 0) := (others=>'0');

	--Outputs
	SIGNAL ram_adr :  std_logic_vector(17 downto 0);
	SIGNAL led_nr :  std_logic_vector(7 downto 0);
	SIGNAL winkel_diag :  std_logic_vector(9 downto 0);

signal count_winkel :std_logic_vector (15 downto 0):=(others =>'0');
signal count_write  :std_logic_vector (15 downto 0):=(others =>'0');

BEGIN

	-- Instantiate the Unit Under Test (UUT)
	uut: ram_read PORT MAP(
		clk => clk,
		winkel => winkel,
		write_enable => write_enable,
		ram_adr => ram_adr,
		led_nr => led_nr,
		winkel_diag => winkel_diag
	);
process begin
		clk <= '0';
		wait for 10 ns;
		clk <= '1';
		wait for 10 ns; 
end process;

process (clk) begin
	if rising_edge (clk) then

			count_winkel <= count_winkel + 1;
	if count_winkel >= 100 then count_winkel <= (others => '0'); end if;
	if count_winkel = 100 then winkel <= winkel + 1;end if;

			count_write <= count_write + 1;
	if count_write >= 20 then count_write <= (others => '0'); end if;
	
	if count_write >= 10 then write_enable <= '1'; else write_enable <= '0'; end if;

	end if;
end process;
	
	
END;
