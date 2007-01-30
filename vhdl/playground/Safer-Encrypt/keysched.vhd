library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity keysched is
	port (
		input    : in  STD_LOGIC_VECTOR(63 downto 0);
		sel      : in  STD_LOGIC_VECTOR( 3 downto 0);		
		rotated  : out STD_LOGIC_VECTOR(63 downto 0);
		biased   : out STD_LOGIC_VECTOR(63 downto 0) );
end keysched;

-----------------------------------------------------------------------------
-- Implementation -----------------------------------------------------------
architecture rtl of keysched is

constant BIAS1 : STD_LOGIC_VECTOR(63 downto 0) := x"0000000000000000";
constant BIAS2 : STD_LOGIC_VECTOR(63 downto 0) := x"16733B1E8E70BD86";
constant BIAS3 : STD_LOGIC_VECTOR(63 downto 0) := x"477E2456F1778846";
constant BIAS4 : STD_LOGIC_VECTOR(63 downto 0) := x"B1BAA3B7100AC537";
constant BIAS5 : STD_LOGIC_VECTOR(63 downto 0) := x"C95A28AC64A5ECAB";
constant BIAS6 : STD_LOGIC_VECTOR(63 downto 0) := x"C66795580DF89AF6";
constant BIAS7 : STD_LOGIC_VECTOR(63 downto 0) := x"66DC053DD38AC3D8";
constant BIAS8 : STD_LOGIC_VECTOR(63 downto 0) := x"6AE9364943BFEBD4";	
constant BIAS9 : STD_LOGIC_VECTOR(63 downto 0) := x"9B68A0655D57921F";
constant BIAS10: STD_LOGIC_VECTOR(63 downto 0) := x"715CBB22C1BE7BBC";
constant BIAS11: STD_LOGIC_VECTOR(63 downto 0) := x"63945F2A61B83432";
constant BIAS12: STD_LOGIC_VECTOR(63 downto 0) := x"63945F2A61B83432";
constant BIAS13: STD_LOGIC_VECTOR(63 downto 0) := x"8F29DD0480DEE731";

-----------------------------------------------------------------------------
-- Local Signals ------------------------------------------------------------
signal bias : STD_LOGIC_VECTOR(63 downto 0);
signal temp : STD_LOGIC_VECTOR(63 downto 0);

begin

bias <= BIAS1  when sel=x"0" else
        BIAS2  when sel=x"1" else
        BIAS3  when sel=x"2" else
        BIAS4  when sel=x"3" else
        BIAS5  when sel=x"4" else		  
        BIAS6  when sel=x"5" else
        BIAS7  when sel=x"6" else		  
        BIAS8  when sel=x"7" else
        BIAS9  when sel=x"8" else
        BIAS10 when sel=x"9" else
        BIAS11 when sel=x"A" else		  
        BIAS12 when sel=x"B" else
        BIAS13 when sel=x"C" else		  
		  (others => '-');

bytes: for i in 7 downto 0 generate 
begin
	temp(8*i+7 downto 8*i) <= 
		input(8*i+4 downto 8*i) & input(8*i+7 downto 8*i+5);

	biased(8*i+7 downto 8*i) <=
		input(8*i+7 downto 8*i) + bias(8*i+7 downto 8*i);
end generate;

rotated <= temp;

end rtl;

