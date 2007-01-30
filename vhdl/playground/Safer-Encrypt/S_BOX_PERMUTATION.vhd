library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity S_BOX_PERMUTATION is
    Port ( clk    : in  STD_LOGIC;
	        we     : in  STD_LOGIC := '0';
			  a_in   : in  STD_LOGIC_VECTOR (7 downto 0);
			  a_out  : out STD_LOGIC_VECTOR (7 downto 0);
			  b_in   : in  STD_LOGIC_VECTOR (7 downto 0);
			  b_out  : out STD_LOGIC_VECTOR (7 downto 0));
end S_BOX_PERMUTATION;

architecture Behavioral of S_BOX_PERMUTATION is
type SBOX_TABLE is array( 0 to 255) of STD_LOGIC_VECTOR(7 downto 0);
signal SBOX : SBOX_TABLE :=
			(	x"01",x"2D",x"E2",x"93",x"BE",x"45",x"15",x"AE",x"78",x"03",x"87",x"A4",x"B8",x"38",x"CF",x"3F",
				x"08",x"67",x"09",x"94",x"EB",x"26",x"A8",x"6B",x"BD",x"18",x"34",x"1B",x"BB",x"BF",x"72",x"F7",
				x"40",x"35",x"48",x"9C",x"51",x"2F",x"3B",x"55",x"E3",x"C0",x"9F",x"D8",x"D3",x"F3",x"8D",x"B1",
				x"FF",x"A7",x"3E",x"DC",x"86",x"77",x"D7",x"A6",x"11",x"FB",x"F4",x"BA",x"92",x"91",x"64",x"83",
				x"F1",x"33",x"EF",x"DA",x"2C",x"B5",x"B2",x"2B",x"88",x"D1",x"99",x"CB",x"8C",x"84",x"1D",x"14",
				x"81",x"97",x"71",x"CA",x"5F",x"A3",x"8B",x"57",x"3C",x"82",x"C4",x"52",x"5C",x"1C",x"E8",x"A0",
				x"04",x"B4",x"85",x"4A",x"F6",x"13",x"54",x"B6",x"DF",x"0C",x"1A",x"8E",x"DE",x"E0",x"39",x"FC",
				x"20",x"9B",x"24",x"4E",x"A9",x"98",x"9E",x"AB",x"F2",x"60",x"D0",x"6C",x"EA",x"FA",x"C7",x"D9",
				x"00",x"D4",x"1F",x"6E",x"43",x"BC",x"EC",x"53",x"89",x"FE",x"7A",x"5D",x"49",x"C9",x"32",x"C2",
				x"F9",x"9A",x"F8",x"6D",x"16",x"DB",x"59",x"96",x"44",x"E9",x"CD",x"E6",x"46",x"42",x"8F",x"0A",
				x"C1",x"CC",x"B9",x"65",x"B0",x"D2",x"C6",x"AC",x"1E",x"41",x"62",x"29",x"2E",x"0E",x"74",x"50",
				x"02",x"5A",x"C3",x"25",x"7B",x"8A",x"2A",x"5B",x"F0",x"06",x"0D",x"47",x"6F",x"70",x"9D",x"7E",
				x"10",x"CE",x"12",x"27",x"D5",x"4C",x"4F",x"D6",x"79",x"30",x"68",x"36",x"75",x"7D",x"E4",x"ED",
				x"80",x"6A",x"90",x"37",x"A2",x"5E",x"76",x"AA",x"C5",x"7F",x"3D",x"AF",x"A5",x"E5",x"19",x"61",
				x"FD",x"4D",x"7C",x"B7",x"0B",x"EE",x"AD",x"4B",x"22",x"F5",x"E7",x"73",x"23",x"21",x"C8",x"05",
				x"E1",x"66",x"DD",x"B3",x"58",x"69",x"63",x"56",x"0F",x"A1",x"31",x"95",x"17",x"07",x"3A",x"28");


begin
	
sbox_proc : process(clk) is
variable select_a : integer range 0 to 255;
variable select_b : integer range 0 to 255;	
begin                                                                 
	if clk'event and clk = '1' then                                       
		select_a := CONV_INTEGER(a_in(7 downto 0));
		select_b := CONV_INTEGER(b_in(7 downto 0));					
		a_out <= SBOX(select_a);		
		b_out <= SBOX(select_b);
		if we='1' then
				SBOX(select_b) <= a_in;
		end if;
	end if;                                                               
end process;                                                          

end Behavioral;

