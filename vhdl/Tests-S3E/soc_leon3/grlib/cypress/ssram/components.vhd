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
-- Package: 	components
-- File:	components.vhd
-- Author:	Jiri Gaisler, Gaisler Research
-- Description:	Component declaration of Cypress sync-sram
------------------------------------------------------------------------------

-- pragma translate_off

library ieee;
use ieee.std_logic_1164.all;
use std.textio.all;

package components is

  component cy7c1354

    generic (

	fname  : string := "sram.srec"; -- File to read from
			 
        -- Constant parameters
	addr_bits : INTEGER := 18;
	data_bits : INTEGER := 36;

        -- Timing parameters for -5 (225 Mhz)
        tCYC	: TIME    := 4.4 ns;
        tCH	: TIME    :=  1.8 ns;
        tCL	: TIME    :=  1.8 ns;
        tCO	: TIME    :=  2.8 ns;
        tAS	: TIME    :=  1.4 ns;
        tCENS	: TIME    :=  1.4 ns;
        tWES	: TIME    :=  1.4 ns;
        tDS	: TIME    :=  1.4 ns;
        tAH	: TIME    :=  0.4 ns;
        tCENH	: TIME    :=  0.4 ns;
        tWEH	: TIME    :=  0.4 ns;
        tDH	: TIME    :=  0.4 ns


     
        -- Timing parameters for -5 (200 Mhz)
        --tCYC	: TIME    := 5.0 ns;
        --tCH	: TIME    :=  2.0 ns;
        --tCL	: TIME    :=  2.0 ns;
        --tCO	: TIME    :=  3.2 ns;
        --tAS	: TIME    :=  1.5 ns;
        --tCENS	: TIME    :=  1.5 ns;
        --tWES	: TIME    :=  1.5 ns;
        --tDS	: TIME    :=  1.5 ns;
        --tAH	: TIME    :=  0.5 ns;
        --tCENH	: TIME    :=  0.5 ns;
        --tWEH	: TIME    :=  0.5 ns;
        --tDH	: TIME    :=  0.5 ns
         

        -- Timing parameters for -5 (166 Mhz)
        --tCYC	: TIME    := 6.0 ns;
        --tCH	: TIME    :=  2.4 ns;
        --tCL	: TIME    :=  2.4 ns;
        --tCO	: TIME    :=  3.5 ns;
        --tAS	: TIME    :=  1.5 ns;
        --tCENS	: TIME    :=  1.5 ns;
        --tWES	: TIME    :=  1.5 ns;
        --tDS	: TIME    :=  1.5 ns;
        --tAH	: TIME    :=  0.5 ns;
        --tCENH	: TIME    :=  0.5 ns;
        --tWEH	: TIME    :=  0.5 ns;
        --tDH	: TIME    :=  0.5 ns

         );
    -- Port Declarations
    PORT (
	Dq	: INOUT STD_LOGIC_VECTOR ((data_bits - 1) DOWNTO 0);   	-- Data I/O
	Addr	: IN	STD_LOGIC_VECTOR ((addr_bits - 1) DOWNTO 0);   	-- Address
	Mode	: IN	STD_LOGIC	:= '1'; 			-- Burst Mode
	Clk	: IN	STD_LOGIC;                                   -- Clk
	CEN_n	: IN	STD_LOGIC;                                   -- CEN#
	AdvLd_n	: IN	STD_LOGIC;                                   -- Adv/Ld#
	Bwa_n	: IN	STD_LOGIC;                                   -- Bwa#
	Bwb_n	: IN	STD_LOGIC;                                   -- BWb#
	Bwc_n	: IN	STD_LOGIC;                                   -- Bwc#
	Bwd_n	: IN	STD_LOGIC;                                   -- BWd#
	Rw_n	: IN	STD_LOGIC;                                   -- RW#
	Oe_n	: IN	STD_LOGIC;                                   -- OE#
	Ce1_n	: IN	STD_LOGIC;                                   -- CE1#
	Ce2	: IN	STD_LOGIC;                                   -- CE2
	Ce3_n	: IN	STD_LOGIC;                                   -- CE3#
	Zz	: IN	STD_LOGIC                                    -- Snooze Mode
    );

  end component;

end;

-- pragma translate_on
