library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity System is
	Port (
		clk      : in  std_logic;
		reset    : in  std_logic;
		-- 
		leds     : out std_logic_vector(7 downto 0);
		btn      : in  std_logic_vector(3 downto 0);
		uart_rx  : in  std_logic;
		uart_tx  : out std_logic
	);
end System;

architecture rtl of System is

-----------------------------------------------------------------------------
-- Components ---------------------------------------------------------------
-----------------------------------------------------------------------------
component intercon is
  port (
  -- wishbone master port(s)
  -- lm32i
  lm32i_dat_i : out std_logic_vector(31 downto 0);
  lm32i_ack_i : out std_logic;
  lm32i_err_i : out std_logic;
  lm32i_rty_i : out std_logic;
  lm32i_dat_o : in  std_logic_vector(31 downto 0);
  lm32i_we_o  : in  std_logic;
  lm32i_sel_o : in  std_logic_vector(3 downto 0);
  lm32i_adr_o : in  std_logic_vector(31 downto 0);
  lm32i_cyc_o : in  std_logic;
  lm32i_stb_o : in  std_logic;
  -- lm32d
  lm32d_dat_i : out std_logic_vector(31 downto 0);
  lm32d_ack_i : out std_logic;
  lm32d_err_i : out std_logic;
  lm32d_rty_i : out std_logic;
  lm32d_dat_o : in  std_logic_vector(31 downto 0);
  lm32d_we_o  : in  std_logic;
  lm32d_sel_o : in  std_logic_vector(3 downto 0);
  lm32d_adr_o : in  std_logic_vector(31 downto 0);
  lm32d_cyc_o : in  std_logic;
  lm32d_stb_o : in  std_logic;
  -- wishbone slave port(s)
  -- bram0
  bram0_dat_o : in  std_logic_vector(31 downto 0);
  bram0_ack_o : in  std_logic;
  bram0_dat_i : out std_logic_vector(31 downto 0);
  bram0_we_i  : out std_logic;
  bram0_sel_i : out std_logic_vector(3 downto 0);
  bram0_adr_i : out std_logic_vector(31 downto 0);
  bram0_cyc_i : out std_logic;
  bram0_stb_i : out std_logic;
  -- timer0
  timer0_dat_o : in  std_logic_vector(31 downto 0);
  timer0_ack_o : in  std_logic;
  timer0_dat_i : out std_logic_vector(31 downto 0);
  timer0_we_i  : out std_logic;
  timer0_sel_i : out std_logic_vector(3 downto 0);
  timer0_adr_i : out std_logic_vector(31 downto 0);
  timer0_cyc_i : out std_logic;
  timer0_stb_i : out std_logic;
  -- uart0
  uart0_dat_o : in  std_logic_vector(31 downto 0);
  uart0_ack_o : in  std_logic;
  uart0_dat_i : out std_logic_vector(31 downto 0);
  uart0_we_i  : out std_logic;
  uart0_sel_i : out std_logic_vector(3 downto 0);
  uart0_adr_i : out std_logic_vector(31 downto 0);
  uart0_cyc_i : out std_logic;
  uart0_stb_i : out std_logic;
  -- gpio
  gpio0_dat_o : in  std_logic_vector(31 downto 0);
  gpio0_ack_o : in  std_logic;
  gpio0_dat_i : out std_logic_vector(31 downto 0);
  gpio0_we_i  : out std_logic;
  gpio0_sel_i : out std_logic_vector(3 downto 0);
  gpio0_adr_i : out std_logic_vector(31 downto 0);
  gpio0_cyc_i : out std_logic;
  gpio0_stb_i : out std_logic;
  -- clock and reset
  clk   : in std_logic;
  reset : in std_logic);
end component intercon;

component lm32_cpu
	Port (
		clk_i            : in  std_logic;
--		clk_n_i          : in  std_logic;
		rst_i            : in  std_logic;
		--
		interrupt_n      : in  std_logic_vector(31 downto 0);
		--
--		user_result      : ;
--		user_complete    : ;
--		user_valid       : ;
--		user_opcode      : ;
--		user_operand_0   : ;
--		user_operand_1   : ;
		--
--		jtag_clk         : in  std_logic;
--		jtag_update      : in  std_logic;
--		jtag_reg_q       : in  std_logic_vector( 7 downto 0);
--		jtag_reg_addr_q  : in  std_logic_vector( 2 downto 0);
--		jtag_reg_d       : out std_logic_vector( 7 downto 0);
--		jtag_reg_addr_d  : out std_logic_vector( 2 downto 0);
		-- Instruction Wishbone
		I_ADR_O          : out std_logic_vector(31 downto 0);
		I_DAT_I          : in  std_logic_vector(31 downto 0);
		I_DAT_O          : out std_logic_vector(31 downto 0);
		I_SEL_O          : out std_logic_vector( 3 downto 0);
		I_CYC_O          : out std_logic;
		I_STB_O          : out std_logic;
		I_ACK_I          : in  std_logic;
		I_WE_O           : out std_logic;
		I_CTI_O          : out std_logic_vector( 2 downto 0);
		I_LOCK_O         : out std_logic;
		I_BTE_O          : out std_logic_vector( 1 downto 0);
		I_ERR_I          : in  std_logic;
		I_RTY_I          : in  std_logic;		
		-- Data Wishbone
		D_ADR_O          : out std_logic_vector(31 downto 0);
		D_DAT_I          : in  std_logic_vector(31 downto 0);
		D_DAT_O          : out std_logic_vector(31 downto 0);
		D_SEL_O          : out std_logic_vector( 3 downto 0);
		D_CYC_O          : out std_logic;
		D_STB_O          : out std_logic;
		D_ACK_I          : in  std_logic;		
		D_WE_O           : out std_logic;
		D_CTI_O          : out std_logic_vector( 2 downto 0);
		D_LOCK_O         : out std_logic;
		D_BTE_O          : out std_logic_vector( 1 downto 0);
		D_ERR_I          : in  std_logic;
		D_RTY_I          : in  std_logic
		
	);
end component;

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

component wb_uart is
	port (
		clk        : in  std_logic;
		reset      : in  std_logic;
		-- Wishbone slave
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
		-- Serial I/O ports
		uart_rx    : in  std_logic;
		uart_tx    : out std_logic );
end component wb_uart;

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
end component wb_gpio;


-----------------------------------------------------------------------------
-- Local Signals ------------------------------------------------------------
-----------------------------------------------------------------------------
signal iport     : std_logic_vector(31 downto 0);
signal oport     : std_logic_vector(31 downto 0);
signal intr_n    : std_logic_vector(31 downto 0);

signal d_adr_o   : std_logic_vector(31 downto 0);
signal d_dat_i   : std_logic_vector(31 downto 0);
signal d_dat_o   : std_logic_vector(31 downto 0);
signal d_sel_o   : std_logic_vector( 3 downto 0);
signal d_cyc_o   : std_logic;
signal d_stb_o   : std_logic;
signal d_ack_i   : std_logic;
signal d_we_o    : std_logic;
signal d_cti_o   : std_logic_vector( 2 downto 0);
signal d_lock_o  : std_logic;
signal d_bte_o   : std_logic_vector( 1 downto 0);
signal d_err_i   : std_logic;
signal d_rty_i   : std_logic;

signal i_adr_o   : std_logic_vector(31 downto 0);
signal i_dat_i   : std_logic_vector(31 downto 0);
signal i_dat_o   : std_logic_vector(31 downto 0);
signal i_sel_o   : std_logic_vector( 3 downto 0);
signal i_cyc_o   : std_logic;
signal i_stb_o   : std_logic;
signal i_ack_i   : std_logic;
signal i_we_o    : std_logic;
signal i_cti_o   : std_logic_vector( 2 downto 0);
signal i_lock_o  : std_logic;
signal i_bte_o   : std_logic_vector( 1 downto 0);
signal i_err_i   : std_logic;
signal i_rty_i    : std_logic;

signal bram0_adr_i   : std_logic_vector(31 downto 0);
signal bram0_dat_o   : std_logic_vector(31 downto 0);
signal bram0_dat_i   : std_logic_vector(31 downto 0);
signal bram0_sel_i   : std_logic_vector( 3 downto 0);
signal bram0_cyc_i   : std_logic;
signal bram0_stb_i   : std_logic;
signal bram0_ack_o   : std_logic;
signal bram0_we_i    : std_logic;

signal timer0_adr_i   : std_logic_vector(31 downto 0);
signal timer0_dat_o   : std_logic_vector(31 downto 0);
signal timer0_dat_i   : std_logic_vector(31 downto 0);
signal timer0_sel_i   : std_logic_vector( 3 downto 0);
signal timer0_cyc_i   : std_logic;
signal timer0_stb_i   : std_logic;
signal timer0_ack_o   : std_logic;
signal timer0_we_i    : std_logic;


signal uart0_adr_i   : std_logic_vector(31 downto 0);
signal uart0_dat_o   : std_logic_vector(31 downto 0);
signal uart0_dat_i   : std_logic_vector(31 downto 0);
signal uart0_sel_i   : std_logic_vector( 3 downto 0);
signal uart0_cyc_i   : std_logic;
signal uart0_stb_i   : std_logic;
signal uart0_ack_o   : std_logic;
signal uart0_we_i    : std_logic;

signal gpio0_adr_i   : std_logic_vector(31 downto 0);
signal gpio0_dat_o   : std_logic_vector(31 downto 0);
signal gpio0_dat_i   : std_logic_vector(31 downto 0);
signal gpio0_sel_i   : std_logic_vector( 3 downto 0);
signal gpio0_cyc_i   : std_logic;
signal gpio0_stb_i   : std_logic;
signal gpio0_ack_o   : std_logic;
signal gpio0_we_i    : std_logic;



-----------------------------------------------------------------------------
-- Implementation -----------------------------------------------------------
-----------------------------------------------------------------------------
begin

leds <= oport(7 downto 0);
iport(3 downto 0) <= btn;


-----------------------------------------------------------------------------
-- LM32 CPU -----------------------------------------------------------------
-----------------------------------------------------------------------------
cpu0: lm32_cpu
	Port map (
		clk_i        => clk,
		rst_i        => reset,
		--
		interrupt_n  => intr_n,
		--
		i_adr_o      => i_adr_o,
		i_dat_i      => i_dat_i,
		i_dat_o      => i_dat_o,
		i_sel_o      => i_sel_o,
		i_cyc_o      => i_cyc_o,
		i_stb_o      => i_stb_o,
		i_ack_i      => i_ack_i,
		i_we_o       => i_we_o,
		i_cti_o      => i_cti_o,
		i_lock_o     => i_lock_o,
		i_bte_o      => i_bte_o,
		i_err_i      => i_err_i,
		i_rty_i      => i_rty_i,		
		--
		d_adr_o      => d_adr_o,
		d_dat_i      => d_dat_i,
		d_dat_o      => d_dat_o,
		d_sel_o      => d_sel_o,
		d_cyc_o      => d_cyc_o,
		d_stb_o      => d_stb_o,
		d_ack_i      => d_ack_i,		
		d_we_o       => d_we_o,
		d_cti_o      => d_cti_o,
		d_lock_o     => d_lock_o,
		d_bte_o      => d_bte_o,
		d_err_i      => d_err_i,
		d_rty_i      => d_rty_i		
	);
	
bram0: wb_bram
		Port map (
			clk       => clk,
			reset     => reset,
			--
			wb_adr_i  => bram0_adr_i,
			wb_dat_o  => bram0_dat_o,
			wb_dat_i  => bram0_dat_i,
			wb_sel_i  => bram0_sel_i,
			wb_stb_i  => bram0_stb_i,
			wb_cyc_i  => bram0_cyc_i,
			wb_ack_o  => bram0_ack_o,
			wb_we_i   => bram0_we_i
		);
		
uart0: wb_uart
	Port map (
			clk       => clk,
			reset     => reset,
			--
			wb_adr_i  => uart0_adr_i,
			wb_dat_o  => uart0_dat_o,
			wb_dat_i  => uart0_dat_i,
			wb_sel_i  => uart0_sel_i,
			wb_stb_i  => uart0_stb_i,
			wb_cyc_i  => uart0_cyc_i,
			wb_ack_o  => uart0_ack_o,
			wb_we_i   => uart0_we_i,
			wb_rxirq_o=> open,
			wb_txirq_o=> open,			
			--
			uart_rx   => uart_rx,
			uart_tx   => uart_tx
	);
	
gpio0: wb_gpio
	Port map (
			clk       => clk,
			reset     => reset,
			--
			wb_adr_i  => gpio0_adr_i,
			wb_dat_o  => gpio0_dat_o,
			wb_dat_i  => gpio0_dat_i,
			wb_sel_i  => gpio0_sel_i,
			wb_stb_i  => gpio0_stb_i,
			wb_cyc_i  => gpio0_cyc_i,
			wb_ack_o  => gpio0_ack_o,
			wb_we_i   => gpio0_we_i,
			--
			iport     => oport,
			oport     => iport
	);
	


-----------------------------------------------------------------------------
-- Wishbone Interconnection -------------------------------------------------
-----------------------------------------------------------------------------
wb0: intercon 
	Port map (
	-- wishbone master port(s)
	-- lm32i
  lm32i_dat_i   => i_dat_i,
  lm32i_ack_i   => i_ack_i,
  lm32i_err_i   => i_err_i,
  lm32i_rty_i   => i_rty_i,
  lm32i_dat_o   => i_dat_o,
  lm32i_we_o    => i_we_o,
  lm32i_sel_o   => i_sel_o,
  lm32i_adr_o   => i_adr_o,
  lm32i_cyc_o   => i_cyc_o,
  lm32i_stb_o   => i_stb_o,
  -- lm32d
  lm32d_dat_i   => d_dat_i,
  lm32d_ack_i   => d_ack_i,
  lm32d_err_i   => d_err_i,
  lm32d_rty_i   => d_rty_i,
  lm32d_dat_o   => d_dat_o,
  lm32d_we_o    => d_we_o,
  lm32d_sel_o   => d_sel_o,
  lm32d_adr_o   => d_adr_o,
  lm32d_cyc_o   => d_cyc_o,
  lm32d_stb_o   => d_stb_o,
  -- wishbone slave port(s)
  -- wb_bram
  bram0_dat_o => bram0_dat_o,
  bram0_ack_o => bram0_ack_o,
  bram0_dat_i => bram0_dat_i,
  bram0_we_i  => bram0_we_i,
  bram0_sel_i => bram0_sel_i,
  bram0_adr_i => bram0_adr_i,
  bram0_cyc_i => bram0_cyc_i,
  bram0_stb_i => bram0_stb_i,
  -- timer0
  timer0_dat_o => timer0_dat_o,
  timer0_ack_o => timer0_ack_o,
  timer0_dat_i => timer0_dat_i,
  timer0_we_i  => timer0_we_i,
  timer0_sel_i => timer0_sel_i,
  timer0_adr_i => timer0_adr_i,
  timer0_cyc_i => timer0_cyc_i,
  timer0_stb_i => timer0_stb_i,
  -- uart0
  uart0_dat_o => uart0_dat_o,
  uart0_ack_o => uart0_ack_o,
  uart0_dat_i => uart0_dat_i,
  uart0_we_i  => uart0_we_i,
  uart0_sel_i => uart0_sel_i,
  uart0_adr_i => uart0_adr_i,
  uart0_cyc_i => uart0_cyc_i,
  uart0_stb_i => uart0_stb_i,
  -- gpio0
  gpio0_dat_o => gpio0_dat_o,
  gpio0_ack_o => gpio0_ack_o,
  gpio0_dat_i => gpio0_dat_i,
  gpio0_we_i  => gpio0_we_i,
  gpio0_sel_i => gpio0_sel_i,
  gpio0_adr_i => gpio0_adr_i,
  gpio0_cyc_i => gpio0_cyc_i,
  gpio0_stb_i => gpio0_stb_i,
  -- clock and reset
  clk           => clk,
  reset         => reset
	);
	
intr_n <= (others => '1');

end rtl;
