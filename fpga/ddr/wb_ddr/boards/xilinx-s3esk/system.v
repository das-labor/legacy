//----------------------------------------------------------------------------
// DDR Controller Test Design for Xilinx Spartan 3E 500 Starter Kit
//
// (c) Joerg Bornschein <jb@capsec.org>
//----------------------------------------------------------------------------

`include "ddr_include.v"

module system
#(
	parameter   phase_shift  = 0,
	parameter   clk_multiply = 13,
	parameter   clk_divide   = 5,
	parameter   wait200_init = 26
) (
	input                   clk,
	input                   reset,
	// DDR connection
	output                  ddr_clk,
	output                  ddr_clk_n,
	input                   ddr_clk_fb,
	output                  ddr_ras_n,
	output                  ddr_cas_n,
	output                  ddr_we_n,
	output                  ddr_cke,
	output                  ddr_cs_n,
	output [  `A_RNG]       ddr_a,
	output [ `BA_RNG]       ddr_ba,
	inout  [ `DQ_RNG]       ddr_dq,
	inout  [`DQS_RNG]       ddr_dqs,
	output [ `DM_RNG]       ddr_dm,
	// LAC (LogicAnalyzerComponent)
	input                   uart_rxd,
	output                  uart_txd,
	// diagnosis
	output [7:0]            led,
	input  [2:0]            rot
);

//----------------------------------------------------------------------------
// Memory-Tester 
//----------------------------------------------------------------------------
wire [`WB_ADR_RNG]  wb_adr;
wire [`WB_DAT_RNG]  wb_dat_r;
wire [`WB_DAT_RNG]  wb_dat_w;
wire [`WB_SEL_RNG]  wb_sel;
wire                wb_we;
wire                wb_stb;
wire                wb_cyc;
wire                wb_ack;

wb_memtest test0 (
	.clk(     clk   ),
	.reset(   reset ),
	.led(     led   ),
	// Wishbone master
	.wb_cyc_o(    wb_cyc     ),
	.wb_stb_o(    wb_stb     ),
	.wb_we_o(     wb_we      ),
	.wb_adr_o(    wb_adr     ),
	.wb_dat_i(    wb_dat_r   ),
	.wb_dat_o(    wb_dat_w   ),
	.wb_sel_o(    wb_sel     ),
	.wb_ack_i(    wb_ack     )
);

//----------------------------------------------------------------------------
// DDR Controller
//----------------------------------------------------------------------------
wb_ddr #(
	.phase_shift(  phase_shift  ),
	.clk_multiply( clk_multiply ),
	.clk_divide(   clk_divide   ),
	.wait200_init( wait200_init )
) ddr0 (
	.clk(     clk    ),
	.reset(   reset  ),
	// DDR Ports
	.ddr_clk(      ddr_clk     ),
	.ddr_clk_n(    ddr_clk_n   ),
	.ddr_clk_fb(   ddr_clk_fb  ),
	.ddr_ras_n(    ddr_ras_n   ),
	.ddr_cas_n(    ddr_cas_n   ),
	.ddr_we_n(     ddr_we_n    ),
	.ddr_cke(      ddr_cke     ),
	.ddr_cs_n(     ddr_cs_n    ),
	.ddr_a(        ddr_a       ),
	.ddr_ba(       ddr_ba      ),
	.ddr_dq(       ddr_dq      ),
	.ddr_dqs(      ddr_dqs     ),
	.ddr_dm(       ddr_dm      ),
	// FML (FastMemoryLink)
	.wb_cyc_i(    wb_cyc     ),
	.wb_stb_i(    wb_stb     ),
	.wb_we_i(     wb_we      ),
	.wb_adr_i(    wb_adr     ),
	.wb_dat_o(    wb_dat_r   ),
	.wb_dat_i(    wb_dat_w   ),
	.wb_sel_i(    wb_sel     ),
	.wb_ack_o(    wb_ack     ),
	// phase shifting
	.rot(          rot       )
);

//----------------------------------------------------------------------------
// LAC
//----------------------------------------------------------------------------
wire [7:0] probe;
wire [7:0] probe_sel;

assign probe = { wb_cyc, wb_stb, wb_ack, wb_we, 4'b0 };

lac #(
	.uart_freq_hz( 50000000 ),
	.uart_baud( 115200 ),
	.adr_width( 13 )
) lac0 (
	.reset(      reset    ),
	.uart_clk(   clk      ),
	.uart_rxd(   uart_rxd ),
	.uart_cts(),
	.uart_txd(   uart_txd ),
	.uart_rts(   1'b1     ),
	//
	.probe_clk(  clk      ),
	.probe(      probe    ),
	.select(     probe_sel)
);

endmodule

// vim: set ts=4
