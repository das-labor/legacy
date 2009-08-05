///////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2005 Xilinx, Inc.
// This design is confidential and proprietary of Xilinx, All Rights Reserved.
///////////////////////////////////////////////////////////////////////////////
//   ____  ____
//  /   /\/   /
// /___/  \  /   Vendor		    : Xilinx
// \   \   \/    Version	    : $Name: mig_v1_73_b0 $
//  \   \        Application	    : MIG
//  /   /        Filename	    : mem_interface_top_parameters_0.v
// /___/   /\    Date Last Modified : $Date: 2007/06/06 05:44:42 $
// \   \  /  \   Date Created       : Mon May 2 2005
//  \___\/\___\
// Device	: Spartan-3/3E/3A
// Design Name  : DDR1 SDRAM
// Purpose	: This module has the parameters used in the design.
///////////////////////////////////////////////////////////////////////////////

`define   data_width                               16
`define   data_strobe_width                        2
`define   data_mask_width                          2
`define   clk_width                                1
`define   fifo_16                                  1
`define   ReadEnable                               1
`define   memory_width                             8
`define   DatabitsPerReadClock                     8
`define   DatabitsPerMask                          8
`define   no_of_cs                                 1
`define   data_mask                                1
`define   mask_disable                             0
`define   RESET                                    0
`define   cke_width                                1
`define   registered                               0
`define   col_ap_width                             11
`define   write_pipe_itr                           1
`define   write_pipeline                           4
`define   top_bottom                                0
`define   left_right                                1
`define   row_address                              13
`define   column_address                           10
`define   bank_address                             2
`define   spartan3e                                1
`define   burst_length                             3'b001
`define   burst_type                               1'b0
`define   cas_latency_value                        3'b110
`define   Operating_mode                           5'b00000
`define   load_mode_register                       13'b0000001100001
`define   drive_strengh                            1'b0
`define   dll_enable                               1'b0
`define   ext_load_mode_register                   13'b0000000000000
`define   chip_address                             1
`define   reset_active_low                         1'b1
`define   rcd_count_value                          3'b001
`define   ras_count_value                          4'b0101
`define   mrd_count_value                          1'b0
`define   rp_count_value                           3'b001
`define   rfc_count_value                          6'b001001
`define   twr_count_value                          3'b110
`define   twtr_count_value                         3'b100
`define   max_ref_width                            11
`define   max_ref_cnt                        11'b10000000001

`timescale 1ns/100ps
                                        
