library ieee;
use ieee.std_logic_1164.all;

-----------------------------------------------------------------------------
-- UART ---------------------------------------------------------------------
entity myuart is
	port (
		clk       : in  std_logic;
		reset     : in  std_logic;
		--
		divisor   : in  std_logic_vector(15 downto 0);
		txdata    : in  std_logic_vector( 7 downto 0);
		rxdata    : out std_logic_vector( 7 downto 0);
		wr        : in  std_logic;
		rd        : in  std_logic;
		tx_avail  : out std_logic;
		tx_busy   : out std_logic;
		rx_avail  : out std_logic;
		rx_full   : out std_logic;
		rx_error  : out std_logic;
		-- 
		uart_rxd  : in  std_logic;
		uart_txd  : out std_logic );
end myuart;

-----------------------------------------------------------------------------
-- implementation -----------------------------------------------------------
architecture rtl of myuart is

-----------------------------------------------------------------------------
-- component declarations ---------------------------------------------------
component uart_rx is
	port (
		clk      : in  std_logic;
		reset    : in  std_logic;
		--
		divisor  : in  std_logic_vector(15 downto 0);
		dout     : out std_logic_vector(7 downto 0);
		avail    : out std_logic;
		error    : out std_logic;
		clear    : in  std_logic;
		--
		rxd      : in  std_logic );
end component;

component uart_tx is
	port (
		clk      : in  std_logic;
		reset    : in  std_logic;
		--  
		divisor  : in  std_logic_vector(15 downto 0);
		din      : in  std_logic_vector(7 downto 0);
		wr       : in  std_logic;
		busy     : out std_logic;
		--
		txd      : out std_logic );
end component;		

-----------------------------------------------------------------------------
-- local signals ------------------------------------------------------------
signal utx_busy   : std_logic;
signal utx_wr     : std_logic;

signal urx_dout   : std_logic_vector(7 downto 0);
signal urx_avail  : std_logic;
signal urx_clear  : std_logic;
signal urx_error  : std_logic;

signal txbuf      : std_logic_vector(7 downto 0);
signal txbuf_full : std_logic;
signal rxbuf      : std_logic_vector(7 downto 0);
signal rxbuf_full : std_logic;

begin

iotxproc: process(clk) is
begin
	if clk'event and clk='1' then
		if reset='1' then
			utx_wr     <= '0';
			txbuf_full <= '0';
			
			urx_clear  <= '0';
			rxbuf_full <= '0';
		else
			-- TX Buffer Logic
			if  wr='1' and not txbuf_full='1' then
				txbuf      <= txdata;
				txbuf_full <= '1';
			end if;
			
			if txbuf_full='1' and utx_busy='0' then
				utx_wr     <= '1';
				txbuf_full <= '0';
			else
				utx_wr     <= '0';		
			end if;
			
			-- RX Buffer Logic
			if rd='1' then
				rxbuf_full <= '0';
			end if;
			
			if urx_avail='1' and rxbuf_full='0' then 
				rxbuf      <= urx_dout;
				rxbuf_full <= '1';
				urx_clear  <= '1';
			else
				urx_clear  <= '0';
			end if;
		end if;
	end if;
end process;

rxdata   <= rxbuf;
rx_avail <= rxbuf_full and not rd;
rx_full  <= rxbuf_full and urx_avail and not rd;
rx_error <= urx_error;

tx_busy  <= utx_busy or txbuf_full; -- or wr;
tx_avail <= not txbuf_full;

-- Instantiate RX and TX engine
uart_rx0: uart_rx 
	port map (
		clk     => clk,
		reset   => reset,
		--
		divisor => divisor,
		dout    => urx_dout,
		avail   => urx_avail,
		error   => urx_error,
		clear   => urx_clear,
		--
		rxd     => uart_rxd );
		
uart_tx0: uart_tx 
	port map (
		clk     => clk,
		reset   => reset,
		--
		divisor => divisor,
		din     => txbuf,
		wr      => utx_wr,
		busy    => utx_busy,
		--
		txd     => uart_txd );
end rtl;
