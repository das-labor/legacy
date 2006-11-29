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
-- Entity:  ata
-- File: ata.vhd
-- Author:  Nils-Johan Wessman - Gaisler Research
-- Description: ATA controller package
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
library grlib;
use grlib.amba.all;

package ata is

type ata_type is record
   rst   : std_logic;
   ddi   : std_logic_vector(15 downto 0);
   ddo   : std_logic_vector(15 downto 0);
   oen   : std_logic;
   da    : std_logic_vector(2 downto 0);
   cs0   : std_logic; 
   cs1   : std_logic;
   dior  : std_logic;
   diow  : std_logic;
   iordy : std_logic;
   intrq : std_logic;
   dmarq : std_logic;
   dmack : std_logic;
end record;

type cf_out_type is record
   power    : std_logic;
   atasel   : std_logic;
   we       : std_logic;
   csel     : std_logic;
   da       : std_logic_vector(10 downto 3);
end record;

type atadev_type is record
   rst   : std_logic;
   data  : std_logic_vector(15 downto 0);
   da    : std_logic_vector(2 downto 0);
   cs0   : std_logic; 
   cs1   : std_logic;
   dior  : std_logic;
   diow  : std_logic;
   iordy : std_logic;
   intrq : std_logic;
end record;

component atactrl is
 generic (
   hindex  : integer := 0;
   haddr   : integer := 0;
   hmask   : integer := 16#ff0#;
   pirq    : integer := 0;
  
   TWIDTH : natural := 8;                      -- counter width
   
   -- PIO mode 0 settings (@100MHz clock)
   PIO_mode0_T1 : natural := 6;                -- 70ns
   PIO_mode0_T2 : natural := 28;               -- 290ns
   PIO_mode0_T4 : natural := 2;                -- 30ns
   PIO_mode0_Teoc : natural := 23              -- 240ns ==> T0 - T1 - T2 = 600 - 70 - 290 = 240
    
 );
 port (
   rst     : in  std_ulogic;
   arst    : in  std_ulogic;
   clk     : in  std_ulogic;
   ahbsi   : in  ahb_slv_in_type;
   ahbso   : out ahb_slv_out_type;
   cfo     : out cf_out_type;

   -- ATA signals
   ddin       : in  std_logic_vector(15 downto 0);
   iordy      : in  std_logic;
   intrq      : in  std_logic;
   ata_resetn : out std_logic;
   ddout      : out std_logic_vector(15 downto 0);
   ddoe       : out std_logic;
   da         : out std_logic_vector(2 downto 0);
   cs0n       : out std_logic;
   cs1n       : out std_logic;
   diorn      : out std_logic;
   diown      : out std_logic;
   dmack      : out std_logic
 );
end component atactrl; 


end;
