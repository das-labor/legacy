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
// File             : lm32_jtag.v
// Title            : JTAG interface
// Dependencies     : lm32_include.v
// Version          : 6.1.17
// =============================================================================

`include "lm32_include.v"

`ifdef CFG_JTAG_ENABLED

`define LM32_DP                             3'b000
`define LM32_TX                             3'b001
`define LM32_RX                             3'b010

// LM32 Debug Protocol commands IDs
`define LM32_DP_RNG                         3:0
`define LM32_DP_READ_MEMORY                 4'b0001
`define LM32_DP_WRITE_MEMORY                4'b0010
`define LM32_DP_READ_SEQUENTIAL             4'b0011
`define LM32_DP_WRITE_SEQUENTIAL            4'b0100
`define LM32_DP_WRITE_CSR                   4'b0101
`define LM32_DP_BREAK                       4'b0110
`define LM32_DP_RESET                       4'b0111

// States for FSM
`define LM32_JTAG_STATE_RNG                 3:0
`define LM32_JTAG_STATE_READ_COMMAND        4'h0
`define LM32_JTAG_STATE_READ_BYTE_0         4'h1
`define LM32_JTAG_STATE_READ_BYTE_1         4'h2
`define LM32_JTAG_STATE_READ_BYTE_2         4'h3
`define LM32_JTAG_STATE_READ_BYTE_3         4'h4
`define LM32_JTAG_STATE_READ_BYTE_4         4'h5
`define LM32_JTAG_STATE_PROCESS_COMMAND     4'h6
`define LM32_JTAG_STATE_WAIT_FOR_MEMORY     4'h7
`define LM32_JTAG_STATE_WAIT_FOR_CSR        4'h8

/////////////////////////////////////////////////////
// Module interface
/////////////////////////////////////////////////////

module lm32_jtag (
    // ----- Inputs -------
    clk_i,
    rst_i,
    jtag_clk, 
    jtag_update,
    jtag_reg_q,
    jtag_reg_addr_q,
`ifdef CFG_JTAG_UART_ENABLED
    csr,
    csr_write_enable,
    csr_write_data,
    stall_x,
`endif
`ifdef CFG_HW_DEBUG_ENABLED
    jtag_read_data,
    jtag_access_complete,
`endif
`ifdef CFG_DEBUG_ENABLED
    exception_q_w,
`endif
    // ----- Outputs -------
`ifdef CFG_JTAG_UART_ENABLED
    jtx_csr_read_data,
    jrx_csr_read_data,
`endif
`ifdef CFG_HW_DEBUG_ENABLED
    jtag_csr_write_enable,
    jtag_csr_write_data,
    jtag_csr,
    jtag_read_enable,
    jtag_write_enable,
    jtag_write_data,
    jtag_address,
`endif
`ifdef CFG_DEBUG_ENABLED
    jtag_break,
    jtag_reset,
`endif
    jtag_reg_d,
    jtag_reg_addr_d
    );

   parameter lat_family = `LATTICE_FAMILY;
   
/////////////////////////////////////////////////////
// Inputs
/////////////////////////////////////////////////////

input clk_i;                                            // Clock
input rst_i;                                            // Reset

input jtag_clk;                                         // JTAG clock
input jtag_update;                                      // JTAG data register has been updated
input [`LM32_BYTE_RNG] jtag_reg_q;                      // JTAG data register
input [2:0] jtag_reg_addr_q;                            // JTAG data register

`ifdef CFG_JTAG_UART_ENABLED
input [`LM32_CSR_RNG] csr;                              // CSR to write
input csr_write_enable;                                 // CSR write enable
input [`LM32_WORD_RNG] csr_write_data;                  // Data to write to specified CSR
input stall_x;                                          // Stall instruction in X stage
`endif
`ifdef CFG_HW_DEBUG_ENABLED
input [`LM32_BYTE_RNG] jtag_read_data;                  // Data read from requested address
input jtag_access_complete;                             // Memory access if complete
`endif
`ifdef CFG_DEBUG_ENABLED
input exception_q_w;                                    // Indicates an exception has occured in W stage
`endif

/////////////////////////////////////////////////////
// Outputs
/////////////////////////////////////////////////////
       
`ifdef CFG_JTAG_UART_ENABLED
output [`LM32_WORD_RNG] jtx_csr_read_data;              // Value of JTX CSR for rcsr instructions
wire   [`LM32_WORD_RNG] jtx_csr_read_data;
output [`LM32_WORD_RNG] jrx_csr_read_data;              // Value of JRX CSR for rcsr instructions
wire   [`LM32_WORD_RNG] jrx_csr_read_data;
`endif
`ifdef CFG_HW_DEBUG_ENABLED
output jtag_csr_write_enable;                           // CSR write enable
reg    jtag_csr_write_enable;
output [`LM32_WORD_RNG] jtag_csr_write_data;            // Data to write to specified CSR
wire   [`LM32_WORD_RNG] jtag_csr_write_data;
output [`LM32_CSR_RNG] jtag_csr;                        // CSR to write
wire   [`LM32_CSR_RNG] jtag_csr;
output jtag_read_enable;                                // Memory read enable
reg    jtag_read_enable;
output jtag_write_enable;                               // Memory write enable
reg    jtag_write_enable;
output [`LM32_BYTE_RNG] jtag_write_data;                // Data to write to specified address
wire   [`LM32_BYTE_RNG] jtag_write_data;        
output [`LM32_WORD_RNG] jtag_address;                   // Memory read/write address
wire   [`LM32_WORD_RNG] jtag_address;
`endif
`ifdef CFG_DEBUG_ENABLED
output jtag_break;                                      // Request to raise a breakpoint exception
reg    jtag_break;
output jtag_reset;                                      // Request to raise a reset exception
reg    jtag_reset;
`endif
output [`LM32_BYTE_RNG] jtag_reg_d;
reg    [`LM32_BYTE_RNG] jtag_reg_d;
output [2:0] jtag_reg_addr_d;
wire   [2:0] jtag_reg_addr_d;
             
/////////////////////////////////////////////////////
// Internal nets and registers 
/////////////////////////////////////////////////////

reg rx_toggle;                          // Clock-domain crossing registers
reg rx_toggle_r;                        // Registered version of rx_toggle
reg rx_toggle_r_r;                      // Registered version of rx_toggle_r
reg rx_toggle_r_r_r;                    // Registered version of rx_toggle_r_r

reg [`LM32_BYTE_RNG] rx_byte;   
reg [2:0] rx_addr;

`ifdef CFG_JTAG_UART_ENABLED                 
reg [`LM32_BYTE_RNG] uart_tx_byte;      // UART TX data
reg uart_tx_valid;                      // TX data is valid
reg [`LM32_BYTE_RNG] uart_rx_byte;      // UART RX data
reg uart_rx_valid;                      // RX data is valid
`endif

reg [`LM32_DP_RNG] command;             // The last received command
`ifdef CFG_HW_DEBUG_ENABLED
reg [`LM32_BYTE_RNG] jtag_byte_0;       // Registers to hold command paramaters
reg [`LM32_BYTE_RNG] jtag_byte_1;
reg [`LM32_BYTE_RNG] jtag_byte_2;
reg [`LM32_BYTE_RNG] jtag_byte_3;
reg [`LM32_BYTE_RNG] jtag_byte_4;
reg processing;                         // Indicates if we're still processing a memory read/write
`endif

reg [`LM32_JTAG_STATE_RNG] state;       // Current state of FSM

/////////////////////////////////////////////////////
// Combinational Logic
/////////////////////////////////////////////////////

`ifdef CFG_HW_DEBUG_ENABLED
assign jtag_csr_write_data = {jtag_byte_0, jtag_byte_1, jtag_byte_2, jtag_byte_3};
assign jtag_csr = jtag_byte_4[`LM32_CSR_RNG];
assign jtag_address = {jtag_byte_0, jtag_byte_1, jtag_byte_2, jtag_byte_3};
assign jtag_write_data = jtag_byte_4;
`endif
                 
// Generate status flags for reading via the JTAG interface                 
`ifdef CFG_JTAG_UART_ENABLED                 
assign jtag_reg_addr_d[1:0] = {uart_rx_valid, uart_tx_valid};         
`else
assign jtag_reg_addr_d[1:0] = 2'b00;
`endif
`ifdef CFG_HW_DEBUG_ENABLED
assign jtag_reg_addr_d[2] = processing;
`else
assign jtag_reg_addr_d[2] = 1'b0;
`endif

`ifdef CFG_JTAG_UART_ENABLED                 
assign jtx_csr_read_data = {{`LM32_WORD_WIDTH-9{1'b0}}, uart_tx_valid, 8'h00};
assign jrx_csr_read_data = {{`LM32_WORD_WIDTH-9{1'b0}}, uart_rx_valid, uart_rx_byte};
`endif         
                 
/////////////////////////////////////////////////////
// Sequential Logic
/////////////////////////////////////////////////////

// Toggle a flag when a JTAG write occurs
generate
      if (lat_family == "EC" || lat_family == "ECP"  || 
	  lat_family == "XP" || lat_family == "ECP2" || lat_family == "ECP2M") begin

	 always @(posedge jtag_clk `CFG_RESET_SENSITIVITY)
	   begin
	      if (rst_i == `TRUE)
		rx_toggle <= 1'b0;
	      else 
		if (jtag_update == `TRUE)
		  rx_toggle <= ~rx_toggle;
	   end
   
      end else begin // for SC & SCM

	 always @(negedge jtag_update `CFG_RESET_SENSITIVITY)
	   begin
	      if (rst_i == `TRUE)
		rx_toggle <= 1'b0;
	      else 
		rx_toggle <= ~rx_toggle;
	   end
	 
      end
endgenerate   

always @(*)
begin
    rx_byte = jtag_reg_q;
    rx_addr = jtag_reg_addr_q;
end

// Clock domain crossing from JTAG clock domain to CPU clock domain
always @(posedge clk_i `CFG_RESET_SENSITIVITY)
begin
    if (rst_i == `TRUE)
    begin
        rx_toggle_r <= 1'b0;
        rx_toggle_r_r <= 1'b0;
        rx_toggle_r_r_r <= 1'b0;
    end
    else
    begin
        rx_toggle_r <= rx_toggle;
        rx_toggle_r_r <= rx_toggle_r;
        rx_toggle_r_r_r <= rx_toggle_r_r;
    end
end

// LM32 debug protocol state machine
always @(posedge clk_i `CFG_RESET_SENSITIVITY)
begin
    if (rst_i == `TRUE)
    begin
        state <= `LM32_JTAG_STATE_READ_COMMAND;
        command <= 4'b0000;
        jtag_reg_d <= 8'h00;
`ifdef CFG_HW_DEBUG_ENABLED
        processing <= `FALSE;
        jtag_csr_write_enable <= `FALSE;
        jtag_read_enable <= `FALSE;
        jtag_write_enable <= `FALSE;
`endif
`ifdef CFG_DEBUG_ENABLED
        jtag_break <= `FALSE;
        jtag_reset <= `FALSE;
`endif
`ifdef CFG_JTAG_UART_ENABLED                 
        uart_tx_byte <= 8'h00;
        uart_tx_valid <= `FALSE;
        uart_rx_byte <= 8'h00;
        uart_rx_valid <= `FALSE;
`endif
    end
    else
    begin
`ifdef CFG_JTAG_UART_ENABLED                 
        if ((csr_write_enable == `TRUE) && (stall_x == `FALSE))
        begin
            case (csr)
            `LM32_CSR_JTX:
            begin
                // Set flag indicating data is available
                uart_tx_byte <= csr_write_data[`LM32_BYTE_0_RNG];
                uart_tx_valid <= `TRUE;
            end
            `LM32_CSR_JRX:
            begin
                // Clear flag indidicating data has been received
                uart_rx_valid <= `FALSE;
            end
            endcase
        end
`endif
`ifdef CFG_DEBUG_ENABLED
        // When an exception has occured, clear the requests
        if (exception_q_w == `TRUE)
        begin
            jtag_break <= `FALSE;
            jtag_reset <= `FALSE;
        end
`endif
        case (state)
        `LM32_JTAG_STATE_READ_COMMAND:
        begin
            // Wait for rx register to toggle which indicates new data is available
            if (rx_toggle_r_r != rx_toggle_r_r_r)
            begin
                command <= rx_byte[7:4];                
                case (rx_addr)
`ifdef CFG_DEBUG_ENABLED
                `LM32_DP:
                begin
                    case (rx_byte[7:4])
`ifdef CFG_HW_DEBUG_ENABLED
                    `LM32_DP_READ_MEMORY:
                        state <= `LM32_JTAG_STATE_READ_BYTE_0;
                    `LM32_DP_READ_SEQUENTIAL:
                    begin
                        {jtag_byte_2, jtag_byte_3} <= {jtag_byte_2, jtag_byte_3} + 1'b1;
                        state <= `LM32_JTAG_STATE_PROCESS_COMMAND;
                    end
                    `LM32_DP_WRITE_MEMORY:
                        state <= `LM32_JTAG_STATE_READ_BYTE_0;
                    `LM32_DP_WRITE_SEQUENTIAL:
                    begin
                        {jtag_byte_2, jtag_byte_3} <= {jtag_byte_2, jtag_byte_3} + 1'b1;
                        state <= 5;
                    end
                    `LM32_DP_WRITE_CSR:
                        state <= `LM32_JTAG_STATE_READ_BYTE_0;
`endif                    
                    `LM32_DP_BREAK:
                    begin
`ifdef CFG_JTAG_UART_ENABLED     
                        uart_rx_valid <= `FALSE;    
                        uart_tx_valid <= `FALSE;         
`endif
                        jtag_break <= `TRUE;
                    end
                    `LM32_DP_RESET:
                    begin
`ifdef CFG_JTAG_UART_ENABLED     
                        uart_rx_valid <= `FALSE;    
                        uart_tx_valid <= `FALSE;         
`endif
                        jtag_reset <= `TRUE;
                    end
                    endcase                               
                end
`endif
`ifdef CFG_JTAG_UART_ENABLED                 
                `LM32_TX:
                begin
                    uart_rx_byte <= rx_byte;
                    uart_rx_valid <= `TRUE;
                end                    
                `LM32_RX:
                begin
                    jtag_reg_d <= uart_tx_byte;
                    uart_tx_valid <= `FALSE;
                end
`endif
                default:
                    ;
                endcase                
            end
        end
`ifdef CFG_HW_DEBUG_ENABLED
        `LM32_JTAG_STATE_READ_BYTE_0:
        begin
            if (rx_toggle_r_r != rx_toggle_r_r_r)
            begin
                jtag_byte_0 <= rx_byte;
                state <= `LM32_JTAG_STATE_READ_BYTE_1;
            end
        end
        `LM32_JTAG_STATE_READ_BYTE_1:
        begin
            if (rx_toggle_r_r != rx_toggle_r_r_r)
            begin
                jtag_byte_1 <= rx_byte;
                state <= `LM32_JTAG_STATE_READ_BYTE_2;
            end
        end
        `LM32_JTAG_STATE_READ_BYTE_2:
        begin
            if (rx_toggle_r_r != rx_toggle_r_r_r)
            begin
                jtag_byte_2 <= rx_byte;
                state <= `LM32_JTAG_STATE_READ_BYTE_3;
            end
        end
        `LM32_JTAG_STATE_READ_BYTE_3:
        begin
            if (rx_toggle_r_r != rx_toggle_r_r_r)
            begin
                jtag_byte_3 <= rx_byte;
                if (command == `LM32_DP_READ_MEMORY)
                    state <= `LM32_JTAG_STATE_PROCESS_COMMAND;
                else 
                    state <= `LM32_JTAG_STATE_READ_BYTE_4;
            end
        end
        `LM32_JTAG_STATE_READ_BYTE_4:
        begin
            if (rx_toggle_r_r != rx_toggle_r_r_r)
            begin
                jtag_byte_4 <= rx_byte;
                state <= `LM32_JTAG_STATE_PROCESS_COMMAND;
            end
        end
        `LM32_JTAG_STATE_PROCESS_COMMAND:
        begin
            case (command)
            `LM32_DP_READ_MEMORY,
            `LM32_DP_READ_SEQUENTIAL:
            begin
                jtag_read_enable <= `TRUE;
                processing <= `TRUE;
                state <= `LM32_JTAG_STATE_WAIT_FOR_MEMORY;
            end
            `LM32_DP_WRITE_MEMORY,
            `LM32_DP_WRITE_SEQUENTIAL:
            begin
                jtag_write_enable <= `TRUE;
                processing <= `TRUE;
                state <= `LM32_JTAG_STATE_WAIT_FOR_MEMORY;
            end
            `LM32_DP_WRITE_CSR:
            begin
                jtag_csr_write_enable <= `TRUE;
                processing <= `TRUE;
                state <= `LM32_JTAG_STATE_WAIT_FOR_CSR;
            end
            endcase
        end
        `LM32_JTAG_STATE_WAIT_FOR_MEMORY:
        begin
            if (jtag_access_complete == `TRUE)
            begin          
                jtag_read_enable <= `FALSE;
                jtag_reg_d <= jtag_read_data;
                jtag_write_enable <= `FALSE;  
                processing <= `FALSE;
                state <= `LM32_JTAG_STATE_READ_COMMAND;
            end
        end    
        `LM32_JTAG_STATE_WAIT_FOR_CSR:
        begin
            jtag_csr_write_enable <= `FALSE;
            processing <= `FALSE;
            state <= `LM32_JTAG_STATE_READ_COMMAND;
        end    
`endif
        endcase
    end
end
  
endmodule

`endif
