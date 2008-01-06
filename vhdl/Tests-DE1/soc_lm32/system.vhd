library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity System is
	Port (
		clk      : in  std_logic;
		reset    : in  std_logic;
		-- 
		leds     : out std_logic_vector(7 downto 0);
		hex0     : out std_logic_vector(6 downto 0);
		hex1     : out std_logic_vector(6 downto 0);
		hex2     : out std_logic_vector(6 downto 0);
		hex3     : out std_logic_vector(6 downto 0);
		btn      : in  std_logic_vector(9 downto 0);
		uart_rx  : in  std_logic;
		uart_tx  : out std_logic;
		
		sram_oe_n : out std_logic;
	  	sram_we_n : out std_logic;
	  	sram_ub_n : out std_logic;
	  	sram_lb_n : out std_logic;
		sram_ce_n : out std_logic;
	  	sram_addr : out std_logic_vector(17 downto 0);
		sram_dq   : inout std_logic_vector(15 downto 0);
	
		-- farbborg interface
		clk_pwm    : in  std_logic;
      
		-- Latch shift-register
	  	lsr_clr    : out std_logic;  
		lsr_d      : out std_logic;
		lsr_c      : out std_logic;
      
	  	latch_data : out std_logic_vector(7 downto 0);
	 
	  	-- Plane shift-register	
	  	psr_c      : out std_logic;
	  	psr_d      : out std_logic;
	 
	  	-- May be debug stuff
	  	col_enable : out std_logic;
	  	plane_out  : out std_logic_vector(4 downto 0)
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
  -- sram0
  sram0_dat_i : out std_logic_vector(31 downto 0);
  sram0_ack_o : in  std_logic;
  sram0_dat_o : in  std_logic_vector(31 downto 0);
  sram0_we_i  : out std_logic;
  sram0_sel_i : out std_logic_vector(3 downto 0);
  sram0_adr_i : out std_logic_vector(31 downto 0);
  sram0_cyc_i : out std_logic;
  sram0_stb_i : out std_logic;
  sram0_rty_o : in  std_logic;
  sram0_err_o : in  std_logic;  
  -- timer0
  timer0_dat_o : in  std_logic_vector(31 downto 0);
  timer0_ack_o : in  std_logic;
  timer0_dat_i : out std_logic_vector(31 downto 0);
  timer0_we_i  : out std_logic;
  timer0_sel_i : out std_logic_vector( 3 downto 0);
  timer0_adr_i : out std_logic_vector(31 downto 0);
  timer0_cyc_i : out std_logic;
  timer0_stb_i : out std_logic;
  -- uart0
  uart0_dat_o : in  std_logic_vector(31 downto 0);
  uart0_ack_o : in  std_logic;
  uart0_dat_i : out std_logic_vector(31 downto 0);
  uart0_we_i  : out std_logic;
  uart0_sel_i : out std_logic_vector( 3 downto 0);
  uart0_adr_i : out std_logic_vector(31 downto 0);
  uart0_cyc_i : out std_logic;
  uart0_stb_i : out std_logic;
  -- gpio
  gpio0_dat_o : in  std_logic_vector(31 downto 0);
  gpio0_ack_o : in  std_logic;
  gpio0_dat_i : out std_logic_vector(31 downto 0);
  gpio0_we_i  : out std_logic;
  gpio0_sel_i : out std_logic_vector( 3 downto 0);
  gpio0_adr_i : out std_logic_vector(31 downto 0);
  gpio0_cyc_i : out std_logic;
  gpio0_stb_i : out std_logic;
  -- farbborg0
  farbborg0_adr_i   : out std_logic_vector(31 downto 0); 
  farbborg0_dat_o   : in  std_logic_vector(31 downto 0);
  farbborg0_dat_i   : out std_logic_vector(31 downto 0);
  farbborg0_sel_i   : out std_logic_vector( 3 downto 0);
  farbborg0_cyc_i   : out std_logic;
  farbborg0_stb_i   : out std_logic;
  farbborg0_ack_o   : in  std_logic;
  farbborg0_we_i    : out std_logic;	

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
end component wb_timer;

component uart_core is
	port (
		clk        : in  std_logic;
		reset      : in  std_logic;
		-- Wishbone slave
		UART_ADR_I : in  std_logic_vector(31 downto 0);
		UART_DAT_I : in  std_logic_vector(31 downto 0);
		UART_DAT_O : out std_logic_vector(31 downto 0);
		UART_SEL_I : in  std_logic_vector( 3 downto 0);
		UART_STB_I : in  std_logic;
		UART_CYC_I : in  std_logic;
		UART_ACK_O : out std_logic;
		UART_WE_I  : in  std_logic;
		INTR	   : out std_logic;
		TXRDY_N    : out std_logic;
		
		SIN        : in  std_logic;
		SOUT       : out std_logic);
end component uart_core;

component asram_top is
	port(
        clk_i        : in  std_logic;
        rst_i        : in  std_logic;
                
        ASRAM_CTI_I  : in  std_logic_vector(2 downto 0); 
        ASRAM_BTE_I  : in  std_logic_vector(1 downto 0);
        ASRAM_ADR_I  : in  std_logic_vector(31 downto 0);
        ASRAM_DAT_I  : in  std_logic_vector(31 downto 0);
        ASRAM_SEL_I  : in  std_logic_vector( 3 downto 0);
        ASRAM_WE_I   : in  std_logic;
        ASRAM_STB_I  : in  std_logic;
        ASRAM_CYC_I  : in  std_logic;
        ASRAM_LOCK_I : in  std_logic;
        ASRAM_ACK_O  : out std_logic;
        ASRAM_DAT_O  : out std_logic_vector(31 downto 0);
        ASRAM_ERR_O  : out std_logic;
        ASRAM_RTY_O  : out std_logic;
        
        -- SRAM side interface
        sram_addr    : out std_logic_vector(17 downto 0);
        sram_data_in : in  std_logic_vector(15 downto 0);
        sram_data_out: out std_logic_vector(15 downto 0);
        sram_csn     : out std_logic;
        sram_wen     : out std_logic;
        sram_oen     : out std_logic;
        sram_be      : out std_logic_vector(1 downto 0)
		);
end component asram_top;

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

component wb_farbborg is
   port (
      clk        : in  std_logic;
      reset      : in  std_logic;
     
      -- Wishbone bus
      wb_adr_i   : in  std_logic_vector(12 downto 0);
      wb_dat_i   : in  std_logic_vector( 7 downto 0);
      wb_dat_o   : out std_logic_vector( 7 downto 0);
      wb_sel_i   : in  std_logic_vector( 3 downto 0);
      wb_cyc_i   : in  std_logic;
      wb_stb_i   : in  std_logic;
      wb_ack_o   : out std_logic;
      wb_we_i    : in  std_logic;
      
      -- farbborg interface
      clk_pwm    : in  std_logic;
      
      -- Latch shift-register
	  lsr_clr    : out std_logic;  
      lsr_d      : out std_logic;
      lsr_c      : out std_logic;
      
      latch_data : out std_logic_vector(7 downto 0);
	 
	  -- Plane shift-register	
      psr_c      : out std_logic;
	  psr_d      : out std_logic;
		 
	  col_enable : out std_logic;
	
	  -- May be debug stuff
	  plane_out  : out std_logic_vector(4 downto 0)
       );
end component;


component nibble7Seg is
   port (

      nibble0  : in  std_logic_vector(3 downto 0);
	  nibble1  : in  std_logic_vector(3 downto 0);
	  sel      : in  std_logic;
	  hex      : out std_logic_vector(6 downto 0)	
        );
end component nibble7Seg;
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
signal timer0_irq0    : std_logic;
signal timer0_irq1    : std_logic;

signal uart0_adr_i   : std_logic_vector(31 downto 0);
signal uart0_dat_o   : std_logic_vector(31 downto 0);
signal uart0_dat_i   : std_logic_vector(31 downto 0);
signal uart0_sel_i   : std_logic_vector( 3 downto 0);
signal uart0_cyc_i   : std_logic;
signal uart0_stb_i   : std_logic;
signal uart0_ack_o   : std_logic;
signal uart0_we_i    : std_logic;
signal uart0_irq_rx  : std_logic;
signal uart0_irq_tx  : std_logic;	

signal gpio0_adr_i   : std_logic_vector(31 downto 0);
signal gpio0_dat_o   : std_logic_vector(31 downto 0);
signal gpio0_dat_i   : std_logic_vector(31 downto 0);
signal gpio0_sel_i   : std_logic_vector( 3 downto 0);
signal gpio0_cyc_i   : std_logic;
signal gpio0_stb_i   : std_logic;
signal gpio0_ack_o   : std_logic;
signal gpio0_we_i    : std_logic;

signal farbborg0_adr_i   : std_logic_vector(31 downto 0);
signal farbborg0_dat_o   : std_logic_vector(31 downto 0);
signal farbborg0_dat_i   : std_logic_vector(31 downto 0);
signal farbborg0_sel_i   : std_logic_vector( 3 downto 0);
signal farbborg0_cyc_i   : std_logic;
signal farbborg0_stb_i   : std_logic;
signal farbborg0_ack_o   : std_logic;
signal farbborg0_we_i    : std_logic;

signal sram0_adr_i   : std_logic_vector(31 downto 0);
signal sram0_dat_o   : std_logic_vector(31 downto 0);
signal sram0_dat_i   : std_logic_vector(31 downto 0);
signal sram0_sel_i   : std_logic_vector( 3 downto 0);
signal sram0_cti_i   : std_logic_vector( 2 downto 0);
signal sram0_cyc_i   : std_logic;
signal sram0_stb_i   : std_logic;
signal sram0_ack_o   : std_logic;
signal sram0_rty_o   : std_logic;
signal sram0_err_o   : std_logic;
signal sram0_we_i    : std_logic;
signal sram0_dq_i    : std_logic_vector(15 downto 0);
signal sram0_dq_o    : std_logic_vector(15 downto 0);
signal sram0_we_n    : std_logic;

signal hex_view      : std_logic_vector(31 downto 0);

-------------------------------------------------------------------------
-- Implementation -----------------------------------------------------------
-----------------------------------------------------------------------------
begin

--leds <= oport(7 downto 0);
iport(3 downto 0) <= btn(3 downto 0);
leds <= clk & reset &  uart0_stb_i & uart0_ack_o & 
        uart0_irq_rx & uart0_irq_tx & uart0_we_i & uart0_we_i;

sram_dq    <= sram0_dq_o when sram0_we_n='0' else 
	          (others => 'Z');
sram0_dq_i <= sram_dq;

sram_we_n  <= sram0_we_n;
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
		
uart0: uart_core
	Port map (
			clk       => clk,
			reset     => reset,
			--
			UART_ADR_I => uart0_adr_i,
			UART_DAT_I => uart0_dat_i,
			UART_DAT_O => uart0_dat_o,
			UART_SEL_I => uart0_sel_i,
			UART_STB_I => uart0_stb_i,
			UART_CYC_I => uart0_cyc_i,
			UART_ACK_O => uart0_ack_o,
			UART_WE_I  => uart0_we_i,
			INTR	   => uart0_irq_rx,
			TXRDY_N    => uart0_irq_tx,			
			--
			SIN        => uart_rx,
			SOUT       => uart_tx
	);

sram0: asram_top
	Port map (
		clk_i        => clk,
		rst_i        => reset,
        
		ASRAM_CTI_I  => (others => '0'),
		ASRAM_BTE_I  => (others => '0'),
		ASRAM_ADR_I  => sram0_adr_i,
		ASRAM_DAT_I  => sram0_dat_i,
		ASRAM_SEL_I  => sram0_sel_i,
		ASRAM_WE_I   => sram0_we_i,
		ASRAM_STB_I  => sram0_stb_i,
		ASRAM_CYC_I  => sram0_cyc_i,
		ASRAM_LOCK_I => '0',
		ASRAM_ACK_O  => sram0_ack_o,
		ASRAM_DAT_O  => sram0_dat_o,
		ASRAM_ERR_O  => sram0_err_o,
		ASRAM_RTY_O  => sram0_rty_o,

		-- SRAM side 
		sram_addr    => sram_addr,
		sram_data_in => sram0_dq_i,
		sram_data_out=> sram0_dq_o,
		sram_csn     => sram_ce_n,
		sram_wen     => sram0_we_n,
		sram_oen     => sram_oe_n,  
		sram_be(1)   => sram_ub_n,
		sram_be(0)   => sram_lb_n	   
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

farbborg0 : wb_farbborg 
   port map (
      clk        	=> clk,
      reset         => reset,
     
      wb_adr_i      => farbborg0_adr_i(12 downto 0), 
      wb_dat_i      => farbborg0_dat_o( 7 downto 0), 
      wb_dat_o      => farbborg0_dat_i( 7 downto 0), 
      wb_sel_i      => farbborg0_sel_i, 
      wb_cyc_i      => farbborg0_cyc_i, 
      wb_stb_i      => farbborg0_stb_i, 
      wb_ack_o      => farbborg0_ack_o, 
      wb_we_i       => farbborg0_we_i,  
      
      clk_pwm       => clk_pwm,
      
	  lsr_clr       => lsr_clr,
      lsr_d         => lsr_d,
      lsr_c         => lsr_c,
         
      latch_data    => latch_data,

      psr_c         => psr_c,
	  psr_d         => psr_d,
		 
	  col_enable    => col_enable,
	
	  plane_out     => plane_out
       );
	
timer0: wb_timer
	Port map (
			clk       => clk,
			reset     => reset,
			--
			wb_adr_i  => timer0_adr_i,
			wb_dat_o  => timer0_dat_o,
			wb_dat_i  => timer0_dat_i,
			wb_sel_i  => timer0_sel_i,
			wb_stb_i  => timer0_stb_i,
			wb_cyc_i  => timer0_cyc_i,
			wb_ack_o  => timer0_ack_o,
			wb_we_i   => timer0_we_i,
			wb_irq0_o => timer0_irq0,
			wb_irq1_o => timer0_irq1
	);
	
hex_view <= i_adr_o when btn(8 downto 7)="00" else
            d_adr_o when btn(8 downto 7)="01" else
            i_dat_i when btn(8 downto 6)="100" else
            i_dat_o when btn(8 downto 6)="101" else
            d_dat_i when btn(8 downto 6)="110" else
            d_dat_o;
            
           
digit0:nibble7Seg
	Port map (
		sel       => btn(9),
		nibble0   => hex_view(3 downto 0),
		nibble1   => hex_view(19 downto 16),
		hex       => hex0	 	
			 );

digit1:nibble7Seg
	Port map (
		sel       => btn(9),
		nibble0   => hex_view(7 downto 4),
		nibble1   => hex_view(23 downto 20),
		hex       => hex1	 	
			 );
digit2:nibble7Seg
	Port map (
		sel       => btn(9),
		nibble0   => hex_view(11 downto 8),
		nibble1   => hex_view(27 downto 24),
		hex       => hex2	 	
			 );
digit3:nibble7Seg
	Port map (
		sel       => btn(9),
		nibble0   => hex_view(15 downto 12),
		nibble1   => hex_view(31 downto 28),
		hex       => hex3	 	
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
  -- sram0
  sram0_dat_o  => sram0_dat_o, 
  sram0_ack_o  => sram0_ack_o,  
  sram0_dat_i  => sram0_dat_i, 
  sram0_we_i   => sram0_we_i,  
  sram0_sel_i  => sram0_sel_i, 
  sram0_adr_i  => sram0_adr_i, 
  sram0_cyc_i  => sram0_cyc_i, 
  sram0_stb_i  => sram0_stb_i, 
  sram0_rty_o  => sram0_rty_o,
  sram0_err_o  => sram0_err_o, 
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
  -- farbborg0
  farbborg0_adr_i => farbborg0_adr_i,     
  farbborg0_dat_o => farbborg0_dat_o, 
  farbborg0_dat_i => farbborg0_dat_i, 
  farbborg0_sel_i => farbborg0_sel_i, 
  farbborg0_cyc_i => farbborg0_cyc_i, 
  farbborg0_stb_i => farbborg0_stb_i, 
  farbborg0_ack_o => farbborg0_ack_o, 
  farbborg0_we_i  => farbborg0_we_i,  
  -- clock and reset
  clk           => clk,
  reset         => reset
	);
	
intr_n <= x"0000000" & timer0_irq1 & timer0_irq0 & uart0_irq_rx & uart0_irq_tx;

end rtl;
