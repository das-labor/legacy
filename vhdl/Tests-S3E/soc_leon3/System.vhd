library ieee;
use ieee.std_logic_1164.all;
use work.components.all;

-----------------------------------------------------------------------------
-- Leon/Wishbone System -----------------------------------------------------
entity System is
	port (
		clk_in       : in    std_logic;
		reset_in     : in    std_logic;
		-- I/O Ports
		rotary       : in    std_logic_vector(2 downto 0);
		sw           : in    std_logic_vector(3 downto 0);
		led          : out   std_logic_vector(7 downto 0);
		-- UART
		uart_rx      : in    std_logic;
		uart_tx      : out   std_logic;
		-- DDR Connections
		ddr_clk      : out   std_logic;
		ddr_clk_n    : out   std_logic;
		ddr_clk_fb   : in    std_logic;
		ddr_dq       : inout std_logic_vector(15 downto 0);
		ddr_dqs      : inout std_logic_vector( 1 downto 0);
		ddr_dm       : out   std_logic_vector( 1 downto 0);
		ddr_addr     : out   std_logic_vector(12 downto 0);
		ddr_ba       : out   std_logic_vector( 1 downto 0);
		ddr_cke      : out   std_logic;
		ddr_cs_n     : out   std_logic;
		ddr_ras_n    : out   std_logic;
		ddr_cas_n    : out   std_logic;
		ddr_we_n     : out   std_logic );
end entity;

-----------------------------------------------------------------------------
-- Implementation -----------------------------------------------------------
architecture rtl of System is

-- System Signals
signal clk        : std_logic;
signal clk_2x     : std_logic;
signal clk_2x90   : std_logic;
signal locked     : std_logic;
signal reset      : std_logic;

signal ddr_addr13 : std_logic;
signal ddr_dbg    : std_logic_vector(7 downto 0);
signal debug_led  : std_logic_vector(7 downto 0);

signal leon_irqs  : std_logic_vector(15 downto 0);

signal probe      : std_logic_vector(31 downto 0);

-- 32 bit wishbone bus
signal wb32_cyc   : std_logic;
signal wb32_stb   : std_logic;
signal wb32_ack   : std_logic;
signal wb32_we    : std_logic;
signal wb32_cache : std_logic;
signal wb32_sel   : std_logic_vector( 3 downto 0);
signal wb32_adr   : std_logic_vector(31 downto 0);
signal wb32_dat_r : std_logic_vector(31 downto 0);
signal wb32_dat_w : std_logic_vector(31 downto 0);

signal prom0_en   : std_logic;
signal prom0_ack  : std_logic;
signal prom0_stb  : std_logic;
signal prom0_dat  : std_logic_vector(31 downto 0);

signal gpio0_en   : std_logic;
signal gpio0_ack  : std_logic;
signal gpio0_stb  : std_logic;
signal gpio0_dat  : std_logic_vector(31 downto 0);

signal uart0_en   : std_logic;
signal uart0_ack  : std_logic;
signal uart0_stb  : std_logic;
signal uart0_dat  : std_logic_vector(31 downto 0);
signal uart0_txirq: std_logic;
signal uart0_rxirq: std_logic;

signal timer0_en  : std_logic;
signal timer0_ack : std_logic;
signal timer0_stb : std_logic;
signal timer0_dat : std_logic_vector(31 downto 0);
signal timer0_irq0: std_logic;
signal timer0_irq1: std_logic;

signal scope0_en  : std_logic;
signal scope0_ack : std_logic;
signal scope0_stb : std_logic;
signal scope0_dat : std_logic_vector(31 downto 0);
signal scope0_irq : std_logic;

signal bridge_en  : std_logic;
signal bridge_ack : std_logic;
signal bridge_stb : std_logic;
signal bridge_dat : std_logic_vector(31 downto 0);

-- 64 bit wishbone bus
signal wb64_cyc   : std_logic;
signal wb64_stb   : std_logic;
signal wb64_ack   : std_logic;
signal wb64_we    : std_logic;
signal wb64_sel   : std_logic_vector( 7 downto 0);
signal wb64_adr   : std_logic_vector(31 downto 0);
signal wb64_dat_r : std_logic_vector(63 downto 0);
signal wb64_dat_w : std_logic_vector(63 downto 0);

-- I/O Ports
signal iport    : std_logic_vector(31 downto 0);
signal oport    : std_logic_vector(31 downto 0);


begin

-----------------------------------------------------------------------------
-- Clock and Reset  ---------------------------------------------------------
reset <= reset_in or not locked;

clkgen0: entity work.ClkGen
	port map (
		clk_in     => clk_in,
		reset      => reset_in,
		clk        => clk,
		clk_2x     => clk_2x,
		clk_2x90   => clk_2x90,
		locked     => locked );

-----------------------------------------------------------------------------
-- LEON3 CPU with Wishbone interface ----------------------------------------
cpu0: wb_leon
	port map (
		clk         => clk,
		reset       => reset,
		-- Interrupt requests
		irq         => leon_irqs,
		-- 32 bit Wishbone
		wb_adr_o    => wb32_adr,
		wb_dat_o    => wb32_dat_w,
		wb_dat_i    => wb32_dat_r,
		wb_cache_i  => wb32_cache,
		wb_sel_o    => wb32_sel,
		wb_we_o     => wb32_we,
		wb_cyc_o    => wb32_cyc,
		wb_stb_o    => wb32_stb,
		wb_ack_i    => wb32_ack );

-----------------------------------------------------------------------------
-- PROM ---------------------------------------------------------------------
prom0: wb_bram
	port map (
		clk         => clk,
		reset       => reset,
		-- Wishbone bus
		wb_adr_i    => wb32_adr,
		wb_dat_i    => wb32_dat_w,
		wb_dat_o    => prom0_dat,
		wb_sel_i    => wb32_sel,
		wb_cyc_i    => wb32_cyc,
		wb_stb_i    => prom0_stb,
		wb_ack_o    => prom0_ack,
		wb_we_i     => wb32_we );		

-----------------------------------------------------------------------------
-- GPIO ---------------------------------------------------------------------
gpio0: wb_gpio
	port map (
		clk         => clk,
		reset       => reset,
		-- Wishbone bus
		wb_adr_i    => wb32_adr,
		wb_dat_i    => wb32_dat_w,
		wb_dat_o    => gpio0_dat,
		wb_sel_i    => wb32_sel,
		wb_cyc_i    => wb32_cyc,
		wb_stb_i    => gpio0_stb,
		wb_ack_o    => gpio0_ack,
		wb_we_i     => wb32_we,
		-- i/o ports 
		iport       => iport,
		oport       => oport );		

-----------------------------------------------------------------------------
-- UART ---------------------------------------------------------------------
uart0: wb_uart
	port map (
		clk         => clk,
		reset       => reset,
		-- Wishbone bus
		wb_adr_i    => wb32_adr,
		wb_dat_i    => wb32_dat_w,
		wb_dat_o    => uart0_dat,
		wb_sel_i    => wb32_sel,
		wb_cyc_i    => wb32_cyc,
		wb_stb_i    => uart0_stb,
		wb_ack_o    => uart0_ack,
		wb_we_i     => wb32_we,
		wb_txirq_o  => uart0_txirq,
		wb_rxirq_o  => uart0_rxirq,
		-- i/o ports 
		uart_rx     => uart_rx,
		uart_tx     => uart_tx );		

-----------------------------------------------------------------------------
-- TIMER --------------------------------------------------------------------
timer0: wb_timer
	port map (
		clk         => clk,
		reset       => reset,
		-- Wishbone bus
		wb_adr_i    => wb32_adr,
		wb_dat_i    => wb32_dat_w,
		wb_dat_o    => timer0_dat,
		wb_sel_i    => wb32_sel,
		wb_cyc_i    => wb32_cyc,
		wb_stb_i    => timer0_stb,
		wb_ack_o    => timer0_ack,
		wb_we_i     => wb32_we,
		wb_irq0_o    => timer0_irq0,
		wb_irq1_o    => timer0_irq1 );

-----------------------------------------------------------------------------
-- Scope --------------------------------------------------------------------
scope0: wb_scope
	generic map (
		depth       => 4096 )   -- *32Bit =^ *8 Samples
	port map (
		clk         => clk,
		reset       => reset,
		-- Wishbone bus
		wb_adr_i    => wb32_adr,
		wb_dat_i    => wb32_dat_w,
		wb_dat_o    => scope0_dat,
		wb_sel_i    => wb32_sel,
		wb_cyc_i    => wb32_cyc,
		wb_stb_i    => scope0_stb,
		wb_ack_o    => scope0_ack,
		wb_we_i     => wb32_we,
		wb_irq_o    => scope0_irq,
        -- I/O Probe
		probe       => probe );

-----------------------------------------------------------------------------
-- 32 to 64 Bit Bus Bridge --------------------------------------------------
bridge: wb32_wb64
	port map (
		clk         => clk,
		reset       => reset,
		-- 32 bit wishbone slave
		wb32_cyc_i  => wb32_cyc,
		wb32_stb_i  => bridge_stb,
		wb32_ack_o  => bridge_ack,
		wb32_we_i   => wb32_we,
		wb32_sel_i  => wb32_sel,
		wb32_adr_i  => wb32_adr,
		wb32_dat_i  => wb32_dat_w,
		wb32_dat_o  => bridge_dat,
		-- 64 bit wishbone master
		wb64_cyc_o  => wb64_cyc,
		wb64_stb_o  => wb64_stb,
		wb64_ack_i  => wb64_ack,
		wb64_we_o   => wb64_we,
		wb64_sel_o  => wb64_sel,
		wb64_adr_o  => wb64_adr,
		wb64_dat_o  => wb64_dat_w,
		wb64_dat_i  => wb64_dat_r );

-----------------------------------------------------------------------------
-- 64 Bit DDR Controller ----------------------------------------------------
ddr0: wb_ddr
	port map(
		clk        => clk,
		clk_2x     => clk_2x,
		clk_2x90   => clk_2x90,      
		reset      => reset,
		-- DDR connection
		ddr_clk    => ddr_clk,
		ddr_clk_n  => ddr_clk_n,
		ddr_clk_fb => ddr_clk_fb,
		ddr_dq     => ddr_dq,
		ddr_dqs    => ddr_dqs,
		ddr_dm     => ddr_dm,
		ddr_ba     => ddr_ba,
		ddr_cke    => ddr_cke,
		ddr_cs_n   => ddr_cs_n,
		ddr_ras_n  => ddr_ras_n,
		ddr_cas_n  => ddr_cas_n,
		ddr_we_n   => ddr_we_n,
		ddr_addr(12 downto 0)   => ddr_addr,
		ddr_addr(13)=> ddr_addr13,
		-- 64 bit Whishbone connection
		wb_adr_i   => wb64_adr,
		wb_dat_i   => wb64_dat_w,
		wb_dat_o   => wb64_dat_r,
		wb_we_i    => wb64_we,
		wb_stb_i   => wb64_stb,
		wb_sel_i   => wb64_sel,
		wb_cyc_i   => wb64_cyc,
		wb_ack_o   => wb64_ack,
		-- Debug and rotary
		sw         => "0000",
		debug      => ddr_dbg,
		rotary     => "000" );


-----------------------------------------------------------------------------
-- 32 Bit Wishbone Interconnect ---------------------------------------------
wb32_cache  <= '0' when wb32_adr(31 downto 28)=x"F" else
               '1';

prom0_en  <= '1' when wb32_adr(31 downto 28)=x"0" else
             '0';

bridge_en <= '1' when wb32_adr(31 downto 28)=x"8" else
             '0';

gpio0_en  <= '1' when wb32_adr(31 downto 16)=x"F000" else
             '0';

uart0_en  <= '1' when wb32_adr(31 downto 16)=x"F001" else
             '0';

timer0_en <= '1' when wb32_adr(31 downto 16)=x"F002" else
             '0';

scope0_en <= '1' when wb32_adr(31 downto 20)=x"F01" else
             '0';

prom0_stb  <= prom0_en  and wb32_stb and wb32_cyc;
bridge_stb <= bridge_en and wb32_stb and wb32_cyc;
gpio0_stb  <= gpio0_en  and wb32_stb and wb32_cyc;
uart0_stb  <= uart0_en  and wb32_stb and wb32_cyc;
timer0_stb <= timer0_en and wb32_stb and wb32_cyc;
scope0_stb <= scope0_en and wb32_stb and wb32_cyc;

wb32_dat_r <=  prom0_dat  when  prom0_en='1' else
              bridge_dat  when bridge_en='1' else
               gpio0_dat  when  gpio0_en='1' else
               uart0_dat  when  uart0_en='1' else
              timer0_dat  when timer0_en='1' else
              scope0_dat  when scope0_en='1' else
              (others => '-');

wb32_ack   <= prom0_ack  or gpio0_ack  or uart0_ack or  
              bridge_ack or timer0_ack or scope0_ack;



-----------------------------------------------------------------------------
-- IRQs----- ----------------------------------------------------------------
leon_irqs <= (
	12     => timer0_irq0,         -- high priority
	11     => scope0_irq,
	10     => uart0_rxirq,
	6      => timer0_irq1,
	4      => uart0_txirq,         -- low priority
	others => '0' );


-----------------------------------------------------------------------------
-- Debug Probes for On-Chip-Scope -------------------------------------------
probe <= (
	0      => wb32_stb,          -- Group 1: Wishbone Bus
	1      => wb32_ack,
	2      => wb32_we,
	3      => wb64_stb,
	4      => wb64_ack,
	5      => wb64_we,

	6      => oport(0),          -- Group 2: LEDs
	7      => oport(1),

	8      => timer0_irq0,       -- Group 3: Interrupts
	9      => uart0_rxirq,
	10     => uart0_txirq,
	                             -- Group 4: DDR
	11     => ddr_dbg(0),  -- need AUTO_REFRESH
	
	others => '0' );

-----------------------------------------------------------------------------
-- LEDs and I/O Ports -------------------------------------------------------
debug_led <= reset & clk & uart0_en & bridge_en & scope0_en & scope0_ack & wb32_stb & wb32_ack;

led <=  oport(7 downto 0) when sw(3 downto 2)="00" else
        debug_led;

iport <= (others => '0');


end architecture;
