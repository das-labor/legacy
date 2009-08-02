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
// File             : lm32_interrupt.v
// Title            : Interrupt logic
// Dependencies     : lm32_include.v
// Version          : 6.1.17
// =============================================================================

`include "system_conf.v"
`include "lm32_include.v"

/////////////////////////////////////////////////////
// Module interface
/////////////////////////////////////////////////////

module lm32_interrupt (
    // ----- Inputs -------
    clk_i, 
    rst_i,
    // From external devices
    interrupt_n,
    // From pipeline
    stall_x,
`ifdef CFG_DEBUG_ENABLED
    non_debug_exception,
    debug_exception,
`else
    exception,
`endif
    eret_q_x,
`ifdef CFG_DEBUG_ENABLED
    bret_q_x,
`endif
    csr,
    csr_write_data,
    csr_write_enable,
    // ----- Outputs -------
    interrupt_exception,
    // To pipeline
    csr_read_data
    );

/////////////////////////////////////////////////////
// Parameters
/////////////////////////////////////////////////////

parameter interrupts = `CFG_INTERRUPTS;         // Number of interrupts

/////////////////////////////////////////////////////
// Inputs
/////////////////////////////////////////////////////

input clk_i;                                    // Clock
input rst_i;                                    // Reset

input [interrupts-1:0] interrupt_n;             // Interrupt pins, active-low
                           
input stall_x;                                  // Stall X pipeline stage

`ifdef CFG_DEBUG_ENABLED
input non_debug_exception;                      // Non-debug related exception has been raised
input debug_exception;                          // Debug-related exception has been raised
`else
input exception;                                // Exception has been raised
`endif
input eret_q_x;                                 // Return from exception 
`ifdef CFG_DEBUG_ENABLED
input bret_q_x;                                 // Return from breakpoint 
`endif

input [`LM32_CSR_RNG] csr;                      // CSR read/write index
input [`LM32_WORD_RNG] csr_write_data;          // Data to write to specified CSR
input csr_write_enable;                         // CSR write enable

/////////////////////////////////////////////////////
// Outputs
/////////////////////////////////////////////////////

output interrupt_exception;                     // Request to raide an interrupt exception
wire   interrupt_exception;

output [`LM32_WORD_RNG] csr_read_data;          // Data read from CSR
reg    [`LM32_WORD_RNG] csr_read_data;

/////////////////////////////////////////////////////
// Internal nets and registers 
/////////////////////////////////////////////////////

wire [interrupts-1:0] asserted;                 // Which interrupts are currently being asserted
wire [interrupts-1:0] interrupt_n_exception;

// Interrupt CSRs

reg ie;                                         // Interrupt enable
reg eie;                                        // Exception interrupt enable
`ifdef CFG_DEBUG_ENABLED
reg bie;                                        // Breakpoint interrupt enable
`endif
reg [interrupts-1:0] ip;                        // Interrupt pending
reg [interrupts-1:0] im;                        // Interrupt mask

/////////////////////////////////////////////////////
// Combinational Logic
/////////////////////////////////////////////////////

// Determine which interrupts have occured and are unmasked
assign interrupt_n_exception = ip & im;

// Determine if any unmasked interrupts have occured
assign interrupt_exception = (|interrupt_n_exception) & ie;

// Determine which interrupts are currently being asserted (active-low) or are already pending
assign asserted = ip | ~interrupt_n;
       
//assign ie_csr_read_data = {{`LM32_WORD_WIDTH-3{1'b0}}, 
//`ifdef CFG_DEBUG_ENABLED
//                           bie,
//`else
//                           1'b0,
//`endif                             
//                           eie, 
//                           ie
//                          };
wire   ip_csr_read_data = ip;
wire   im_csr_read_data = im;
// XXX JB XXX
// generate
//    if (interrupts > 1) 
//    begin
// CSR read
always @*
begin
    case (csr)
    `LM32_CSR_IE:  csr_read_data = {{`LM32_WORD_WIDTH-3{1'b0}}, 
`ifdef CFG_DEBUG_ENABLED
                                    bie,
`else
                                    1'b0,                                     
`endif
                                    eie, 
                                    ie
                                   };
    `LM32_CSR_IP:  csr_read_data = ip;
    `LM32_CSR_IM:  csr_read_data = im;
    default:       csr_read_data = {`LM32_WORD_WIDTH{1'bx}};
    endcase
end
// XXX JB XXX
//    end
//    else
//    begin
//// CSR read
//always @*
//begin
//    case (csr)
//    `LM32_CSR_IE:  csr_read_data = {{`LM32_WORD_WIDTH-3{1'b0}}, 
//`ifdef CFG_DEBUG_ENABLED
//                                    bie, 
//`else
//                                    1'b0,                                    
//`endif
//                                    eie, 
//                                    ie
//                                   };
//    `LM32_CSR_IP:  csr_read_data = ip;
//    default:       csr_read_data = {`LM32_WORD_WIDTH{1'bx}};
//    endcase
//end
//    end
//endgenerate
    
/////////////////////////////////////////////////////
// Sequential Logic
/////////////////////////////////////////////////////

// XXX JB XXX
//generate
//    if (interrupts > 1)
//    begin
// IE, IM, IP - Interrupt Enable, Interrupt Mask and Interrupt Pending CSRs
always @(posedge clk_i `CFG_RESET_SENSITIVITY)
begin
    if (rst_i == `TRUE)
    begin
        ie <= `FALSE;
        eie <= `FALSE;
`ifdef CFG_DEBUG_ENABLED
        bie <= `FALSE;
`endif
        im <= {interrupts{1'b0}};
        ip <= {interrupts{1'b0}};
    end
    else
    begin
        // Set IP bit when interrupt line is asserted
        ip <= asserted;
`ifdef CFG_DEBUG_ENABLED
        if (non_debug_exception == `TRUE)
        begin
            // Save and then clear interrupt enable
            eie <= ie;
            ie <= `FALSE;
        end
        else if (debug_exception == `TRUE)
        begin
            // Save and then clear interrupt enable
            bie <= ie;
            ie <= `FALSE;
        end
`else
        if (exception == `TRUE)
        begin
            // Save and then clear interrupt enable
            eie <= ie;
            ie <= `FALSE;
        end
`endif
        else if (stall_x == `FALSE)
        begin
            if (eret_q_x == `TRUE)
                // Restore interrupt enable
                ie <= eie;          
`ifdef CFG_DEBUG_ENABLED
            else if (bret_q_x == `TRUE)
                // Restore interrupt enable
                ie <= bie;
`endif
            else if (csr_write_enable == `TRUE)
            begin
                // Handle wcsr write
                if (csr == `LM32_CSR_IE)
                begin
                    ie <= csr_write_data[0];
                    eie <= csr_write_data[1];
`ifdef CFG_DEBUG_ENABLED
                    bie <= csr_write_data[2];
`endif
                end
                if (csr == `LM32_CSR_IM)
                    im <= csr_write_data[interrupts-1:0];
                if (csr == `LM32_CSR_IP)
                    ip <= asserted & ~csr_write_data[interrupts-1:0];
            end
        end
    end
end
//    end
//else
//    begin
//// IE, IM, IP - Interrupt Enable, Interrupt Mask and Interrupt Pending CSRs
//always @(posedge clk_i `CFG_RESET_SENSITIVITY)
//begin
//    if (rst_i == `TRUE)
//    begin
//        ie <= `FALSE;
//        eie <= `FALSE;
//`ifdef CFG_DEBUG_ENABLED
//        bie <= `FALSE;
//`endif
//        ip <= {interrupts{1'b0}};
//    end
//    else
//    begin
//        // Set IP bit when interrupt line is asserted
//        ip <= asserted;
//`ifdef CFG_DEBUG_ENABLED
//        if (non_debug_exception == `TRUE)
//        begin
//            // Save and then clear interrupt enable
//            eie <= ie;
//            ie <= `FALSE;
//        end
//        else if (debug_exception == `TRUE)
//        begin
//            // Save and then clear interrupt enable
//            bie <= ie;
//            ie <= `FALSE;
//        end
//`else
//        if (exception == `TRUE)
//        begin
//            // Save and then clear interrupt enable
//            eie <= ie;
//            ie <= `FALSE;
//        end
//`endif
//        else if (stall_x == `FALSE)
//        begin
//            if (eret_q_x == `TRUE)
//                // Restore interrupt enable
//                ie <= eie;          
//`ifdef CFG_DEBUG_ENABLED
//            else if (bret_q_x == `TRUE)
//                // Restore interrupt enable
//                ie <= bie;
//`endif
//            else if (csr_write_enable == `TRUE)
//            begin
//                // Handle wcsr write
//                if (csr == `LM32_CSR_IE)
//                begin
//                    ie <= csr_write_data[0];
//                    eie <= csr_write_data[1];
//`ifdef CFG_DEBUG_ENABLED
//                    bie <= csr_write_data[2];
//`endif
//                end
//                if (csr == `LM32_CSR_IP)
//                    ip <= asserted & ~csr_write_data[interrupts-1:0];
//            end
//        end
//    end
//end
//    end
//endgenerate

endmodule

