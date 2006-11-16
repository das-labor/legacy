library ieee;
use ieee.std_logic_1164.ALL;
use ieee.numeric_std.all;

entity my_uart is
	generic (
		uart_div : in  integer := 2600 );  -- default to 19200 baud
	Port (
		clk      : in  std_logic;
		reset    : in  std_logic;
		-- Data bus
		cs       : in  std_logic;
		we       : in  std_logic;
		data_in  : in  std_logic_vector(7 downto 0);
		data_out : out std_logic_vector(7 downto 0);
		rx_full  : out std_logic;
		tx_busy_n: out std_logic;
		-- Serial IN/OUT
		rxd      : in  std_logic;
		txd      : out std_logic );
end my_uart;

architecture rtl of my_uart is

	signal baudreg : unsigned(11 downto 0);

	signal tx_baudcount : unsigned(11 downto 0);
	signal tx_baudclk   : std_logic;

	signal tx_reg       : std_logic_vector(7 downto 0);
	signal tx_buf       : std_logic_vector(7 downto 0);
	signal tx_phase     : std_logic_vector(1 downto 0);
	signal tx_count     : unsigned(3 downto 0);
	
	signal rx_baudcount : unsigned(11 downto 0);
	signal rx_baudclk   : std_logic;

	signal rx_reg       : std_logic_vector(7 downto 0);
	signal rx_buf       : std_logic_vector(7 downto 0);
	signal rx_active    : std_logic;
	signal rx_phase     : std_logic;
	signal rx_count     : unsigned(3 downto 0);
	
	
	signal rx_flag : std_logic;
	signal c_rx_flag : std_logic;
	signal s_rx_flag : std_logic;
	
	signal tx_flag : std_logic;
	signal c_tx_flag : std_logic;

begin

	--tx baudrate generator
	process(clk, reset)
	begin
		if reset = '1' then
			tx_baudclk <= '0';
			tx_baudcount <= (others => '0');
			tx_flag <= '1';
			txd <= '1';
			tx_phase <= "00";
		elsif clk'event and clk = '1' then
			tx_baudclk <= '0';
			tx_baudcount <= tx_baudcount + 1;
			if tx_baudcount = baudreg then
				tx_baudcount <= (others => '0');
				tx_baudclk <= '1';
			end if;
			if c_tx_flag = '1' then
				tx_flag <= '0';
			end if;
			if tx_baudclk = '1' then
				if tx_phase = "00" and tx_flag = '0' then
					tx_reg <= tx_buf;
					tx_phase <= "01";
					tx_flag <= '1';
				elsif tx_phase = "01" then
					txd <= '0';
					tx_phase <= "10";
					tx_count <= "0000";
				elsif	tx_phase = "10" and tx_count /= "1000" then
					txd <= tx_reg(0);
					tx_reg(0) <= tx_reg(1);
					tx_reg(1) <= tx_reg(2);
					tx_reg(2) <= tx_reg(3);
					tx_reg(3) <= tx_reg(4);
					tx_reg(4) <= tx_reg(5);
					tx_reg(5) <= tx_reg(6);
					tx_reg(6) <= tx_reg(7);
					
					tx_count <= tx_count + 1;
				elsif tx_phase = "10" then
					tx_phase <= "11";
					txd <= '1';
				else
					tx_phase <= "00";
				end if;
			end if;
		end if;
	end process;


--rx part
	process(clk, reset, rx_baudclk)
	begin
		if reset = '1' then
			rx_baudclk <= '0';
			rx_active <= '0';
			rx_phase <= '0';
		elsif clk'event and clk = '1' then
			s_rx_flag  <= '0';
			rx_baudclk <= '0';
			if rx_active = '0' and rxd = '0' then
				rx_active <= '1';
				rx_baudcount <= "0" & baudreg(11 downto 1);
			elsif rx_active = '1' then
				rx_baudcount <= rx_baudcount + 1;
				if rx_baudcount = baudreg then
					rx_baudcount <= (others => '0');
					rx_baudclk <= '1';
				end if;
			end if;
			
			if rx_baudclk = '1' and rx_active = '1' then
				if rx_phase = '0' then
					rx_phase <= '1';
					rx_count <= "0000";
				elsif	rx_phase = '1' and rx_count /= "1000" then
					rx_reg(0) <= rx_reg(1);
					rx_reg(1) <= rx_reg(2);
					rx_reg(2) <= rx_reg(3);
					rx_reg(3) <= rx_reg(4);
					rx_reg(4) <= rx_reg(5);
					rx_reg(5) <= rx_reg(6);
					rx_reg(6) <= rx_reg(7);
					rx_reg(7) <= rxd;
					rx_count <= rx_count + 1;
				else
					rx_active <= '0';
					rx_phase  <= '0';
					rx_buf    <= rx_reg;
					s_rx_flag <= '1';
				end if;
			end if;
		end if;
	end process;


	process(clk, reset)
	begin
		if reset = '1' then
			baudreg <= to_unsigned(uart_div, 12);--debug
			rx_flag <= '0';
		elsif clk'event and clk = '0' then
			c_tx_flag <= '0';
			c_rx_flag <= '0';
			if cs = '1' and we = '0' then
				c_rx_flag <= '1';
			elsif cs = '1' and we = '1' then
				tx_buf <= data_in; c_tx_flag <= '1';
			end if;
			
			if s_rx_flag = '1' then
				rx_flag <= '1';
			elsif c_rx_flag = '1' then
				rx_flag <= '0';
			end if;
		end if;
	
	end process;

data_out <= rx_buf;
rx_full   <= rx_flag;
tx_busy_n <= tx_flag;

end rtl;

