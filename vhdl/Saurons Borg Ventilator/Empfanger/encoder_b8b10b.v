/*******************************************************************************
*     This file is owned and controlled by Xilinx and must be used             *
*     solely for design, simulation, implementation and creation of            *
*     design files limited to Xilinx devices or technologies. Use              *
*     with non-Xilinx devices or technologies is expressly prohibited          *
*     and immediately terminates your license.                                 *
*                                                                              *
*     XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"            *
*     SOLELY FOR USE IN DEVELOPING PROGRAMS AND SOLUTIONS FOR                  *
*     XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION          *
*     AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE, APPLICATION              *
*     OR STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS                *
*     IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,                  *
*     AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE         *
*     FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY                 *
*     WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE                  *
*     IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR           *
*     REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF          *
*     INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          *
*     FOR A PARTICULAR PURPOSE.                                                *
*                                                                              *
*     Xilinx products are not intended for use in life support                 *
*     appliances, devices, or systems. Use in such applications are            *
*     expressly prohibited.                                                    *
*                                                                              *
*     (c) Copyright 1995-2007 Xilinx, Inc.                                     *
*     All rights reserved.                                                     *
*******************************************************************************/
// The synthesis directives "translate_off/translate_on" specified below are
// supported by Xilinx, Mentor Graphics and Synplicity synthesis
// tools. Ensure they are correct for your synthesis tool(s).

// You must compile the wrapper file encoder_b8b10b.v when simulating
// the core, encoder_b8b10b. When compiling the wrapper file, be sure to
// reference the XilinxCoreLib Verilog simulation library. For detailed
// instructions, please refer to the "CORE Generator Help".

`timescale 1ns/1ps

module encoder_b8b10b(
	din,
	kin,
	clk,
	dout,
	ce);


input [7 : 0] din;
input kin;
input clk;
output [9 : 0] dout;
input ce;

// synthesis translate_off

      ENCODE_8B10B_V5_0 #(
		.c_enable_rlocs(0),
		.c_encode_type(0),
		.c_force_code_disp(0),
		.c_force_code_disp_b(0),
		.c_force_code_val("1010101010"),
		.c_force_code_val_b("1010101010"),
		.c_has_bports(0),
		.c_has_ce(1),
		.c_has_ce_b(0),
		.c_has_disp_in(0),
		.c_has_disp_in_b(0),
		.c_has_disp_out(0),
		.c_has_disp_out_b(0),
		.c_has_force_code(0),
		.c_has_force_code_b(0),
		.c_has_kerr(0),
		.c_has_kerr_b(0),
		.c_has_nd(0),
		.c_has_nd_b(0))
	inst (
		.DIN(din),
		.KIN(kin),
		.CLK(clk),
		.DOUT(dout),
		.CE(ce),
		.CE_B(),
		.DIN_B(),
		.KIN_B(),
		.CLK_B(),
		.DOUT_B(),
		.FORCE_CODE(),
		.FORCE_CODE_B(),
		.FORCE_DISP(),
		.FORCE_DISP_B(),
		.DISP_IN(),
		.DISP_IN_B(),
		.DISP_OUT(),
		.DISP_OUT_B(),
		.ND(),
		.ND_B(),
		.KERR(),
		.KERR_B());


// synthesis translate_on

// XST black box declaration
// box_type "black_box"
// synthesis attribute box_type of encoder_b8b10b is "black_box"

endmodule

