library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity safer_enc is
	port (
		clk     : in  std_logic;
		rst     : in  std_logic;
		--
		key     : in  std_logic_vector(63 downto 0);
		data_in : in  std_logic_vector(63 downto 0);
		data_out: out std_logic_vector(63 downto 0)
	);
end safer_enc;

-----------------------------------------------------------------------------
-- Implementation -----------------------------------------------------------
architecture rtl of safer_enc is

-----------------------------------------------------------------------------
-- Components ---------------------------------------------------------------
component round_enc is
	port (
		clk         : in  std_logic;
		rst         : in  std_logic;		
		--
		data_in     : in  std_logic_vector(63 downto 0);
		key_in      : in  std_logic_vector(63 downto 0);
		ctrl_in     : in  std_logic_vector( 3 downto 0);
		data_out    : out std_logic_vector(63 downto 0);
		key_out     : out std_logic_vector(63 downto 0);
		ctrl_out    : out std_logic_vector( 3 downto 0)
	);
end component round_enc;

component STEP_1_3
	Port ( input 		: in  STD_LOGIC_VECTOR ( 63 downto 0);
			 key 			: in 	STD_LOGIC_VECTOR ( 63 downto 0);
			 output 		: out STD_LOGIC_VECTOR ( 63 downto 0));
end component;
	
component STEP_2
	Port ( input 		: in  STD_LOGIC_VECTOR ( 63 downto 0);
			 key 			: in 	STD_LOGIC_VECTOR ( 63 downto 0);
			 output 		: out STD_LOGIC_VECTOR ( 63 downto 0));
end component;

component KEYSCHED is
	port (
		input     : in  STD_LOGIC_VECTOR(63 downto 0);
		sel       : in  STD_LOGIC_VECTOR( 3 downto 0); 
		rotated   : out STD_LOGIC_VECTOR(63 downto 0);
		biased    : out STD_LOGIC_VECTOR(63 downto 0) );
end component;

-----------------------------------------------------------------------------
-- Local signals ------------------------------------------------------------
signal data0  : std_logic_vector(63 downto 0);
signal data1  : std_logic_vector(63 downto 0);
signal data2  : std_logic_vector(63 downto 0);
signal data3  : std_logic_vector(63 downto 0);

signal key0   : std_logic_vector(63 downto 0);
signal key1   : std_logic_vector(63 downto 0);
signal key2   : std_logic_vector(63 downto 0);
signal key3   : std_logic_vector(63 downto 0);

signal ctrl0  : std_logic_vector( 3 downto 0);
signal ctrl1  : std_logic_vector( 3 downto 0);
signal ctrl2  : std_logic_vector( 3 downto 0);
signal ctrl3  : std_logic_vector( 3 downto 0);

-- final step
signal data_f : std_logic_vector(63 downto 0);
signal bkey_f : std_logic_vector(63 downto 0);
signal bkey_fu: std_logic_vector(63 downto 0);

begin

-----------------------------------------------------------------------------
-- Muxer
data0 <= data3 when ctrl3=x"6" else
         data_in;
			
key0  <= key3  when ctrl3=x"6" else
         key;
			
ctrl0 <= ctrl3 when ctrl3=x"6" else
         x"0";  

-----------------------------------------------------------------------------
-- 3-round pipe
round0: round_enc
	port map (
		clk      => clk,
		rst      => rst,
		data_in  => data0,
		key_in   => key0,
		ctrl_in  => ctrl0,
		data_out => data1,
		key_out  => key1,
		ctrl_out => ctrl1 );

round1: round_enc
	port map (
		clk      => clk,
		rst      => rst,
		data_in  => data1,
		key_in   => key1,
		ctrl_in  => ctrl1,
		data_out => data2,
		key_out  => key2,
		ctrl_out => ctrl2 );

round2: round_enc
	port map (
		clk      => clk,
		rst      => rst,
		data_in  => data2,
		key_in   => key2,
		ctrl_in  => ctrl2,
		data_out => data3,
		key_out  => key3,
		ctrl_out => ctrl3 );

-----------------------------------------------------------------------------
-- final round
fsched: keysched
	port map (
		input   => key3,
		sel     => ctrl3,		
		rotated => open,
		biased  => bkey_fu );

finalstep: STEP_1_3 
	port map ( 
		input => data_f,
		key => bkey_f,
		output => data_out );

freg: process(clk) is
begin
	if clk='1' and clk'event then 
		data_f <= data3;
		bkey_f <= bkey_fu;
	end if;
end process;


end rtl;

