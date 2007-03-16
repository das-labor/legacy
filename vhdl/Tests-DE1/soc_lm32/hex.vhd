
------------------------------------------------------------------------
-- Hexausgabe ----------------------------------------------------------
------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity nibble7Seg is
   port (
      nibble0  : in  std_logic_vector(3 downto 0);
	  nibble1  : in  std_logic_vector(3 downto 0);
	  sel      : in  std_logic;
	  hex      : out std_logic_vector(6 downto 0)	
        );
end nibble7Seg;

architecture rtl of nibble7Seg is
signal dig  : std_logic_vector(3 downto 0);
begin

dig <= nibble0 when sel='0' else
       nibble1;

hex <= "1000000" when dig="0000" else
	   "1111001" when dig="0001" else
	   "0100100" when dig="0010" else
	   "0110000" when dig="0011" else
	   "0011001" when dig="0100" else
	   "0010010" when dig="0101" else
	   "0000010" when dig="0110" else
	   "1111000" when dig="0111" else
	   "0000000" when dig="1000" else
	   "0010000" when dig="1001" else
	   "0001000" when dig="1010" else
	   "0000011" when dig="1011" else
	   "1000110" when dig="1100" else
	   "0100001" when dig="1101" else
	   "0000110" when dig="1110" else
	   "0001110" when dig="1111";

--process is
--begin
--	case digit is
--		when "0000" => hex <= "1000000";
--		when "0001" => hex <= "1111001";
--		when "0010" => hex <= "0100100";
--		when "0011" => hex <= "0110000";
--		when "0100" => hex <= "0011001";
--		when "0101" => hex <= "0010010";
--		when "0110" => hex <= "0000010";
--		when "0111" => hex <= "1111000";
--		when "1000" => hex <= "0000000";
--		when "1001" => hex <= "0011000";
--		when "1010" => hex <= "0001000";
--		when "1011" => hex <= "0000011";
--		when "1100" => hex <= "1000110";
--		when "1101" => hex <= "0100001";
--		when "1110" => hex <= "0000110";
--		when "1111" => hex <= "0001110";
--	end case;	
--end process;

end rtl;

