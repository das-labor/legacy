library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.ALL;

-----------------------------------------------------------------------------
-- Wishbone UART ------------------------------------------------------------
entity wb_uart is
	generic (
		uart_div   : in  integer := 286 );  -- default to 56700 baud
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
end wb_uart;



-----------------------------------------------------------------------------
-- 0x00 Status Register
-- 0x04 RX / TX Data
--
-- Status Register:
-- 
--       +-------------+----------+----------+---------+---------+
--       |  ... 0 ...  | TX_IRQEN | RX_IRQEN | TX_BUSY | RX_FULL |
--       +-------------+----------+----------+---------+---------+
--
-----------------------------------------------------------------------------
-- Implementation -----------------------------------------------------------
architecture rtl of wb_uart is

-----------------------------------------------------------------------------
-- Components ---------------------------------------------------------------
component myuart is
	generic (
		halfbit   : integer := 217;
		fullbit   : integer := 434 );
	port (
		clk       : in  std_logic;
		reset     : in  std_logic;
		--
		txdata    : in  std_logic_vector(7 downto 0);
		rxdata    : out std_logic_vector(7 downto 0);
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
end component;


-----------------------------------------------------------------------------
-- Local Signals ------------------------------------------------------------

constant ZEROS  : std_logic_vector(31 downto 0) := (others => '0');

signal wr         : std_logic;
signal rd         : std_logic;
signal rx_avail   : std_logic;
signal tx_avail   : std_logic;
signal rxdata     : std_logic_vector(7 downto 0);
signal txdata     : std_logic_vector(7 downto 0);

signal status_reg : std_logic_vector(31 downto 0);
signal data_reg   : std_logic_vector(31 downto 0);

signal tx_irqen   : std_logic;
signal rx_irqen   : std_logic;

begin


wb_rxirq_o <= rx_avail and rx_irqen;
wb_txirq_o <= tx_avail and tx_irqen;

uart0: myuart
	port map (
		clk       => clk,
		reset     => reset,
        -- Sync Interface
		txdata    => txdata,
		rxdata    => rxdata,
		wr        => wr,
		rd        => rd,
		tx_avail  => tx_avail,
		tx_busy   => open,
		rx_avail  => rx_avail,
		rx_full   => open,
		rx_error  => open,
		-- Async Interface
		uart_txd  => uart_tx,
		uart_rxd  => uart_rx );


status_reg <= ZEROS(31 downto 2) & not tx_avail & rx_avail;
data_reg   <= ZEROS(31 downto 8) & rxdata;

wb_dat_o <= status_reg when wb_stb_i='1' and wb_adr_i(3 downto 0)=x"0" else
            data_reg   when wb_stb_i='1' and wb_adr_i(3 downto 0)=x"4" else
            (others => '-');

rd <= '1' when wb_stb_i='1' and wb_adr_i(3 downto 0)=x"4" and wb_we_i='0' else
      '0';

wr <= '1' when wb_stb_i='1' and wb_adr_i(3 downto 0)=x"4" and wb_we_i='1' else
      '0';

txdata <= wb_dat_i(7 downto 0);

wb_ack_o <= wb_stb_i; 


proc: process(reset, clk) is
begin
	if reset='1' then
		tx_irqen <= '0';
		rx_irqen <= '0';
	elsif clk'event and clk='1' then
		-- WB write request
		if wb_stb_i='1' and wb_we_i='1' then
			if wb_adr_i(3 downto 0)=x"0" then
				tx_irqen <= wb_dat_i(3);
				rx_irqen <= wb_dat_i(2);
			end if;
		end if;
	end if;
end process;


end rtl;

