--DO NOT EDIT - This is a generated - edit Software/Boootloader/srectovhdl.c 
-- Wishbone Block Ram -------------------------------------------------------
library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.all;

entity wb_bram is
   port (
      clk      : in  std_logic;
      reset    : in  std_logic;
      -- Wishbone bus
      wb_adr_i : in  std_logic_vector(31 downto 0);
      wb_dat_i : in  std_logic_vector(31 downto 0);
      wb_dat_o : out std_logic_vector(31 downto 0);
      wb_sel_i : in  std_logic_vector( 3 downto 0);
      wb_cyc_i : in  std_logic;
      wb_stb_i : in  std_logic;
      wb_ack_o : out std_logic;
      wb_we_i  : in  std_logic );
end wb_bram;

-----------------------------------------------------------------------------
-- Implementation -----------------------------------------------------------
architecture rtl of wb_bram is

constant mem_size : natural := (8*512) - 1;
type mem_type is array(0 to mem_size) of std_logic_vector(31 downto 0);

-----------------------------------------------------------------------------
-- Pre-Initialized-Data -----------------------------------------------------
signal mem : mem_type := (
	x"98000000", x"D0000000", x"78010000", x"38210000", 
	x"D0E10000", x"F8000003", x"34000000", x"34000000", 
	x"98000000", x"781C0000", x"3B9C1000", x"379CFFFC", 
	x"781A0000", x"3B5A0800", x"34010000", x"34020000", 
	x"34030000", x"F800007C", x"34010001", x"D0010000", 
	x"C3A00000", x"3401000F", x"D0210000", x"C3A00000", 
	x"34010000", x"D0010000", x"C3A00000", x"C0200000", 
	x"E0000000", x"379CFF80", x"5B810004", x"5B820008", 
	x"5B83000C", x"5B840010", x"5B850014", x"5B860018", 
	x"5B87001C", x"5B880020", x"5B890024", x"5B8A0028", 
	x"5B9E0078", x"5B9F007C", x"2B810080", x"5B810074", 
	x"BB800800", x"34210080", x"5B810070", x"C3A00000", 
	x"2B810004", x"2B820008", x"2B83000C", x"2B840010", 
	x"2B850014", x"2B860018", x"2B87001C", x"2B880020", 
	x"2B890024", x"2B8A0028", x"2B9D0074", x"2B9E0078", 
	x"2B9F007C", x"2B9C0070", x"C3C00000", x"379CFFF8", 
	x"5B8B0008", x"5B9D0004", x"F80000D2", x"3C2B0008", 
	x"F80000D0", x"B5615800", x"3D6B0008", x"F80000CD", 
	x"B5615800", x"3D6B0008", x"F80000CA", x"B5610800", 
	x"2B8B0008", x"2B9D0004", x"379C0008", x"C3A00000", 
	x"379CFFEC", x"5B8B0014", x"5B8C0010", x"5B8D000C", 
	x"5B8E0008", x"5B9D0004", x"780DF000", x"B8205800", 
	x"39AD0000", x"340E0009", x"340C0007", x"A16D0800", 
	x"0022001C", x"34410037", x"4DC2000C", x"3D6B0004", 
	x"F80000C0", x"358CFFFF", x"4D80FFF9", x"2B8B0014", 
	x"2B8C0010", x"2B8D000C", x"2B8E0008", x"2B9D0004", 
	x"379C0014", x"C3A00000", x"34410030", x"E3FFFFF4", 
	x"379CFFF4", x"5B8B000C", x"5B8C0008", x"5B9D0004", 
	x"780BB000", x"B9601000", x"7801B003", x"396B0000", 
	x"3821FFFF", x"596B0000", x"356B0004", x"55610002", 
	x"E3FFFFFD", x"B8405800", x"780CB003", x"396B0000", 
	x"398CFFFF", x"29620000", x"78010000", x"382104F0", 
	x"444B0002", x"F80000A9", x"356B0004", x"556C0002", 
	x"E3FFFFF9", x"78010000", x"38210508", x"F80000A3", 
	x"2B8B000C", x"2B8C0008", x"2B9D0004", x"379C000C", 
	x"C3A00000", x"379CFFE8", x"5B8B0018", x"5B8C0014", 
	x"5B8D0010", x"5B8E000C", x"5B8F0008", x"5B9D0004", 
	x"F8000078", x"78010000", x"3821051C", x"F8000093", 
	x"FBFFFFD4", x"F800007B", x"64220067", x"B8201800", 
	x"5C400034", x"68210067", x"5C200008", x"64610065", 
	x"4420FFF9", x"F8000073", x"F800007E", x"F8000071", 
	x"F800007C", x"E3FFFFFC", x"64610072", x"5C200038", 
	x"64610075", x"4420FFF0", x"34010075", x"F8000075", 
	x"3401003A", x"F8000073", x"FBFFFF91", x"B8206000", 
	x"FBFFFFA0", x"3401003A", x"F800006E", x"FBFFFF8C", 
	x"B8205800", x"FBFFFF9B", x"3401003A", x"F8000069", 
	x"B58B7800", x"340D0000", x"B9807000", x"518F0027", 
	x"F8000058", x"3C2B0018", x"B5A16800", x"F8000055", 
	x"B8201800", x"3C210010", x"B5A36800", x"B5615800", 
	x"F8000050", x"B8201800", x"3C210008", x"B5A36800", 
	x"B5615800", x"F800004B", x"B5615800", x"31CB0000", 
	x"B5A16800", x"35CE0001", x"51CF0014", x"E3FFFFED", 
	x"F8000050", x"3401003A", x"F800004E", x"FBFFFF6C", 
	x"B8206000", x"FBFFFF7B", x"3401002E", x"F8000049", 
	x"FBFFFF40", x"B9800800", x"FBFFFF41", x"78010000", 
	x"38210538", x"F800004D", x"E3FFFFBB", x"34010000", 
	x"FBFFFF3B", x"E3FFFFC9", x"B9A00800", x"FBFFFF6D", 
	x"78010000", x"38210540", x"F8000044", x"E3FFFFB2", 
	x"78020000", x"78040000", x"64210002", x"384204EC", 
	x"388404DC", x"5C200002", x"C3A00000", x"28410000", 
	x"28820000", x"28230000", x"34420001", x"58820000", 
	x"C3A00000", x"78020000", x"384204EC", x"28430000", 
	x"082161A8", x"3402000A", x"58610010", x"34010000", 
	x"58610014", x"5862000C", x"2861000C", x"20210001", 
	x"4420FFFE", x"C3A00000", x"78010000", x"382104EC", 
	x"28230000", x"340261A8", x"58620004", x"34010000", 
	x"58610008", x"3402000E", x"58620000", x"C3A00000", 
	x"78010000", x"382104E8", x"28230000", x"340201B2", 
	x"58620004", x"34010000", x"58610000", x"C3A00000", 
	x"78030000", x"B8601000", x"384204E8", x"28420000", 
	x"28410000", x"20210001", x"4420FFFB", x"34010000", 
	x"58410000", x"28410008", x"202100FF", x"C3A00000", 
	x"78020000", x"384204E8", x"28420000", x"202300FF", 
	x"28410000", x"20210002", x"5C20FFFE", x"58430008", 
	x"58410000", x"C3A00000", x"379CFFF8", x"5B8B0008", 
	x"5B9D0004", x"B8205800", x"E0000003", x"FBFFFFF1", 
	x"356B0001", x"41610000", x"5C20FFFD", x"2B8B0008", 
	x"2B9D0004", x"379C0008", x"C3A00000", x"00000000", 
	x"B0000000", x"80002000", x"80001000", x"80000000", 
	x"5352414D", x"204D454D", x"54455354", x"20455252", 
	x"4F520A0D", x"00000000", x"5352414D", x"204D454D", 
	x"54455354", x"204F4B0A", x"0D000000", x"0D0A2A2A", 
	x"20535049", x"4B452042", x"4F4F544C", x"4F414445", 
	x"52202A2A", x"0A0D0000", x"58585858", x"00000000", 
	x"2E0D0A00",    others => x"00000000" 
);

signal ack : std_logic;

begin

wb_ack_o <= wb_stb_i and ack;

memproc: process (clk) is
variable a : integer;
begin
        if clk'event and clk='1' then
        if wb_stb_i='1' and wb_cyc_i='1' then
                a := to_integer(unsigned(wb_adr_i(12 downto 2)));
        
                if wb_we_i='1' then 
                        mem(a) <= wb_dat_i;
                end if;

                wb_dat_o <= mem(a);
                ack <= '1' and not ack after 2 ns;
        else
                ack <= '0';
        end if;
        end if;
end process;

end rtl;
