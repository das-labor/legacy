
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

---- Uncomment the following library declaration if instantiating
---- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity testbench is
  port (
      button       : in  std_logic_vector(3 downto 0);
		switch       : in  std_logic_vector(7 downto 0);
      led          : out std_logic_vector(7 downto 0);
      --Segments
      segments     : out std_logic_vector(7 downto 0);
      mux          : out std_logic_vector(3 downto 0);
		-- SRAM
      ram_ce_n : out   std_logic_vector(1 downto 0);
      ram_be_n : out   std_logic_vector(3 downto 0);
      ram_we_n : out   std_logic;
      ram_oe_n : out   std_logic;
      ram_addr : out   std_logic_vector(17 downto 0);
--      ram_io   : inout std_logic_vector(31 downto 0);		
		
		--UART
		rxd : in std_logic;
		txd : out std_logic
		--SPI
--		spi_cs : out std_logic;
--		spi_mosi : out std_logic;
--		spi_miso : in std_logic;
--		spi_sck : out std_logic;

		);
end testbench;

architecture Behavioral of testbench is

signal clk16 : std_logic;
signal clk32 : std_logic;

signal reset : std_logic;
signal reset_n : std_logic;

signal hex_out : std_logic_vector(15 downto 0);

signal led1 : std_logic_vector(7 downto 0);
signal ram_io : std_logic_vector(31 downto 0);


begin
	
	reset_n <= not reset;
	
	reset <= '1', '0' after 100ns;
	
	led <= led1;
	
	ram_io <= x"AA552233";
      
	avr: entity work.top_avr_core_sim
		port map(
					ireset => reset_n,
					cp2 => clk16,
					clk32 => clk32,
					porta => led1,
					--portb  : inout std_logic_vector(7 downto 0);
	            -- UART 
					rxd => rxd,
					txd => txd,
					-- External interrupt inputs
					nINT0 => '1',
					nINT1 => '1',
					nINT2 => '1',
					nINT3 => '1',
					INT4 => '0',
					INT5 => '0',
					INT6 => '0',
					INT7 => '0',
					addr_out => hex_out,
					
					 -- RAM
      ram_ce_n   => ram_ce_n,
      ram_be_n   => ram_be_n,
      ram_we_n   => ram_we_n,
      ram_oe_n   => ram_oe_n,
      ram_addr   => ram_addr,
      ram_io     => ram_io
								);

clkproc: process is
	begin
		clk32 <= '1', '0' after 100ns;
		wait for 200ns;
	end process;

	process (clk32, reset)
	begin
		if reset = '1' then
			clk16 <= '0';
		elsif clk32'event and clk32 = '0' then
			clk16 <= not clk16;
		end if;
	end process;
	


end Behavioral;
