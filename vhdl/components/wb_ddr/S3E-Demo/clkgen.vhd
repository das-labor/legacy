library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VComponents.all;

entity clkgen is
	port(
		clk_in   : in  std_logic;
		reset    : in  std_logic;
		clk      : out std_logic;
		clk_2x   : out std_logic;      
		clk_2x90 : out std_logic;
		locked   : out std_logic );
end ClkGen;

architecture rtl of clkgen is

component DCM
-- xxxpragma translate_off
    generic ( 
              DLL_FREQUENCY_MODE    : string := "LOW";
              CLK_FEEDBACK          : string := "1X";				  
              DUTY_CYCLE_CORRECTION : boolean := TRUE
            );  
-- xxxpragma translate_on
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


component BUFG
  port ( I : in std_logic;
         O : out std_logic);
end component;

signal gnd : std_logic;

signal locked0    : std_logic;
signal reset1     : std_logic;

signal clk_1xu  : std_logic;
signal clk_1xb  : std_logic;
signal clk_2x_int_u  : std_logic;
signal clk_2x_int_b  : std_logic;

signal clk_2xu      : std_logic;
signal clk_2xb      : std_logic;
signal clk_2x90u    : std_logic;
signal clk_2x90b    : std_logic;

begin

gnd <= '0';

dcm0: DCM
	generic map (
      CLK_FEEDBACK  => "2X" )
	port map(
		CLKIN    => clk_in,
		CLKFB    => clk_2x_int_b,
		RST      => reset,
		PSINCDEC => clk_1xu,
		PSEN     => gnd,
		PSCLK    => gnd,
		CLK0     => clk_1xu,
		CLK90    => open,
		CLK180   => open,
		CLK270   => open,
		CLK2X    => clk_2x_int_u,
		CLK2X180 => open,
		CLKDV    => open,
		CLKFX    => open,
		CLKFX180 => open,
		LOCKED   => locked0,
		PSDONE   => open,
		STATUS   => open );

bufg2x_int: BUFG
	port map(
		I => clk_2x_int_u,
		O => clk_2x_int_b );

bufg1x: BUFG
	port map(
		I => clk_1xu,
		O => clk_1xb );


reset1 <= reset or not locked0;

dcm1: DCM
	port map(
		CLKIN    => clk_2x_int_b,
		CLKFB    => clk_2xb,
		RST      => reset1,
		PSINCDEC => gnd,
		PSEN     => gnd,
		PSCLK    => gnd,
		CLK0     => clk_2xu,
		CLK90    => clk_2x90u,
		CLK180   => open,
		CLK270   => open,
		CLK2X    => open,
		CLK2X180 => open,
		CLKDV    => open,
		CLKFX    => open,
		CLKFX180 => open,
		LOCKED   => locked,
		PSDONE   => open,
		STATUS   => open );
		
bufg2x: BUFG
	port map(
		I => clk_2xu,
		O => clk_2xb );

bufg2x90: BUFG
	port map(
		I => clk_2x90u,
		O => clk_2x90b );

clk      <= clk_1xb;
clk_2x   <= clk_2xb;
clk_2x90 <= clk_2x90b;	

end rtl;

