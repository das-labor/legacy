library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity TestBench is
end TestBench;

architecture Behavioral of TestBench is

constant Tclk : time := 10ns;

-----------------------------------------------------------------------------
-- Component declaration ----------------------------------------------------
component UartTest is
	port (
		clk         : in  std_logic;
		reset       : in  std_logic;
		-- 
		uart_rxd    : in  std_logic;
		uart_txd    : out std_logic );
end component;

-----------------------------------------------------------------------------

signal clk       : std_logic;
signal reset     : std_logic;
signal uart_txd  : std_logic;

begin

testee0: UartTest
	port map (
		clk       => clk,
		reset     => reset,
		--
		uart_rxd  => uart_txd,
		uart_txd  => uart_txd
	);


clkgen: process is
begin
	clk <= '1', '0' after Tclk;
	wait for 2*Tclk;
end process;

reset <= '1', '0' after 2.5*Tclk;

end Behavioral;

