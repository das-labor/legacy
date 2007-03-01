-- Input HEX file name : uart.hex
library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;

entity prom is port (
address_in : in  std_logic_vector (15 downto 0);
data_out   : out std_logic_vector (15 downto 0));
end prom;

architecture rtl of prom is
begin
data_out <= 
             x"C02F" when address_in = 16#0000# else
             x"9518" when address_in = 16#0002# else
             x"9518" when address_in = 16#0003# else
             x"9518" when address_in = 16#0004# else
             x"9518" when address_in = 16#0005# else
             x"9518" when address_in = 16#0006# else
             x"9518" when address_in = 16#0007# else
             x"9518" when address_in = 16#0008# else
             x"9518" when address_in = 16#0009# else
             x"9518" when address_in = 16#000A# else
             x"9518" when address_in = 16#000B# else
             x"9518" when address_in = 16#000C# else
             x"9518" when address_in = 16#000D# else
             x"9518" when address_in = 16#000E# else
             x"9518" when address_in = 16#000F# else
             x"9518" when address_in = 16#0010# else
             x"9518" when address_in = 16#0011# else
             x"9518" when address_in = 16#0012# else
             x"9518" when address_in = 16#0013# else
             x"9518" when address_in = 16#0014# else
             x"9518" when address_in = 16#0015# else
             x"9518" when address_in = 16#0016# else
             x"9518" when address_in = 16#0017# else
             x"9518" when address_in = 16#0018# else
             x"9518" when address_in = 16#0019# else
             x"9518" when address_in = 16#001A# else
             x"9518" when address_in = 16#001B# else
             x"9518" when address_in = 16#001C# else
             x"9518" when address_in = 16#001D# else
             x"9518" when address_in = 16#001E# else
             x"9518" when address_in = 16#001F# else
             x"9518" when address_in = 16#0020# else
             x"9518" when address_in = 16#0021# else
             x"9518" when address_in = 16#0022# else
             x"9518" when address_in = 16#0023# else
             x"940C" when address_in = 16#0024# else
             x"0040" when address_in = 16#0025# else
             x"9518" when address_in = 16#0026# else
             x"9518" when address_in = 16#0027# else
             x"9518" when address_in = 16#0028# else
             x"9518" when address_in = 16#0029# else
             x"9518" when address_in = 16#002A# else
             x"9518" when address_in = 16#002B# else
             x"9518" when address_in = 16#002C# else
             x"9518" when address_in = 16#002D# else
             x"9518" when address_in = 16#002E# else
             x"9518" when address_in = 16#002F# else
             x"EDEF" when address_in = 16#0030# else
             x"BFED" when address_in = 16#0031# else
             x"E0F0" when address_in = 16#0032# else
             x"BFEE" when address_in = 16#0033# else
             x"E000" when address_in = 16#0034# else
             x"B909" when address_in = 16#0035# else
             x"2700" when address_in = 16#0036# else
             x"BB0B" when address_in = 16#0037# else
             x"BB08" when address_in = 16#0038# else
             x"EF0F" when address_in = 16#0039# else
             x"BB0A" when address_in = 16#003A# else
             x"BB07" when address_in = 16#003B# else
             x"E900" when address_in = 16#003C# else
             x"B90A" when address_in = 16#003D# else
             x"9478" when address_in = 16#003E# else
             x"CFFF" when address_in = 16#003F# else
             x"B13C" when address_in = 16#0040# else
             x"B14B" when address_in = 16#0041# else
             x"BB3B" when address_in = 16#0042# else
             x"BB48" when address_in = 16#0043# else
             x"E031" when address_in = 16#0044# else
             x"9533" when address_in = 16#0045# else
             x"F7F1" when address_in = 16#0046# else
             x"9518" when address_in = 16#0047# else
             x"ffff";
end rtl;
