library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity Testbench is
end Testbench;

-----------------------------------------------------------------------------
-- Testbench implementation -------------------------------------------------
architecture Behavioral of Testbench is

component saver_enc is
	port (
		clk     : in  std_logic;
		rst     : in  std_logic;
		--
		key     : in  std_logic_vector(31 downto 0);
		data_in : in  std_logic_vector(63 downto 0);
		data_out: out std_logic_vector(63 downto 0)
	);
end component saver_enc;

constant Tclk : time := 10ns;

-----------------------------------------------------------------------------
-- Local signals ------------------------------------------------------------

signal clk      : std_logic;
signal rst      : std_logic;

signal key      : std_logic_vector(31 downto 0);
signal data_in  : std_logic_vector(63 downto 0);
signal data_out : std_logic_vector(63 downto 0);

begin

key     <= (others => '0');
data_in <= (others => '0');


safer: saver_enc
	port map (
		clk      => clk,
		rst      => rst,
		key      => key,
		data_in  => data_in,
		data_out => data_out );


-----------------------------------------------------------------------------
-- CLK and RST renerator ----------------------------------------------------
rst <= '1', '0' after 3.5*Tclk;

clkproc: process is
begin
	clk <= '1', '0' after Tclk;
	wait for 2*Tclk;
end process;

end Behavioral;

