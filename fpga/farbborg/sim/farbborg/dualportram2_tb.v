`timescale 1 ns / 100 ps

module dualportram2_tb;

parameter tcka = 20;
parameter tckb = 15;

reg         clka;
reg         clkb;
reg         wea;
reg  [10:0] addra;
reg  [ 8:0] addrb;
reg  [ 7:0] dia;
wire [31:0] dob;

dualportram2 dut (
    .clka (clka), 
    .clkb (clkb), 
    .wea (wea), 
    .addra (addra), 
    .addrb (addrb), 
    .dia (dia), 
    .dob (dob)
);

initial 
begin
    $dumpvars(-1, dut);
    $dumpfile("dualportram2_tb.vcd");
    
    clka <= 0;
    clkb <= 0;

    addrb <= 0;
       
    // Port a
    #(0.5*tcka)
    dia   <= 8'h21;
    addra <= 0;
    wea   <= 1;
    
    //#(0.5*tcka)
    //wea   <= 0;
   
    #(1*tcka)
    dia   <= 8'h43;
    addra <= 1;
    wea   <= 1;
    
    //#(0.5*tcka)
    //wea   <= 0;
    
    #(1*tcka)
    dia   <= 8'h65;
    addra <= 2;
    wea   <= 1;
    
    //#(0.5*tcka)
    //wea   <= 0;
    
    #(1*tcka)
    dia   <= 8'h87;
    addra <= 3;
    wea   <= 1;
    
    //#(0.5*tcka)
    //wea   <= 0;

    #(1*tcka)
    dia   <= 8'h21;
    addra <= 4;
    wea   <= 1;
    addrb <= 1;
    
    //#(0.5*tcka)
    //wea   <= 0;
   
    #(1*tcka)
    dia   <= 8'h43;
    addra <= 5;
    wea   <= 1;
    
    #(0.5*tcka)
    wea   <= 0;
    
    #(0.5*tcka)
    dia   <= 8'h65;
    addra <= 6;
    wea   <= 1; 
    
    #(0.5*tcka)
    wea   <= 0;
    
    #(0.5*tcka)
    dia   <= 8'h87;
    addra <= 7;
    wea   <= 1;
    
    #(0.5*tcka)
    wea   <= 0;
    
    #(2*tcka)
    addrb = 10;
       
    #(50*tcka) $finish;      
end

always #(tcka/2) clka = ~clka;
always #(tckb/2) clkb = ~clkb;

endmodule