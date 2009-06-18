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

// You must compile the wrapper file fifo.v when simulating
// the core, fifo. When compiling the wrapper file, be sure to
// reference the XilinxCoreLib Verilog simulation library. For detailed
// instructions, please refer to the "CORE Generator Help".

`timescale 1ns/1ps

module fifo(
	clk,
	din,
	rd_en,
	wr_en,
	data_count,
	dout,
	empty,
	full,
	prog_empty,
	prog_full);


input clk;
input [33 : 0] din;
input rd_en;
input wr_en;
output [5 : 0] data_count;
output [33 : 0] dout;
output empty;
output full;
output prog_empty;
output prog_full;

// synthesis translate_off

      FIFO_GENERATOR_V4_2 #(
		.C_COMMON_CLOCK(1),
		.C_COUNT_TYPE(0),
		.C_DATA_COUNT_WIDTH(6),
		.C_DEFAULT_VALUE("BlankString"),
		.C_DIN_WIDTH(34),
		.C_DOUT_RST_VAL("0"),
		.C_DOUT_WIDTH(34),
		.C_ENABLE_RLOCS(0),
		.C_FAMILY("spartan3"),
		.C_FULL_FLAGS_RST_VAL(1),
		.C_HAS_ALMOST_EMPTY(0),
		.C_HAS_ALMOST_FULL(0),
		.C_HAS_BACKUP(0),
		.C_HAS_DATA_COUNT(1),
		.C_HAS_INT_CLK(0),
		.C_HAS_MEMINIT_FILE(0),
		.C_HAS_OVERFLOW(0),
		.C_HAS_RD_DATA_COUNT(0),
		.C_HAS_RD_RST(0),
		.C_HAS_RST(0),
		.C_HAS_SRST(0),
		.C_HAS_UNDERFLOW(0),
		.C_HAS_VALID(0),
		.C_HAS_WR_ACK(0),
		.C_HAS_WR_DATA_COUNT(0),
		.C_HAS_WR_RST(0),
		.C_IMPLEMENTATION_TYPE(0),
		.C_INIT_WR_PNTR_VAL(0),
		.C_MEMORY_TYPE(1),
		.C_MIF_FILE_NAME("BlankString"),
		.C_OPTIMIZATION_MODE(0),
		.C_OVERFLOW_LOW(0),
		.C_PRELOAD_LATENCY(1),
		.C_PRELOAD_REGS(0),
		.C_PRIM_FIFO_TYPE("512x36"),
		.C_PROG_EMPTY_THRESH_ASSERT_VAL(15),
		.C_PROG_EMPTY_THRESH_NEGATE_VAL(16),
		.C_PROG_EMPTY_TYPE(1),
		.C_PROG_FULL_THRESH_ASSERT_VAL(25),
		.C_PROG_FULL_THRESH_NEGATE_VAL(24),
		.C_PROG_FULL_TYPE(1),
		.C_RD_DATA_COUNT_WIDTH(6),
		.C_RD_DEPTH(64),
		.C_RD_FREQ(1),
		.C_RD_PNTR_WIDTH(6),
		.C_UNDERFLOW_LOW(0),
		.C_USE_DOUT_RST(0),
		.C_USE_ECC(0),
		.C_USE_EMBEDDED_REG(0),
		.C_USE_FIFO16_FLAGS(0),
		.C_USE_FWFT_DATA_COUNT(0),
		.C_VALID_LOW(0),
		.C_WR_ACK_LOW(0),
		.C_WR_DATA_COUNT_WIDTH(6),
		.C_WR_DEPTH(64),
		.C_WR_FREQ(1),
		.C_WR_PNTR_WIDTH(6),
		.C_WR_RESPONSE_LATENCY(1))
	inst (
		.CLK(clk),
		.DIN(din),
		.RD_EN(rd_en),
		.WR_EN(wr_en),
		.DATA_COUNT(data_count),
		.DOUT(dout),
		.EMPTY(empty),
		.FULL(full),
		.PROG_EMPTY(prog_empty),
		.PROG_FULL(prog_full),
		.INT_CLK(),
		.BACKUP(),
		.BACKUP_MARKER(),
		.PROG_EMPTY_THRESH(),
		.PROG_EMPTY_THRESH_ASSERT(),
		.PROG_EMPTY_THRESH_NEGATE(),
		.PROG_FULL_THRESH(),
		.PROG_FULL_THRESH_ASSERT(),
		.PROG_FULL_THRESH_NEGATE(),
		.RD_CLK(),
		.RD_RST(),
		.RST(),
		.SRST(),
		.WR_CLK(),
		.WR_RST(),
		.ALMOST_EMPTY(),
		.ALMOST_FULL(),
		.OVERFLOW(),
		.VALID(),
		.RD_DATA_COUNT(),
		.UNDERFLOW(),
		.WR_ACK(),
		.WR_DATA_COUNT(),
		.SBITERR(),
		.DBITERR());


// synthesis translate_on

// XST black box declaration
// box_type "black_box"
// synthesis attribute box_type of fifo is "black_box"

endmodule

