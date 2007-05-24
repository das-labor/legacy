`ifndef SYSTEM_CONF
`define SYSTEM_CONF
`timescale 1ns / 100 ps
`define ADDRESS_LOCK
`define ebrEBR_DATA_WIDTH 32
`define ebrINIT_FILE_NAME "C:/LatticeMico32/micosystem/platforms/PlatformA/PlatformA.mem"
`define ebrINIT_FILE_FORMAT "hex"
`define ADDRESS_LOCK
`define LEDBOTH_INPUT_AND_OUTPUT
`define LEDDATA_WIDTH 32'h8
`define LEDINPUT_WIDTH 32'h1
`define LEDOUTPUT_WIDTH 32'h8
`define LEDPOSE_EDGE_IRQ
`define CFG_EBA_RESET 32'h0
`define CFG_SIGN_EXTEND_ENABLED
`define INCLUDE_LM32
`define ADDRESS_LOCK
`define MULT_ENABLE
`define CFG_PL_MULTIPLY_ENABLED
`define SHIFT_ENABLE
`define CFG_PL_BARREL_SHIFT_ENABLED
// `define CFG_ICACHE_ENABLED
// `define CFG_ICACHE_BASE_ADDRESS 0
// `define CFG_ICACHE_LIMIT 2048
// `define CFG_ICACHE_SETS 256
// `define CFG_ICACHE_BYTES_PER_LINE 8
// `define CFG_ICACHE_ASSOCIATIVITY 1

`endif // SYSTEM_CONF
