`timescale 1 ns / 100 ps

module pwm_tb;

parameter tcka = 20;
parameter tckb = 20;

reg         reset;
reg         cp2;
reg  [10:0] addr;
reg  [ 7:0] din;
reg         we;
reg         clk;
wire        lsr_clr;
wire        lsr_d;
wire        lsr_c;
wire [ 7:0] latch_data;
wire        psr_c;
wire        psr_d;
wire        col_enable;	

pwm  pwm1 (
    .reset(reset),	  
    .cp2(cp2), 		  
    .addr(addr),		  
    .din(din),		  
    .we(we),		  
    .clk(clk),		  
    .lsr_clr(lsr_clr),	  
    .lsr_d(lsr_d),	  
    .lsr_c(lsr_c),	  
    .latch_data(latch_data), 
    .psr_c(psr_c), 	  
    .psr_d(psr_d),	  
    .col_enable(col_enable) 
);   

initial 
begin
    $dumpvars(-1, pwm1);
    $dumpfile("pwm_tb.vcd");
    clk = 0;
    cp2 = 0;
    reset = 1;
    
    #(4*tcka)
    reset = 0;   
    
    #(500000*tcka) $finish;       
end

always #(tcka/2) clk = ~clk;
always #(tckb/2) cp2 = ~cp2;

endmodule
