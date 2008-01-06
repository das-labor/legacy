library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity TestBench is
end TestBench;

architecture Behavioral of TestBench is

constant Tclk : time := 10 ns;

-----------------------------------------------------------------------------
-- Components ---------------------------------------------------------------
-----------------------------------------------------------------------------
component System is
	Port (
		clk      : in  std_logic;
		reset    : in  std_logic;
		-- 
		leds     : out std_logic_vector(7 downto 0);
		hex0     : out std_logic_vector(6 downto 0);
		hex1     : out std_logic_vector(6 downto 0);
		hex2     : out std_logic_vector(6 downto 0);
		hex3     : out std_logic_vector(6 downto 0);
		btn      : in  std_logic_vector(9 downto 0);
		uart_rx  : in  std_logic;
		uart_tx  : out std_logic;
		
		sram_oe_n : out std_logic;
	  	sram_we_n : out std_logic;
	  	sram_ub_n : out std_logic;
	  	sram_lb_n : out std_logic;
	   sram_ce_n : out std_logic;
	  	sram_addr : out std_logic_vector(17 downto 0);
	   sram_dq   : inout std_logic_vector(15 downto 0)
	);
end component System;



-----------------------------------------------------------------------------
-- Local Signals
-----------------------------------------------------------------------------
signal clk       : std_logic;
signal reset     : std_logic;

signal leds      : std_logic_vector(7 downto 0);
signal hex0      : std_logic_vector(6 downto 0);
signal hex1      : std_logic_vector(6 downto 0);
signal hex2      : std_logic_vector(6 downto 0);
signal hex3      : std_logic_vector(6 downto 0);
signal btn       : std_logic_vector(9 downto 0);

signal uart_rx   : std_logic;
signal uart_tx   : std_logic;
		
signal sram_oe_n : std_logic;
signal sram_we_n : std_logic;
signal sram_ub_n : std_logic;
signal sram_lb_n : std_logic;
signal sram_ce_n : std_logic;
signal sram_addr : std_logic_vector(17 downto 0);
signal sram_dq   : std_logic_vector(15 downto 0);

-----------------------------------------------------------------------------
-- Implementation -----------------------------------------------------------
-----------------------------------------------------------------------------
begin

soc0: System
	Port map (
		clk      => clk,
		reset    => reset,
		--
		leds     => leds,
		hex0     => hex0,
		hex1     => hex1,
		hex2     => hex2,
		hex3     => hex3,
		btn      => btn,
		uart_rx  => uart_rx,
		uart_tx  => uart_tx,
		--
		sram_oe_n => sram_oe_n,
	  	sram_we_n => sram_we_n,
	  	sram_ub_n => sram_ub_n,
	  	sram_lb_n => sram_lb_n,
	   sram_ce_n => sram_ce_n,
	  	sram_addr => sram_addr,
	   sram_dq   => sram_dq
	);

-----------------------------------------------------------------------------
-- Clock and Reset generation -----------------------------------------------
-----------------------------------------------------------------------------
clkproc: process is
begin
	clk <= '1', '0' after Tclk/2;
	wait for Tclk;
end process;

reset <= '1', '0' after 3.5*Tclk;

btn <= (others => '0');

end Behavioral;

