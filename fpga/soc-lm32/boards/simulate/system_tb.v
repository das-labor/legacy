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

parameter clk_freq = 1000000000 / tck; // Frequenzy in HZ
//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
reg        clk;
reg        reset;
reg  [7:0] btn;
wire [7:0] led;

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

//------------------------------------------------------------------
// Decive Under Test 
//------------------------------------------------------------------
system #(
	.clk_freq(           clk_freq         ),
	.uart_baud_rate(     uart_baud_rate   )
) dut  (
	.clk(       clk   ),
	.reset(     reset ),
	// Debug
	.led(       led   ),
	.btn(       btn   ),
	// UART
	.uart_rxd(  uart_rxd  ),
	.uart_txd(  uart_txd  )
);

assign sw  = { 4'b0001 };

/* Clocking device */
initial         clk <= 0;
always #(tck/2) clk <= ~clk;

/* Simulation setup */
initial begin
	$dumpfile("system_tb.vcd");
	$dumpvars(-1, dut);

	// reset
	#0  reset <= 1;
	#80 reset <= 0;

/*
	// send select value
	uart_send( 'h00 );
	uart_wait_tx;
	#(tck*5000)
*/

	#(tck*1000000) $finish;
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

/*
always @(posedge clk)
begin
	if (dut.lm32i_ack) begin
		$display( "LM32I transaction: ADR=%x WE=%b DAT=%x", 
		            dut.lm32i_adr, dut.lm32i_we, 
		            (dut.lm32i_we) ? dut.lm32i_dat_w : dut.lm32i_dat_r );
	end
end
*/

endmodule
