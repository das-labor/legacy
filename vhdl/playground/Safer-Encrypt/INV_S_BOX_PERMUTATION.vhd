library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity INV_S_BOX_PERMUTATION is
    Port ( clk    : in  STD_LOGIC;
	        we     : in  STD_LOGIC := '0';
			  a_in   : in  STD_LOGIC_VECTOR (7 downto 0);
			  a_out  : out STD_LOGIC_VECTOR (7 downto 0);
			  b_in   : in  STD_LOGIC_VECTOR (7 downto 0);
			  b_out  : out STD_LOGIC_VECTOR (7 downto 0));
end INV_S_BOX_PERMUTATION;

architecture rtl of INV_S_BOX_PERMUTATION is
type SBOX_INV_TABLE is array( 0 to 255 ) of STD_LOGIC_VECTOR(7 downto 0);
  
signal SBOX_INV : SBOX_INV_TABLE :=
	( 	x"80",x"00",x"B0",x"09",x"60",x"EF",x"B9",x"FD",x"10",x"12",x"9F",x"E4",x"69",x"BA",x"AD",x"F8", 
		x"C0",x"38",x"C2",x"65",x"4F",x"06",x"94",x"FC",x"19",x"DE",x"6A",x"1B",x"5D",x"4E",x"A8",x"82", 
		x"70",x"ED",x"E8",x"EC",x"72",x"B3",x"15",x"C3",x"FF",x"AB",x"B6",x"47",x"44",x"01",x"AC",x"25", 
		x"C9",x"FA",x"8E",x"41",x"1A",x"21",x"CB",x"D3",x"0D",x"6E",x"FE",x"26",x"58",x"DA",x"32",x"0F", 
		x"20",x"A9",x"9D",x"84",x"98",x"05",x"9C",x"BB",x"22",x"8C",x"63",x"E7",x"C5",x"E1",x"73",x"C6", 
		x"AF",x"24",x"5B",x"87",x"66",x"27",x"F7",x"57",x"F4",x"96",x"B1",x"B7",x"5C",x"8B",x"D5",x"54", 
		x"79",x"DF",x"AA",x"F6",x"3E",x"A3",x"F1",x"11",x"CA",x"F5",x"D1",x"17",x"7B",x"93",x"83",x"BC", 
		x"BD",x"52",x"1E",x"EB",x"AE",x"CC",x"D6",x"35",x"08",x"C8",x"8A",x"B4",x"E2",x"CD",x"BF",x"D9", 
		x"D0",x"50",x"59",x"3F",x"4D",x"62",x"34",x"0A",x"48",x"88",x"B5",x"56",x"4C",x"2E",x"6B",x"9E", 
		x"D2",x"3D",x"3C",x"03",x"13",x"FB",x"97",x"51",x"75",x"4A",x"91",x"71",x"23",x"BE",x"76",x"2A", 
		x"5F",x"F9",x"D4",x"55",x"0B",x"DC",x"37",x"31",x"16",x"74",x"D7",x"77",x"A7",x"E6",x"07",x"DB", 
		x"A4",x"2F",x"46",x"F3",x"61",x"45",x"67",x"E3",x"0C",x"A2",x"3B",x"1C",x"85",x"18",x"04",x"1D", 
		x"29",x"A0",x"8F",x"B2",x"5A",x"D8",x"A6",x"7E",x"EE",x"8D",x"53",x"4B",x"A1",x"9A",x"C1",x"0E", 
		x"7A",x"49",x"A5",x"2C",x"81",x"C4",x"C7",x"36",x"2B",x"7F",x"43",x"95",x"33",x"F2",x"6C",x"68", 
		x"6D",x"F0",x"02",x"28",x"CE",x"DD",x"9B",x"EA",x"5E",x"99",x"7C",x"14",x"86",x"CF",x"E5",x"42", 
		x"B8",x"40",x"78",x"2D",x"3A",x"E9",x"64",x"1F",x"92",x"90",x"7D",x"39",x"6F",x"E0",x"89",x"30");

begin

sbox_proc : process(clk) is
variable select_a : integer range 0 to 255;
variable select_b : integer range 0 to 255;	
begin                                                                 
	if clk'event and clk = '1' then                                       
		select_a := CONV_INTEGER(a_in(7 downto 0));
		select_b := CONV_INTEGER(b_in(7 downto 0));					
		a_out <= SBOX_INV(select_a);		
		b_out <= SBOX_INV(select_b);
		if we='1' then
				SBOX_INV(select_b) <= a_in;
		end if;
	end if;                                                               
end process;                                                          

end rtl;
