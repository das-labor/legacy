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
-- Entity:      tap_gen
-- File:        tap_gen.vhd
-- Author:      Edvin Catovic - Gaisler Research
-- Description: JTAG Test Access Port (TAP) Controller component declaration
------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.all;

package libtap is

component tap_gen 
  generic (
    irlen  : integer range 2 to 8 := 2;
    idcode : integer range 0 to 255 := 9;
    id_msb : integer range 0 to 65536 := 0;
    id_lsb : integer range 0 to 65536 := 0);
  port (
    rst         : in std_ulogic;
    tck         : in std_ulogic;
    tms         : in std_ulogic;
    tdi         : in std_ulogic;
    tdo         : out std_ulogic;
    tapi_en1    : in std_ulogic;
    tapi_tdo1   : in std_ulogic;
    tapi_tdo2   : in std_ulogic;
    tapo_tck    : out std_ulogic;
    tapo_tdi    : out std_ulogic;
    tapo_inst   : out std_logic_vector(7 downto 0);
    tapo_rst    : out std_ulogic;
    tapo_capt   : out std_ulogic;
    tapo_shft   : out std_ulogic;
    tapo_upd    : out std_ulogic;
    tapo_xsel1  : out std_ulogic;
    tapo_xsel2  : out std_ulogic
    );
end component;

component virtex_tap 
port (
     tapi_tdo1   : in std_ulogic;
     tapi_tdo2   : in std_ulogic;
     tapo_tck    : out std_ulogic;
     tapo_tdi    : out std_ulogic;
     tapo_rst    : out std_ulogic;
     tapo_capt   : out std_ulogic;
     tapo_shft   : out std_ulogic;
     tapo_upd    : out std_ulogic;
     tapo_xsel1  : out std_ulogic;
     tapo_xsel2  : out std_ulogic
    );
end component;

component virtex2_tap 
port (
     tapi_tdo1   : in std_ulogic;
     tapi_tdo2   : in std_ulogic;
     tapo_tck    : out std_ulogic;
     tapo_tdi    : out std_ulogic;
     tapo_rst    : out std_ulogic;
     tapo_capt   : out std_ulogic;
     tapo_shft   : out std_ulogic;
     tapo_upd    : out std_ulogic;
     tapo_xsel1  : out std_ulogic;
     tapo_xsel2  : out std_ulogic
    );
end component;

component virtex4_tap 
port (
     tapi_tdo1   : in std_ulogic;
     tapi_tdo2   : in std_ulogic;
     tapo_tck    : out std_ulogic;
     tapo_tdi    : out std_ulogic;
     tapo_rst    : out std_ulogic;
     tapo_capt   : out std_ulogic;
     tapo_shft   : out std_ulogic;
     tapo_upd    : out std_ulogic;
     tapo_xsel1  : out std_ulogic;
     tapo_xsel2  : out std_ulogic
    );
end component;

component spartan3_tap 
port (
     tapi_tdo1   : in std_ulogic;
     tapi_tdo2   : in std_ulogic;
     tapo_tck    : out std_ulogic;
     tapo_tdi    : out std_ulogic;
     tapo_rst    : out std_ulogic;
     tapo_capt   : out std_ulogic;
     tapo_shft   : out std_ulogic;
     tapo_upd    : out std_ulogic;
     tapo_xsel1  : out std_ulogic;
     tapo_xsel2  : out std_ulogic
    );
end component;

end;
