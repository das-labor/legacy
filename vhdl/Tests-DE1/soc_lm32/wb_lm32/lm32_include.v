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
// File             : lm32_include.v
// Title            : CPU global macros
// Version          : 6.0.13
// =============================================================================

`ifdef LM32_INCLUDE_V
`else
`define LM32_INCLUDE_V

// Configuration options
`include "system_conf.v"

`ifdef TRUE
`else
`define TRUE    1'b1
`define FALSE   1'b0
`define TRUE_N  1'b0
`define FALSE_N 1'b1
`endif

// Wishbone configuration
`define CFG_IWB_ENABLED
`define CFG_DWB_ENABLED

`define LM32_WORD_WIDTH                 32
`define LM32_WORD_RNG                   (`LM32_WORD_WIDTH-1):0
`define LM32_SHIFT_WIDTH                5
`define LM32_SHIFT_RNG                  (`LM32_SHIFT_WIDTH-1):0
`define LM32_BYTE_SELECT_WIDTH          4
`define LM32_BYTE_SELECT_RNG            (`LM32_BYTE_SELECT_WIDTH-1):0

`define LM32_REGISTERS                  32
`define LM32_REG_IDX_WIDTH              5
`define LM32_REG_IDX_RNG                (`LM32_REG_IDX_WIDTH-1):0

// Standard register numbers
`define LM32_RA_REG                     `LM32_REG_IDX_WIDTH'd29
`define LM32_EA_REG                     `LM32_REG_IDX_WIDTH'd30
`define LM32_BA_REG                     `LM32_REG_IDX_WIDTH'd31

// Range of Program Counter. Two LSBs are always 0. 
`ifdef CFG_ICACHE_ENABLED
`define LM32_PC_WIDTH                   (clogb2(`CFG_ICACHE_LIMIT-`CFG_ICACHE_BASE_ADDRESS)-2)
`else
`ifdef CFG_IWB_ENABLED
`define LM32_PC_WIDTH                   (`LM32_WORD_WIDTH-2)
`else
`define LM32_PC_WIDTH                   `LM32_IROM_ADDRESS_WIDTH
`endif
`endif
`define LM32_PC_RNG                     (`LM32_PC_WIDTH+2-1):2

// Range of an instruction
`define LM32_INSTRUCTION_WIDTH          32
`define LM32_INSTRUCTION_RNG            (`LM32_INSTRUCTION_WIDTH-1):0

// Adder operation
`define LM32_ADDER_OP_ADD               1'b0
`define LM32_ADDER_OP_SUBTRACT          1'b1

// Shift direction
`define LM32_SHIFT_OP_RIGHT             1'b0
`define LM32_SHIFT_OP_LEFT              1'b1

// Currently always enabled
`define CFG_BUS_ERRORS_ENABLED

// Derive macro that indicates whether we have single-stepping or not
`ifdef CFG_ROM_DEBUG_ENABLED
`define LM32_SINGLE_STEP_ENABLED
`else
`ifdef CFG_HW_DEBUG_ENABLED
`define LM32_SINGLE_STEP_ENABLED
`endif
`endif

// Derive macro that indicates whether JTAG interface is required
`ifdef CFG_JTAG_UART_ENABLED
`define LM32_JTAG_ENABLED
`else
`ifdef CFG_DEBUG_ENABLED
`define LM32_JTAG_ENABLED
`else
`endif
`endif

// Derive macro that indicates whether we have a barrel-shifter or not
`ifdef CFG_PL_BARREL_SHIFT_ENABLED
`define LM32_BARREL_SHIFT_ENABLED
`else // CFG_PL_BARREL_SHIFT_ENABLED
`ifdef CFG_MC_BARREL_SHIFT_ENABLED
`define LM32_BARREL_SHIFT_ENABLED
`else
`define LM32_NO_BARREL_SHIFT
`endif
`endif // CFG_PL_BARREL_SHIFT_ENABLED

// Derive macro that indicates whether we have a multiplier or not
`ifdef CFG_PL_MULTIPLY_ENABLED
`define LM32_MULTIPLY_ENABLED
`else
`ifdef CFG_MC_MULTIPLY_ENABLED
`define LM32_MULTIPLY_ENABLED
`endif
`endif

`ifdef CFG_MC_DIVIDE_ENABLED
`define LM32_MC_ARITHMETIC_ENABLED
`endif
`ifdef CFG_MC_MULTIPLY_ENABLED
`define LM32_MC_ARITHMETIC_ENABLED
`endif
`ifdef CFG_MC_BARREL_SHIFT_ENABLED
`define LM32_MC_ARITHMETIC_ENABLED
`endif

// Derive macro that indicates if we are using an EBR register file
`ifdef CFG_EBR_POSEDGE_REGISTER_FILE
`define LM32_EBR_REGISTER_FILE
`endif
`ifdef CFG_EBR_NEGEDGE_REGISTER_FILE
`define LM32_EBR_REGISTER_FILE
`endif

// Revision number
`define LM32_REVISION                   6'hd

// Logical operations - Function encoded directly in instruction
`define LM32_LOGIC_OP_RNG               3:0

// Conditions for conditional branches
`define LM32_CONDITION_WIDTH            3
`define LM32_CONDITION_RNG              (`LM32_CONDITION_WIDTH-1):0
`define LM32_CONDITION_E                3'b001
`define LM32_CONDITION_G                3'b010
`define LM32_CONDITION_GE               3'b011
`define LM32_CONDITION_GEU              3'b100
`define LM32_CONDITION_GU               3'b101
`define LM32_CONDITION_NE               3'b111
`define LM32_CONDITION_U1               3'b000
`define LM32_CONDITION_U2               3'b110

// Size of load or store instruction - Encoding corresponds to opcode
`define LM32_SIZE_WIDTH                 2
`define LM32_SIZE_RNG                   1:0
`define LM32_SIZE_BYTE                  2'b00
`define LM32_SIZE_HWORD                 2'b11
`define LM32_SIZE_WORD                  2'b10
`define LM32_ADDRESS_LSBS_WIDTH         2

// Width and range of a CSR index
`ifdef CFG_DEBUG_ENABLED
`define LM32_CSR_WIDTH                  5
`define LM32_CSR_RNG                    (`LM32_CSR_WIDTH-1):0
`else
`ifdef CFG_JTAG_ENABLED
`define LM32_CSR_WIDTH                  4
`define LM32_CSR_RNG                    (`LM32_CSR_WIDTH-1):0
`else
`define LM32_CSR_WIDTH                  3
`define LM32_CSR_RNG                    (`LM32_CSR_WIDTH-1):0
`endif
`endif

// CSR indices
`define LM32_CSR_IE                     `LM32_CSR_WIDTH'h0
`define LM32_CSR_IM                     `LM32_CSR_WIDTH'h1
`define LM32_CSR_IP                     `LM32_CSR_WIDTH'h2
`define LM32_CSR_ICC                    `LM32_CSR_WIDTH'h3
`define LM32_CSR_DCC                    `LM32_CSR_WIDTH'h4
`define LM32_CSR_CC                     `LM32_CSR_WIDTH'h5
`define LM32_CSR_CFG                    `LM32_CSR_WIDTH'h6
`define LM32_CSR_EBA                    `LM32_CSR_WIDTH'h7
`ifdef CFG_DEBUG_ENABLED
`define LM32_CSR_DC                     `LM32_CSR_WIDTH'h8
`define LM32_CSR_DEBA                   `LM32_CSR_WIDTH'h9
`endif
`ifdef CFG_JTAG_ENABLED
`define LM32_CSR_JTX                    `LM32_CSR_WIDTH'he
`define LM32_CSR_JRX                    `LM32_CSR_WIDTH'hf
`endif
`ifdef CFG_DEBUG_ENABLED
`define LM32_CSR_BP0                    `LM32_CSR_WIDTH'h10
`define LM32_CSR_BP1                    `LM32_CSR_WIDTH'h11
`define LM32_CSR_BP2                    `LM32_CSR_WIDTH'h12
`define LM32_CSR_BP3                    `LM32_CSR_WIDTH'h13
`define LM32_CSR_WP0                    `LM32_CSR_WIDTH'h18
`define LM32_CSR_WP1                    `LM32_CSR_WIDTH'h19
`define LM32_CSR_WP2                    `LM32_CSR_WIDTH'h1a
`define LM32_CSR_WP3                    `LM32_CSR_WIDTH'h1b
`endif 

// Values for WPC CSR
`define LM32_WPC_C_RNG                  1:0
`define LM32_WPC_C_DISABLED             2'b00
`define LM32_WPC_C_READ                 2'b01
`define LM32_WPC_C_WRITE                2'b10
`define LM32_WPC_C_READ_WRITE           2'b11

// Exception IDs
`define LM32_EID_WIDTH                  3
`define LM32_EID_RNG                    (`LM32_EID_WIDTH-1):0
`define LM32_EID_RESET                  3'h0
`define LM32_EID_BREAKPOINT             3'd1
`define LM32_EID_INST_BUS_ERROR         3'h2
`define LM32_EID_WATCHPOINT             3'd3
`define LM32_EID_DATA_BUS_ERROR         3'h4
`define LM32_EID_DIVIDE_BY_ZERO         3'h5
`define LM32_EID_INTERRUPT              3'h6
`define LM32_EID_SCALL                  3'h7

// Pipeline result selection mux controls

`define LM32_D_RESULT_SEL_0_RNG          0:0
`define LM32_D_RESULT_SEL_0_REG_0        1'b0
`define LM32_D_RESULT_SEL_0_NEXT_PC      1'b1

`define LM32_D_RESULT_SEL_1_RNG          1:0
`define LM32_D_RESULT_SEL_1_ZERO         2'b00
`define LM32_D_RESULT_SEL_1_REG_1        2'b01
`define LM32_D_RESULT_SEL_1_IMMEDIATE    2'b10

`define STYLE_MUX_X
`define STYLE_MUX_M
`define STYLE_MUX_W

`ifdef STYLE_MUX_X   
`else 
`define LM32_X_RESULT_SEL_WIDTH         4
`define LM32_X_RESULT_SEL_RNG           3:0
`define LM32_X_RESULT_SEL_ADDER         4'd0
`define LM32_X_RESULT_SEL_LOGIC         4'd1
`define LM32_X_RESULT_SEL_CSR           4'd2
`define LM32_X_RESULT_SEL_SEXT          4'd3
`define LM32_X_RESULT_SEL_USER          4'd4
`define LM32_X_RESULT_SEL_SHIFT         4'd5
`define LM32_X_RESULT_SEL_MULTIPLY      4'd6
`define LM32_X_RESULT_SEL_DIVIDE        4'd7
`define LM32_X_RESULT_SEL_MODULUS       4'd8
`define LM32_X_RESULT_SEL_SHIFT_LEFT    4'd9
`define LM32_X_RESULT_SEL_SHIFT_RIGHT   4'da
`endif

`ifdef CFG_MC_MULTIPLY_ENABLED
`ifdef CFG_USER_ENABLED
`ifdef CFG_SIGN_EXTEND_ENABLED
`ifdef CFG_PL_BARREL_SHIFT_ENABLED
`define LM32_X_RESULT_SEL_WIDTH         5
`define LM32_X_RESULT_SEL_RNG           4:0
`define LM32_X_RESULT_SEL_ADDER         5'b00001
`define LM32_X_RESULT_SEL_LOGIC         5'b00010
`define LM32_X_RESULT_SEL_CSR           5'b00100
`define LM32_X_RESULT_SEL_SEXT          5'b01000
`define LM32_X_RESULT_SEL_USER          5'b10000
`define LM32_X_RESULT_SEL_MULTIPLY      5'b00000
`else // CFG_PL_BARREL_SHIFT_ENABLED
`endif // CFG_PL_BARREL_SHIFT_ENABLED
`else // CFG_SIGN_EXTEND_ENABLED
`ifdef CFG_PL_BARREL_SHIFT_ENABLED
`define LM32_X_RESULT_SEL_WIDTH         4
`define LM32_X_RESULT_SEL_RNG           3:0
`define LM32_X_RESULT_SEL_ADDER         4'b0001
`define LM32_X_RESULT_SEL_LOGIC         4'b0010
`define LM32_X_RESULT_SEL_CSR           4'b0100
`define LM32_X_RESULT_SEL_USER          4'b1000
`define LM32_X_RESULT_SEL_MULTIPLY      4'b0000
`else // CFG_PL_BARREL_SHIFT_ENABLED
`define LM32_X_RESULT_SEL_WIDTH         5
`define LM32_X_RESULT_SEL_RNG           4:0
`define LM32_X_RESULT_SEL_ADDER         5'b00001
`define LM32_X_RESULT_SEL_LOGIC         5'b00010
`define LM32_X_RESULT_SEL_CSR           5'b00100
`define LM32_X_RESULT_SEL_USER          5'b01000
`define LM32_X_RESULT_SEL_SHIFT         5'b10000
`define LM32_X_RESULT_SEL_MULTIPLY      5'b00000
`endif // CFG_PL_BARREL_SHIFT_ENABLED
`endif // CFG_SIGN_EXTEND_ENABLED
`else // CFG_USER_ENABLED
`ifdef CFG_SIGN_EXTEND_ENABLED
`ifdef CFG_PL_BARREL_SHIFT_ENABLED
`define LM32_X_RESULT_SEL_WIDTH         4
`define LM32_X_RESULT_SEL_RNG           3:0
`define LM32_X_RESULT_SEL_ADDER         4'b0001
`define LM32_X_RESULT_SEL_LOGIC         4'b0010
`define LM32_X_RESULT_SEL_CSR           4'b0100
`define LM32_X_RESULT_SEL_SEXT          4'b1000
`define LM32_X_RESULT_SEL_MULTIPLY      4'b0000
`else // CFG_PL_BARREL_SHIFT_ENABLED
`define LM32_X_RESULT_SEL_WIDTH         5
`define LM32_X_RESULT_SEL_RNG           4:0
`define LM32_X_RESULT_SEL_ADDER         5'b00001
`define LM32_X_RESULT_SEL_LOGIC         5'b00010
`define LM32_X_RESULT_SEL_CSR           5'b00100
`define LM32_X_RESULT_SEL_SEXT          5'b01000
`define LM32_X_RESULT_SEL_SHIFT         5'b10000
`define LM32_X_RESULT_SEL_MULTIPLY      5'b00000
`endif // CFG_PL_BARREL_SHIFT_ENABLED
`else // CFG_SIGN_EXTEND_ENABLED
`ifdef CFG_PL_BARREL_SHIFT_ENABLED
`define LM32_X_RESULT_SEL_WIDTH         3
`define LM32_X_RESULT_SEL_RNG           2:0
`define LM32_X_RESULT_SEL_ADDER         3'b001
`define LM32_X_RESULT_SEL_LOGIC         3'b010
`define LM32_X_RESULT_SEL_CSR           3'b100
`define LM32_X_RESULT_SEL_MULTIPLY      3'b000
`else // CFG_PL_BARREL_SHIFT_ENABLED
`define LM32_X_RESULT_SEL_WIDTH         4
`define LM32_X_RESULT_SEL_RNG           3:0
`define LM32_X_RESULT_SEL_ADDER         4'b0001
`define LM32_X_RESULT_SEL_LOGIC         4'b0010
`define LM32_X_RESULT_SEL_CSR           4'b0100
`define LM32_X_RESULT_SEL_SHIFT         4'b1000
`define LM32_X_RESULT_SEL_MULTIPLY      4'b0000
`endif // CFG_PL_BARREL_SHIFT_ENABLED
`endif // CFG_SIGN_EXTEND_ENABLED
`endif // CFG_USER_ENABLED
`else // CFG_MC_MULTIPLY_ENABLED
`ifdef CFG_USER_ENABLED
`ifdef CFG_SIGN_EXTEND_ENABLED
`ifdef CFG_PL_BARREL_SHIFT_ENABLED
`define LM32_X_RESULT_SEL_WIDTH         4
`define LM32_X_RESULT_SEL_RNG           3:0
`define LM32_X_RESULT_SEL_ADDER         4'b0001
`define LM32_X_RESULT_SEL_LOGIC         4'b0010
`define LM32_X_RESULT_SEL_CSR           4'b0100
`define LM32_X_RESULT_SEL_SEXT          4'b1000
`define LM32_X_RESULT_SEL_USER          4'b0000
`else // CFG_PL_BARREL_SHIFT_ENABLED
`define LM32_X_RESULT_SEL_WIDTH         5
`define LM32_X_RESULT_SEL_RNG           4:0
`define LM32_X_RESULT_SEL_ADDER         5'b00001
`define LM32_X_RESULT_SEL_LOGIC         5'b00010
`define LM32_X_RESULT_SEL_CSR           5'b00100
`define LM32_X_RESULT_SEL_SEXT          5'b01000
`define LM32_X_RESULT_SEL_USER          5'b10000
`define LM32_X_RESULT_SEL_SHIFT         5'b00000
`endif // CFG_PL_BARREL_SHIFT_ENABLED
`else // CFG_SIGN_EXTEND_ENABLED
`ifdef CFG_PL_BARREL_SHIFT_ENABLED
`define LM32_X_RESULT_SEL_WIDTH         3
`define LM32_X_RESULT_SEL_RNG           2:0
`define LM32_X_RESULT_SEL_ADDER         3'b001
`define LM32_X_RESULT_SEL_LOGIC         3'b010
`define LM32_X_RESULT_SEL_CSR           3'b100
`define LM32_X_RESULT_SEL_USER          3'b000
`else // CFG_PL_BARREL_SHIFT_ENABLED
`define LM32_X_RESULT_SEL_WIDTH         4
`define LM32_X_RESULT_SEL_RNG           3:0
`define LM32_X_RESULT_SEL_ADDER         4'b0001
`define LM32_X_RESULT_SEL_LOGIC         4'b0010
`define LM32_X_RESULT_SEL_CSR           4'b0100
`define LM32_X_RESULT_SEL_USER          4'b1000
`define LM32_X_RESULT_SEL_SHIFT         4'b0000
`endif // CFG_PL_BARREL_SHIFT_ENABLED
`endif // CFG_SIGN_EXTEND_ENABLED
`else // CFG_USER_ENABLED
`ifdef CFG_SIGN_EXTEND_ENABLED
`ifdef CFG_PL_BARREL_SHIFT_ENABLED
`define LM32_X_RESULT_SEL_WIDTH         3
`define LM32_X_RESULT_SEL_RNG           2:0
`define LM32_X_RESULT_SEL_ADDER         3'b001
`define LM32_X_RESULT_SEL_LOGIC         3'b010
`define LM32_X_RESULT_SEL_CSR           3'b100
`define LM32_X_RESULT_SEL_SEXT          3'b000
`else // CFG_PL_BARREL_SHIFT_ENABLED
`define LM32_X_RESULT_SEL_WIDTH         4
`define LM32_X_RESULT_SEL_RNG           3:0
`define LM32_X_RESULT_SEL_ADDER         4'b0001
`define LM32_X_RESULT_SEL_LOGIC         4'b0010
`define LM32_X_RESULT_SEL_CSR           4'b0100
`define LM32_X_RESULT_SEL_SEXT          4'b1000
`define LM32_X_RESULT_SEL_SHIFT         4'b0000
`endif // CFG_PL_BARREL_SHIFT_ENABLED
`else // CFG_SIGN_EXTEND_ENABLED
`ifdef CFG_PL_BARREL_SHIFT_ENABLED
`define LM32_X_RESULT_SEL_WIDTH         2
`define LM32_X_RESULT_SEL_RNG           1:0
`define LM32_X_RESULT_SEL_ADDER         2'b01
`define LM32_X_RESULT_SEL_LOGIC         2'b10
`define LM32_X_RESULT_SEL_CSR           2'b00
`else // CFG_PL_BARREL_SHIFT_ENABLED
`define LM32_X_RESULT_SEL_WIDTH         3
`define LM32_X_RESULT_SEL_RNG           2:0
`define LM32_X_RESULT_SEL_ADDER         3'b001
`define LM32_X_RESULT_SEL_LOGIC         3'b010
`define LM32_X_RESULT_SEL_CSR           3'b100
`define LM32_X_RESULT_SEL_SHIFT         3'b000
`endif // CFG_PL_BARREL_SHIFT_ENABLED
`endif // CFG_SIGN_EXTEND_ENABLED
`endif // CFG_USER_ENABLED
`endif // CFG_MC_MULTIPLY_ENABLED

`ifdef STYLE_MUX_M
`else
`ifdef CFG_PL_BARREL_SHIFT_ENABLED
`define LM32_M_RESULT_SEL_WIDTH         2
`define LM32_M_RESULT_SEL_RNG           1:0
`define LM32_M_RESULT_SEL_NOP           2'b00
`define LM32_M_RESULT_SEL_COMPARE       2'b01
`define LM32_M_RESULT_SEL_SHIFTER       2'b10
`else
`define LM32_M_RESULT_SEL_WIDTH         1
`define LM32_M_RESULT_SEL_RNG           0:0
`define LM32_M_RESULT_SEL_NOP           1'b0
`define LM32_M_RESULT_SEL_COMPARE       1'b1
`endif
`endif

`ifdef STYLE_MUX_W
`else
`ifdef CFG_PL_MULTIPLY_ENABLED
`define LM32_W_RESULT_SEL_WIDTH         2
`define LM32_W_RESULT_SEL_RNG           1:0
`define LM32_W_RESULT_SEL_NOP           2'b00
`define LM32_W_RESULT_SEL_LOAD          2'b01
`define LM32_W_RESULT_SEL_MULTIPLIER    2'b10
`else
`define LM32_W_RESULT_SEL_WIDTH         1
`define LM32_W_RESULT_SEL_RNG           0:0
`define LM32_W_RESULT_SEL_NOP           1'b0
`define LM32_W_RESULT_SEL_LOAD          1'b1
`endif
`endif

`define LM32_USER_OPCODE_WIDTH           11
`define LM32_USER_OPCODE_RNG             (`LM32_USER_OPCODE_WIDTH-1):0

`ifdef CFG_ICACHE_ENABLED
`define LM32_CACHE_ENABLED      
`else
`ifdef CFG_DCACHE_ENABLED
`define LM32_CACHE_ENABLED
`endif
`endif

/////////////////////////////////////////////////////
// Interrupts
/////////////////////////////////////////////////////

// Always enable interrupts
`define CFG_INTERRUPTS_ENABLED

// Currently this is fixed to 32 and should not be changed
`define CFG_INTERRUPTS                  32
`define LM32_INTERRUPT_WIDTH            `CFG_INTERRUPTS
`define LM32_INTERRUPT_RNG              (`LM32_INTERRUPT_WIDTH-1):0

/////////////////////////////////////////////////////
// General
/////////////////////////////////////////////////////

// Sub-word range types
`define LM32_BYTE_WIDTH                 8
`define LM32_BYTE_RNG                   7:0
`define LM32_HWORD_WIDTH                16
`define LM32_HWORD_RNG                  15:0

// Word sub-byte indicies
`define LM32_BYTE_0_RNG                  7:0
`define LM32_BYTE_1_RNG                  15:8
`define LM32_BYTE_2_RNG                  23:16
`define LM32_BYTE_3_RNG                  31:24

// Word sub-halfword indices
`define LM32_HWORD_0_RNG                 15:0
`define LM32_HWORD_1_RNG                 31:16

// Use an asynchronous reset
`define CFG_RESET_SENSITIVITY or posedge rst_i
// Use muxes - To be removed: MUX Seems to be smaller
`define STYLE_MUX
//`define STYLE_CASE
//`define ONE_BIG_CSR_MUX

`define SRCE

// Bizarre - Removing this increases LUT count!
`define CFG_DEBUG_EXCEPTIONS_ENABLED

// Wishbone - These should probably be moved elsewhere
`define LM32_BTYPE_WIDTH                2
`define LM32_BTYPE_RNG                  (`LM32_BTYPE_WIDTH-1):0
`define LM32_CTYPE_WIDTH                3
`define LM32_CTYPE_RNG                  (`LM32_CTYPE_WIDTH-1):0

`endif
