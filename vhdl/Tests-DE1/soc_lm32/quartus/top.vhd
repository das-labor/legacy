library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.numeric_std.all;

entity Top is
	Port (
		CLOCK_50 : in  std_logic;
		LEDG     : out std_logic_vector(7 downto 0);
		LEDR     : out std_logic_vector(9 downto 0);
		SW       : in  std_logic_vector(9 downto 0);
		KEY      : in  std_logic_vector(3 downto 0);
		HEX0     : out std_logic_vector(6 downto 0);
		HEX1     : out std_logic_vector(6 downto 0);
		HEX2     : out std_logic_vector(6 downto 0);
		HEX3     : out std_logic_vector(6 downto 0);
		UART_RXD : in  std_logic;
		UART_TXD : out std_logic;
		
		SRAM_OE_N : out std_logic;
	  	SRAM_WE_N : out std_logic;
	  	SRAM_UB_N : out std_logic;
	  	SRAM_LB_N : out std_logic;
	    SRAM_CE_N : out std_logic;
	  	SRAM_ADDR : out std_logic_vector(17 downto 0);
	    SRAM_DQ   : inout std_logic_vector(15 downto 0)
	);
	
end Top;

architecture rtl of Top is

component System is
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
	    sram_dq   : inout std_logic_vector(15 downto 0)
	);
end component;

signal clock     : std_logic;
signal reset     : std_logic;
signal cnt       : unsigned(6 downto 0);
		
begin

sys0: System
	Port map (
	clk		=> clock,      
	reset	=> reset,   

	leds	=> LEDG,
	hex0 	=> HEX0,
	hex1    => HEX1,
	hex2    => HEX2,
	hex3    => HEX3,
	btn     => SW,
	uart_rx => uart_rxd,
	uart_tx => uart_txd,
	
	sram_oe_n => SRAM_OE_N,
	sram_we_n => SRAM_WE_N,
	sram_ub_n => SRAM_UB_N,
	sram_lb_n => SRAM_LB_N,
	sram_ce_n => SRAM_CE_N,
	sram_addr => SRAM_ADDR,
	sram_dq   => SRAM_DQ
	);
	
clkproc: process(CLOCK_50) is
begin
	if CLOCK_50'event and CLOCK_50='1' then
		cnt <= cnt + 1;
	end if;
end process;

clock <= '1' when (cnt(0)='1' and KEY(0)='1') or KEY(1)='0' else
         '0';

reset <= not KEY(3);

end rtl;
