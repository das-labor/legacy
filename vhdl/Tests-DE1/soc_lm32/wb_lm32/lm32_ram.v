// =============================================================================
//                           COPYRIGHT NOTICE
// Copyright 2006 (c) Lattice Semiconductor Corporation
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
//                         FILE DETAILS
// Project          : LatticeMico32
// File             : lm32_ram.v
// Title            : PMI pseudo dual port RAM.
// Version          : 6.0.13
// =============================================================================

`include "lm32_include.v"

/////////////////////////////////////////////////////
// Module interface
/////////////////////////////////////////////////////

module lm32_ram (
    // ----- Inputs -------
    read_clk,
    write_clk,
    reset,
    enable_read,
    read_address,
    enable_write,
    write_address,
    write_data,
    write_enable,
    // ----- Outputs -------
    read_data
    );

/////////////////////////////////////////////////////
// Parameters
/////////////////////////////////////////////////////

parameter data_width = 1;               // Width of the data ports
parameter address_width = 1;            // Width of the address ports

/////////////////////////////////////////////////////
// Inputs
/////////////////////////////////////////////////////

input read_clk;                         // Read clock
input write_clk;                        // Write clock
input reset;                            // Reset

input enable_read;                      // Access enable
input [address_width-1:0] read_address; // Read/write address
input enable_write;                     // Access enable
input [address_width-1:0] write_address;// Read/write address
input [data_width-1:0] write_data;      // Data to write to specified address
input write_enable;                     // Write enable

/////////////////////////////////////////////////////
// Outputs
/////////////////////////////////////////////////////

output [data_width-1:0] read_data;      // Data read from specified addess
wire   [data_width-1:0] read_data;
    
/////////////////////////////////////////////////////
// Instantiations
/////////////////////////////////////////////////////

pmi_ram_dp #(
    // ----- Parameters -------
    .pmi_wr_addr_depth  (1 << address_width),
    .pmi_wr_addr_width  (address_width),
    .pmi_wr_data_width  (data_width),
    .pmi_rd_addr_depth  (1 << address_width),
    .pmi_rd_addr_width  (address_width),
    .pmi_rd_data_width  (data_width),
    .pmi_regmode        ("noreg"),
    .pmi_gsr            ("enable"),
    .pmi_resetmode      ("async"),
    .pmi_init_file      ("none"),
    .pmi_init_file_format("binary"),
    .pmi_family         ("ECP"),
    .module_type        ("pmi_ram_dp")
  ) ram (
    // ----- Inputs -------
    .RdClock            (read_clk),
    .WrClock            (write_clk),
    .Reset              (reset),
    .Data               (write_data),
    .RdAddress          (read_address),
    .WrAddress          (write_address),
    .RdClockEn          (enable_read),
    .WrClockEn          (enable_write),
    .WE                 (write_enable),
    // ----- Outputs -------
    .Q                  (read_data)
    );

endmodule
