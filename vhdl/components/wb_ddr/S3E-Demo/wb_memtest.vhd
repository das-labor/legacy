library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity wb_memtest is
	port (
		wb_clk_i   : in  std_logic;
		wb_rst_i   : in  std_logic;
		wb_adr_o   : out std_logic_vector(31 downto 0);
		wb_dat_o   : out std_logic_vector(63 downto 0);
		wb_dat_i   : in  std_logic_vector(63 downto 0);
		wb_sel_o   : out std_logic_vector( 7 downto 0);
		wb_we_o    : out std_logic;
		wb_stb_o   : out std_logic;
		wb_cyc_o   : out std_logic;
		wb_ack_i   : in  std_logic;
		-- Status output
		led        : out std_logic_vector(7 downto 0);
      btn0       : in  std_logic;
      btn1       : in  std_logic ); 
end wb_memtest;

architecture rtl of wb_memtest is

signal data : std_logic_vector(63 downto 0);
begin

wb_sel_o <= "11010111";

sim: process(wb_clk_i) is
type TState is (read, write, nop);
variable state : TState := read;

begin
	if wb_rst_i='1' then
		led <= (others => '0');	
		data <= (others => '0');
		data( 1 downto  0) <= (others => '1');
		data(18 downto 16) <= (others => '1');		
		data(35 downto 32) <= (others => '1');				
		data(52 downto 48) <= (others => '1');						
		state := nop;
	elsif wb_clk_i'event and wb_clk_i='1' then
		case state is
		when nop =>
			led(1 downto 0) <= "00";		
			wb_cyc_o <= '0';
			wb_stb_o <= '0';
			wb_dat_o <= (others => '0');
         if btn0='1' then 
            led <= (others => '0');
            data <= (others => '0');
            data( 1 downto  0) <= (others => '1');
            data(18 downto 16) <= (others => '1');		
            data(35 downto 32) <= (others => '1');				
            data(52 downto 48) <= (others => '1');						
         end if;
         if btn1='1' then 
            data(62 downto 0) <= data(63 downto 1);
            data(63) <= data(11) xor data(13) xor data(14) xor data(16);
         end if;
			state := write;
		when write =>
			led(1 downto 0) <= "01";
			wb_adr_o <= "00000010100000000000010000000000";
			wb_dat_o <= data;
			wb_cyc_o <= '1';
			wb_we_o  <= '1';	
			wb_stb_o <= '1';
			if wb_ack_i='1' then
				wb_cyc_o <= '0';
				wb_stb_o <= '0';
				state := read;
			end if;
		when read =>
			led(1 downto 0) <= "10";		
			wb_adr_o <= "00000010100000000000010000000000";						
			wb_cyc_o <= '1';
			wb_we_o  <= '0';	
			wb_stb_o <= '1';
			wb_dat_o <= (others => '0');			
			if wb_ack_i='1' then
				if wb_dat_i /= data then
               led(4) <= '0'; 
					led(2) <= '1';  -- DATA ERROR
					led(3) <= '1';  -- DATA ERROR
				else
               led(2) <= '0';  -- 
					led(4) <= '1';  -- DATA OK								
					led(5) <= '1';  -- DATA OK
				end if;
				wb_cyc_o <= '0';
				wb_stb_o <= '0';
				state := nop;			
			end if;			
		end case;
	end if;
end process;

end rtl;

