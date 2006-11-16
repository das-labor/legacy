----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    18:40:56 06/15/2006 
-- Design Name: 
-- Module Name:    Main - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

---- Uncomment the following library declaration if instantiating
---- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;



entity Main is
    Port ( LED : out  STD_LOGIC_VECTOR (7 downto 0);
           SEGMENT  : out  STD_LOGIC_VECTOR (7 downto 0);
           DIGIT    : out  STD_LOGIC_VECTOR (3 downto 0);
           UART_TXD : out  STD_LOGIC;
           UART_RXD : in  STD_LOGIC;
           BTN : in  STD_LOGIC_VECTOR (3 downto 0);
           SW : in  STD_LOGIC_VECTOR (7 downto 0);
           CLK : in  STD_LOGIC);
end Main;

architecture Behavioral of Main is

------------------------------------------------------------------------------------
-- declaration of KCPSM3
--
component kcpsm3
  Port (      address : out std_logic_vector(9 downto 0);
          instruction : in std_logic_vector(17 downto 0);
              port_id : out std_logic_vector(7 downto 0);
         write_strobe : out std_logic;
             out_port : out std_logic_vector(7 downto 0);
          read_strobe : out std_logic;
              in_port : in std_logic_vector(7 downto 0);
            interrupt : in std_logic;
        interrupt_ack : out std_logic;
                reset : in std_logic;
                  clk : in std_logic);
end component;
--
-- declaration of program ROM
--
component mpu_rom 
  Port (      address : in std_logic_vector(9 downto 0);
          instruction : out std_logic_vector(17 downto 0);
                  clk : in std_logic);
end component;
--
-- UART declaration
--
component uart_rx 
  Port (            serial_in : in std_logic;
                     data_out : out std_logic_vector(7 downto 0);
                  read_buffer : in std_logic;
                 reset_buffer : in std_logic;
                 en_16_x_baud : in std_logic;
          buffer_data_present : out std_logic;
                  buffer_full : out std_logic;
             buffer_half_full : out std_logic;
                          clk : in std_logic);
end component;
    
component uart_tx
  Port (            data_in : in std_logic_vector(7 downto 0);
               write_buffer : in std_logic;
               reset_buffer : in std_logic;
               en_16_x_baud : in std_logic;
                 serial_out : out std_logic;
                buffer_full : out std_logic;
           buffer_half_full : out std_logic;
                        clk : in std_logic);
end component;    

--
-- 7-Segment Multiplexer    
--
component SegmentMUX
  Port ( digit0 : in  STD_LOGIC_VECTOR (3 downto 0);
         digit1 : in  STD_LOGIC_VECTOR (3 downto 0);
         digit2 : in  STD_LOGIC_VECTOR (3 downto 0);
         digit3 : in  STD_LOGIC_VECTOR (3 downto 0);
         segments : out  STD_LOGIC_VECTOR (7 downto 0);
         mux : out  STD_LOGIC_VECTOR (3 downto 0);
         clk : in  STD_LOGIC);
end component;


------------------------------------------------------------------------------------
--
-- Signals used to connect KCPSM3 to program ROM
--
signal     address : std_logic_vector(9 downto 0);
signal instruction : std_logic_vector(17 downto 0);
--
signal RESET          : STD_LOGIC;
signal port_id        : STD_LOGIC_VECTOR(7 downto 0);
signal write_strobe   : STD_LOGIC;
signal read_strobe    : STD_LOGIC;
signal in_port        : STD_LOGIC_VECTOR(7 downto 0);
signal out_port       : STD_LOGIC_VECTOR(7 downto 0);
signal interrupt      : STD_LOGIC;
signal interrupt_ack  : STD_LOGIC;
signal cpuclk         : STD_LOGIC;
signal cpucount       : STD_LOGIC_VECTOR(20 downto 0);

-- UART
signal baudclk        : STD_LOGIC;
signal baudcount      : INTEGER RANGE 0 TO 55 := 0;
signal uart0_rxdata   : STD_LOGIC_VECTOR(7 downto 0);
signal uart0_txdata   : STD_LOGIC_VECTOR(7 downto 0);
signal uart0_status   : STD_LOGIC_VECTOR(7 downto 0);
signal uart0_rxready  : STD_LOGIC;
signal uart0_txready  : STD_LOGIC;

-- 7 Segment Display
signal digit0         : STD_LOGIC_VECTOR(3 downto 0);
signal digit1         : STD_LOGIC_VECTOR(3 downto 0);
signal digit2         : STD_LOGIC_VECTOR(3 downto 0);
signal digit3         : STD_LOGIC_VECTOR(3 downto 0);


------------------------------------------------------------------------------------
-- Start of circuit description
--

begin

processor: kcpsm3
    port map(      address => address,
               instruction => instruction,
                   port_id => port_id,
              write_strobe => write_strobe,
                  out_port => out_port,
               read_strobe => read_strobe,
                   in_port => in_port,
                 interrupt => interrupt,
             interrupt_ack => interrupt_ack,
                     reset => reset,
                       clk => cpuclk);

program: mpu_rom
    port map(      address => address,
               instruction => instruction,
                       clk => cpuclk);

uart0_rx: uart_rx
   port map (      serial_in => uart_rxd,
                    data_out => uart0_rxdata,
                 read_buffer => uart0_rxready,
                reset_buffer => RESET,
         buffer_data_present => uart0_status(0),
                 buffer_full => uart0_status(1),
            buffer_half_full => uart0_status(2),
                en_16_x_baud => baudclk,
                         clk => cpuclk
   );
   
uart0_tx: uart_tx
   Port map (        data_in  => uart0_txdata,
                 write_buffer => uart0_txready,
                 reset_buffer => RESET,
                 en_16_x_baud => baudclk,
                   serial_out => uart_txd,
                  buffer_full => uart0_status(6),
             buffer_half_full => uart0_status(7),
                          clk => cpuclk );

segment0: SegmentMux
   Port map (  digit0   => digit0,
               digit1   => digit1,
               digit2   => digit2,
               digit3   => digit3,
               segments => segment,
               mux      => digit,
               clk      => clk );
-----------------------------------------------------------------------------
-- Implementation
--
BAUDPROC: process(clk)
begin
   if clk='1' and clk'event then
      if baudcount = 55 then
         baudcount <= 0;
         baudclk <= '1';
      else
         baudcount <= baudcount + 1;
         baudclk <= '0';
      end if;
   end if;
end process;

INPORTPROC: process(clk)
begin
   if clk='1' and clk'event then
      case port_id(3 downto 0) is 
         -- Switches and buttons
         when "0010" => in_port(3 downto 0) <= BTN(3 downto 0); in_port(7 downto 4) <= "0000";
         when "0011" => in_port <= SW;
         -- UART
         when "0100" => in_port <= uart0_status;
         when "0101" => in_port <= uart0_rxdata; 
         -- Others         
         when others => null;
      end case;
   end if;
end process;

-- Make UART FIFO happy
uart0_rxready <= not port_id(3) and port_id(2) and 
                 not port_id(1) and port_id(0) and read_strobe;
  
OUTPORTPROC: process(clk)
begin
   if clk='1' and clk'event then
      case port_id(3 downto 0) is
         -- Status display
         when "0000" => digit1 <= out_port(7 downto 4); digit0 <= out_port(3 downto 0);
         when "0001" => digit3 <= out_port(7 downto 4); digit2 <= out_port(3 downto 0);         
--         when "0111" => led    <= out_port;
         -- UART
         when "0110" => uart0_txdata <= out_port; 
         -- Others
         when others => null;  
      end case;
   end if;
end process;

-- Make UART happy
uart0_txready <= not port_id(0) and port_id(1) and 
                 port_id(2) and not port_id(3) and write_strobe;         
         
RESET  <= BTN(3);
led  <= uart0_status;
cpuclk <= clk;
end Behavioral;

