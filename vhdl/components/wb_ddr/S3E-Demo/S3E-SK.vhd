-----------------------------------------------------------------------------
-- DDR memory test for the Xilinx Spartan3E 500 Starter Kit
-- by Joerg Bornschein (jb@capsec.org)
-- 
-- all files under GPLv2 
--
-----------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;

entity S3E_SK is
	port (
		clk_in       : in    std_logic;
		reset_in     : in    std_logic;
		-- DDR Connection
		ddr_clk      : out   std_logic;
		ddr_clk_n    : out   std_logic;
		ddr_clk_fb   : in    std_logic;
		ddr_dq       : inout std_logic_vector(15 downto 0);
		ddr_dqs      : inout std_logic_vector( 1 downto 0);
		ddr_dm       : out   std_logic_vector( 1 downto 0);
		ddr_addr     : out   std_logic_vector(12 downto 0);
		ddr_ba       : out   std_logic_vector( 1 downto 0);
		ddr_cke      : out   std_logic;
		ddr_cs_n     : out   std_logic;
		ddr_ras_n    : out   std_logic;
		ddr_cas_n    : out   std_logic;
		ddr_we_n     : out   std_logic;
		-- status output
		sw           : in    std_logic_vector(3 downto 0);
		led			 : out   std_logic_vector(7 downto 0);
		rotary  		 : in    std_logic_vector(2 downto 0);
      btn0         : in    std_logic;
      btn1         : in    std_logic;
      btn2         : in    std_logic );
end S3E_SK;

-----------------------------------------------------------------------------
-- Implementation -----------------------------------------------------------
architecture rtl of S3E_SK is

-----------------------------------------------------------------------------
-- Component declarations ---------------------------------------------------
component wb_ddr is
	generic (
		wait200us    : in    std_logic := '1' );
	port(
	    clk          : in    std_logic;
		clk_2x       : in    std_logic;
		clk_2x90     : in    std_logic;
		reset        : in    std_logic;
		-- DDR Connection
		ddr_clk      : out   std_logic;
		ddr_clk_n    : out   std_logic;
		ddr_clk_fb   : in    std_logic;
		ddr_dq       : inout std_logic_vector(15 downto 0);
		ddr_dqs      : inout std_logic_vector( 1 downto 0);
		ddr_dm       : out   std_logic_vector( 1 downto 0);
		ddr_addr     : out   std_logic_vector(13 downto 0);
		ddr_ba       : out   std_logic_vector( 1 downto 0);
		ddr_cke      : out   std_logic;
		ddr_cs_n     : out   std_logic;
		ddr_ras_n    : out   std_logic;
		ddr_cas_n    : out   std_logic;
		ddr_we_n     : out   std_logic;
		-- Whishbone Interface
		wb_adr_i     : in    std_logic_vector(31 downto 0);
		wb_dat_i     : in    std_logic_vector(63 downto 0);
		wb_dat_o     : out   std_logic_vector(63 downto 0);
		wb_sel_i     : in    std_logic_vector( 7 downto 0);
		wb_we_i      : in    std_logic;
		wb_stb_i     : in    std_logic;
		wb_cyc_i     : in    std_logic;
		wb_ack_o     : out   std_logic;
		-- Debug & Rotary 
		sw           : in    std_logic_vector(3 downto 0);
		rotary       : in    std_logic_vector(2 downto 0);
		debug        : out   std_logic_vector(7 downto 0) );
end component wb_ddr;

-----------------------------------------------------------------------------
-- Local signals and types --------------------------------------------------

signal clk, clk_2x, clk_2x90 : std_logic;
signal reset, locked         : std_logic;

-- Whishbone signals
signal wb_adr    : std_logic_vector(31 downto 0);
signal wb_dat_r  : std_logic_vector(63 downto 0);
signal wb_dat_w  : std_logic_vector(63 downto 0);
signal wb_we     : std_logic;
signal wb_stb    : std_logic;
signal wb_ack    : std_logic;
signal wb_cyc    : std_logic;

signal ctrl_led   : std_logic_vector(7 downto 0);
signal test_led   : std_logic_vector(7 downto 0);
  
begin

clkgen0: entity work.clkgen
	port map(
		clk_in     => clk_in,
		reset      => reset_in,
		locked     => locked,
		clk        => clk,
		clk_2x     => clk_2x,      
		clk_2x90   => clk_2x90 );

reset <= reset_in or not locked;

ctrl0: wb_ddr
	port map(
	   clk        => clk,
		clk_2x     => clk_2x,
		clk_2x90   => clk_2x90,      
		reset      => reset,
		-- DDR connection
		ddr_clk    => ddr_clk,
		ddr_clk_n  => ddr_clk_n,
		ddr_clk_fb => ddr_clk_fb,
		ddr_dq     => ddr_dq,
		ddr_dqs    => ddr_dqs,
		ddr_dm     => ddr_dm,
		ddr_ba     => ddr_ba,
		ddr_cke    => ddr_cke,
		ddr_cs_n   => ddr_cs_n,
		ddr_ras_n  => ddr_ras_n,
		ddr_cas_n  => ddr_cas_n,
		ddr_we_n   => ddr_we_n,
		ddr_addr(12 downto 0)   => ddr_addr,
		ddr_addr(13)=> open,
		-- Whishbone connection
		wb_adr_i   => wb_adr,
		wb_dat_i   => wb_dat_w,
		wb_dat_o   => wb_dat_r,
		wb_we_i    => wb_we,
		wb_stb_i   => wb_stb,
		wb_cyc_i   => wb_cyc,
		wb_ack_o   => wb_ack,
		wb_sel_i   => (others => '1'),
		-- Debug and rotary
      debug      => ctrl_led,
		sw         => sw,
		rotary     => rotary );

test0: entity work.wb_memtest
	port map (
	   -- Whishbone connection
		wb_clk_i   => clk,
		wb_rst_i   => reset,
		wb_adr_o   => wb_adr,
		wb_dat_o   => wb_dat_w,
		wb_dat_i   => wb_dat_r,
		wb_we_o    => wb_we,
		wb_stb_o   => wb_stb,
		wb_cyc_o   => wb_cyc,
		wb_ack_i   => wb_ack,
		-- status output 
		led        => test_led,
      btn0       => btn0,
      btn1       => btn1 ); 	

led <= ctrl_led when btn2='0' else
       test_led;

end rtl;
