------------------------------------------------------------------------------
--  This file is a part of the GRLIB VHDL IP LIBRARY
--  Copyright (C) 2003, Gaisler Research
--
--  This program is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License as published by
--  the Free Software Foundation; either version 2 of the License, or
--  (at your option) any later version.
--
--  This program is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU General Public License for more details.
--
--  You should have received a copy of the GNU General Public License
--  along with this program; if not, write to the Free Software
--  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
-----------------------------------------------------------------------------
-- Package: 	misc
-- File:	misc.vhd
-- Author:	Jiri Gaisler - Gaisler Research
-- Description:	Misc models
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;

library grlib;
use grlib.amba.all;
use grlib.devices.all;
use grlib.stdlib.all;
library techmap;
use techmap.gencomp.all;
library gaisler;
use gaisler.leon3.all;

package misc is

-- reset generator with filter

  component rstgen
  generic (acthigh : integer := 0; syncrst : integer := 0);
  port (
    rstin     : in  std_ulogic;
    clk       : in  std_ulogic;
    clklock   : in  std_ulogic;
    rstout    : out std_ulogic;
    rstoutraw : out std_ulogic);
  end component;

  type gptimer_in_type is record
    dhalt    : std_ulogic;
    extclk   : std_ulogic;
  end record;

  type gptimer_out_type is record
    tick     : std_logic_vector(0 to 7);
    timer1   : std_logic_vector(31 downto 0);
    wdogn    : std_ulogic;
    wdog    : std_ulogic;
  end record;

  component gptimer
  generic (
    pindex   : integer := 0;
    paddr    : integer := 0;
    pmask    : integer := 16#fff#;
    pirq     : integer := 0;
    sepirq   : integer := 0;	-- use separate interrupts for each timer
    sbits    : integer := 16;			-- scaler bits
    ntimers  : integer range 1 to 7 := 1; 	-- number of timers
    nbits    : integer := 32;			-- timer bits
    wdog     : integer := 0
  );
  port (
    rst    : in  std_ulogic;
    clk    : in  std_ulogic;
    apbi   : in  apb_slv_in_type;
    apbo   : out apb_slv_out_type;
    gpti   : in gptimer_in_type;
    gpto   : out gptimer_out_type
  );
  end component;

-- 32-bit ram with AHB interface

  component ahbram
  generic (
    hindex  : integer := 0;
    haddr   : integer := 0;
    hmask   : integer := 16#fff#;
    tech    : integer := DEFMEMTECH; 
    kbytes  : integer := 1); 
  port (
    rst    : in  std_ulogic;
    clk    : in  std_ulogic;
    ahbsi  : in  ahb_slv_in_type;
    ahbso  : out ahb_slv_out_type);
  end component;

  type ahbram_out_type is record
    ce : std_ulogic; 
  end record;
  
  component ftahbram is
    generic (
      hindex    : integer := 0;
      haddr     : integer := 0;
      hmask     : integer := 16#fff#;
      tech      : integer := DEFMEMTECH; 
      kbytes    : integer := 1;
      pindex    : integer := 0;
      paddr     : integer := 0;
      pmask     : integer := 16#fff#;
      edacen    : integer := 1;
      autoscrub : integer := 0; 
      errcnten  : integer := 0;
      cntbits   : integer range 1 to 8 := 1);
    port (
      rst     : in  std_ulogic;
      clk     : in  std_ulogic;
      ahbsi   : in  ahb_slv_in_type;
      ahbso   : out ahb_slv_out_type;
      apbi    : in  apb_slv_in_type;
      apbo    : out apb_slv_out_type;
      aramo   : out ahbram_out_type
    );
  end component;

  component ahbtrace is
  generic (
    hindex  : integer := 0;
    ioaddr    : integer := 16#000#;
    iomask    : integer := 16#E00#;
    tech    : integer := DEFMEMTECH; 
    irq     : integer := 0; 
    kbytes  : integer := 1); 
  port (
    rst    : in  std_ulogic;
    clk    : in  std_ulogic;
    ahbmi  : in  ahb_mst_in_type;
    ahbsi  : in  ahb_slv_in_type;
    ahbso  : out ahb_slv_out_type
  );
  end component; 

type ahb_dma_in_type is record
  address         : std_logic_vector(31 downto 0);
  wdata           : std_logic_vector(31 downto 0);
  start           : std_ulogic;
  burst           : std_ulogic;
  write           : std_ulogic;
  busy            : std_ulogic;
  irq             : std_ulogic;
  size            : std_logic_vector(1 downto 0);
end record;

type ahb_dma_out_type is record
  start           : std_ulogic;
  active          : std_ulogic;
  ready           : std_ulogic;
  retry           : std_ulogic;
  mexc            : std_ulogic;
  haddr           : std_logic_vector(9 downto 0);
  rdata           : std_logic_vector(31 downto 0);
end record;

  component ahbmst
  generic (
    hindex  : integer := 0;
    hirq    : integer := 0;
    venid   : integer := VENDOR_GAISLER;
    devid   : integer := 0;
    version : integer := 0;
    chprot  : integer := 3;
    incaddr : integer := 0); 
   port (
      rst  : in  std_ulogic;
      clk  : in  std_ulogic;
      dmai : in ahb_dma_in_type;
      dmao : out ahb_dma_out_type;
      ahbi : in  ahb_mst_in_type;
      ahbo : out ahb_mst_out_type 
      );
  end component;

  type gpio_in_type is record
    din      : std_logic_vector(31 downto 0);
  end record;

  type gpio_out_type is record
    dout     : std_logic_vector(31 downto 0);
    oen      : std_logic_vector(31 downto 0);
    val      : std_logic_vector(31 downto 0);
  end record;

  type ahb2ahb_ctrl_type is record
    slck  : std_ulogic;
    blck  : std_ulogic;
  end record;                             
  
 component grgpio
  generic (
    pindex   : integer := 0;
    paddr    : integer := 0;
    pmask    : integer := 16#fff#;
    imask    : integer := 16#0000#;
    nbits    : integer := 16;			-- GPIO bits
    oepol    : integer := 0;                    -- Output enable polarity
    syncrst  : integer := 0
  );
  port (
    rst    : in  std_ulogic;
    clk    : in  std_ulogic;
    apbi   : in  apb_slv_in_type;
    apbo   : out apb_slv_out_type;
    gpioi  : in  gpio_in_type;
    gpioo  : out gpio_out_type
  );
  end component;

  component ahb2ahb
  generic(
    tech    : integer := 0;    
    hsindex : integer := 0;
    hmindex : integer := 0;
    slv     : integer := 0;
    dir     : integer := 0;   -- 0 - down, 1 - up
    ffact    : integer := 0;
    pfen    : integer range 0 to 1 := 0;
    rbufsz  : integer range 2 to 32 := 8;
    wbufsz  : integer range 2 to 32 := 2;    
    iburst   : integer range 4 to 8 :=  8;
    rburst   : integer range 2 to 32 := 8;
    irqsync : integer range 0 to 1 := 0;    
    bar0     : integer range 0 to 1073741823 := 0;
    bar1     : integer range 0 to 1073741823 := 0;
    bar2     : integer range 0 to 1073741823 := 0;
    bar3     : integer range 0 to 1073741823 := 0;           
    sbus     : integer := 0;
    mbus     : integer := 0;    
    ioarea   : integer := 0);
  port (
    rstn   : in std_ulogic;    
    hclkm  : in std_ulogic;
    hclks  : in std_ulogic;
    ahbsi  : in ahb_slv_in_type;
    ahbso  : out ahb_slv_out_type;
    ahbmi  : in ahb_mst_in_type;
    ahbmo  : out ahb_mst_out_type;
    ahbso2 : in ahb_slv_out_vector;
    lcki     : in ahb2ahb_ctrl_type;
    lcko     : out ahb2ahb_ctrl_type
    );
  end component;
  
  component ahbbridge 
  generic(
    tech        : integer := 0;
    ffact       : integer := 2;
    ncpu        : integer := 1;
    -- high-speed bus    
    hsb_hsindex : integer := 0;
    hsb_hmindex : integer := 0;
    hsb_iclsize : integer range 4 to 8 := 8;
    hsb_bank0     : integer range 0 to 1073741823 := 0;
    hsb_bank1     : integer range 0 to 1073741823 := 0;
    hsb_bank2     : integer range 0 to 1073741823 := 0;
    hsb_bank3     : integer range 0 to 1073741823 := 0;
    hsb_ioarea   : integer := 0;
    -- low-speed bus
    lsb_hsindex : integer := 0;
    lsb_hmindex : integer := 0;
    lsb_rburst  : integer range 16 to 32 := 16;
    lsb_wburst  : integer range 2 to 32 :=  8;
    lsb_bank0     : integer range 0 to 1073741823 := 0;
    lsb_bank1     : integer range 0 to 1073741823 := 0;
    lsb_bank2     : integer range 0 to 1073741823 := 0;
    lsb_bank3     : integer range 0 to 1073741823 := 0;
    lsb_ioarea   : integer := 0);    
  port (
    rstn    : in std_ulogic;    
    hsb_clk : in std_ulogic;
    lsb_clk : in std_ulogic;
    hsb_ahbsi : in  ahb_slv_in_type;
    hsb_ahbso : out ahb_slv_out_type;
    hsb_ahbsov: in  ahb_slv_out_vector;
    hsb_ahbmi : in  ahb_mst_in_type;
    hsb_ahbmo : out ahb_mst_out_type;
    lsb_ahbsi : in  ahb_slv_in_type;
    lsb_ahbso : out ahb_slv_out_type;
    lsb_ahbsov: in  ahb_slv_out_vector;
    lsb_ahbmi : in  ahb_mst_in_type;
    lsb_ahbmo : out ahb_mst_out_type;
    irqc_irqo : in irq_in_vector(0 to ncpu-1);
    irqc_irqi : out irq_out_vector(0 to ncpu-1);
    cpu_irqo  : in irq_out_vector(0 to ncpu-1);
    cpu_irqi  : out irq_in_vector(0 to ncpu-1);
    dhalti    : in std_ulogic;
    dhalto    : out std_ulogic);
  end component;

  function ahb2ahb_membar(memaddr : ahb_addr_type; prefetch, cache : std_ulogic;
                          addrmask : ahb_addr_type)
  return integer;  

  function ahb2ahb_iobar(memaddr : ahb_addr_type; addrmask : ahb_addr_type)
  return integer;  
  
  type ahbstat_in_type is record
    cerror : std_logic_vector(0 to NAHBSLV-1);
  end record; 
  
  component ahbstat is
    generic(
      pindex : integer := 0;
      paddr  : integer := 0;
      pmask  : integer := 16#FFF#;
      pirq   : integer := 0;
      nftslv : integer range 1 to NAHBSLV - 1 := 3);
    port(
      rst   : in std_ulogic;
      clk   : in std_ulogic;
      ahbmi : in ahb_mst_in_type;
      ahbsi : in ahb_slv_in_type;
      stati : in ahbstat_in_type;
      apbi  : in apb_slv_in_type;
      apbo  : out apb_slv_out_type
    );
  end component;

  type nuhosp3_in_type is record
    flash_d	: std_logic_vector(15 downto 0);
    smsc_data 	: std_logic_vector(31 downto 0);
    smsc_ardy  	: std_ulogic;
    smsc_intr  	: std_ulogic;
    smsc_nldev 	: std_ulogic;
    lcd_data 	: std_logic_vector(7 downto 0);
  end record;

  type nuhosp3_out_type is record
    flash_a 	: std_logic_vector(20 downto 0);
    flash_d	: std_logic_vector(15 downto 0);
    flash_oen  	: std_ulogic;
    flash_wen 	: std_ulogic;
    flash_cen  	: std_ulogic;
    smsc_addr 	: std_logic_vector(14 downto 0);
    smsc_data 	: std_logic_vector(31 downto 0);
    smsc_nbe  	: std_logic_vector(3 downto 0);
    smsc_resetn	: std_ulogic;
    smsc_nrd   	: std_ulogic;
    smsc_nwr   	: std_ulogic;
    smsc_ncs   	: std_ulogic;
    smsc_aen   	: std_ulogic;
    smsc_lclk  	: std_ulogic;
    smsc_wnr   	: std_ulogic;
    smsc_rdyrtn	: std_ulogic;
    smsc_cycle 	: std_ulogic;
    smsc_nads  	: std_ulogic;
    smsc_ben   	: std_ulogic;
    lcd_data 	: std_logic_vector(7 downto 0);
    lcd_rs	: std_ulogic;
    lcd_rw	: std_ulogic;
    lcd_en	: std_ulogic;
    lcd_backl	: std_ulogic;
    lcd_ben	: std_ulogic;
  end record;

  component nuhosp3
  generic (
    hindex  : integer := 0;
    haddr   : integer := 0;
    hmask   : integer := 16#fff#;
    ioaddr : integer := 16#200#;
    iomask : integer := 16#fff#); 
  port (
    rst    : in  std_ulogic;
    clk    : in  std_ulogic;
    ahbsi  : in  ahb_slv_in_type;
    ahbso  : out ahb_slv_out_type;
    nui    : in  nuhosp3_in_type;
    nuo    : out nuhosp3_out_type
  );
  end component;

-- On-chip Logic Analyzer

  component logan is
  
  generic (
    dbits   : integer range 0 to 256 := 32;        -- Number of traced signals
    depth   : integer range 256 to 16384 := 1024;  -- Depth of trace buffer
    trigl   : integer range 1 to 63 := 1;          -- Number of trigger levels
    usereg  : integer range 0 to 1 := 1;           -- Use input register
    usequal : integer range 0 to 1 := 0;
    pindex  : integer := 0;
    paddr   : integer := 0;
    pmask   : integer := 16#F00#;
    memtech : integer := DEFMEMTECH);                           
  port (
    rstn    : in  std_logic;
    clk     : in  std_logic;
    tclk    : in  std_logic;
    apbi    : in  apb_slv_in_type;                        -- APB in record
    apbo    : out apb_slv_out_type;                       -- APB out record
    signals : in  std_logic_vector(dbits - 1 downto 0));  -- Traced signals

  end component;

  type ps2_in_type is record
    ps2_clk_i      : std_ulogic;
    ps2_data_i     : std_ulogic;
  end record;

  type ps2_out_type is record
    ps2_clk_o      : std_ulogic;
    ps2_clk_oe     : std_ulogic;
    ps2_data_o     : std_ulogic;
    ps2_data_oe    : std_ulogic;
  end record;

  component apbps2 
  generic(
    pindex      : integer := 0; 
    paddr       : integer := 0;
    pmask       : integer := 16#fff#;
    pirq        : integer := 0;
    fKHz        : integer := 50000;
    fixed       : integer := 1);
  port(
    rst         : in std_ulogic;                -- Global asynchronous reset
    clk         : in std_ulogic;                -- Global clock
    apbi        : in apb_slv_in_type;
    apbo        : out apb_slv_out_type; 
    ps2i        : in ps2_in_type;
    ps2o        : out ps2_out_type
    );
  end component;

  type apbvga_out_type is record
    hsync           : std_ulogic;                       -- horizontal sync
    vsync           : std_ulogic;                       -- vertical sync
    comp_sync       : std_ulogic;                       -- composite sync
    blank           : std_ulogic;                       -- blank signal
    video_out_r     : std_logic_vector(7 downto 0);     -- red channel
    video_out_g     : std_logic_vector(7 downto 0);     -- green channel
    video_out_b     : std_logic_vector(7 downto 0);     -- blue channel 
  end record;

  component apbvga 
  generic(
    memtech     : integer := DEFMEMTECH;
    pindex      : integer := 0; 
    paddr       : integer := 0;
    pmask       : integer := 16#fff#);
  port( 
    rst             : in std_ulogic;                        -- Global asynchronous reset
    clk             : in std_ulogic;                        -- Global clock
    vgaclk          : in std_ulogic;                        -- VGA clock
    apbi            : in apb_slv_in_type;
    apbo            : out apb_slv_out_type;
    vgao            : out apbvga_out_type
    );
  end component;

  constant vgao_none : apbvga_out_type := 
	('0', '0', '0', '0', "00000000", "00000000", "00000000");
  constant ps2o_none : ps2_out_type := ('1', '1', '1', '1');

--  component ahbrom
--  generic (
--    hindex  : integer := 0;
--    haddr   : integer := 0;
--    hmask   : integer := 16#fff#;
--    pipe    : integer := 0;
--    tech    : integer := 0;
--    kbytes  : integer := 1);
--  port (
--    rst     : in  std_ulogic;
--    clk     : in  std_ulogic;
--    ahbsi   : in  ahb_slv_in_type;
--    ahbso   : out ahb_slv_out_type
--  );
--  end component;

end;


package body misc is

  function ahb2ahb_membar(memaddr : ahb_addr_type; prefetch, cache : std_ulogic;
                          addrmask : ahb_addr_type)    
  return integer is
    variable tmp : std_logic_vector(29 downto 0);
    variable bar : std_logic_vector(31 downto 0);
    variable res : integer range 0 to 1073741823;
  begin    
    bar := ahb_membar(memaddr, prefetch, cache, addrmask);
    tmp := (others => '0');
    tmp(29 downto 18) := bar(31 downto 20);
    tmp(17 downto 0) := bar(17 downto 0);
    res := conv_integer(tmp);
    return(res);
  end;

  function ahb2ahb_iobar(memaddr : ahb_addr_type; addrmask : ahb_addr_type)
  return integer is
    variable tmp : std_logic_vector(29 downto 0);
    variable bar : std_logic_vector(31 downto 0);
    variable res : integer range 0 to 1073741823;    
  begin
    bar := ahb_iobar(memaddr, addrmask);
    tmp := (others => '0');
    tmp(29 downto 18) := bar(31 downto 20);
    tmp(17 downto 0) := bar(17 downto 0);
    res := conv_integer(tmp);
    return(res);
  end;
  
end;
