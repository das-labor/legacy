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
-- Entity: 	devices
-- File:	devices.vhd
-- Author:	Jiri Gaisler, Gaisler Research
-- Description:	Vendor and devices id's for amba plug&play
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
library grlib;
use grlib.amba.all;
-- pragma translate_off
use std.textio.all;
-- pragma translate_on

package devices_esa is

-- Vendor code

  constant VENDOR_ESA       : amba_vendor_type := 16#04#;

-- European Space Agency device id's

  constant ESA_LEON2        : amba_device_type := 16#002#;
  constant ESA_LEON2APB     : amba_device_type := 16#003#;
  constant ESA_IRQ          : amba_device_type := 16#005#;
  constant ESA_TIMER        : amba_device_type := 16#006#;
  constant ESA_UART         : amba_device_type := 16#007#;
  constant ESA_CFG          : amba_device_type := 16#008#;
  constant ESA_IO           : amba_device_type := 16#009#;
  constant ESA_MCTRL        : amba_device_type := 16#00F#;
  constant ESA_PCIARB       : amba_device_type := 16#010#;
  constant ESA_HURRICANE    : amba_device_type := 16#011#;
  constant ESA_SPW_RMAP     : amba_device_type := 16#012#;
  constant ESA_AHBUART      : amba_device_type := 16#013#;


-- pragma translate_off

  constant ESA_DESC : vendor_description := "European Space Agency   ";

  constant esa_device_table : device_table_type := (
   ESA_LEON2        => "Leon2 SPARC V8 Processor       ",
   ESA_LEON2APB     => "Leon2 Peripheral Bus           ",
   ESA_IRQ          => "Leon2 Interrupt Controller     ",
   ESA_TIMER        => "Leon2 Timer                    ",
   ESA_UART         => "Leon2 UART                     ",
   ESA_CFG          => "Leon2 Configuration Register   ",
   ESA_IO           => "Leon2 Input/Output             ",
   ESA_MCTRL        => "Leon2 Memory Controller        ",
   ESA_PCIARB       => "PCI Arbiter                    ",
   ESA_HURRICANE    => "HurriCANe/HurryAMBA CAN Ctrl   ",
   ESA_SPW_RMAP     => "UoD/Saab SpaceWire/RMAP link   ",
   ESA_AHBUART      => "Leon2 AHB Debug UART           ",
   others           => "Unknown Device                 ");

   constant esa_lib : vendor_library_type := (
     vendorid       => VENDOR_ESA,
     vendordesc     => ESA_DESC,
     device_table   => esa_device_table
   );

-- pragma translate_on

end;