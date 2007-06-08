// ////////////////////////////////////////////////////////////////////
// >>>>>>>>>>>>>>>>>>>>>>>>> COPYRIGHT NOTICE <<<<<<<<<<<<<<<<<<<<<<<<<
// ////////////////////////////////////////////////////////////////////
// Copyright (c) 2001 by Lattice Semiconductor Corporation
// ////////////////////////////////////////////////////////////////////
//
// Permission:
//
//   Lattice Semiconductor grants permission to use this code for use
//   in synthesis for any Lattice programmable logic product.  Other
//   use of this code, including the selling or duplication of any
//   portion is strictly prohibited.
//
// Disclaimer:
//
//   This VHDL or Verilog source code is intended as a design reference
//   which illustrates how these types of functions can be implemented.
//   It is the user's responsibility to verify their design for
//   consistency and functionality through the use of formal
//   verification methods.  Lattice Semiconductor provides no warranty
//   regarding the use or functionality of this code.
//
// ////////////////////////////////////////////////////////////////////
//           
//                     Lattice Semiconductor Corporation
//                     5555 NE Moore Court
//                     Hillsboro, OR 97214
//                     U.S.A
//
//                     TEL: 1-800-Lattice (USA and Canada)
//                          408-826-6000 (other locations)
//
//                     web: http://www.latticesemi.com/
//                     email: techsupport@latticesemi.com
//
// ////////////////////////////////////////////////////////////////////
//
//  Project:           Universal Asynchronous Receiver Transmitter
//  File:              txmitt.v
//  Title:             txmitt
//  Design Library:    
//  Dependencies:      
//  Description:       Verilog file for the UART Transmitter Module
//
//    <Global reset and clock>
//      reset        : Master reset
//      clk16x       : UART internal clock
//
//    <Register>
//      thr         : Transmitter Holding Register
//
//    <Rising edge of thr write strobe>
//      thr_wr      : one clk16x width pulse indicating rising edge of ThrWRn_r
//
//    <Transmitter output>
//      sout        : Transmitter serial output
//
//    <Transmitter control>
//      databits     : "00"=5-bit, "01"=6-bit, "10"=7-bit, "11"=8-bit
//      stopbits    : "00"=1-bit, "01"=1.5-bit(5-bit data),
//                    "10"=2-bit(6,7,8-bit data)
//      parity_en   : '0'=Parity Bit Enable, '1'=Parity Bit Disable
//      parity_even : '0'=Even Parity Selected, '1'=Odd Parity Selected
//      parity_stick: '0'=Stick Parity Disable, '1'=Stick Parity Enable
//      tx_break    : '0'=Disable BREAK assertion, '1'=Assert BREAK
//
//    <Transmitter status>
//      thre        : thr is empty
//      temt        : Both thr and tsr are empty
//
// ////////////////////////////////////////////////////////////////////
`ifndef TXMITT_FILE
`define TXMITT_FILE
//`include "system_conf.v"
module txmitt #(parameter DATAWIDTH = 8) 
  (  
  reset,         // Global reset and clock
  clk16x,  
  clk,      
  thr,           // Register thr                    
  thr_wr,        // Rising edge of thr write strobe 
  sout,          // Transmitter output              
  databits,      // Transmitter control             
  stopbits,      
  parity_en,     
  parity_even,   
  parity_stick,  
  tx_break,      
  thre,          // Transmitter status  
  temt);

  input    reset ;
  input    clk16x;
  input    clk   ;
  input [DATAWIDTH-1 :0]   thr;
                 
  input                    thr_wr ;
  input [1:0]              databits;
  input [1:0]              stopbits;
  input                    parity_en;
  input                    parity_even ;
  input                    parity_stick; 
  input                    tx_break ;
                 
  output                   thre;
  output                   temt;
  output                   sout;
  
  reg                      tx_output;
  reg [DATAWIDTH-1 :0]     tsr;
  reg                      tx_parity;
  reg                      thr_empty;
  reg                      tsr_empty;
  reg                      tx_in_start_s;
  reg                      tx_in_shift_s;
  reg                      tx_in_stop_s;
  reg                      tx_in_start_s1; //tx_in_start_s delayed 1 Clk16X clock
  reg                      tx_in_shift_s1_fastclk; //tx_in_shift_s delayed 1 fast clock
  reg                      tx_in_stop_s1_fastclk; //tx_in_stop_s delayed 1 fast clock
  reg                      txclk_ena;
  reg                      txclk_enb;
  reg [2:0]                tx_cnt;
  reg [3:0]                count_v;
  
  // State Machine Definition
  parameter start        = 3'b000;
  parameter shift        = 3'b001;
  parameter parity       = 3'b010;
  parameter stop_1bit    = 3'b011;
  parameter stop_2bit    = 3'b100;
  parameter stop_halfbit = 3'b101;
  reg [2:0] tx_state;

   ////////////////////////////////////////////////////////////////////////////////
   // Transmitter Finite State Machine
   ////////////////////////////////////////////////////////////////////////////////
   always @(posedge clk16x or posedge reset) begin
     if (reset) begin
          tx_cnt    <= 0;
          tsr       <= 0;
          tx_output <= 1'b1;
          tx_parity <= 1'b1;
          tx_state  <= start; end 
     else begin
        case (tx_state)
          start: 
            if (!thr_empty && txclk_ena) begin
                 tsr <= thr;                 // Load data from THR to TSR
                 tx_parity <= ~parity_even;  // TxParity initialization
                 tx_output <= 1'b0;          // Start bit='0'
                 tx_cnt <= 0;
                 tx_state <= shift; end
            else 
                 tx_output <= 1'b1;      
          
          shift: 
            if (txclk_ena) begin
               tx_output <= tsr[0];          // output Data bits
               tsr <= {1'b0, tsr[7:1]};      // Shift serial data out
               tx_parity  <= tx_parity ^ tsr[0];
               tx_cnt <= tx_cnt + 1;
               if ((databits==2'b00 && tx_cnt==3'h4) || 
                   (databits==2'b01 && tx_cnt==3'h5) || 
                   (databits==2'b10 && tx_cnt==3'h6) || 
                   (databits==2'b11 && tx_cnt==3'h7))   
                   tx_state <= (parity_en) ? parity : stop_1bit;
               end 
          
          parity: 
            if (txclk_ena) begin
              tx_output <= (parity_stick) ? (~parity_even) : tx_parity;
              tx_state <= stop_1bit;
              end
          
          stop_1bit: 
            if (txclk_ena) begin
               tx_output  <= 1'b1;
               if (stopbits == 2'b00)      // 1 stop bit
                 tx_state <= start;
               else if (stopbits == 2'b01) // 1.5 stop bits(for 5-bit data only)
                 tx_state <= stop_halfbit;
               else
                 tx_state <= stop_2bit;    // 2 stop bits(for 6,7,8-bit data)
              end 
   
          stop_2bit:
            if (txclk_ena)  // Output 2nd bit of 2 Stopbits
              begin
                 tx_output <= 1'b1;
                 tx_state <= start;
              end
          
          stop_halfbit:
            if (txclk_enb)  // Half bit
              begin
                 tx_output <= 1'b1;
                 tx_state <= start;
              end
          
          default: tx_state <= start;
        endcase             
     end
   end

   ////////////////////////////////////////////////////////////////////////////////
   // Generate tsr_empty and thr_empty signals
   ////////////////////////////////////////////////////////////////////////////////

   // tsr_empty : will be set whenever tsr is empty
   always @(posedge clk or posedge reset) begin 
        if (reset)
          tsr_empty <= 1'b1;
        else if (tx_in_stop_s == 1'b1 && tx_in_stop_s1_fastclk == 'b0)
          tsr_empty <= 1'b1; // Set TsrEmpty flag to '1' when StopBit(s) is transmitted
        else if (tx_in_shift_s == 1'b1 && tx_in_shift_s1_fastclk == 1'b0)
          tsr_empty <= 1'b0; //Reset TsrEmpty flag to '0' when data is transferred from THR to TSR
     end
   
   
   // thr_empty : will be set whenever thr is empty
   always @(posedge clk or posedge reset) begin 
     if (reset)
       thr_empty <= 1'b1;
     else if (thr_wr)
       thr_empty <= 1'b0; // Reset ThrEmpty flag to '0' when data is written into THR by CPU   
     else if (tx_in_shift_s && !tx_in_shift_s1_fastclk)
       thr_empty <= 1'b1; // Set ThrEmpty flag to '1' when data is transferred from THR to TSR      
     end
   
   
   ////////////////////////////////////////////////////////////////////////////////
          // Delayed signals for edge detections
   ////////////////////////////////////////////////////////////////////////////////
   always @(posedge clk or posedge reset) begin
        if (reset) begin
          tx_in_shift_s1_fastclk <= 1'b1;
          tx_in_stop_s1_fastclk  <= 1'b1;
          end          
        else begin
          tx_in_shift_s1_fastclk <= tx_in_shift_s;
          tx_in_stop_s1_fastclk  <= tx_in_stop_s;
          end
     end

   always @(posedge clk16x or posedge reset) begin
        if (reset) begin
          tx_in_start_s1 <= 1'b1;
          end          
        else begin
          tx_in_start_s1 <= tx_in_start_s;
          end
     end
   
   ////////////////////////////////////////////////////////////////////////////////
   // Transmitter FSM state indication signals
   ////////////////////////////////////////////////////////////////////////////////
   
   // tx_in_shift_s : will be set whenever transmitter is in shift state
   always @(posedge clk or posedge reset) begin 
        if (reset)
          tx_in_shift_s <= 1'b0;
        else if (tx_state == shift)
          tx_in_shift_s <= 1'b1;
        else
          tx_in_shift_s <= 1'b0;
     end
   
   // tx_in_stop_s : will be set whenever transmitter is in stop_1bit state
   always @(posedge clk or posedge reset) begin 
        if (reset)
          tx_in_stop_s <= 1'b0;
        else if (tx_state == stop_1bit)
          tx_in_stop_s <= 1'b1;
        else
          tx_in_stop_s <= 1'b0;
     end
   
   
   // tx_in_start_s : will be set whenever transmitter is in start state
   always @(posedge clk16x or posedge reset) begin 
        if (reset)
          tx_in_start_s <= 1'b0;
        else if (tx_state == start)
          tx_in_start_s <= 1'b1;
        else
          tx_in_start_s <= 1'b0;
     end
   
   
   ////////////////////////////////////////////////////////////////////////////////
   // Generate txclk_ena/txclk_enb signals
   ////////////////////////////////////////////////////////////////////////////////
   
   // Transmitter operates at 1/16th of the input clock frequency.
   // txclk_ena, txclk_enb are clock enable signals used in transmitter FSM
   always @(posedge clk16x or posedge reset) begin 
        if (reset) begin
          count_v <= 4'hf;
          txclk_ena <= 1'b0;
          txclk_enb <= 1'b0;
          end
        else begin
        
          if (count_v == 4'b0000)
            txclk_ena <= 1'b1;
          else 
            txclk_ena <= 1'b0; 
            
          // TxClkEnB will be used only for 1.5 stop bit generation  
          if (count_v == 4'b1000)
            txclk_enb <= 1'b1;
          else
            txclk_enb <= 1'b0;
            
          if ((tx_in_start_s == 1'b1) && (tx_in_start_s1 == 1'b0))
            count_v <= 4'b0011; // Offset to the 2 cycle's delay
          else
            count_v <= count_v + 1'b1;             
          end
     end
   
   
 ////////////////////////////////////////////////////////////////////////////////
 // Generate thre/temt flags
 ////////////////////////////////////////////////////////////////////////////////
 
 // Transmitter Holding Register Empty Indicator
 assign thre = thr_empty;
 
 // Transmitter Empty Indicator is set to '1' whenever thr and tsr are
 // both empty, and reset to '0' when either thr or tsr contain a character
 assign temt =((thr_empty==1'b1) && (tsr_empty==1'b1)) ?  1'b1 : 1'b0;
 
 // Serial Data Output
 // If Break Control bit is set to 1, the serial output is forced to Zero
 assign sout = (tx_break==1'b1) ? 1'b0 : tx_output;
   
endmodule
`endif // TXMITT_FILE
