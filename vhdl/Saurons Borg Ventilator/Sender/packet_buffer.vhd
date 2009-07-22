library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity packet_buffer is
    Port ( 
	 clk20     :  in  STD_LOGIC;
	 clk50     :  in  STD_LOGIC;
	 sram_read :  in  STD_LOGIC_VECTOR (15 downto 0);
    sram_pos  :  in  STD_LOGIC_VECTOR ( 7 downto 0);
    winkel    :  in  STD_LOGIC_VECTOR ( 9 downto 0);
    b10code   : out  STD_LOGIC;			  
    freeze_diag  : out STD_LOGIC_VECTOR ( 2 downto 0);
    winkel_diag  : out STD_LOGIC_VECTOR ( 9 downto 0);
    addra_diag   : out STD_LOGIC_VECTOR (11 downto 0);
    b8_code_diag : out STD_LOGIC_VECTOR ( 7 downto 0);
    rdy_diag     : out STD_LOGIC

																						);
end packet_buffer;

architecture Behavioral of packet_buffer is

component frame_full
	port (
	addra: IN  std_logic_VECTOR(11 downto 0);
	addrb: IN  std_logic_VECTOR(10 downto 0);
	clka : IN  std_logic;
	clkb : IN  std_logic;
	dinb : IN  std_logic_VECTOR(15 downto 0);
	douta: OUT std_logic_VECTOR( 7 downto 0);
	ena  : IN  std_logic;
	web  : IN  std_logic);
end component;


component packet_write
	PORT(
	b8_code : IN  std_logic_vector(7 downto 0);
	clk     : IN  std_logic;          
	q1      : OUT std_logic;
	rdy     : OUT std_logic;
	counter : OUT std_logic_vector(9 downto 0));
end component;

signal addra    : std_logic_VECTOR(11 downto 0);
signal addrb    : std_logic_VECTOR(10 downto 0);
signal dinb     : std_logic_VECTOR(15 downto 0);
signal b8_code  : std_logic_VECTOR( 7 downto 0);
signal counter  : std_logic_VECTOR( 9 downto 0);
signal rdy      : std_logic;
signal winkel_1 : std_logic_VECTOR( 9 downto 0);
signal winkel_2 : std_logic_VECTOR( 9 downto 0);
signal counter_1: std_logic_VECTOR( 9 downto 0);
signal freeze   : std_logic_VECTOR( 2 downto 0);


begin

-- Port A Lesen  / Port B Schreiben

bram : frame_full
		port map (
			addra => addra,
			addrb => addrb,
			clka  => clk20,
			clkb  => clk50,
			dinb  => dinb,
			douta => b8_code,
			ena   => rdy,
			web   => '1');


pwrite: packet_write
		port map(
			b8_code => b8_code,
			clk     => clk20,
			q1      => b10code,
			rdy     => rdy,
			counter => counter );


-- Daten in Blockram schreiben (Kanal B)
process (clk50) begin
	if rising_edge (clk50) then
		addrb <= winkel(2 downto 0) & sram_pos;
		 dinb <= sram_read;
	end if;
end process;


--============ Taktübergang 50 - 20 Mhz ===================
process (clk20) begin
	if rising_edge (clk20) then
		winkel_1  <= winkel;
		winkel_2  <= winkel_1;
		counter_1 <= counter;
	end if;
end process;


-- Winkel am Paketanfang Sichern
process (clk20) begin
	if rising_edge (clk20) then
		if counter = 0 and counter_1 /= 0 then
			freeze <= winkel_2(2 downto 0) - 1;
		end if;
	end if;
end process;


-- Addresse zum Lesen

addra <= freeze & counter (8 downto 0);

freeze_diag  <= freeze;
winkel_diag  <= winkel;
addra_diag   <= addra;
b8_code_diag <= b8_code;
rdy_diag     <= rdy;

end Behavioral;

