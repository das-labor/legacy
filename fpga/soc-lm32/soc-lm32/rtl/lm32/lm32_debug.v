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
// File             : lm32_debug.v
// Title            : Hardware debug registers and associated logic.
// Dependencies     : lm32_include.v
// Version          : 6.1.17
// =============================================================================

`include "lm32_include.v"

`ifdef CFG_DEBUG_ENABLED

// States for single-step FSM
`define LM32_DEBUG_SS_STATE_RNG                 2:0
`define LM32_DEBUG_SS_STATE_IDLE                3'b000
`define LM32_DEBUG_SS_STATE_WAIT_FOR_RET        3'b001
`define LM32_DEBUG_SS_STATE_EXECUTE_ONE_INSN    3'b010
`define LM32_DEBUG_SS_STATE_RAISE_BREAKPOINT    3'b011
`define LM32_DEBUG_SS_STATE_RESTART             3'b100

/////////////////////////////////////////////////////
// Module interface
/////////////////////////////////////////////////////

module lm32_debug (
    // ----- Inputs -------
    clk_i, 
    rst_i,
    pc_x,
    load_x,
    store_x,
    load_store_address_x,
    csr_write_enable_x,
    csr_write_data,
    csr_x,
`ifdef CFG_HW_DEBUG_ENABLED
    jtag_csr_write_enable,
    jtag_csr_write_data,
    jtag_csr,
`endif
`ifdef LM32_SINGLE_STEP_ENABLED
    eret_q_x,
    bret_q_x,
    stall_x,
    exception_x,
    q_x,
`ifdef CFG_DCACHE_ENABLED
    dcache_refill_request,
`endif
`endif
    // ----- Outputs -------
`ifdef LM32_SINGLE_STEP_ENABLED
    dc_ss,
`endif
    dc_re,
    bp_match,
    wp_match
    );
    
/////////////////////////////////////////////////////
// Parameters
/////////////////////////////////////////////////////

parameter breakpoints = 0;                      // Number of breakpoint CSRs
parameter watchpoints = 0;                      // Number of watchpoint CSRs

/////////////////////////////////////////////////////
// Inputs
/////////////////////////////////////////////////////

input clk_i;                                    // Clock
input rst_i;                                    // Reset

input [`LM32_PC_RNG] pc_x;                      // X stage PC
input load_x;                                   // Load instruction in X stage
input store_x;                                  // Store instruction in X stage
input [`LM32_WORD_RNG] load_store_address_x;    // Load or store effective address
input csr_write_enable_x;                       // wcsr instruction in X stage
input [`LM32_WORD_RNG] csr_write_data;          // Data to write to CSR
input [`LM32_CSR_RNG] csr_x;                    // Which CSR to write
`ifdef CFG_HW_DEBUG_ENABLED
input jtag_csr_write_enable;                    // JTAG interface CSR write enable
input [`LM32_WORD_RNG] jtag_csr_write_data;     // Data to write to CSR
input [`LM32_CSR_RNG] jtag_csr;                 // Which CSR to write
`endif
`ifdef LM32_SINGLE_STEP_ENABLED
input eret_q_x;                                 // eret instruction in X stage
input bret_q_x;                                 // bret instruction in X stage
input stall_x;                                  // Instruction in X stage is stalled
input exception_x;                              // An exception has occured in X stage 
input q_x;                                      // Indicates the instruction in the X stage is qualified
`ifdef CFG_DCACHE_ENABLED
input dcache_refill_request;                    // Indicates data cache wants to be refilled 
`endif
`endif

/////////////////////////////////////////////////////
// Outputs
/////////////////////////////////////////////////////

`ifdef LM32_SINGLE_STEP_ENABLED
output dc_ss;                                   // Single-step enable
reg    dc_ss;
`endif
output dc_re;                                   // Remap exceptions
reg    dc_re;
output bp_match;                                // Indicates a breakpoint has matched
wire   bp_match;        
output wp_match;                                // Indicates a watchpoint has matched
wire   wp_match;

/////////////////////////////////////////////////////
// Internal nets and registers 
/////////////////////////////////////////////////////

genvar i;                                       // Loop index for generate statements

// Debug CSRs

reg [`LM32_PC_RNG] bp_a[0:breakpoints-1];       // Instruction breakpoint address
reg bp_e[0:breakpoints-1];                      // Instruction breakpoint enable
wire [0:breakpoints-1]bp_match_n;               // Indicates if a h/w instruction breakpoint matched

reg [`LM32_WPC_C_RNG] wpc_c[0:watchpoints-1];   // Watchpoint enable
reg [`LM32_WORD_RNG] wp[0:watchpoints-1];       // Watchpoint address
wire [0:watchpoints-1]wp_match_n;               // Indicates if a h/w data watchpoint matched

wire debug_csr_write_enable;                    // Debug CSR write enable (from either a wcsr instruction of external debugger)
wire [`LM32_WORD_RNG] debug_csr_write_data;     // Data to write to debug CSR
wire [`LM32_CSR_RNG] debug_csr;                 // Debug CSR to write to

`ifdef LM32_SINGLE_STEP_ENABLED
// FIXME: Declaring this as a reg causes ModelSim 6.1.15b to crash, so use integer for now
//reg [`LM32_DEBUG_SS_STATE_RNG] state;           // State of single-step FSM
integer state;                                  // State of single-step FSM
`endif

/////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////

`include "lm32_functions.v"

/////////////////////////////////////////////////////
// Combinational Logic
/////////////////////////////////////////////////////

// Check for breakpoints
generate
    for (i = 0; i < breakpoints; i = i + 1)
    begin : bp_comb
assign bp_match_n[i] = ((bp_a[i] == pc_x) && (bp_e[i] == `TRUE));
    end
endgenerate
generate 
`ifdef LM32_SINGLE_STEP_ENABLED
    if (breakpoints > 0) 
assign bp_match = (|bp_match_n) || (state == `LM32_DEBUG_SS_STATE_RAISE_BREAKPOINT);
    else
assign bp_match = state == `LM32_DEBUG_SS_STATE_RAISE_BREAKPOINT;
`else
    if (breakpoints > 0) 
assign bp_match = |bp_match_n;
    else
assign bp_match = `FALSE;
`endif
endgenerate    
               
// Check for watchpoints
generate 
    for (i = 0; i < watchpoints; i = i + 1)
    begin : wp_comb
assign wp_match_n[i] = (wp[i] == load_store_address_x) && ((load_x & wpc_c[i][0]) | (store_x & wpc_c[i][1]));
    end               
endgenerate
generate
    if (watchpoints > 0) 
assign wp_match = |wp_match_n;                
    else
assign wp_match = `FALSE;
endgenerate
                
`ifdef CFG_HW_DEBUG_ENABLED                
// Multiplex between wcsr instruction writes and debugger writes to the debug CSRs
assign debug_csr_write_enable = (csr_write_enable_x == `TRUE) || (jtag_csr_write_enable == `TRUE);
assign debug_csr_write_data = jtag_csr_write_enable == `TRUE ? jtag_csr_write_data : csr_write_data;
assign debug_csr = jtag_csr_write_enable == `TRUE ? jtag_csr : csr_x;
`else
assign debug_csr_write_enable = csr_write_enable_x;
assign debug_csr_write_data = csr_write_data;
assign debug_csr = csr_x;
`endif

/////////////////////////////////////////////////////
// Sequential Logic
/////////////////////////////////////////////////////

// Breakpoint address and enable CSRs
generate
    for (i = 0; i < breakpoints; i = i + 1)
    begin : bp_seq
always @(posedge clk_i `CFG_RESET_SENSITIVITY)
begin
    if (rst_i == `TRUE)
    begin
        bp_a[i] <= {`LM32_PC_WIDTH{1'bx}};
        bp_e[i] <= `FALSE;
    end
    else
    begin
        if ((debug_csr_write_enable == `TRUE) && (debug_csr == `LM32_CSR_BP0 + i))
        begin
            bp_a[i] <= debug_csr_write_data[`LM32_PC_RNG];
            bp_e[i] <= debug_csr_write_data[0];
        end
    end
end    
    end
endgenerate

// Watchpoint address and control flags CSRs
generate
    for (i = 0; i < watchpoints; i = i + 1)
    begin : wp_seq
always @(posedge clk_i `CFG_RESET_SENSITIVITY)
begin
    if (rst_i == `TRUE)
    begin
        wp[i] <= {`LM32_WORD_WIDTH{1'bx}};
        wpc_c[i] <= `LM32_WPC_C_DISABLED;
    end
    else
    begin
        if (debug_csr_write_enable == `TRUE)
        begin
            if (debug_csr == `LM32_CSR_DC)
                wpc_c[i] <= debug_csr_write_data[3+i*2:2+i*2];
            if (debug_csr == `LM32_CSR_WP0 + i)
                wp[i] <= debug_csr_write_data;
        end
    end  
end
    end
endgenerate

// Remap exceptions control bit
always @(posedge clk_i `CFG_RESET_SENSITIVITY)
begin
    if (rst_i == `TRUE)
        dc_re <= `FALSE;
    else
    begin
        if ((debug_csr_write_enable == `TRUE) && (debug_csr == `LM32_CSR_DC))
            dc_re <= debug_csr_write_data[1];
    end
end    

`ifdef LM32_SINGLE_STEP_ENABLED
// Single-step control flag
always @(posedge clk_i `CFG_RESET_SENSITIVITY)
begin
    if (rst_i == `TRUE)
    begin
        state <= `LM32_DEBUG_SS_STATE_IDLE;
        dc_ss <= `FALSE;
    end
    else
    begin
        if ((debug_csr_write_enable == `TRUE) && (debug_csr == `LM32_CSR_DC))
        begin
            dc_ss <= debug_csr_write_data[0];
            if (debug_csr_write_data[0] == `FALSE) 
                state <= `LM32_DEBUG_SS_STATE_IDLE;
            else 
                state <= `LM32_DEBUG_SS_STATE_WAIT_FOR_RET;
        end
        case (state)
        `LM32_DEBUG_SS_STATE_WAIT_FOR_RET:
        begin
            // Wait for eret or bret instruction to be executed
            if (   (   (eret_q_x == `TRUE)
                    || (bret_q_x == `TRUE)
                    )
                && (stall_x == `FALSE)
               )
                state <= `LM32_DEBUG_SS_STATE_EXECUTE_ONE_INSN; 
        end
        `LM32_DEBUG_SS_STATE_EXECUTE_ONE_INSN:
        begin
            // Wait for an instruction to be executed
            if ((q_x == `TRUE) && (stall_x == `FALSE))
                state <= `LM32_DEBUG_SS_STATE_RAISE_BREAKPOINT;
        end
        `LM32_DEBUG_SS_STATE_RAISE_BREAKPOINT:
        begin
            // Wait for exception to be raised
`ifdef CFG_DCACHE_ENABLED
            if (dcache_refill_request == `TRUE)
                state <= `LM32_DEBUG_SS_STATE_EXECUTE_ONE_INSN;
            else 
`endif
                 if ((exception_x == `TRUE) && (q_x == `TRUE) && (stall_x == `FALSE))
            begin
                dc_ss <= `FALSE;
                state <= `LM32_DEBUG_SS_STATE_RESTART;
            end
        end
        `LM32_DEBUG_SS_STATE_RESTART:
        begin
            // Watch to see if stepped instruction is restarted due to a cache miss
`ifdef CFG_DCACHE_ENABLED
            if (dcache_refill_request == `TRUE)
                state <= `LM32_DEBUG_SS_STATE_EXECUTE_ONE_INSN;
            else 
`endif
                state <= `LM32_DEBUG_SS_STATE_IDLE;
        end
        endcase
    end
end
`endif

endmodule

`endif
