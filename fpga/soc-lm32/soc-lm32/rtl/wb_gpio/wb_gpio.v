//---------------------------------------------------------------------------
// Wishbone General Pupose IO Component
//
//     0x00	
//     0x10     gpio_in    (read-only)
//     0x14     gpio_out   (read/write)
//     0x18     gpio_oe    (read/write)
//
//---------------------------------------------------------------------------

module wb_gpio (
	input              clk,
	input              reset,
	// Wishbone interface
	input              wb_stb_i,
	input              wb_cyc_i,
	output             wb_ack_o,
	input              wb_we_i,
	input       [31:0] wb_adr_i,
	input        [3:0] wb_sel_i,
	input       [31:0] wb_dat_i,
	output reg  [31:0] wb_dat_o,
	//
	output             intr,
	// IO Wires
	input       [31:0] gpio_in,
	output reg  [31:0] gpio_out,
	output reg  [31:0] gpio_oe
);

//---------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------

wire [31:0] gpiocr = 32'b0;

// Wishbone
reg  ack;
assign wb_ack_o = wb_stb_i & wb_cyc_i & ack;

wire wb_rd = wb_stb_i & wb_cyc_i & ~wb_we_i;
wire wb_wr = wb_stb_i & wb_cyc_i &  wb_we_i;

always @(posedge clk)
begin
	if (reset) begin
		ack      <= 0;
		gpio_out <= 'b0;
	end else begin
		// Handle WISHBONE access
		ack    <= 0;

		if (wb_rd & ~ack) begin           // read cycle
			ack <= 1;

			case (wb_adr_i[7:0])
			'h00: wb_dat_o <= gpiocr;
			'h10: wb_dat_o <= gpio_in;
			'h14: wb_dat_o <= gpio_out;
			'h18: wb_dat_o <= gpio_oe;
			default: wb_dat_o <= 32'b0;
			endcase
		end else if (wb_wr & ~ack ) begin // write cycle
			ack <= 1;

			case (wb_adr_i[7:0])
			'h00: begin
			end
			'h14: gpio_out <= wb_dat_i;
			'h18: gpio_oe  <= wb_dat_i;
			endcase
		end
	end
end


endmodule
