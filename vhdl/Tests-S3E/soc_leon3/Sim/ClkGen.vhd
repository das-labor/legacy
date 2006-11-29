library ieee;
use ieee.std_logic_1164.ALL;

-----------------------------------------------------------------------------
-- Simulated Clock Generator ------------------------------------------------
entity ClkGen is
	port(
		clk_in   : in  std_logic;
		reset    : in  std_logic;
		clk      : out std_logic;
		clk_2x   : out std_logic;      
		clk_2x90 : out std_logic;
		locked   : out std_logic );
end ClkGen;

-----------------------------------------------------------------------------
-- Implementation -----------------------------------------------------------
architecture Behavioral of ClkGen is

constant Tclk : time := 20 ns;

begin

clkproc: process is
begin
	clk <= '1', '0' after Tclk/2;
	wait for Tclk;
end process;

clk_2xproc: process is
begin
	clk_2x <= '1', '0' after Tclk/4;
	wait for Tclk/2;
end process;

clk_2x90proc: process is
begin
	wait for Tclk/8;
	clk_2x90 <= '1', '0' after Tclk/4;
	wait for 3*Tclk/8;
end process;

locked <= not reset;

end Behavioral;

