-----------------------------------------------------------------------------
-- Simple UART without hardware Flowcontrol und with 1 byte buffer.
--   by Joerg Bornschein  (jb@capsec.org)
--
-- halfbit anf fullbit parametrize the boundrate:
--
--     fullbit = 50 MHz / 115000 Baud
--     halfbit = fullbit / 2;
--
-- This module was NOT intensely --  but works for me
--
-- All files under GPLv2   
-----------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.all;

-----------------------------------------------------------------------------
-- UART ---------------------------------------------------------------------
entity uart is
	generic (
		halfbit   : integer := 217;
		fullbit   : integer := 434 );
	port (
		clk       : in  std_logic;
		reset     : in  std_logic;
		--
		din       : in  std_logic_vector(7 downto 0);
		dout      : out std_logic_vector(7 downto 0);
		wr        : in  std_logic;
		rd        : in  std_logic;
		tx_busy   : out std_logic;
		tx_free   : out std_logic;
		rx_avail  : out std_logic;
		rx_full   : out std_logic;
		rx_error  : out std_logic;
		-- 
		uart_rxd  : in  std_logic;
		uart_txd  : out std_logic );
end uart;

-----------------------------------------------------------------------------
-- implementation -----------------------------------------------------------
architecture rtl of uart is

-----------------------------------------------------------------------------
-- component declarations ---------------------------------------------------
component uart_rx is
	generic (
		fullbit  : integer := 434;
		halfbit  : integer := 217 );
	port (
		clk      : in  std_logic;
		reset    : in  std_logic;
		--
		dout     : out std_logic_vector(7 downto 0);
		full     : out std_logic;
		error    : out std_logic;
		clear    : in  std_logic;
		--
		rxd      : in  std_logic );
end component;

component uart_tx is
	generic (
		fullbit  : integer := 434;
		halfbit  : integer := 217 );
	port (
		clk      : in  std_logic;
		reset    : in  std_logic;
		--
		din      : in  std_logic_vector(7 downto 0);
		wr       : in  std_logic;
		busy     : out std_logic;
		--
		txd      : out std_logic );
end component;		

-----------------------------------------------------------------------------
-- local signals ------------------------------------------------------------

signal busy   : std_logic;
signal full   : std_logic;

signal txbuf : std_logic_vector(7 downto 0);
signal rxbuf : std_logic_vector(7 downto 0);

begin


txproc: process(clk, reset) is
begin
	if reset='1' then
		wr     <= '0';
	elsif clk'event and clk='1' then
		if wr='1' then
			txbuf     <= din;
			txbuffull <= '1';
		end if;
		
		if txbuffull='1' and  then
			uart_wr   <= '1';
			
		end if;
	end if;
end process;


uart_rx0: uart_rx 
	generic map (
		fullbit => fullbit,
		halfbit => halfbit )
	port map (
		clk     => clk,
		reset   => reset,
		--
		dout    => dout,
		full    => full,
		error   => rx_error,
		clear   => rd,
		--
		rxd     => uart_rxd );
		
uart_tx0: uart_tx 
	generic map (
		fullbit => fullbit,
		halfbit => halfbit )
	port map (
		clk     => clk,
		reset   => reset,
		--
		din     => din,
		wr      => wr,
		busy    => busy,
		--
		txd     => uart_txd );
		
rx_full <= full and not rd;
tx_busy <= busy or wr;

end rtl;

