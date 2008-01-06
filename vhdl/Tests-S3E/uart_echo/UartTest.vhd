-----------------------------------------------------------------------------
-- Test Design for UART -- Echo Test
--
-- See uart.vhd for additional documentation
-----------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.all;

entity UartTest is
	port (
		clk         : in  std_logic;
		reset       : in  std_logic;
		-- 
		uart_rxd    : in  std_logic;
		uart_txd    : out std_logic );
end UartTest;

architecture rtl of UartTest is

-----------------------------------------------------------------------------
-- Component declaration ----------------------------------------------------
component myuart is
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
		rx_full   : out std_logic;
		rx_error  : out std_logic;
		-- 
		uart_rxd  : in  std_logic;
		uart_txd  : out std_logic );
end component;


-----------------------------------------------------------------------------
signal rd        : std_logic;
signal wr        : std_logic;
signal rx_full   : std_logic;
signal rx_error  : std_logic;
signal tx_busy   : std_logic;
signal odata     : std_logic_vector(7 downto 0);
signal idata     : std_logic_vector(7 downto 0);
signal data_buf  : std_logic_vector(7 downto 0);
signal buf_full  : std_logic;

signal uart_rxd2 : std_logic;

signal count : unsigned(4 downto 0);

begin

syncproc: process(reset, clk) is
begin
	if reset='1' then
		uart_rxd2 <= '1';
	elsif clk'event and clk='1' then
		uart_rxd2 <= uart_rxd;
	end if;
end process;


uart0: myuart
	generic map (
		fullbit   => 434,
		halfbit   => 170 )
	port map (
		clk       => clk,
		reset     => reset,
		-- bus interface
		din       => idata,
		dout      => odata,
		wr        => wr,
		rd        => rd,
		tx_busy   => tx_busy,
		rx_full   => rx_full,
		rx_error  => rx_error,
		-- 
		uart_txd  => uart_txd,
		uart_rxd  => uart_rxd2 );
		



--proc: process(reset, clk) is
--begin
--	if reset='1' then
--		count <= "00000";
--		wr    <= '0';
--		rd    <= '0';
--	elsif clk'event and clk='1' then
--		if tx_busy='0' then
--			if count="00000" then 		
--				idata <= x"0D";
--			elsif count="00001" then 		
--				idata <= x"0A";
--			else
--				idata <= x"61";			
--			end if;
--			
--			if count="11111" then
--				count <= "00000";
--			else
--				count <= count + 1;		
--			end if;
--			
--			wr <= '1';
--		else
--			wr <= '0';		
--		end if;
--	end if;
--end process;


proc: process(clk, reset) is
begin
	if reset='1' then
		buf_full <= '0';
		data_buf <= (others => '0');
		rd       <= '0';
		wr       <= '0';		
	elsif clk'event and clk='1' then
		if rx_full='1' then
			data_buf <= odata;
			buf_full <= '1';
			rd       <= '1';
		else
			rd       <= '0';		
		end if;
		
		if tx_busy='0' and buf_full='1' then
			idata    <= data_buf;
			wr       <= '1';
			buf_full <= '0';
		else 
			wr       <= '0';
		end if;
	end if;
end process;


end rtl;

