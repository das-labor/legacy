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
----------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
library grlib;
use grlib.stdlib.all;
use grlib.amba.all;
library gaisler;
use gaisler.misc.all;

package usb is
  type usb_in_type is record
    din        : std_logic_vector(7 downto 0);
    dinh       : std_logic_vector(7 downto 0);
    rxactive   : std_ulogic;
    rxvalid    : std_ulogic;
    rhvalid    : std_ulogic;
    rxerror    : std_ulogic;
    txready    : std_ulogic;
    linestate  : std_logic_vector(1 downto 0);
    vbus       : std_ulogic;
  end record;
  
  type usb_out_type is record
    dout       : std_logic_vector(7 downto 0);
    douth      : std_logic_vector(7 downto 0);
    txvalid    : std_ulogic;
    thvalid    : std_ulogic;
    opmode     : std_logic_vector(1 downto 0);
    xcvrselect : std_ulogic;
    termselect : std_ulogic;
    suspend    : std_ulogic;
    reset      : std_ulogic;

    --debug
    sie        : std_logic_vector(4 downto 0);
    sne        : std_logic_vector(2 downto 0);
    snehs      : std_logic_vector(2 downto 0);
    aie        : std_logic_vector(4 downto 0);
    suspenddbg : std_ulogic;
    vbusa      : std_ulogic;
    vbusu      : std_ulogic;
    urst       : std_ulogic;
    rsta       : std_ulogic;
    grsta      : std_ulogic;
    dev_addr   : std_logic_vector(6 downto 0);
    configured : std_ulogic;
    status     : std_ulogic;
    set_address: std_ulogic;
    address_set: std_ulogic;
    setup      : std_ulogic;
    toggle     : std_ulogic;
    epno       : std_logic_vector(3 downto 0);
    tpid       : std_logic_vector(3 downto 0);
    wrop       : std_ulogic;
    terr       : std_ulogic;
    odav0      : std_ulogic;
    odav1      : std_ulogic;
    obufsel    : std_ulogic;
    idav0      : std_ulogic;
    idav1      : std_ulogic;
    ibufsel    : std_ulogic;
    ahbmreq    : std_ulogic;
    ahbmack    : std_ulogic;
    ahbmwr     : std_ulogic;
    ahbmrd     : std_ulogic;
    ahbmbusy   : std_ulogic;
    bufsel     : std_ulogic;
    aepno      : std_logic_vector(4 downto 0);
    addr       : std_logic_vector(31 downto 0);
    wordcnt    : std_logic_vector(13 downto 0);
    bytecnt    : std_logic_vector(10 downto 0);
    bytecnt0   : std_logic_vector(10 downto 0);
    bytecnt1   : std_logic_vector(10 downto 0);
    lerr       : std_ulogic;
  end record;

  component usbdcl is
    generic (
      hindex     : integer := 0;
      memtech    : integer := 1
    );
    port (
      uclk       : in  std_ulogic;
      usbi       : in  usb_in_type;
      usbo       : out usb_out_type;
      hclk       : in  std_ulogic;
      hrst       : in  std_ulogic;
      ahbi       : in  ahb_mst_in_type;
      ahbo       : out ahb_mst_out_type
    );
  end component usbdcl;

  -- descriptor type
  type descriptor_type is array (natural range <>) of std_logic_vector(7 downto 0);
  
end usb;
