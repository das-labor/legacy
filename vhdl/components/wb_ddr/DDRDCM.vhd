-----------------------------------------------------------------------------
-- Wishbone DDR controller component
-- (c) 2007 Joerg Bornschein (jb@capsec.org)
--
-- This file is part of wb_ddr and should not be used directly
-----------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library UNISIM;              -- for Xilinx DCM and BUFG
use UNISIM.VComponents.all;  -- for Xilinx DCM and BUFG

-----------------------------------------------------------------------------
-- DDRDCM -------------------------------------------------------------------
entity DDRDCM is
	port (
		clk_fb   : in  std_logic;		
		clk_fb90 : out std_logic;
		-- Calibration & Control
		reset    : in  std_logic;		
		locked   : out std_logic;		
		clk      : in  std_logic;
		cal_inc  : in  std_logic;
		cal_dec  : in  std_logic;
		cal_done : out std_logic;
		cal_ovf  : out std_logic );
end DDRDCM;

-----------------------------------------------------------------------------
-- Implementation -----------------------------------------------------------
architecture rtl of DDRDCM is

-----------------------------------------------------------------------------
-- Component declaration ----------------------------------------------------
component DCM
-- pppragma translate_off
    generic ( 
              DLL_FREQUENCY_MODE    : string := "LOW";
              DUTY_CYCLE_CORRECTION : boolean := TRUE;
				  CLKOUT_PHASE_SHIFT    : string := "FIXED";
              PHASE_SHIFT           : integer
            );  
-- pppragma translate_on
    port ( CLKIN     : in  std_logic;
           CLKFB     : in  std_logic;
           PSINCDEC  : in  std_logic;
           PSEN      : in  std_logic;
           PSCLK     : in  std_logic;
           RST       : in  std_logic;
           CLK0      : out std_logic;
           CLK90     : out std_logic;
           CLK180    : out std_logic;
           CLK270    : out std_logic;
           CLK2X     : out std_logic;
           CLK2X180  : out std_logic;
           CLKDV     : out std_logic;
           CLKFX     : out std_logic;
           CLKFX180  : out std_logic;
           LOCKED    : out std_logic;
           PSDONE    : out std_logic;
           STATUS    : out std_logic_vector(7 downto 0)
          );
end component;

component BUFG
  port ( I : in std_logic;
         O : out std_logic);
end component;

-----------------------------------------------------------------------------
-- Local signals ------------------------------------------------------------
signal Vcc        : std_logic := '1';
signal GND        : std_logic := '0';

signal clk0u      : std_logic;  -- gen. clock without BUFG
signal clk0b      : std_logic;  -- geb. clock with BUFG
signal clk90u     : std_logic;  -- gen. clock without BUFG
signal clk90b     : std_logic;  -- geb. clock with BUFG

signal locked_int : std_logic;

signal psen       : std_logic;
signal psincdec   : std_logic;
signal psdone     : std_logic;
signal status     : std_logic_vector(7 downto 0);
signal ps_counter : unsigned(7 downto 0);

begin

dcm0: DCM
	generic map (
		CLKOUT_PHASE_SHIFT => "VARIABLE",
      PHASE_SHIFT => -50 )
	port map (
	   RST      => reset,
		CLKIN    => clk_fb,
		CLKFB    => clk0b,
		PSINCDEC => psincdec,
		PSEN     => psen,
		PSCLK    => clk,
		CLK0     => clk0u,
		CLK90    => open,
		CLK180   => open,
		CLK270   => clk90u,
		CLK2X    => open,
		CLK2X180 => open,
		CLKDV    => open,
		CLKFX    => open,
		CLKFX180 => open,
		LOCKED   => locked_int,
		PSDONE   => psdone,
		STATUS   => status ) ;
		
locked <= locked_int;

bufg0: BUFG 
	port map (
		I => clk0u,
		O => clk0b );

bufg90: BUFG 
	port map (
		I => clk90u,
		O => clk90b );

clk_fb90 <= clk90b;
		
-----------------------------------------------------------------------------
-- Dynamic Phase Shifting
cal_done <= psdone;

--cal_ovf  <= status(0);

cal_ovf <= '1' when ps_counter="01111110" else
           '1' when ps_counter="10000010" else
		  	  '0';

psproc: process(clk) is
type TState is (idle, waitdone);
variable state : TState;

begin
	if reset='1' then
	   psen     <= '0';
		psincdec <= '0';
		state := idle;
		ps_counter <= (others => '0');
	elsif clk'event and clk='1' then 
		case state is
		when idle =>
			psen <= '0';
			if cal_inc='1' and ps_counter/="01111110" then
				psen <= '1';
				psincdec <= '1';
				ps_counter <= ps_counter + 1;
				state := waitdone;
			elsif cal_dec='1' and ps_counter/="10000010" then
				psen <= '1';
				psincdec <= '0';
				ps_counter <= ps_counter - 1;				
				state := waitdone;
			end if;
		when waitdone =>
			psen <= '0';
			if psdone='1' then
				state := idle;
			end if;
		end case;
	end if;
end process;
		
end rtl;
