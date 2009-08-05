//---------------------------------------------------------------------------
// LatticeMico32 System On A Chip
//
// Top Level Design for the Xilinx XUP Virtex II Pro Board
//---------------------------------------------------------------------------
`include "ddr_include.v"

module system
#(
//	parameter   bootram_file     = "../../firmware/ddr-phaser/image.ram",
	parameter   bootram_file     = "../../firmware/boot0-serial/image.ram",
	parameter   clk_freq         = 100000000,
	parameter   uart_baud_rate   = 115200,
	parameter   ddr_clk_multiply = 13,
	parameter   ddr_clk_divide   = 10,
	parameter   ddr_phase_shift  = 180,
	parameter   ddr_wait200_init = 52
) (
	input                   clk, 
	input                   reset_n,
	// DDR connection
	output            [2:0] ddr_clk,
	output            [2:0] ddr_clk_n,
	input                   ddr_clk_fb,
	output                  ddr_ras_n,
	output                  ddr_cas_n,
	output                  ddr_we_n,
	output            [1:0] ddr_cke,
	output            [1:0] ddr_cs_n,
	output       [  `A_RNG] ddr_a,
	output       [ `BA_RNG] ddr_ba,
	inout        [ `DQ_RNG] ddr_dq,
	inout        [`DQS_RNG] ddr_dqs,
	output       [ `DM_RNG] ddr_dm,
	// Debug 
	output            [3:0] led_n,
	input             [4:0] btn_n,
	input             [3:0] sw,
	// Uart
	input                   uart_rxd, 
	output                  uart_cts,
	output                  uart_txd,
	input                   uart_rts
);
	
assign uart_cts = 1;
wire   rst      = ~reset_n;

wire         probe_clk;
wire   [7:0] probe_sel;
wire   [7:0] probe;

//------------------------------------------------------------------
// Wishbone Wires
//------------------------------------------------------------------
wire         gnd   = 1'b0;
wire   [3:0] gnd4  = 4'h0;
wire  [31:0] gnd32 = 32'h00000000;
 
wire [31:0]  lm32i_adr,
             lm32d_adr,
             uart0_adr,
             timer0_adr,
             gpio0_adr,
             bram0_adr,
             ddr0_adr;


wire [31:0]  lm32i_dat_r,
             lm32i_dat_w,
             lm32d_dat_r,
             lm32d_dat_w,
             uart0_dat_r,
             uart0_dat_w,
             timer0_dat_r,
             timer0_dat_w,
             gpio0_dat_r,
             gpio0_dat_w,
             bram0_dat_r,
             bram0_dat_w,
             ddr0_dat_w,
             ddr0_dat_r;

wire [3:0]   lm32i_sel,
             lm32d_sel,
             uart0_sel,
             timer0_sel,
             gpio0_sel,
             bram0_sel,
             ddr0_sel;

wire         lm32i_we,
             lm32d_we,
             uart0_we,
             timer0_we,
             gpio0_we,
             bram0_we,
             ddr0_we;

wire         lm32i_cyc,
             lm32d_cyc,
             uart0_cyc,
             timer0_cyc,
             gpio0_cyc,
             bram0_cyc,
             ddr0_cyc;

wire         lm32i_stb,
             lm32d_stb,
             uart0_stb,
             timer0_stb,
             gpio0_stb,
             bram0_stb,
             ddr0_stb;

wire         lm32i_ack,
             lm32d_ack,
             uart0_ack,
             timer0_ack,
             gpio0_ack,
             bram0_ack,
             ddr0_ack;

wire         lm32i_rty,
             lm32d_rty;

wire         lm32i_err,
             lm32d_err;

wire         lm32i_lock,
             lm32d_lock;

wire [2:0]   lm32i_cti,
             lm32d_cti;

wire [1:0]   lm32i_bte,
             lm32d_bte;

//---------------------------------------------------------------------------
// Interrupts
//---------------------------------------------------------------------------
wire  [31:0] intr_n;
wire         uart0_intr = 0;
wire         gpio0_intr;
wire   [1:0] timer0_intr;

assign intr_n = { 28'hFFFFFFF, ~timer0_intr[1], ~gpio0_intr, ~timer0_intr[0], ~uart0_intr };

//---------------------------------------------------------------------------
// Wishbone Interconnect
//---------------------------------------------------------------------------
wb_conbus_top #(
	.s0_addr_w ( 3 ),
	.s0_addr   ( 3'h4 ),        // ddr0
	.s1_addr_w ( 3 ),
	.s1_addr   ( 3'h5 ),        
	.s27_addr_w( 15 ),
	.s2_addr   ( 15'h0000 ),    // bram0 
	.s3_addr   ( 15'h7000 ),    // uart0
	.s4_addr   ( 15'h7001 ),    // timer0
	.s5_addr   ( 15'h7002 ),    // gpio0
	.s6_addr   ( 15'h7003 ),
	.s7_addr   ( 15'h7004 )
) conmax0 (
	.clk_i( clk ),
	.rst_i( rst ),
	// Master0
	.m0_dat_i(  lm32i_dat_w  ),
	.m0_dat_o(  lm32i_dat_r  ),
	.m0_adr_i(  lm32i_adr    ),
	.m0_we_i (  lm32i_we     ),
	.m0_sel_i(  lm32i_sel    ),
	.m0_cyc_i(  lm32i_cyc    ),
	.m0_stb_i(  lm32i_stb    ),
	.m0_ack_o(  lm32i_ack    ),
	.m0_rty_o(  lm32i_rty    ),
	.m0_err_o(  lm32i_err    ),
	// Master1
	.m1_dat_i(  lm32d_dat_w  ),
	.m1_dat_o(  lm32d_dat_r  ),
	.m1_adr_i(  lm32d_adr    ),
	.m1_we_i (  lm32d_we     ),
	.m1_sel_i(  lm32d_sel    ),
	.m1_cyc_i(  lm32d_cyc    ),
	.m1_stb_i(  lm32d_stb    ),
	.m1_ack_o(  lm32d_ack    ),
	.m1_rty_o(  lm32d_rty    ),
	.m1_err_o(  lm32d_err    ),
	// Master2
	.m2_dat_i(  gnd32  ),
	.m2_adr_i(  gnd32  ),
	.m2_sel_i(  gnd4   ),
	.m2_cyc_i(  gnd    ),
	.m2_stb_i(  gnd    ),
	// Master3
	.m3_dat_i(  gnd32  ),
	.m3_adr_i(  gnd32  ),
	.m3_sel_i(  gnd4   ),
	.m3_cyc_i(  gnd    ),
	.m3_stb_i(  gnd    ),
	// Master4
	.m4_dat_i(  gnd32  ),
	.m4_adr_i(  gnd32  ),
	.m4_sel_i(  gnd4   ),
	.m4_cyc_i(  gnd    ),
	.m4_stb_i(  gnd    ),
	// Master5
	.m5_dat_i(  gnd32  ),
	.m5_adr_i(  gnd32  ),
	.m5_sel_i(  gnd4   ),
	.m5_cyc_i(  gnd    ),
	.m5_stb_i(  gnd    ),
	// Master6
	.m6_dat_i(  gnd32  ),
	.m6_adr_i(  gnd32  ),
	.m6_sel_i(  gnd4   ),
	.m6_cyc_i(  gnd    ),
	.m6_stb_i(  gnd    ),
	// Master7
	.m7_dat_i(  gnd32  ),
	.m7_adr_i(  gnd32  ),
	.m7_sel_i(  gnd4   ),
	.m7_cyc_i(  gnd    ),
	.m7_stb_i(  gnd    ),

	// Slave0
	.s0_dat_i(  ddr0_dat_r   ),
	.s0_dat_o(  ddr0_dat_w   ),
	.s0_adr_o(  ddr0_adr     ),
	.s0_sel_o(  ddr0_sel     ),
	.s0_we_o(   ddr0_we      ),
	.s0_cyc_o(  ddr0_cyc     ),
	.s0_stb_o(  ddr0_stb     ),
	.s0_ack_i(  ddr0_ack     ),
	.s0_err_i(  gnd    ),
	.s0_rty_i(  gnd    ),
	// Slave1
	.s1_dat_i(  gnd32  ),
	.s1_ack_i(  gnd    ),
	.s1_err_i(  gnd    ),
	.s1_rty_i(  gnd    ),
	// Slave2
	.s2_dat_i(  bram0_dat_r ),
	.s2_dat_o(  bram0_dat_w ),
	.s2_adr_o(  bram0_adr   ),
	.s2_sel_o(  bram0_sel   ),
	.s2_we_o(   bram0_we    ),
	.s2_cyc_o(  bram0_cyc   ),
	.s2_stb_o(  bram0_stb   ),
	.s2_ack_i(  bram0_ack   ),
	.s2_err_i(  gnd         ),
	.s2_rty_i(  gnd         ),
	// Slave3
	.s3_dat_i(  uart0_dat_r ),
	.s3_dat_o(  uart0_dat_w ),
	.s3_adr_o(  uart0_adr   ),
	.s3_sel_o(  uart0_sel   ),
	.s3_we_o(   uart0_we    ),
	.s3_cyc_o(  uart0_cyc   ),
	.s3_stb_o(  uart0_stb   ),
	.s3_ack_i(  uart0_ack   ),
	.s3_err_i(  uart0_err   ),
	.s3_rty_i(  uart0_rty   ),
	// Slave4
	.s4_dat_i(  timer0_dat_r ),
	.s4_dat_o(  timer0_dat_w ),
	.s4_adr_o(  timer0_adr   ),
	.s4_sel_o(  timer0_sel   ),
	.s4_we_o(   timer0_we    ),
	.s4_cyc_o(  timer0_cyc   ),
	.s4_stb_o(  timer0_stb   ),
	.s4_ack_i(  timer0_ack   ),
	.s4_err_i(  timer0_err   ),
	.s4_rty_i(  timer0_rty   ),
	// Slave5
	.s5_dat_i(  gpio0_dat_r  ),
	.s5_dat_o(  gpio0_dat_w  ),
	.s5_adr_o(  gpio0_adr    ),
	.s5_sel_o(  gpio0_sel    ),
	.s5_we_o(   gpio0_we     ),
	.s5_cyc_o(  gpio0_cyc    ),
	.s5_stb_o(  gpio0_stb    ),
	.s5_ack_i(  gpio0_ack    ),
	.s5_err_i(  gnd          ),
	.s5_rty_i(  gnd          ),
	// Slave6
	.s6_dat_i(  gnd32  ),
	.s6_ack_i(  gnd    ),
	.s6_err_i(  gnd    ),
	.s6_rty_i(  gnd    ),
	// Slave7
	.s7_dat_i(  gnd32  ),
	.s7_ack_i(  gnd    ),
	.s7_err_i(  gnd    ),
	.s7_rty_i(  gnd    )
);

//---------------------------------------------------------------------------
// LM32 CPU 
//---------------------------------------------------------------------------
lm32_cpu lm0 (
	.clk_i(  clk  ),
	.rst_i(  rst  ),
	.interrupt_n(  intr_n  ),
	//
	.I_ADR_O(  lm32i_adr    ),
	.I_DAT_I(  lm32i_dat_r  ),
	.I_DAT_O(  lm32i_dat_w  ),
	.I_SEL_O(  lm32i_sel    ),
	.I_CYC_O(  lm32i_cyc    ),
	.I_STB_O(  lm32i_stb    ),
	.I_ACK_I(  lm32i_ack    ),
	.I_WE_O (  lm32i_we     ),
	.I_CTI_O(  lm32i_cti    ),
	.I_LOCK_O( lm32i_lock   ),
	.I_BTE_O(  lm32i_bte    ),
	.I_ERR_I(  lm32i_err    ),
	.I_RTY_I(  lm32i_rty    ),
	//
	.D_ADR_O(  lm32d_adr    ),
	.D_DAT_I(  lm32d_dat_r  ),
	.D_DAT_O(  lm32d_dat_w  ),
	.D_SEL_O(  lm32d_sel    ),
	.D_CYC_O(  lm32d_cyc    ),
	.D_STB_O(  lm32d_stb    ),
	.D_ACK_I(  lm32d_ack    ),
	.D_WE_O (  lm32d_we     ),
	.D_CTI_O(  lm32d_cti    ),
	.D_LOCK_O( lm32d_lock   ),
	.D_BTE_O(  lm32d_bte    ),
	.D_ERR_I(  lm32d_err    ),
	.D_RTY_I(  lm32d_rty    )
);
	
//------------------------------------------------------------------
// Block RAM
//------------------------------------------------------------------
wb_bram #(
	.adr_width( 12 ),
	.mem_file_name( bootram_file )
) bram0 (
	.clk_i(  clk  ),
	.rst_i(  rst  ),
	//
	.wb_adr_i(  bram0_adr    ),
	.wb_dat_o(  bram0_dat_r  ),
	.wb_dat_i(  bram0_dat_w  ),
	.wb_sel_i(  bram0_sel    ),
	.wb_stb_i(  bram0_stb    ),
	.wb_cyc_i(  bram0_cyc    ),
	.wb_ack_o(  bram0_ack    ),
	.wb_we_i(   bram0_we     )
);

//------------------------------------------------------------------
// ddr0
//------------------------------------------------------------------
wire ddr0_ps_ready, ddr0_ps_up, ddr0_ps_down;

wb_ddr #(
	.clk_freq(     clk_freq         ),
	.clk_multiply( ddr_clk_multiply ),
	.clk_divide(   ddr_clk_divide   ),
	.phase_shift(  ddr_phase_shift  ),
	.wait200_init( ddr_wait200_init )
) ddr0 (
	.clk(          clk         ),
	.reset(        rst         ),
	// DDR Ports
	.ddr_clk(      ddr_clk     ),
	.ddr_clk_n(    ddr_clk_n   ),
	.ddr_clk_fb(   ddr_clk_fb  ),
	.ddr_ras_n(    ddr_ras_n   ),
	.ddr_cas_n(    ddr_cas_n   ),
	.ddr_we_n(     ddr_we_n    ),
	.ddr_cke(      ddr_cke     ),
	.ddr_cs_n(     ddr_cs_n    ),
	.ddr_a(        ddr_a       ),
	.ddr_ba(       ddr_ba      ),
	.ddr_dq(       ddr_dq      ),
	.ddr_dqs(      ddr_dqs     ),
	.ddr_dm(       ddr_dm      ),
	// FML (FastMemoryLink)
	.wb_cyc_i(    ddr0_cyc     ),
	.wb_stb_i(    ddr0_stb     ),
	.wb_we_i(     ddr0_we      ),
	.wb_adr_i(    ddr0_adr     ),
	.wb_dat_o(    ddr0_dat_r   ),
	.wb_dat_i(    ddr0_dat_w   ),
	.wb_sel_i(    ddr0_sel     ),
	.wb_ack_o(    ddr0_ack     ),
	// phase shifting
	.ps_ready(   ddr0_ps_ready ),
	.ps_up(      ddr0_ps_up    ),
	.ps_down(    ddr0_ps_down  ),
	// logic probe
//	.probe_clk(    probe_clk   ),
	.probe_sel(    'b0         )
//	.probe(        probe       )
);


//------------------------------------------------------------------
// uart0
//------------------------------------------------------------------
wire uart0_rxd;
wire uart0_txd;

wb_uart #(
	.clk_freq( clk_freq        ),
	.baud(     uart_baud_rate  )
) uart0 (
	.clk( clk ),
	.reset( rst ),
	//
	.wb_adr_i( uart0_adr ),
	.wb_dat_i( uart0_dat_w ),
	.wb_dat_o( uart0_dat_r ),
	.wb_stb_i( uart0_stb ),
	.wb_cyc_i( uart0_cyc ),
	.wb_we_i(  uart0_we ),
	.wb_sel_i( uart0_sel ),
	.wb_ack_o( uart0_ack ), 
//	.intr(       uart0_intr ),
	.uart_rxd( uart0_rxd ),
	.uart_txd( uart0_txd )
);

//------------------------------------------------------------------
// timer0
//------------------------------------------------------------------
wb_timer #(
	.clk_freq(   clk_freq  )
) timer0 (
	.clk(      clk          ),
	.reset(    rst          ),
	//
	.wb_adr_i( timer0_adr   ),
	.wb_dat_i( timer0_dat_w ),
	.wb_dat_o( timer0_dat_r ),
	.wb_stb_i( timer0_stb   ),
	.wb_cyc_i( timer0_cyc   ),
	.wb_we_i(  timer0_we    ),
	.wb_sel_i( timer0_sel   ),
	.wb_ack_o( timer0_ack   ), 
	.intr(     timer0_intr  )
);

//------------------------------------------------------------------
// General Purpose IO
//------------------------------------------------------------------
wire [31:0] gpio0_in;
wire [31:0] gpio0_out;
wire [31:0] gpio0_oe;

wb_gpio gpio0 (
	.clk(      clk          ),
	.reset(    rst          ),
	//
	.wb_adr_i( gpio0_adr    ),
	.wb_dat_i( gpio0_dat_w  ),
	.wb_dat_o( gpio0_dat_r  ),
	.wb_stb_i( gpio0_stb    ),
	.wb_cyc_i( gpio0_cyc    ),
	.wb_we_i(  gpio0_we     ),
	.wb_sel_i( gpio0_sel    ),
	.wb_ack_o( gpio0_ack    ), 
	.intr(     gpio0_intr   ),
	// GPIO
	.gpio_in(  gpio0_in     ),
	.gpio_out( gpio0_out    ),
	.gpio_oe(  gpio0_oe     )
);

//------------------------------------------------------------------
// LogicAnalyzerComponent
//------------------------------------------------------------------
wire        lac_rxd;
wire        lac_txd;
wire        lac_cts;
wire        lac_rts;
assign      lac_rts = 1;

/* Diabled LAC

lac #(
	.uart_freq_hz(     clk_freq ),
	.uart_baud(  uart_baud_rate ),
	.adr_width(              11 ),
	.width(                   8 )
) lac0 (
	.reset(           rst  ),
	.uart_clk(        clk  ),
	.uart_rxd(    lac_rxd  ),
	.uart_cts(    lac_cts  ),
	.uart_txd(    lac_txd  ),
	.uart_rts(    lac_rts  ),
	//
	.probe_clk(  probe_clk ),
	.select(     probe_sel ),
	.probe(      probe     )
);

assign probe_clk = clk;
assign probe = (probe_sel[3:0] == 'h0) ? { rst, intr_n[1], lm32i_stb, lm32i_ack, lm32d_stb, lm32d_cyc, lm32d_we, lm32d_ack } :
               (probe_sel[3:0] == 'h1) ? lm32i_adr[31:24] :
               (probe_sel[3:0] == 'h2) ? lm32i_adr[23:16] :
               (probe_sel[3:0] == 'h3) ? lm32i_adr[15: 8] :
               (probe_sel[3:0] == 'h4) ? lm32i_adr[ 7: 0] :
               (probe_sel[3:0] == 'h5) ? lm32i_dat_r[31:24] :
               (probe_sel[3:0] == 'h6) ? lm32i_dat_r[23:16] :
               (probe_sel[3:0] == 'h7) ? lm32i_dat_r[15: 8] :
               (probe_sel[3:0] == 'h8) ? lm32i_dat_r[ 7: 0] :
               (probe_sel[3:0] == 'h9) ? lm32d_adr[31:24] :
               (probe_sel[3:0] == 'ha) ? lm32d_adr[23:16] :
               (probe_sel[3:0] == 'hb) ? lm32d_adr[15: 8] :
                                         lm32d_adr[ 7: 0] ;

*/

//----------------------------------------------------------------------------
// Mux UART wires according to sw[0]
//----------------------------------------------------------------------------
assign uart_txd  = (sw[0]) ? uart0_txd : lac_txd;
assign lac_rxd   = (sw[0]) ?         1 : uart_rxd;
assign uart0_rxd = (sw[0]) ? uart_rxd  : 1;

//----------------------------------------------------------------------------
// Mux LEDs and Push Buttons according to sw[1]
//----------------------------------------------------------------------------
wire [3:0] debug_leds = { lm32i_stb, lm32i_ack, lm32d_stb, lm32d_ack };
wire [3:0] gpio_leds  = gpio0_out[3:0];

assign led_n   = (sw[1]) ? ~gpio_leds : ~debug_leds;

assign gpio0_in[ 7: 0] =  8'b0;
assign gpio0_in[12: 8] = (sw[1]) ?     ~btn_n : 5'b0;
assign gpio0_in[15:13] =  3'b0;
assign gpio0_in[31:17] = 15'b0;

//----------------------------------------------------------------------------
// Wire DDR phase shifting signals
//----------------------------------------------------------------------------
assign gpio0_in[16]  = ddr0_ps_ready;          // ddr0 phase shifter 
assign ddr0_ps_up    = gpio0_out[16];          // ddr0 phase shifter
assign ddr0_ps_down  = gpio0_out[17];          // ddr0 phase shifter

endmodule 
