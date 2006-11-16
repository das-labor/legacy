library ieee;
use ieee.std_logic_1164.all;

entity transmitter is
	port (
		reset      : in  std_logic;                     -- asynch reset
		clk        : in  std_logic;                     -- clock 40 MHz
		data       : in  std_logic_vector(7 downto 0);  -- data input
		data_ready : in  std_logic;                     -- control signal data ready
		tx_ready   : out std_logic;                     -- transmitter for new data
		tx         : out std_logic);                    -- transmitter output
end transmitter;

architecture behavior of transmitter is

-- internal signals
signal sending       : boolean;                       -- sending mode
signal sent          : boolean;                       -- sent mode
signal count_clk     : natural range 0 to 4167;       -- clock counter for clock divider
signal count_bit     : natural range 0 to 10;         -- bit counter
signal enable        : std_logic;                     -- clock enable for sending mode
signal tx_ready_int  : std_logic := '1';              -- initalized to 1 for tx_ready = 1 on start
signal data_register : std_logic_vector(7 downto 0);  -- for storage of data input to send

begin

  
-------------------------------------
-- load tx_ready with rx_ready_int --
-------------------------------------
tx_ready <= tx_ready_int;


--------------------------------------------------
-- clock divider for baudrate generation        --
-- sensitive to clock, asynch reset by received --
-- receiving mode: generate clock enable        --
-- for wished baudrate                          --
--------------------------------------------------
clock_divider : process (clk, sent) is
begin
if sent then
  count_clk <= 0;
elsif clk'event and clk = '1' then
    if sending then
      enable <= '0';
      if count_clk = 4167 then
        count_clk <= 0;
      elsif count_clk = 0 then
        enable <= '1';
        count_clk <= count_clk + 1;
      else
        count_clk <= count_clk + 1;
      end if;
    end if;
  end if;
end process clock_divider;


-----------------------------------------------------------
-- data storage of input data                            --
-- sensitive to clock, asynch reset by reset             --
-- storage data in internal register when data_ready = 1 --
-----------------------------------------------------------
data_storage : process (clk, reset, sent) is
begin
if reset = '1' or sent then
  data_register <= (others => '0');
  sending       <= false;
elsif clk'event and clk = '1' then
  if data_ready = '1' then
    if not sending then
      data_register <= data;
      sending       <= true;
    end if;
  end if;
end if;
end process data_storage;


-------------------------------------------------------
-- send storaged data                                -- 
-- sensitive to clock, asynch reset by reset         --
-- load tx with content of data register from 0 to 9 --
-- when sending complete : ready for new data        --
-------------------------------------------------------
data_send : process (clk, reset) is
begin
if reset = '1' then
  tx        <= '1';
  count_bit <= 0;
elsif clk'event and clk = '1' then
sent <= false;
  if enable = '1' then
    tx_ready_int <= '0';
    tx           <= '1';
    case count_bit is
      when 0  => tx        <= '0';
                 count_bit <= count_bit + 1;
      when 9  => tx        <= '1';
                 count_bit <= count_bit + 1;
      when 10 => tx            <= '1';
                 count_bit     <= 0;
                 sent          <= true;
                 tx_ready_int  <= '1';
      when others => count_bit <= count_bit + 1;
                     tx        <= data_register(count_bit - 1);
    end case;
  end if;
end if;
end process data_send;
    
end architecture behavior;
