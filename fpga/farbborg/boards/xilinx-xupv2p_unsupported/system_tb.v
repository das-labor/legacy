//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
`timescale 1 ns / 100 ps
`include "ddr_include.v"

module system_tb;

//----------------------------------------------------------------------------
// Parameter (may differ for physical synthesis)
//----------------------------------------------------------------------------
parameter tck              = 10;       // clock period in ns
parameter uart_baud_rate   = 1152000;  // uart baud rate for simulation 
parameter ddr_phase_shift  = 100;      //
parameter ddr_wait200_init = 26;       //

parameter clk_freq = 1000000000 / tck; // Frequenzy in HZ
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
reg        clk;
reg        reset;
reg  [4:0] btn_n;
wire [3:0] led_n;
wire [3:0] sw;

//------------------------------------------------------------------
// UART STUFF (testbench uart, simulating a comm. partner)
//------------------------------------------------------------------
wire         uart_rxd;
wire         uart_txd;

reg          tx_wr;
wire         tx_busy;
reg    [7:0] tx_data;

wire   [7:0] rx_data;
wire         rx_avail;
reg          rx_ack;

uart #(
	.freq_hz(  clk_freq       ),
	.baud(     uart_baud_rate )
) tb_uart (
	.reset(    reset     ),
	.clk(      clk       ),
	//
	.uart_txd( uart_rxd  ),
	.uart_rxd( uart_txd  ),
	//
	.rx_data(  rx_data   ),
	.rx_avail( rx_avail  ),
	.rx_error( rx_error  ),
	.rx_ack(   rx_ack    ),
	.tx_data(  tx_data   ),
	.tx_wr(    tx_wr     ),
	.tx_busy(  tx_busy   )
);

//------------------------------------------------------------------
// uart helper function
//------------------------------------------------------------------
always @(posedge clk)
begin
	if (rx_avail && ~rx_ack) begin
		$display( "Reading from UART: rx_data=%h", rx_data );
		rx_ack <= 1;
	end else 
		rx_ack <= 0;
end

task uart_send;
	input [7:0] byte;
	begin
		while (tx_busy) begin
			@(posedge clk);
		end

		@(negedge clk);
		$display( "Writing to UART: tx_data=%h", byte );

		tx_data = byte;
		tx_wr   = 1;
		#(tck)
		tx_wr   = 0;
	end
endtask

task uart_wait_tx;
	begin
		while (tx_busy) begin
			@(posedge clk);
		end

		$display( "Writing to UART: done." );
	end
endtask

//----------------------------------------------------------------------------
// DDR connection
//----------------------------------------------------------------------------
wire      [2:0] ddr_clk;
wire      [2:0] ddr_clk_n;
wire            ddr_clk_fb;
wire            ddr_ras_n;
wire            ddr_cas_n;
wire            ddr_we_n;
wire      [1:0] ddr_cke;
wire      [1:0] ddr_cs_n;
wire [  `A_RNG] ddr_a;
wire [ `BA_RNG] ddr_ba;
wire [ `DQ_RNG] ddr_dq;
wire [`DQS_RNG] ddr_dqs;
wire [ `DM_RNG] ddr_dm;

//----------------------------------------------------------------------------
// Micron DDR Memory
//----------------------------------------------------------------------------
ddr mt46v16m16 (
	.Dq(     ddr_dq       ),
	.Dqs(    ddr_dqs      ),
	.Addr(   ddr_a        ),
	.Ba(     ddr_ba       ),
	.Clk(    ddr_clk[0]   ),
	.Clk_n(  ddr_clk_n[0] ),
	.Cke(    ddr_cke[0]   ),
	.Cs_n(   ddr_cs_n[0]  ),
	.Ras_n(  ddr_ras_n    ),
	.Cas_n(  ddr_cas_n    ),
	.We_n(   ddr_we_n     ),
	.Dm(     ddr_dm       )
);

assign ddr_clk_fb = ddr_clk;

//------------------------------------------------------------------
// Decive Under Test 
//------------------------------------------------------------------
system #(
	.clk_freq(           clk_freq         ),
	.uart_baud_rate(     uart_baud_rate   ),
	.ddr_phase_shift(    ddr_phase_shift  ),
	.ddr_wait200_init(   ddr_wait200_init )
) dut  (
	.clk(       clk       ),
	.reset_n(  ~reset     ),
	// Debug
	.led_n(     led_n     ),
	.btn_n(     btn_n     ),
	// UART
	.uart_rxd(  uart_rxd  ),
	.uart_txd(  uart_txd  ),
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

assign sw  = { 4'b0001 };

/* Clocking device */
initial         clk <= 0;
always #(tck/2) clk <= ~clk;

/* Simulation setup */
initial begin
	$dumpvars(-1, dut);
	$dumpfile("system_tb.vcd");

	// reset
	#0  reset <= 1;
	#80 reset <= 0;

/*
	// send select value
	uart_send( 'h00 );
	uart_wait_tx;
	#(tck*5000)

	// send trigger mask
	uart_send( 'hFF );
	uart_wait_tx;
	#(tck*5000)

	// send trigger comop
	uart_send( 'h80 );
	uart_wait_tx;
	#(tck*5000)

	// send pretrigger value
	uart_send( 'h00 );
	uart_wait_tx;
	#(tck*5000)
*/

	#(tck*50000) $finish;
end

//------------------------------------------------------------------
// Monitor Wishbone transactions
//------------------------------------------------------------------
always @(posedge clk)
begin
	if (dut.lm32d_ack) begin
		$display( "LM32D transaction: ADR=%x WE=%b DAT=%x", 
		            dut.lm32d_adr, dut.lm32d_we, 
		            (dut.lm32d_we) ? dut.lm32d_dat_w : dut.lm32d_dat_r );
	end
end

always @(posedge clk)
begin
	if (dut.lm32i_ack) begin
		$display( "LM32I transaction: ADR=%x WE=%b DAT=%x", 
		            dut.lm32i_adr, dut.lm32i_we, 
		            (dut.lm32i_we) ? dut.lm32i_dat_w : dut.lm32i_dat_r );
	end
end

endmodule
