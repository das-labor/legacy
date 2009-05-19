--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
-- Hackerspace:		Das Labor    www.das-labor.org											--
-- Hacker: 				Sauron																			--
-- 																											--
-- Datum:    			09.03.2009        															--
-- Projekt: 			Der Borg Ventilator															--
-- Modul Name:     	packed_write 																	--
-- Beschreibung: 		Ich Erstelle einen seriellen Datenstream, der Gleichspannungs- --
--                   frei ist. An jedes Datenpaket kommt noch eine Synchron-marke,  --
--							und noch eine 16 Bit Checksumme.											--
--																												--
-- Dependencies: 		B8B10 Encoder																	--
--																												--
-- Version:          V9.3.1																			--
-- 																											--
-- Additional Comments: Wiederstand ist Zwecklos												--
--																												--
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------

-- was noch fehlt:
-- constraints für den ausgang
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.numeric_std.ALL;
use IEEE.std_logic_unsigned.all;


--------------------------ENTITY-BESCHREIBUNG---------------------------------------
entity packet_write is

    Port (

       b8_code         :  in  std_logic_vector (7 downto 0);
		 clk             :  in  std_logic;
		 q1				  :  out std_logic;
		 rdy             :  out std_logic;
		 counter         :  out std_logic_vector (9 downto 0)

--		 diag_bitcounter :  out std_logic_vector (3 downto 0);
--		 diag_schieb_reg :  out std_logic_vector (9 downto 0);
--	    diag_b10        :  out std_logic_vector (9 downto 0);
--		 diag_crc1       :  out std_logic_vector (7 downto 0);
--		 diag_crc2       :  out std_logic_vector (7 downto 0);
--		 diag_daten      :  out std_logic_vector (7 downto 0);
--		 diag_crc		  :  out std_logic_vector (15 downto 0)
	 
	 
			 );

end packet_write;
architecture Behavioral of packet_write is

--****************************************************************
CONSTANT packetlange: integer := 512;  -- Länge der Datenpakete  *
--****************************************************************


----------------SIGNALE-INNERHALB-DER-ARCHITECTURE------------------------------------

signal bitcounter   : integer range 0 to 10;

--signal sync_res     : std_logic;
--signal rdy_i        : std_logic;
signal schieb_reg   : std_logic_vector (9 downto 0);
signal daten_i      : std_logic_vector (7 downto 0);
signal b10_code     : std_logic_vector (9 downto 0);
signal crc_tmp      : std_logic_vector (15 downto 0);
signal crc1         : std_logic_vector (7 downto 0);
signal crc2         : std_logic_vector (7 downto 0);
signal kin2         : std_logic;
signal encoder_ce   : std_logic;
signal q1_i1        : std_logic;
signal q1_i2        : std_logic;
signal bytecounter  : integer range 0 to 521;
	

--------------------KOMPONENTEN-BEKANNTMACHEN-----------------------------------------

component encoder_b8b10b
	port (
	din         : in  std_logic_VECTOR(7 downto 0);
	kin         : in  std_logic;
	ce          : in  std_logic;
	clk         : in  std_logic;
	dout        : out std_logic_VECTOR(9 downto 0));
end component;

attribute box_type : string; 
attribute box_type of encoder_b8b10b : component is "black_box";


----------------------BEGINN-ARCHITECTURE---------------------------------------------
BEGIN

----------------------BITS-EINES-BYTES-ZÄHLEN-----------------------------------------
bitcount:process (clk)
begin
   if rising_edge (clk) then  
		if bitcounter=9 then
			bitcounter <= 0;
		else
			bitcounter <= bitcounter + 1;
		end if;
	end if;
end process bitcount;
--diag_bitcounter<= CONV_STD_LOGIC_VECTOR(bitcounter, 4);


---------------------BYTES-EINES-DATENPAKETES-ZÄHLEN----------------------------------
bytecount:process (clk)

begin	 
  if rising_edge (clk) then
      if bytecounter=(packetlange+8) then bytecounter <=0;
		  else 
		    if bitcounter = 0 then bytecounter <= bytecounter + 1;
		    end if;
		end if;
  end if;
end process bytecount;   

  counter<=CONV_STD_LOGIC_VECTOR(bytecounter,10);
-----------------2=BEREIT-FÜR-EINGABEN---7=ENCODIERUNG-STARTEN------------------------
bitready2:process (clk)   
begin
	  if rising_edge (clk) then
	    if bitcounter = 2 and bytecounter < packetlange then rdy <= '1';
	    else rdy <='0';
	    end if;
     end if;		 
end process bitready2;

bitready7:process (clk)   
begin
	  if rising_edge (clk) then
	    if bitcounter = 7 then encoder_ce <= '1';
	    else encoder_ce <='0';
	    end if;
     end if;		 
end process bitready7;

-----------SCHIEBEREGISTER---PARALLEL REIN----SERIEL-RAUS-----------------------------
schieb:process (clk)
begin
if rising_edge (clk) then  
	if bitcounter=0 then
			schieb_reg <= b10_code;
		else		  
		schieb_reg <= schieb_reg(8 downto 0) & '0';
	end if;
end if;  
end process schieb;
--diag_schieb_reg <= schieb_reg;
--diag_b10        <=b10_code;
q1_i1 <=schieb_reg (9);

----------------------DATEN-AUF-CLOCK-SYNCHRONISIEREN-(2*FF)--------------------------
dout1:process (clk)
begin
   if rising_edge (clk) then q1_i2 <= q1_i1; end if;
end process dout1;

dout2:process (clk)
begin
   if rising_edge (clk) then q1 <= q1_i2; end if;
end process dout2;

--------------------------DATEN-ODER-CRC-ODER-SYNC-SENDEN-?---------------------------
daten:process (clk)
begin
if rising_edge (clk) then  
	if bitcounter=0 then
		if bytecounter > (Packetlange-1) and bytecounter < (packetlange+6) then 
			daten_i <= "00011100"; -- Synchronmarke senden
			kin2 <= '1'; -- Synchronmarke encodieren
		elsif bytecounter = packetlange + 6 then
			daten_i <= crc1; -- crc Hi Byte Senden
		   kin2 <= '0'; 
		elsif bytecounter = packetlange + 7 then
			daten_i <= crc2; -- crc Lo Byte senden
			kin2 <= '0';
		else	 
			daten_i <= b8_code; -- Normale Daten senden
			kin2 <= '0';
		end if; 
	end if;
end if;  
end process daten;
--diag_daten <= daten_i;
---------CHECKSUMME-BERECHNEN--------------------------------------------------------- 

crccount:process (clk)
begin
if rising_edge(clk) then
  if (bitcounter = 0 and bytecounter < (packetlange)) then 
    crc_tmp <= crc_tmp + b8_code;
  elsif bytecounter = (packetlange+7) then
        crc_tmp <= (others =>'0');  
  end if;	 
end if;
end process crccount;

crcsave:process (clk)
begin
if rising_edge(clk) then
  if (bitcounter = 0 and bytecounter = packetlange) then 
  crc1 <=crc_tmp (15 downto 8);
  crc2 <=crc_tmp (7  downto 0);
  end if;	 
end if;
end process crcsave;

--diag_crc <= crc_tmp;
--diag_crc1 <= crc1;
--diag_crc2 <= crc2;
-------------------------B8B10-ENCODER-EINBINDEN--------------------------------------
u1 : encoder_b8b10b
		port map (
			din => daten_i,
			kin => kin2,
			clk => clk,
			ce  => encoder_ce,
			dout => b10_code);

-----------------------WIE?-----DAS-WARS-SCHON-?---------------BYE--------------------
end Behavioral;
