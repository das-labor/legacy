-----------------------------------------------------------------------------
-- AHB-to-Wishbone bridge ---------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;

entity ahb2wb is
	port (
		clk          : in  std_logic;
		reset        : in  std_logic;
		-- 32 bit AHB 
		haddr        : in  std_logic_vector(31 downto 0);
		htrans       : in  std_logic_vector( 1 downto 0);
		hwrite       : in  std_logic;
		hsize        : in  std_logic_vector( 2 downto 0);
		hburst       : in  std_logic_vector( 2 downto 0);
		hwdata       : in  std_logic_vector(31 downto 0);
		hrdata       : out std_logic_vector(31 downto 0);
		hcache       : out std_logic;
		hsel         : in  std_logic;
		hreadyi      : in  std_logic;
		hreadyo      : out std_logic;
		hresp        : out std_logic_vector( 1 downto 0);
		-- 32 bit Wishbone
		wb32_adr_o   : out std_logic_vector(31 downto 0);
		wb32_dat_o   : out std_logic_vector(31 downto 0);
		wb32_dat_i   : in  std_logic_vector(31 downto 0);		
		wb32_cache_i : in  std_logic;
		wb32_we_o    : out std_logic;
		wb32_sel_o   : out std_logic_vector( 3 downto 0);
		wb32_cyc_o   : out std_logic;
		wb32_stb_o   : out std_logic;		
		wb32_ack_i   : in  std_logic	);
end ahb2wb;

-----------------------------------------------------------------------------
-- Implementation -----------------------------------------------------------
architecture rtl of ahb2wb is

constant HTRANS_IDLE  : std_logic_vector( 1 downto 0) := "00";
constant HTRANS_BUSY  : std_logic_vector( 1 downto 0) := "01";
constant HTRANS_NONSEQ: std_logic_vector( 1 downto 0) := "10";
constant HTRANS_SEQ   : std_logic_vector( 1 downto 0) := "11";

constant HRESP_OKAY   : std_logic_vector( 1 downto 0) := "00";
constant HRESP_ERROR  : std_logic_vector( 1 downto 0) := "01";
constant HRESP_RETRY  : std_logic_vector( 1 downto 0) := "10";
constant HRESP_SPLIT  : std_logic_vector( 1 downto 0) := "11";

type state_type is (control, write);
signal state     : state_type;

signal waitack   : std_logic;
signal sel       : std_logic_vector(3 downto 0);

begin


hreadyo <= '1'        when state=control and waitack='0' else
           wb32_ack_i when state=control else
           '0';			 

hrdata <= wb32_dat_i;
hcache <= wb32_cache_i;

sel <= "1111" when hsize="010" and haddr(1 downto 0)="00" else
              "1100" when hsize="001" and haddr(1 downto 0)="00" else
              "0011" when hsize="001" and haddr(1 downto 0)="10" else
              "1000" when hsize="000" and haddr(1 downto 0)="00" else
              "0100" when hsize="000" and haddr(1 downto 0)="01" else
              "0010" when hsize="000" and haddr(1 downto 0)="10" else
              "0001" when hsize="000" and haddr(1 downto 0)="11" else
              "----";

proc: process(reset, clk) is
begin
	if reset='1' then
		state  <= control;
		waitack<= '0';
		hresp  <= HRESP_OKAY;		
	elsif clk'event and clk='1' then
		case state is
		when control =>
			if waitack='0' or wb32_ack_i='1' then
				if htrans=HTRANS_IDLE or hsel='0' then
					state      <= control;
					waitack    <= '0';					
					hresp      <= HRESP_OKAY;
					wb32_we_o  <= '0';					
					wb32_cyc_o <= '0';
					wb32_stb_o <= '0';					
				elsif (htrans=HTRANS_NONSEQ or htrans=HTRANS_SEQ) and 
				      hsel='1' and hwrite='0' then
					state      <= control;
					waitack    <= '1';					
					hresp      <= HRESP_OKAY;				
					wb32_sel_o <= sel;
					wb32_adr_o <= haddr(31 downto 2) & "00";
					wb32_we_o  <= '0';
					wb32_cyc_o <= '1';
					wb32_stb_o <= '1';									
				elsif (htrans=HTRANS_NONSEQ or htrans=HTRANS_SEQ) and 
				      hsel='1' and hwrite='1' then
					state      <= write;
					waitack    <= '1';					
					hresp      <= HRESP_OKAY;				
					wb32_sel_o <= sel;
					wb32_adr_o <= haddr(31 downto 2) & "00";
					wb32_we_o  <= '1';
					wb32_cyc_o <= '0';
					wb32_stb_o <= '0';					
				end if; 
			end if;
		when write =>
			state      <= control;
			waitack    <= '1';					
			wb32_dat_o <= hwdata;
			wb32_cyc_o <= '1';
			wb32_stb_o <= '1';
		end case;
	end if;
end process;

end rtl;

