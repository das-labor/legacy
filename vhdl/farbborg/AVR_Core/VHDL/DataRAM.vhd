--************************************************************************************************
-- Data RAM(behavioural) for AVR microcontroller (for simulation)
-- Version 0.1
-- Designed by Ruslan Lepetenok 
-- Modified 02.11.2002
--************************************************************************************************

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.std_logic_arith.all;

use WORK.AVRuCPackage.all;

entity DataRAM is generic(RAMSize : positive := 128);
	              port(
                        cp2     : in std_logic;
  	                    address : in  std_logic_vector (LOG2(RAMSize)-1 downto 0);
                        ramwe   : in  std_logic;
		                din     : in  std_logic_vector (7 downto 0);
		                dout    : out	std_logic_vector (7 downto 0)
					   );


end DataRAM;

architecture Beh of DataRAM is
type RAMFileType is array(RAMSize-1 downto 0) of std_logic_vector(din'range);
signal RAMFile : RAMFileType := (others => x"00"); 

begin

DataWrite:process(cp2)
begin
if cp2='1' and cp2'event then            -- Clock
 if ramwe='1' then                       -- Clock enable 
  RAMFile(CONV_INTEGER(address)) <= din;	
 end if;
end if;
end process;	

dout <= RAMFile(CONV_INTEGER(address));

end Beh;
