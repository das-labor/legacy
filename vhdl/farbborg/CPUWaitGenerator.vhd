--************************************************************************************************
-- Behavioural description of cpuwait generator for AVR microcontroller (for simulation)
-- Version 0.1
-- Designed by Ruslan Lepetenok 
-- Modified 02.11.2002
--************************************************************************************************
library	IEEE;
use IEEE.std_logic_1164.all;

entity CPUWaitGenerator is
			   generic(InsertWaitSt : boolean := FALSE);
	           port(
	           ireset  : in  std_logic;
			   cp2     : in  std_logic;
			   ramre   : in  std_logic;
			   ramwe   : in  std_logic;
			   cpuwait : out std_logic                 
			        );
end CPUWaitGenerator;

architecture Beh of CPUWaitGenerator is
signal cpuwait_int : std_logic := '0';
begin

cpuwaitGenerator:process(cp2,ireset)
begin
 if ireset='0' then                  -- Reset
   cpuwait_int <= '0';
   elsif cp2='0' and cp2'event then      -- Clock (falling edge)
   cpuwait_int <= not cpuwait_int and (ramre or ramwe);
   end if;
 end process;		

cpuwait <= cpuwait_int when InsertWaitSt else '0';	
	
end Beh;
