//---------------------------------------------------------------------------
// DDR Controller
// 
// (c) Joerg Bornschein (<jb@capsec.org>)
//---------------------------------------------------------------------------

`ifdef DDR_INCLUDE_V
`else
`define DDR_INCLUDE_V

`timescale 1ns/10ps

//---------------------------------------------------------------------------
// Frequency 
//---------------------------------------------------------------------------
`define SYS_CLK_FREQUENCY   50000     // in kHz
`define DDR_CLK_MULTIPLY    5        
`define DDR_CLK_DIVIDE      2

//---------------------------------------------------------------------------
// FML ranges
//---------------------------------------------------------------------------
`define FML_ADR_RNG     25:4 
`define FML_ADR_BA_RNG  25:24
`define FML_ADR_ROW_RNG 23:11
`define FML_ADR_COL_RNG 10:4
`define FML_DAT_RNG    127:0
`define FML_MSK_RNG     15:0

//---------------------------------------------------------------------------
// Width
//---------------------------------------------------------------------------
`define CMD_WIDTH  3
`define A_WIDTH    13
`define BA_WIDTH   2
`define DQ_WIDTH   16
`define DQS_WIDTH  2
`define DM_WIDTH   2

`define RFIFO_WIDTH  (2 * `DQ_WIDTH )
`define WFIFO_WIDTH  (2 * (`DQ_WIDTH + `DM_WIDTH))
`define CBA_WIDTH    (`CMD_WIDTH+`BA_WIDTH+`A_WIDTH)

// Ranges
`define CMD_RNG      (`CMD_WIDTH-1):0
`define A_RNG        (`A_WIDTH-1):0
`define BA_RNG       (`BA_WIDTH-1):0
`define DQ_RNG       (`DQ_WIDTH-1):0
`define DQS_RNG      (`DQS_WIDTH-1):0
`define DM_RNG       (`DM_WIDTH-1):0

`define RFIFO_RNG    (`RFIFO_WIDTH-1):0
`define WFIFO_RNG    (`WFIFO_WIDTH-1):0
`define WFIFO_D0_RNG (1*`DQ_WIDTH-1):0
`define WFIFO_D1_RNG (2*`DQ_WIDTH-1):(`DQ_WIDTH)
`define WFIFO_M0_RNG (2*`DQ_WIDTH+1*`DM_WIDTH-1):(2*`DQ_WIDTH+0*`DM_WIDTH)
`define WFIFO_M1_RNG (2*`DQ_WIDTH+2*`DM_WIDTH-1):(2*`DQ_WIDTH+1*`DM_WIDTH)
`define CBA_RNG      (`CBA_WIDTH-1):0
`define CBA_CMD_RNG  (`CBA_WIDTH-1):(`CBA_WIDTH-3)
`define CBA_BA_RNG   (`CBA_WIDTH-4):(`CBA_WIDTH-5)
`define CBA_A_RNG    (`CBA_WIDTH-6):0

`define ROW_RNG      12:0

//---------------------------------------------------------------------------
// DDR 
//---------------------------------------------------------------------------
`define DDR_CMD_NOP   3'b111
`define DDR_CMD_ACT   3'b011
`define DDR_CMD_READ  3'b101
`define DDR_CMD_WRITE 3'b100
`define DDR_CMD_TERM  3'b110
`define DDR_CMD_PRE   3'b010
`define DDR_CMD_AR    3'b001
`define DDR_CMD_MRS   3'b000

`define T_MRD   2           // Mode register set 
`define T_RP    2           // Precharge Command Period
`define T_RFC   8           // Precharge Command Period

`define DDR_INIT_EMRS	`A_WIDTH'b0000000000000  // DLL enable
`define DDR_INIT_MRS1	`A_WIDTH'b0000101100011  // BURST=8, CL=2.5, DLL RESET
`define DDR_INIT_MRS2	`A_WIDTH'b0000001100011  // BURST=8, CL=2.5

`define ADR_BA_RNG    25:24
`define ADR_ROW_RNG   23:11
`define ADR_COL_RNG   10:4

`endif
