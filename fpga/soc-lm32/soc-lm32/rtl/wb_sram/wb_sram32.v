//----------------------------------------------------------------------------
// Wishbone SRAM controller
//----------------------------------------------------------------------------
module wb_sram32 #(
	parameter                  adr_width = 18,    
	parameter                  latency   = 0    // 0 .. 7
) (
	input                      clk,
	input                      reset,
	// Wishbone interface
	input                      wb_stb_i,
	input                      wb_cyc_i,
	output reg                 wb_ack_o,
	input                      wb_we_i,
	input               [31:0] wb_adr_i,
	input                [3:0] wb_sel_i,
	input               [31:0] wb_dat_i,
	output reg          [31:0] wb_dat_o,
	// SRAM connection
	output reg [adr_width-1:0] sram_adr,
	inout               [31:0] sram_dat,
    output reg           [3:0] sram_be_n,    // Byte   Enable
	output reg                 sram_ce_n,    // Chip   Enable
	output reg                 sram_oe_n,    // Output Enable
	output reg                 sram_we_n     // Write  Enable
);

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------

// Wishbone handling
wire wb_rd = wb_stb_i & wb_cyc_i & ~wb_we_i & ~wb_ack_o;
wire wb_wr = wb_stb_i & wb_cyc_i &  wb_we_i & ~wb_ack_o;

// Translate wishbone address to sram address
wire [adr_width-1:0] adr = wb_adr_i[adr_width+1:2];

// Tri-State-Driver
reg [31:0] wdat;
reg        wdat_oe;

assign sram_dat = wdat_oe ? wdat : 32'bz;


// Latency countdown
reg  [2:0] lcount;

//----------------------------------------------------------------------------
// State Machine
//----------------------------------------------------------------------------
parameter s_idle   = 0;
parameter s_read   = 1;
parameter s_write  = 2;

reg [2:0] state;

always @(posedge clk)
begin
	if (reset) begin
		state    <= s_idle;
		lcount   <= 0;
		wb_ack_o <= 0;
	end else begin
		case (state)
		s_idle: begin
			wb_ack_o <= 0;

			if (wb_rd) begin
				sram_ce_n  <=  0;
				sram_oe_n  <=  0;
				sram_we_n  <=  1;
				sram_adr   <=  adr;
				sram_be_n  <=  4'b0000;
				wdat_oe    <=  0;
				lcount     <=  latency;
				state      <=  s_read;
			end else if (wb_wr) begin
				sram_ce_n  <=  0;
				sram_oe_n  <=  1;
				sram_we_n  <=  0;
				sram_adr   <=  adr;
				sram_be_n  <= ~wb_sel_i;
				wdat       <=  wb_dat_i;
				wdat_oe    <=  1;
				lcount     <=  latency;
				state      <=  s_write;
			end else begin
				sram_ce_n  <=  1;
				sram_oe_n  <=  1;
				sram_we_n  <=  1;
				wdat_oe    <=  0;
			end
		end
		s_read: begin
			if (lcount != 0) begin
				lcount     <= lcount - 1;
			end else begin
				sram_ce_n  <=  1;
				sram_oe_n  <=  1;
				sram_we_n  <=  1;
				wb_dat_o   <=  sram_dat;
				wb_ack_o   <=  1;
				state      <=  s_idle;
			end
		end
		s_write: begin
			if (lcount != 0) begin
				lcount     <= lcount - 1;
			end else begin
				sram_ce_n  <=  1;
				sram_oe_n  <=  1;
				sram_we_n  <=  1;
				wb_ack_o   <=  1;       // XXX   We could acknoledge write  XXX
				state      <=  s_idle;  // XXX   requests 1 cycle ahead     XXX
			end
		end
		endcase
	end
end

endmodule
