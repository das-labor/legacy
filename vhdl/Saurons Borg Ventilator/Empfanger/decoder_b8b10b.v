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

// You must compile the wrapper file decoder_b8b10b.v when simulating
// the core, decoder_b8b10b. When compiling the wrapper file, be sure to
// reference the XilinxCoreLib Verilog simulation library. For detailed
// instructions, please refer to the "CORE Generator Help".

`timescale 1ns/1ps

module decoder_b8b10b(
	clk,
	din,
	dout,
	kout,
	ce,
	code_err);


input clk;
input [9 : 0] din;
output [7 : 0] dout;
output kout;
input ce;
output code_err;

// synthesis translate_off

      DECODE_8B10B_V7_1 #(
		.c_decode_type(0),
		.c_enable_rlocs(0),
		.c_has_bports(0),
		.c_has_ce(1),
		.c_has_ce_b(0),
		.c_has_code_err(1),
		.c_has_code_err_b(0),
		.c_has_disp_err(0),
		.c_has_disp_err_b(0),
		.c_has_disp_in(0),
		.c_has_disp_in_b(0),
		.c_has_nd(0),
		.c_has_nd_b(0),
		.c_has_run_disp(0),
		.c_has_run_disp_b(0),
		.c_has_sinit(0),
		.c_has_sinit_b(0),
		.c_has_sym_disp(0),
		.c_has_sym_disp_b(0),
		.c_sinit_dout("00000000"),
		.c_sinit_dout_b("00000000"),
		.c_sinit_kout(0),
		.c_sinit_kout_b(0),
		.c_sinit_run_disp(1),
		.c_sinit_run_disp_b(1))
	inst (
		.CLK(clk),
		.DIN(din),
		.DOUT(dout),
		.KOUT(kout),
		.CE(ce),
		.CODE_ERR(code_err),
		.CE_B(),
		.CLK_B(),
		.DIN_B(),
		.DISP_IN(),
		.DISP_IN_B(),
		.SINIT(),
		.SINIT_B(),
		.CODE_ERR_B(),
		.DISP_ERR(),
		.DISP_ERR_B(),
		.DOUT_B(),
		.KOUT_B(),
		.ND(),
		.ND_B(),
		.RUN_DISP(),
		.RUN_DISP_B(),
		.SYM_DISP(),
		.SYM_DISP_B());


// synthesis translate_on

// XST black box declaration
// box_type "black_box"
// synthesis attribute box_type of decoder_b8b10b is "black_box"

endmodule

