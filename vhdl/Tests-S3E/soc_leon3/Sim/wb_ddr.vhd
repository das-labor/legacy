library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

-----------------------------------------------------------------------------
-- 64 Bit Wishbone DDR Controller -------------------------------------------
entity wb_ddr is
	generic (
		wait200us    : in    std_logic := '1' );
	port(
		clk          : in    std_logic;
		clk_2x       : in    std_logic;
		clk_2x90     : in    std_logic;
		reset        : in    std_logic;
		-- DDR Connection
		ddr_clk      : out   std_logic;
		ddr_clk_n    : out   std_logic;
		ddr_clk_fb   : in    std_logic;
		ddr_dq       : inout std_logic_vector(15 downto 0);
		ddr_dqs      : inout std_logic_vector( 1 downto 0);
		ddr_dm       : out   std_logic_vector( 1 downto 0);
		ddr_addr     : out   std_logic_vector(13 downto 0);
		ddr_ba       : out   std_logic_vector( 1 downto 0);
		ddr_cke      : out   std_logic;
		ddr_cs_n     : out   std_logic;
		ddr_ras_n    : out   std_logic;
		ddr_cas_n    : out   std_logic;
		ddr_we_n     : out   std_logic;
		-- Whishbone Interface
		wb_adr_i     : in    std_logic_vector(31 downto 0);
		wb_dat_i     : in    std_logic_vector(63 downto 0);
		wb_dat_o     : out   std_logic_vector(63 downto 0);
		wb_sel_i     : in    std_logic_vector( 7 downto 0);
		wb_we_i      : in    std_logic;
		wb_stb_i     : in    std_logic;
		wb_cyc_i     : in    std_logic;
		wb_ack_o     : out   std_logic;
		-- Debug & Rotary 
		sw           : in    std_logic_vector(3 downto 0);
		rotary       : in    std_logic_vector(2 downto 0);
		debug        : out   std_logic_vector(7 downto 0) );
end wb_ddr;

-----------------------------------------------------------------------------
-- Implementation -----------------------------------------------------------
architecture behavioral of wb_ddr is

constant mem_size : natural := (2*1024) - 1;
type mem_type is array(0 to mem_size) of std_logic_vector(63 downto 0);

-----------------------------------------------------------------------------
-- Pre-Initialized-Data -----------------------------------------------------
signal mem : mem_type := (
    others => x"0000000000000000" 
);

signal ack : std_logic;

begin

wb_ack_o <= wb_stb_i and ack;

memproc: process (clk) is
variable a : integer;
begin
	if clk'event and clk='1' then
	if wb_stb_i='1' then
		a := to_integer(unsigned(wb_adr_i(15 downto 3)));
	
		if wb_we_i='1' then 
			if wb_sel_i(7)='1' then
				mem(a)(63 downto 56) <= wb_dat_i(63 downto 56);
			end if;
			if wb_sel_i(6)='1' then
				mem(a)(55 downto 48) <= wb_dat_i(55 downto 48);
			end if;
			if wb_sel_i(5)='1' then
				mem(a)(47 downto 40) <= wb_dat_i(47 downto 40);
			end if;
			if wb_sel_i(4)='1' then
				mem(a)(39 downto 32) <= wb_dat_i(39 downto 32);
			end if;
			if wb_sel_i(3)='1' then
				mem(a)(31 downto 24) <= wb_dat_i(31 downto 24);
			end if;
			if wb_sel_i(2)='1' then
				mem(a)(23 downto 16) <= wb_dat_i(23 downto 16);
			end if;
			if wb_sel_i(1)='1' then
				mem(a)(15 downto  8) <= wb_dat_i(15 downto  8);
			end if;
			if wb_sel_i(0)='1' then
				mem(a)( 7 downto  0) <= wb_dat_i( 7 downto  0);
			end if;
		end if;

		wb_dat_o <= mem(a);
		ack <= '1' and not ack;
	else
		ack <= '0';
	end if;
	end if;
end process;


end architecture;

