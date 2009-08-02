`timescale 1 ns / 100 ps

module wb_farbborg_tb;

parameter    tcka = 20;
parameter    tckb = 20;

reg          clk;
reg          reset;

// Wishbone bus
reg   [12:0] wb_adr_i;
reg   [ 7:0] wb_dat_i;
wire  [ 7:0] wb_dat_o;
reg   [ 3:0] wb_sel_i;
reg          wb_cyc_i;
reg          wb_stb_i;
wire         wb_ack_o;
reg          wb_we_i;

// farbborg interface
reg          clk_pwm;

// Latch shift-register
wire         lsr_clr;
wire         lsr_d;
wire         lsr_c;
wire  [ 7:0] latch_data;

// Plane shift-register	
wire         psr_c;
wire         psr_d;
wire         col_enable;

// May be debug stuff
wire  [ 4:0] plane_out;

wb_farbborg wb_farbborg1 (
    .clk(clk),	  
    .reset(reset), 		  
    .wb_dat_i(wb_dat_i),
    .wb_dat_o(wb_dat_o),		  
    .wb_adr_i(wb_adr_i),		  
    .wb_sel_i(wb_sel_i),		  
    .wb_cyc_i(wb_cyc_i),	  
    .wb_stb_i(wb_stb_i),	  
    .wb_ack_o(wb_ack_o),	  
    .wb_we_i(wb_we_i), 
    .clk_pwm(clk_pwm),
    .lsr_clr(lsr_clr),
    .lsr_d(lsr_d),
    .lsr_c(lsr_c),
    .latch_data(latch_data),
    .psr_c(psr_c),
    .psr_d(psr_d),
    .col_enable(col_enable),
    .plane_out(plane_out)
);   

initial 
begin
    $dumpvars(-1, wb_farbborg1);
    $dumpfile("wb_farbborg_tb.vcd");
    clk       = 0;
    clk_pwm   = 0;
    reset     = 1;
    
    #(2*tcka)
    reset     = 0;   
    wb_we_i   = 1;
    
    #(2*tcka)
    wb_dat_i  = 8'h33;
    wb_adr_i  = 12'h004;
    
    wb_stb_i  = 1;
    wb_cyc_i  = 1;
    #(1*tcka)
    
    wb_stb_i  = 0;
    wb_cyc_i  = 0;
    
    
    #(50000*tcka) $finish;       
end

always #(tcka/2) clk = ~clk;
always #(tckb/2) clk_pwm = ~clk_pwm;

endmodule
