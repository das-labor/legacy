-----------------------------------------------------------------------------
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
     x"98000000", x"d0000000", x"78010000", x"38210000", 
     x"d0e10000", x"f8000003", x"34000000", x"34000000", 
     x"98000000", x"781c8400", x"3b9c0000", x"781a8300", 
     x"3b5a0000", x"34010000", x"34020000", x"34030000", 
     x"f8000023", x"379cff80", x"5b810004", x"5b820008", 
     x"5b83000c", x"5b840010", x"5b850014", x"5b860018", 
     x"5b87001c", x"5b880020", x"5b890024", x"5b8a0028", 
     x"5b87001c", x"5b880020", x"5b890024", x"5b8a0028", 
     x"5b9e0078", x"5b9f007c", x"2b810080", x"5b810074", 
     x"bb800800", x"34210080", x"5b810070", x"c3a00000", 
     x"2b810004", x"2b820008", x"2b83000c", x"2b840010", 
     x"2b850014", x"2b860018", x"2b87001c", x"2b880020", 
     x"2b890024", x"2b8a0028", x"2b9d0074", x"2b9e0078", 
     x"2b9f007c", x"2b9c0070", x"c3c00000", x"379cfff8", 
     x"5b9b0008", x"5b9d0004", x"341b0008", x"b77cd800", 
     x"e0000000", 

    others => x"00000000" 
);
	
signal ack : std_logic;

begin

wb_ack_o <= wb_stb_i and ack;

memproc: process (clk) is
variable a : integer;
begin
	if clk'event and clk='1' then
	if wb_stb_i='1' then
		a := to_integer(unsigned(wb_adr_i(12 downto 2)));
	
		if wb_we_i='1' then 
			mem(a) <= wb_dat_i;
		end if;
		
--			if wb_sel_i(3)='1' then
--				mem(a)(31 downto 24) <= wb_dat_i(31 downto 24);
--			end if;
--			if wb_sel_i(2)='1' then
--				mem(a)(23 downto 16) <= wb_dat_i(23 downto 16);
--			end if;
--			if wb_sel_i(1)='1' then
--				mem(a)(15 downto  8) <= wb_dat_i(15 downto  8);
--			end if;
--			if wb_sel_i(0)='1' then
--				mem(a)( 7 downto  0) <= wb_dat_i( 7 downto  0);
--			end if;
--		end if;


		wb_dat_o <= mem(a);
		ack <= '1' and not ack;
	else
		ack <= '0';
	end if;
	end if;
end process;

end rtl;

