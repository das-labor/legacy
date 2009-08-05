//----------------------------------------------------------------------------
// Wishbone DDR Controller
// 
// (c) Joerg Bornschein (<jb@capsec.org>)
//----------------------------------------------------------------------------
`include "ddr_include.v"

module wb_memtest 
(
	input                         clk,
	input                         reset,
	output  [7:0]                 led,
	// Wishbone Master
	output reg [`WB_ADR_RNG]      wb_adr_o,
	output reg [`WB_DAT_RNG]      wb_dat_o,
	input      [`WB_DAT_RNG]      wb_dat_i,
	output reg [`WB_SEL_RNG]      wb_sel_o,
	output reg                    wb_cyc_o,
	output reg                    wb_stb_o,
	output reg                    wb_we_o,
	input                         wb_ack_i
);

//----------------------------------------------------------------------------
// Wishbone Memory Test 
//----------------------------------------------------------------------------
reg [7:0] r_counter; 
reg [7:0] w_counter; 

wire [`WB_ADR_RNG] r_adr;
wire [`WB_ADR_RNG] w_adr;

assign r_adr = { 9'b0, r_counter[7], 10'b0, r_counter[6:4], 4'b0, r_counter[3:0], 2'b0 };
assign w_adr = { 9'b0, w_counter[7], 10'b0, w_counter[6:4], 4'b0, w_counter[3:0], 2'b0 };

wire [31:0] c_data = 'h76543210;
wire [31:0] r_data = r_adr;
wire [31:0] w_data = w_adr;


reg  [31:0] comp_data;

reg    wait_for_ack;
assign stall = (wait_for_ack & ~wb_ack_i);

reg    reading;

always @(posedge clk)
begin
	if (reset) begin
		wait_for_ack <= 0;
		wb_sel_o     <= 'hf;
		wb_cyc_o     <= 0;
		wb_we_o      <= 0;
		r_counter    <= 1;
		w_counter    <= 1;
		reading      <= 0;
	end else begin
		if (~stall) begin
			if (reading) begin            // READ
				if (r_counter == 0) begin
					reading      <= 0;
					r_counter    <= 1;
				end else begin
					wb_cyc_o     <= 1;
					wb_stb_o     <= 1;
					wb_we_o      <= 0;
					wb_adr_o     <= r_adr;
					comp_data    <= r_data;
					r_counter    <= r_counter + 1;
					wait_for_ack <= 1;
				end
			end else begin               // WRITE
				if (w_counter == 0) begin
					reading      <= 1;
					w_counter    <= 1;
				end else begin
					wb_cyc_o     <= 1;
					wb_stb_o     <= 1;
					wb_we_o      <= 1;
					wb_adr_o     <= w_adr;
					wb_dat_o     <= w_data;
					w_counter    <= w_counter + 1;
					wait_for_ack <= 1;
				end
			end
		end
	end
end

reg data_ok;
reg data_ok_lt;
reg data_error;
reg data_error_lt;

always @(posedge clk)
begin
	if (reset) begin
		data_ok         <= 0;
		data_ok_lt      <= 0;   // long term
		data_error      <= 0;
		data_error_lt   <= 0;   // long term
	end else begin
		if (~wb_we_o & wb_ack_i) begin
			if (wb_dat_i == comp_data) begin
				data_ok       <= 1;
				data_ok_lt    <= 1;
				data_error    <= 0;
			end else begin
				data_error    <= 1;
				data_error_lt <= 1;
				data_ok       <= 0;
			end
		end
	end
end

//----------------------------------------------------------------------------
// LED - Status output
//----------------------------------------------------------------------------
assign led[7] = wb_cyc_o & ~wb_we_o;
assign led[6] = wb_cyc_o &  wb_we_o;
assign led[5] = wb_ack_i;
assign led[4] = 0;
assign led[3] = data_ok;
assign led[2] = data_ok_lt;
assign led[1] = data_error;
assign led[0] = data_error_lt;

endmodule

