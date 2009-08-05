//------------------------------------------------------------------
// Dual port memory (one read and one write port, same width)
//------------------------------------------------------------------

module dp_ram #(
	parameter adr_width = 11,
	parameter dat_width = 8 
) (
	// read port a 
	input                       clk_a,
	input      [adr_width-1:0]  adr_a,
	output reg [dat_width-1:0]  dat_a,
	// write port b
	input                       clk_b,
	input      [adr_width-1:0]  adr_b,
	input      [dat_width-1:0]  dat_b,
	input                       we_b
);

parameter depth = (1 << adr_width);

// actual ram cells
reg [dat_width-1:0] ram [0:depth-1];

//------------------------------------------------------------------
// read port
//------------------------------------------------------------------
always @(posedge clk_a)
begin
	dat_a <= ram[adr_a];
end

//------------------------------------------------------------------
// write port
//------------------------------------------------------------------
always @(posedge clk_b)
begin
	if (we_b) begin
		ram[adr_b] <= dat_b;
	end
end

endmodule
