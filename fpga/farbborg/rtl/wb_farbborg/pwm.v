// Farborg PWM Anteuerung
module pwm (
    reset,	  
    
    cpu_clk, 		  
    cpu_addr,
    din,
    we,
    pwm_clk,		  
    lsr_clr,	  
    lsr_d,	  
    lsr_c,	  
    latch_data, 
    psr_c, 	  
    psr_d,	  
    col_enable 
);   

input             reset;
input             cpu_clk;
input  [10:0]     cpu_addr;
input  [15:0]     din;
input             we;
input             pwm_clk;
output            lsr_clr;
output reg        lsr_d;
output            lsr_c;
output reg [ 7:0] latch_data;
output reg        psr_c;
output reg        psr_d;
output reg        col_enable;


reg    [7:0] pwmcount;
reg    [7:0] pwmtable_idx;

wire  [15:0] pwmtable_out;

reg          trigger;
reg   [15:0] delaycnt;

//readline from the pixmap. represents 8 values, that we compare with simultaneously.
wire  [63:0] line;

//selects if the enable line for the matrix controller is asserted.
//is used to protect the leds as long as the configuration is not yet complete.
reg   pwm_enable;

//address for the 64bit line we read.
wire   [6:0] pixmap_addr;
reg    [2:0] planecount;
reg    [3:0] latchcount;
assign pixmap_addr = {planecount, latchcount};

//clock for the latch shift register.
reg lsr_clk_en;
assign lsr_c   = ~pwm_clk & lsr_clk_en;
assign lsr_clr = ~reset;

//writes from BASE to BASE+0x3ff go to pixmap
wire   we_pixmap;
assign we_pixmap   = we & ~cpu_addr[10];

//writes from BASE+0x400 to BASE+0x800 go to pwmtable
wire   we_pwmtable;
assign we_pwmtable = we & cpu_addr[10] & ~cpu_addr[9];

always @(posedge pwm_clk or posedge reset) begin
	if (reset == 1'b 1) begin
		delaycnt    <= 16'h0000;
		pwmcount    <= 8'b00000000;
		pwmtable_idx<= 8'b00000000;
		planecount  <= 3'b000;
		trigger     <= 1'b0;
		psr_d       <= 1'b0;
		psr_c       <= 1'b0;
		col_enable  <= 1'b1;
	end else begin
		trigger     <= 1'b0;
		delaycnt    <= delaycnt - 1;

		//turn col_enable off immediately, if pwm_enable is reset.
		if (pwm_enable == 1'b0) begin
			col_enable <= 1'b1;
		end

		//when delaycount reaches 0, the timeslot is over
		if (delaycnt == 16'h0000) begin
			
			//load new delay value from pwmtable
			delaycnt    <= pwmtable_out;
			
			//store the index, whos delay value we are loading, to pwmcount
			pwmcount     <= pwmtable_idx;
			//increment pwmtable_idx for next timeslot
			pwmtable_idx <= pwmtable_idx + 1;
			
			//trigger update of the latches
			trigger     <= 1'b1;
			
			psr_c <= 1'b0;
			
			if (pwmtable_idx == 8'b00000000) begin
				//if the new slot is 0, switch planes
				psr_c <= 1'b1;
				
				//enable leds if we are enabled
				if (pwm_enable == 1'b1) begin
					col_enable <= 1'b0;
				end
				
				//increment plane
				planecount <= planecount + 1;			
				if (planecount == 3'b100) begin
					planecount <= 3'b000;
			    end
			end

			if (pwmtable_idx == 8'b11111111) begin
				$display("newpwmcount = 11111111");
			    if (planecount == 3'b100) begin
					psr_d <= 1'b1;
				end else
					psr_d <= 1'b0;
			end
		end
    end
end

always @(posedge pwm_clk or posedge reset) begin
    if (reset == 1'b 1) begin
        latchcount <= 4'b1011;
        lsr_clk_en <= 1'b0;
    end
    else begin
        lsr_d      <= 1'b0;
        latchcount <= latchcount + 1;
        
        if (latchcount == 4'b0001) begin
            lsr_d      <= 1'b1;
            lsr_clk_en <= 1'b1;
        end
        
        if (latchcount == 4'b1011) begin
          lsr_clk_en <= 1'b0;
          latchcount <= 4'b1011;
          if (trigger == 1'b1) begin
              latchcount <= 4'b0000;
          end
        end
        
         
        if (pwmcount < line[(8*8)-1:(7*8)]) begin
            latch_data[7] <= 1'b1;
        end else begin
            latch_data[7] <= 1'b0;
        end
        if (pwmcount < line[(7*8)-1:(6*8)]) begin
            latch_data[6] <= 1'b1;
        end else begin
            latch_data[6] <= 1'b0;
        end       
        if (pwmcount < line[(6*8)-1:(5*8)]) begin
            latch_data[5] <= 1'b1;
        end else begin
            latch_data[5] <= 1'b0;
        end
        if (pwmcount < line[(5*8)-1:(4*8)]) begin
            latch_data[4] <= 1'b1;
        end else begin
            latch_data[4] <= 1'b0;
        end
        if (pwmcount < line[(4*8)-1:(3*8)]) begin
            latch_data[3] <= 1'b1;
        end else begin
            latch_data[3] <= 1'b0;
        end
        if (pwmcount < line[(3*8)-1:(2*8)]) begin
            latch_data[2] <= 1'b1;
        end else begin
            latch_data[2] <= 1'b0;
        end
        if (pwmcount < line[(2*8)-1:(1*8)]) begin
            latch_data[1] <= 1'b1;
        end else begin
            latch_data[1] <= 1'b0;
        end
        if (pwmcount < line[(1*8)-1:0]) begin
            latch_data[0] <= 1'b1;
        end else begin
            latch_data[0] <= 1'b0;
        end
    end
end

always @(posedge cpu_clk or posedge reset) begin
    if (reset == 1'b1) begin
        pwm_enable <= 1'b0;
    end else begin
        if (we == 1'b 1) begin
            if (cpu_addr[10]  == 1'b1 )
                pwm_enable <= 1'b0; //disable pwm when user accesses pwm table
            if (cpu_addr == {3'b111, 8'h55})  //0x755
                if (din == 8'h23)
                    pwm_enable <= 1'b1;
        end
    end
end

dualportram_8x16_8x16 pwmtable (
        .clka(cpu_clk),
        .clkb(pwm_clk), 
        .wea(we_pwmtable),
        .addra(cpu_addr[7:0]),
        .addrb(pwmtable_idx),
        .dia(din),
        .dob(pwmtable_out)
);

dualportram_10x8_7x64 pixmap(
        .clka(cpu_clk),
        .clkb(pwm_clk),
        .wea(we_pixmap),
        .addra(cpu_addr[9:0]),
        .addrb(pixmap_addr),
        .dia(din[7:0]),
        .dob(line[63:0])
);

endmodule
