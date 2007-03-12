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
// ============================================================================/
//                         FILE DETAILS
// Project          : LatticeMico32
// File             : lm32_adder.v
// Title            : Integer adder
// Dependencies     : lm32_include.v
// Version          : 6.0.13
// =============================================================================

`include "lm32_include.v"

/////////////////////////////////////////////////////
// Module interface
/////////////////////////////////////////////////////

module lm32_adder (
    // ----- Inputs -------
    adder_op_x,
    adder_op_x_n,
    operand_0_x,
    operand_1_x,
    // ----- Outputs -------
    adder_result_x,
    adder_carry_n_x,
    adder_overflow_x
    );

/////////////////////////////////////////////////////
// Inputs
/////////////////////////////////////////////////////

input adder_op_x;                                       // 0 for addition, 1 for subtraction
input adder_op_x_n;                                     // Inverted version of adder_op_x
input [`LM32_WORD_RNG] operand_0_x;
input [`LM32_WORD_RNG] operand_1_x;                 

/////////////////////////////////////////////////////
// Outputs
/////////////////////////////////////////////////////

output [`LM32_WORD_RNG] adder_result_x;
wire   [`LM32_WORD_RNG] adder_result_x;
output adder_carry_n_x;                                 // Inverted carry
wire   adder_carry_n_x;
output adder_overflow_x;                                // Only valid for subtractions
reg    adder_overflow_x;
    
/////////////////////////////////////////////////////
// Internal nets and registers 
/////////////////////////////////////////////////////

wire a_sign;
wire b_sign;
wire result_sign;

/////////////////////////////////////////////////////
// Instantiations 
/////////////////////////////////////////////////////

lm32_addsub addsub (
    // ----- Inputs -----
    .DataA          (operand_0_x), 
    .DataB          (operand_1_x), 
    .Cin            (adder_op_x), 
    .Add_Sub        (adder_op_x_n), 
    // ----- Ouputs -----
    .Result         (adder_result_x), 
    .Cout           (adder_carry_n_x)
    );

/////////////////////////////////////////////////////
// Combinational Logic
/////////////////////////////////////////////////////

// Determine whether an overflow occured when performing a subtraction

assign a_sign = operand_0_x[`LM32_WORD_WIDTH-1];
assign b_sign = operand_1_x[`LM32_WORD_WIDTH-1];
assign result_sign = adder_result_x[`LM32_WORD_WIDTH-1];
always @(*)
begin    
    //  +ve - -ve = -ve -> overflow
    //  -ve - +ve = +ve -> overflow
    if  (   (!a_sign & b_sign & result_sign)
         || (a_sign & !b_sign & !result_sign)
        )
        adder_overflow_x = `TRUE;
    else
        adder_overflow_x = `FALSE;
end
    
endmodule

