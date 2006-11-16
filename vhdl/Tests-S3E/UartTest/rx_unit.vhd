brary ieee;
use ieee.std_logic_1164.all;

entity receiver is
	port (
		reset      : in  std_logic;                      -- asynch reset
		clk        : in  std_logic;                      -- clock 40 MHz
		rx         : in  std_logic;                      -- receive input
		data_ready : out std_logic;                      -- status signal data received
		data_error : out std_logic;                      -- status signal receive error
		data_out   : out std_logic_vector(7 downto 0));  -- received data
end receiver;

architecture behaviour of receiver is

signal received      : boolean;                       -- for received mode
signal receiving     : boolean;                       -- for receive mode
signal count_start   : natural range 0 to 2083;       -- start counter for detecting middle of startbit
signal count_clk     : natural range 0 to 4167;       -- clock counter for clock divider
signal count_bit     : natural range 0 to 9;          -- bit counter for received bits
signal enable        : std_logic;                     -- clock enable for receive mode
signal data_register : std_logic_vector(7 downto 0);  -- data register for received bits


begin

--------------------------------------------------
-- detecting startbit                           --
-- sensitive to clock, asynch reset by received --
-- if rx = 0 counts to 2083 to know if there is --
-- a startbit                                   --
-- when startbit:   go in receiving mode        --
-- when no starbit: count again from 0 to 2083  --
--------------------------------------------------
sampling : process (clk, received) is
begin
if received then
  receiving <= false;
  count_start <= 0;
elsif clk'event and clk = '1' then
  if rx = '0' then
    if count_start = 2083 then
      receiving <= true;
    else
      count_start <= count_start + 1;
    end if;
  end if;
end if;
end process sampling;


--------------------------------------------------
-- clock divider for baudrate generation        --
-- sensitive to clock, asynch reset by received --
-- receiving mode: generate clock enable        --
-- for wished baudrate                          --
--------------------------------------------------
clock_divider: process (clk, received) is
begin
if received then
count_clk <= 0;
elsif clk'event and clk = '1' then
    if receiving then
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


--------------------------------------------------------
-- receive data                                       --
-- sensitive to clock. asynch reset by reset          --
-- clock enable by enable                             --
-- load 9 bit in data register & put data on data out --
-- last bit = 1 : valid stopbit => data ready         --
-- last bit = 0 : invalid stopbit => data error       --
--------------------------------------------------------
data_read: process (clk, reset) is
begin
if reset = '1' then
  data_register <= (others => '0');
elsif clk'event and clk = '1' then
received   <= false;  
  if enable = '1' then
     data_ready <= '0';
     data_error <= '0';
     case count_bit is
       when 0 => count_bit <= count_bit + 1;
       when 9 => count_bit <= 0;
                 received  <= true;
     data_out  <= data_register;
                 if rx = '1' then
                   data_ready <= '1';
                 else
                   data_error <= '1';
                 end if;
       when others => count_bit <= count_bit + 1;
                      data_register(count_bit-1) <= rx;
     end case;
  end if;
end if;
end process data_read;
                
end behaviour;
