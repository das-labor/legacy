library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Testbench is
end Testbench;

-----------------------------------------------------------------------------
-- Testbench implementation -------------------------------------------------
architecture Behavioral of Testbench is

constant Tclk : time := 10ns;   -- 100 MHz



component safer_enc is
	port (
		clk     : in  std_logic;
		rst     : in  std_logic;
		--
		key     : in  std_logic_vector(63 downto 0);
		data_in : in  std_logic_vector(63 downto 0);
		data_out: out std_logic_vector(63 downto 0)
	);
end component safer_enc;


-----------------------------------------------------------------------------
-- Local signals ------------------------------------------------------------
signal clk      : std_logic;
signal rst      : std_logic;

signal key      : std_logic_vector(63 downto 0);
signal data_in  : std_logic_vector(63 downto 0);
signal data_out : std_logic_vector(63 downto 0);

begin

-----------------------------------------------------------------------------
-- Unit Under Test ----------------------------------------------------------
key     <= x"0807060504030201";
data_in <= x"0102030405060708";

safer: safer_enc
	port map (
		clk      => clk,
		rst      => rst,
		key      => key,
		data_in  => data_in,
		data_out => data_out );

-----------------------------------------------------------------------------
-- CLK and RST renerator ----------------------------------------------------
rst <= '1', '0' after 2.5*Tclk;

clkproc: process is
begin
	clk <= '1', '0' after Tclk/2;
	wait for Tclk;
end process;

end Behavioral;

