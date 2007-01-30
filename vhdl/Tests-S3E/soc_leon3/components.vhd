library ieee;
use ieee.std_logic_1164.all;
library techmap;

package components is

-----------------------------------------------------------------------------
-- DDR Controller (64 Bit Wishbone Slave) -----------------------------------
component wb_ddr is
	generic (
		wait200us    : in    std_logic := '1' );
	port(
	   clk          : in    std_logic;
		clk_2x       : in    std_logic;
		clk_2x90     : in    std_logic;
		reset        : in    std_logic;
		-- DDR Connection
		ddr_clk      : out   std_logic;
		ddr_clk_n    : out   std_logic;
		ddr_clk_fb   : in    std_logic;
		ddr_dq       : inout std_logic_vector(15 downto 0);
		ddr_dqs      : inout std_logic_vector( 1 downto 0);
		ddr_dm       : out   std_logic_vector( 1 downto 0);
		ddr_addr     : out   std_logic_vector(13 downto 0);
		ddr_ba       : out   std_logic_vector( 1 downto 0);
		ddr_cke      : out   std_logic;
		ddr_cs_n     : out   std_logic;
		ddr_ras_n    : out   std_logic;
		ddr_cas_n    : out   std_logic;
		ddr_we_n     : out   std_logic;
		-- Whishbone Interface
		wb_adr_i     : in    std_logic_vector(31 downto 0);
		wb_dat_i     : in    std_logic_vector(63 downto 0);
		wb_dat_o     : out   std_logic_vector(63 downto 0);
		wb_sel_i     : in    std_logic_vector( 7 downto 0);
		wb_we_i      : in    std_logic;
		wb_stb_i     : in    std_logic;
		wb_cyc_i     : in    std_logic;
		wb_ack_o     : out   std_logic );
end component wb_ddr;

-----------------------------------------------------------------------------
-- AHB2WB Bus Bridge --------------------------------------------------------
component ahb2wb is
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
end component;

-----------------------------------------------------------------------------
-- Leon3 with Wishbone Interface --------------------------------------------
component wb_leon is
	port (
		clk   : in  std_logic;
		reset : in  std_logic;
		-- Interrupt requests
		irq        : in  std_logic_vector(15 downto 0);
		-- 32 bit Wishbone Master
		wb_adr_o   : out std_logic_vector(31 downto 0);
		wb_dat_o   : out std_logic_vector(31 downto 0);
		wb_dat_i   : in  std_logic_vector(31 downto 0);		
		wb_cache_i : in  std_logic;
		wb_we_o    : out std_logic;
		wb_sel_o   : out std_logic_vector( 3 downto 0);
		wb_cyc_o   : out std_logic;
		wb_stb_o   : out std_logic;		
		wb_ack_i   : in  std_logic );
end component;

-----------------------------------------------------------------------------
-- 32 bit to 64 bit Wishbone Bridge -----------------------------------------
component wb32_wb64 is
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
end component wb32_wb64;

-----------------------------------------------------------------------------
-- Wishbone Block Ram -------------------------------------------------------
component wb_bram is
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
end component;

-----------------------------------------------------------------------------
-- Wishbone GPIO ------------------------------------------------------------
component wb_gpio is
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
      wb_we_i  : in  std_logic;
      -- I/O ports 
      iport    : in  std_logic_vector(31 downto 0);
      oport    : out std_logic_vector(31 downto 0) );
end component;

-----------------------------------------------------------------------------
-- Wishbone UART ------------------------------------------------------------
component wb_uart is
   port (
      clk        : in  std_logic;
      reset      : in  std_logic;
      -- Wishbone bus
      wb_adr_i   : in  std_logic_vector(31 downto 0);
      wb_dat_i   : in  std_logic_vector(31 downto 0);
      wb_dat_o   : out std_logic_vector(31 downto 0);
      wb_sel_i   : in  std_logic_vector( 3 downto 0);
      wb_cyc_i   : in  std_logic;
      wb_stb_i   : in  std_logic;
      wb_ack_o   : out std_logic;
      wb_we_i    : in  std_logic;
      wb_rxirq_o : out std_logic;
      wb_txirq_o : out std_logic;
      -- I/O ports 
      uart_rx    : in  std_logic;
      uart_tx    : out std_logic );
end component;

-----------------------------------------------------------------------------
-- Wishbone TIMER -----------------------------------------------------------
component wb_timer is
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
      wb_we_i  : in  std_logic;
      wb_irq0_o: out std_logic;
      wb_irq1_o: out std_logic );
end component;

-----------------------------------------------------------------------------
-- Wishbone LogicAnalyzer ---------------------------------------------------
component wb_scope is
	generic (
		depth      : natural := 4096 );
	port (
		clk        : in  std_logic;
		reset      : in  std_logic;
		-- 32 Bit Wishbone Slave
		wb_adr_i   : in  std_logic_vector(31 downto 0);
		wb_dat_i   : in  std_logic_vector(31 downto 0);
		wb_dat_o   : out std_logic_vector(31 downto 0);
		wb_sel_i   : in  std_logic_vector( 3 downto 0);
		wb_cyc_i   : in  std_logic;
		wb_stb_i   : in  std_logic;
		wb_ack_o   : out std_logic;
		wb_we_i    : in  std_logic;
		wb_irq_o   : out std_logic;
		-- I/O ports
		probe      : in  std_logic_vector(31 downto 0) );
end component;








end;
