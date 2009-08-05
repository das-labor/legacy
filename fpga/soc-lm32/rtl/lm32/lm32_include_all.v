`ifndef LM32_ALL_FILES
`define LM32_ALL_FILES
//`ifndef SIMULATION
//`include "pmi_def.v"
//`endif
// JTAG Debug related files
`include "../components/lm32_top/rtl/verilog/er1.v"
`include "../components/lm32_top/rtl/verilog/typeb.v"
`include "../components/lm32_top/rtl/verilog/typea.v"
`include "../components/lm32_top/rtl/verilog/jtag_cores.v"
`include "../components/lm32_top/rtl/verilog/jtag_lm32.v"
// CPU Core related files
`include "../components/lm32_top/rtl/verilog/lm32_addsub.v"
`include "../components/lm32_top/rtl/verilog/lm32_adder.v"
`include "../components/lm32_top/rtl/verilog/lm32_cpu.v"
`include "../components/lm32_top/rtl/verilog/lm32_dcache.v"
`include "../components/lm32_top/rtl/verilog/lm32_debug.v"
`include "../components/lm32_top/rtl/verilog/lm32_decoder.v"
`include "../components/lm32_top/rtl/verilog/lm32_icache.v"
`include "../components/lm32_top/rtl/verilog/lm32_instruction_unit.v"
`include "../components/lm32_top/rtl/verilog/lm32_interrupt.v"
`include "../components/lm32_top/rtl/verilog/lm32_load_store_unit.v"
`include "../components/lm32_top/rtl/verilog/lm32_logic_op.v"
`include "../components/lm32_top/rtl/verilog/lm32_mc_arithmetic.v"
`include "../components/lm32_top/rtl/verilog/lm32_multiplier.v"
`include "../components/lm32_top/rtl/verilog/lm32_shifter.v"
`include "../components/lm32_top/rtl/verilog/lm32_top.v"
`include "../components/lm32_top/rtl/verilog/lm32_monitor.v"
`include "../components/lm32_top/rtl/verilog/lm32_monitor_ram.v"
`include "../components/lm32_top/rtl/verilog/lm32_ram.v"
`include "../components/lm32_top/rtl/verilog/lm32_jtag.v"
`endif // LM32_ALL_FILES
