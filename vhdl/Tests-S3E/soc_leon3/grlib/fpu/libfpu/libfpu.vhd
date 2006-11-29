----------------------------------------------------------------------------
--  This file is a part of the GRLIB VHDL IP LIBRARY
--  Copyright (C) 2004 GAISLER RESEARCH
--
--  This program is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License as published by
--  the Free Software Foundation; either version 2 of the License, or
--  (at your option) any later version.
--
--  See the file COPYING for the full details of the license.
--
-----------------------------------------------------------------------------
-- package: 	libfpu
-- File:	libfpu.vhd
-- Author:	Jiri Gaisler, Gaisler Research
-- Description:	LEON3 FPU interface types
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;

package libfpu is 
  
  constant RDBITS : integer := 32;  --FIXME
  
  type fp_rf_in_type is record
    rd1addr 	: std_logic_vector(3 downto 0); -- read address 1
    rd2addr 	: std_logic_vector(3 downto 0); -- read address 2
    wraddr 	: std_logic_vector(3 downto 0); -- write address
    wrdata 	: std_logic_vector(RDBITS-1 downto 0);     -- write data
    ren1        : std_ulogic;			     -- read 1 enable
    ren2        : std_ulogic;			     -- read 2 enable
    wren        : std_ulogic;			     -- write enable
  end record;
 
  type fp_rf_out_type is record
    data1    	: std_logic_vector(RDBITS-1 downto 0); -- read data 1
    data2    	: std_logic_vector(RDBITS-1 downto 0); -- read data 2
  end record;  
  
  type fpc_pipeline_control_type is record
    pc    : std_logic_vector(31 downto 0);
    inst  : std_logic_vector(31 downto 0);
    cnt   : std_logic_vector(1 downto 0);
    trap  : std_ulogic;
    annul : std_ulogic;
    pv    : std_ulogic;
  end record;  

  type fpc_debug_in_type is record
    enable : std_ulogic;
    write  : std_ulogic;
    fsr    : std_ulogic;                            -- FSR access
    addr   : std_logic_vector(4 downto 0);
    data   : std_logic_vector(31 downto 0);
  end record;

  type fpc_debug_out_type is record
    data   : std_logic_vector(31 downto 0);
  end record;                           

  type fpc_in_type is record
    flush  	: std_ulogic;			  -- pipeline flush
    exack    	: std_ulogic;			  -- FP exception acknowledge
    a_rs1  	: std_logic_vector(4 downto 0);
    d             : fpc_pipeline_control_type;
    a             : fpc_pipeline_control_type;
    e             : fpc_pipeline_control_type;
    m             : fpc_pipeline_control_type;
    x             : fpc_pipeline_control_type;    
    lddata        : std_logic_vector(31 downto 0);     -- load data
    dbg           : fpc_debug_in_type;               -- debug signals
  end record;

  type fpc_out_type is record
    data          : std_logic_vector(31 downto 0); -- store data
    exc  	        : std_logic;			 -- FP exception
    cc            : std_logic_vector(1 downto 0);  -- FP condition codes
    ccv  	        : std_ulogic;			 -- FP condition codes valid
    ldlock        : std_logic;			 -- FP pipeline hold
    holdn          : std_ulogic;
    dbg           : fpc_debug_out_type;             -- FP debug signals    
  end record;

-- GRFPU interface

  type grfpu_in_type is record
    start   : std_logic;
    nonstd  : std_logic;
    flop    : std_logic_vector(8 downto 0);
    op1     : std_logic_vector(63 downto 0);
    op2     : std_logic_vector(63 downto 0);
    opid    : std_logic_vector(5 downto 0);
    flush   : std_logic;
    flushid : std_logic_vector(5 downto 0);
    rndmode : std_logic_vector(1 downto 0);                          
  end record;
  
  type grfpu_out_type is record
    res     : std_logic_vector(63 downto 0);
    exc     : std_logic_vector(5 downto 0);
    allow   : std_logic_vector(2 downto 0);
    rdy     : std_logic;
    cc      : std_logic_vector(1 downto 0);
    idout   : std_logic_vector(5 downto 0);
  end record;

  component grfpc
    generic (
      pclow    : integer range 0 to 2 := 2;
      dsu      : integer := 0;
      disas    : integer range 0 to 2 := 0       
    );
    port (
      rst    : in  std_ulogic;			-- Reset
      clk    : in  std_ulogic;
      holdn  : in  std_ulogic;			-- pipeline hold
      cpi    : in  fpc_in_type;
      fpo    : in  grfpu_out_type;      
      cpo    : out fpc_out_type;
      fpi    : out grfpu_in_type;      
      rfi1   : out fp_rf_in_type;
      rfi2   : out fp_rf_in_type;
      rfo1   : in  fp_rf_out_type;
      rfo2   : in  fp_rf_out_type
      );
  end component;

  component grfpu
  generic (infmul : integer range 0 to 1 := 1);
  port(
    clk     : in std_logic;
    reset   : in std_logic;
    start   : in std_logic;
    nonstd  : in std_logic;
    flop    : in std_logic_vector(8 downto 0);
    op1     : in std_logic_vector(63 downto 0);
    op2     : in std_logic_vector(63 downto 0);
    opid    : in std_logic_vector(5 downto 0);
    flush   : in std_logic;
    flushid : in std_logic_vector(5 downto 0);
    rndmode : in std_logic_vector(1 downto 0);
    res     : out std_logic_vector(63 downto 0);
    exc     : out std_logic_vector(5 downto 0);
    allow   : out std_logic_vector(2 downto 0);
    rdy     : out std_logic;
    cc      : out std_logic_vector(1 downto 0);
    idout   : out std_logic_vector(5 downto 0)
    );
  end component;

  component mfpc
  generic (
    pclow    : integer range 0 to 2 := 2;
    dsu      : integer := 0;
    disas    : integer range 0 to 2 := 0
    );
  port (
    rst    : in  std_ulogic;			-- Reset
    clk    : in  std_ulogic;
    holdn  : in  std_ulogic;			-- pipeline hold
    fpci   : in  fpc_in_type;
    fpco   : out fpc_out_type;
    rfi1   : out fp_rf_in_type;
    rfi2   : out fp_rf_in_type;
    rfo1   : in  fp_rf_out_type;
    rfo2   : in  fp_rf_out_type
    );
  end component;

  component grlfpc
  generic (
    pclow    : integer range 0 to 2 := 2;
    dsu      : integer := 0;
    disas    : integer range 0 to 2 := 0;
    grfpu    : integer range 0 to 1 := 0
    );
  port (
    rst    : in  std_ulogic;			-- Reset
    clk    : in  std_ulogic;
    holdn  : in  std_ulogic;			-- pipeline hold
    fpci   : in  fpc_in_type;
    fpco   : out fpc_out_type;
    rfi1   : out fp_rf_in_type;
    rfi2   : out fp_rf_in_type;
    rfo1   : in  fp_rf_out_type;
    rfo2   : in  fp_rf_out_type
    );
  end component;

  component mfpw 
  generic (pclow    : integer range 0 to 2 := 2;
           dsu      : integer range 0 to 1 := 0;           
           disas    : integer range 0 to 2 := 0
           );
  port (
    rst    : in  std_ulogic;			-- Reset
    clk    : in  std_ulogic;
    holdn  : in  std_ulogic;			-- pipeline hold
    cpi_flush  	: in std_ulogic;			  -- pipeline flush
    cpi_exack    	: in std_ulogic;			  -- FP exception acknowledge
    cpi_a_rs1  	: in std_logic_vector(4 downto 0);
    cpi_d_pc    : in std_logic_vector(31 downto 0);
    cpi_d_inst  : in std_logic_vector(31 downto 0);
    cpi_d_cnt   : in std_logic_vector(1 downto 0);
    cpi_d_trap  : in std_ulogic;
    cpi_d_annul : in std_ulogic;
    cpi_d_pv    : in std_ulogic;
    cpi_a_pc    : in std_logic_vector(31 downto 0);
    cpi_a_inst  : in std_logic_vector(31 downto 0);
    cpi_a_cnt   : in std_logic_vector(1 downto 0);
    cpi_a_trap  : in std_ulogic;
    cpi_a_annul : in std_ulogic;
    cpi_a_pv    : in std_ulogic;
    cpi_e_pc    : in std_logic_vector(31 downto 0);
    cpi_e_inst  : in std_logic_vector(31 downto 0);
    cpi_e_cnt   : in std_logic_vector(1 downto 0);
    cpi_e_trap  : in std_ulogic;
    cpi_e_annul : in std_ulogic;
    cpi_e_pv    : in std_ulogic;
    cpi_m_pc    : in std_logic_vector(31 downto 0);
    cpi_m_inst  : in std_logic_vector(31 downto 0);
    cpi_m_cnt   : in std_logic_vector(1 downto 0);
    cpi_m_trap  : in std_ulogic;
    cpi_m_annul : in std_ulogic;
    cpi_m_pv    : in std_ulogic;
    cpi_x_pc    : in std_logic_vector(31 downto 0);
    cpi_x_inst  : in std_logic_vector(31 downto 0);
    cpi_x_cnt   : in std_logic_vector(1 downto 0);
    cpi_x_trap  : in std_ulogic;
    cpi_x_annul : in std_ulogic;
    cpi_x_pv    : in std_ulogic;    
    cpi_lddata        : in std_logic_vector(31 downto 0);     -- load data
    cpi_dbg_enable : in std_ulogic;
    cpi_dbg_write  : in std_ulogic;
    cpi_dbg_fsr    : in std_ulogic;                            -- FSR access
    cpi_dbg_addr   : in std_logic_vector(4 downto 0);
    cpi_dbg_data   : in std_logic_vector(31 downto 0);

    cpo_data          : out std_logic_vector(31 downto 0); -- store data
    cpo_exc  	        : out std_logic;			 -- FP exception
    cpo_cc           : out std_logic_vector(1 downto 0);  -- FP condition codes
    cpo_ccv  	       : out std_ulogic;			 -- FP condition codes valid
    cpo_ldlock       : out std_logic;			 -- FP pipeline hold
    cpo_holdn         : out std_ulogic;
    cpo_dbg_data     : out std_logic_vector(31 downto 0);

    rfi1_rd1addr 	: out std_logic_vector(3 downto 0); 
    rfi1_rd2addr 	: out std_logic_vector(3 downto 0); 
    rfi1_wraddr 	: out std_logic_vector(3 downto 0); 
    rfi1_wrdata 	: out std_logic_vector(31 downto 0);
    rfi1_ren1        : out std_ulogic;			   
    rfi1_ren2        : out std_ulogic;			   
    rfi1_wren        : out std_ulogic;			   
    
    rfi2_rd1addr 	: out std_logic_vector(3 downto 0); 
    rfi2_rd2addr 	: out std_logic_vector(3 downto 0); 
    rfi2_wraddr 	: out std_logic_vector(3 downto 0); 
    rfi2_wrdata 	: out std_logic_vector(31 downto 0);
    rfi2_ren1        : out std_ulogic;
    rfi2_ren2        : out std_ulogic;			    
    rfi2_wren        : out std_ulogic;

    rfo1_data1    	: in std_logic_vector(31 downto 0);
    rfo1_data2    	: in std_logic_vector(31 downto 0);
    rfo2_data1    	: in std_logic_vector(31 downto 0);
    rfo2_data2    	: in std_logic_vector(31 downto 0)        
    );
  end component;

  component grfpw
  generic (infmul   : integer range 0 to 1 := 1;
           pclow    : integer range 0 to 2 := 2;
           dsu      : integer range 0 to 1 := 0;           
           disas    : integer range 0 to 2 := 0
           );
  port (
    rst    : in  std_ulogic;			-- Reset
    clk    : in  std_ulogic;
    holdn  : in  std_ulogic;			-- pipeline hold
    cpi_flush  	: in std_ulogic;			  -- pipeline flush
    cpi_exack    	: in std_ulogic;			  -- FP exception acknowledge
    cpi_a_rs1  	: in std_logic_vector(4 downto 0);
    cpi_d_pc    : in std_logic_vector(31 downto 0);
    cpi_d_inst  : in std_logic_vector(31 downto 0);
    cpi_d_cnt   : in std_logic_vector(1 downto 0);
    cpi_d_trap  : in std_ulogic;
    cpi_d_annul : in std_ulogic;
    cpi_d_pv    : in std_ulogic;
    cpi_a_pc    : in std_logic_vector(31 downto 0);
    cpi_a_inst  : in std_logic_vector(31 downto 0);
    cpi_a_cnt   : in std_logic_vector(1 downto 0);
    cpi_a_trap  : in std_ulogic;
    cpi_a_annul : in std_ulogic;
    cpi_a_pv    : in std_ulogic;
    cpi_e_pc    : in std_logic_vector(31 downto 0);
    cpi_e_inst  : in std_logic_vector(31 downto 0);
    cpi_e_cnt   : in std_logic_vector(1 downto 0);
    cpi_e_trap  : in std_ulogic;
    cpi_e_annul : in std_ulogic;
    cpi_e_pv    : in std_ulogic;
    cpi_m_pc    : in std_logic_vector(31 downto 0);
    cpi_m_inst  : in std_logic_vector(31 downto 0);
    cpi_m_cnt   : in std_logic_vector(1 downto 0);
    cpi_m_trap  : in std_ulogic;
    cpi_m_annul : in std_ulogic;
    cpi_m_pv    : in std_ulogic;
    cpi_x_pc    : in std_logic_vector(31 downto 0);
    cpi_x_inst  : in std_logic_vector(31 downto 0);
    cpi_x_cnt   : in std_logic_vector(1 downto 0);
    cpi_x_trap  : in std_ulogic;
    cpi_x_annul : in std_ulogic;
    cpi_x_pv    : in std_ulogic;    
    cpi_lddata        : in std_logic_vector(31 downto 0);     -- load data
    cpi_dbg_enable : in std_ulogic;
    cpi_dbg_write  : in std_ulogic;
    cpi_dbg_fsr    : in std_ulogic;                            -- FSR access
    cpi_dbg_addr   : in std_logic_vector(4 downto 0);
    cpi_dbg_data   : in std_logic_vector(31 downto 0);

    cpo_data          : out std_logic_vector(31 downto 0); -- store data
    cpo_exc  	        : out std_logic;			 -- FP exception
    cpo_cc           : out std_logic_vector(1 downto 0);  -- FP condition codes
    cpo_ccv  	       : out std_ulogic;			 -- FP condition codes valid
    cpo_ldlock       : out std_logic;			 -- FP pipeline hold
    cpo_holdn         : out std_ulogic;
    cpo_dbg_data     : out std_logic_vector(31 downto 0);

    rfi1_rd1addr 	: out std_logic_vector(3 downto 0); 
    rfi1_rd2addr 	: out std_logic_vector(3 downto 0); 
    rfi1_wraddr 	: out std_logic_vector(3 downto 0); 
    rfi1_wrdata 	: out std_logic_vector(31 downto 0);
    rfi1_ren1        : out std_ulogic;			   
    rfi1_ren2        : out std_ulogic;			   
    rfi1_wren        : out std_ulogic;			   
    
    rfi2_rd1addr 	: out std_logic_vector(3 downto 0); 
    rfi2_rd2addr 	: out std_logic_vector(3 downto 0); 
    rfi2_wraddr 	: out std_logic_vector(3 downto 0); 
    rfi2_wrdata 	: out std_logic_vector(31 downto 0);
    rfi2_ren1        : out std_ulogic;
    rfi2_ren2        : out std_ulogic;			        
    rfi2_wren        : out std_ulogic;

    rfo1_data1    	: in std_logic_vector(31 downto 0);
    rfo1_data2    	: in std_logic_vector(31 downto 0);
    
    rfo2_data1    	: in std_logic_vector(31 downto 0);
    rfo2_data2    	: in std_logic_vector(31 downto 0)
    );
  end component;

  component grlfpw 
  generic (pclow    : integer range 0 to 2 := 2;
           dsu      : integer range 0 to 1 := 1;           
           disas    : integer range 0 to 2 := 0
           );
  port (
    rst    : in  std_ulogic;			-- Reset
    clk    : in  std_ulogic;
    holdn  : in  std_ulogic;			-- pipeline hold
    cpi_flush  	: in std_ulogic;			  -- pipeline flush
    cpi_exack    	: in std_ulogic;			  -- FP exception acknowledge
    cpi_a_rs1  	: in std_logic_vector(4 downto 0);
    cpi_d_pc    : in std_logic_vector(31 downto 0);
    cpi_d_inst  : in std_logic_vector(31 downto 0);
    cpi_d_cnt   : in std_logic_vector(1 downto 0);
    cpi_d_trap  : in std_ulogic;
    cpi_d_annul : in std_ulogic;
    cpi_d_pv    : in std_ulogic;
    cpi_a_pc    : in std_logic_vector(31 downto 0);
    cpi_a_inst  : in std_logic_vector(31 downto 0);
    cpi_a_cnt   : in std_logic_vector(1 downto 0);
    cpi_a_trap  : in std_ulogic;
    cpi_a_annul : in std_ulogic;
    cpi_a_pv    : in std_ulogic;
    cpi_e_pc    : in std_logic_vector(31 downto 0);
    cpi_e_inst  : in std_logic_vector(31 downto 0);
    cpi_e_cnt   : in std_logic_vector(1 downto 0);
    cpi_e_trap  : in std_ulogic;
    cpi_e_annul : in std_ulogic;
    cpi_e_pv    : in std_ulogic;
    cpi_m_pc    : in std_logic_vector(31 downto 0);
    cpi_m_inst  : in std_logic_vector(31 downto 0);
    cpi_m_cnt   : in std_logic_vector(1 downto 0);
    cpi_m_trap  : in std_ulogic;
    cpi_m_annul : in std_ulogic;
    cpi_m_pv    : in std_ulogic;
    cpi_x_pc    : in std_logic_vector(31 downto 0);
    cpi_x_inst  : in std_logic_vector(31 downto 0);
    cpi_x_cnt   : in std_logic_vector(1 downto 0);
    cpi_x_trap  : in std_ulogic;
    cpi_x_annul : in std_ulogic;
    cpi_x_pv    : in std_ulogic;    
    cpi_lddata        : in std_logic_vector(31 downto 0);     -- load data
    cpi_dbg_enable : in std_ulogic;
    cpi_dbg_write  : in std_ulogic;
    cpi_dbg_fsr    : in std_ulogic;                            -- FSR access
    cpi_dbg_addr   : in std_logic_vector(4 downto 0);
    cpi_dbg_data   : in std_logic_vector(31 downto 0);

    cpo_data          : out std_logic_vector(31 downto 0); -- store data
    cpo_exc  	        : out std_logic;			 -- FP exception
    cpo_cc           : out std_logic_vector(1 downto 0);  -- FP condition codes
    cpo_ccv  	       : out std_ulogic;			 -- FP condition codes valid
    cpo_ldlock       : out std_logic;			 -- FP pipeline hold
    cpo_holdn         : out std_ulogic;
    cpo_dbg_data     : out std_logic_vector(31 downto 0);

    rfi1_rd1addr 	: out std_logic_vector(3 downto 0); 
    rfi1_rd2addr 	: out std_logic_vector(3 downto 0); 
    rfi1_wraddr 	: out std_logic_vector(3 downto 0); 
    rfi1_wrdata 	: out std_logic_vector(31 downto 0);
    rfi1_ren1        : out std_ulogic;			   
    rfi1_ren2        : out std_ulogic;			   
    rfi1_wren        : out std_ulogic;			   
    
    rfi2_rd1addr 	: out std_logic_vector(3 downto 0); 
    rfi2_rd2addr 	: out std_logic_vector(3 downto 0); 
    rfi2_wraddr 	: out std_logic_vector(3 downto 0); 
    rfi2_wrdata 	: out std_logic_vector(31 downto 0);
    rfi2_ren1        : out std_ulogic;
    rfi2_ren2        : out std_ulogic;			    
    rfi2_wren        : out std_ulogic;

    rfo1_data1    	: in std_logic_vector(31 downto 0);
    rfo1_data2    	: in std_logic_vector(31 downto 0);
    rfo2_data1    	: in std_logic_vector(31 downto 0);
    rfo2_data2    	: in std_logic_vector(31 downto 0)        
    );
  end component;

-- disassembly dummy module

  component cpu_disas is
  port (
    clk   : in std_ulogic;
    rstn  : in std_ulogic;
    dummy : out std_ulogic;
    inst  : in std_logic_vector(31 downto 0);
    pc    : in std_logic_vector(31 downto 2);
    result: in std_logic_vector(31 downto 0);
    index : in std_logic_vector(3 downto 0);
    wreg  : in std_ulogic;
    annul : in std_ulogic;
    holdn : in std_ulogic;
    pv    : in std_ulogic;
    trap  : in std_ulogic);
  end component;

end;
