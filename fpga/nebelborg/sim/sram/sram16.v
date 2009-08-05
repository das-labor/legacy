//---------------------------------------------------------------------------
// Behavioral model of a static ram chip
//
// Organization: 
//      
//       16 bit x 2**(adr_width-1)
//---------------------------------------------------------------------------
module sram16 #(
	parameter adr_width = 18
) (
	input   [adr_width-1:0] adr,
	inout            [15:0] dat,
	input                   ub_n,
	input                   lb_n,
	input                   cs_n, 
	input                   we_n, 
	input                   oe_n
);

parameter dat_width = 16;

//---------------------------------------------------------------------------
// Actual RAM cells
//---------------------------------------------------------------------------
reg [7:0] mem_ub [0:1<<adr_width];
reg [7:0] mem_lb [0:1<<adr_width];

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
wire [15:0] mem = { mem_ub[adr], mem_lb[adr] };
wire [15:0] zzz = 16'bz;

// Drive output
assign dat = (!cs_n && !oe_n) ? mem : zzz;

// Write to UB
always @(*)
	if (!cs_n && !we_n && !ub_n)
		mem_ub[adr] = dat[15:8];

// Write to LB
always @(*)
	if (!cs_n && !we_n && !lb_n)
		mem_lb[adr] = dat[7:0];

always @(*)
	if (!we_n && !oe_n)
		$display("Operational error in RamChip: OE and WE both active");

endmodule

