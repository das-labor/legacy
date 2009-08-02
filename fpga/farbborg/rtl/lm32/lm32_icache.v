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
// File             : lm32_icache.v
// Title            : Instruction cache
// Dependencies     : lm32_include.v
// Version          : 6.1.17
// =============================================================================
					  
`include "lm32_include.v"

`define LM32_IC_ADDR_OFFSET_RNG          addr_offset_msb:addr_offset_lsb
`define LM32_IC_ADDR_SET_RNG             addr_set_msb:addr_set_lsb
`define LM32_IC_ADDR_TAG_RNG             addr_tag_msb:addr_tag_lsb
`define LM32_IC_ADDR_IDX_RNG             addr_set_msb:addr_offset_lsb

`define LM32_IC_TMEM_ADDR_WIDTH          addr_set_width
`define LM32_IC_TMEM_ADDR_RNG            (`LM32_IC_TMEM_ADDR_WIDTH-1):0
`define LM32_IC_DMEM_ADDR_WIDTH          (addr_offset_width+addr_set_width)
`define LM32_IC_DMEM_ADDR_RNG            (`LM32_IC_DMEM_ADDR_WIDTH-1):0

`define LM32_IC_TAGS_WIDTH               (addr_tag_width+1)
`define LM32_IC_TAGS_RNG                 (`LM32_IC_TAGS_WIDTH-1):0
`define LM32_IC_TAGS_TAG_RNG             (`LM32_IC_TAGS_WIDTH-1):1
`define LM32_IC_TAGS_VALID_RNG           0

`define LM32_IC_STATE_RNG                3:0
`define LM32_IC_STATE_FLUSH_INIT         4'b0001
`define LM32_IC_STATE_FLUSH              4'b0010
`define LM32_IC_STATE_CHECK              4'b0100
`define LM32_IC_STATE_REFILL             4'b1000

/////////////////////////////////////////////////////
// Module interface
/////////////////////////////////////////////////////

module lm32_icache ( 
    // ----- Inputs -----
    clk_i,
    rst_i,    
    stall_a,
    stall_f,
    address_a,
    address_f,
    read_enable_f,
    refill_ready,
    refill_data,
    iflush,
    // ----- Outputs -----
    stall_request,
    restart_request,
    refill_request,
    refill_address,
    refilling,
    inst
    );

/////////////////////////////////////////////////////
// Parameters
/////////////////////////////////////////////////////

parameter associativity = 1;                            // Associativity of the cache (Number of ways)
parameter sets = 512;                                   // Number of sets
parameter bytes_per_line = 16;                          // Number of bytes per cache line
parameter base_address = 0;                             // Base address of cachable memory
parameter limit = 0;                                    // Limit (highest address) of cachable memory

localparam addr_offset_width = 2;
localparam addr_set_width = 9;
localparam addr_offset_lsb = 2;
localparam addr_offset_msb = (addr_offset_lsb+addr_offset_width-1);
localparam addr_set_lsb = (addr_offset_msb+1);
localparam addr_set_msb = (addr_set_lsb+addr_set_width-1);
localparam addr_tag_lsb = (addr_set_msb+1);
localparam addr_tag_msb = 31;
localparam addr_tag_width = (addr_tag_msb-addr_tag_lsb+1);

/////////////////////////////////////////////////////
// Inputs
/////////////////////////////////////////////////////

input clk_i;                                        // Clock 
input rst_i;                                        // Reset

input stall_a;                                      // Stall instruction in A stage
input stall_f;                                      // Stall instruction in F stage

input [`LM32_PC_RNG] address_a;                     // Address of instruction in A stage
input [`LM32_PC_RNG] address_f;                     // Address of instruction in F stage
input read_enable_f;                                // Indicates if cache access is valid

input refill_ready;                                 // Next word of refill data is ready
input [`LM32_INSTRUCTION_RNG] refill_data;          // Data to refill the cache with

input iflush;                                       // Flush the cache

/////////////////////////////////////////////////////
// Outputs
/////////////////////////////////////////////////////

output stall_request;                               // Request to stall the pipeline
wire   stall_request;
output restart_request;                             // Request to restart instruction that caused the cache miss
reg    restart_request;
output refill_request;                              // Request to refill a cache line
wire   refill_request;
output [`LM32_PC_RNG] refill_address;               // Base address of cache refill
reg    [`LM32_PC_RNG] refill_address;               
output refilling;                                   // Indicates the instruction cache is currently refilling
reg    refilling;
output [`LM32_INSTRUCTION_RNG] inst;                // Instruction read from cache
wire   [`LM32_INSTRUCTION_RNG] inst;

/////////////////////////////////////////////////////
// Internal nets and registers 
/////////////////////////////////////////////////////

wire enable;
wire [0:associativity-1] way_mem_we;
wire [`LM32_INSTRUCTION_RNG] way_data[0:associativity-1];
wire [`LM32_IC_TAGS_TAG_RNG] way_tag[0:associativity-1];
wire [0:associativity-1] way_valid;
wire [0:associativity-1] way_match;
wire miss;

wire [`LM32_IC_TMEM_ADDR_RNG] tmem_read_address;
wire [`LM32_IC_TMEM_ADDR_RNG] tmem_write_address;
wire [`LM32_IC_DMEM_ADDR_RNG] dmem_read_address;
wire [`LM32_IC_DMEM_ADDR_RNG] dmem_write_address;
wire [`LM32_IC_TAGS_RNG] tmem_write_data;

reg [`LM32_IC_STATE_RNG] state;
wire flushing;
wire check;
wire refill;

reg [associativity-1:0] refill_way_select;
reg [`LM32_IC_ADDR_OFFSET_RNG] refill_offset;
wire last_refill;
reg [`LM32_IC_TMEM_ADDR_RNG] flush_set;

genvar i;

/////////////////////////////////////////////////////
// Functions
/////////////////////////////////////////////////////

`include "lm32_functions.v"

/////////////////////////////////////////////////////
// Instantiations
/////////////////////////////////////////////////////

generate
    for (i = 0; i < associativity; i = i + 1)
    begin : memories

// Way 0 data
lm32_ram #(
    // ----- Parameters -------
    .data_width                 (32),
    .address_width              (`LM32_IC_DMEM_ADDR_WIDTH)
  ) way_0_data_ram (
    // ----- Inputs -------
    .read_clk                   (clk_i),
    .write_clk                  (clk_i),
    .reset                      (rst_i),
    .read_address               (dmem_read_address),
    .enable_read                (enable),
    .write_address              (dmem_write_address),
    .enable_write               (`TRUE),
    .write_enable               (way_mem_we[i]),
    .write_data                 (refill_data),    
    // ----- Outputs -------
    .read_data                  (way_data[i])
    );

// Way 0 tags
lm32_ram #(
    // ----- Parameters -------
    .data_width                 (`LM32_IC_TAGS_WIDTH),
    .address_width              (`LM32_IC_TMEM_ADDR_WIDTH)
  ) way_0_tag_ram (
    // ----- Inputs -------
    .read_clk                   (clk_i),
    .write_clk                  (clk_i),
    .reset                      (rst_i),
    .read_address               (tmem_read_address),
    .enable_read                (enable),
    .write_address              (tmem_write_address),
    .enable_write               (`TRUE),
    .write_enable               (way_mem_we[i] | flushing),
    .write_data                 (tmem_write_data),
    // ----- Outputs -------
    .read_data                  ({way_tag[i], way_valid[i]})
    );

    end
endgenerate

/////////////////////////////////////////////////////
// Combinational logic
/////////////////////////////////////////////////////

// Compute which ways in the cache match the address address being read
generate
    for (i = 0; i < associativity; i = i + 1)
    begin : match
assign way_match[i] = ({way_tag[i], way_valid[i]} == {address_f[`LM32_IC_ADDR_TAG_RNG], `TRUE});
    end
endgenerate

// Select data from way that matched the address being read     
generate
    if (associativity == 1)
    begin : inst_1
assign inst = way_data[0];
    end
    else if (associativity == 2)
	 begin : inst_2
assign inst = way_match[0] ? way_data[0] : way_data[1];
    end
endgenerate

// Compute address to use to index into the data memories
generate 
    if (bytes_per_line > 4)
	begin : wadr1
assign dmem_write_address = {refill_address[`LM32_IC_ADDR_SET_RNG], refill_offset};
	end
    else
	begin : wadr2
assign dmem_write_address = refill_address[`LM32_IC_ADDR_SET_RNG];
	end
endgenerate
    
assign dmem_read_address = address_a[`LM32_IC_ADDR_IDX_RNG];

// Compute address to use to index into the tag memories                        
assign tmem_read_address = address_a[`LM32_IC_ADDR_SET_RNG];
assign tmem_write_address = flushing 
                                ? flush_set
                                : refill_address[`LM32_IC_ADDR_SET_RNG];

// Compute signal to indicate when we are on the last refill accesses
generate 
    if (bytes_per_line > 4)                            
	begin : lrefil1
assign last_refill = refill_offset == {addr_offset_width{1'b1}};
	end
    else
	begin : lrefil2
assign last_refill = `TRUE;
	end
endgenerate

// Compute data and tag memory access enable
assign enable = (stall_a == `FALSE);

// Compute data and tag memory write enables
generate
    if (associativity == 1) 
    begin : we_1     
assign way_mem_we[0] = (refill_ready == `TRUE);
    end
    else
    begin : we_2
assign way_mem_we[0] = (refill_ready == `TRUE) && (refill_way_select[0] == `TRUE);
assign way_mem_we[1] = (refill_ready == `TRUE) && (refill_way_select[1] == `TRUE);
    end
endgenerate                     

// On the last refill cycle set the valid bit, for all other writes it should be cleared
assign tmem_write_data[`LM32_IC_TAGS_VALID_RNG] = last_refill & !flushing;
assign tmem_write_data[`LM32_IC_TAGS_TAG_RNG] = refill_address[`LM32_IC_ADDR_TAG_RNG];

// Signals that indicate which state we are in
assign flushing = |state[1:0];
assign check = state[2];
assign refill = state[3];

assign miss = (~(|way_match)) && (read_enable_f == `TRUE) && (stall_f == `FALSE);
assign stall_request = (check == `FALSE);
assign refill_request = (refill == `TRUE);
                      
/////////////////////////////////////////////////////
// Sequential logic
/////////////////////////////////////////////////////

// Record way selected for replacement on a cache miss
generate
    if (associativity >= 2) 
    begin : way_select      
always @(posedge clk_i `CFG_RESET_SENSITIVITY)
begin
    if (rst_i == `TRUE)
        refill_way_select <= {{associativity-1{1'b0}}, 1'b1};
    else
    begin        
        if (miss == `TRUE)
            refill_way_select <= {refill_way_select[0], refill_way_select[1]};
    end
end
    end
endgenerate

// Record whether we are refilling
always @(posedge clk_i `CFG_RESET_SENSITIVITY)
begin
    if (rst_i == `TRUE)
        refilling <= `FALSE;
    else
        refilling <= refill;
end

// Instruction cache control FSM
always @(posedge clk_i `CFG_RESET_SENSITIVITY)
begin
    if (rst_i == `TRUE)
    begin
        state <= `LM32_IC_STATE_FLUSH_INIT;
        flush_set <= {`LM32_IC_TMEM_ADDR_WIDTH{1'b1}};
        refill_address <= {`LM32_PC_WIDTH{1'bx}};
        restart_request <= `FALSE;
    end
    else 
    begin
        case (state)

        // Flush the cache for the first time after reset
        `LM32_IC_STATE_FLUSH_INIT:
        begin            
            if (flush_set == {`LM32_IC_TMEM_ADDR_WIDTH{1'b0}})
                state <= `LM32_IC_STATE_CHECK;
            flush_set <= flush_set - 1'b1;
        end

        // Flush the cache in response to an write to the ICC CSR
        `LM32_IC_STATE_FLUSH:
        begin            
            if (flush_set == {`LM32_IC_TMEM_ADDR_WIDTH{1'b0}})
                state <= `LM32_IC_STATE_REFILL;
            flush_set <= flush_set - 1'b1;
        end
        
        // Check for cache misses
        `LM32_IC_STATE_CHECK:
        begin            
            if (stall_a == `FALSE)
                restart_request <= `FALSE;
            if (iflush == `TRUE)
            begin
                refill_address <= address_f;
                state <= `LM32_IC_STATE_FLUSH;
            end
            else if (miss == `TRUE)
            begin
                refill_address <= address_f;
                state <= `LM32_IC_STATE_REFILL;
            end
        end

        // Refill a cache line
        `LM32_IC_STATE_REFILL:
        begin            
            if (refill_ready == `TRUE)
            begin
                if (last_refill == `TRUE)
                begin
                    restart_request <= `TRUE;
                    state <= `LM32_IC_STATE_CHECK;
                end
            end
        end

        endcase        
    end
end

generate 
    if (bytes_per_line > 4)
    begin : refilof1
// Refill offset
always @(posedge clk_i `CFG_RESET_SENSITIVITY)
begin
    if (rst_i == `TRUE)
        refill_offset <= {addr_offset_width{1'b0}};
    else 
    begin
        case (state)
        
        // Check for cache misses
        `LM32_IC_STATE_CHECK:
        begin            
            if (iflush == `TRUE)
                refill_offset <= {addr_offset_width{1'b0}};
            else if (miss == `TRUE)
                refill_offset <= {addr_offset_width{1'b0}};
        end

        // Refill a cache line
        `LM32_IC_STATE_REFILL:
        begin            
            if (refill_ready == `TRUE)
                refill_offset <= refill_offset + 1'b1;
        end

        endcase        
    end
end
    end
endgenerate
   
endmodule
