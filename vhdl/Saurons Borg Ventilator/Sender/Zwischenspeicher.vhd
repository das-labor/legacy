library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity zwischenspeicher is
    Port ( sram_read : in  STD_LOGIC_VECTOR (15 downto 0);
           sram_pos :  in  STD_LOGIC_VECTOR ( 7 downto 0));
end zwischenspeicher;

architecture Behavioral of zwischenspeicher is


component frame_full
	port (
	addra  : IN  std_logic_VECTOR( 8 downto 0);
	addrb  : IN  std_logic_VECTOR( 7 downto 0);
	clka   : IN  std_logic;
	clkb   : IN  std_logic;
	dinb   : IN  std_logic_VECTOR(15 downto 0);
	douta  : OUT std_logic_VECTOR( 7 downto 0);
	enb    : IN  std_logic;
	web    : IN  std_logic     );
end component;





signal addrb        : std_logic_vector(11 downto 0); 
signal dinb         : std_logic_vector(15 downto 0);
signal framecount_b : std_logic_vector( 3 downto 0)

begin

bram1 : frame_full
port map (
			addra => addra,
			addrb => addrb,
			clka  => clka,
			clkb  => clkb,
			dinb  => dinb,
			douta => douta,
			enb   => enb,
			web   => web       );



 



end Behavioral;

