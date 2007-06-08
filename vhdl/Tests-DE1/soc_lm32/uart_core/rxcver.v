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
//  File:              rxcver.v
//  Title:             rxcver
//  Design Library:    
//  Dependencies:      
//  Description:       Verilog file for the UART Receiver Module
//
//    <Global reset and clock>
//      reset      : Master reset
//	    clk        : Master clock
//      clk16x     : UART internal clock
//
//    <Register>
//      rbr        : Receiver Buffer Register
//
//    <Rising edge of rbr, lsr read strobes>
//      rbr_rd     : one CPU clk width pulse indicating rising edge of rbr Read
//      lsr_rd     : one CPU clk width pulse indicating rising edge of lsr Read
//
//    <Receiver input>
//      sin         : Receiver serial input
//
//    <Receiver control>
//      databits     : "00"=5-bit, "01"=6-bit, "10"=7-bit, "11"=8-bit
//      parity_en    : '0'=Parity Bit Enable, '1'=Parity Bit Disable
//      parity_even  : '0'=Even Parity Selected, '1'=Odd Parity Selected
//      parity_stick : '0'=Stick Parity Disable, '1'=Stick Parity Enable
//
//    <Receiver/Transmitter status>
//      rx_rdy       : rbr data is ready to be read
//      overrun_err  : Overrun error
//      parity_err   : Parity error
//      frame_err    : Frame error
//      break_int    : BREAK interrupt
//
// ////////////////////////////////////////////////////////////////////
`ifndef RXCVER_FILE
`define RXCVER_FILE
//`include "system_conf.v"
module rxcver #(parameter DATAWIDTH=8)(
     // Global reset and clock
     reset,
     clk,
     clk16x,
     // Register
     rbr,
     // Rising edge of rbr, lsr read strobes(One clk16x cycle)
     rbr_rd,
     lsr_rd,
     // Receiver input
     sin,
     // Receiver control
     databits,
     parity_en,
     parity_even,
     parity_stick,
     // Receiver status
     rx_rdy,
     overrun_err,
     parity_err,
     frame_err,
     break_int    
     );
   
   input         reset   ;
   input         clk     ;
   input         clk16x  ;
   input         rbr_rd ;
   input         lsr_rd ;
   input         sin    ;
   input [1:0]   databits ;
   input         parity_en;
   input         parity_even;
   input         parity_stick;
   
   output [DATAWIDTH-1:0] rbr ;             
   output 		   rx_rdy      ;
   output 		   overrun_err ;
   output 		   parity_err  ;
   output 		   frame_err   ;
   output 		   break_int   ;
   
   reg [3:0] 	databit_recved_num;
   reg [DATAWIDTH-1:0] rsr;
   reg 			   rx_parity_err  ;
   reg 			   rx_frame_err ;
   reg 			   rx_idle;
   reg 			   rbr_datardy;
   reg [3:0] 	 count;
   reg 			   hunt;
   reg 			   hunt_one;
   reg 			   sin_d1;
   reg 			   rx_frame_err_d1;
   reg 			   rx_idle_d1;
   reg 			   overrun_err;
   reg 			   parity_err;
   reg 			   frame_err;
   reg 			   break_int;
   reg 			   sampled_once;
   reg 			   rxclk_en;    
   reg [DATAWIDTH-1:0] rbr;
   
   // State Machine Definition
   parameter 	idle   = 2'b00;
   parameter 	shift  = 2'b01;
   parameter 	parity = 2'b10;
   parameter 	stop   = 2'b11;
   reg [1:0]  cs_state;
   
   ////////////////////////////////////////////////////////////////////////////////
   // Generate rxclk_en signal
   ////////////////////////////////////////////////////////////////////////////////
   
   // rxclk_en : serial port data receiving clock enable
   always @(posedge clk16x or posedge reset) begin 
        if (reset)
          rxclk_en <= 1'b0;
        else if(count == 4'b0110)
          rxclk_en <= 1'b1;
        else
          rxclk_en <= 1'b0;
     end
   
   
   // count : 4-bit counter for rxclk_en waveform generation
   always @(posedge clk16x or posedge reset) begin
        if (reset)
          count <= 4'h0;
        else if (cs_state !== idle || hunt)
          // Increment count when not idle or when Hunt_r is TRUE
          count <= count + 1'b1;
        else if (sampled_once)
          // Adjust 2 clks forward for RxClkEn during the resync after framing error
          count <= 4'b0010;
        else
          count <= 4'b0000;
     end
   
   
   ////////////////////////////////////////////////////////////////////////////////
   // Generate hunt
   ////////////////////////////////////////////////////////////////////////////////
   
   // hunt : will be TRUE when start bit is found
   always @(posedge clk16x or posedge reset) begin 
     if (reset)
       hunt <= 1'b0;
     else if ((cs_state == idle) && (sin == 1'b0) && (sin_d1 == 1'b1))
       // Set Hunt when SIN falling edge is found at the idle state
       hunt <= 1'b1;
     else if (sampled_once && ~sin)
        // Start bit is successfully sampled twice after framing error
        // set Hunt_r "true" for resynchronizing of next frame
       hunt <= 1'b1;
     else if (~rx_idle || sin)
       hunt <= 1'b0;
     end
   
   
   // hunt_one :
   //   hunt_one, used for BI flag generation, indicates that there is at
   //   least a '1' in the (data + parity + stop) bits of the frame.
   //   Break Interrupt flag(BI) is set to '1' whenever the received input
   //   is held at the '0' state for all bits in the frame (Start bit +
   //   Data bits + Parity bit + Stop bit).  So, as long as hunt_one is still
   //   low after all bits are received, BI will be set to '1'.
   always @(posedge clk16x or posedge reset) begin 
        if (reset)
          hunt_one <= 1'b0;
        else if (hunt)
          hunt_one <= 1'b0;
        else if ((rx_idle == 1'b0) && (count[3] == 1'b1) && (sin == 1'b1))
          hunt_one <= 1'b1;
     end
   
   // rbr_datardy :
   // This will be set to indicate that the data in rbr is ready for read and
   // will be cleared after rbr is read.
   always @(posedge clk or posedge reset) begin
        if (reset) 
          rbr_datardy <= 1'b0;
        else  
          if (rbr_rd)
            // clear RbrDataRDY when RBR is read by CPU
            rbr_datardy <= 1'b0; 
          else if ((rx_idle == 1'b1) && (rx_idle_d1 == 1'b0))
            // set RbrDataRDY at RxIdle_r rising edge
            rbr_datardy <= 1'b1;
     end
   
   
   // sampled_once :
   //   This will be set for one clk16x clock after a framing error occurs not
   //   because of BREAK and a low sin signal is sampled by the clk16x right
   //   after the sample time of the Stop bit which causes the framing error.
   always @ (posedge clk16x or posedge reset)  begin 
        if (reset)
          sampled_once <= 1'b0;
        else if (rx_frame_err && ~rx_frame_err_d1 && ~sin && hunt_one)
          // Start bit got sampled once
          sampled_once <= 1'b1;
        else
          sampled_once <= 1'b0;
     end
   
   
   // rx_idle Flag
   always @ (posedge clk16x or posedge reset)  begin 
        if (reset)
          rx_idle <= 1'b1;                               
        else if (cs_state == idle)
          rx_idle <= 1'b1;
        else if (count[3] == 1'b1)
          rx_idle <= 1'b0;
     end
   
   
   ////////////////////////////////////////////////////////////////////////////////
   // Receiver Finite State Machine
   ////////////////////////////////////////////////////////////////////////////////
   //  rx_parity_err:
   //               rx_parity_err is a dynamic Parity Error indicator which is
   //               initialized to 0 for even parity and 1 for odd parity.
   //               For odd parity, if there are odd number of '1's in the
   //               (data + parity) bits, the XOR will bring rx_parity_err back to 0
   //               which means no parity error, otherwise rx_parity_err will be 1 to
   //               indicate a parity error.
   // parity_stick='1' means Stick Parity is enabled.  In this case,
   //               the accumulated dynamic rx_parity_err result will be ignored.  A new
   //               value will be assigned to rx_parity_err based on the even/odd parity
   //               mode setting and the sin sampled in parity bit.
   //                  parity_even='0'(odd parity):
   //                     sin needs to be '1', otherwise it's a stick parity error.
   //                  parity_even='1'(even parity):
   //                     sin needs to be '0', otherwise it's a stick parity error.
   
   always @ (posedge clk16x or posedge reset) begin 
        if (reset) begin
           rsr <= 0;
           databit_recved_num <= 4'h0;
           rx_parity_err <= 1'b1;
           rx_frame_err <= 1'b0;
           cs_state <= idle;
           end   
        else case (cs_state)
           idle: 
               if (rx_idle && !sin && rxclk_en) begin
                 rsr <= 0;
                 databit_recved_num <= 4'h0;
                 rx_parity_err <= ~ parity_even;
                 rx_frame_err <= 1'b0;
                 cs_state <= shift; 
                 end
           shift: 
               if (rxclk_en) begin
                 rsr <= {sin, rsr[7:1]};
                 rx_parity_err <= rx_parity_err ^ sin;
                 databit_recved_num <= databit_recved_num + 1;
                 if ((databits==2'b00 && databit_recved_num == 4'h4) ||
                     (databits==2'b01 && databit_recved_num == 4'h5) ||
                     (databits==2'b10 && databit_recved_num == 4'h6) ||
                     (databits==2'b11 && databit_recved_num == 4'h7))  
                      if (parity_en == 1'b0)
                        cs_state <= stop;
                      else
                        cs_state <= parity; 
                 end
           parity: 
              if (rxclk_en) begin
                if (parity_stick == 1'b0)
                   rx_parity_err <= rx_parity_err ^ sin;
                else
                   if (parity_even == 1'b0)
                      rx_parity_err <= ~sin;
                   else
                      rx_parity_err <= sin;
                cs_state <= stop;
                end
           stop:   
             if (rxclk_en) begin
                 // The Receiver checks the 1st Stopbit only regardless of the number
                 // of Stop bits selected.
                 // Stop bit needs to be '1', otherwise it's a Framing error
                 rx_frame_err <= ~sin;
                 cs_state     <= idle;
                 end
          default: 
             if (rxclk_en)
                cs_state <= idle;
          endcase
     end
   
      
   ////////////////////////////////////////////////////////////////////////////////
			// Receiver Buffer Register
   ////////////////////////////////////////////////////////////////////////////////
   always @(posedge clk or posedge reset) begin 
        if (reset)
          rbr <= 0;
        else if ((rx_idle == 1'b1) && (rx_idle_d1 == 1'b0))
          case (databits)
            2'b00: rbr <= { 3'b000, rsr[7:3]};
            2'b01: rbr <= { 2'b00 , rsr[7:2]};
            2'b10: rbr <= { 1'b0  , rsr[7:1]};
            default: rbr <= rsr;
          endcase
     end
   
   
   ////////////////////////////////////////////////////////////////////////////////
   // Delayed Signals for edge detections
   ////////////////////////////////////////////////////////////////////////////////
   always @(posedge clk16x or posedge reset) begin 
        if (reset) begin
          sin_d1 <= 1'b0;
          rx_frame_err_d1 <= 1'b1;
          end
        else begin
          //sin_d1 : Signal for falling edge detection of signal SIN
          sin_d1 <= sin;
          // rx_frame_err_d1 :
          // a delayed version of rx_frame_err for detacting the rising edge
          // used to resynchronize the next frame after framing error
          rx_frame_err_d1 <= rx_frame_err;
          end
     end 
   
   always @(posedge clk or posedge reset) begin 
        if (reset) begin
          rx_idle_d1 <= 1'b1;
          end
        else begin
          // rx_idle_d1 : Signal for rising edge detection of signal rx_idle
          rx_idle_d1 <= rx_idle;
          end
     end 
        
   ////////////////////////////////////////////////////////////////////////////////
   // Generate Error Flags
   ////////////////////////////////////////////////////////////////////////////////
   
   // Receiver Error Flags in lsr
   //   overrun_err(OE), parity_err(PE), frame_err(FE), break_int(BI)
   //   will be set to reflect the sin line status only after the whole frame
   //   (Start bit + Data bits + Parity bit + Stop bit) is received.  A rising
   //   edge of rx_idle indicates the whole frame is received.
   always @(posedge clk or posedge reset) begin 
        if (reset) begin
          overrun_err <= 1'b0;
          parity_err  <= 1'b0;
          frame_err   <= 1'b0;
          break_int   <= 1'b0;
          end               
        else if (rx_idle && !rx_idle_d1) begin  // update at rxidle rising
          // Set overrun_err flag if RBR data is still not read by CPU
          overrun_err <= rbr_datardy;   
          // Set parity_err flag if rx_parity_err is 1 when Parity enable
          parity_err  <= (parity_err | rx_parity_err) & parity_en; 
          // Set frame_err flag if rx_frame_err is 1(Stop bit is sampled low)
          frame_err   <= frame_err | rx_frame_err;    
          // Set break_int flag if hunt_one is still low
          break_int   <= break_int | (~ hunt_one);  
          end
        else if (lsr_rd) begin  // clear when LSR is read
          parity_err  <= 1'b0;
          frame_err   <= 1'b0;
          overrun_err <= 1'b0;
          break_int   <= 1'b0;
          end
     end
   
   // Receiver ready for read when data is available in rbr
   assign  rx_rdy = rbr_datardy;

endmodule
`endif // RXCVER_FILE
