//-----------------------------------------------------------------
// Wishbone BlockRAM
//-----------------------------------------------------------------

module wb_bram #(
	parameter mem_file_name = "none",
	parameter adr_width = 11
) (
	input             clk_i, 
	input             rst_i,
	//
	input             wb_stb_i,
	input             wb_cyc_i,
	input             wb_we_i,
	output            wb_ack_o,
	input      [31:0] wb_adr_i,
	output reg [31:0] wb_dat_o,
	input      [31:0] wb_dat_i,
	input      [ 3:0] wb_sel_i
);

//-----------------------------------------------------------------
// Storage depth in 32 bit words
//-----------------------------------------------------------------
parameter word_width = adr_width - 2;
parameter word_depth = (1 << word_width);

//-----------------------------------------------------------------
// 
//-----------------------------------------------------------------
reg            [31:0] ram [0:word_depth-1];    // actual RAM
reg                   ack;
wire [word_width-1:0] adr;


assign adr        = wb_adr_i[adr_width-1:2];      // 
assign wb_ack_o   = wb_stb_i & ack;

always @(posedge clk_i)
begin
	if (wb_stb_i && wb_cyc_i) 
	begin
		if (wb_we_i) 
			ram[ adr ] <= wb_dat_i;

		wb_dat_o <= ram[ adr ];
		ack <= ~ack;
	end else
		ack <= 0;
    
end

initial 
begin
	if (mem_file_name != "none")
	begin
		$readmemh(mem_file_name, ram);
	end
end

endmodule

