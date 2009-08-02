
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
    reg        [7:0] RAM0 [127:0];
    reg        [7:0] RAM1 [127:0];
    reg        [7:0] RAM2 [127:0];
    reg        [7:0] RAM3 [127:0];
    reg        [7:0] RAM4 [127:0];
    reg        [7:0] RAM5 [127:0];
    reg        [7:0] RAM6 [127:0];
    reg        [7:0] RAM7 [127:0];
    
    
    
    always @(posedge clka)
    begin
        if (wea == 1'b1) begin
	        case (addra[2:0])
	                3'b000 : RAM0[addra[9:3]] <= dia;
	                3'b001 : RAM1[addra[9:3]] <= dia;
	                3'b010 : RAM2[addra[9:3]] <= dia;
	                3'b011 : RAM3[addra[9:3]] <= dia;
					3'b100 : RAM4[addra[9:3]] <= dia;
	                3'b101 : RAM5[addra[9:3]] <= dia;
	                3'b110 : RAM6[addra[9:3]] <= dia;
	                3'b111 : RAM7[addra[9:3]] <= dia;
	        endcase
        end
    end
    
	reg [7:0] byte7;
	reg [7:0] byte6;
	reg [7:0] byte5;
	reg [7:0] byte4;
	reg [7:0] byte3;
	reg [7:0] byte2;
	reg [7:0] byte1;
	reg [7:0] byte0;
	
    always @(posedge clkb)
    begin
		byte7 <= RAM7[addrb];
		byte6 <= RAM6[addrb];
		byte5 <= RAM5[addrb];
		byte4 <= RAM4[addrb];
		byte3 <= RAM3[addrb];
		byte2 <= RAM2[addrb];
		byte1 <= RAM1[addrb];
		byte0 <= RAM0[addrb];
		
	end
	
	assign dob = {byte7, byte6, byte5, byte4, byte3, byte2, byte1, byte0 };
	
endmodule
