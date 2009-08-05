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
// File             : lm32_instruction_unit.v
// Title            : Instruction unit
// Dependencies     : lm32_include.v
// Version          : 6.1.17
// =============================================================================

`include "lm32_include.v"

/////////////////////////////////////////////////////
// Module interface
/////////////////////////////////////////////////////

module lm32_instruction_unit (
    // ----- Inputs -------
    clk_i,
    rst_i,
    // From pipeline
    stall_a,
    stall_f,
    stall_d,
    stall_x,
    stall_m,
    valid_f,
    kill_f,
`ifdef CFG_FAST_UNCONDITIONAL_BRANCH    
    branch_taken_x,
    branch_target_x,
`endif
    branch_taken_m,
    branch_target_m,
`ifdef CFG_ICACHE_ENABLED
    iflush,
`endif
`ifdef CFG_DCACHE_ENABLED
    dcache_restart_request,
    dcache_refill_request,
    dcache_refilling,
`endif        
`ifdef CFG_IWB_ENABLED
    // From Wishbone
    i_dat_i,
    i_ack_i,
    i_err_i,
    i_rty_i,
`endif
`ifdef CFG_HW_DEBUG_ENABLED
    jtag_read_enable,
    jtag_write_enable,
    jtag_write_data,
    jtag_address,
`endif
    // ----- Outputs -------
    // To pipeline
    pc_f,
    pc_d,
    pc_x,
    pc_m,
    pc_w,
`ifdef CFG_ICACHE_ENABLED
    icache_stall_request,
    icache_restart_request,
    icache_refill_request,
    icache_refilling,
`endif
`ifdef CFG_IWB_ENABLED
    // To Wishbone
    i_dat_o,
    i_adr_o,
    i_cyc_o,
    i_sel_o,
    i_stb_o,
    i_we_o,
    i_cti_o,
    i_lock_o,
    i_bte_o,
`endif
`ifdef CFG_HW_DEBUG_ENABLED
    jtag_read_data,
    jtag_access_complete,
`endif
`ifdef CFG_BUS_ERRORS_ENABLED
    bus_error_d,
`endif
`ifdef CFG_EBR_POSEDGE_REGISTER_FILE
    instruction_f,
`endif    
    instruction_d
    );

/////////////////////////////////////////////////////
// Parameters
/////////////////////////////////////////////////////

parameter associativity = 1;                            // Associativity of the cache (Number of ways)
parameter sets = 512;                                   // Number of sets
parameter bytes_per_line = 16;                          // Number of bytes per cache line
parameter base_address = 0;                             // Base address of cachable memory
parameter limit = 0;                                    // Limit (highest address) of cachable memory

// For bytes_per_line == 4, we set 1 so part-select range isn't reversed, even though not really used 
//localparam addr_offset_width = (bytes_per_line == 4 ? 1 : clogb2(bytes_per_line)-1-2);
localparam addr_offset_width = 2;
localparam addr_offset_lsb = 2;
localparam addr_offset_msb = (addr_offset_lsb+addr_offset_width-1);

/////////////////////////////////////////////////////
// Inputs
/////////////////////////////////////////////////////

input clk_i;                                            // Clock
input rst_i;                                            // Reset

input stall_a;                                          // Stall A stage instruction
input stall_f;                                          // Stall F stage instruction
input stall_d;                                          // Stall D stage instruction
input stall_x;                                          // Stall X stage instruction
input stall_m;                                          // Stall M stage instruction
input valid_f;                                          // Instruction in F stage is valid
input kill_f;                                           // Kill instruction in F stage

`ifdef CFG_FAST_UNCONDITIONAL_BRANCH    
input branch_taken_x;                                   // Branch instruction in X stage is taken
input [`LM32_PC_RNG] branch_target_x;                   // Target PC of X stage branch instruction
`endif
input branch_taken_m;                                   // Branch instruction in M stage is taken
input [`LM32_PC_RNG] branch_target_m;                   // Target PC of M stage branch instruction

`ifdef CFG_ICACHE_ENABLED
input iflush;                                           // Flush instruction cache
`endif
`ifdef CFG_DCACHE_ENABLED
input dcache_restart_request;                           // Restart instruction that caused a data cache miss
input dcache_refill_request;                            // Request to refill data cache
input dcache_refilling;
`endif        

`ifdef CFG_IWB_ENABLED
input [`LM32_WORD_RNG] i_dat_i;                         // Instruction Wishbone interface read data
input i_ack_i;                                          // Instruction Wishbone interface acknowledgement
input i_err_i;                                          // Instruction Wishbone interface error
input i_rty_i;                                          // Instruction Wishbone interface retry
`endif

`ifdef CFG_HW_DEBUG_ENABLED
input jtag_read_enable;                                 // JTAG read memory request
input jtag_write_enable;                                // JTAG write memory request
input [`LM32_BYTE_RNG] jtag_write_data;                 // JTAG wrirte data
input [`LM32_WORD_RNG] jtag_address;                    // JTAG read/write address
`endif

/////////////////////////////////////////////////////
// Outputs
/////////////////////////////////////////////////////
        
output [`LM32_PC_RNG] pc_f;                             // F stage PC
reg    [`LM32_PC_RNG] pc_f;
output [`LM32_PC_RNG] pc_d;                             // D stage PC
reg    [`LM32_PC_RNG] pc_d;
output [`LM32_PC_RNG] pc_x;                             // X stage PC
reg    [`LM32_PC_RNG] pc_x;
output [`LM32_PC_RNG] pc_m;                             // M stage PC
reg    [`LM32_PC_RNG] pc_m;
output [`LM32_PC_RNG] pc_w;                             // W stage PC
reg    [`LM32_PC_RNG] pc_w;

`ifdef CFG_ICACHE_ENABLED
output icache_stall_request;                            // Instruction cache stall request
wire   icache_stall_request;
output icache_restart_request;                          // Request to restart instruction that cached instruction cache miss
wire   icache_restart_request;
output icache_refill_request;                           // Instruction cache refill request
wire   icache_refill_request;
output icache_refilling;                                // Indicates the icache is refilling
wire   icache_refilling;
`endif

`ifdef CFG_IWB_ENABLED
output [`LM32_WORD_RNG] i_dat_o;                        // Instruction Wishbone interface write data
`ifdef CFG_HW_DEBUG_ENABLED
reg    [`LM32_WORD_RNG] i_dat_o;
`else
wire   [`LM32_WORD_RNG] i_dat_o;
`endif
output [`LM32_WORD_RNG] i_adr_o;                        // Instruction Wishbone interface address
reg    [`LM32_WORD_RNG] i_adr_o;
output i_cyc_o;                                         // Instruction Wishbone interface cycle
reg    i_cyc_o; 
output [`LM32_BYTE_SELECT_RNG] i_sel_o;                 // Instruction Wishbone interface byte select
`ifdef CFG_HW_DEBUG_ENABLED
reg    [`LM32_BYTE_SELECT_RNG] i_sel_o;
`else
wire   [`LM32_BYTE_SELECT_RNG] i_sel_o;
`endif
output i_stb_o;                                         // Instruction Wishbone interface strobe
reg    i_stb_o;
output i_we_o;                                          // Instruction Wishbone interface write enable
`ifdef CFG_HW_DEBUG_ENABLED
reg    i_we_o;
`else
wire   i_we_o;
`endif
output [`LM32_CTYPE_RNG] i_cti_o;                       // Instruction Wishbone interface cycle type 
reg    [`LM32_CTYPE_RNG] i_cti_o;
output i_lock_o;                                        // Instruction Wishbone interface lock bus
reg    i_lock_o;
output [`LM32_BTYPE_RNG] i_bte_o;                       // Instruction Wishbone interface burst type 
wire   [`LM32_BTYPE_RNG] i_bte_o;
`endif

`ifdef CFG_HW_DEBUG_ENABLED
output [`LM32_BYTE_RNG] jtag_read_data;                 // Data read for JTAG interface
reg    [`LM32_BYTE_RNG] jtag_read_data;
output jtag_access_complete;                            // Requested memory access by JTAG interface is complete
wire   jtag_access_complete;
`endif

`ifdef CFG_BUS_ERRORS_ENABLED
output bus_error_d;                                     // Indicates a bus error occured while fetching the instruction
reg    bus_error_d;
`endif
`ifdef CFG_EBR_POSEDGE_REGISTER_FILE
output [`LM32_INSTRUCTION_RNG] instruction_f;           // F stage instruction (only to have register indices extracted from)
wire   [`LM32_INSTRUCTION_RNG] instruction_f;
`endif
output [`LM32_INSTRUCTION_RNG] instruction_d;           // D stage instruction to be decoded
reg    [`LM32_INSTRUCTION_RNG] instruction_d;

/////////////////////////////////////////////////////
// Internal nets and registers 
/////////////////////////////////////////////////////

reg [`LM32_PC_RNG] pc_a;                                // A stage PC

`ifdef LM32_CACHE_ENABLED
reg [`LM32_PC_RNG] restart_address;                     // Address to restart from after a cache miss  
`endif

`ifdef CFG_ICACHE_ENABLED
wire icache_read_enable_f;                              // Indicates if instruction cache miss is valid
wire [`LM32_PC_RNG] icache_refill_address;              // Address that caused cache miss
reg icache_refill_ready;                                // Indicates when next word of refill data is ready to be written to cache
reg [`LM32_INSTRUCTION_RNG] icache_refill_data;         // Next word of refill data, fetched from Wishbone
wire [`LM32_INSTRUCTION_RNG] icache_data_f;             // Instruction fetched from instruction cache
wire [`LM32_CTYPE_RNG] first_cycle_type;                // First Wishbone cycle type
wire [`LM32_CTYPE_RNG] next_cycle_type;                 // Next Wishbone cycle type
wire last_word;                                         // Indicates if this is the last word in the cache line
wire [`LM32_PC_RNG] first_address;                      // First cache refill address
`else
`ifdef CFG_IWB_ENABLED
reg [`LM32_INSTRUCTION_RNG] wb_data_f;                  // Instruction fetched from Wishbone
`endif
`endif
`ifdef CFG_IROM_ENABLED
wire irom_select_a;                                     // Indicates if A stage PC maps to a ROM address
reg irom_select_f;                                      // Indicates if F stage PC maps to a ROM address
wire [`LM32_INSTRUCTION_RNG] irom_data_f;               // Instruction fetched from ROM
`endif
`ifdef CFG_EBR_POSEDGE_REGISTER_FILE
`else
wire [`LM32_INSTRUCTION_RNG] instruction_f;             // F stage instruction
`endif
`ifdef CFG_BUS_ERRORS_ENABLED
reg bus_error_f;                                        // Indicates if a bus error occured while fetching the instruction in the F stage
`endif

`ifdef CFG_HW_DEBUG_ENABLED
reg jtag_access;                                        // Indicates if a JTAG WB access is in progress
`endif

/////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////

`include "lm32_functions.v"

/////////////////////////////////////////////////////
// Instantiations
/////////////////////////////////////////////////////

// Instruction ROM
`ifdef CFG_IROM_ENABLED  
pmi_ram_dp #(
    // ----- Parameters -------
    .pmi_wr_addr_depth      (1 << (clogb2(`CFG_IROM_LIMIT/4-`CFG_IROM_BASE_ADDRESS/4+1)-1)),
    .pmi_wr_addr_width      ((clogb2(`CFG_IROM_LIMIT/4-`CFG_IROM_BASE_ADDRESS/4+1)-1)),
    .pmi_wr_data_width      (`LM32_INSTRUCTION_WIDTH),
    .pmi_rd_addr_depth      (1 << (clogb2(`CFG_IROM_LIMIT/4-`CFG_IROM_BASE_ADDRESS/4+1)-1)),
    .pmi_rd_addr_width      ((clogb2(`CFG_IROM_LIMIT/4-`CFG_IROM_BASE_ADDRESS/4+1)-1)),
    .pmi_rd_data_width      (`LM32_INSTRUCTION_WIDTH),
    .pmi_regmode            ("noreg"),
    .pmi_gsr                ("enable"),
    .pmi_resetmode          ("async"),
    .pmi_init_file          (`CFG_IROM_INIT_FILE),
    .pmi_init_file_format   ("hex"),
    .module_type            ("pmi_ram_dp")
  ) ram (
    // ----- Inputs -------
    .RdClock                (clk_i),
    .WrClock                (`FALSE),
    .Reset                  (rst_i),
    .Data                   ({32{1'b0}}),
    .RdAddress              (pc_a[(clogb2(`CFG_IROM_LIMIT/4-`CFG_IROM_BASE_ADDRESS/4+1)-1)+2-1:2]),
    .WrAddress              ({(clogb2(`CFG_IROM_LIMIT/4-`CFG_IROM_BASE_ADDRESS/4+1)-1){1'b0}}),
    .RdClockEn              (~stall_a),
    .WrClockEn              (`FALSE),
    .WE                     (`FALSE),
    // ----- Outputs -------
    .Q                      (irom_data_f)
    );
`endif    
 
`ifdef CFG_ICACHE_ENABLED
// Instruction cache
lm32_icache #(
    .associativity          (associativity),
    .sets                   (sets),
    .bytes_per_line         (bytes_per_line),
    .base_address           (base_address),
    .limit                  (limit)
    ) icache ( 
    // ----- Inputs -----
    .clk_i                  (clk_i),
    .rst_i                  (rst_i),      
    .stall_a                (stall_a),
    .stall_f                (stall_f),
    .address_a              (pc_a),
    .address_f              (pc_f),
    .read_enable_f          (icache_read_enable_f),
    .refill_ready           (icache_refill_ready),
    .refill_data            (icache_refill_data),
    .iflush                 (iflush),
    // ----- Outputs -----
    .stall_request          (icache_stall_request),
    .restart_request        (icache_restart_request),
    .refill_request         (icache_refill_request),
    .refill_address         (icache_refill_address),
    .refilling              (icache_refilling),
    .inst                   (icache_data_f)
    );
`endif

/////////////////////////////////////////////////////
// Combinational Logic
/////////////////////////////////////////////////////

`ifdef CFG_ICACHE_ENABLED
// Generate signal that indicates when instruction cache misses are valid
assign icache_read_enable_f =    (valid_f == `TRUE)
                              && (kill_f == `FALSE)
`ifdef CFG_DCACHE_ENABLED
                              && (dcache_restart_request == `FALSE)
`endif                         
`ifdef CFG_IROM_ENABLED 
                              && (irom_select_f == `FALSE)
`endif       
                              ;
`endif

// Compute address of next instruction to fetch
always @(*)
begin
    // The request from the latest pipeline stage must take priority
`ifdef CFG_DCACHE_ENABLED
    if (dcache_restart_request == `TRUE)
        pc_a = restart_address;
    else 
`endif    
         if (branch_taken_m == `TRUE)
        pc_a = branch_target_m;
`ifdef CFG_FAST_UNCONDITIONAL_BRANCH    
    else if (branch_taken_x == `TRUE)
        pc_a = branch_target_x;
`endif
    else
`ifdef CFG_ICACHE_ENABLED
         if (icache_restart_request == `TRUE)
        pc_a = restart_address;
    else 
`endif        
        pc_a = pc_f + 1'b1;
end

// Select where instruction should be fetched from
`ifdef CFG_IROM_ENABLED
assign irom_select_a = ({pc_a, 2'b00} >= `CFG_IROM_BASE_ADDRESS) && ({pc_a, 2'b00} <= `CFG_IROM_LIMIT);
`endif
                     
// Select instruction from selected source
`ifdef CFG_ICACHE_ENABLED
`ifdef CFG_IROM_ENABLED
assign instruction_f = irom_select_f == `TRUE ? irom_data_f : icache_data_f;
`else
assign instruction_f = icache_data_f;
`endif
`else
`ifdef CFG_IROM_ENABLED
`ifdef CFG_IWB_ENABLED
assign instruction_f = irom_select_f == `TRUE ? irom_data_f : wb_data_f;
`else
assign instruction_f = irom_data_f;
`endif
`else
assign instruction_f = wb_data_f;
`endif
`endif

// Unused/constant Wishbone signals
`ifdef CFG_IWB_ENABLED
`ifdef CFG_HW_DEBUG_ENABLED
`else
assign i_dat_o = 32'd0;
assign i_we_o = `FALSE;
assign i_sel_o = 4'b1111;
`endif
assign i_bte_o = `LM32_BTYPE_LINEAR;
`endif

`ifdef CFG_ICACHE_ENABLED
// Determine parameters for next cache refill Wishbone access                
// generate
//     case (bytes_per_line)
//     4:
//     begin
// assign first_cycle_type = `LM32_CTYPE_END;
// assign next_cycle_type = `LM32_CTYPE_END;
// assign last_word = `TRUE;
// assign first_address = icache_refill_address;
//     end
//     8:
//     begin
// assign first_cycle_type = `LM32_CTYPE_INCREMENTING;
// assign next_cycle_type = `LM32_CTYPE_END;
// assign last_word = i_adr_o[addr_offset_msb:addr_offset_lsb] == 1'b1;
// assign first_address = {icache_refill_address[`LM32_PC_WIDTH+2-1:addr_offset_msb+1], {addr_offset_width{1'b0}}};
//     end
//     16:
//     begin
assign first_cycle_type = `LM32_CTYPE_INCREMENTING;
assign next_cycle_type = i_adr_o[addr_offset_msb] == 1'b1 ? `LM32_CTYPE_END : `LM32_CTYPE_INCREMENTING;
assign last_word = i_adr_o[addr_offset_msb:addr_offset_lsb] == 2'b11;
assign first_address = {icache_refill_address[`LM32_PC_WIDTH+2-1:addr_offset_msb+1], {addr_offset_width{1'b0}}};
//     end
//     endcase
// endgenerate
`endif
                     
/////////////////////////////////////////////////////
// Sequential Logic
/////////////////////////////////////////////////////

// PC 
always @(posedge clk_i `CFG_RESET_SENSITIVITY)
begin
    if (rst_i == `TRUE)
    begin
        pc_f <= (`CFG_EBA_RESET-4)/4;
        pc_d <= {`LM32_PC_WIDTH{1'b0}};
        pc_x <= {`LM32_PC_WIDTH{1'b0}};
        pc_m <= {`LM32_PC_WIDTH{1'b0}};
        pc_w <= {`LM32_PC_WIDTH{1'b0}};
    end
    else
    begin
        if (stall_f == `FALSE)
            pc_f <= pc_a;
        if (stall_d == `FALSE)
            pc_d <= pc_f;
        if (stall_x == `FALSE)
            pc_x <= pc_d;
        if (stall_m == `FALSE)
            pc_m <= pc_x;
        pc_w <= pc_m;
    end
end

`ifdef LM32_CACHE_ENABLED
// Address to restart from after a cache miss has been handled
always @(posedge clk_i `CFG_RESET_SENSITIVITY)
begin
    if (rst_i == `TRUE)
        restart_address <= {`LM32_PC_WIDTH{1'b0}};
    else
    begin
`ifdef CFG_DCACHE_ENABLED
`ifdef CFG_ICACHE_ENABLED        
            // D-cache restart address must take priority, otherwise instructions will be lost
            if (dcache_refill_request == `TRUE)
                restart_address <= pc_w;
            else if ((icache_refill_request == `TRUE) && (!dcache_refilling))
                restart_address <= icache_refill_address;
`else
            if (dcache_refill_request == `TRUE)
                restart_address <= pc_w;
`endif
`else
`ifdef CFG_ICACHE_ENABLED        
            if (icache_refill_request == `TRUE)
                restart_address <= icache_refill_address;
`endif
`endif
    end
end
`endif

// Record where instruction was fetched from
`ifdef CFG_IROM_ENABLED
always @(posedge clk_i `CFG_RESET_SENSITIVITY)
begin
    if (rst_i == `TRUE)
        irom_select_f <= `FALSE;
    else
    begin
        if (stall_f == `FALSE)
            irom_select_f <= irom_select_a;
    end
end
`endif

`ifdef CFG_HW_DEBUG_ENABLED
assign jtag_access_complete = (i_cyc_o == `TRUE) && ((i_ack_i == `TRUE) || (i_err_i == `TRUE)) && (jtag_access == `TRUE);
always @*
begin
    case (jtag_address[1:0])
    2'b00: jtag_read_data = i_dat_i[`LM32_BYTE_3_RNG];
    2'b01: jtag_read_data = i_dat_i[`LM32_BYTE_2_RNG];
    2'b10: jtag_read_data = i_dat_i[`LM32_BYTE_1_RNG];
    2'b11: jtag_read_data = i_dat_i[`LM32_BYTE_0_RNG];
    endcase 
end
`endif

`ifdef CFG_IWB_ENABLED
// Instruction Wishbone interface
`ifdef CFG_ICACHE_ENABLED                
always @(posedge clk_i `CFG_RESET_SENSITIVITY)
begin
    if (rst_i == `TRUE)
    begin
        i_cyc_o <= `FALSE;
        i_stb_o <= `FALSE;
        i_adr_o <= {`LM32_WORD_WIDTH{1'b0}};
        i_cti_o <= `LM32_CTYPE_END;
        i_lock_o <= `FALSE;
        icache_refill_data <= {`LM32_INSTRUCTION_WIDTH{1'b0}};
        icache_refill_ready <= `FALSE;
`ifdef CFG_BUS_ERRORS_ENABLED
        bus_error_f <= `FALSE;
`endif
`ifdef CFG_HW_DEBUG_ENABLED
        i_we_o <= `FALSE;
        i_sel_o <= 4'b1111;
        jtag_access <= `FALSE;
`endif
    end
    else
    begin   
        icache_refill_ready <= `FALSE;
        // Is a cycle in progress?
        if (i_cyc_o == `TRUE)
        begin
            // Has cycle completed?
            if ((i_ack_i == `TRUE) || (i_err_i == `TRUE))
            begin
`ifdef CFG_HW_DEBUG_ENABLED
                if (jtag_access == `TRUE)
                begin
                    i_cyc_o <= `FALSE;
                    i_stb_o <= `FALSE;       
                    i_we_o <= `FALSE;  
                    jtag_access <= `FALSE;    
                end
                else
`endif
                begin
                    if (last_word == `TRUE)
                    begin
                        // Cache line fill complete 
                        i_cyc_o <= `FALSE;
                        i_stb_o <= `FALSE;
                        i_lock_o <= `FALSE;
                    end
                    // Fetch next word in cache line
                    i_adr_o[addr_offset_msb:addr_offset_lsb] <= i_adr_o[addr_offset_msb:addr_offset_lsb] + 1'b1;
                    i_cti_o <= next_cycle_type;
                    // Write fetched data into instruction cache
                    icache_refill_ready <= `TRUE;
                    icache_refill_data <= i_dat_i;
                end
            end
`ifdef CFG_BUS_ERRORS_ENABLED
            if (i_err_i == `TRUE)
            begin
                bus_error_f <= `TRUE;
                $display ("Instruction bus error. Address: %x", i_adr_o);
            end
`endif
        end
        else
        begin
            if ((icache_refill_request == `TRUE) && (icache_refill_ready == `FALSE))
            begin
                // Read first word of cache line
`ifdef CFG_HW_DEBUG_ENABLED     
                i_sel_o <= 4'b1111;
`endif
                i_adr_o <= {first_address, 2'b00};
                i_cyc_o <= `TRUE;
                i_stb_o <= `TRUE;                
                i_cti_o <= first_cycle_type;
                //i_lock_o <= `TRUE;
`ifdef CFG_BUS_ERRORS_ENABLED
                bus_error_f <= `FALSE;
`endif
            end
`ifdef CFG_HW_DEBUG_ENABLED
            else
            begin
                if ((jtag_read_enable == `TRUE) || (jtag_write_enable == `TRUE))
                begin
                    case (jtag_address[1:0])
                    2'b00: i_sel_o <= 4'b1000;
                    2'b01: i_sel_o <= 4'b0100;
                    2'b10: i_sel_o <= 4'b0010;
                    2'b11: i_sel_o <= 4'b0001;
                    endcase
                    i_adr_o <= jtag_address;
                    i_dat_o <= {4{jtag_write_data}};
                    i_cyc_o <= `TRUE;
                    i_stb_o <= `TRUE;
                    i_we_o <= jtag_write_enable;
                    i_cti_o <= `LM32_CTYPE_END;
                    jtag_access <= `TRUE;
                end
            end 
`endif                    
`ifdef CFG_BUS_ERRORS_ENABLED
            // Clear bus error when exception taken, otherwise they would be 
            // continually generated if exception handler is cached
`ifdef CFG_FAST_UNCONDITIONAL_BRANCH    
            if (branch_taken_x == `TRUE)
                bus_error_f <= `FALSE;
`endif
            if (branch_taken_m == `TRUE)
                bus_error_f <= `FALSE;
`endif
        end
    end
end
`else
always @(posedge clk_i `CFG_RESET_SENSITIVITY)
begin
    if (rst_i == `TRUE)
    begin
        i_cyc_o <= `FALSE;
        i_stb_o <= `FALSE;
        i_adr_o <= {`LM32_WORD_WIDTH{1'b0}};
        i_cti_o <= `LM32_CTYPE_CLASSIC;
        i_lock_o <= `FALSE;
        wb_data_f <= {`LM32_INSTRUCTION_WIDTH{1'b0}};
`ifdef CFG_BUS_ERRORS_ENABLED
        bus_error_f <= `FALSE;
`endif
    end
    else
    begin   
        // Is a cycle in progress?
        if (i_cyc_o == `TRUE)
        begin
            // Has cycle completed?
            if((i_ack_i == `TRUE) || (i_err_i == `TRUE))
            begin
                // Cycle complete
                i_cyc_o <= `FALSE;
                i_stb_o <= `FALSE;
                // Register fetched instruction
                wb_data_f <= i_dat_i;
            end
`ifdef CFG_BUS_ERRORS_ENABLED
            if (i_err_i == `TRUE)
            begin
                bus_error_f <= `TRUE;
                $display ("Instruction bus error. Address: %x", i_adr_o);
            end
`endif
        end
        else
        begin
            // Wait for an instruction fetch from an external address 
            if (   (stall_a == `FALSE) 
`ifdef CFG_IROM_ENABLED 
                && (irom_select_a == `FALSE)
`endif       
               )
            begin
                // Fetch instruction
`ifdef CFG_HW_DEBUG_ENABLED     
                i_sel_o <= 4'b1111;
`endif
                i_adr_o <= {pc_a, 2'b00};
                i_cyc_o <= `TRUE;
                i_stb_o <= `TRUE;
`ifdef CFG_BUS_ERRORS_ENABLED
                bus_error_f <= `FALSE;
`endif
            end
        end
    end
end
`endif
`endif

// Instruction register
always @(posedge clk_i `CFG_RESET_SENSITIVITY)
begin
    if (rst_i == `TRUE)
    begin
        instruction_d <= {`LM32_INSTRUCTION_WIDTH{1'b0}};
`ifdef CFG_BUS_ERRORS_ENABLED
        bus_error_d <= `FALSE;
`endif
    end
    else
    begin
        if (stall_d == `FALSE)
        begin
            instruction_d <= instruction_f;
`ifdef CFG_BUS_ERRORS_ENABLED
            bus_error_d <= bus_error_f;
`endif
        end
    end
end  
  
endmodule
