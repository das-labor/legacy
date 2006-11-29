-----------------------------------------------------------------------------
-- Leon3 with Wishbone Interface --------------------------------------------
--
library ieee;
use ieee.std_logic_1164.all;
library grlib;
use grlib.amba.all;
use grlib.stdlib.all;
library techmap;
use techmap.gencomp.all;
library gaisler;
-- use gaisler.sim.all;      -- XXX
use gaisler.leon3.all;
use gaisler.misc.all;

use work.config.all;	-- configuration

entity wb_leon is
	port (
		clk   : in  std_logic;
		reset : in  std_logic;
		-- Interrupt requests
		irq        : in  std_logic_vector(15 downto 0);
		-- 32 bit Wishbone Master
		wb_adr_o   : out std_logic_vector(31 downto 0);
		wb_dat_o   : out std_logic_vector(31 downto 0);
		wb_dat_i   : in  std_logic_vector(31 downto 0);		
		wb_cache_i : in  std_logic;
		wb_we_o    : out std_logic;
		wb_sel_o   : out std_logic_vector( 3 downto 0);
		wb_cyc_o   : out std_logic;
		wb_stb_o   : out std_logic;		
		wb_ack_i   : in  std_logic );

end entity; 

-----------------------------------------------------------------------------
-- Implementation -----------------------------------------------------------
architecture rtl of wb_leon is

signal resetn   : std_logic;			-- Reset

signal GND      : std_ulogic := '0';
signal VCC      : std_ulogic := '1';
signal NC       : std_ulogic := 'Z';

signal ahbsi    : ahb_slv_in_type;
signal ahbso    : ahb_slv_out_vector := (others => ahbs_none);
signal ahbmi    : ahb_mst_in_type;
signal ahbmo    : ahb_mst_out_type;
--signal ahbmo    : ahb_mst_out_vector := (others => ahbm_none);

signal irqi     : l3_irq_in_type;
signal irqo     : l3_irq_out_type;

signal dbgi     : l3_debug_in_vector(0 to 0);
signal dbgo     : l3_debug_out_vector(0 to 0);

begin


----------------------------------------------------------------------
---  AHB CONTROLLER  -------------------------------------------------
--ahb0 : ahbctrl                              -- AHB arbiter/multiplexer
--	generic map (  defmast  => CFG_DEFMST, 
--                   split    => CFG_SPLIT, 
--	           rrobin   => CFG_RROBIN, 
--                   ioaddr   => CFG_AHBIO,
--	             ioen     => IOAEN, 
--                   nahbm    => 1,
--                   nahbs    => 1 )
--    port map (reset, clk, ahbmi, ahbmo, ahbsi, ahbso);

----------------------------------------------------------------------
---  LEON3 processor and DSU  ----------------------------------------
cpu0 : leon3s                                       -- LEON3 processor      
    generic map ( hindex    => 0,
                  fabtech   => CFG_FABTECH,
                  memtech   => CFG_MEMTECH,
                  nwindows  => 8,      -- SPARC register windows
                  dsu       => 0,      -- Debug Support Unit
                  fpu       => 0,      -- 
                  v8        => 0,      -- MUL & DIV instructions?
                  cp        => 0,      -- CoProcessor interface?
                  mac       => 0,      -- SMAC & UMAC instructions?
                  pclow     => 2,      -- LSB PC-Reg
                  nwp       => 0,      -- number of watchpoints
                  icen      => 0,      -- enable ICACHE
                  isets     => 1,      -- number ICACHE sets
                  isetsize  => 4,      -- ICACHE set size in kB
                  0linesize => 4,      -- ICACHE line size
                  dcen      => 0,      -- enable DCACHE 
                  dsets     => 1,      -- number ICACHE sets
                  dsetsize  => 2,      -- ICACHE set size in kB
                  dlinesize => 4,      -- ICAHCE line size
                  mmuen     => 0,      -- enable MMU
                  lddel     => 2,      -- load delay
                  disas     => 1,      -- instruction dissembly 
                  pwd       => 1,      -- power down
                  svt       => 0,      -- single vector trapping
                  smp       => 0 )     -- SMP support
    port map (    clk       => clk, 
                  rstn      => resetn, 
                  ahbi      => ahbmi, 
                  ahbo      => ahbmo,
                  ahbsi     => ahbsi,
                  ahbso     => ahbso, 
                  irqi      => irqi,
                  irqo      => irqo,
                  dbgi      => dbgi(0),
                  dbgo      => dbgo(0) );

----------------------------------------------------------------------
---  Bus bridge  -----------------------------------------------------

ahbmi.hgrant <= (others => '1');
ahbmi.hirq   <= (others => '0');

ahb2wb0:  entity work.ahb2wb
	port map (
		clk           => clk,
		reset         => reset,
		-- 32 bit AHB
		haddr         => ahbmo.haddr,
		htrans        => ahbmo.htrans,
		hwrite        => ahbmo.hwrite,
		hsize         => ahbmo.hsize,
		hburst        => ahbmo.hburst,
		hwdata        => ahbmo.hwdata,
		hsel          => '1',
		hreadyi       => ahbmi.hready,
		hreadyo       => ahbmi.hready,
		hrdata        => ahbmi.hrdata,
		hresp         => ahbmi.hresp,
		hcache        => ahbmi.hcache,
		-- 32 bit Wishbone
		wb32_adr_o    => wb_adr_o,
		wb32_dat_o    => wb_dat_o,
		wb32_dat_i    => wb_dat_i,
		wb32_we_o     => wb_we_o,
		wb32_cache_i  => wb_cache_i,
		wb32_sel_o    => wb_sel_o,
		wb32_cyc_o    => wb_cyc_o,
		wb32_stb_o    => wb_stb_o,
		wb32_ack_i    => wb_ack_i  );
	
----------------------------------------------------------------------
-- Interrupt Controller ----------------------------------------------
irqctrl0: entity work.irqctrl
	port map (
		clk           => clk,
		reset         => reset,
		-- 
		irq           => irq,
		--
		irqi          => irqi,
		irqo          => irqo );
 
----------------------------------------------------------------------
--  CLK and RESET  ---------------------------------------------------
resetn <= not reset;

end architecture;
