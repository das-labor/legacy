----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    14:14:58 12/19/2006 
-- Design Name: 
-- Module Name:    pwm - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-- Xilinx includes
--Library UNISIM;
--use UNISIM.vcomponents.all;

-- Altera inlcudes
library altera_mf;
use altera_mf.all;

---- Uncomment the following library declaration if instantiating
---- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity pwm is
    Port ( reset	  : in  std_logic;
           cp2 		  : in  std_logic;
		   addr		  : in  std_logic_vector(10 downto 0);
		   din		  : in  std_logic_vector(7 downto 0);
		   we		  : in  std_logic;
           clk		  : in  std_logic;
           lsr_clr	  : out std_logic;
           lsr_d	  : out std_logic;
           lsr_c	  : out std_logic;
           latch_data : out std_logic_vector(7 downto 0);
		   psr_c 	  : out std_logic;
		   psr_d	  : out std_logic;
		   col_enable : out std_logic		  
			  );
end pwm;

architecture Behavioral of pwm is

--constant USE_XILINX : boolean := false;
--constant USE_ALTERA : boolean := true;

--this holds the brightnesses for the LEDs
--type RAM128Type is array(127 downto 0) of std_logic_vector(63 downto 0);
--signal RAMFile : RAM128Type := (others => x"0102030405060708"); 

--this holds the states for the leds
type ledstatusType is array(15 downto 0) of std_logic_vector(7 downto 0);
signal ledstatus : ledstatusType := (others => x"00"); 

--delays for each pwm cycle
type pwmtableType is array(255 downto 0) of std_logic_vector(15 downto 0);

signal pwmtable : pwmtableType := (others => x"0030"); 

signal address : std_logic_vector(8 downto 0);

signal planecount : std_logic_vector(2 downto 0);
signal latchcount : std_logic_vector(3 downto 0);

signal pwmcount : std_logic_vector(8 downto 0);
signal newpwmcount : std_logic_vector(8 downto 0);

signal pwmdata : std_logic_vector(15 downto 0);
signal br3_adr : std_logic_vector(10 downto 0);
signal br3_adrb : std_logic_vector(9 downto 0);

signal we_br3 : std_logic;
signal addr_br : std_logic_vector(10 downto 0);

signal we_br1 : std_logic;
signal we_br2 : std_logic;

signal lsr_clk_en : std_logic;

signal trigger : std_logic;

signal delaycnt : std_logic_vector(15 downto 0);

signal line : std_logic_vector(63 downto 0);

signal pwm_enable : std_logic;

COMPONENT altsyncram
	GENERIC (
		address_reg_b				: STRING;
		clock_enable_input_a		: STRING;
		clock_enable_input_b		: STRING;
		clock_enable_output_a		: STRING;
		clock_enable_output_b		: STRING;
		init_file					: STRING;
		init_file_layout			: STRING;
		intended_device_family		: STRING;
		lpm_type					: STRING;
		numwords_a					: NATURAL;
		numwords_b					: NATURAL;
		operation_mode				: STRING;
		outdata_aclr_b				: STRING;
		outdata_reg_b				: STRING;
		power_up_uninitialized		: STRING;
		read_during_write_mode_mixed_ports	: STRING;
		widthad_a					: NATURAL;
		widthad_b					: NATURAL;
		width_a						: NATURAL;
		width_b						: NATURAL;
		width_byteena_a				: NATURAL
	);
	PORT (
			wren_a					: IN  STD_LOGIC ;
			clock0					: IN  STD_LOGIC ;
			clock1					: IN  STD_LOGIC ;
			address_a				: IN  STD_LOGIC_VECTOR ((widthad_a - 1) DOWNTO 0);
			address_b				: IN  STD_LOGIC_VECTOR ((widthad_b - 1) DOWNTO 0);
			q_b						: OUT STD_LOGIC_VECTOR ((width_b - 1) DOWNTO 0);
			data_a					: IN  STD_LOGIC_VECTOR ((width_a - 1)  DOWNTO 0)
	);
	END COMPONENT;

begin
	address <= "00" & planecount & latchcount;

							
	lsr_c <= (not clk) and lsr_clk_en;
	
	
	lsr_clr <= not reset;
	
	process(clk, reset)
	begin
		if reset = '1' then
			delaycnt <= x"0000";
			pwmcount <= "000000000";
			newpwmcount <= "000000000";
			planecount <= "000";
			trigger <= '0';
			psr_d <= '0';
			psr_c <= '0';
			col_enable <= '1';
		elsif clk'event and clk = '1' then
			trigger <= '0';
			delaycnt <= delaycnt - 1;
			if pwm_enable = '0' then
				col_enable <= '1';
			end if;
			if delaycnt = x"0000" then
				delaycnt <= pwmdata;
				newpwmcount <= newpwmcount + 1;
				pwmcount <= newpwmcount;
				trigger <= '1';
				
				if newpwmcount = "100000000" then
					planecount <= planecount + 1;
					psr_d <= '0';
					if planecount = "100" then
						psr_d <= '1';
						planecount <= "000";
					end if;
				end if;
				
				psr_c <= '0';
				if newpwmcount = "100000001" then
					psr_c <= '1';
					newpwmcount <= "000000000";
					if pwm_enable = '1' then
						col_enable <= '0';
					end if;
				end if;
			end if;
		end if;
	end process;

	process(clk, reset)
	--variable line : std_logic_vector(63 downto 0);
	--variable lstat : std_logic_vector(7 downto 0);
	begin
		if reset = '1' then
			latchcount <= "1011";
			lsr_clk_en <= '0';
		elsif clk'event and clk = '1' then
			lsr_d <= '0';
			latchcount <= latchcount + 1;

			if latchcount = "0001" then
				lsr_d <= '1';
				lsr_clk_en <= '1';
			end if;
			
			--if latchcount = "1010" then
				
			--end if;
			
			if latchcount = "1011" then
				lsr_clk_en <= '0';
				latchcount <= "1011";
				if trigger = '1' then
					latchcount <= "0000";
				end if;
			end if;
			
			latch_data <= ledstatus(CONV_INTEGER(latchcount));
			if pwmcount = "00000000" then
				ledstatus(CONV_INTEGER(latchcount)) <= "11111111";
				latch_data <= "11111111";
			end if;
			
			--line <= RAMFile(CONV_INTEGER(address));
			
			for i in 7 downto 0 loop
				if pwmcount >= line( (i+1)*8-1 downto 8*i) then
					ledstatus(CONV_INTEGER(latchcount))(i) <= '0';
					latch_data(i) <= '0';
				end if;
			end loop;
			
		end if;
		
	end process;
	
	process(cp2, reset)
	begin
		if reset = '1' then
			pwm_enable <= '0';
		elsif cp2'event and cp2 = '1' then
			if we = '1' then
				
				if addr(10) = '1' then
					pwm_enable <= '0'; --disable pwm when user accesse pwm table
				end if;
				
				if addr = "111" & x"55" then --0xE755
					if din = x"23" then
						pwm_enable <= '1';
					end if;
				end if;
			end if;
		end if;
	
	end process;
	
	
	addr_br <= "00" & addr(9 downto 3) & addr(1 downto 0);
	we_br1 <= (not addr(10)) and (not addr(2)) and we;
	we_br2 <= (not addr(10)) and addr(2) and we;
	
	br3_adrb <= "0" & newpwmcount;
	br3_adr <= "0" & addr(9 downto 0);
	
	we_br3 <= addr(10) and we;
	
-----------------------------------------------------------------------------
-- Xilinx -------------------------------------------------------------------
--xilinx: if USE_XILINX generate 	

--   RAMB16_S9_S18_inst : RAMB16_S9_S18
--   generic map (
--      INIT_A => X"000", --  Value of output RAM registers on Port A at startup
--      INIT_B => X"00000", --  Value of output RAM registers on Port B at startup
--      SRVAL_A => X"000", --  Port A ouput value upon SSR assertion
--      SRVAL_B => X"00000", --  Port B ouput value upon SSR assertion
--      WRITE_MODE_A => "WRITE_FIRST", --  WRITE_FIRST, READ_FIRST or NO_CHANGE
--      WRITE_MODE_B => "WRITE_FIRST", --  WRITE_FIRST, READ_FIRST or NO_CHANGE
--      SIM_COLLISION_CHECK => "ALL", -- "NONE", "WARNING", "GENERATE_X_ONLY", "ALL
      -- The following INIT_xx declarations specify the initial contents of the RAM
      -- Port A Address 0 to 511, Port B Address 0 to 255
--      INIT_00 => X"3333333333333333333333333333333333333333333333333333333333330033")
--   port map (
      --DOA => DOA,      -- Port A 8-bit Data Output
--      DOB => pwmdata,      -- Port B 16-bit Data Output
      --DOPA => DOPA,    -- Port A 1-bit Parity Output
      --DOPB => DOPB,    -- Port B 2-bit Parity Output
--      ADDRA => br3_adr,  -- Port A 11-bit Address Input
--      ADDRB => br3_adrb,  -- Port B 10-bit Address Input
--      CLKA => cp2,    -- Port A Clock
--      CLKB => clk,    -- Port B Clock
--      DIA => din,      -- Port A 8-bit Data Input
--      DIB => (others => '0'),      -- Port B 16-bit Data Input
--      DIPA => "1",    -- Port A 1-bit parity Input
--      DIPB => "11",    -- Port-B 2-bit parity Input
--      ENA => '1',      -- Port A RAM Enable Input
--      ENB => '1',      -- PortB RAM Enable Input
--      SSRA => '0',    -- Port A Synchronous Set/Reset Input
--      SSRB => '0',    -- Port B Synchronous Set/Reset Input
--      WEA => we_br3,      -- Port A Write Enable Input
--      WEB => '0'       -- Port B Write Enable Input
--   );
	
--   RAMB16_S9_S36_inst : RAMB16_S9_S36
--   generic map (
--      INIT_A => X"000", --  Value of output RAM registers on Port A at startup
--      INIT_B => X"000000000", --  Value of output RAM registers on Port B at startup
--      SRVAL_A => X"000", --  Port A ouput value upon SSR assertion
--      SRVAL_B => X"000000000", --  Port B ouput value upon SSR assertion
--      WRITE_MODE_A => "WRITE_FIRST", --  WRITE_FIRST, READ_FIRST or NO_CHANGE
--      WRITE_MODE_B => "WRITE_FIRST", --  WRITE_FIRST, READ_FIRST or NO_CHANGE
--      SIM_COLLISION_CHECK => "ALL", -- "NONE", "WARNING", "GENERATE_X_ONLY", "ALL
      -- The following INIT_xx declarations specify the initial contents of the RAM
      -- Port A Address 0 to 511, Port B Address 0 to 127
--      INIT_00 => X"55555555555555555555555555555555555555555555555555555555555555")
--   port map (
      --DOA => DOA,      -- Port A 8-bit Data Output
--      DOB => line(31 downto 0),      -- Port B 32-bit Data Output
      --DOPA => DOPA,    -- Port A 1-bit Parity Output
      --DOPB => DOPB,    -- Port B 4-bit Parity Output
--      ADDRA => addr_br,  -- Port A 11-bit Address Input
--      ADDRB => address,  -- Port B 9-bit Address Input
--      CLKA => cp2,    -- Port A Clock
--      CLKB => clk,    -- Port B Clock
--      DIA => din,      -- Port A 8-bit Data Input
--      DIB => (others => '0'),      -- Port B 32-bit Data Input
--      DIPA => "0",    -- Port A 1-bit parity Input
--      DIPB => "1111",    -- Port-B 4-bit parity Input
--      ENA => '1',      -- Port A RAM Enable Input
--      ENB => '1',      -- Port B RAM Enable Input
--      SSRA => '0',    -- Port A Synchronous Set/Reset Input
--      SSRB => '0',    -- Port B Synchronous Set/Reset Input
--      WEA => we_br1,      -- Port A Write Enable Input
--      WEB => '0'       -- Port B Write Enable Input
--   );


--   RAMB16_S9_S36_inst2 : RAMB16_S9_S36
--   generic map (
--      INIT_A => X"000", --  Value of output RAM registers on Port A at startup
--      INIT_B => X"000000000", --  Value of output RAM registers on Port B at startup
--      SRVAL_A => X"000", --  Port A ouput value upon SSR assertion
--      SRVAL_B => X"000000000", --  Port B ouput value upon SSR assertion
--      WRITE_MODE_A => "WRITE_FIRST", --  WRITE_FIRST, READ_FIRST or NO_CHANGE
--      WRITE_MODE_B => "WRITE_FIRST", --  WRITE_FIRST, READ_FIRST or NO_CHANGE
--     SIM_COLLISION_CHECK => "ALL", -- "NONE", "WARNING", "GENERATE_X_ONLY", "ALL
      -- The following INIT_xx declarations specify the initial contents of the RAM
      -- Port A Address 0 to 511, Port B Address 0 to 127
--      INIT_00 => X"0506000000000000000000000000000000000000000000000000000000000000")
--   port map (
      --DOA => DOA,      -- Port A 8-bit Data Output
--      DOB => line(63 downto 32),      -- Port B 32-bit Data Output
      --DOPA => DOPA,    -- Port A 1-bit Parity Output
      --DOPB => DOPB,    -- Port B 4-bit Parity Output
--      ADDRA => addr_br,  -- Port A 11-bit Address Input
--      ADDRB => address,  -- Port B 9-bit Address Input
--      CLKA => cp2,    -- Port A Clock
--      CLKB => clk,    -- Port B Clock
--      DIA => din,      -- Port A 8-bit Data Input
--      DIB => (others => '0'),      -- Port B 32-bit Data Input
--      DIPA => "0",    -- Port A 1-bit parity Input
--      DIPB => "1111",    -- Port-B 4-bit parity Input
--      ENA => '1',      -- Port A RAM Enable Input
--      ENB => '1',      -- Port B RAM Enable Input
--      SSRA => '0',    -- Port A Synchronous Set/Reset Input
--      SSRB => '0',    -- Port B Synchronous Set/Reset Input
--      WEA => we_br2,      -- Port A Write Enable Input
--      WEB => '0'       -- Port B Write Enable Input
--   );
--end generate;

-----------------------------------------------------------------------------
-- Altrera ------------------------------------------------------------------
--altera: if USE_ALTERA generate

	timeTable : altsyncram
	GENERIC MAP (
		address_reg_b => "CLOCK1",
		clock_enable_input_a => "BYPASS",
		clock_enable_input_b => "BYPASS",
		clock_enable_output_a => "BYPASS",
		clock_enable_output_b => "BYPASS",
		init_file => "time_data.hex",
		init_file_layout => "PORT_B",
		intended_device_family => "Cyclone II",
		lpm_type => "altsyncram",
		numwords_a => 2048,
		numwords_b => 1024,
		operation_mode => "DUAL_PORT",
		outdata_aclr_b => "NONE",
		outdata_reg_b => "CLOCK1",
		power_up_uninitialized => "FALSE",
		read_during_write_mode_mixed_ports => "DONT_CARE",
		widthad_a => 11,
		widthad_b => 10,
		width_a => 8,
		width_b => 16,
		width_byteena_a => 1
	)
	PORT MAP (
		wren_a => we_br3,
		clock0 => cp2,
		clock1 => clk,
		address_a => br3_adr,
		address_b => br3_adrb,
		data_a => din,
		q_b => pwmdata
	);

    colorTable1 : altsyncram
	GENERIC MAP (
		address_reg_b => "CLOCK1",
		clock_enable_input_a => "BYPASS",
		clock_enable_input_b => "BYPASS",
		clock_enable_output_a => "BYPASS",
		clock_enable_output_b => "BYPASS",
		init_file => "image_data1.hex",
		init_file_layout => "PORT_B",
		intended_device_family => "Cyclone II",
		lpm_type => "altsyncram",
		numwords_a => 2048,
		numwords_b => 512,
		operation_mode => "DUAL_PORT",
		outdata_aclr_b => "NONE",
		outdata_reg_b => "CLOCK1",
		power_up_uninitialized => "FALSE",
		read_during_write_mode_mixed_ports => "DONT_CARE",
		widthad_a => 11,
		widthad_b => 9,
		width_a => 8,
		width_b => 32,
		width_byteena_a => 1
	)
	PORT MAP (
		wren_a => we_br3,
		clock0 => cp2,
		clock1 => clk,
		address_a => br3_adr,
		address_b => address,
		data_a => din,
		q_b => line(31 downto 0)
	);
	
    colorTable2 : altsyncram
	GENERIC MAP (
		address_reg_b => "CLOCK1",
		clock_enable_input_a => "BYPASS",
		clock_enable_input_b => "BYPASS",
		clock_enable_output_a => "BYPASS",
		clock_enable_output_b => "BYPASS",
		init_file => "image_data2.hex",
		init_file_layout => "PORT_B",
		intended_device_family => "Cyclone II",
		lpm_type => "altsyncram",
		numwords_a => 2048,
		numwords_b => 512,
		operation_mode => "DUAL_PORT",
		outdata_aclr_b => "NONE",
		outdata_reg_b => "CLOCK1",
		power_up_uninitialized => "FALSE",
		read_during_write_mode_mixed_ports => "DONT_CARE",
		widthad_a => 11,
		widthad_b => 9,
		width_a => 8,
		width_b => 32,
		width_byteena_a => 1
	)
	PORT MAP (
		wren_a => we_br1,
		clock0 => cp2,
		clock1 => clk,
		address_a => br3_adr,
		address_b => address,
		data_a => din,
		q_b => line(63 downto 32)
	);

--end generate;	

end Behavioral;

