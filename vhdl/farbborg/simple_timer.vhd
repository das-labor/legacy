--**********************************************************************************************
--                           Simple timer Peripheral for the AVR Core
--                           Version 0.2 12.11.2001	
--                           Designed by Ruslan Lepetenok
--**********************************************************************************************
library IEEE;
use IEEE.std_logic_1164.all;
USE IEEE.std_logic_arith.all;
USE IEEE.std_logic_unsigned.all;

entity simple_timer is port (
ireset        : in std_logic;
cp2           : in std_logic;
-- INTERRUPTS PORT
irqline       : out std_logic;
timer_irqack  : in std_logic
							 );
end simple_timer;

architecture rtl of simple_timer is

signal counter     : std_logic_vector (7 downto 0) := (others=> '0');
signal irqline_int : std_logic := '0';
signal count_end   : std_logic := '0';

-- *****************************************************************************************
begin

main_counter:process(cp2,ireset)
begin
if ireset='0' then 
counter <= (others => '0');
elsif cp2='1' and cp2'event then
counter <= counter + 1;
end if;	
end process;
	
count_end  <= '1' when counter = "11111111" else '0';
	
irq_out:process(cp2,ireset)
begin
if ireset='0' then 
irqline_int <= '0';
elsif cp2='1' and cp2'event then
irqline_int <= (not irqline_int and count_end)or(irqline_int and not timer_irqack); 
end if;	
end process;

irqline <= not irqline_int;

end rtl;
