// --------------------------------------------------------------------
// >>>>>>>>>>>>>>>>>>>>>>>>> COPYRIGHT NOTICE <<<<<<<<<<<<<<<<<<<<<<<<<
// --------------------------------------------------------------------
// Copyright (c) 2001 by Lattice Semiconductor Corporation
// --------------------------------------------------------------------
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
// --------------------------------------------------------------------
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
// --------------------------------------------------------------------
//
//  Project:           Universal Asynchronous Receiver Transmitter
//  File:              modem.v
//  Title:             modem
//  Design Library:    
//  Dependencies:      
//  Description:       Verilog file for the Modem Interface
//
//    <Global reset and clock>
//      reset     : Master reset
//      clk16x    : Master clock
//
//    <Registers>
//      msr        : Modem Status Register
//      mcr        : Modem Control Register
//
//    <Rising edge of registers read/write strobes>
//      msr_rd     : one Clk16X width pulse indicating rising edge of MsrRDn_r
//
//    <Modem interface>
//      dcd_n        : Data Carrier Detect
//      cts_n        : Clear To Send
//      dsr_n        : Data Set Ready
//      ri_n         : Ring Indicator
//      dtr_n        : Data Terminal Ready
//      rts_n        : Request To Send
//
// --------------------------------------------------------------------


`ifndef MODEM_FILE
`define MODEM_FILE
`include "system_conf.v"
module modem #(parameter DATAWIDTH = 8)
            (
             // Global reset and clock
             reset      , // Master reset
             clk16x     , // UART internal clock
             // Registers
             msr        , // Modem Status Reg
             mcr        , // Modem Control Reg
             // Rising Edge of msr Read Strobe
             msr_rd     , // pulse indicating rising of MsrRDn_r
             // Modem interface
             dcd_n      , // Data Carrier Detect
             cts_n      , // Clear To Send
             dsr_n      , // Data Set Ready
             ri_n       , // Ring Indicator
             dtr_n      , // Data Terminal Ready
             rts_n        // Request To Send
           );
           
 
  input                  reset   ;
  input                  clk16x  ;
  input [1:0]            mcr     ;
             
  input                  msr_rd  ;
                         
  input                  dcd_n   ;
  input                  cts_n   ;
  input                  dsr_n   ;
  input                  ri_n    ;     
  output [DATAWIDTH-1:0] msr     ; 
             
  output                 dtr_n   ;
  output                 rts_n   ;

  reg    [DATAWIDTH-1:0] msr_reg ;
  reg                    ctsn_d1 ;
  reg                    dsrn_d1 ;
  reg                    dcdn_d1 ;
  reg                    rin_d1  ;
  
  assign dtr_n = ~mcr[0];
  assign rts_n = ~mcr[1];

////////////////////////////////////////////////////////////////////////////////
//      Modem Status Register Setup
////////////////////////////////////////////////////////////////////////////////

  assign msr = msr_reg;

  always @(posedge clk16x or posedge reset)  begin
    if (reset) begin
        msr_reg <= 0;
        ctsn_d1 <= 1'b1;
        dsrn_d1 <= 1'b1;
        dcdn_d1 <= 1'b1;
        rin_d1  <= 1'b1;
        end
    else  begin
        ctsn_d1 <= cts_n;
        dsrn_d1 <= dsr_n;
        dcdn_d1 <= dcd_n;
        rin_d1  <= ri_n ;
        if (msr_rd) 
          msr_reg <= 0;
        else begin
          msr_reg[0] <= msr_reg[0] | (ctsn_d1 ^ cts_n); // Indicate CTSn changes
          msr_reg[1] <= msr_reg[1] | (dsrn_d1 ^ dsr_n); // Indicate DSRn changes
          msr_reg[2] <= msr_reg[2] | (~rin_d1 & ri_n ); // Rising edge of RI    
          msr_reg[3] <= msr_reg[3] | (dcdn_d1 ^ dcd_n); // Indicate DCDn changes
          msr_reg[4] <= !cts_n;  // Compliment of CTSn
          msr_reg[5] <= !dsr_n;  // Compliment of DSRn
          msr_reg[6] <= !ri_n ;  // Compliment of RIn 
          msr_reg[7] <= !dcd_n;  // Compliment of DCDn
          end
        end
     end

endmodule
`endif // MODEM_FILE
