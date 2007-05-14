library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;



library UNISIM;
use UNISIM.VComponents.all;

entity ClkGen is
   port (
      reset    : in std_logic;
      locked   : out std_logic;
      clk50    : in std_logic;
		clk16_out: out std_logic;
		clk32_out: out std_logic;
		clk32_np_out : out std_logic;
		pause : in std_logic;
		switch : in std_logic_vector(2 downto 0)
    );
end ClkGen;

architecture Behavioral of ClkGen is

component DCM
    generic (
              DLL_FREQUENCY_MODE    : string  := "LOW";
              DUTY_CYCLE_CORRECTION : boolean := TRUE;
              CLK_FEEDBACK          : string  := "None";
              CLKFX_MULTIPLY        : integer := 16;
              CLKFX_DIVIDE          : integer := 25;
				  CLKIN_PERIOD				: string := "20"
            );
    port ( CLKIN     : in  std_logic;
           CLKFB     : in  std_logic;
           PSINCDEC  : in  std_logic;
           PSEN      : in  std_logic;
           PSCLK     : in  std_logic;
           RST       : in  std_logic;
           CLK0      : out std_logic;
           CLK90     : out std_logic;
           CLK180    : out std_logic;
           CLK270    : out std_logic;
           CLK2X     : out std_logic;
           CLK2X180  : out std_logic;
           CLKDV     : out std_logic;
           CLKFX     : out std_logic;
           CLKFX180  : out std_logic;
           LOCKED    : out std_logic;
           PSDONE    : out std_logic;
           STATUS    : out std_logic_vector(7 downto 0)
          );
end component;
signal GND     : std_logic;

signal clk32_pre   :std_logic;
signal clk_prescaler : std_logic_vector(20 downto 0);

signal clk32   :std_logic;
signal clk16	:std_logic;

begin

clk16_out <= clk16;
clk32_out <= clk32_pre;

clk32_np_out <= clk32;

GND <= '0';

dcm0: dcm
   port map(
      CLKIN     => clk50,
      CLKFB     => GND,
      PSINCDEC  => GND,
      PSEN      => GND,
      PSCLK     => GND,
      RST       => reset,
      CLKFX     => clk32,
      LOCKED    => locked );


with switch select
		clk32_pre <=clk32 					when "111",
						clk_prescaler(20) 	when "000",
						clk_prescaler(19)	when "001",
						clk_prescaler(15)	when "010",
						clk_prescaler(9)	when "011",
						clk_prescaler(5)	when "100",
						clk_prescaler(1)	when "101",
						clk_prescaler(0)	when "110",
						'0'					when others;

process(clk32, reset, pause)
begin
	if reset = '1' then
		clk_prescaler <= (others => '0');
	elsif clk32'event and clk32 = '1' then
		if pause = '0' then
			clk_prescaler <= clk_prescaler + 1;
		end if;
	end if;
end process;



clkproc1: process(clk32_pre, reset) is
begin
   if reset='1' then
     clk16 <= '0';
   elsif clk32_pre'event and clk32_pre='0' then 
      clk16 <= not clk16;
   end if;
end process;


end Behavioral;

