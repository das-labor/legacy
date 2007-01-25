-----------------------------------------------------------------------------
-- Testbench for WBLeon3 ----------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use work.config.all;

entity TestWBLeon is
end entity;

-----------------------------------------------------------------------------
-- Implementation -----------------------------------------------------------
architecture rtl of TestWBLeon is

constant Tclk : time := 10 ns;

-- System signals
signal clk      : std_logic;
signal reset    : std_logic;

-- DDR Signals
signal ddr_clk   : std_logic;
signal ddr_clk_n : std_logic;
signal ddr_dq    : std_logic_vector(15 downto 0);
signal ddr_dqs   : std_logic_vector( 1 downto 0);
signal ddr_addr  : std_logic_vector(12 downto 0);
signal ddr_ba    : std_logic_vector( 1 downto 0);
signal ddr_dm    : std_logic_vector( 1 downto 0);
signal ddr_cke   : std_logic;
signal ddr_cs_n  : std_logic;
signal ddr_ras_n : std_logic;
signal ddr_cas_n : std_logic;
signal ddr_we_n  : std_logic;

-- UART
signal uart_rx   : std_logic;
signal uart_tx   : std_logic;

-- I/O Ports
signal led      : std_logic_vector(7 downto 0);

begin

-----------------------------------------------------------------------------
-- LEON3 System -------------------------------------------------------------
system0: entity work.System
	port map (
		clk_in      => clk,
		reset_in    => reset,
		-- I/O Ports
		sw          => "0000",
		rotary      => "000",
		led         => led,
		-- UART 
		uart_rx     => uart_tx,
		uart_tx     => uart_tx,
		-- DDR connection
		ddr_clk    => ddr_clk,
		ddr_clk_n  => ddr_clk_n,
		ddr_clk_fb => ddr_clk,
		ddr_dq     => ddr_dq,
		ddr_dqs    => ddr_dqs,
		ddr_dm     => ddr_dm,
		ddr_addr   => ddr_addr,
		ddr_ba     => ddr_ba,
		ddr_cke    => ddr_cke,
		ddr_cs_n   => ddr_cs_n,
		ddr_ras_n  => ddr_ras_n,
		ddr_cas_n  => ddr_cas_n,
		ddr_we_n   => ddr_we_n );

-----------------------------------------------------------------------------
-- CLK and RESET ------------------------------------------------------------
clkproc: process is
begin
	clk <= '1', '0' after Tclk;
	wait for 2*Tclk;
end process;

reset <= '1', '0' after 10.5*Tclk;

end architecture;
