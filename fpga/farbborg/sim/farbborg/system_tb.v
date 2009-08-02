`timescale 1 ns / 100 ps

module system_tb;

parameter tck = 20;

reg        clk;
reg  [3:0] btn;
wire [7:0] led;
wire       uart_rxd;
wire       uart_txd;

system dut  (
	.clk(  clk  ),
	// Debug
	.led(  led  ),
	.btn(  btn  ),
	// Uart
	.uart_rxd(  uart_rxd  ),
	.uart_txd(  uart_txd  )
);


/* Clocking device */
always #(tck/2) clk = ~clk;

/* Simulation setup */
initial begin
	$dumpvars(-1, dut);
	$dumpfile("system_tb.vcd");

        // reset
	btn[0] = 1;
	clk    = 0;
        
	#(tck*3)
	btn[0] = 0;
	$display("RESET DONE");

	#(tck*50000) $finish;
end

endmodule
