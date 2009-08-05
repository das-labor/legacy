//---------------------------------------------------------------------------
// Wishbone General Pupose IO Component
//
//     0x00	
//     0x10     gpio_in    (read-only)
//     0x14     gpio_out   (read/write)
//     0x18     gpio_oe    (read/write)
//
//---------------------------------------------------------------------------

module wb_gpio_8bit (
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
	// IO Wires
	input		[7:0]  gpio_in_0,
	input		[7:0]  gpio_in_1,
	input		[7:0]  gpio_in_2,
	input		[7:0]  gpio_in_3,
	input		[7:0]  gpio_in_4,
	input		[7:0]  gpio_in_5,
	input		[7:0]  gpio_in_6,
	input		[7:0]  gpio_in_7,
	
	output reg  [7:0]  gpio_out_0,
	output reg  [7:0]  gpio_out_1,
	output reg  [7:0]  gpio_out_2,
	output reg  [7:0]  gpio_out_3,
	output reg  [7:0]  gpio_out_4,
	output reg  [7:0]  gpio_out_5,
	output reg  [7:0]  gpio_out_6,
	output reg  [7:0]  gpio_out_7,
	
	output reg  [7:0]  gpio_oe_0,
	output reg  [7:0]  gpio_oe_1,
	output reg  [7:0]  gpio_oe_2,
	output reg  [7:0]  gpio_oe_3,
	output reg  [7:0]  gpio_oe_4,
	output reg  [7:0]  gpio_oe_5,
	output reg  [7:0]  gpio_oe_6,
	output reg  [7:0]  gpio_oe_7
);

//---------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------


// Wishbone
reg  ack;
assign wb_ack_o = wb_stb_i & wb_cyc_i & ack;

wire wb_rd = wb_stb_i & wb_cyc_i & ~wb_we_i;
wire wb_wr = wb_stb_i & wb_cyc_i &  wb_we_i;

always @(posedge clk)
begin
	if (reset) begin
		ack      <= 0;
		gpio_out_0 <= 'b0;
		gpio_out_1 <= 'b0;
		gpio_out_2 <= 'b0;
		gpio_out_3 <= 'b0;
		gpio_out_4 <= 'b0;
		gpio_out_5 <= 'b0;
		gpio_out_6 <= 'b0;
		gpio_out_7 <= 'b0;

		gpio_oe_0 <= 'b0;
		gpio_oe_1 <= 'b0;
		gpio_oe_2 <= 'b0;
		gpio_oe_3 <= 'b0;
		gpio_oe_4 <= 'b0;
		gpio_oe_5 <= 'b0;
		gpio_oe_6 <= 'b0;
		gpio_oe_7 <= 'b0;

	end else begin
		// Handle WISHBONE access
		ack    <= 0;

		if (wb_rd & ~ack) begin           // read cycle
			ack <= 1;
			wb_dat_o[31:8] <= 24'b0;
			
			case (wb_adr_i[7:0])
				'h00: wb_dat_o[7:0] <= gpio_in_0;
				'h04: wb_dat_o[7:0] <= gpio_in_1;
				'h08: wb_dat_o[7:0] <= gpio_in_2;
				'h0C: wb_dat_o[7:0] <= gpio_in_3;
				'h10: wb_dat_o[7:0] <= gpio_in_4;
				'h14: wb_dat_o[7:0] <= gpio_in_5;
				'h18: wb_dat_o[7:0] <= gpio_in_6;
				'h1C: wb_dat_o[7:0] <= gpio_in_7;
				
				'h20: wb_dat_o[7:0] <= gpio_out_0;
				'h24: wb_dat_o[7:0] <= gpio_out_1;
				'h28: wb_dat_o[7:0] <= gpio_out_2;
				'h2C: wb_dat_o[7:0] <= gpio_out_3;
				'h30: wb_dat_o[7:0] <= gpio_out_4;
				'h34: wb_dat_o[7:0] <= gpio_out_5;
				'h38: wb_dat_o[7:0] <= gpio_out_6;
				'h3C: wb_dat_o[7:0] <= gpio_out_7;
				
				'h40: wb_dat_o[7:0] <= gpio_oe_0;
				'h44: wb_dat_o[7:0] <= gpio_oe_1;
				'h48: wb_dat_o[7:0] <= gpio_oe_2;
				'h4C: wb_dat_o[7:0] <= gpio_oe_3;
				'h50: wb_dat_o[7:0] <= gpio_oe_4;
				'h54: wb_dat_o[7:0] <= gpio_oe_5;
				'h58: wb_dat_o[7:0] <= gpio_oe_6;
				'h5C: wb_dat_o[7:0] <= gpio_oe_7;
				
				default: wb_dat_o <= 32'b0;
			endcase
		end else if (wb_wr & ~ack ) begin // write cycle
			ack <= 1;

			case (wb_adr_i[7:0])
				'h20: gpio_out_0 <= wb_dat_i[7:0];
				'h24: gpio_out_1 <= wb_dat_i[7:0];
				'h28: gpio_out_2 <= wb_dat_i[7:0];
				'h2C: gpio_out_3 <= wb_dat_i[7:0];
				'h30: gpio_out_4 <= wb_dat_i[7:0];
				'h34: gpio_out_5 <= wb_dat_i[7:0];
				'h38: gpio_out_6 <= wb_dat_i[7:0];
				'h3C: gpio_out_7 <= wb_dat_i[7:0];
				
				'h40: gpio_oe_0 <= wb_dat_i[7:0];
				'h44: gpio_oe_1 <= wb_dat_i[7:0];
				'h48: gpio_oe_2 <= wb_dat_i[7:0];
				'h4C: gpio_oe_3 <= wb_dat_i[7:0];
				'h50: gpio_oe_4 <= wb_dat_i[7:0];
				'h54: gpio_oe_5 <= wb_dat_i[7:0];
				'h58: gpio_oe_6 <= wb_dat_i[7:0];
				'h5C: gpio_oe_7 <= wb_dat_i[7:0];
			endcase
		end
	end
end


endmodule
