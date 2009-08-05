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
// File             : lm_mc_arithmetic.v
// Title            : Multi-cycle arithmetic unit.
// Dependencies     : lm32_include.v
// Version          : 6.1.17
// =============================================================================

`include "lm32_include.v"
           
`define LM32_MC_STATE_RNG         2:0
`define LM32_MC_STATE_IDLE        3'b000
`define LM32_MC_STATE_MULTIPLY    3'b001
`define LM32_MC_STATE_MODULUS     3'b010   
`define LM32_MC_STATE_DIVIDE      3'b011 
`define LM32_MC_STATE_SHIFT_LEFT  3'b100
`define LM32_MC_STATE_SHIFT_RIGHT 3'b101

/////////////////////////////////////////////////////
// Module interface
/////////////////////////////////////////////////////

module lm32_mc_arithmetic (
    // ----- Inputs -----
    clk_i,
    rst_i,
    stall_d,
    kill_x,
`ifdef CFG_MC_DIVIDE_ENABLED
    divide_d,
    modulus_d,
`endif
`ifdef CFG_MC_MULTIPLY_ENABLED
    multiply_d,
`endif
`ifdef CFG_MC_BARREL_SHIFT_ENABLED
    shift_left_d,
    shift_right_d,
    sign_extend_d,
`endif
    operand_0_d,
    operand_1_d,
    // ----- Ouputs -----
    result_x,
`ifdef CFG_MC_DIVIDE_ENABLED
    divide_by_zero_x,
`endif
    stall_request_x
    );

/////////////////////////////////////////////////////
// Inputs
/////////////////////////////////////////////////////

input clk_i;                                    // Clock
input rst_i;                                    // Reset
input stall_d;                                  // Stall instruction in D stage
input kill_x;                                   // Kill instruction in X stage
`ifdef CFG_MC_DIVIDE_ENABLED
input divide_d;                                 // Perform divide
input modulus_d;                                // Perform modulus
`endif
`ifdef CFG_MC_MULTIPLY_ENABLED
input multiply_d;                               // Perform multiply
`endif
`ifdef CFG_MC_BARREL_SHIFT_ENABLED
input shift_left_d;                             // Perform left shift
input shift_right_d;                            // Perform right shift
input sign_extend_d;                            // Whether to sign-extend (arithmetic) or zero-extend (logical)
`endif
input [`LM32_WORD_RNG] operand_0_d;
input [`LM32_WORD_RNG] operand_1_d;

/////////////////////////////////////////////////////
// Outputs
/////////////////////////////////////////////////////

output [`LM32_WORD_RNG] result_x;               // Result of operation
reg    [`LM32_WORD_RNG] result_x;
`ifdef CFG_MC_DIVIDE_ENABLED
output divide_by_zero_x;                        // A divide by zero was attempted
reg    divide_by_zero_x;
`endif
output stall_request_x;                         // Request to stall pipeline from X stage back
wire   stall_request_x;

/////////////////////////////////////////////////////
// Internal nets and registers 
/////////////////////////////////////////////////////

reg [`LM32_WORD_RNG] p;                         // Temporary registers
reg [`LM32_WORD_RNG] a;
reg [`LM32_WORD_RNG] b;
`ifdef CFG_MC_DIVIDE_ENABLED
wire [32:0] t;
`endif

reg [`LM32_MC_STATE_RNG] state;                 // Current state of FSM
reg [5:0] cycles;                               // Number of cycles remaining in the operation

`ifdef CFG_MC_BARREL_SHIFT_ENABLED
reg sign_extend_x;                              // Whether to sign extend of zero extend right shifts
wire fill_value;                                // Value to fill with for right barrel-shifts
`endif

/////////////////////////////////////////////////////
// Combinational logic
/////////////////////////////////////////////////////

// Stall pipeline while any operation is being performed
assign stall_request_x = state != `LM32_MC_STATE_IDLE;

`ifdef CFG_MC_DIVIDE_ENABLED
// Subtraction
assign t = {p[`LM32_WORD_WIDTH-2:0], a[`LM32_WORD_WIDTH-1]} - b;
`endif

`ifdef CFG_MC_BARREL_SHIFT_ENABLED
// Determine fill value for right shift - Sign bit for arithmetic shift, or zero for logical shift
assign fill_value = (sign_extend_x == `TRUE) & b[`LM32_WORD_WIDTH-1];
`endif

/////////////////////////////////////////////////////
// Sequential logic
/////////////////////////////////////////////////////

// Perform right shift
always @(posedge clk_i `CFG_RESET_SENSITIVITY)
begin
    if (rst_i == `TRUE)
    begin
        cycles <= {6{1'b0}};
        p <= {`LM32_WORD_WIDTH{1'b0}};
        a <= {`LM32_WORD_WIDTH{1'b0}};
        b <= {`LM32_WORD_WIDTH{1'b0}};
`ifdef CFG_MC_BARREL_SHIFT_ENABLED
        sign_extend_x <= 1'b0;
`endif
`ifdef CFG_MC_DIVIDE_ENABLED
        divide_by_zero_x <= `FALSE;
`endif
        result_x <= {`LM32_WORD_WIDTH{1'b0}};
        state <= `LM32_MC_STATE_IDLE;
    end
    else
    begin
`ifdef CFG_MC_DIVIDE_ENABLED
        divide_by_zero_x <= `FALSE;
`endif
        case (state)
        `LM32_MC_STATE_IDLE:
        begin
            if (stall_d == `FALSE)                 
            begin          
                cycles <= `LM32_WORD_WIDTH;
                p <= 32'b0;
                a <= operand_0_d;
                b <= operand_1_d;                    
`ifdef CFG_MC_DIVIDE_ENABLED
                if (divide_d == `TRUE)
                    state <= `LM32_MC_STATE_DIVIDE;
                if (modulus_d == `TRUE)
                    state <= `LM32_MC_STATE_MODULUS;
`endif                    
`ifdef CFG_MC_MULTIPLY_ENABLED
                if (multiply_d == `TRUE)
                    state <= `LM32_MC_STATE_MULTIPLY;
`endif
`ifdef CFG_MC_BARREL_SHIFT_ENABLED
                if (shift_left_d == `TRUE)
                begin
                    state <= `LM32_MC_STATE_SHIFT_LEFT;
                    sign_extend_x <= sign_extend_d;
                    cycles <= operand_1_d[4:0];
                    a <= operand_0_d;
                    b <= operand_0_d;
                end
                if (shift_right_d == `TRUE)
                begin
                    state <= `LM32_MC_STATE_SHIFT_RIGHT;
                    sign_extend_x <= sign_extend_d;
                    cycles <= operand_1_d[4:0];
                    a <= operand_0_d;
                    b <= operand_0_d;
                end
`endif
            end            
        end
`ifdef CFG_MC_DIVIDE_ENABLED
        `LM32_MC_STATE_DIVIDE:
        begin
            if (t[32] == 1'b0)
            begin
                p <= t[31:0];
                a <= {a[`LM32_WORD_WIDTH-2:0], 1'b1};
            end
            else 
            begin
                p <= {p[`LM32_WORD_WIDTH-2:0], a[`LM32_WORD_WIDTH-1]};
                a <= {a[`LM32_WORD_WIDTH-2:0], 1'b0};
            end
            result_x <= a;
            if ((cycles == `LM32_WORD_WIDTH'd0) || (kill_x == `TRUE))
            begin
                // Check for divide by zero
                divide_by_zero_x <= b == {`LM32_WORD_WIDTH{1'b0}};
                state <= `LM32_MC_STATE_IDLE;
            end
            cycles <= cycles - 1'b1;
        end
        `LM32_MC_STATE_MODULUS:
        begin
            if (t[32] == 1'b0)
            begin
                p <= t[31:0];
                a <= {a[`LM32_WORD_WIDTH-2:0], 1'b1};
            end
            else 
            begin
                p <= {p[`LM32_WORD_WIDTH-2:0], a[`LM32_WORD_WIDTH-1]};
                a <= {a[`LM32_WORD_WIDTH-2:0], 1'b0};
            end
            result_x <= p;
            if ((cycles == `LM32_WORD_WIDTH'd0) || (kill_x == `TRUE))
            begin
                // Check for divide by zero
                divide_by_zero_x <= b == {`LM32_WORD_WIDTH{1'b0}};
                state <= `LM32_MC_STATE_IDLE;
            end
            cycles <= cycles - 1'b1;
        end
`endif        
`ifdef CFG_MC_MULTIPLY_ENABLED
        `LM32_MC_STATE_MULTIPLY:
        begin
            if (b[0] == 1'b1)
                p <= p + a;
            b <= {1'b0, b[`LM32_WORD_WIDTH-1:1]};
            a <= {a[`LM32_WORD_WIDTH-2:0], 1'b0};
            result_x <= p;
            if ((cycles == `LM32_WORD_WIDTH'd0) || (kill_x == `TRUE))
                state <= `LM32_MC_STATE_IDLE;
            cycles <= cycles - 1'b1;
        end
`endif     
`ifdef CFG_MC_BARREL_SHIFT_ENABLED
        `LM32_MC_STATE_SHIFT_LEFT:
        begin       
            a <= {a[`LM32_WORD_WIDTH-2:0], 1'b0};
            result_x <= a;
            if ((cycles == `LM32_WORD_WIDTH'd0) || (kill_x == `TRUE))
                state <= `LM32_MC_STATE_IDLE;
            cycles <= cycles - 1'b1;
        end
        `LM32_MC_STATE_SHIFT_RIGHT:
        begin       
            b <= {fill_value, b[`LM32_WORD_WIDTH-1:1]};
            result_x <= b;
            if ((cycles == `LM32_WORD_WIDTH'd0) || (kill_x == `TRUE))
                state <= `LM32_MC_STATE_IDLE;
            cycles <= cycles - 1'b1;
        end
`endif   
        endcase
    end
end 

endmodule
