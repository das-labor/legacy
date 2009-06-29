--------------------------------------------------------------------------------
--     This file is owned and controlled by Xilinx and must be used           --
--     solely for design, simulation, implementation and creation of          --
--     design files limited to Xilinx devices or technologies. Use            --
--     with non-Xilinx devices or technologies is expressly prohibited        --
--     and immediately terminates your license.                               --
--                                                                            --
--     XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"          --
--     SOLELY FOR USE IN DEVELOPING PROGRAMS AND SOLUTIONS FOR                --
--     XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION        --
--     AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE, APPLICATION            --
--     OR STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS              --
--     IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,                --
--     AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE       --
--     FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY               --
--     WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE                --
--     IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR         --
--     REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF        --
--     INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS        --
--     FOR A PARTICULAR PURPOSE.                                              --
--                                                                            --
--     Xilinx products are not intended for use in life support               --
--     appliances, devices, or systems. Use in such applications are          --
--     expressly prohibited.                                                  --
--                                                                            --
--     (c) Copyright 1995-2007 Xilinx, Inc.                                   --
--     All rights reserved.                                                   --
--------------------------------------------------------------------------------
-- You must compile the wrapper file encoder_b8b10b.vhd when simulating
-- the core, encoder_b8b10b. When compiling the wrapper file, be sure to
-- reference the XilinxCoreLib VHDL simulation library. For detailed
-- instructions, please refer to the "CORE Generator Help".

-- The synthesis directives "translate_off/translate_on" specified
-- below are supported by Xilinx, Mentor Graphics and Synplicity
-- synthesis tools. Ensure they are correct for your synthesis tool(s).

LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
-- synthesis translate_off
Library XilinxCoreLib;
-- synthesis translate_on
ENTITY encoder_b8b10b IS
	port (
	din: IN std_logic_VECTOR(7 downto 0);
	kin: IN std_logic;
	clk: IN std_logic;
	dout: OUT std_logic_VECTOR(9 downto 0);
	ce: IN std_logic);
END encoder_b8b10b;

ARCHITECTURE encoder_b8b10b_a OF encoder_b8b10b IS
-- synthesis translate_off
component wrapped_encoder_b8b10b
	port (
	din: IN std_logic_VECTOR(7 downto 0);
	kin: IN std_logic;
	clk: IN std_logic;
	dout: OUT std_logic_VECTOR(9 downto 0);
	ce: IN std_logic);
end component;

-- Configuration specification 
	for all : wrapped_encoder_b8b10b use entity XilinxCoreLib.encode_8b10b_v5_0(behavioral)
		generic map(
			c_has_nd_b => 0,
			c_force_code_disp => 0,
			c_encode_type => 0,
			c_force_code_disp_b => 0,
			c_has_disp_out => 0,
			c_has_kerr => 0,
			c_has_disp_out_b => 0,
			c_has_nd => 0,
			c_enable_rlocs => 0,
			c_has_ce => 1,
			c_has_bports => 0,
			c_has_kerr_b => 0,
			c_force_code_val_b => "1010101010",
			c_has_disp_in_b => 0,
			c_force_code_val => "1010101010",
			c_has_force_code_b => 0,
			c_has_disp_in => 0,
			c_has_ce_b => 0,
			c_has_force_code => 0);
-- synthesis translate_on
BEGIN
-- synthesis translate_off
U0 : wrapped_encoder_b8b10b
		port map (
			din => din,
			kin => kin,
			clk => clk,
			dout => dout,
			ce => ce);
-- synthesis translate_on

END encoder_b8b10b_a;

