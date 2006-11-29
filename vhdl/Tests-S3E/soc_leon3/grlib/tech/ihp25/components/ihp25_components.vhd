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
-- Package: 	ihp25_components
-- File:	ihp25_components.vhd
-- Author:	Marko Isomaki - Gaisler Research
-- Description:	IHP 0.25 component declarations
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;

package ihp25_components is
  --single port memory
  component sram2k_pin is
    port(
      do0, do1, do2, do3, do4, do5, do6, do7, do8, do9, do10, do11,
      do12, do13, do14, do15, do16, do17, do18, do19, do20, do21,
      do22, do23, do24, do25, do26, do27, do28, do29, do30, do31    : out std_ulogic; 
      di0, di1, di2, di3, di4, di5, di6, di7, di8, di9, di10, di11,  
      di12, di13, di14, di15, di16, di17, di18, di19, di20, di21,   
      di22, di23, di24, di25, di26, di27, di28, di29, di30, di31,   
      a0, a1, a2, a3, a4, a5, a6, a7, a8,                           
      oen0, oen1, oen2, oen3,                                       
      iwen0, iwen1, iwen2, iwen3,                                   
      clk, wrb, ceb                                                 : in std_ulogic);                                                 
  end component; 

  component M_64_128_8f is
    port(
      out0, out1, out2, out3, out4, out5, out6, out7 : out std_ulogic; 
      data0, data1, data2, data3, data4, data5, data6, data7,
      adr0, adr1, adr2, adr3, adr4, adr5, adr6,
      CLK, WRSTR, CS, RDSTR : in std_ulogic);                                                 
  end component; 

  component M_64_128_32f is
    port(
      out0, out1, out2, out3, out4, out5, out6, out7, out8, out9, out10, out11,
      out12, out13, out14, out15, out16, out17, out18, out19, out20, out21,
      out22, out23, out24, out25, out26, out27, out28, out29, out30, out31 : out std_ulogic; 
      data0, data1, data2, data3, data4, data5, data6, data7, data8, data9, data10, data11,  
      data12, data13, data14, data15, data16, data17, data18, data19, data20, data21,   
      data22, data23, data24, data25, data26, data27, data28, data29, data30, data31,   
      adr0, adr1, adr2, adr3, adr4, adr5, adr6,
      CLK, WRSTR, CS, RDSTR : in std_ulogic);                                                 
  end component; 

  component M_64_512_8f is
    port(
      out0, out1, out2, out3, out4, out5, out6, out7 : out std_ulogic; 
      data0, data1, data2, data3, data4, data5, data6, data7,
      adr0, adr1, adr2, adr3, adr4, adr5, adr6, adr7, adr8,                           
      CLK, WRSTR, CS, RDSTR : in std_ulogic);                                                 
  end component; 

  component M_64_512_32f is
    port(
      out0, out1, out2, out3, out4, out5, out6, out7, out8, out9, out10, out11,
      out12, out13, out14, out15, out16, out17, out18, out19, out20, out21,
      out22, out23, out24, out25, out26, out27, out28, out29, out30, out31 : out std_ulogic; 
      data0, data1, data2, data3, data4, data5, data6, data7, data8, data9, data10, data11,  
      data12, data13, data14, data15, data16, data17, data18, data19, data20, data21,   
      data22, data23, data24, data25, data26, data27, data28, data29, data30, data31,   
      adr0, adr1, adr2, adr3, adr4, adr5, adr6, adr7, adr8,                           
      CLK, WRSTR, CS, RDSTR : in std_ulogic);                                                 
  end component; 

  --3.3 V compliant input pad 16X input drive
  component inbuf3_16 is
    port(
      di  : out std_logic;
      pad : in std_logic);
  end component; 

  --3.3 V compliant i/o pad 4mA output drive 16X input drive
  component iobuf3_16_4 is
    port(
      pad : inout std_logic;
      di  : out   std_logic;
      do  : in    std_logic;
      en  : in    std_logic);
  end component;

  --3.3 V compliant i/o pad 8mA output drive 16X input drive
  component iobuf3_16_8 is
    port(
      pad : inout std_logic;
      di  : out   std_logic;
      do  : in    std_logic;
      en  : in    std_logic);
  end component;
  
  --3.3 V compliant i/o pad 12mA output drive 16X input drive
  component iobuf3_16_12 is
    port(
      pad : inout std_logic;
      di  : out   std_logic;
      do  : in    std_logic;
      en  : in    std_logic);
  end component;

  --3.3 compliant i/o pad with pull-up 4mA output drive 16X input drive
  component iobuf3_16pu_4 is
    port(
      pad : inout std_logic;
      di  : out   std_logic;
      do  : in    std_logic;
      en  : in    std_logic);
  end component;

  --3.3 compliant i/o pad with pull-up 8mA output drive 16X input drive
  component iobuf3_16pu_8 is
    port(
      pad : inout std_logic;
      di  : out   std_logic;
      do  : in    std_logic;
      en  : in    std_logic);
  end component;

  --3.3 compliant i/o pad with pull-up 12mA output drive 16X input drive
  component iobuf3_16pu_12 is
    port(
      pad : inout std_logic;
      di  : out   std_logic;
      do  : in    std_logic;
      en  : in    std_logic);
  end component; 
end package;
