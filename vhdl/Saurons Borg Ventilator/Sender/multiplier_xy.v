////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1995-2007 Xilinx, Inc.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////
//   ____  ____
//  /   /\/   /
// /___/  \  /    Vendor: Xilinx
// \   \   \/     Version: J.40
//  \   \         Application: netgen
//  /   /         Filename: multiplier_xy.v
// /___/   /\     Timestamp: Thu Aug 06 12:24:23 2009
// \   \  /  \ 
//  \___\/\___\
//             
// Command	: -intstyle ise -w -sim -ofmt verilog "D:\Saurons Borg Ventilator\Sender\tmp\_cg\multiplier_xy.ngc" "D:\Saurons Borg Ventilator\Sender\tmp\_cg\multiplier_xy.v" 
// Device	: 3s200ft256-5
// Input file	: D:/Saurons Borg Ventilator/Sender/tmp/_cg/multiplier_xy.ngc
// Output file	: D:/Saurons Borg Ventilator/Sender/tmp/_cg/multiplier_xy.v
// # of Modules	: 1
// Design Name	: multiplier_xy
// Xilinx        : C:\Xilinx92i
//             
// Purpose:    
//     This verilog netlist is a verification model and uses simulation 
//     primitives which may not represent the true implementation of the 
//     device, however the netlist is functionally correct and should not 
//     be modified. This file cannot be synthesized and should only be used 
//     with supported simulation tools.
//             
// Reference:  
//     Development System Reference Guide, Chapter 23
//     Synthesis and Simulation Design Guide, Chapter 6
//             
////////////////////////////////////////////////////////////////////////////////

`timescale 1 ns/1 ps

module multiplier_xy (
  clk, a, b, p
);
  input clk;
  input [7 : 0] a;
  input [8 : 0] b;
  output [8 : 0] p;
  
  // synopsys translate_off
  
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[0] ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[1] ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[2] ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[3] ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[4] ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[5] ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[6] ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[7] ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[8] ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[9] ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[10] ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[11] ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[12] ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[13] ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[14] ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[15] ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[35] ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_0_2 ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_1_3 ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_2_4 ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_3_5 ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_4_6 ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_5_7 ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_6_8 ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_7_9 ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_8_10 ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_9_11 ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_10_12 ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_11_13 ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_12_14 ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_13_15 ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_14_16 ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_15_17 ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_35_18 ;
  wire \BU2/N1 ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/Pdelay/N1 ;
  wire \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/Pdelay/N0 ;
  wire NLW_VCC_P_UNCONNECTED;
  wire NLW_GND_G_UNCONNECTED;
  wire \NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<34>_UNCONNECTED ;
  wire \NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<33>_UNCONNECTED ;
  wire \NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<32>_UNCONNECTED ;
  wire \NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<31>_UNCONNECTED ;
  wire \NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<30>_UNCONNECTED ;
  wire \NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<29>_UNCONNECTED ;
  wire \NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<28>_UNCONNECTED ;
  wire \NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<27>_UNCONNECTED ;
  wire \NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<26>_UNCONNECTED ;
  wire \NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<25>_UNCONNECTED ;
  wire \NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<24>_UNCONNECTED ;
  wire \NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<23>_UNCONNECTED ;
  wire \NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<22>_UNCONNECTED ;
  wire \NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<21>_UNCONNECTED ;
  wire \NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<20>_UNCONNECTED ;
  wire \NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<19>_UNCONNECTED ;
  wire \NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<18>_UNCONNECTED ;
  wire \NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<17>_UNCONNECTED ;
  wire \NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<16>_UNCONNECTED ;
  wire [7 : 0] a_19;
  wire [8 : 0] b_20;
  wire [0 : 0] \BU2/zero_detect ;
  assign
    a_19[7] = a[7],
    a_19[6] = a[6],
    a_19[5] = a[5],
    a_19[4] = a[4],
    a_19[3] = a[3],
    a_19[2] = a[2],
    a_19[1] = a[1],
    a_19[0] = a[0],
    b_20[8] = b[8],
    b_20[7] = b[7],
    b_20[6] = b[6],
    b_20[5] = b[5],
    b_20[4] = b[4],
    b_20[3] = b[3],
    b_20[2] = b[2],
    b_20[1] = b[1],
    b_20[0] = b[0],
    p[8] = \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_35_18 ,
    p[7] = \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_15_17 ,
    p[6] = \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_14_16 ,
    p[5] = \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_13_15 ,
    p[4] = \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_12_14 ,
    p[3] = \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_11_13 ,
    p[2] = \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_10_12 ,
    p[1] = \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_9_11 ,
    p[0] = \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_8_10 ;
  VCC VCC_0 (
    .P(NLW_VCC_P_UNCONNECTED)
  );
  GND GND_1 (
    .G(NLW_GND_G_UNCONNECTED)
  );
  defparam \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_0 .INIT = 1'b0;
  FD \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_0  (
    .C(clk),
    .D(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[0] ),
    .Q(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_0_2 )
  );
  defparam \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_1 .INIT = 1'b0;
  FD \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_1  (
    .C(clk),
    .D(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[1] ),
    .Q(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_1_3 )
  );
  defparam \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_2 .INIT = 1'b0;
  FD \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_2  (
    .C(clk),
    .D(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[2] ),
    .Q(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_2_4 )
  );
  defparam \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_3 .INIT = 1'b0;
  FD \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_3  (
    .C(clk),
    .D(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[3] ),
    .Q(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_3_5 )
  );
  defparam \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_4 .INIT = 1'b0;
  FD \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_4  (
    .C(clk),
    .D(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[4] ),
    .Q(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_4_6 )
  );
  defparam \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_5 .INIT = 1'b0;
  FD \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_5  (
    .C(clk),
    .D(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[5] ),
    .Q(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_5_7 )
  );
  defparam \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_6 .INIT = 1'b0;
  FD \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_6  (
    .C(clk),
    .D(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[6] ),
    .Q(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_6_8 )
  );
  defparam \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_7 .INIT = 1'b0;
  FD \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_7  (
    .C(clk),
    .D(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[7] ),
    .Q(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_7_9 )
  );
  defparam \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_8 .INIT = 1'b0;
  FD \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_8  (
    .C(clk),
    .D(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[8] ),
    .Q(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_8_10 )
  );
  defparam \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_9 .INIT = 1'b0;
  FD \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_9  (
    .C(clk),
    .D(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[9] ),
    .Q(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_9_11 )
  );
  defparam \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_10 .INIT = 1'b0;
  FD \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_10  (
    .C(clk),
    .D(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[10] ),
    .Q(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_10_12 )
  );
  defparam \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_11 .INIT = 1'b0;
  FD \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_11  (
    .C(clk),
    .D(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[11] ),
    .Q(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_11_13 )
  );
  defparam \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_12 .INIT = 1'b0;
  FD \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_12  (
    .C(clk),
    .D(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[12] ),
    .Q(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_12_14 )
  );
  defparam \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_13 .INIT = 1'b0;
  FD \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_13  (
    .C(clk),
    .D(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[13] ),
    .Q(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_13_15 )
  );
  defparam \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_14 .INIT = 1'b0;
  FD \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_14  (
    .C(clk),
    .D(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[14] ),
    .Q(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_14_16 )
  );
  defparam \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_15 .INIT = 1'b0;
  FD \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_15  (
    .C(clk),
    .D(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[15] ),
    .Q(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_15_17 )
  );
  defparam \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_35 .INIT = 1'b0;
  FD \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_35  (
    .C(clk),
    .D(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[35] ),
    .Q(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Preg<0>_0_35_18 )
  );
  MULT18X18S \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s  (
    .C(clk),
    .CE(\BU2/N1 ),
    .R(\BU2/zero_detect [0]),
    .A({\BU2/zero_detect [0], \BU2/zero_detect [0], \BU2/zero_detect [0], \BU2/zero_detect [0], \BU2/zero_detect [0], \BU2/zero_detect [0], 
\BU2/zero_detect [0], \BU2/zero_detect [0], \BU2/zero_detect [0], \BU2/zero_detect [0], a_19[7], a_19[6], a_19[5], a_19[4], a_19[3], a_19[2], a_19[1]
, a_19[0]}),
    .B({b_20[8], b_20[8], b_20[8], b_20[8], b_20[8], b_20[8], b_20[8], b_20[8], b_20[8], b_20[8], b_20[7], b_20[6], b_20[5], b_20[4], b_20[3], b_20[2]
, b_20[1], b_20[0]}),
    .P({\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[35] , 
\NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<34>_UNCONNECTED , 
\NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<33>_UNCONNECTED , 
\NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<32>_UNCONNECTED , 
\NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<31>_UNCONNECTED , 
\NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<30>_UNCONNECTED , 
\NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<29>_UNCONNECTED , 
\NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<28>_UNCONNECTED , 
\NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<27>_UNCONNECTED , 
\NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<26>_UNCONNECTED , 
\NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<25>_UNCONNECTED , 
\NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<24>_UNCONNECTED , 
\NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<23>_UNCONNECTED , 
\NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<22>_UNCONNECTED , 
\NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<21>_UNCONNECTED , 
\NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<20>_UNCONNECTED , 
\NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<19>_UNCONNECTED , 
\NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<18>_UNCONNECTED , 
\NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<17>_UNCONNECTED , 
\NLW_BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/use_mult18x18.appMULT[0].bppMULT[0].use_Mreg.m18x18s_P<16>_UNCONNECTED , 
\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[15] , \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[14] , 
\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[13] , \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[12] , 
\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[11] , \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[10] , 
\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[9] , \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[8] , 
\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[7] , \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[6] , 
\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[5] , \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[4] , 
\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[3] , \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[2] , 
\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[1] , \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/pi_Mreg<0><0>[0] })
  );
  VCC \BU2/XST_VCC  (
    .P(\BU2/N1 )
  );
  GND \BU2/XST_GND  (
    .G(\BU2/zero_detect [0])
  );
  VCC \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/Pdelay/XST_VCC  (
    .P(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/Pdelay/N1 )
  );
  GND \BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/Pdelay/XST_GND  (
    .G(\BU2/U0/gEMBEDDED_MULT.gEMB_MULTS_only.gMULT18.iMULT18/Pdelay/N0 )
  );

// synopsys translate_on

endmodule

// synopsys translate_off

`timescale  1 ps / 1 ps

module glbl ();

    parameter ROC_WIDTH = 100000;
    parameter TOC_WIDTH = 0;

    wire GSR;
    wire GTS;
    wire PRLD;

    reg GSR_int;
    reg GTS_int;
    reg PRLD_int;

//--------   JTAG Globals --------------
    wire JTAG_TDO_GLBL;
    wire JTAG_TCK_GLBL;
    wire JTAG_TDI_GLBL;
    wire JTAG_TMS_GLBL;
    wire JTAG_TRST_GLBL;

    reg JTAG_CAPTURE_GLBL;
    reg JTAG_RESET_GLBL;
    reg JTAG_SHIFT_GLBL;
    reg JTAG_UPDATE_GLBL;

    reg JTAG_SEL1_GLBL = 0;
    reg JTAG_SEL2_GLBL = 0 ;
    reg JTAG_SEL3_GLBL = 0;
    reg JTAG_SEL4_GLBL = 0;

    reg JTAG_USER_TDO1_GLBL = 1'bz;
    reg JTAG_USER_TDO2_GLBL = 1'bz;
    reg JTAG_USER_TDO3_GLBL = 1'bz;
    reg JTAG_USER_TDO4_GLBL = 1'bz;

    assign (weak1, weak0) GSR = GSR_int;
    assign (weak1, weak0) GTS = GTS_int;
    assign (weak1, weak0) PRLD = PRLD_int;

    initial begin
	GSR_int = 1'b1;
	PRLD_int = 1'b1;
	#(ROC_WIDTH)
	GSR_int = 1'b0;
	PRLD_int = 1'b0;
    end

    initial begin
	GTS_int = 1'b1;
	#(TOC_WIDTH)
	GTS_int = 1'b0;
    end

endmodule

// synopsys translate_on
