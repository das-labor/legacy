-----------------------------------------------------------------------------
-- DDR Controller with 64 Bit Wishbone interface and 16 bit DDR data width
--   by Joerg Bornschein  (jb@capsec.org)
--
-- All files under GPLv2  --  please contact me if you use this component
-----------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

-----------------------------------------------------------------------------
-- 64 Bit Wishbone DDR Controller -------------------------------------------
entity wb_ddr is
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
end wb_ddr;

-----------------------------------------------------------------------------
-- Implementation -----------------------------------------------------------
architecture rtl of wb_ddr is

constant CASLatency : std_logic_vector(2 downto 0) := "010";

constant wRP  : unsigned(3 downto 0) := "0011";
constant wMRD : unsigned(3 downto 0) := "0001";
constant wRCD : unsigned(3 downto 0) := "0010";
constant wRFC : unsigned(3 downto 0) := "1111";
constant wDAL : unsigned(3 downto 0) := "0100";

constant cmd_nop    : std_logic_vector(2 downto 0) := "111";
constant cmd_act    : std_logic_vector(2 downto 0) := "011";
constant cmd_read   : std_logic_vector(2 downto 0) := "101";
constant cmd_write  : std_logic_vector(2 downto 0) := "100";
constant cmd_mrs    : std_logic_vector(2 downto 0) := "000";
constant cmd_ref    : std_logic_vector(2 downto 0) := "001";
constant cmd_pre    : std_logic_vector(2 downto 0) := "010";

-----------------------------------------------------------------------------
-- Component declarations ---------------------------------------------------
component CmdPath is
	port (
	   clk        : in  std_logic;
		clk_2x     : in  std_logic;
		reset      : in  std_logic;
		-- 
		cmd        : in  std_logic_vector(2 downto 0);
		rdone      : out std_logic;
		wdone      : out std_logic;
		-- to high speed DataPath
		path_rtrig : out std_logic;
		path_wtrig : out std_logic;
		-- DDR Connection
		ddr_ras_n  : out std_logic;
		ddr_cas_n  : out std_logic;
		ddr_we_n   : out std_logic );
end component CmdPath;

component DataPath is
	port (
		clk_2x   : in    std_logic;
		clk_2x90 : in    std_logic;
		clk_fb90 : in    std_logic;
		reset    : in    std_logic;
		cal_xing : in    std_logic_vector(5 downto 0);
		--
		rtrig    : in    std_logic;
		wtrig    : in    std_logic;
		rdone    : out   std_logic;
		wdone    : out   std_logic;
		rdata    : out   std_logic_vector(63 downto 0);
		wdata    : in    std_logic_vector(63 downto 0);
		dmask    : in    std_logic_vector( 7 downto 0);
		-- DDR Connection
		ddr_dqs  : inout std_logic_vector( 1 downto 0);
		ddr_dq   : inout std_logic_vector(15 downto 0);
		ddr_dm   : out   std_logic_vector( 1 downto 0);
		-- Debug 
		led      : out   std_logic_vector( 7 downto 0);
		sw       : in    std_logic_vector(3 downto 0) );
end component DataPath;

component DDRCal is
	port (
		clk       : in  std_logic;
		reset     : in  std_logic;
		-- Calibration
		cal_xing  : out std_logic_vector(5 downto 0);
		cal_inc   : out std_logic;
		cal_dec   : out std_logic;
		cal_done  : in  std_logic;
		cal_ovf   : in  std_logic;
		-- Status & Buttons
		match_led : in  std_logic_vector(7 downto 0);		
		rotary    : in  std_logic_vector(2 downto 0);
		led       : out std_logic_vector(7 downto 0) );
end component DDRCal;

component DDRDCM is
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
end component DDRDCM;

-----------------------------------------------------------------------------
-- Local signals and types --------------------------------------------------
subtype TWaits is unsigned(3 downto 0);

signal clk_fb90   : std_logic;
signal dcm_locked : std_logic;

signal ref        : std_logic;

signal ddr_cmd    : std_logic_vector(2 downto 0);

signal rdone      : std_logic;
signal wdone      : std_logic;
signal dq_rdata   : std_logic_vector(63 downto 0);
signal dq_wdata   : std_logic_vector(63 downto 0);
signal dmask      : std_logic_vector( 7 downto 0);

signal cal_inc    : std_logic;
signal cal_dec    : std_logic;
signal cal_done   : std_logic;
signal cal_ovf    : std_logic;
signal cal_xing   : std_logic_vector(5 downto 0);

signal match_led  : std_logic_vector(7 downto 0);  -- XXX debug LEDs XXX
signal led        : std_logic_vector(7 downto 0);  -- XXX debug LEDs XXX
--signal sw       : std_logic_vector(3 downto 0);  -- XXX debug switches XXX

-- CmdPath <-> DataPath
signal path_rtrig : std_logic;
signal path_wtrig : std_logic;

begin

------------------------------------------------------------------------------
-- Clock handling ------------------------------------------------------------
ddr_clk    <= not clk_2x;
ddr_clk_n  <=     clk_2x;

-- generate phase shifted clock for data sampling (reading)
dcm0: DDRDCM
	port map(
		clk_fb   => ddr_clk_fb,
		clk_fb90 => clk_fb90,
		-- Calibration
		clk      => clk,		
		reset    => reset,
		locked   => dcm_locked,		
		cal_inc  => cal_inc,
		cal_dec  => cal_dec,
		cal_done => cal_done,
		cal_ovf  => cal_ovf );
	
-- calibrate phase shifted clock
cal0: DDRCal
	port map(
		reset    => reset,
		clk      => clk,
		-- Calibration
		cal_xing => cal_xing,
		cal_inc  => cal_inc,
		cal_dec  => cal_dec,
		cal_done => cal_done,
		cal_ovf  => cal_ovf,
		-- Status & Buttons
		match_led=> match_led,
		rotary   => rotary,
		led      => led );

-- DQ/DQS high speed path
dataPath0: DataPath
	port map (
		clk_2x   => clk_2x,
		clk_2x90 => clk_2x90,
		clk_fb90 => clk_fb90,
		reset    => reset,
		cal_xing => cal_xing,
		-- Data
		rdone    => rdone,
		wdone    => wdone,
		wdata    => dq_wdata,
		rdata    => dq_rdata,
		dmask    => dmask,
		-- To high speed CmdPath 
		rtrig    => path_rtrig,
		wtrig    => path_wtrig,
		-- DDR connection
		ddr_dqs  => ddr_dqs,
		ddr_dq   => ddr_dq,
		ddr_dm   => ddr_dm,
		-- Debug & Status
		sw       => sw,
		led      => match_led );
		
-- High speed CMD path
cmdPath0: CmdPath
	port map(
		clk        => clk,
		clk_2x     => clk_2x,
		reset      => reset,
		-- 
		cmd        => ddr_cmd,
		-- To high speed DataPath
		path_rtrig => path_rtrig,
		path_wtrig => path_wtrig,
		-- DDR Connection
		ddr_ras_n  => ddr_ras_n,
		ddr_cas_n  => ddr_cas_n,
		ddr_we_n   => ddr_we_n );

------------------------------------------------------------------------------
-- Main state machine --------------------------------------------------------
stateproc: process(clk) is 
type TState is (resetState, wait200, init1, init2, init3, init4, init5, init6,
                 init7, init8, idle, read1, read2, write1, write2);
variable state      : TState := wait200;
variable refcount   : unsigned(8 downto 0);
variable wait2count : unsigned(13 downto 0);
variable waits      : TWaits;
begin
	if clk'event and clk='1' then
		-- synchronous reset logic
		if dcm_locked='0' or reset='1' then 
			--ddr_dm    <= (others => '0');		
			ddr_cke   <= '0';  
			ddr_cs_n  <= '1';
			ddr_cmd   <= cmd_nop;
			refcount := "000000000";
			wait2count := (others => '0');
			waits    := "0000";
			state    := wait200;
			ref <= '0';		
		end if;

		-- count until 255+127=382 ^= 7.8us at 50 MHz
		refcount := refcount - 1;
		if refcount=0 then
			refcount := "110000000";
			ref <= '1';
		end if;

		wait2count := wait2count + 1;
		-- wait states?
		if waits /= 0 then
			waits := waits - 1;
			ddr_cmd      <= cmd_nop;
			wb_ack_o     <= '0';
		else
			-- state machine
			case state is
			when wait200 =>                  -- WAIT 200us 
				ddr_cke  <= '0';
				ddr_cs_n <= '0';
				if (wait2count(13)='1' and wait2count(11)='1') or wait200us='0' then
					ddr_cke      <= '1';
					ddr_cs_n     <= '0';
					state := init1;        
					waits := "1111";
				end if;
			when init1 =>
				ddr_cmd      <= cmd_pre;     -- PRECHARGE ALL 
				ddr_addr(10) <= '1'; 
				state := init2;				
				waits := wRP;
			when init2 =>
				ddr_cmd      <= cmd_mrs;     -- EXTENDED MODE REGISTER
				ddr_ba       <= "01";
				ddr_addr     <= (others => '0');
				state := init3;
				waits := wMRD;
			when init3 =>
				ddr_cmd      <= cmd_mrs;     -- MODE REGISTER
				ddr_ba       <= "00";
				ddr_addr     <= "0000010" & CASLatency & "0" & "010";
				state := init4;
				waits := wMRD;
			when init4 => 
				ddr_cmd      <= cmd_pre;     -- PRECHARGE ALL 
				ddr_addr(10) <= '1'; 
				state := init5;
				waits := wRP;
			when init5 => 
				ddr_cmd      <= cmd_ref;     -- AUTO REFRESH
				state := init6;
				waits := wRFC;
			when init6 =>
				ddr_cmd      <= cmd_ref;     -- AUTO REFRESH			
				state := init7;
				waits := wRFC;
			when init7 =>	
				ddr_cmd      <= cmd_mrs;     -- MODE REGISTER
				ddr_ba       <= "00";
				ddr_addr     <= "0000000" & CASLatency & "0" & "010";
				refcount := "001100000"; -- wait 200 cycl, misuse refresh counter
				ref <= '0';				
				waits := wMRD;
				state := init8;
			when init8 =>	
				if ref='1' then 
					state := idle;
				end if;
			when idle =>
				ddr_cmd      <= cmd_nop;     -- NOP
				wb_ack_o     <=  '0';
				if ref='1' then              -- need AUTO REFRESH
					ddr_cmd      <= cmd_ref;  
					ref          <= '0';
					state := idle;
					waits := wRFC;
				elsif wb_stb_i='1' and wb_cyc_i='1' and wb_we_i='1' then
				   -- Start write cycle
					ddr_addr(12 downto 0) <= wb_adr_i(23 downto 11);					
					ddr_ba(1)    <= wb_adr_i(25);  
					ddr_ba(0)    <= wb_adr_i(24);					
					ddr_cmd      <= cmd_act;  -- ACTIVE
					state := write1;
					waits := wRCD;
				elsif wb_stb_i='1' and wb_cyc_i='1' and wb_we_i='0' then
				   -- Start read cycle
					ddr_addr(12 downto 0) <= wb_adr_i(23 downto 11);					
					ddr_ba(1)           <= wb_adr_i(25);  
					ddr_ba(0)           <= wb_adr_i(24);
					ddr_cmd             <= cmd_act;  -- ACTIVE					
					state               := read1;
					waits               := wRCD;
				end if;
			when read1 =>
				ddr_cmd                <= cmd_read;   -- READ
				ddr_addr(12 downto 10) <= "001";
				ddr_addr( 9 downto  0) <= wb_adr_i(10 downto 1);
				state := read2;
			when read2 =>
				ddr_cmd      <= cmd_nop; -- NOP
				if (rdone='1') then
					wb_dat_o <= dq_rdata;
					wb_ack_o <= '1';
					state := idle;
					waits := wRP;
				end if;
			when write1 =>
				ddr_cmd                <= cmd_write; -- WRITE
				ddr_addr(12 downto 10) <= "001";
				ddr_addr( 9 downto  0) <= wb_adr_i(10 downto 1);
				dq_wdata               <= wb_dat_i;
				state := write2;
			when write2 =>
				ddr_cmd      <= cmd_nop; -- NOP
				if (wdone='1') then
					wb_ack_o <= '1';
					state := idle;
					waits := wDAL;
				end if;
			when others =>
			end case;
		end if;
	end if;
end process;

dmask <= wb_sel_i;

--Debug output --------------------------------------------------------------
debug <= ( 
	0      => ref,       -- need AUTO_REFRESH
	others => '0' );

end rtl;

