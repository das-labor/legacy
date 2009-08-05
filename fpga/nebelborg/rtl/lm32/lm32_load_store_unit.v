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
// File             : lm32_load_store_unit.v
// Title            : Load and store unit
// Dependencies     : lm32_include.v
// Version          : 6.0.13
// =============================================================================

`include "lm32_include.v"

/////////////////////////////////////////////////////
// Module interface
/////////////////////////////////////////////////////

module lm32_load_store_unit (
    // ----- Inputs -------
    clk_i,
    rst_i,
    // From pipeline
    stall_a,
    stall_x,
    stall_m,
    kill_x,
    kill_m,
    exception_m,
    store_operand_x,
    load_store_address_x,
    load_store_address_m,
    load_store_address_w,
    load_q_x,
    load_q_m,
    store_q_m,
    sign_extend_x,
    size_x,
`ifdef CFG_DCACHE_ENABLED
    dflush,
`endif
    // From Wishbone
    d_dat_i,
    d_ack_i,
    d_err_i,
    d_rty_i,
    // ----- Outputs -------
    // To pipeline
`ifdef CFG_DCACHE_ENABLED
    dcache_refill_request,
    dcache_restart_request,
    dcache_stall_request,
    dcache_refilling,
`endif    
    load_data_w,
    stall_wb_load,
    // To Wishbone
    d_dat_o,
    d_adr_o,
    d_cyc_o,
    d_sel_o,
    d_stb_o,
    d_we_o,
    d_cti_o,
    d_lock_o,
    d_bte_o
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
//localparam addr_offset_width = bytes_per_line == 4 ? 1 : clogb2(bytes_per_line)-1-2;
localparam addr_offset_width = 2;
localparam addr_offset_lsb = 2;
localparam addr_offset_msb = (addr_offset_lsb+addr_offset_width-1);

/////////////////////////////////////////////////////
// Inputs
/////////////////////////////////////////////////////

input clk_i;                                            // Clock 
input rst_i;                                            // Reset

input stall_a;                                          // A stage stall 
input stall_x;                                          // X stage stall        
input stall_m;                                          // M stage stall
input kill_x;                                           // Kill instruction in X stage
input kill_m;                                           // Kill instruction in M stage
input exception_m;                                      // An exception occured in the M stage

input [`LM32_WORD_RNG] store_operand_x;                 // Data read from register to store
input [`LM32_WORD_RNG] load_store_address_x;            // X stage load/store address
input [`LM32_WORD_RNG] load_store_address_m;            // M stage load/store address
input [1:0] load_store_address_w;                       // W stage load/store address (only least two significant bits are needed)
input load_q_x;                                         // Load instruction in X stage
input load_q_m;                                         // Load instruction in M stage
input store_q_m;                                        // Store instruction in M stage
input sign_extend_x;                                    // Whether load instruction in X stage should sign extend or zero extend
input [`LM32_SIZE_RNG] size_x;                          // Size of load or store (byte, hword, word)

`ifdef CFG_DCACHE_ENABLED
input dflush;                                           // Flush the data cache
`endif

input [`LM32_WORD_RNG] d_dat_i;                         // Data Wishbone interface read data
input d_ack_i;                                          // Data Wishbone interface acknowledgement
input d_err_i;                                          // Data Wishbone interface error
input d_rty_i;                                          // Data Wishbone interface retry

/////////////////////////////////////////////////////
// Outputs
/////////////////////////////////////////////////////

`ifdef CFG_DCACHE_ENABLED
output dcache_refill_request;                           // Request to refill data cache
wire   dcache_refill_request;
output dcache_restart_request;                          // Request to restart the instruction that caused a data cache miss
wire   dcache_restart_request;
output dcache_stall_request;                            // Data cache stall request
wire   dcache_stall_request;
output dcache_refilling;
wire   dcache_refilling;
`endif

output [`LM32_WORD_RNG] load_data_w;                    // Result of a load instruction
reg    [`LM32_WORD_RNG] load_data_w;
output stall_wb_load;                                   // Request to stall pipeline due to a load from the Wishbone interface
reg    stall_wb_load;

output [`LM32_WORD_RNG] d_dat_o;                        // Data Wishbone interface write data
reg    [`LM32_WORD_RNG] d_dat_o;
output [`LM32_WORD_RNG] d_adr_o;                        // Data Wishbone interface address
reg    [`LM32_WORD_RNG] d_adr_o;
output d_cyc_o;                                         // Data Wishbone interface cycle
reg    d_cyc_o;
output [`LM32_BYTE_SELECT_RNG] d_sel_o;                 // Data Wishbone interface byte select
reg    [`LM32_BYTE_SELECT_RNG] d_sel_o;
output d_stb_o;                                         // Data Wishbone interface strobe
reg    d_stb_o; 
output d_we_o;                                          // Data Wishbone interface write enable
reg    d_we_o;
output [`LM32_CTYPE_RNG] d_cti_o;                       // Data Wishbone interface cycle type 
wire   [`LM32_CTYPE_RNG] d_cti_o;
output d_lock_o;                                        // Date Wishbone interface lock bus
wire   d_lock_o;
output [`LM32_BTYPE_RNG] d_bte_o;                       // Data Wishbone interface burst type 
wire   [`LM32_BTYPE_RNG] d_bte_o;

/////////////////////////////////////////////////////
// Internal nets and registers 
/////////////////////////////////////////////////////

// Microcode pipeline registers - See inputs for description
reg [`LM32_SIZE_RNG] size_m;
reg [`LM32_SIZE_RNG] size_w;
reg sign_extend_m;
reg sign_extend_w;
reg [`LM32_WORD_RNG] store_data_x;       
reg [`LM32_WORD_RNG] store_data_m;       
reg [`LM32_BYTE_SELECT_RNG] byte_enable_x;
reg [`LM32_BYTE_SELECT_RNG] byte_enable_m;
wire [`LM32_WORD_RNG] data_m;
reg [`LM32_WORD_RNG] data_w;

`ifdef CFG_DCACHE_ENABLED
wire dcache_select_x;                                   // Select data cache to load from / store to
reg dcache_select_m;
wire [`LM32_WORD_RNG] dcache_data_m;                    // Data read from cache
wire [`LM32_WORD_RNG] dcache_refill_address;            // Address to refill data cache from
reg dcache_refill_ready;                                // Indicates the next word of refill data is ready
wire last_word;                                         // Indicates if this is the last word in the cache line
wire [`LM32_WORD_RNG] first_address;                    // First cache refill address
`endif
`ifdef CFG_DRAM_ENABLED
wire dram_select_x;                                     // Select data RAM to load from / store to
reg dram_select_m;
wire [`LM32_WORD_RNG] dram_data_m;                      // Data read from data RAM
wire [`LM32_WORD_RNG] dram_store_data_m;                // Data to write to RAM
`endif
wire wb_select_x;                                       // Select Wishbone to load from / store to
reg wb_select_m;
reg [`LM32_WORD_RNG] wb_data_m;                         // Data read from Wishbone
reg wb_load_complete;                                   // Indicates when a Wishbone load is complete

/////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////

`include "lm32_functions.v"

/////////////////////////////////////////////////////
// Instantiations
/////////////////////////////////////////////////////

`ifdef CFG_DRAM_ENABLED
// Data RAM
pmi_ram_dp_true #(
    // ----- Parameters -------
    .pmi_addr_depth_a       (1 << (clogb2(`CFG_DRAM_LIMIT/4-`CFG_DRAM_BASE_ADDRESS/4+1)-1)),
    .pmi_addr_width_a       ((clogb2(`CFG_DRAM_LIMIT/4-`CFG_DRAM_BASE_ADDRESS/4+1)-1)),
    .pmi_data_width_a       (`LM32_WORD_WIDTH),
    .pmi_addr_depth_b       (1 << (clogb2(`CFG_DRAM_LIMIT/4-`CFG_DRAM_BASE_ADDRESS/4+1)-1)),
    .pmi_addr_width_b       ((clogb2(`CFG_DRAM_LIMIT/4-`CFG_DRAM_BASE_ADDRESS/4+1)-1)),
    .pmi_data_width_b       (`LM32_WORD_WIDTH),
    .pmi_regmode_a          ("noreg"),
    .pmi_regmode_b          ("noreg"),
    .pmi_gsr                ("enable"),
    .pmi_resetmode          ("async"),
    .pmi_init_file          (`CFG_DRAM_INIT_FILE),
    .pmi_init_file_format   ("hex"),
    .module_type            ("pmi_ram_dp")
  ) ram (
    // ----- Inputs -------
    .ClockA                 (clk_i),
    .ClockB                 (clk_i),
    .ResetA                 (rst_i),
    .ResetB                 (rst_i),
    .DataInA                ({32{1'b0}}),
    .DataInB                (dram_store_data_m),
    .AddressA               (load_store_address_x[(clogb2(`CFG_DRAM_LIMIT/4-`CFG_DRAM_BASE_ADDRESS/4+1)-1)+2-1:2]),
    .AddressB               (load_store_address_m[(clogb2(`CFG_DRAM_LIMIT/4-`CFG_DRAM_BASE_ADDRESS/4+1)-1)+2-1:2]),
    .ClockEnA               (!stall_x),
    .ClockEnB               (!stall_m),
    .WrA                    (`FALSE),
    .WrB                    (store_q_m), 
    // ----- Outputs -------
    .QA                     (dram_data_m),
    .QB                     ()
    );
`endif

`ifdef CFG_DCACHE_ENABLED
// Data cache
lm32_dcache #(
    .associativity          (associativity),
    .sets                   (sets),
    .bytes_per_line         (bytes_per_line),
    .base_address           (base_address),
    .limit                  (limit)
    ) dcache ( 
    // ----- Inputs -----
    .clk_i                  (clk_i),
    .rst_i                  (rst_i),      
    .stall_a                (stall_a),
    .stall_x                (stall_x),
    .stall_m                (stall_m),
    .address_x              (load_store_address_x),
    .address_m              (load_store_address_m),
    .load_q_m               (load_q_m & dcache_select_m),
    .store_q_m              (store_q_m),
    .store_data             (store_data_m),
    .store_byte_select      (byte_enable_m),
    .refill_ready           (dcache_refill_ready),
    .refill_data            (wb_data_m),
    .dflush                 (dflush),
    // ----- Outputs -----
    .stall_request          (dcache_stall_request),
    .restart_request        (dcache_restart_request),
    .refill_request         (dcache_refill_request),
    .refill_address         (dcache_refill_address),
    .refilling              (dcache_refilling),
    .load_data              (dcache_data_m)
    );
`endif

/////////////////////////////////////////////////////
// Combinational Logic
/////////////////////////////////////////////////////

// Select where data should be loaded from / stored to
`ifdef CFG_DCACHE_ENABLED
assign dcache_select_x = (load_store_address_x >= `CFG_DCACHE_BASE_ADDRESS) && (load_store_address_x <= `CFG_DCACHE_LIMIT);
//assign dcache_select_x = (load_store_address_x >= `CFG_DCACHE_BASE_ADDRESS) && ~|load_store_address_x[`LM32_WORD_WIDTH-1:clogb2(`CFG_DCACHE_LIMIT-`CFG_DCACHE_BASE_ADDRESS)-1];
`endif
`ifdef CFG_DRAM_ENABLED
assign dram_select_x = (load_store_address_x >= `CFG_DRAM_BASE_ADDRESS) && (load_store_address_x <= `CFG_DRAM_LIMIT);
`endif
assign wb_select_x =    `TRUE
`ifdef CFG_DCACHE_ENABLED
                     && !dcache_select_x 
`endif
`ifdef CFG_DRAM_ENABLED
                     && !dram_select_x
`endif
                     ;

// Make sure data to store is in correct byte lane
always @(*)
begin
    case (size_x)
    `LM32_SIZE_BYTE:  store_data_x = {4{store_operand_x[7:0]}};
    `LM32_SIZE_HWORD: store_data_x = {2{store_operand_x[15:0]}};
    `LM32_SIZE_WORD:  store_data_x = store_operand_x;    
    default:          store_data_x = {`LM32_WORD_WIDTH{1'bx}};
    endcase
end

// Generate byte enable accoring to size of load or store and address being accessed
always @(*)
begin
    casez ({size_x, load_store_address_x[1:0]})
    {`LM32_SIZE_BYTE, 2'b11}:  byte_enable_x = 4'b0001;
    {`LM32_SIZE_BYTE, 2'b10}:  byte_enable_x = 4'b0010;
    {`LM32_SIZE_BYTE, 2'b01}:  byte_enable_x = 4'b0100;
    {`LM32_SIZE_BYTE, 2'b00}:  byte_enable_x = 4'b1000;
    {`LM32_SIZE_HWORD, 2'b1?}: byte_enable_x = 4'b0011;
    {`LM32_SIZE_HWORD, 2'b0?}: byte_enable_x = 4'b1100;
    {`LM32_SIZE_WORD, 2'b??}:  byte_enable_x = 4'b1111;
    default:                   byte_enable_x = 4'bxxxx;
    endcase
end

`ifdef CFG_DRAM_ENABLED
// Only replace selected bytes
assign dram_store_data_m[`LM32_BYTE_0_RNG] = byte_enable_m[0] ? store_data_m[`LM32_BYTE_0_RNG] : dram_data_m[`LM32_BYTE_0_RNG];
assign dram_store_data_m[`LM32_BYTE_1_RNG] = byte_enable_m[1] ? store_data_m[`LM32_BYTE_1_RNG] : dram_data_m[`LM32_BYTE_1_RNG];
assign dram_store_data_m[`LM32_BYTE_2_RNG] = byte_enable_m[2] ? store_data_m[`LM32_BYTE_2_RNG] : dram_data_m[`LM32_BYTE_2_RNG];
assign dram_store_data_m[`LM32_BYTE_3_RNG] = byte_enable_m[3] ? store_data_m[`LM32_BYTE_3_RNG] : dram_data_m[`LM32_BYTE_3_RNG];
`endif

// Select data from selected source
`ifdef CFG_DCACHE_ENABLED
`ifdef CFG_DRAM_ENABLED
assign data_m = wb_select_m == `TRUE 
                ? wb_data_m
                : dram_select_m == `TRUE 
                   ? dram_data_m
                   : dcache_data_m;
`else
assign data_m = wb_select_m == `TRUE ? wb_data_m : dcache_data_m;
`endif
`else
`ifdef CFG_DRAM_ENABLED
assign data_m = wb_select_m == `TRUE ? wb_data_m : dram_data_m;
`else
assign data_m = wb_data_m;
`endif
`endif

// Sub-word selection and sign/zero-extension for loads
always @(*)
begin
    casez ({size_w, load_store_address_w[1:0]})
    {`LM32_SIZE_BYTE, 2'b11}:  load_data_w = {{24{sign_extend_w & data_w[7]}}, data_w[7:0]};
    {`LM32_SIZE_BYTE, 2'b10}:  load_data_w = {{24{sign_extend_w & data_w[15]}}, data_w[15:8]};
    {`LM32_SIZE_BYTE, 2'b01}:  load_data_w = {{24{sign_extend_w & data_w[23]}}, data_w[23:16]};
    {`LM32_SIZE_BYTE, 2'b00}:  load_data_w = {{24{sign_extend_w & data_w[31]}}, data_w[31:24]};
    {`LM32_SIZE_HWORD, 2'b1?}: load_data_w = {{16{sign_extend_w & data_w[15]}}, data_w[15:0]};
    {`LM32_SIZE_HWORD, 2'b0?}: load_data_w = {{16{sign_extend_w & data_w[31]}}, data_w[31:16]};
    {`LM32_SIZE_WORD, 2'b??}:  load_data_w = data_w;
    default:                   load_data_w = {`LM32_WORD_WIDTH{1'bx}};
    endcase
end

// Unused Wishbone signals
assign d_cti_o = `LM32_CTYPE_WIDTH'd0;
assign d_lock_o = `FALSE;
assign d_bte_o = `LM32_BTYPE_WIDTH'd0;

`ifdef CFG_DCACHE_ENABLED                
// Generate signal to indicate last word in cache line
generate 
        if (bytes_per_line > 4)
        begin
assign first_address = {dcache_refill_address[`LM32_WORD_WIDTH-1:addr_offset_msb+1], {addr_offset_width{1'b0}}, 2'b00};
assign last_word = (&d_adr_o[addr_offset_msb:addr_offset_lsb]) == 1'b1;
        end
        else
        begin
assign first_address = {dcache_refill_address[`LM32_WORD_WIDTH-1:2], 2'b00};
assign last_word = `TRUE;
        end
endgenerate
`endif

/////////////////////////////////////////////////////
// Sequential Logic
/////////////////////////////////////////////////////

// Data Wishbone interface
always @(posedge clk_i `CFG_RESET_SENSITIVITY)
begin
    if (rst_i == `TRUE)
    begin
        d_cyc_o <= `FALSE;
        d_stb_o <= `FALSE;
        d_dat_o <= {`LM32_WORD_WIDTH{1'b0}};
        d_adr_o <= {`LM32_WORD_WIDTH{1'b0}};
        d_sel_o <= {`LM32_BYTE_SELECT_WIDTH{`FALSE}};
        d_we_o <= `FALSE;
        wb_data_m <= {`LM32_WORD_WIDTH{1'b0}};
        wb_load_complete <= `FALSE;
        stall_wb_load <= `FALSE;
`ifdef CFG_DCACHE_ENABLED                
        dcache_refill_ready <= `FALSE;
`endif                
    end
    else
    begin
`ifdef CFG_DCACHE_ENABLED 
        // Refill ready should only be asserted for a single cycle               
        dcache_refill_ready <= `FALSE;
`endif                
        // Is a Wishbone cycle already in progress?
        if (d_cyc_o == `TRUE)
        begin
            // Is the cycle complete?
            if ((d_ack_i == `TRUE) || (d_err_i == `TRUE))
            begin
`ifdef CFG_DCACHE_ENABLED                
                if ((dcache_refilling == `TRUE) && (!last_word))
                begin
                    // Fetch next word of cache line    
                    d_adr_o[addr_offset_msb:addr_offset_lsb] <= d_adr_o[addr_offset_msb:addr_offset_lsb] + 1'b1;
                end
                else
`endif                
                begin
                    // Refill/access complete
                    d_cyc_o <= `FALSE;
                    d_stb_o <= `FALSE;
                end
`ifdef CFG_DCACHE_ENABLED    
                // If we are performing a refill, indicate to cache next word of data is ready            
                dcache_refill_ready <= dcache_refilling;
`endif
                // Register data read from Wishbone interface
                wb_data_m <= d_dat_i;
                // Don't set when stores complete - otherwise we'll deadlock if load in m stage
                wb_load_complete <= !d_we_o;
            end
            // synthesis translate_off            
            if (d_err_i == `TRUE)
                $display ("Data bus error. Address: %x", d_adr_o);
            // synthesis translate_on
        end
        else
        begin
`ifdef CFG_DCACHE_ENABLED                
            if (dcache_refill_request == `TRUE)
            begin
                // Start cache refill
                d_adr_o <= first_address;
                d_cyc_o <= `TRUE;
                d_sel_o <= {`LM32_WORD_WIDTH/8{`TRUE}};
                d_stb_o <= `TRUE;                
                d_we_o <= `FALSE;
            end
            else 
`endif            
                 if (   (store_q_m == `TRUE)
                     && (stall_m == `FALSE)
`ifdef CFG_DRAM_ENABLED
                     && !dram_select_m
`endif
                    )
            begin
                // Data cache is write through, so all stores go to memory
                d_dat_o <= store_data_m;
                d_adr_o <= load_store_address_m;
                d_cyc_o <= `TRUE;
                d_sel_o <= byte_enable_m;
                d_stb_o <= `TRUE;
                d_we_o <= `TRUE;
            end        
            else if (   (load_q_m == `TRUE) 
                     && (wb_select_m == `TRUE) 
                     && (wb_load_complete == `FALSE)
                     /* stall_m will be TRUE, because stall_wb_load will be TRUE */
                    )
            begin
                // Read requested address
                stall_wb_load <= `FALSE;
                d_adr_o <= load_store_address_m;
                d_cyc_o <= `TRUE;
                d_sel_o <= byte_enable_m;
                d_stb_o <= `TRUE;
                d_we_o <= `FALSE;
            end
        end
        // Clear load/store complete flag when instruction leaves M stage
        if (stall_m == `FALSE)
            wb_load_complete <= `FALSE;
        // When a Wishbone load first enters the M stage, we need to stall it
        if ((load_q_x == `TRUE) && (wb_select_x == `TRUE) && (stall_x == `FALSE))
            stall_wb_load <= `TRUE;
        // Clear stall request if load instruction is killed
        if ((kill_m == `TRUE) || (exception_m == `TRUE))
            stall_wb_load <= `FALSE;
    end
end

// Pipeline registers  

// X/M stage pipeline registers
always @(posedge clk_i `CFG_RESET_SENSITIVITY)
begin
    if (rst_i == `TRUE)
    begin
        sign_extend_m <= `FALSE;
        size_m <= 2'b00;
        byte_enable_m <= `FALSE;
        store_data_m <= {`LM32_WORD_WIDTH{1'b0}};
`ifdef CFG_DCACHE_ENABLED
        dcache_select_m <= `FALSE;
`endif
`ifdef CFG_DRAM_ENABLED
        dram_select_m <= `FALSE;
`endif
        wb_select_m <= `FALSE;        
    end
    else
    begin
        if (stall_m == `FALSE)
        begin
            sign_extend_m <= sign_extend_x;
            size_m <= size_x;
            byte_enable_m <= byte_enable_x;    
            store_data_m <= store_data_x;
`ifdef CFG_DCACHE_ENABLED
            dcache_select_m <= dcache_select_x;
`endif
`ifdef CFG_DRAM_ENABLED
            dram_select_m <= dram_select_x;
`endif
            wb_select_m <= wb_select_x;
        end
    end
end

// M/W stage pipeline registers
always @(posedge clk_i `CFG_RESET_SENSITIVITY)
begin
    if (rst_i == `TRUE)
    begin
        size_w <= 2'b00;
        data_w <= {`LM32_WORD_WIDTH{1'b0}};
        sign_extend_w <= `FALSE;
    end
    else
    begin
        size_w <= size_m;
        data_w <= data_m;
        sign_extend_w <= sign_extend_m;
    end
end

/////////////////////////////////////////////////////
// Behavioural Logic
/////////////////////////////////////////////////////

// synthesis translate_off

// Check for non-aligned loads or stores
always @(posedge clk_i)
begin
    if (((load_q_m == `TRUE) || (store_q_m == `TRUE)) && (stall_m == `FALSE)) 
    begin
        if ((size_m === `LM32_SIZE_HWORD) && (load_store_address_m[0] !== 1'b0))
            $display ("Warning: Non-aligned halfword access. Address: 0x%0x Time: %0t.", load_store_address_m, $time);
        if ((size_m === `LM32_SIZE_WORD) && (load_store_address_m[1:0] !== 2'b00))
            $display ("Warning: Non-aligned word access. Address: 0x%0x Time: %0t.", load_store_address_m, $time);
    end
end

// synthesis translate_on

endmodule

