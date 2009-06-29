////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1995-2007 Xilinx, Inc.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////
//   ____  ____ 
//  /   /\/   / 
// /___/  \  /    Vendor: Xilinx 
// \   \   \/     Version : 9.2.04i
//  \   \         Application : sch2verilog
//  /   /         Filename : videodaten.vf
// /___/   /\     Timestamp : 05/24/2009 23:40:06
// \   \  /  \ 
//  \___\/\___\ 
//
//Command: C:\Xilinx92i\bin\nt\sch2verilog.exe -intstyle ise -family spartan3 -w "D:/Borg Ventilator empfanger/videodaten.sch" videodaten.vf
//Design Name: videodaten
//Device: spartan3
//Purpose:
//    This verilog netlist is translated from an ECS schematic.It can be 
//    synthesized and simulated, but it should not be modified. 
//
`timescale 1ns / 1ps

module videodaten(clk, 
                  in0a, 
                  in0b, 
                  in1a, 
                  in1b, 
                  in2a, 
                  in2b, 
                  in3a, 
                  in3b, 
                  in4a, 
                  in4b, 
                  XLXN_989, 
                  led, 
                  p_adr, 
                  p_data, 
                  tp1, 
                  tp2, 
                  tp3, 
                  XLXN_981, 
                  XLXN_1010, 
                  XLXN_1023, 
                  XLXN_1024, 
                  XLXN_1025, 
                  XLXN_1026);

    input clk;
    input in0a;
    input in0b;
    input in1a;
    input in1b;
    input in2a;
    input in2b;
    input in3a;
    input in3b;
    input in4a;
    input in4b;
    input XLXN_989;
   output led;
   output [7:0] p_adr;
   output [7:0] p_data;
   output tp1;
   output tp2;
   output tp3;
   output [7:0] XLXN_981;
   output [7:0] XLXN_1010;
   output [7:0] XLXN_1023;
   output [7:0] XLXN_1024;
   output XLXN_1025;
   output XLXN_1026;
   
   wire clk60;
   wire clk200;
   wire com20;
   wire [7:0] data1;
   wire [7:0] data2;
   wire [7:0] data3;
   wire [7:0] data4;
   wire kanal0;
   wire kanal1;
   wire kanal2;
   wire kanal3;
   wire slow_clk;
   wire XLXN_821;
   wire XLXN_824;
   wire XLXN_825;
   wire XLXN_828;
   wire XLXN_832;
   wire XLXN_833;
   wire XLXN_896;
   wire XLXN_950;
   wire XLXN_976;
   wire XLXN_985;
   wire XLXN_1009;
   wire XLXN_1021;
   wire XLXN_1022;
   wire XLXN_1027;
   wire XLXN_1032;
   wire [7:0] XLXN_1033;
   wire [7:0] XLXN_1034;
   wire XLXN_1037;
   wire [7:0] p_adr_DUMMY;
   wire [7:0] p_data_DUMMY;
   
   assign p_adr[7:0] = p_adr_DUMMY[7:0];
   assign p_data[7:0] = p_data_DUMMY[7:0];
   IBUFDS XLXI_172 (.I(in1a), 
                    .IB(in1b), 
                    .O(kanal1));
   // synthesis attribute IOSTANDARD of XLXI_172 is "DEFAULT"
   takt XLXI_203 (.CLKIN_IN(clk), 
                  .RST_IN(XLXN_1027), 
                  .CLKIN_IBUFG_OUT(XLXN_985), 
                  .CLK0_OUT(clk60), 
                  .LOCKED_OUT());
   f_null XLXI_208 (.fo(XLXN_1027));
   IBUFDS XLXI_213 (.I(in2a), 
                    .IB(in2b), 
                    .O(kanal3));
   // synthesis attribute IOSTANDARD of XLXI_213 is "DEFAULT"
   IBUFDS XLXI_214 (.I(in0a), 
                    .IB(in0b), 
                    .O(XLXN_976));
   // synthesis attribute IOSTANDARD of XLXI_214 is "DEFAULT"
   IBUFDS XLXI_215 (.I(in4a), 
                    .IB(in4b), 
                    .O(kanal2));
   // synthesis attribute IOSTANDARD of XLXI_215 is "DEFAULT"
   bram2p XLXI_238 (.addra(p_adr_DUMMY[7:0]), 
                    .addrb(data4[7:0]), 
                    .clka(XLXN_1022), 
                    .clkb(XLXN_1022), 
                    .dina(p_data_DUMMY[7:0]), 
                    .wea(XLXN_1021), 
                    .doutb(XLXN_1010[7:0]));
   f_eins XLXI_239 (.f1(XLXN_1021));
   switch4_8 XLXI_249 (.a(data1[7:0]), 
                       .b(data2[7:0]), 
                       .c(data3[7:0]), 
                       .clk(clk60), 
                       .d(data4[7:0]), 
                       .taste(XLXN_989), 
                       .q(XLXN_981[7:0]));
   IBUFDS XLXI_263 (.I(in3a), 
                    .IB(in3b), 
                    .O(kanal0));
   // synthesis attribute IOSTANDARD of XLXI_263 is "DEFAULT"
   com_clock XLXI_270 (.com_clk(XLXN_976), 
                       .MHZ_200(clk200), 
                       .com_clk_2x(XLXN_1009), 
                       .diag_cf1(), 
                       .diag_cf2(), 
                       .diag_ff1(), 
                       .diag_ff2(), 
                       .diag_res1(), 
                       .diag_res2());
   OBUF XLXI_276 (.I(kanal2), 
                  .O(tp1));
   // synthesis attribute IOSTANDARD of XLXI_276 is "DEFAULT"
   // synthesis attribute SLEW of XLXI_276 is "SLOW"
   // synthesis attribute DRIVE of XLXI_276 is "12"
   clk_200 XLXI_277 (.CLKIN_IN(XLXN_985), 
                     .RST_IN(XLXN_1027), 
                     .CLKFX_OUT(clk200), 
                     .CLK0_OUT(), 
                     .LOCKED_OUT());
   packet_read XLXI_282 (.clk(slow_clk), 
                         .data_in(kanal0), 
                         .counter(), 
                         .crc_er(XLXN_896), 
                         .crc_ok(XLXN_821), 
                         .data_out(data1[7:0]), 
                         .rdy(), 
                         .synchron());
   packet_read XLXI_283 (.clk(slow_clk), 
                         .data_in(kanal1), 
                         .counter(), 
                         .crc_er(XLXN_825), 
                         .crc_ok(XLXN_824), 
                         .data_out(data2[7:0]), 
                         .rdy(), 
                         .synchron());
   packet_read XLXI_284 (.clk(slow_clk), 
                         .data_in(kanal2), 
                         .counter(), 
                         .crc_er(XLXN_950), 
                         .crc_ok(XLXN_828), 
                         .data_out(data3[7:0]), 
                         .rdy(), 
                         .synchron());
   packet_read XLXI_286 (.clk(slow_clk), 
                         .data_in(kanal3), 
                         .counter(), 
                         .crc_er(XLXN_833), 
                         .crc_ok(XLXN_832), 
                         .data_out(data4[7:0]), 
                         .rdy(), 
                         .synchron());
   BUFG XLXI_287 (.I(com20), 
                  .O(slow_clk));
   OBUF XLXI_312 (.I(slow_clk), 
                  .O(tp3));
   // synthesis attribute IOSTANDARD of XLXI_312 is "DEFAULT"
   // synthesis attribute SLEW of XLXI_312 is "SLOW"
   // synthesis attribute DRIVE of XLXI_312 is "12"
   com_20 XLXI_313 (.CLKIN_IN(XLXN_1009), 
                    .RST_IN(XLXN_1037), 
                    .CLK0_OUT(com20), 
                    .LOCKED_OUT(XLXN_1032), 
                    .STATUS_OUT(XLXN_1033[7:0]));
   OBUF XLXI_316 (.I(kanal3), 
                  .O(tp2));
   // synthesis attribute IOSTANDARD of XLXI_316 is "DEFAULT"
   // synthesis attribute SLEW of XLXI_316 is "SLOW"
   // synthesis attribute DRIVE of XLXI_316 is "12"
   Picoblaze_e XLXI_317 (.clk(clk60), 
                         .dcm_sts(XLXN_1034[7:0]), 
                         .in0(XLXN_821), 
                         .in1(XLXN_896), 
                         .in2(), 
                         .in3(), 
                         .in4(XLXN_824), 
                         .in5(XLXN_825), 
                         .in6(), 
                         .in7(), 
                         .in8(XLXN_828), 
                         .in9(XLXN_950), 
                         .in10(), 
                         .in11(), 
                         .in12(XLXN_832), 
                         .in13(XLXN_833), 
                         .in14(), 
                         .in15(), 
                         .irq(), 
                         .com_res(XLXN_1037), 
                         .en_ram(XLXN_1022), 
                         .irq_ack(XLXN_1025), 
                         .led(led), 
                         .out_p(XLXN_1023[7:0]), 
                         .p_id(XLXN_1024[7:0]), 
                         .q2(p_data_DUMMY[7:0]), 
                         .q3(p_adr_DUMMY[7:0]), 
                         .wr_str(XLXN_1026));
   com_20_status XLXI_318 (.a(XLXN_1032), 
                           .b(XLXN_1033[7:0]), 
                           .q(XLXN_1034[7:0]));
endmodule
