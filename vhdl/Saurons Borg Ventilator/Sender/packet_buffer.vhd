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
    freeze_diag  : out STD_LOGIC_VECTOR ( 4 downto 0);
    winkel_diag  : out STD_LOGIC_VECTOR ( 9 downto 0);
    addrb_diag   : out STD_LOGIC_VECTOR (12 downto 0);
	 doutb_diag   : out STD_LOGIC_VECTOR (15 downto 0);
    b8_code_diag : out STD_LOGIC_VECTOR ( 7 downto 0);
    rdy_diag     : out STD_LOGIC;
	 counter_diag : out STD_LOGIC_VECTOR ( 9 downto 0)
 
																						);
end packet_buffer;

architecture Behavioral of packet_buffer is

component frame_full
	port (
	addra: IN  std_logic_VECTOR(12 downto 0);
	addrb: IN  std_logic_VECTOR(12 downto 0);
	clka : IN  std_logic;
	clkb : IN  std_logic;
	dina : IN  std_logic_VECTOR(15 downto 0);
	doutb: OUT std_logic_VECTOR(15 downto 0);
	enb  : IN  std_logic;
	wea  : IN  std_logic		);
end component;


component packet_write
	PORT(
	b8_code : IN  std_logic_vector(7 downto 0);
	clk     : IN  std_logic;          
	q1      : OUT std_logic;
	rdy     : OUT std_logic;
	counter : OUT std_logic_vector(9 downto 0));
end component;

signal addra    : std_logic_VECTOR(12 downto 0);
signal addrb    : std_logic_VECTOR(12 downto 0);
signal dina     : std_logic_VECTOR(15 downto 0);
signal b8_code  : std_logic_VECTOR( 7 downto 0);
signal counter  : std_logic_VECTOR( 9 downto 0);
signal rdy      : std_logic;
signal winkel_1 : std_logic_VECTOR( 9 downto 0):= (others => '0');
signal winkel_2 : std_logic_VECTOR( 9 downto 0):= (others => '0');
signal counter_1: std_logic_VECTOR( 9 downto 0):= (others => '0');
signal freeze   : std_logic_VECTOR( 4 downto 0):= (others => '0');
signal doutb    : std_logic_VECTOR(15 downto 0):= (others => '0');


begin

-- Port A Schreiben  / Port B Lesen

bram : frame_full
		port map (
			addra => addra,
			addrb => addrb,
			clka  => clk20,
			clkb  => clk50,
			dina  => dina,
			doutb => doutb,
			enb   => rdy,
			wea   => '1');


pwrite: packet_write
		port map(
			b8_code => b8_code,
			clk     => clk20,
			q1      => b10code,
			rdy     => rdy,
			counter => counter );


-- Daten in Blockram schreiben (Kanal A)
process (clk50) begin
	if rising_edge (clk50) then
		addra <= winkel(4 downto 0) & sram_pos;
		 dina <= sram_read;
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
			freeze <= winkel_2(4 downto 0) - 1;
		end if;
	end if;
end process;


-- Addresse zum Lesen

addrb <= freeze & counter (8 downto 1);



-- Eine Farbe aus Bram lesen
process (clk20) 
variable read_color : std_logic_vector (15 downto 0);
begin

	if rising_edge (clk20) then
		if (counter < 512) and rdy = '1' then		-- Ich brauche einen
			read_color := doutb;							-- Farbwert  JETZT
			if counter (0) = '0' then 
				b8_code <= doutb (15 downto 8);		-- Erst High Byte
			else 
				b8_code <= doutb ( 7 downto 0);		-- Dann low Byte
			end if;
		end if;
	end if;
end process;




freeze_diag  <= freeze;
winkel_diag  <= winkel;
addrb_diag   <= addrb;
doutb_diag   <= doutb;
b8_code_diag <= b8_code;
rdy_diag     <= rdy;
counter_diag <= counter;

end Behavioral;
