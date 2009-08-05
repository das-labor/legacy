//------------------------------------------------------------------
// Logic Analyzer Component 
//------------------------------------------------------------------

module lac #(
	parameter       uart_freq_hz   =   100000000,
	parameter       uart_baud      =   115200,
	parameter       adr_width      =   11,
	parameter       width          =   8
) (
	input              reset,
	input              uart_clk,
	input              uart_rxd,
	output             uart_cts,
	output             uart_txd,
	input              uart_rts,
	// actual probe input
	input              probe_clk,
	input  [width-1:0] probe,
	output reg   [7:0] select
);

parameter cmd_nop      = 8'h20;
parameter cmd_arm      = 8'h01;
parameter cmd_disarm   = 8'h02;

//------------------------------------------------------------------
// uart instantiation
//------------------------------------------------------------------
assign uart_cts = 1;

reg          tx_wr;
wire         tx_busy;
reg    [7:0] tx_data;

wire   [7:0] rx_data;
wire         rx_avail;
reg          rx_ack;

uart #(
	.freq_hz( uart_freq_hz ),
	.baud(    uart_baud    )
) uart0 (
	.reset(       reset         ),
	.clk(         uart_clk      ),
	// UART
	.uart_txd(    uart_txd      ),
	.uart_rxd(    uart_rxd      ),
	//
	.rx_data(     rx_data       ),
	.rx_avail(    rx_avail      ),
	.rx_error(    rx_error      ),
	.rx_ack(      rx_ack        ),
	.tx_data(     tx_data       ),
	.tx_wr(       tx_wr         ),
	.tx_busy(     tx_busy       )
);

//------------------------------------------------------------------
// handshake signal between clock domains
//------------------------------------------------------------------
reg            [7:0] trig_mask;
reg            [7:0] trig_cond;
reg            [7:0] trig_pre;

reg  [adr_width-1:0] start_adr;   // set in probe_clk domain

reg                  armed;       // set in uart_clk domain
reg                  triggered;   // set in probe_clk domain

//------------------------------------------------------------------
// uart state machine
//------------------------------------------------------------------
wire   rx_req;
assign rx_req = rx_avail & ~rx_ack;

reg                  triggered_synced;
wire     [width-1:0] read_dat;
reg  [adr_width-1:0] read_adr;
wire [adr_width-1:0] read_adr_next;

assign read_adr_next = read_adr + 1;

reg [2:0] state;

parameter s_idle       = 0;
parameter s_read_select= 1;
parameter s_read_mask  = 2;
parameter s_read_comp  = 3;
parameter s_read_pre   = 4;
parameter s_triggered  = 5;
parameter s_send_byte  = 6;

always @(posedge uart_clk)
begin
	if (reset) begin
		state     <= s_idle;
		select    <= 0;
		armed     <= 0;
		tx_wr     <= 0;
	end else begin
		triggered_synced <= triggered;

		rx_ack <= 0;     // default until set otherwise
		tx_wr  <= 0;

		case (state)
		s_idle: begin

			if (rx_req) begin
				case (rx_data)
				cmd_arm: begin
					rx_ack <= 1;
					state  <= s_read_select;
				end
				cmd_disarm: begin
					rx_ack <= 1;
					armed  <= 0;
				end
				default: begin
					rx_ack <= 1;
				end
				endcase
			end

			if (~rx_req && triggered_synced) begin
				state  <= s_triggered;
			end
		end
		s_read_select: begin
			if (rx_req) begin
				rx_ack    <= 1;
				select    <= rx_data;
				state     <= s_read_mask;
			end
		end
		s_read_mask: begin
			if (rx_req) begin
				rx_ack    <= 1;
				trig_mask <= rx_data;
				state     <= s_read_comp;
			end
		end
		s_read_comp: begin
			if (rx_req) begin
				rx_ack    <= 1;
				trig_cond <= rx_data;
				armed     <= 1;
				state     <= s_read_pre;
			end
		end
		s_read_pre: begin
			if (rx_req) begin
				rx_ack    <= 1;
				trig_pre  <= rx_data;
				armed     <= 1;
				state     <= s_idle;
			end
		end
		s_triggered: begin
			armed    <= 0;
			read_adr <= start_adr;
			tx_data  <= adr_width;
			tx_wr    <= 1;
			state    <= s_send_byte;
		end
		s_send_byte: begin
			tx_wr <= 0;

			if (~tx_busy & ~tx_wr) begin
				if (read_adr_next == start_adr)
					state    <= s_idle;

				read_adr <= read_adr_next;
				tx_data  <= read_dat;
				tx_wr    <= 1;
			end
		end
		default:
			state    <= s_idle;
		endcase
	end
end

//------------------------------------------------------------------
// Sampling clock domain
//------------------------------------------------------------------

// register probe input for better F_max
reg        [width-1:0] probe_r;    

always @(posedge probe_clk)
	probe_r <= probe;

// Sampling machinery
reg                    armed_synced;
reg                    armed_synced2;
reg                    sampling;

reg    [adr_width-1:0] write_adr;
wire   [adr_width-1:0] next_adr;
assign                 next_adr = write_adr + 1;

wire    cond_match;
assign  cond_match = (probe_r & trig_mask) == (trig_cond & trig_mask) && armed_synced2;

always @(posedge probe_clk)
begin
	if (reset) begin
		armed_synced  <= 0;
		armed_synced2 <= 0;
		sampling      <= 0;
		triggered     <= 0;
		write_adr     <= 0;
	end else begin
		armed_synced  <= armed;
		armed_synced2 <= armed_synced;
	
		if (armed_synced2 || sampling) begin
			write_adr <= next_adr;
		end

		if (~triggered && armed_synced2) begin
			if (cond_match) begin
				sampling  <= 1;
				triggered <= 1;
				start_adr <= write_adr;
			end
		end
		
		if (sampling && next_adr == start_adr) begin
			sampling <= 0;
		end

		if (~sampling && ~armed_synced2 && triggered) 
			triggered <= 0;
	end
end


//------------------------------------------------------------------
// dual port memory
//------------------------------------------------------------------
wire   write_en;
assign write_en = sampling || cond_match;

dp_ram #(
	.adr_width( adr_width ),
	.dat_width(     width )
) ram0 (
	// read port a 
	.clk_a(     uart_clk    ),
	.adr_a(     read_adr    ),
	.dat_a(     read_dat    ),
	// write port b
	.clk_b(     probe_clk   ),
	.adr_b(     write_adr   ),
	.dat_b(     probe_r     ),
	.we_b(      write_en    )
);

endmodule
