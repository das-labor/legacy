
library ieee;
use ieee.std_logic_1164.all;


-----------------------------------------------------------------------------
-- 32 bit to 64 bit Wishbone Bridge -----------------------------------------
entity wb32_wb64 is
	port (
		clk         : in  std_logic;
		reset       : in  std_logic;
		-- 32 bit wishbone slave
		wb32_cyc_i  : in  std_logic;
		wb32_stb_i  : in  std_logic;
		wb32_ack_o  : out std_logic;
		wb32_we_i   : in  std_logic;
		wb32_sel_i  : in  std_logic_vector(3 downto 0);
		wb32_adr_i  : in  std_logic_vector(31 downto 0);
		wb32_dat_i  : in  std_logic_vector(31 downto 0);
		wb32_dat_o  : out std_logic_vector(31 downto 0);
		-- 64 bit wishbone master
		wb64_cyc_o  : out std_logic;
		wb64_stb_o  : out std_logic;
		wb64_ack_i  : in  std_logic;
		wb64_we_o   : out std_logic;
		wb64_sel_o  : out std_logic_vector(7 downto 0);		
		wb64_adr_o  : out std_logic_vector(31 downto 0);
		wb64_dat_o  : out std_logic_vector(63 downto 0);
		wb64_dat_i  : in  std_logic_vector(63 downto 0) );
end wb32_wb64;

architecture rtl of wb32_wb64 is

signal cache_adr : std_logic_vector(31 downto 0);
signal cache_dat : std_logic_vector(63 downto 0);
signal stb64     : std_logic;

begin

-- reading data from cache
wb32_dat_o <= cache_dat(63 downto 32) when wb32_adr_i(2 downto 0)="000" and wb32_stb_i='1' and wb32_we_i='0' else
              cache_dat(31 downto  0) when wb32_adr_i(2 downto 0)="100" and wb32_stb_i='1' and wb32_we_i='0' else 
				  (others => '-');
				  
wb32_ack_o <= '1'        when wb32_adr_i(31 downto 3)=cache_adr(31 downto 3) and wb32_we_i='0' and wb32_stb_i='1' else
              wb64_ack_i when wb32_we_i='1' and wb32_stb_i='1' else
              '0';
				  
-- transaction on wb64
wb64_we_o  <= wb32_we_i;

wb64_sel_o <= wb32_sel_i & "0000" when wb32_adr_i(2 downto 0)="000" and wb32_stb_i='1' else
              "0000" & wb32_sel_i when wb32_adr_i(2 downto 0)="100" and wb32_stb_i='1' else 
				  (others => '-');				  

wb64_dat_o <= wb32_dat_i & x"00000000" when wb32_adr_i(2 downto 0)="000" and wb32_stb_i='1' else
              x"00000000" & wb32_dat_i when wb32_adr_i(2 downto 0)="100" and wb32_stb_i='1' else 
				  (others => '-');

wb64_adr_o <= wb32_adr_i(31 downto 3) & "000";

stb64 <= '1' when wb32_adr_i(31 downto 3)/=cache_adr(31 downto 3) and wb32_we_i='0' and wb32_stb_i='1' else
         '1' when wb32_we_i='1' and wb32_stb_i='1' else
         '0';

wb64_stb_o <= stb64;
wb64_cyc_o <= stb64;

proc: process(clk, reset) is
begin
	if reset='1' then 
		cache_adr <= (others => '1');
	elsif clk'event and clk='1' then
		if wb64_ack_i='1' and wb32_we_i='0' then
			cache_adr <= wb32_adr_i;
			cache_dat <= wb64_dat_i;
		elsif wb64_ack_i='1' and wb32_we_i='1' and wb32_adr_i(31 downto 3)=cache_adr(31 downto 3) then
			cache_adr <= (others => '1');
		end if;
	end if;
end process;


end rtl;

