//----------------------------------------------------------------------------
// FML (FastMemoryLink) Memory Test Component
// 
// (c) Joerg Bornschein (<jb@capsec.org>)
//----------------------------------------------------------------------------
`include "ddr_include.v"

module fml_memtest 
(
	input                        clk,
	input                        reset,
	output  [7:0]                led,
	// 
	output reg                   fml_rd,
	output reg                   fml_wr,
	input                        fml_done,
	output reg  [`FML_ADR_RNG]   fml_adr,
	output reg  [`FML_DAT_RNG]   fml_wdata,
	output reg  [`FML_MSK_RNG]   fml_msk,
	input       [`FML_DAT_RNG]   fml_rdata
);


//----------------------------------------------------------------------------
// StartUp wait
//----------------------------------------------------------------------------
reg [10:0]  su_counter;
wire       su_wait;
assign     su_wait = ~(su_counter == 600);

always @(posedge clk)
begin
	if (reset)
		su_counter <= 0;
	else if (su_wait)
		su_counter <= su_counter + 1;
end

//----------------------------------------------------------------------------
// FML Memory Test 
//----------------------------------------------------------------------------
reg [30:0] r_instr;
reg [30:0] w_instr;

reg [4:0] r_counter; 
reg [4:0] w_counter; 

wire [`FML_ADR_RNG] r_adr;
wire [`FML_ADR_RNG] w_adr;

assign r_adr = { 1'b0, r_counter[4], 11'b0, r_counter[3:2], 5'b0, r_counter[1:0] };
assign w_adr = { 1'b0, w_counter[4], 11'b0, w_counter[3:2], 5'b0, w_counter[1:0] };

wire [127:0] c_data = 'hFFEEDDCCBBAA99887766554433221100;
wire [127:0] r_data = c_data ^ { r_adr, r_adr, r_adr, r_adr, r_adr, 18'b0 };
wire [127:0] w_data = c_data ^ { w_adr, w_adr, w_adr, w_adr, w_adr, 18'b0 };

reg  [127:0] comp_data;

reg    wait_done;
assign stall = (wait_done & ~fml_done);

always @(posedge clk)
begin
	if (reset) begin
//		r_instr     <= 'b001010101100110011110000;
//		w_instr     <= 'b000101010011001100001111;
		r_instr     <= 'b0101010101011001100110011110000;
		w_instr     <= 'b0010101010100110011001100001111;
		fml_msk     <= 'h0000;
		wait_done   <= 0;
		fml_rd      <= 0;
		fml_wr      <= 0;
		r_counter   <= 1;
		w_counter   <= 1;
	end else begin
		if (~stall & ~su_wait) begin
			if (r_instr[0]) begin            // READ
				fml_rd    <= 1;
				fml_wr    <= 0;
				fml_adr   <= r_adr;
				comp_data <= r_data;
				r_counter <= r_counter + 1;
				wait_done <= 1;
			end else if (w_instr[0]) begin   // WRITE
				fml_rd    <= 0;
				fml_wr    <= 1;
				fml_wdata <= w_data;
				fml_adr   <= w_adr;
				w_counter <= w_counter + 1;
				wait_done <= 1;
			end else begin                   // IDLE CYCLE
				fml_rd    <= 0;
				fml_wr    <= 0;
				fml_adr   <= 'b0;
				wait_done <= 0;
			end

			r_instr <= { r_instr[0], r_instr[23:1] };
			w_instr <= { w_instr[0], w_instr[23:1] };
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
		data_ok_lt      <= 0;
		data_error      <= 0;
		data_error_lt   <= 0;
	end else begin
		if (fml_rd & fml_done) begin
			if (fml_rdata == comp_data) begin
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

assign led[7] = fml_rd;
assign led[6] = fml_wr;
assign led[5] = fml_done;
assign led[4] = 0;
assign led[3] = data_ok;
assign led[2] = data_ok_lt;
assign led[1] = data_error;
assign led[0] = data_error_lt;

endmodule

