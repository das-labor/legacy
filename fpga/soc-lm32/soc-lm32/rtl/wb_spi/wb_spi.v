//-----------------------------------------------------------------
// SPI Master
//-----------------------------------------------------------------

module wb_spi(
	input               clk;
	input               reset;
	// Wishbone bus
	input      [31:0]   wb_adr_i;
	input      [31:0]   wb_dat_i;
	output reg [31:0]   wb_dat_o;
	input      [ 3:0]   wb_sel_i;
	input               wb_cyc_i;
	input               wb_stb_i;
	output              wb_ack_o;
	input               wb_we_i;
	// SPI 
	output              spi_sck;
	output              spi_mosi;
	input               spi_miso;
	output reg   [7:0]  spi_cs;
);


	reg  ack;
	assign wb_ack_o = wb_stb_i & wb_cyc_i & ack;

	wire wb_rd = wb_stb_i & wb_cyc_i & ~ack & ~wb_we_i;
	wire wb_wr = wb_stb_i & wb_cyc_i & ~ack & wb_we_i;

	
	reg [2:0] bitcount;
	reg ilatch;
	reg run;

	reg sck;

	//prescaler registers for sclk
	reg [7:0] prescaler;
	reg [7:0] divisor;

	//data shift register
	reg [7:0] sreg;

	assign spi_sck = sck;
	assign spi_mosi = sreg[7];

	always @(posedge clk) begin
		if (reset == 1'b1) begin
			ack      <= 0;
			sck <= 1'b0;
			bitcount <= 3'b000;
			run <= 1'b0;
			prescaler <= 8'h00;
			divisor <= 8'hff;
		end else begin
			prescaler <= prescaler + 1;
			if (prescaler == divisor) begin
				prescaler <= 8'h00;
				if (run == 1'b1) begin
					sck <= ~sck;
					if(sck == 1'b1) begin
						bitcount <= bitcount + 1;
						if(bitcount == 3'b111) begin
							run <= 1'b0;
						end
						
						sreg [7:0] <= {sreg[6:0], ilatch};
					end else begin
						ilatch <= spi_miso;
					end
				end
			end

			ack <= wb_stb_i & wb_cyc_i;
			
			if (wb_rd) begin           // read cycle
				case (wb_adr_i[5:2])
					4'b0000: wb_dat_o <= sreg;
					4'b0001: wb_dat_o <= {7'b0000000 , run};
				endcase
			end
			
			
			if (wb_wr) begin // write cycle
				case (wb_adr_i[5:2])
					4'b0000: begin
							sreg    <=  wb_dat_i[7:0];
							run     <=  1'b1;
						end
					4'b0010:
							spi_cs  <=  wb_dat_i[7:0];
					4'b0100: 
							divisor <=  wb_dat_i[7:0];
				endcase
			end
		end
	end


endmodule
