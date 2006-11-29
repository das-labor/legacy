-----------------------------------------------------------------------------
-- IRQ Controller for wb_leon -----------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
library gaisler;
use gaisler.leon3.all;

entity irqctrl is
	port (
		clk     : in  std_logic;
		reset   : in  std_logic;
		-- IRQ Lines
		irq     : in  std_logic_vector(15 downto 0);
		-- IRQ Protocol to Leon3
		irqi    : out l3_irq_in_type;
		irqo    : in  l3_irq_out_type );
end entity;

-----------------------------------------------------------------------------
-- Implementation -----------------------------------------------------------
architecture rtl of irqctrl is

signal  acked : std_logic_vector(15 downto 0);
signal  sirl  : std_logic_vector(3 downto 0);
begin

irqproc: process(reset, clk)
variable irl : unsigned(3 downto 0);
begin
	if reset='1' then
		acked <= (others => '0');
		irqi.rst <= '1';
		irqi.run <= '0';
		irqi.irl <= (others => '0');
	elsif clk'event and clk='1' then
		if irqo.intack='1' then
			acked( to_integer(unsigned(irqo.irl) ) ) <= '1';
		end if;

		irl := to_unsigned(0, 4);
		for i in 15 downto 0 loop
			if irq(i)='0' then
				acked(i) <= '0';
			end if;

			if irq(i)='1' and acked(i)='0' then
				irl := to_unsigned(i, 4);
			end if;
		end loop;
		irqi.rst <= '0';
		irqi.irl <= std_logic_vector(irl);
		sirl <= std_logic_vector(irl);
	end if;
end process;



end architecture;
