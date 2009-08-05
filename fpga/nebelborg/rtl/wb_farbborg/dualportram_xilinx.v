
module dualportram_8x16_8x16(clka, clkb, wea, addra, addrb, dia, dob);
    input             clka;
    input             clkb;
    input             wea;
    input      [ 7:0] addra;
    input      [ 7:0] addrb;
    input      [15:0] dia;
    output reg [15:0] dob;
    reg        [15:0] RAM0 [255:0];
        
    always @(posedge clka)
    begin
        if (wea == 1'b1)
            RAM0[addra[7:0]] <= dia;
    end
    
    always @(posedge clkb)
    begin
        dob <= RAM0[addrb];
    end
endmodule

module dualportram_10x8_7x64(clka, clkb, wea, addra, addrb, dia, dob);
    input             clka;
    input             clkb;
    input             wea;
    input      [ 9:0] addra;
    input      [ 6:0] addrb;
    input      [ 7:0] dia;
    output     [63:0] dob;
	
	wire [10:0] xaddra;
	wire [8:0] xaddrb;
	assign xaddra = {2'b00, addra[9:3], addra[1:0]};
	assign xaddrb = {2'b00, addrb };
	
	wire xwea_o, xwea_1;
	assign xwea_0 = wea & (~ addra[2]);
	assign xwea_1 = wea &    addra[2] ;
	
	wire [31:0] dob_0;
	wire [31:0] dob_1;
	
	assign dob = {dob_1, dob_0};
	
	
	// RAMB16_S9_S36: Virtex-II/II-Pro, Spartan-3/3E 2k/512 x 8/32 + 1/4 Parity bits Parity bits Dual-Port RAM
	// Xilinx HDL Language Template, version 9.2.1i
	
	RAMB16_S9_S36 #(
		.INIT_A(9'h000),         // Value of output RAM registers on Port A at startup
		.INIT_B(36'h000000000),  // Value of output RAM registers on Port B at startup
		.SRVAL_A(9'h000),        // Port A output value upon SSR assertion
		.SRVAL_B(36'h000000000), // Port B output value upon SSR assertion
		.WRITE_MODE_A("WRITE_FIRST"), // WRITE_FIRST, READ_FIRST or NO_CHANGE
		.WRITE_MODE_B("WRITE_FIRST"), // WRITE_FIRST, READ_FIRST or NO_CHANGE
		.SIM_COLLISION_CHECK("ALL")  // "NONE", "WARNING_ONLY", "GENERATE_X_ONLY", "ALL" 
	
	) RAMB16_S9_S36_0 (
		//.DOA(DOA),   // Port A 8-bit Data Output
		.DOB(dob_0),   // Port B 32-bit Data Output
		//.DOPA(DOPA), // Port A 1-bit Parity Output
		//.DOPB(DOPB), // Port B 4-bit Parity Output
		.ADDRA(xaddra),  // Port A 11-bit Address Input
		.ADDRB(xaddrb),  // Port B 9-bit Address Input
		.CLKA(clka),    // Port A Clock
		.CLKB(clkb),    // Port B Clock
		.DIA(dia),      // Port A 8-bit Data Input
		.DIB(32'h00000000),// Port B 32-bit Data Input
		.DIPA(1'b0),    // Port A 1-bit parity Input
		.DIPB(4'h0),    // Port-B 4-bit parity Input
		.ENA(1'b1),      // Port A RAM Enable Input
		.ENB(1'b1),      // Port B RAM Enable Input
		.SSRA(1'b0),    // Port A Synchronous Set/Reset Input
		.SSRB(1'b0),    // Port B Synchronous Set/Reset Input
		.WEA(xwea_0),      // Port A Write Enable Input
		.WEB(1'b0)       // Port B Write Enable Input
	);


	RAMB16_S9_S36 #(
		.INIT_A(9'h000),         // Value of output RAM registers on Port A at startup
		.INIT_B(36'h000000000),  // Value of output RAM registers on Port B at startup
		.SRVAL_A(9'h000),        // Port A output value upon SSR assertion
		.SRVAL_B(36'h000000000), // Port B output value upon SSR assertion
		.WRITE_MODE_A("WRITE_FIRST"), // WRITE_FIRST, READ_FIRST or NO_CHANGE
		.WRITE_MODE_B("WRITE_FIRST"), // WRITE_FIRST, READ_FIRST or NO_CHANGE
		.SIM_COLLISION_CHECK("ALL")  // "NONE", "WARNING_ONLY", "GENERATE_X_ONLY", "ALL" 
	
	) RAMB16_S9_S36_1 (
		//.DOA(DOA),   // Port A 8-bit Data Output
		.DOB(dob_1),   // Port B 32-bit Data Output
		//.DOPA(DOPA), // Port A 1-bit Parity Output
		//.DOPB(DOPB), // Port B 4-bit Parity Output
		.ADDRA(xaddra),  // Port A 11-bit Address Input
		.ADDRB(xaddrb),  // Port B 9-bit Address Input
		.CLKA(clka),    // Port A Clock
		.CLKB(clkb),    // Port B Clock
		.DIA(dia),      // Port A 8-bit Data Input
		.DIB(32'h00000000),// Port B 32-bit Data Input
		.DIPA(1'b0),    // Port A 1-bit parity Input
		.DIPB(4'h0),    // Port-B 4-bit parity Input
		.ENA(1'b1),      // Port A RAM Enable Input
		.ENB(1'b1),      // Port B RAM Enable Input
		.SSRA(1'b0),    // Port A Synchronous Set/Reset Input
		.SSRB(1'b0),    // Port B Synchronous Set/Reset Input
		.WEA(xwea_1),      // Port A Write Enable Input
		.WEB(1'b0)       // Port B Write Enable Input
	);

endmodule
