library ieee;
use ieee.std_logic_1164.all;

entity Main is
	Port( clk     : in  std_logic;
          btn     : in  std_logic_vector(3 downto 0);
          led     : out std_logic_vector(7 downto 0) );
end Main;



architecture rtl of Main is
	
begin

led(7 downto 4) <= (others => '1');
led(3 downto 0) <= btn(3 downto 0);

end rtl;

