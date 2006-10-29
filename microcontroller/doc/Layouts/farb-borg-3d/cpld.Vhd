library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;



entity CPLD is
       port (
            cpldclk     : in  std_logic;
            reset       : in  std_logic;
            encpld      : in  std_logic;
            -- To LED and shifters
            a           : out std_logic_vector(13 downto 0);
            planes      : out std_logic_vector( 4 downto 0);
            ShiftClk    : out std_logic;
            ShiftOutClk : out std_logic;

end entity;


architecture rtl of CPLD is
signal clk;
signal planes_reg : std_logic_vector(4 downto 0);
signal shiftindex : unsigned(3 downto 0);

signal enshifter  : std_logic;

begin

clk <= cpldclk;

-----------------------------------------------------------------------------
-- generate a output --------------------------------------------------------
a(3 downto 0) <= std_logic_vector(shiftindex) when encpld='1' else
                 (others => 'Z');
a(



shiters: process(reset, clk) is
begin
     if reset='1' then
        shiftindex <= 0;
     elsif clk'event and clk='1' then
        if enshifter='1' then
           shiftindex <= shiftindex + 1;
        else
           shiftindex <= 0;
        end if;

        if

     end if;
end process;



----------------------------------------------------------------------------
-- PLANES SHIFT REGISTER ---------------------------------------------------
planes <= planes_reg;

planesproc: process( planesclk, reset)      -- XXX planesclk generieren XXX
begin
     if reset='1' then
        planes_reg <= "10000";
     elsif planesclk'event and planesclk='1' then
        planes_reg(3 downto 0) <= planes_reg(4 downto 1);
        planes_reg(4) <= planes(0);
     end if;
end;

end architecture;

