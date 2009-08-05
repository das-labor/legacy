//----------------------------------------------------------------------------
// Pipelined, asyncronous DDR Controller
//
// (c) Joerg Bornschein (<jb@capsec.org>)
//----------------------------------------------------------------------------
module ddr_ctrl 
#(
	parameter clk_freq     = 50000000,
	parameter clk_multiply = 12,
	parameter clk_divide   = 5,
	parameter phase_shift  = 0,
	parameter wait200_init = 26
) (
	input                   clk, 
	input                   reset,
	//  DDR ports
	output                  ddr_clk,
	output                  ddr_clk_n,
	input                   ddr_clk_fb,
	output                  ddr_ras_n,
	output                  ddr_cas_n,
	output                  ddr_we_n,
	output                  ddr_cke,
	output                  ddr_cs_n,
	output [  `A_RNG]       ddr_a,
	output [ `BA_RNG]       ddr_ba,
	inout  [ `DQ_RNG]       ddr_dq,
	inout  [`DQS_RNG]       ddr_dqs,
	output [ `DM_RNG]       ddr_dm,
	// FML (FastMemoryLink)
	input                   fml_wr,
	input                   fml_rd,
	output reg              fml_done,
	input  [`FML_ADR_RNG]   fml_adr,
	input  [`FML_DAT_RNG]   fml_din,
	input  [`FML_MSK_RNG]   fml_msk,
	output reg [`FML_DAT_RNG]   fml_dout,
	// Temporary DCM control input
	input  [2:0]            rot      // XXX
);

wire [ `DQ_RNG]       ddr_dq_i,  ddr_dq_o;
wire [`DQS_RNG]       ddr_dqs_i, ddr_dqs_o;
wire                  ddr_dqs_oe;

//----------------------------------------------------------------------------
// clock generator
//----------------------------------------------------------------------------
wire clk_locked;
wire write_clk, write_clk90;
wire read_clk;

wire reset_int = reset | ~clk_locked;

ddr_clkgen #(
	.phase_shift(  phase_shift  ),
	.clk_multiply( clk_multiply ),
	.clk_divide(   clk_divide   )
) clkgen (
	.clk(             clk            ),
	.reset(           reset          ),
	.locked(          clk_locked     ),
	// ddr-clk 
	.read_clk(        read_clk       ),
	.write_clk(       write_clk      ),
	.write_clk90(     write_clk90    ),
	// phase shift control
	.rot(             rot            )      // XXX
);

//----------------------------------------------------------------------------
// async_fifos (cmd, wdata, rdata)
//----------------------------------------------------------------------------
wire                  cba_fifo_full;
reg  [`CBA_RNG]       cba_fifo_din;
reg                   cba_fifo_we;

wire                  wfifo_full;
reg   [`WFIFO_RNG]    wfifo_din;
reg                   wfifo_we;


wire [`RFIFO_RNG]     rfifo_dout;
wire                  rfifo_empty;
wire                  rfifo_next;

//----------------------------------------------------------------------------
// High-speed cmd, write and read datapath
//----------------------------------------------------------------------------
ddr_wpath wpath0 (
	.clk(         write_clk    ),
	.clk90(       write_clk90  ),
	.reset(       reset_int    ),
	// CBA async fifo
	.cba_clk(     clk           ),
	.cba_din(     cba_fifo_din  ),
	.cba_wr(      cba_fifo_we   ),
	.cba_full(    cba_fifo_full ),
	// WDATA async fifo
	.wdata_clk(   clk           ),
	.wdata_din(   wfifo_din     ),
	.wdata_wr(    wfifo_we      ),
	.wdata_full(  wfifo_full    ),
	//
	.sample(     sample      ), 
	// DDR
	.ddr_clk(     ddr_clk    ),
	.ddr_clk_n(   ddr_clk_n  ),
	.ddr_ras_n(   ddr_ras_n  ),
	.ddr_cas_n(   ddr_cas_n  ),
	.ddr_we_n(    ddr_we_n   ),
	.ddr_a(       ddr_a      ),
	.ddr_ba(      ddr_ba     ),
	.ddr_dm(      ddr_dm     ),
	.ddr_dq(      ddr_dq_o   ),
	.ddr_dqs(     ddr_dqs_o  ),
	.ddr_dqs_oe(  ddr_dqs_oe )
);

ddr_rpath rpath0 (
	.clk(         read_clk   ),
	.reset(       reset_int  ),
	// 
	.sample(      sample     ),
	//
	.rfifo_clk(   clk        ),
	.rfifo_empty( rfifo_empty),
	.rfifo_dout(  rfifo_dout ),
	.rfifo_next(  rfifo_next ),
	// DDR
	.ddr_dq(      ddr_dq_i   ),
	.ddr_dqs(     ddr_dqs_i  )
);

//----------------------------------------------------------------------------
// 7.8 us pulse generator
//----------------------------------------------------------------------------
wire pulse78;
reg  ar_req;
reg  ar_done;

ddr_pulse78 #(
	.clk_freq( clk_freq )
) pulse78_gen (
	.clk(      clk        ),
	.reset(    reset_int  ),
	.pulse78(  pulse78    )
);

//----------------------------------------------------------------------------
// Auto Refresh request generator
//----------------------------------------------------------------------------
always @(posedge clk)
	if (reset_int)
		ar_req <= 0;
	else
		ar_req <= pulse78 | (ar_req & ~ar_done);

// operations we might want to submit
wire [`CBA_RNG] ar_pre_cba;
wire [`CBA_RNG] ar_ar_cba;

assign ar_pre_cba   = { `DDR_CMD_PRE, 2'b00, 13'b1111111111111 };
assign ar_ar_cba    = { `DDR_CMD_AR,  2'b00, 13'b0000000000000 };

//----------------------------------------------------------------------------
// Init & management
//----------------------------------------------------------------------------
wire                 init_req;
reg                  init_ack;
wire [`CBA_RNG]      init_cba;
wire                 init_done;
wire                 wait200;

ddr_init #(
	.wait200_init( wait200_init )
) init (
	.clk(         clk         ),
	.reset(       reset_int   ),
	.pulse78(     pulse78     ),
	.wait200(     wait200     ),
	.init_done(   init_done   ),
	//
	.mngt_req(    init_req    ),
	.mngt_ack(    init_ack    ),
	.mngt_cba(    init_cba    )
);

//----------------------------------------------------------------------------
// Active Bank Information 
//----------------------------------------------------------------------------
reg [`ROW_RNG] ba_row [3:0];
reg [3:0]      ba_active;

//----------------------------------------------------------------------------
// Handle write FIFO
//----------------------------------------------------------------------------
wire wfifo_proc_reset;
wire wfifo_proc_start;
reg  wfifo_proc_done;

parameter w_idle   = 0;
parameter w_1      = 1;
parameter w_2      = 2;
parameter w_3      = 3;
parameter w_finish = 4;

reg [2:0] wstate;

always @(posedge clk)
begin
	if (reset_int) begin
		wstate          <= w_idle;
		wfifo_proc_done <= 0;
	end else begin
		wfifo_we <= 0;

		case (wstate)
			w_idle: begin
				if (wfifo_proc_start & ~wfifo_full) begin
					wfifo_din[`WFIFO_D0_RNG] <= fml_din[15: 0];
					wfifo_din[`WFIFO_D1_RNG] <= fml_din[31:16];
					wfifo_din[`WFIFO_M0_RNG] <= fml_msk[ 1: 0];
					wfifo_din[`WFIFO_M1_RNG] <= fml_msk[ 3: 2];
					wfifo_we                 <= 1;
					wstate                   <= w_1;
				end
			end
			w_1: begin
				if (~wfifo_full) begin
					wfifo_din[`WFIFO_D0_RNG] <= fml_din[47:32];
					wfifo_din[`WFIFO_D1_RNG] <= fml_din[63:48];
					wfifo_din[`WFIFO_M0_RNG] <= fml_msk[ 5: 4];
					wfifo_din[`WFIFO_M1_RNG] <= fml_msk[ 7: 6];
					wfifo_we                 <= 1;
					wstate                   <= w_2;
				end
			end
			w_2: begin
				if (~wfifo_full) begin
					wfifo_din[`WFIFO_D0_RNG] <= fml_din[79:64];
					wfifo_din[`WFIFO_D1_RNG] <= fml_din[95:80];
					wfifo_din[`WFIFO_M0_RNG] <= fml_msk[ 9: 8];
					wfifo_din[`WFIFO_M1_RNG] <= fml_msk[11:10];
					wfifo_we                 <= 1;
					wstate                   <= w_3;
				end
			end
			w_3: begin
				if (~wfifo_full) begin
					wfifo_din[`WFIFO_D0_RNG] <= fml_din[111: 96];
					wfifo_din[`WFIFO_D1_RNG] <= fml_din[127:112];
					wfifo_din[`WFIFO_M0_RNG] <= fml_msk[12:12];
					wfifo_din[`WFIFO_M1_RNG] <= fml_msk[15:14];
					wfifo_we                 <= 1;
					wstate                   <= w_finish;
					wfifo_proc_done          <= 1;
				end
			end
			w_finish: begin
				if (wfifo_proc_reset) begin
					wstate          <= w_idle;
					wfifo_proc_done <= 0;
				end
			end
		endcase
	end
end

//----------------------------------------------------------------------------
// Handle read FIFO
//----------------------------------------------------------------------------
wire rfifo_proc_reset;
wire rfifo_proc_start;
reg  rfifo_proc_done ;

parameter r_idle   = 0;
parameter r_1      = 1;
parameter r_2      = 2;
parameter r_3      = 3;
parameter r_finish = 4;

reg [2:0] rstate;

assign rfifo_next = ~rfifo_empty;

always @(posedge clk)
begin
	if (reset_int) begin
		rstate          <= r_idle;
		rfifo_proc_done <= 0;
	end else begin
		case (rstate)
			r_idle: begin
				if (rfifo_proc_start & ~rfifo_empty) begin
					fml_dout[31:0]   <= rfifo_dout;
					rstate           <= r_1;
				end
			end
			r_1: begin
				if (~rfifo_empty) begin
					fml_dout[63:32]  <= rfifo_dout;
					rstate           <= r_2;
				end
			end
			r_2: begin
				if (~rfifo_empty) begin
					fml_dout[95:64]  <= rfifo_dout;
					rstate           <= r_3;
				end
			end
			r_3: begin
				if (~rfifo_empty) begin
					fml_dout[127:96] <= rfifo_dout;
					rstate           <= r_finish;
					rfifo_proc_done  <= 1;
				end
			end
			r_finish: begin
				if (rfifo_proc_reset) begin
					rfifo_proc_done  <= 0;
					rstate           <= r_idle;
				end
			end
		endcase
	end
end

//----------------------------------------------------------------------------
// FML decoding
//----------------------------------------------------------------------------
wire [`FML_ADR_BA_RNG]    fml_ba  = fml_adr[`FML_ADR_BA_RNG];
wire [`FML_ADR_ROW_RNG]   fml_row = fml_adr[`FML_ADR_ROW_RNG];
wire [`FML_ADR_COL_RNG]   fml_col = fml_adr[`FML_ADR_COL_RNG];

wire [`FML_ADR_ROW_RNG]   fml_cur_row;   // current active row in sel. bank
assign fml_cur_row    = ba_row[fml_ba];

wire   fml_row_active;  // is row in selected ba really active?
assign fml_row_active = ba_active[fml_ba]; 


/*
wire   fml_row_active = (fml_ba == 0) ? ba0_active :     // is row in selected
                        (fml_ba == 1) ? ba1_active :     // bank really active?
                        (fml_ba == 2) ? ba2_active :
                                        ba3_active ;
*/

// request operation iff correct bank is active
wire fml_req       = fml_rd | fml_wr;
wire fml_row_match = (fml_row == fml_cur_row) & fml_row_active;
wire fml_pre_req   = fml_req & ~fml_row_match & fml_row_active;
wire fml_act_req   = fml_req & ~fml_row_active;
wire fml_read_req  = fml_rd  &  fml_row_match & ~fml_done;
wire fml_write_req = fml_wr  &  fml_row_match & ~fml_done;

// actual operations we might want to submit
wire [`CBA_RNG] fml_pre_cba;
wire [`CBA_RNG] fml_act_cba;
wire [`CBA_RNG] fml_read_cba;
wire [`CBA_RNG] fml_write_cba;

assign fml_pre_cba   = { `DDR_CMD_PRE,   fml_ba, 13'b0  };
assign fml_act_cba   = { `DDR_CMD_ACT,   fml_ba, fml_row };
assign fml_read_cba  = { `DDR_CMD_READ,  fml_ba, {3'b000}, fml_col, {3'b000} };
assign fml_write_cba = { `DDR_CMD_WRITE, fml_ba, {3'b000}, fml_col, {3'b000} };

//----------------------------------------------------------------------------
// Schedule and issue commands
//----------------------------------------------------------------------------

parameter s_init      = 0;
parameter s_idle      = 1;
parameter s_ar        = 2;
parameter s_reading   = 3;

reg [1:0] state;

always @(posedge clk)
begin
	if (reset_int) begin
		state        <= s_init;
		ba_active    <= 0;
		ba_row[0]    <= 0;
		ba_row[1]    <= 0;
		ba_row[2]    <= 0;
		ba_row[3]    <= 0;

		fml_done     <= 0;
		init_ack     <= 0;
		cba_fifo_we  <= 0;
		ar_done      <= 0;
	end else begin
		fml_done     <= 0;
		init_ack     <= 0;
		cba_fifo_we  <= 0;
		ar_done      <= 0;

		case (state)
			s_init: begin
				if (init_done)
					state <= s_idle;

				if (init_req & ~cba_fifo_full) begin
					cba_fifo_we       <= 1;
					cba_fifo_din      <= init_cba;
					init_ack          <= 1;
				end
			end
			s_idle: begin
				if (fml_read_req & ~cba_fifo_full) begin
					cba_fifo_we       <= 1;
					cba_fifo_din      <= fml_read_cba;
					state             <= s_reading;
				end else if (fml_write_req & ~cba_fifo_full & wfifo_proc_done) begin
					cba_fifo_we       <= 1;
					cba_fifo_din      <= fml_write_cba;
					fml_done          <= 1;
				end else if (ar_req & ~cba_fifo_full) begin
					cba_fifo_we       <= 1;
					cba_fifo_din      <= ar_pre_cba;
					ar_done           <= 1;
					ba_active         <= 'b0;
					state             <= s_ar;
				end else if (fml_pre_req & ~cba_fifo_full) begin
					cba_fifo_we       <= 1;
					cba_fifo_din      <= fml_pre_cba;
					ba_active[fml_ba] <= 0;
				end else if (fml_act_req & ~cba_fifo_full) begin
					cba_fifo_we       <= 1;
					cba_fifo_din      <= fml_act_cba;
					ba_active[fml_ba] <= 1;
					ba_row[fml_ba]    <= fml_row;
				end
			end
			s_ar: begin
				if (~cba_fifo_full) begin
					cba_fifo_we       <= 1;
					cba_fifo_din      <= ar_ar_cba;
					state             <= s_idle;
				end
			end
			s_reading: begin
				if (rfifo_proc_done) begin
					fml_done          <= 1;
					state             <= s_idle;
				end
			end
		endcase
	end
end

// start and stop rfifo engine
assign rfifo_proc_start = fml_read_req;
assign rfifo_proc_reset = fml_done;

// start and stop wfifo engine
assign wfifo_proc_start = fml_wr;
assign wfifo_proc_reset = fml_done;

//----------------------------------------------------------------------------
// Demux dqs and dq
//----------------------------------------------------------------------------
assign ddr_cke   = ~wait200;     // bring up CKE as soon 200us wait is finished

assign ddr_dqs = ddr_dqs_oe!=1'b0 ? ddr_dqs_o : 'bz;
assign ddr_dq  = ddr_dqs_oe!=1'b0 ? ddr_dq_o  : 'bz;

assign ddr_dqs_i = ddr_dqs;
assign ddr_dq_i  = ddr_dq;

assign ddr_cs_n = 0;

endmodule

// vim: set ts=4
