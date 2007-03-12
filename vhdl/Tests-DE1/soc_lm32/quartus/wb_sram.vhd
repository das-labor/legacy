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
      -- Pins für das SRAM
	  sram_oe_n : out std_logic;
	  sram_we_n : out std_logic;
	  sram_ub_n : out std_logic;
	  sram_lb_n : out std_logic;
	  sram_addr : out std_logic_vector(18 downto 0);
	  sram_dq   : inout std_logic_vector(15 downto 0)
		);
end wb_sram;

architecture rtl of wb_sram is

signal high_word : std_logic;
signal active    : std_logic;

begin	

SRAM_WE_N <= not (wb_we_i and wb_cyc_i and wb_stb_i);

sram_oe_n <= not (not wb_we_i and wb_cyc_i and wb_stb_i); 
 
sram_addr <= wb_adr_i(17 downto 0) & high_word;

sram_dq <= wb_dat_i(31 downto 16) when high_word='1' and wb_we_i='1' else
           wb_dat_i(15 downto 0)  when high_word='0' and wb_we_i='1' else 
           (others => 'Z');

sram_ub_n <= not wb_sel_i(1) when high_word='0' else
             not wb_sel_i(3) when high_word='1';
             
sram_lb_n <= not wb_sel_i(0) when high_word='0' else
             not wb_sel_i(2) when high_word='1';

read2words: process(clk) is
begin
	if reset='1' then
		active <= '0';
		high_word <= '0';
	elsif clk'event and clk='1' then
		if active='0' and wb_stb_i='1' and wb_cyc_i='1' and high_word='0' then
			active <= '1';
		elsif active='1' and high_word='0' then
			high_word <= '1';
			wb_dat_o(15 downto 0) <= sram_dq;
		elsif active='1' and high_word='1' then
			active <= '0';
			wb_dat_o(31 downto 16) <= sram_dq;	
			wb_ack_o <= '1';
		elsif active='0' and high_word='1' then
			wb_ack_o <= '0';
			high_word <= '0';
		end if;
	end if;
end process;	
end rtl;
