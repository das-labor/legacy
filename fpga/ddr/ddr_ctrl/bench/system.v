//----------------------------------------------------------------------------
// DDR Controller Test Design
// 
// (c) Joerg Bornschein (<jb@capsec.org>)
//----------------------------------------------------------------------------
`timescale 1ns / 1ps
`include "ddr_include.v"

module system
#(
	parameter clk_freq         = 50000000,
	parameter ddr_clk_multiply = 12,
	parameter ddr_clk_divide   = 5,
	parameter ddr_phase_shift  = 20,
	parameter ddr_wait200_init = 26
)
(
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
wire                fml_rd;
wire                fml_wr;
wire                fml_done;
wire [`FML_ADR_RNG] fml_adr;
wire [`FML_DAT_RNG] fml_wdata;
wire [`FML_MSK_RNG] fml_msk;
wire [`FML_DAT_RNG] fml_rdata;

fml_memtest test0 (
	.clk(          clk         ),
	.reset(        reset       ),
	.led(          led         ),
	// FML
	.fml_wr(       fml_wr      ),
	.fml_rd(       fml_rd      ),
	.fml_done(     fml_done    ),
	.fml_adr(      fml_adr     ),
	.fml_wdata(    fml_wdata   ),
	.fml_msk(      fml_msk     ),
	.fml_rdata(    fml_rdata   )
);

//----------------------------------------------------------------------------
// DDR Controller
//----------------------------------------------------------------------------
ddr_ctrl #(
	.clk_freq(     clk_freq         ),
	.clk_multiply( ddr_clk_multiply ),
	.clk_divide(   ddr_clk_divide   ),
	.phase_shift(  ddr_phase_shift  ),
	.wait200_init( ddr_wait200_init )
) ctrl0 (
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
	.fml_wr(       fml_wr      ),
	.fml_rd(       fml_rd      ),
	.fml_done(     fml_done    ),
	.fml_adr(      fml_adr     ),
	.fml_din(      fml_wdata   ),
	.fml_msk(      fml_msk     ),
	.fml_dout(     fml_rdata   ),
	// phase shifting
	.rot(          rot         )
);

//----------------------------------------------------------------------------
// DDR Controller
//----------------------------------------------------------------------------
wire [7:0] probe;
wire [7:0] probe_sel;

assign probe = { fml_rd, fml_wr, fml_done, 5'b0 };


lac #(
	.uart_freq_hz( clk_freq ),
	.uart_baud(    115200   ),
	.adr_width(    13       )
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
