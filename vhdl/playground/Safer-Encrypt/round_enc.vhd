library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-----------------------------------------------------------------------------
-- Single Safer encryption round --------------------------------------------
entity round_enc is
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
end round_enc;

-----------------------------------------------------------------------------
-- Implementation -----------------------------------------------------------
architecture rtl of round_enc is

-----------------------------------------------------------------------------
-- Components ---------------------------------------------------------------
component STEP_1_3
      Port ( input        : in  STD_LOGIC_VECTOR ( 63 downto 0);
             key          : in 	STD_LOGIC_VECTOR ( 63 downto 0);
             output       : out STD_LOGIC_VECTOR ( 63 downto 0));
end component;

component STEP_2
      Port ( input        : in  STD_LOGIC_VECTOR ( 63 downto 0);
             key          : in 	STD_LOGIC_VECTOR ( 63 downto 0);
             output       : out STD_LOGIC_VECTOR ( 63 downto 0));
end component;

component INV_S_BOX_PERMUTATION is
   Port ( clk    : in  STD_LOGIC;
        we     : in  STD_LOGIC := '0';
		  a_in   : in  STD_LOGIC_VECTOR (7 downto 0);
		  a_out  : out STD_LOGIC_VECTOR (7 downto 0);
		  b_in   : in  STD_LOGIC_VECTOR (7 downto 0);
		  b_out  : out STD_LOGIC_VECTOR (7 downto 0));
end component;

component S_BOX_PERMUTATION is
   Port (
		clk    : in  STD_LOGIC;
      we     : in  STD_LOGIC := '0';
		a_in   : in  STD_LOGIC_VECTOR (7 downto 0);
		a_out  : out STD_LOGIC_VECTOR (7 downto 0);
		b_in   : in  STD_LOGIC_VECTOR (7 downto 0);
		b_out  : out STD_LOGIC_VECTOR (7 downto 0));
end component;

component PSEUDO_HADAMARD_TRANSFORMATION
	Port ( 
		in_left	 : in  STD_LOGIC_VECTOR ( 7 downto 0);		in_right	 : in  STD_LOGIC_VECTOR ( 7 downto 0);		out_left	 : out STD_LOGIC_VECTOR ( 7 downto 0);
		out_right : out STD_LOGIC_VECTOR ( 7 downto 0));end component;

component KEYSCHED is
	port (
		input     : in  STD_LOGIC_VECTOR(63 downto 0);
		sel       : in  STD_LOGIC_VECTOR( 3 downto 0);
		rotated   : out STD_LOGIC_VECTOR(63 downto 0);
		biased    : out STD_LOGIC_VECTOR(63 downto 0) );
end component;

-----------------------------------------------------------------------------
-- Local Signals ------------------------------------------------------------

-- Data path --
signal data0  : std_logic_vector(63 downto 0);
signal data1  : std_logic_vector(63 downto 0);
signal data2  : std_logic_vector(63 downto 0);
signal data3  : std_logic_vector(63 downto 0);
signal data4  : std_logic_vector(63 downto 0);
signal data5  : std_logic_vector(63 downto 0);
signal data6  : std_logic_vector(63 downto 0);
signal data7  : std_logic_vector(63 downto 0);
signal data8  : std_logic_vector(63 downto 0);
signal data9  : std_logic_vector(63 downto 0);
signal data10 : std_logic_vector(63 downto 0);

-- Key path --
signal key0   : std_logic_vector(63 downto 0);
signal key1   : std_logic_vector(63 downto 0);
signal key2   : std_logic_vector(63 downto 0);
signal key3   : std_logic_vector(63 downto 0);
signal key4   : std_logic_vector(63 downto 0);
signal key5   : std_logic_vector(63 downto 0);
signal key6   : std_logic_vector(63 downto 0);
signal key7   : std_logic_vector(63 downto 0);

-- Ctrl Path --
signal ctrl0   : std_logic_vector( 3 downto 0);
signal ctrl1   : std_logic_vector( 3 downto 0);
signal ctrl2   : std_logic_vector( 3 downto 0);
signal ctrl3   : std_logic_vector( 3 downto 0);
signal ctrl4   : std_logic_vector( 3 downto 0);
signal ctrl5   : std_logic_vector( 3 downto 0);

-- Actual Keys
signal bkey0   : std_logic_vector(63 downto 0);
signal bkey1   : std_logic_vector(63 downto 0);

signal bkey0_u : std_logic_vector(63 downto 0);
signal bkey1_u : std_logic_vector(63 downto 0);


begin

-----------------------------------------------------------------------------
-- Data Path ----------------------------------------------------------------
step13: STEP_1_3
	Port map ( input => data0, key => bkey0, output => data1 );

sbox0: S_BOX_PERMUTATION 		Port map ( 
		              clk   => clk,
						  we    => '0',						  
		              a_in  => data2( 7 downto  0), 
						  a_out => data3( 7 downto  0),
						  b_in  => data2(31 downto 24), 
						  b_out => data3(31 downto 24));
					 
sbox1: INV_S_BOX_PERMUTATION Port map (
		              clk   => clk,
						  we    => '0',						  
						  a_in  => data2(15 downto  8),
						  a_out => data3(15 downto  8),
						  b_in  => data2(23 downto 16),
						  b_out => data3(23 downto 16));
		
sbox2: S_BOX_PERMUTATION 	 	Port map ( 
						  clk   => clk,
						  we    => '0',
						  a_in  => data2(39 downto 32),
						  a_out => data3(39 downto 32),
						  b_in  => data2(63 downto 56),
						  b_out => data3(63 downto 56));
		
sbox3: INV_S_BOX_PERMUTATION Port map ( 
						  clk   => clk,
						  we    => '0',
						  a_in  => data2(47 downto 40),
						  a_out => data3(47 downto 40),
						  b_in  => data2(55 downto 48),
						  b_out => data3(55 downto 48));
						  
step2: STEP_2 Port map ( input => data3, key => bkey1, output => data4 );

-- This is the first round of the pseudo hadamard transform 
hadamard1 : PSEUDO_HADAMARD_TRANSFORMATION Port map ( data5(63 downto 56), data5(55 downto 48), data6(63 downto 56), data6(31 downto 24));
hadamard2 : PSEUDO_HADAMARD_TRANSFORMATION Port map ( data5(47 downto 40), data5(39 downto 32), data6(55 downto 48), data6(23 downto 16));
hadamard3 : PSEUDO_HADAMARD_TRANSFORMATION Port map ( data5(31 downto 24), data5(23 downto 16), data6(47 downto 40), data6(15 downto  8));
hadamard4 : PSEUDO_HADAMARD_TRANSFORMATION Port map ( data5(15 downto  8), data5( 7 downto  0), data6(39 downto 32), data6( 7 downto  0));

-- This is the second round of the pseudo hadamard transform
hadamard5 : PSEUDO_HADAMARD_TRANSFORMATION Port map ( data7(63 downto 56), data7(55 downto 48), data8(63 downto 56), data8(31 downto 24));
hadamard6 : PSEUDO_HADAMARD_TRANSFORMATION Port map ( data7(47 downto 40), data7(39 downto 32), data8(55 downto 48), data8(23 downto 16));
hadamard7 : PSEUDO_HADAMARD_TRANSFORMATION Port map ( data7(31 downto 24), data7(23 downto 16), data8(47 downto 40), data8(15 downto  8));
hadamard8 : PSEUDO_HADAMARD_TRANSFORMATION Port map ( data7(15 downto  8), data7( 7 downto  0), data8(39 downto 32), data8( 7 downto  0));

-- This is the third round of the pseudo hadamard transform
hadamard9 : PSEUDO_HADAMARD_TRANSFORMATION Port map ( data9(63 downto 56), data9(55 downto 48), data10(63 downto 56), data10(55 downto 48));
hadamard10: PSEUDO_HADAMARD_TRANSFORMATION Port map ( data9(47 downto 40), data9(39 downto 32), data10(47 downto 40), data10(39 downto 32));
hadamard11: PSEUDO_HADAMARD_TRANSFORMATION Port map ( data9(31 downto 24), data9(23 downto 16), data10(31 downto 24), data10(23 downto 16));
hadamard12: PSEUDO_HADAMARD_TRANSFORMATION Port map ( data9(15 downto  8), data9( 7 downto  0), data10(15 downto  8), data10( 7 downto  0));

-----------------------------------------------------------------------------
-- Key Path -----------------------------------------------------------------
sched1: keysched
	port map (
		input   => key_in,
		sel     => ctrl_in,		
		rotated => key0,
		biased  => bkey0_u );

sched2: keysched
	port map (
		input   => key2,
		sel     => ctrl1,		
		rotated => key3,
		biased  => bkey1_u );

-----------------------------------------------------------------------------
-- Resgisters ---------------------------------------------------------------
regs: process(clk, rst) is
begin
	if clk'event and clk='1' then 
		-- data path
		data0    <= data_in;
		data2    <= data1;
		data5    <= data4;
		data7    <= data6;
		data9    <= data8;
		data_out <= data10;
		
		-- key path
		key1     <= key0;
		key2     <= key1;
		key4     <= key3;
		key5     <= key4;		
		key6     <= key5;		
		key7     <= key6;
		key_out  <= key7;

		bkey0    <= bkey0_u;
		bkey1    <= bkey1_u;
		
		-- ctrl path
		ctrl0    <= ctrl_in;
		ctrl1    <= ctrl0 + 1;
		ctrl2    <= ctrl1 + 1;
		ctrl3    <= ctrl2;
		ctrl4    <= ctrl3;
		ctrl5    <= ctrl4;
		ctrl_out <= ctrl5;
	end if;
end process;

end rtl;
