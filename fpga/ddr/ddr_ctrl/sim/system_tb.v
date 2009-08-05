//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
`timescale 1ns / 1ps
`include "ddr_include.v"

module system_tb;

//----------------------------------------------------------------------------
// Parameter (may differ for physical synthesis)
//----------------------------------------------------------------------------
parameter tck              = 10;       // clock period in ns
parameter uart_baud_rate   = 115200;   // uart baud rate for simulation 
parameter ddr_phase_shift  = 100;      //
parameter ddr_wait200_init = 1;        //

parameter clk_freq = 1000000000 / tck; // Frequenzy in HZ

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
reg             clk;
reg             reset;

wire [7:0]      led;
reg  [2:0]      rot;

reg             uart_rxd;
wire            uart_txd;

//----------------------------------------------------------------------------
// DDR connection
//----------------------------------------------------------------------------
wire            ddr_clk;
wire            ddr_clk_n;
wire            ddr_clk_fb;
wire            ddr_ras_n;
wire            ddr_cas_n;
wire            ddr_we_n;
wire            ddr_cke;
wire            ddr_cs_n;
wire [  `A_RNG] ddr_a;
wire [ `BA_RNG] ddr_ba;
wire [ `DQ_RNG] ddr_dq;
wire [`DQS_RNG] ddr_dqs;
wire [ `DM_RNG] ddr_dm;

//----------------------------------------------------------------------------
// Memory-Tester System
//----------------------------------------------------------------------------
system #(
	.clk_freq(         clk_freq         ),
	.ddr_phase_shift(  ddr_phase_shift  ),
	.ddr_wait200_init( ddr_wait200_init )
) dut (
	.clk(          clk   ),
	.reset(        reset ),
	// Status putput
	.led(          led   ),
	.rot(          rot   ),
	// LAC
	.uart_rxd(     uart_rxd    ),
	.uart_txd(     uart_txd    ),
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
	.ddr_dm(       ddr_dm      )
);

//----------------------------------------------------------------------------
// Micron DDR Memory
//----------------------------------------------------------------------------
ddr mt46v16m16 (
	.Dq(     ddr_dq    ),
	.Dqs(    ddr_dqs   ),
	.Addr(   ddr_a     ),
	.Ba(     ddr_ba    ),
	.Clk(    ddr_clk   ),
	.Clk_n(  ddr_clk_n ),
	.Cke(    ddr_cke   ),
	.Cs_n(   ddr_cs_n  ),
	.Ras_n(  ddr_ras_n ),
	.Cas_n(  ddr_cas_n ),
	.We_n(   ddr_we_n  ),
	.Dm(     ddr_dm    )
);

assign ddr_clk_fb = ddr_clk;

//----------------------------------------------------------------------------
// Clock Generation
//----------------------------------------------------------------------------
initial clk <= 1'b0;
always #10 clk <=  ~clk;

//----------------------------------------------------------------------------
//  RESET Generation
//----------------------------------------------------------------------------
initial begin
	$dumpvars;
	$dumpfile("system_tb.vcd");

	#0       reset <= 1'b1; //SPARTAN-3E STARTER KIT
	#80      reset <= 1'b0;

	#150000  $finish;
end

endmodule

// vim: set ts=4
