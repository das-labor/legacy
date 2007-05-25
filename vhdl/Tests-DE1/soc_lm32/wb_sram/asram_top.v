// =============================================================================
//                           COPYRIGHT NOTICE
// Copyright 2004 (c) Lattice Semiconductor Corporation
// ALL RIGHTS RESERVED
// This confidential and proprietary software may be used only as authorised by
// a licensing agreement from Lattice Semiconductor Corporation.
// The entire notice above must be reproduced on all authorized copies and
// copies may only be made to the extent permitted by a licensing agreement from
// Lattice Semiconductor Corporation.
//
// Lattice Semiconductor Corporation        TEL : 1-800-Lattice (USA and Canada)
// 5555 NE Moore Court                            408-826-6000 (other locations)
// Hillsboro, OR 97124                     web  : http://www.latticesemi.com/
// U.S.A                                   email: techsupport@latticesemi.com
// =============================================================================/
//                        FILE DETAILS
// Project          : Async SRAM Controller
// File             : asram_core.v
// Title            : Asynchronous SRAM Controller
// Dependencies     : asram_params.v
// Description      : Implements the logic to interface async SRAM with Wishbone
//                    bus.
// =============================================================================
//                        REVISION HISTORY
// Version          : 1.0
// Author           : Frank Xie
// Mod. Date        : Jun 27, 2005
// Changes Made     : Initial Creation
//
// =============================================================================
`timescale 1ns/100ps


module asram_top
  #(parameter SRAM_DATA_WIDTH = 16,
    parameter SRAM_ADDR_WIDTH = 18,
    parameter READ_LATENCY = 1,
    parameter WRITE_LATENCY = 1,
    parameter FLASH_SIGNALS = 0,
    parameter FLASH_BYTE = 0,
    parameter FLASH_WP = 0,
    parameter FLASH_RST = 0,
    parameter FLASH_BYTEN = 0,
    parameter FLASH_WPN = 0,
    parameter FLASH_RSTN = 0)
  (
    // Clock and reset
    clk_i,
    rst_i,
    // Wishbone side interface
    ASRAM_CTI_I,
    ASRAM_BTE_I,
    ASRAM_ADR_I,
    ASRAM_DAT_I,
    ASRAM_SEL_I,
    ASRAM_WE_I,
    ASRAM_STB_I,
    ASRAM_CYC_I,
    ASRAM_LOCK_I,
    ASRAM_ACK_O,
    ASRAM_DAT_O,
    ASRAM_ERR_O,
    ASRAM_RTY_O,
    // SRAM side interface
    sram_addr,
    sram_data_in,
    sram_data_out,
    sram_csn,
    sram_be,
    sram_wen,
    sram_oen,
    sram_byten,
    sram_wpn,
    sram_rstn);

`define DLY 0
   parameter  SRAM_BE_WIDTH = SRAM_DATA_WIDTH/8;

//////////////////////////////////////////////////////////////////////////////
//clock and reset inputs
input         clk_i;
input         rst_i;
//Wishbone side interface
input [2:0]   ASRAM_CTI_I;
input [1:0]   ASRAM_BTE_I;
input [31:0]  ASRAM_ADR_I;
input [31:0]  ASRAM_DAT_I;
input [3:0]   ASRAM_SEL_I;
input         ASRAM_WE_I;
input         ASRAM_STB_I;
input         ASRAM_CYC_I;
input         ASRAM_LOCK_I;
output        ASRAM_ACK_O;
output [31:0] ASRAM_DAT_O;
output        ASRAM_ERR_O;
wire          ASRAM_ERR_O = 1'b0;
output        ASRAM_RTY_O;
wire          ASRAM_RTY_O = 1'b0;

//SRAM side interface
output [SRAM_ADDR_WIDTH-1:0]  sram_addr;
output                        sram_csn;
output [SRAM_BE_WIDTH-1:0]    sram_be;
output                        sram_wen;
output                        sram_oen;
output  [SRAM_DATA_WIDTH-1:0] sram_data_out;
input   [SRAM_DATA_WIDTH-1:0] sram_data_in;

// These are extra for flash
output sram_byten;
wire   sram_byten ;
output sram_wpn;
wire   sram_wpn   ;
output sram_rstn;
wire   sram_rstn  ; 

generate
  if (FLASH_BYTEN == 1)
    assign sram_byten = 1'b1;
  else 
    assign sram_byten = 1'b0;
endgenerate
generate
  if (FLASH_WPN == 1)
    assign sram_wpn = 1'b1;
  else 
    assign sram_wpn = 1'b0;
endgenerate
generate
  if (FLASH_RSTN == 1)
    assign sram_rstn = 1'b1;
  else 
    assign sram_rstn = 1'b0;
endgenerate

wire [SRAM_ADDR_WIDTH-1:0] sram_addr_core;
wire                        sram_csn_core;
wire [SRAM_BE_WIDTH-1:0]   sram_be_core;
wire                        sram_wen_core;
wire                        sram_oen_core;
wire [SRAM_DATA_WIDTH-1:0] sram_data_out;

asram_core   #(.SRAM_DATA_WIDTH (SRAM_DATA_WIDTH),
	       .SRAM_ADDR_WIDTH (SRAM_ADDR_WIDTH),
	       .READ_LATENCY    (READ_LATENCY),
	       .WRITE_LATENCY   (WRITE_LATENCY))
  core_inst   (.clk_i(clk_i),
	       .rst_i(rst_i),
	       .cti_i(ASRAM_CTI_I),
	       .bte_i(ASRAM_BTE_I),
	       .addr_i(ASRAM_ADR_I),
	       .dat_i(ASRAM_DAT_I),
	       .sel_i(ASRAM_SEL_I),
	       .we_i(ASRAM_WE_I),
	       .stb_i(ASRAM_STB_I),
	       .cyc_i(ASRAM_CYC_I),
	       .ack_o(ASRAM_ACK_O),
	       .dat_o(ASRAM_DAT_O),
	       .sram_addr(sram_addr_core),
	       .sram_data_in(sram_data_in),
	       .sram_data_out(sram_data_out),
	       .sram_csn(sram_csn_core),
	       .sram_be(sram_be_core),
	       .sram_wen(sram_wen_core),
	       .sram_oen(sram_oen_core));

   wire [SRAM_DATA_WIDTH-1: 0] zwire;
   generate
      genvar 		       i;
      for ( i = 0 ; i < SRAM_DATA_WIDTH; i = i + 1)
	begin : zwa
	   assign zwire[i] = 1'bz;
	end
   endgenerate

   assign #`DLY sram_addr = sram_addr_core;
   assign #`DLY  sram_csn  = sram_csn_core;
   assign #`DLY  sram_be   = sram_be_core;
   assign #`DLY  sram_wen  = sram_wen_core;
   assign #`DLY  sram_oen  = sram_oen_core;

endmodule
