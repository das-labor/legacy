-----------------------------------------------------------------------------
-- Wishbone DDR controller component
-- (c) 2007 Joerg Bornschein (jb@capsec.org)
--
-- This file is part of wb_ddr and should not be used directly
-----------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;

-----------------------------------------------------------------------------
-- CmdPath ------------------------------------------------------------------
entity CmdPath is
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
end CmdPath;

-----------------------------------------------------------------------------
-- Implementation -----------------------------------------------------------
architecture rtl of CmdPath is

signal ddr_cmd   : std_logic_vector(2 downto 0);
signal cmd_phase : std_logic;

begin

cmd_phase <= clk;

-- gate cmd to DDR, iff in cmd_phase, else NOP
ddr_cmd <= cmd when cmd_phase='1' else
           "111";    -- NOP

ddr_ras_n <= ddr_cmd(2);
ddr_cas_n <= ddr_cmd(1);			
ddr_we_n  <= ddr_cmd(0);

-- generate trigger signals for DataPath
path_rtrig <= '1' when cmd_phase='1' and cmd="101" else
              '0';
				  
path_wtrig <= '1' when cmd_phase='1' and cmd="100" else
              '0';

end rtl;

