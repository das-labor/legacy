----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    19:09:22 12/17/2006 
-- Design Name: 
-- Module Name:    main - Behavioral 
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

entity main is
  port (
      clk_in       : in  std_logic;
      button       : in  std_logic_vector(3 downto 0);
		switch       : in  std_logic_vector(7 downto 0);
      led          : out std_logic_vector(7 downto 0);
      --Segments
      segments     : out std_logic_vector(7 downto 0);
      mux          : out std_logic_vector(3 downto 0);
		-- SRAM
      ram_ce_n : out   std_logic_vector(1 downto 0);
      ram_be_n : out   std_logic_vector(3 downto 0);
      ram_we_n : out   std_logic;
      ram_oe_n : out   std_logic;
      ram_addr : out   std_logic_vector(17 downto 0);
      ram_io   : inout std_logic_vector(31 downto 0);		
		
		--UART
		rxd : in std_logic;
		txd : out std_logic;
		--SPI
--		spi_cs : out std_logic;
--		spi_mosi : out std_logic;
--		spi_miso : in std_logic;
--		spi_sck : out std_logic;

		lsr_clr : out  STD_LOGIC;
      lsr_d : out  STD_LOGIC;
      lsr_c : out  STD_LOGIC;
      latch_data : out  STD_LOGIC_VECTOR (7 downto 0);
		plane_out : out std_logic_vector (4 downto 0);
		
		psr_c : out std_logic;
		psr_d : out std_logic;
		col_enable : out std_logic
	
		);
end main;

architecture Behavioral of main is

signal clk16_gen : std_logic;
signal clk16 : std_logic;
signal clk32 : std_logic;

signal clk32_np : std_logic;

signal locked : std_logic;
signal reset_n : std_logic;

signal hex_out : std_logic_vector(15 downto 0);

signal led1 : std_logic_vector(7 downto 0);



begin

	clk16 <= clk16_gen;
	
	reset_n <= locked;
	
	led <= led1;
	
	
  clkgen: entity work.ClkGen
	  port map (
			clk50    => clk_in,
			clk16_out=> clk16_gen,
			clk32_out=> clk32,
			clk32_np_out => clk32_np,
			switch   => switch (2 downto 0),
			pause    => button(0),
			reset		=> button(1),
			locked 	=> locked
			);
      
	segmux: entity work.SegmentMUX 
		Port map ( digit0 => hex_out(15 downto 12),
					digit1 => hex_out(11 downto 8),
					digit2 => hex_out(7 downto 4),
					digit3 => hex_out(3 downto 0),
					segments => segments,
					mux => mux,
					clk => clk_in );


	avr: entity work.top_avr_core_sim
		port map(
					ireset => reset_n,
					cp2 => clk16,
					clk32 => clk32,
					clk_pwm => clk32_np,
					porta => led1,
					--portb  : inout std_logic_vector(7 downto 0);
	            -- UART 
					rxd => rxd,
					txd => txd,
					-- External interrupt inputs
					nINT0 => '1',
					nINT1 => '1',
					nINT2 => '1',
					nINT3 => '1',
					INT4 => '0',
					INT5 => '0',
					INT6 => '0',
					INT7 => '0',
					addr_out => hex_out,
					
					 -- RAM
      ram_ce_n   => ram_ce_n,
      ram_be_n   => ram_be_n,
      ram_we_n   => ram_we_n,
      ram_oe_n   => ram_oe_n,
      ram_addr   => ram_addr,
      ram_io     => ram_io,
		
		lsr_clr => lsr_clr,
			lsr_d => lsr_d,
			lsr_c => lsr_c,
			latch_data => latch_data,
			
			psr_c => psr_c,
				psr_d => psr_d,
				col_enable => col_enable
								);


end Behavioral;

