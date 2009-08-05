// Wishbone Farbborg -------------------------------------------------------

module wb_farbborg (
    clk,
    reset,
    
    wb_adr_i,
    wb_dat_i,
    wb_dat_o,
    wb_sel_i,
    wb_cyc_i,
    wb_stb_i,
    wb_ack_o,
    wb_we_i,
    
    clk_pwm,
    lsr_clr,
    lsr_d,
    lsr_c,
    latch_data,
    psr_c,
    psr_d,
    col_enable,
    plane_out
);

input       clk;
input       reset;

// Wishbone bus
input [31:0] wb_adr_i;
input [31:0] wb_dat_i;
output[31:0] wb_dat_o;
input [ 3:0] wb_sel_i;
input        wb_cyc_i;
input        wb_stb_i;
output       wb_ack_o;
input        wb_we_i;

// farbborg interface
input        clk_pwm;

// Latch shift-register
output       lsr_clr;
output       lsr_d;
output       lsr_c;
output[ 7:0] latch_data;

// Plane shift-register	
output       psr_c;
output       psr_d;
output       col_enable;

reg          we; 
reg          ack;

// May be debug stuff
output[ 4:0] plane_out;

pwm pwm1 ( 
    .reset			(reset			),
    .pwm_clk		(clk_pwm		),
    .cpu_addr		(wb_adr_i[12:2]	),
    .din			(wb_dat_i[15:0]	),
    .we				(we				),
    .cpu_clk		(clk			),
    .lsr_clr		(lsr_clr		),
    .lsr_d			(lsr_d			),
    .lsr_c			(lsr_c			),
    .latch_data		(latch_data		),
    .psr_c			(psr_c			),
    .psr_d			(psr_d			),
    .col_enable		(col_enable		)
);

assign wb_ack_o = wb_stb_i & ack;

always @(wb_stb_i, wb_cyc_i, wb_we_i)
begin
	ack <= 0;
	we  <= 0;
	if (wb_stb_i && wb_cyc_i) 
	begin
		if (wb_we_i) 
			we <= 1;
		ack <= 1;
	end
end

endmodule
