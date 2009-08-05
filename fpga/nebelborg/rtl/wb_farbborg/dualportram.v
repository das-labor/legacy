
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
    reg        [31:0] RAM1 [127:0];
    reg        [31:0] RAM0 [127:0];


    always @(posedge clka)
    begin
        if (wea == 1'b1) 
        begin
			case(addra[1:0])
				2'b00 : begin
					case (addra[2])
						1'b0 : RAM0[addra[9:3]] [7:0] <= dia;
						1'b1 : RAM1[addra[9:3]] [7:0] <= dia;
					endcase
				end
				2'b01 : begin
					case (addra[2])
						1'b0 : RAM0[addra[9:3]] [15:8] <= dia;
						1'b1 : RAM1[addra[9:3]] [15:8] <= dia;
					endcase
				end
				2'b10 : begin
					case (addra[2])
						1'b0 : RAM0[addra[9:3]] [23:16] <= dia;
						1'b1 : RAM1[addra[9:3]] [23:16] <= dia;
					endcase
				end
				2'b11 : begin
					case (addra[2])
						1'b0 : RAM0[addra[9:3]] [31:24] <= dia;
						1'b1 : RAM1[addra[9:3]] [31:24] <= dia;
					endcase
				end
        	endcase
		end
    end
    
	reg [31:0] byte1;
	reg [31:0] byte0;
	
	
    always @(posedge clkb)
    begin
		byte1 <= RAM1[addrb];
		byte0 <= RAM0[addrb];
	end
	
	assign dob = {byte1, byte0};
	
endmodule
