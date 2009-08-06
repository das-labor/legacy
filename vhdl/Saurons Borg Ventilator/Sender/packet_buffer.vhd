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
    b10code_k1   : out  STD_LOGIC;			  
    b10code_k2   : out  STD_LOGIC;			  
    b10code_k3   : out  STD_LOGIC;			  
    b10code_k4   : out  STD_LOGIC;			  
    freeze_k1_diag  : out STD_LOGIC_VECTOR ( 4 downto 0);
    freeze_k2_diag  : out STD_LOGIC_VECTOR ( 4 downto 0);
    freeze_k3_diag  : out STD_LOGIC_VECTOR ( 4 downto 0);
    freeze_k4_diag  : out STD_LOGIC_VECTOR ( 4 downto 0);
    winkel_diag  : out STD_LOGIC_VECTOR ( 9 downto 0);
    addrb_diag   : out STD_LOGIC_VECTOR (12 downto 0);
	 doutb_diag   : out STD_LOGIC_VECTOR (15 downto 0);
    b8_code_k1_diag : out STD_LOGIC_VECTOR ( 7 downto 0);
    b8_code_k2_diag : out STD_LOGIC_VECTOR ( 7 downto 0);
    b8_code_k3_diag : out STD_LOGIC_VECTOR ( 7 downto 0);
    b8_code_k4_diag : out STD_LOGIC_VECTOR ( 7 downto 0);
    rdy_k1_diag     : out STD_LOGIC;
    rdy_k2_diag     : out STD_LOGIC;
    rdy_k3_diag     : out STD_LOGIC;
    rdy_k4_diag     : out STD_LOGIC;
	 counter_k1_diag : out STD_LOGIC_VECTOR ( 9 downto 0);
	 counter_k2_diag : out STD_LOGIC_VECTOR ( 9 downto 0);
	 counter_k3_diag : out STD_LOGIC_VECTOR ( 9 downto 0);
	 counter_k4_diag : out STD_LOGIC_VECTOR ( 9 downto 0);
	 doutb_k1_diag : out STD_LOGIC_VECTOR ( 15 downto 0);
	 doutb_k2_diag : out STD_LOGIC_VECTOR ( 15 downto 0);
	 doutb_k3_diag : out STD_LOGIC_VECTOR ( 15 downto 0);
	 doutb_k4_diag : out STD_LOGIC_VECTOR ( 15 downto 0)
	 
 
																						);
end packet_buffer;

architecture Behavioral of packet_buffer is

component frame_full
	port (
	addra: IN  std_logic_VECTOR(12 downto 0);-- 8192
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
   en      : IN  std_logic;	
	q1      : OUT std_logic;
	rdy     : OUT std_logic;
	counter : OUT std_logic_vector(9 downto 0));
end component;

signal addra    : std_logic_VECTOR(12 downto 0);
signal addrb    : std_logic_VECTOR(12 downto 0);
signal dina     : std_logic_VECTOR(15 downto 0);
signal b8_code_k1  : std_logic_VECTOR( 7 downto 0);
signal b8_code_k2  : std_logic_VECTOR( 7 downto 0);
signal b8_code_k3  : std_logic_VECTOR( 7 downto 0);
signal b8_code_k4  : std_logic_VECTOR( 7 downto 0);
signal counter_k1  : std_logic_VECTOR( 9 downto 0);
signal counter_k2  : std_logic_VECTOR( 9 downto 0);
signal counter_k3  : std_logic_VECTOR( 9 downto 0);
signal counter_k4  : std_logic_VECTOR( 9 downto 0);
signal rdy_k1      : std_logic;
signal rdy_k2      : std_logic;
signal rdy_k3      : std_logic;
signal rdy_k4      : std_logic;
signal rdy_k1_2      : std_logic;
signal rdy_k2_2      : std_logic;
signal rdy_k3_2      : std_logic;
signal rdy_k4_2      : std_logic;
signal rdy_k1_3      : std_logic;
signal rdy_k2_3      : std_logic;
signal rdy_k3_3      : std_logic;
signal rdy_k4_3      : std_logic;
signal en_k1       : std_logic := '0' ;
signal en_k2       : std_logic := '0' ;
signal en_k3       : std_logic := '0' ;
signal en_k4       : std_logic := '0' ;
signal counter_k1_1: std_logic_VECTOR( 9 downto 0):= (others => '0');
signal counter_k2_1: std_logic_VECTOR( 9 downto 0):= (others => '0');
signal counter_k3_1: std_logic_VECTOR( 9 downto 0):= (others => '0');
signal counter_k4_1: std_logic_VECTOR( 9 downto 0):= (others => '0');
signal freeze_k1   : std_logic_VECTOR( 4 downto 0):= (others => '0');
signal freeze_k2   : std_logic_VECTOR( 4 downto 0):= (others => '0');
signal freeze_k3   : std_logic_VECTOR( 4 downto 0):= (others => '0');
signal freeze_k4   : std_logic_VECTOR( 4 downto 0):= (others => '0');
signal winkel_1 	: std_logic_VECTOR( 9 downto 0):= (others => '0');
signal winkel_2 	: std_logic_VECTOR( 9 downto 0):= (others => '0');
signal doutb    	: std_logic_VECTOR(15 downto 0):= (others => '0');
signal doutb_k1    	: std_logic_VECTOR(15 downto 0):= (others => '0');
signal doutb_k2    	: std_logic_VECTOR(15 downto 0):= (others => '0');
signal doutb_k3    	: std_logic_VECTOR(15 downto 0):= (others => '0');
signal doutb_k4    	: std_logic_VECTOR(15 downto 0):= (others => '0');
signal en_counter	: std_logic_VECTOR(15 downto 0):= (others => '0');


--****************************************************************
CONSTANT packetlange: integer := 514;  -- Länge der Datenpakete  *
--****************************************************************


begin

-- Port A Schreiben  / Port B Lesen

bram : frame_full
		port map (
			addra => addra,
			addrb => addrb,
			clka  => clk50,
			clkb  => clk20,
			dina  => dina,
			doutb => doutb,
			enb   => '1',
			wea   => '1');


kanal_1: packet_write
		port map(
			b8_code => b8_code_k1,
			clk     => clk20,
			en      => en_k1,
			q1      => b10code_k1,
			rdy     => rdy_k1,
			counter => counter_k1 );


kanal_2: packet_write
		port map(
			b8_code => b8_code_k2,
			clk     => clk20,
			en      => en_k2,
			q1      => b10code_k2,
			rdy     => rdy_k2,
			counter => counter_k2 );


kanal_3: packet_write
		port map(
			b8_code => b8_code_k3,
			clk     => clk20,
			en      => en_k3,
			q1      => b10code_k3,
			rdy     => rdy_k3,
			counter => counter_k3 );


kanal_4: packet_write
		port map(
			b8_code => b8_code_k4,
			clk     => clk20,
			en      => en_k4,
			q1      => b10code_k4,
			rdy     => rdy_k4,
			counter => counter_k4 );


--------------------------
-- Packet_write Enablen --
--------------------------

process (clk20) 
constant packetlange_gesamt :integer := (packetlange + 8); 	-- synchronmarke und crc zum Paket dazu zählen
constant tile :integer := (packetlange_gesamt / 4);        	-- packet um 1/4 versetzen
constant versatz1: integer := (1 * tile * 10) + 0;	  			-- in Bits Umrechnen und um 2 bit versetzen
constant versatz2: integer := (2 * tile * 10) + 2;				-- in Bits Umrechnen und um 2 bit versetzen
constant versatz3: integer := (3 * tile * 10) + 4;				-- in Bits Umrechnen und um 2 bit versetzen
constant versatz4: integer := (4 * tile * 10) + 6;				-- in Bits Umrechnen und um 2 bit versetzen
begin

	if rising_edge (clk20) then
	en_counter <= en_counter + 1;
		if    en_counter = versatz1 then en_k1 <= '1';
		elsif en_counter = versatz2 then en_k2 <= '1';
		elsif en_counter = versatz3 then en_k3 <= '1';
		elsif en_counter = versatz4 then en_k4 <= '1';
		end if;
	end if;
end process;


---------------------------------
-- Daten in Blockram schreiben --
---------------------------------

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
		counter_k1_1 <= counter_k1;
		counter_k2_1 <= counter_k2;
		counter_k3_1 <= counter_k3;
		counter_k4_1 <= counter_k4;
	end if;
end process;


-----------------------------------
-- Winkel am Paketanfang Sichern --
-----------------------------------

process (clk20) begin	if rising_edge (clk20) then
		if counter_k1 = 0 and counter_k1_1 /= 0 then
			freeze_k1 <= winkel_2(4 downto 0) - 1;   end if;	end if; 	end process;

process (clk20) begin	if rising_edge (clk20) then
		if counter_k2 = 0 and counter_k2_1 /= 0 then
			freeze_k2 <= winkel_2(4 downto 0) - 1;   end if;	end if; 	end process;

process (clk20) begin	if rising_edge (clk20) then
		if counter_k3 = 0 and counter_k3_1 /= 0 then
			freeze_k3 <= winkel_2(4 downto 0) - 1;   end if;	end if; 	end process;

process (clk20) begin	if rising_edge (clk20) then
		if counter_k4 = 0 and counter_k4_1 /= 0 then
			freeze_k4 <= winkel_2(4 downto 0) - 1;   end if;	end if; 	end process;


------------------------
-- Addresse zum Lesen --
------------------------


process (clk20) begin
	if rising_edge (clk20) then
	
		if 	rdy_k1 = '1' and counter_k1 (0) = '0' then addrb <= freeze_k1 & counter_k1 (8 downto 1);
		elsif	rdy_k2 = '1' and counter_k2 (0) = '0' then addrb <= freeze_k2 & counter_k2 (8 downto 1);
		elsif	rdy_k3 = '1' and counter_k3 (0) = '0' then addrb <= freeze_k3 & counter_k3 (8 downto 1);
		elsif	rdy_k4 = '1' and counter_k4 (0) = '0' then addrb <= freeze_k4 & counter_k4 (8 downto 1);
		end if;
		
			rdy_k1_2 <= rdy_k1;				-- Nach der Angabe der BRAM Adresse 
			rdy_k2_2 <= rdy_k2;				-- Dauert es 2 Takte, bis die gewünschten Daten 
			rdy_k3_2 <= rdy_k3;				-- zur verfügung Stehen
			rdy_k4_2 <= rdy_k4;

			rdy_k1_3 <= rdy_k1_2;
			rdy_k2_3 <= rdy_k2_2;
			rdy_k3_3 <= rdy_k3_2;
			rdy_k4_3 <= rdy_k4_2;

	end if;
end process;

process (clk20) begin
	if rising_edge (clk20) then
		if 	rdy_k1_3 = '1' and counter_k1 (0) = '0' then 	doutb_k1 <= doutb;
		elsif rdy_k2_3 = '1' and counter_k2 (0) = '0' then 	doutb_k2 <= doutb;
		elsif rdy_k3_3 = '1' and counter_k3 (0) = '0' then 	doutb_k3 <= doutb;
		elsif rdy_k4_3 = '1' and counter_k4 (0) = '0' then 	doutb_k4 <= doutb;
		end if;
	end if;
end process;


----------------------------------
-- Farbe aus Bram Adresse lesen --
----------------------------------

process (clk20) 
begin
	if rising_edge (clk20) then
		if (counter_k1 < 512) and rdy_k1_3 = '1' then 	-- Zeitpunkt zum Farbe 
			if counter_k1 (0) = '0' then 						-- lesen bestimmen
				b8_code_k1 <= doutb    (15 downto 8);		-- High Byte direct lesen  
			else 														-- (Spart einen takt)
				b8_code_k1 <= doutb_k1 ( 7 downto 0);		-- Low byte aus Zwischen
			end if;													-- speicher lesen
		end if;
	end if;
end process;


process (clk20) 
begin
	if rising_edge (clk20) then
		if (counter_k2 < 512) and rdy_k2_3 = '1' then	
			if counter_k2 (0) = '0' then 
				b8_code_k2 <= doutb    (15 downto 8);		 
			else 
				b8_code_k2 <= doutb_k2 ( 7 downto 0);		 
			end if;
		end if;
	end if;
end process;


process (clk20) 
begin
	if rising_edge (clk20) then
		if (counter_k3 < 512) and rdy_k3_3 = '1' then	
			if counter_k3 (0) = '0' then 
				b8_code_k3 <= doutb    (15 downto 8);		 
			else 
				b8_code_k3 <= doutb_k3 ( 7 downto 0);		 
			end if;
		end if;
	end if;
end process;


process (clk20) 
begin
	if rising_edge (clk20) then
		if (counter_k4 < 512) and rdy_k4_3 = '1' then	
			if counter_k4 (0) = '0' then 
				b8_code_k4 <= doutb    (15 downto 8);		 
			else 
				b8_code_k4 <= doutb_k4 ( 7 downto 0);		 
			end if;
		end if;
	end if;
end process;


--------------
-- Diagnose --
--------------

freeze_k1_diag  <= freeze_k1;
freeze_k2_diag  <= freeze_k2;
freeze_k3_diag  <= freeze_k3;
freeze_k4_diag  <= freeze_k4;
winkel_diag  <= winkel;
addrb_diag   <= addrb;
doutb_diag   <= doutb;
b8_code_k1_diag <= b8_code_k1;
b8_code_k2_diag <= b8_code_k2;
b8_code_k3_diag <= b8_code_k3;
b8_code_k4_diag <= b8_code_k4;
rdy_k1_diag     <= rdy_k1;
rdy_k2_diag     <= rdy_k2;
rdy_k3_diag     <= rdy_k3;
rdy_k4_diag     <= rdy_k4;
counter_k1_diag <= counter_k1;
counter_k2_diag <= counter_k2;
counter_k3_diag <= counter_k3;
counter_k4_diag <= counter_k4;
doutb_k1_diag	 <= doutb_k1;
doutb_k2_diag	 <= doutb_k2;
doutb_k3_diag	 <= doutb_k3;
doutb_k4_diag	 <= doutb_k4;


end Behavioral;
