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

entity Main is
    Port ( LED : out  STD_LOGIC_VECTOR (7 downto 0);
           SEGMENT : out  STD_LOGIC_VECTOR (7 downto 0);
           DIGIT : out  STD_LOGIC_VECTOR (3 downto 0);
           UART_TXD : out  STD_LOGIC;
           UART_RXD : in  STD_LOGIC;
           BTN : in  STD_LOGIC_VECTOR (3 downto 0);
           SW : in  STD_LOGIC_VECTOR (7 downto 0);
           CLK : in  STD_LOGIC);
end Main;

architecture Behavioral of Main is

component SegmentMUX 
    Port ( digit0 : in  STD_LOGIC_VECTOR (3 downto 0);
           digit1 : in  STD_LOGIC_VECTOR (3 downto 0);
           digit2 : in  STD_LOGIC_VECTOR (3 downto 0);
           digit3 : in  STD_LOGIC_VECTOR (3 downto 0);
           segments : out  STD_LOGIC_VECTOR (7 downto 0);
           mux : out  STD_LOGIC_VECTOR (3 downto 0);
           clk : in  STD_LOGIC);
end component;

signal RESET : STD_LOGIC;

begin
segmux:  SegmentMux 
   Port map ( digit0   => SW(7 downto 4),
              digit1   => SW(3 downto 0),
              digit2   => SW(7 downto 4),
              digit3   => BTN(3 downto 0),
              clk      => clk,
              segments => segment,
              mux      => digit
   );

	UART_TXD <= UART_RXD;
   LED <= (others => '0');
end Behavioral;

