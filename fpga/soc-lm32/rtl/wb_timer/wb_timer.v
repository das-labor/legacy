//---------------------------------------------------------------------------
//
// Wishbone Timer
//
// Register Description:
//
//    0x00 TCR0
//    0x04 COMPARE0
//    0x08 COUNTER0
//    0x0C TCR1
//    0x10 COMPARE1
//    0x14 COUNTER1
//
// TCRx:  
//    +-------------------+-------+-------+-------+-------+
//    |     28'b0         |  EN   |  AR   | IRQEN |  TRIG |
//    +-------------------+-------+-------+-------+-------+
//
//   EN i  (rw)   if set to '1', COUNTERX counts upwards until it reaches
//                COMPAREX
//   AR    (rw)   AutoRecwstartload -- if COUNTER reaches COMPAREX, shall we 
//                restart at 1, or disable this counter?
//   IRQEN (rw)   Indicate interrupt condition when triggered?
//   TRIG  (ro)   
//
//---------------------------------------------------------------------------

module wb_timer #(
	parameter          clk_freq = 50000000
) (
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
	output       [1:0] intr
);

//---------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------

reg irqen0, irqen1;
reg trig0, trig1;
reg en0, en1;
reg ar0, ar1;

wire [31:0] tcr0 = { 28'b0, en0, ar0, irqen0, trig0 };
wire [31:0] tcr1 = { 28'b0, en1, ar1, irqen1, trig1 };

reg  [31:0] counter0;
reg  [31:0] counter1;

reg  [31:0] compare0;
reg  [31:0] compare1;

wire match0 = (counter0 == compare0);
wire match1 = (counter1 == compare1);

assign intr = { trig1, trig0 };

reg  ack;
assign wb_ack_o = wb_stb_i & wb_cyc_i & ack;

wire wb_rd = wb_stb_i & wb_cyc_i & ~wb_we_i;
wire wb_wr = wb_stb_i & wb_cyc_i &  wb_we_i;

always @(posedge clk)
begin
	if (reset) begin
		ack      <= 0;
		en0      <= 0;
		en1      <= 0;
		ar0      <= 0;
		ar1      <= 0;
		trig0    <= 0;
		trig1    <= 0;
		counter0 <= 0;
		counter1 <= 0;
		compare0 <= 32'hFFFFFFFF;
		compare1 <= 32'hFFFFFFFF;
	end else begin

		// Handle counter 0
		if ( en0 & ~match0) counter0 <= counter0 + 1;
		if ( en0 &  match0) trig0    <= 1;
		if ( ar0 &  match0) counter0 <= 1;
		if (~ar0 &  match0) en0      <= 0;

		// Handle counter 1
		if ( en1 & ~match1) counter1 <= counter1 + 1;
		if ( en1 &  match1) trig1    <= 1;
		if ( ar1 &  match1) counter1 <= 1;
		if (~ar1 &  match1) en1      <= 0;

		// Handle WISHBONE access
		ack    <= 0;

		if (wb_rd & ~ack) begin           // read cycle
			ack <= 1;

			case (wb_adr_i[7:0])
			'h00: wb_dat_o <= tcr0;
			'h04: wb_dat_o <= compare0;
			'h08: wb_dat_o <= counter0;
			'h0c: wb_dat_o <= tcr1;
			'h10: wb_dat_o <= compare1;
			'h14: wb_dat_o <= counter1;
			default: wb_dat_o <= 32'b0;
			endcase
		end else if (wb_wr & ~ack ) begin // write cycle
			ack <= 1;

			case (wb_adr_i[7:0])
			'h00: begin
				trig0   <= 0;
				irqen0  <= wb_dat_i[1];
				ar0     <= wb_dat_i[2];
				en0     <= wb_dat_i[3];
			end
			'h04: compare0 <= wb_dat_i;
			'h08: counter0 <= wb_dat_i;
			'h0c: begin
				trig1   <= 0;
				irqen1  <= wb_dat_i[1];
				ar1     <= wb_dat_i[2];
				en1     <= wb_dat_i[3];
			end
			'h10: compare1 <= wb_dat_i;
			'h14: counter1 <= wb_dat_i;
			endcase
		end
	end
end


endmodule
