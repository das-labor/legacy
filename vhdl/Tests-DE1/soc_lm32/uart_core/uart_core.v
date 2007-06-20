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
//  File:              uart_top.v
//  Title:             uart_top
//  Design Library:    
//  Dependencies:      
//  Description:       TOP verilog file for the UART design
//
//    <Global reset and clock>
//      RESET     : Master reset
//      CLK       : Master clock
//
//    <wishbone interface >
//      UART_ADR_I     : Address bus
//      UART_DAT_I     : Data bus input
//      UART_DAT_O     : Data but output
//      UART_STB_I     : strobe output, used to indicate a valid data transfer cycle
//      UART_CYC_I     : cycle signal, indicate whether a valid bus cycle is in process 
//      UART_WE_I      : write enable output, used to indicate whether the current cycle is a READ or Write
//      UART_SEL_I     : output array select signal, used to indicate where valid data is expected
//      UART_ACK_O     : acknowledge, indicate the nomal termination of a bus cycle
//      INTR           : Interrupt
//      UART_CTI_I     : cycle type identifier, 
//      UART_BTE_I     : burst type extension
//    <Receiver interface>
//      SIN    : Receiver serial input
//      RXRDY_N : Receiver ready, low active when RBR data is available
//
//    <Transmitter interface>
//      SOUT   : Transmitter serial output
//      TXRDY_N : Transmitter ready, low active when THR is empty
//
//    <Modem interface>
//      DCD_N   : Data Carrier Detect
//      CTS_N   : Clear To Send
//      DSR_N   : Data Set Ready
//      RI_N    : Ring Indicator
//      DTR_N   : Data Terminal Ready
//      RTS_N   : Request To Send
//
// ////////////////////////////////////////////////////////////////////
`ifndef UART_CORE_FILE
`define UART_CORE_FILE
//`include "system_conf.v"
//`timescale 1ns/10ps
module uart_core
      #(parameter CLK_IN_MHZ = 25,
        parameter BAUD_RATE  = 57600,
        parameter ADDRWIDTH = 5,
        parameter DATAWIDTH = 8
       )
      (
       // Global reset and clock
       RESET    ,
       CLK      ,
      
       // wishbone interface
       UART_ADR_I   ,
       UART_DAT_I   ,
       UART_DAT_O   ,
       UART_STB_I   ,
       UART_CYC_I   ,
       UART_WE_I    ,
       UART_SEL_I   ,
       UART_CTI_I   ,
       UART_BTE_I   ,
       UART_LOCK_I  ,
       UART_ACK_O   ,
       UART_RTY_O   ,
       UART_ERR_O,
       INTR     ,
      
       // Receiver interface
       SIN      ,
       RXRDY_N  ,
      
       // Modem interface
       `ifdef MODEM
       DCD_N    ,
       CTS_N    ,
       DSR_N    ,
       RI_N     ,
       DTR_N    ,
       RTS_N    ,      
       `endif
        
       // Transmitter interface
       SOUT     ,
       TXRDY_N        
       );

 input                RESET  ;
 input                CLK    ;

 input [31:0]         UART_ADR_I;
 input [31:0]         UART_DAT_I;
 input                SIN ;
                      
 input                UART_STB_I ;
 input                UART_CYC_I ;
 input                UART_WE_I;
 input                UART_LOCK_I;
 input [1:0]          UART_BTE_I;
 input [3:0]          UART_SEL_I;
 input [2:0]          UART_CTI_I;
  `ifdef MODEM        
 input                DCD_N;
 input                CTS_N;
 input                DSR_N ;
 input                RI_N;     
 `endif               
 output[31:0]         UART_DAT_O;
 output               UART_ACK_O;
 output               UART_RTY_O;
 output               UART_ERR_O;
 output               INTR;
                      
 output               RXRDY_N;
 output               SOUT;
 output               TXRDY_N;
`ifdef MODEM          
 output               DTR_N;
 output               RTS_N;
 wire [DATAWIDTH-1:0] MSR;
 wire [1:0]           MCR;  
 `endif
 
 wire [DATAWIDTH-1:0] RBR;
 wire [DATAWIDTH-1:0] THR;
 wire [1:0]           databits;
 wire [1:0]           stopbits;
 wire                 parity_en;
 wire                 parity_stick;
 wire                 tx_break;
 wire                 thr_wr;
 wire                 rbr_rd;
 wire                 lsr_rd;
 wire                 rx_rdy;
 wire                 parity_err;
 wire                 frame_err;
 wire                 overrun_err;
 wire                 break_int;
 wire                 THRE;
 wire                 TEMT; 
 wire[31:0]           UART_DAT_O; 
                      
 wire 		      CLK16X;
 
 assign               UART_RTY_O = 1'b0;
 assign               UART_ERR_O = 1'b0;
   
 `ifdef MODEM
     intface  #(.CLK_IN_MHZ(CLK_IN_MHZ),
                .BAUD_RATE(BAUD_RATE),
                .ADDRWIDTH(ADDRWIDTH),
                .DATAWIDTH(DATAWIDTH)) 
         u_intface (
        .reset        (RESET       ),
        .clk          (CLK         ),
        .clk16x       (CLK16X      ),
        .adr_i        (UART_ADR_I  ),
        .dat_i        (UART_DAT_I  ),
        .dat_o        (UART_DAT_O  ),
        .stb_i        (UART_STB_I  ),
        .cyc_i        (UART_CYC_I  ),
        .we_i         (UART_WE_I   ),
        .sel_i        (UART_SEL_I  ),
        .cti_i        (UART_CTI_I  ),
        .bte_i        (UART_BTE_I  ),
        .ack_o        (UART_ACK_O  ),
        .intr         (intr        ),
        .rbr          (RBR         ),
        .thr          (THR         ),
        .rbr_rd       (rbr_rd      ),
        .thr_wr       (thr_wr      ),
        .lsr_rd       (lsr_rd      ),
        .msr_rd       (msr_rd      ),
        .msr          (MSR         ),
        .mcr          (MCR         ),
        .databits     (databits    ),
        .stopbits     (stopbits    ),
        .parity_en    (parity_en   ),
        .parity_even  (parity_even ),
        .parity_stick (parity_stick),
        .tx_break     (tx_break    ),
        .rx_rdy       (rx_rdy      ),
        .overrun_err  (overrun_err ),
        .parity_err   (parity_err  ),
        .frame_err    (frame_err   ),
        .break_int    (break_int   ),
        .thre         (THRE        ),
        .temt         (TEMT        )
  );
`else
     intface  #(.CLK_IN_MHZ(CLK_IN_MHZ),
                .BAUD_RATE(BAUD_RATE),
                .ADDRWIDTH(ADDRWIDTH),
                .DATAWIDTH(DATAWIDTH)) 
          u_intface (
        .reset        (RESET         ),
        .clk          (CLK           ),
        .clk16x       (CLK16X        ),
        .adr_i        (UART_ADR_I    ),
        .dat_i        (UART_DAT_I    ),
        .dat_o        (UART_DAT_O    ),
        .stb_i        (UART_STB_I    ),
        .cyc_i        (UART_CYC_I    ),
        .we_i         (UART_WE_I     ),
        .sel_i        (UART_SEL_I    ),
        .cti_i        (UART_CTI_I    ),
        .bte_i        (UART_BTE_I    ),
        .ack_o        (UART_ACK_O    ),
        .intr         (intr          ),
        .rbr          (RBR           ),
        .thr          (THR           ),
        .rbr_rd       (rbr_rd        ),
        .thr_wr       (thr_wr        ),
        .lsr_rd       (lsr_rd        ),
        .databits     (databits      ),
        .stopbits     (stopbits      ),
        .parity_en    (parity_en     ),
        .parity_even  (parity_even   ),
        .parity_stick (parity_stick  ),
        .tx_break     (tx_break      ),
        .rx_rdy       (rx_rdy        ),
        .overrun_err  (overrun_err   ),
        .parity_err   (parity_err    ),
        .frame_err    (frame_err     ),
        .break_int    (break_int     ),
        .thre         (THRE          ),
        .temt         (TEMT          )
  );
`endif
   
   
  rxcver #(.DATAWIDTH(DATAWIDTH)) u_rxcver 
        (
        .reset        (RESET         ),
        .clk          (CLK          ),
        .clk16x       (CLK16X        ),
        .rbr          (RBR           ),
        .rbr_rd       (rbr_rd        ),
        .lsr_rd       (lsr_rd        ),
        .sin          (SIN           ),
        .databits     (databits      ),   
        .parity_en    (parity_en     ),
        .parity_even  (parity_even   ),
        .parity_stick (parity_stick  ),
        .rx_rdy       (rx_rdy        ),
        .overrun_err  (overrun_err   ),
        .parity_err   (parity_err    ),
        .frame_err    (frame_err     ),
        .break_int    (break_int     )
  ); 
  
  txmitt #(.DATAWIDTH(DATAWIDTH)) u_txmitt 
        (
        .reset        (RESET         ),
        .clk16x       (CLK16X        ),
        .clk          (CLK           ),
        .thr          (THR           ),
        .thr_wr       (thr_wr        ),
        .sout         (sout          ),
        .databits     (databits      ),
        .stopbits     (stopbits      ),
        .parity_en    (parity_en     ),
        .parity_even  (parity_even   ),
        .parity_stick (parity_stick  ),
        .tx_break     (tx_break      ),
        .thre         (THRE          ),
        .temt         (TEMT          )
  );
  
 `ifdef MODEM
  modem #(.DATAWIDTH(DATAWIDTH)) u_modem 
        ( 
        .reset        (RESET         ), 
        .clk16x       (CLK16X        ), 
        .msr          (MSR           ), 
        .mcr          (MCR           ),
        .msr_rd       (msr_rd        ), 
        .dcd_n        (DCD_N         ), 
        .cts_n        (CTS_N         ), 
        .dsr_n        (DSR_N         ), 
        .ri_n         (RI_N          ), 
        .dtr_n        (dtr_n         ), 
        .rts_n        (rts_n         )  
  );
  `endif

  // TXRDY_N, RXRDY_N is low active output
  assign #5 TXRDY_N = ~THRE;
  assign #5 RXRDY_N = ~rx_rdy; 
  assign #5 INTR    = intr;
  assign #5 SOUT    = sout;
  `ifdef MODEM
  assign #5 DTR_N   = dtr_n;
  assign #5 RTS_N   = rts_n;
  `endif 
endmodule
`endif // UART_CORE_FILE
