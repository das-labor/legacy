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
// File             : lm32_dcache.v
// Title            : Data cache
// Dependencies     : lm32_include.v
// Version          : 6.1.17
// =============================================================================
								 
`include "lm32_include.v"

`ifdef CFG_DCACHE_ENABLED

`define LM32_DC_ADDR_OFFSET_RNG          addr_offset_msb:addr_offset_lsb
`define LM32_DC_ADDR_SET_RNG             addr_set_msb:addr_set_lsb
`define LM32_DC_ADDR_TAG_RNG             addr_tag_msb:addr_tag_lsb
`define LM32_DC_ADDR_IDX_RNG             addr_set_msb:addr_offset_lsb

`define LM32_DC_TMEM_ADDR_WIDTH          addr_set_width
`define LM32_DC_TMEM_ADDR_RNG            (`LM32_DC_TMEM_ADDR_WIDTH-1):0
`define LM32_DC_DMEM_ADDR_WIDTH          (addr_offset_width+addr_set_width)
`define LM32_DC_DMEM_ADDR_RNG            (`LM32_DC_DMEM_ADDR_WIDTH-1):0

`define LM32_DC_TAGS_WIDTH               (addr_tag_width+1)
`define LM32_DC_TAGS_RNG                 (`LM32_DC_TAGS_WIDTH-1):0
`define LM32_DC_TAGS_TAG_RNG             (`LM32_DC_TAGS_WIDTH-1):1
`define LM32_DC_TAGS_VALID_RNG           0

`define LM32_DC_STATE_RNG                2:0
`define LM32_DC_STATE_FLUSH              3'b001
`define LM32_DC_STATE_CHECK              3'b010
`define LM32_DC_STATE_REFILL             3'b100

/////////////////////////////////////////////////////
// Module interface
/////////////////////////////////////////////////////

module lm32_dcache ( 
    // ----- Inputs -----
    clk_i,
    rst_i,    
    stall_a,
    stall_x,
    stall_m,
    address_x,
    address_m,
    load_q_m,
    store_q_m,
    store_data,
    store_byte_select,
    refill_ready,
    refill_data,
    dflush,
    // ----- Outputs -----
    stall_request,
    restart_request,
    refill_request,
    refill_address,
    refilling,
    load_data
    );

/////////////////////////////////////////////////////
// Parameters
/////////////////////////////////////////////////////

parameter associativity = 1;                            // Associativity of the cache (Number of ways)
parameter sets = 512;                                   // Number of sets
parameter bytes_per_line = 16;                          // Number of bytes per cache line
parameter base_address = 0;                             // Base address of cachable memory
parameter limit = 0;                                    // Limit (highest address) of cachable memory

//localparam addr_offset_width = clogb2(bytes_per_line)-1-2;
localparam addr_offset_width = 2;
//localparam addr_set_width = clogb2(sets)-1;
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

input clk_i;                                            // Clock
input rst_i;                                            // Reset

input stall_a;                                          // Stall A stage
input stall_x;                                          // Stall X stage
input stall_m;                                          // Stall M stage

input [`LM32_WORD_RNG] address_x;                       // X stage load/store address
input [`LM32_WORD_RNG] address_m;                       // M stage load/store address
input load_q_m;                                         // Load instruction in M stage
input store_q_m;                                        // Store instruction in M stage
input [`LM32_WORD_RNG] store_data;                      // Data to store
input [`LM32_BYTE_SELECT_RNG] store_byte_select;        // Which bytes in store data should be modified

input refill_ready;                                     // Indicates next word of refill data is ready
input [`LM32_WORD_RNG] refill_data;                     // Refill data

input dflush;                                           // Indicates cache should be flushed

/////////////////////////////////////////////////////
// Outputs
/////////////////////////////////////////////////////

output stall_request;                                   // Request pipeline be stalled because cache is busy
wire   stall_request;
output restart_request;                                 // Request to restart instruction that caused the cache miss
reg    restart_request;
output refill_request;                                  // Request a refill 
reg    refill_request;
output [`LM32_WORD_RNG] refill_address;                 // Address to refill from
reg    [`LM32_WORD_RNG] refill_address;
output refilling;                                       // Indicates if the cache is currently refilling
reg    refilling;
output [`LM32_WORD_RNG] load_data;                      // Data read from cache
wire   [`LM32_WORD_RNG] load_data;

/////////////////////////////////////////////////////
// Internal nets and registers 
/////////////////////////////////////////////////////

wire read_port_enable;                                  // Cache memory read port clock enable
wire write_port_enable;                                 // Cache memory write port clock enable
wire [0:associativity-1] way_tmem_we;                   // Tag memory write enable
wire [0:associativity-1] way_dmem_we;                   // Data memory write enable
wire [`LM32_WORD_RNG] way_data[0:associativity-1];      // Data read from data memory
wire [`LM32_DC_TAGS_TAG_RNG] way_tag[0:associativity-1];// Tag read from tag memory
wire [0:associativity-1] way_valid;                     // Indicates which ways are valid
wire [0:associativity-1] way_match;                     // Indicates which ways matched
wire miss;                                              // Indicates no ways matched

wire [`LM32_DC_TMEM_ADDR_RNG] tmem_read_address;        // Tag memory read address
wire [`LM32_DC_TMEM_ADDR_RNG] tmem_write_address;       // Tag memory write address
wire [`LM32_DC_DMEM_ADDR_RNG] dmem_read_address;        // Data memory read address
wire [`LM32_DC_DMEM_ADDR_RNG] dmem_write_address;       // Data memory write address
wire [`LM32_DC_TAGS_RNG] tmem_write_data;               // Tag memory write data        
reg [`LM32_WORD_RNG] dmem_write_data;                   // Data memory write data

reg [`LM32_DC_STATE_RNG] state;                         // Current state of FSM
wire flushing;                                          // Indicates if cache is currently flushing
wire check;                                             // Indicates if cache is currently checking for hits/misses
wire refill;                                            // Indicates if cache is currently refilling

wire valid_store;                                       // Indicates if there is a valid store instruction
reg [associativity-1:0] refill_way_select;              // Which way should be refilled
reg [`LM32_DC_ADDR_OFFSET_RNG] refill_offset;           // Which word in cache line should be refilled
wire last_refill;                                       // Indicates when on last cycle of cache refill
reg [`LM32_DC_TMEM_ADDR_RNG] flush_set;                 // Which set is currently being flushed

genvar i, j;

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
// Way data

        if (`LM32_DC_DMEM_ADDR_WIDTH < 7)
        begin : data_memories
lm32_ram #(
    // ----- Parameters -------
    .data_width                 (32),
    .address_width              (`LM32_DC_DMEM_ADDR_WIDTH)
  ) way_0_data_ram (
    // ----- Inputs -------
    .read_clk                   (clk_i),
    .write_clk                  (clk_i),
    .reset                      (rst_i),
    .read_address               (dmem_read_address),
    .enable_read                (read_port_enable),
    .write_address              (dmem_write_address),
    .enable_write               (write_port_enable),
    .write_enable               (way_dmem_we[i]),
    .write_data                 (dmem_write_data),    
    // ----- Outputs -------
    .read_data                  (way_data[i])
    );    
        end
        else
        begin
            for (j = 0; j < 4; j = j + 1)    
            begin : byte_memories
lm32_ram #(
    // ----- Parameters -------
    .data_width                 (8),
    .address_width              (`LM32_DC_DMEM_ADDR_WIDTH)
  ) way_0_data_ram (
    // ----- Inputs -------
    .read_clk                   (clk_i),
    .write_clk                  (clk_i),
    .reset                      (rst_i),
    .read_address               (dmem_read_address),
    .enable_read                (read_port_enable),
    .write_address              (dmem_write_address),
    .enable_write               (write_port_enable),
    .write_enable               (way_dmem_we[i] & (store_byte_select[j] | refill)),
    .write_data                 (dmem_write_data[(j+1)*8-1:j*8]),    
    // ----- Outputs -------
    .read_data                  (way_data[i][(j+1)*8-1:j*8])
    );
            end
        end

// Way tags
lm32_ram #(
    // ----- Parameters -------
    .data_width                 (`LM32_DC_TAGS_WIDTH),
    .address_width              (`LM32_DC_TMEM_ADDR_WIDTH)
  ) way_0_tag_ram (
    // ----- Inputs -------
    .read_clk                   (clk_i),
    .write_clk                  (clk_i),
    .reset                      (rst_i),
    .read_address               (tmem_read_address),
    .enable_read                (read_port_enable),
    .write_address              (tmem_write_address),
    .enable_write               (`TRUE),
    .write_enable               (way_tmem_we[i]),
    .write_data                 (tmem_write_data),
    // ----- Outputs -------
    .read_data                  ({way_tag[i], way_valid[i]})
    );

    end
    
endgenerate

/////////////////////////////////////////////////////
// Combinational logic
/////////////////////////////////////////////////////

// Compute which ways in the cache match the address being read
generate
    for (i = 0; i < associativity; i = i + 1)
    begin : match
assign way_match[i] = ({way_tag[i], way_valid[i]} == {address_m[`LM32_DC_ADDR_TAG_RNG], `TRUE});
    end
endgenerate

// Select data from way that matched the address being read     
generate
    if (associativity == 1)    
	 begin : data_1
assign load_data = way_data[0];
    end
    else if (associativity == 2)
	 begin : data_2
assign load_data = way_match[0] ? way_data[0] : way_data[1]; 
    end
endgenerate

generate
    if (`LM32_DC_DMEM_ADDR_WIDTH < 7)
    begin
// Select data to write to data memories
always @(*)
begin
    if (refill == `TRUE)
        dmem_write_data = refill_data;
    else
    begin
        dmem_write_data[`LM32_BYTE_0_RNG] = store_byte_select[0] ? store_data[`LM32_BYTE_0_RNG] : load_data[`LM32_BYTE_0_RNG];
        dmem_write_data[`LM32_BYTE_1_RNG] = store_byte_select[1] ? store_data[`LM32_BYTE_1_RNG] : load_data[`LM32_BYTE_1_RNG];
        dmem_write_data[`LM32_BYTE_2_RNG] = store_byte_select[2] ? store_data[`LM32_BYTE_2_RNG] : load_data[`LM32_BYTE_2_RNG];
        dmem_write_data[`LM32_BYTE_3_RNG] = store_byte_select[3] ? store_data[`LM32_BYTE_3_RNG] : load_data[`LM32_BYTE_3_RNG];
    end
end
    end
    else
    begin
// Select data to write to data memories - FIXME: Should use different write ports on dual port RAMs, but they don't work
always @(*)
begin
    if (refill == `TRUE)
        dmem_write_data = refill_data;
    else
        dmem_write_data = store_data;
end
    end
endgenerate

// Compute address to use to index into the data memories
generate 
     if (bytes_per_line > 4)
assign dmem_write_address = (refill == `TRUE) 
                            ? {refill_address[`LM32_DC_ADDR_SET_RNG], refill_offset}
                            : address_m[`LM32_DC_ADDR_IDX_RNG];
    else
assign dmem_write_address = (refill == `TRUE) 
                            ? refill_address[`LM32_DC_ADDR_SET_RNG]
                            : address_m[`LM32_DC_ADDR_IDX_RNG];
endgenerate
assign dmem_read_address = address_x[`LM32_DC_ADDR_IDX_RNG];
// Compute address to use to index into the tag memories   
assign tmem_write_address = (flushing == `TRUE)
                            ? flush_set
                            : refill_address[`LM32_DC_ADDR_SET_RNG];
assign tmem_read_address = address_x[`LM32_DC_ADDR_SET_RNG];

// Compute signal to indicate when we are on the last refill accesses
generate 
    if (bytes_per_line > 4)                            
assign last_refill = refill_offset == {addr_offset_width{1'b1}};
    else
assign last_refill = `TRUE;
endgenerate

// Compute data and tag memory access enable
assign read_port_enable = (stall_x == `FALSE);
assign write_port_enable = (refill_ready == `TRUE) || !stall_m;

// Determine when we have a valid store
assign valid_store = (store_q_m == `TRUE) && (check == `TRUE);

// Compute data and tag memory write enables
generate
    if (associativity == 1) 
    begin : we_1     
assign way_dmem_we[0] = (refill_ready == `TRUE) || ((valid_store == `TRUE) && (way_match[0] == `TRUE));
assign way_tmem_we[0] = (refill_ready == `TRUE) || (flushing == `TRUE);
    end 
    else 
    begin : we_2
assign way_dmem_we[0] = ((refill_ready == `TRUE) && (refill_way_select[0] == `TRUE)) || ((valid_store == `TRUE) && (way_match[0] == `TRUE));
assign way_dmem_we[1] = ((refill_ready == `TRUE) && (refill_way_select[1] == `TRUE)) || ((valid_store == `TRUE) && (way_match[1] == `TRUE));
assign way_tmem_we[0] = ((refill_ready == `TRUE) && (refill_way_select[0] == `TRUE)) || (flushing == `TRUE);
assign way_tmem_we[1] = ((refill_ready == `TRUE) && (refill_way_select[1] == `TRUE)) || (flushing == `TRUE);
    end
endgenerate

// On the last refill cycle set the valid bit, for all other writes it should be cleared
assign tmem_write_data[`LM32_DC_TAGS_VALID_RNG] = ((last_refill == `TRUE) || (valid_store == `TRUE)) && (flushing == `FALSE);
assign tmem_write_data[`LM32_DC_TAGS_TAG_RNG] = refill_address[`LM32_DC_ADDR_TAG_RNG];

// Signals that indicate which state we are in
assign flushing = state[0];
assign check = state[1];
assign refill = state[2];

assign miss = (~(|way_match)) && (load_q_m == `TRUE) && (stall_m == `FALSE);
assign stall_request = (check == `FALSE);
                      
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
        if (refill_request == `TRUE)
            refill_way_select <= {refill_way_select[0], refill_way_select[1]};
    end
end
    end 
endgenerate   

// Record whether we are currently refilling
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
        state <= `LM32_DC_STATE_FLUSH;
        flush_set <= {`LM32_DC_TMEM_ADDR_WIDTH{1'b1}};
        refill_request <= `FALSE;
        refill_address <= {`LM32_WORD_WIDTH{1'bx}};
        restart_request <= `FALSE;
    end
    else 
    begin
        case (state)

        // Flush the cache 
        `LM32_DC_STATE_FLUSH:
        begin
            if (flush_set == {`LM32_DC_TMEM_ADDR_WIDTH{1'b0}})
                state <= `LM32_DC_STATE_CHECK;
            flush_set <= flush_set - 1'b1;
        end
        
        // Check for cache misses
        `LM32_DC_STATE_CHECK:
        begin
            if (stall_a == `FALSE)
                restart_request <= `FALSE;
            if (miss == `TRUE)
            begin
                refill_request <= `TRUE;
                refill_address <= address_m;
                state <= `LM32_DC_STATE_REFILL;
            end
            else if (dflush == `TRUE)
                state <= `LM32_DC_STATE_FLUSH;
        end

        // Refill a cache line
        `LM32_DC_STATE_REFILL:
        begin
            refill_request <= `FALSE;
            if (refill_ready == `TRUE)
            begin
                if (last_refill == `TRUE)
                begin
                    restart_request <= `TRUE;
                    state <= `LM32_DC_STATE_CHECK;
                end
            end
        end
        
        endcase        
    end
end

generate
    if (bytes_per_line > 4)
    begin
// Refill offset
always @(posedge clk_i `CFG_RESET_SENSITIVITY)
begin
    if (rst_i == `TRUE)
        refill_offset <= {addr_offset_width{1'b0}};
    else 
    begin
        case (state)
        
        // Check for cache misses
        `LM32_DC_STATE_CHECK:
        begin
            if (miss == `TRUE)
                refill_offset <= {addr_offset_width{1'b0}};
        end

        // Refill a cache line
        `LM32_DC_STATE_REFILL:
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

`endif

