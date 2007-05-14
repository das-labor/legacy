library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.numeric_std.all;
use ieee.std_logic_textio.all;
use std.textio.all;


entity wb_sram is
   port (
		clk      : in  std_logic;
		reset		: in  std_logic;
      --
      wb_adr_i : in  std_logic_vector(23 downto 0);
      wb_dat_i : in  std_logic_vector( 7 downto 0);
      wb_dat_o : out std_logic_vector( 15 downto 0);
      wb_cyc_i : in  std_logic;
      wb_stb_i : in  std_logic;
      wb_ack_o : out std_logic;
      wb_we_i  : in  std_logic; 
      -- SRAM
      ram_ce_n : out   std_logic_vector(1 downto 0);
      ram_be_n : out   std_logic_vector(3 downto 0);
      ram_we_n : out   std_logic;
      ram_oe_n : out   std_logic;
      ram_addr : out   std_logic_vector(17 downto 0);
      ram_io   : inout std_logic_vector(31 downto 0)
		
		);
end wb_sram;


architecture rtl of wb_sram is
signal write    : std_logic;
signal wb_off   : std_logic_vector(1 downto 0);

signal addr_latch : std_logic_vector(19 downto 0);

signal ram_we_n_e : std_logic;

begin

wb_dat_o <= ram_io( 15 downto  0) when wb_off="00" else
            "00000000" & ram_io(15 downto  8) when wb_off="01" else
            ram_io(31 downto 16) when wb_off="10" else
            "00000000" & ram_io(31 downto 24) when wb_off="11";

--process (clk)
--begin
--	if reset = '1' then
--		ram_oe_n <= '1';
--		ram_we_n_e <= '1';
--	elsif clk'event and clk= '0' then
--		wb_ack_o  <= wb_cyc_i and wb_stb_i;
--		if write = '1' then
--			ram_io <= wb_dat_i & wb_dat_i & wb_dat_i & wb_dat_i;
--			ram_oe_n <= '1';
--			ram_we_n_e <= '0';
--		else
--			ram_oe_n <= '0';
--			ram_we_n_e <= '1';
--			ram_io <= (others => 'Z');
--		end if;
--	end if;
--end process;

wb_ack_o <= '1';

ram_oe_n <= '0' when write = '0' else '1';
ram_we_n_e <= '0' when write = '1' else '1';

ram_io <= wb_dat_i & wb_dat_i & wb_dat_i & wb_dat_i when write = '1' else
			(others => 'Z');

addr_latch <= wb_adr_i(19 downto 0);

ram_we_n <= ram_we_n_e;--or (clk);

write <= wb_stb_i and wb_we_i;


ram_be_n <= "0000" when write = '0' else
				"1110" when wb_off="00" else
            "1101" when wb_off="01" else
            "1011" when wb_off="10" else
            "0111" when wb_off="11" else
				"1111";

ram_ce_n(0) <= '0';
ram_ce_n(1) <= '0';

ram_addr  <= addr_latch(19 downto 2);
wb_off    <= addr_latch(1 downto 0);

end rtl;
