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
// File             : lm32_addsub.v
// Title            : PMI adder/subtractor.
// Version          : 6.0.13
// =============================================================================

/////////////////////////////////////////////////////
// Module interface
/////////////////////////////////////////////////////

module lm32_addsub (
    // ----- Inputs -------
    DataA, 
    DataB, 
    Cin, 
    Add_Sub, 
    // ----- Outputs -------
    Result, 
    Cout
    );

/////////////////////////////////////////////////////
// Inputs
/////////////////////////////////////////////////////

input [31:0] DataA;
input [31:0] DataB;
input Cin;
input Add_Sub;

/////////////////////////////////////////////////////
// Outputs
/////////////////////////////////////////////////////

output [31:0] Result;
wire   [31:0] Result;
output Cout;
wire   Cout;

/////////////////////////////////////////////////////
// Instantiations
///////////////////////////////////////////////////// 

//pmi_addsub #(
//    // ----- Parameters -------
//    .pmi_data_width     (32),
//    .pmi_result_width   (32),
//    .pmi_sign           ("off"),
//    .pmi_family         ("ECP"),
//    .module_type        ("pmi_addsub")
//  ) addsub (
    // ----- Inputs -------
//    .DataA              (DataA),
//    .DataB              (DataB),
//    .Cin                (Cin),
//    .Add_Sub            (Add_Sub),
    // ----- Outputs -------
//    .Result             (Result),
//    .Cout               (Cout),
//    .Overflow           ()
//    );

assign Result = DataA + DataB;

//my_addsub(
//	.A	(DataA),
//	.B	(DataB),
//	.C_IN	(Cin),
//	.C_OUT	(Cout),
//	.ADD	(Add_Sub),
//	.S	(Result)
//	);

endmodule
