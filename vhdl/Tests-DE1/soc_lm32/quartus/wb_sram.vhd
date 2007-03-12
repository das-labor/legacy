-----------------------------------------------------------------------------
-- Wishbone GPIO ------------------------------------------------------------
-----------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.ALL;

entity wb_sram is
	port(
	  clk      : in  std_logic;
      reset    : in  std_logic;
      -- Wishbone bus
      wb_adr_i : in  std_logic_vector(31 downto 0);
      wb_dat_i : in  std_logic_vector(31 downto 0);
      wb_dat_o : out std_logic_vector(31 downto 0);
      wb_sel_i : in  std_logic_vector( 3 downto 0);
      wb_cyc_i : in  std_logic;
      wb_stb_i : in  std_logic;
      wb_ack_o : out std_logic;
      wb_we_i  : in  std_logic;
      -- I/O ports
      iport    : in  std_logic_vector(31 downto 0);
      oport    : out std_logic_vector(31 downto 0) );
		);
end wb_sram;


---
--
architecture rtl of wb_sram

variable data_o    : std_logic_vector(31 downto 0);
variable high_word : std_logic;

begin	

SRAM_WE_N <= wb_we_i; 
SRAM_ADDR <= wb_adr_i(18 downto 0) & high_byte;

read2words: process(clk) is
begin
	if clk'event and clk='1' then
		if (wb_we_i='0') then
			SRAM_OE_N <= '1';
			if (high_byte='0') then -- read low byte
 				wb_dat_i(15 downto 0) <= SRAM_DQ; 
				high_byte <= '1';
			else -- read high byte
 				wb_dat_i(31 downto 16) <= SRAM_DQ; 
				high_byte <= '0';	
			end if;
		else 
			
			if (high_byte='0') then -- write low byte
 				
			else -- write high byte
			
			end if;
		end if;
	end if;

end process;	
end rtl;
