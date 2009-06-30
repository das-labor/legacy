
--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   16:49:36 06/30/2009
-- Design Name:   ram_control
-- Module Name:   D:/Saurons Borg Ventilator/Sender/vtb_ram_control.vhd
-- Project Name:  borg_sender
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: ram_control
--
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
-- This testbench has been automatically generated using types std_logic and
-- std_logic_vector for the ports of the unit under test.  Xilinx recommends 
-- that these types always be used for the top-level I/O of a design in order 
-- to guarantee that the testbench will bind correctly to the post-implementation 
-- simulation model.
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.numeric_std.ALL;

ENTITY vtb_ram_control_vhd IS
END vtb_ram_control_vhd;

ARCHITECTURE behavior OF vtb_ram_control_vhd IS 

	-- Component Declaration for the Unit Under Test (UUT)
	COMPONENT ram_control
	PORT(
		clk50 : IN std_logic;
		clk100 : IN std_logic;
		winkel : IN std_logic_vector(9 downto 0);
		ad_adr : IN std_logic_vector(17 downto 0);
		ad_dat : IN std_logic_vector(15 downto 0);
		ad_wr : IN std_logic;
		cpu_dat_x : IN std_logic_vector(7 downto 0);
		cpu_dat_y : IN std_logic_vector(7 downto 0);
		cpu_adr_hi_lo : IN std_logic_vector(7 downto 0);
		cpu_adr_hi_hi : IN std_logic;
		cpu_adr_lo_lo : IN std_logic_vector(7 downto 0);
		cpu_adr_lo_hi : IN std_logic;
		cpu_write : IN std_logic;
		cpu_enable : IN std_logic;          
		write_adr_d : OUT std_logic_vector(17 downto 0);
		write_dat_d : OUT std_logic_vector(15 downto 0);
		write_wrt_d : OUT std_logic;
		write_sel_d : OUT std_logic;
		read_adr_d : OUT std_logic_vector(17 downto 0);
		led_nr_d : OUT std_logic_vector(7 downto 0);
		winkel_diag_d : OUT std_logic_vector(9 downto 0);
		sram_pos  : out std_logic_vector ( 7 downto 0)

		);
	END COMPONENT;

	--Inputs
	SIGNAL clk50 :  std_logic := '0';
	SIGNAL clk100 :  std_logic := '0';
	SIGNAL ad_wr :  std_logic := '0';
	SIGNAL cpu_adr_hi_hi :  std_logic := '0';
	SIGNAL cpu_adr_lo_hi :  std_logic := '0';
	SIGNAL cpu_write :  std_logic := '0';
	SIGNAL cpu_enable :  std_logic := '0';
	SIGNAL winkel :  std_logic_vector(9 downto 0) := (others=>'0');
	SIGNAL ad_adr :  std_logic_vector(17 downto 0) := (others=>'0');
	SIGNAL ad_dat :  std_logic_vector(15 downto 0) := (others=>'0');
	SIGNAL cpu_dat_x :  std_logic_vector(7 downto 0) := (others=>'0');
	SIGNAL cpu_dat_y :  std_logic_vector(7 downto 0) := (others=>'0');
	SIGNAL cpu_adr_hi_lo :  std_logic_vector(7 downto 0) := (others=>'0');
	SIGNAL cpu_adr_lo_lo :  std_logic_vector(7 downto 0) := (others=>'0');

	--Outputs
	SIGNAL write_adr_d :  std_logic_vector(17 downto 0);
	SIGNAL write_dat_d :  std_logic_vector(15 downto 0);
	SIGNAL write_wrt_d :  std_logic;
	SIGNAL write_sel_d :  std_logic;
	SIGNAL read_adr_d :  std_logic_vector(17 downto 0);
	SIGNAL led_nr_d :  std_logic_vector(7 downto 0);
	SIGNAL winkel_diag_d :  std_logic_vector(9 downto 0);
	SIGNAL sram_pos  : std_logic_vector ( 7 downto 0);

	
	signal count_i: std_logic_vector(17 downto 0):= (others =>'0');

BEGIN

	-- Instantiate the Unit Under Test (UUT)
	uut: ram_control PORT MAP(
		clk50 => clk50,
		clk100 => clk100,
		winkel => winkel,
		ad_adr => ad_adr,
		ad_dat => ad_dat,
		ad_wr => ad_wr,
		cpu_dat_x => cpu_dat_x,
		cpu_dat_y => cpu_dat_y,
		cpu_adr_hi_lo => cpu_adr_hi_lo,
		cpu_adr_hi_hi => cpu_adr_hi_hi,
		cpu_adr_lo_lo => cpu_adr_lo_lo,
		cpu_adr_lo_hi => cpu_adr_lo_hi,
		cpu_write => cpu_write,
		cpu_enable => cpu_enable,
		write_adr_d => write_adr_d,
		write_dat_d => write_dat_d,
		write_wrt_d => write_wrt_d,
		write_sel_d => write_sel_d,
		read_adr_d => read_adr_d,
		led_nr_d => led_nr_d,
		winkel_diag_d => winkel_diag_d
	);

process begin
		clk50 <= '1';
		wait for 10 ns;
		clk50 <= '0';
		wait for 10 ns; 
end process;

process begin
		clk100 <= '1';
		wait for 5 ns;
		clk100 <= '0';
		wait for 5 ns; 
end process;

process (clk50) begin
	if rising_edge (clk50) then
		if ad_wr = '0' then ad_wr <= '1';
		else ad_wr <= '0';
		end if;
		
		if ad_wr = '0' then
		count_i <= count_i + 1;
		end if;
		
		ad_dat <= count_i (17 downto 10) & count_i (8 downto 1);
		ad_adr <= count_i;
	end if;
end process;

END;
