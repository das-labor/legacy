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
		btn      : in  std_logic_vector(3 downto 0);
		uart_rx  : in  std_logic;
		uart_tx  : out std_logic
	);
end component System;



-----------------------------------------------------------------------------
-- Local Signals
-----------------------------------------------------------------------------
signal clk       : std_logic;
signal reset     : std_logic;

signal btn       : std_logic_vector(3 downto 0);
signal leds      : std_logic_vector(7 downto 0);

signal ack       : std_logic;
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
		btn      => btn,
		uart_rx  => '0',
		uart_tx  => open
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

btn <= "0000";

end Behavioral;

