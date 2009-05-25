--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------
-- Hackerspace:		Das Labor    www.das-labor.org											--
-- Hacker: 				Sauron																			--
-- 																											--
-- Datum:    			14:12:03 03/09/2009 															--
-- Projekt: 			Der Borg Ventilator															--
-- Modul Name:     	packed_read   																	--
-- Beschreibung: 		Ich Empfange einen seriellen Datenstream,								--
--							Synchronisiere mich auf ihn und Überprüfe 							--
--							Synchronität und Checksummen												--
--																												--
-- Dependencies: 		B8B10 Decoder																	--
--																												--
-- Version:          V9.3.2																			--
-- 																											--
-- Additional Comments: Widerstand ist Zwecklos		   										--
--																												--
--------------------------------------------------------------------------------------
--------------------------------------------------------------------------------------

--was noch fehlt:
--Constraints für den eingang (lowskew - pad to setup)


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.numeric_std.ALL;
use IEEE.std_logic_unsigned.all;


--------------------------ENTITY-BESCHREIBUNG-----------------------------------------
entity packet_read is

    Port (

    data_in       :  in   std_logic;
	 clk           :  in   std_logic;
	 data_out      :  out  std_logic_vector (7  downto 0);
	 counter       :  out  std_logic_vector (9  downto 0);
    rdy           :  out  std_logic;
	 synchron      :  out  std_logic;
	 crc_ok        :  out  std_logic;
	 crc_er        :  out  std_logic
    --sync_ok       :  out  std_logic;
    --sync_er       :  out  std_logic
    --diag_crccount :  out  std_logic_vector (7  downto 0)
	 --crc_tmpout    :  out  std_logic_vector (15  downto 0)
    --frame_out     :  out  std_logic;
	 --code_err2     :  out  std_logic;
	 --kout          :  out  std_logic;
	 --sync_found_out:  out  std_logic;
	 --sync_reset    :  out  std_logic;
	 --sc_out        :  out  std_logic_vector (15 downto 0);
	 

	 
			 );

end packet_read;
architecture Behavioral of packet_read is

--**********************************************************************
CONSTANT packetlange: integer := 514 ; -- Länge der Datenpakete (byte) *
CONSTANT kb_puls    : integer :=  64 ; -- alle x KB ein OK Impuls      *
--**********************************************************************

CONSTANT oksum      : integer :=((1024/packetlange)*kb_puls)-1 ;


----------------Signale-innerhalb-der--Architecture-----------------------------------
signal bytecounter  : integer range 0 to packetlange+9;
signal sc_i         : integer range 0 to 65535;
signal bitcounter   : integer range 0 to 9;
signal framecounter : integer range 0 to 65535; 
signal data_i       : std_logic_vector (7 downto 0);
signal schieber_i   : std_logic_vector (9 downto 0);
signal sync_i       : std_logic_vector (59 downto 0);
signal crc_tmp      : std_logic_vector (15 downto 0);
signal crc3         : std_logic_vector (15 downto 0);
signal crchi        : std_logic_vector (7 downto 0);
signal crclo        : std_logic_vector (7 downto 0);
signal crccount     : std_logic_vector (7 downto 0) :="00000000";
signal synccount    : std_logic_vector (7 downto 0) :="00000000";


signal din_s1       : std_logic;
signal din_s2       : std_logic;
signal ce           : std_logic;
signal rdy_i        : std_logic;
signal synchron_i   : std_logic :='0';
signal sync_found   : std_logic;
signal framebeginn_i: std_logic;
signal kout_i       : std_logic;
signal sync_res     : std_logic;
constant s1         : std_logic_vector (9 downto 0) := "0010111100";
constant s2         : std_logic_vector (9 downto 0) := "1101000011";

signal sync_ok      : std_logic;
signal sync_er      : std_logic;



--------------------KOMPONENTEN-BEKANNTMACHEN-----------------------------------------
component decoder_b8b10b
	port (
	clk: IN std_logic;
	din: IN std_logic_VECTOR(9 downto 0);
	dout: OUT std_logic_VECTOR(7 downto 0);
	kout: OUT std_logic;
	ce: IN std_logic
	--code_err: OUT std_logic
			);
end component;
attribute box_type : string; 
attribute box_type of decoder_b8b10b : component is "black_box";

----------------------BEGINN-ARCHITEKTURE---------------------------------------------
BEGIN
----------------------DATEN-AUF-CLOCK-SYNCHRONISIEREN-(2*FF)--------------------------
din1:process (clk)
begin
   if rising_edge (clk) then din_s1 <= data_in; end if;
end process din1;

din2:process (clk)
begin
   if rising_edge (clk) then din_s2 <= din_s1; end if;
end process din2;
----------------------BITS-EINES-BYTES-ZÄHLEN-UND-PARALELISIEREN---------------------
bitcount:process (clk)
begin
   -- Bits zählen
   if rising_edge (clk) then  
		if sync_res = '1' or bitcounter=9 then
			bitcounter <= 0;
		else
			bitcounter <= bitcounter + 1;
		end if;
	end if;
end process bitcount;

bitready:process (clk)   
begin
	  -- Ready Signal erzeugen
	  if rising_edge (clk) then
	    if bitcounter = 7 and bytecounter < packetlange then rdy_i <= '1';
	    else rdy_i <='0';
	    end if;
     end if;		 
end process bitready;
	
bitce:process (clk)
begin
	  -- Freigabe für B8B10 Decoder	
	  if rising_edge (clk) then
	    if bitcounter = 4 then  ce <= '1';
	    else ce <= '0';
	    end if;
	 end if;
end process bitce;	

schieber_i <=sync_i(9 downto 0);
--trick 17, hier wird das schieberegister der syncronisation verwendet,
--statt ein neues zu bauen

   -- rdy signal ausgeben
   rdy<=rdy_i;

---------------------BYTES-EINES-DATENPAKETES-ZÄHLEN-----------------------------------
bytecount:process (clk)

begin	 
  if rising_edge (clk) then
      if sync_res = '1' or bytecounter=(packetlange+8) then bytecounter <=0;
		  else 
		    if ce ='1' then bytecounter <= bytecounter + 1;
		    end if;
		end if;
  end if;
end process bytecount;   

  counter<=CONV_STD_LOGIC_VECTOR(bytecounter,10);

------------------------DECODER-B8B10-EINBINDEN---------------------------------------
u0 :decoder_b8b10b
port map (clk => clk,din => schieber_i,dout => Data_i,kout => kout_i,ce => ce);				
--kout <= kout_i;
data_out<= data_i;

-----------FINDEN-EINER-SYNCHRONMARKE-(nur-fuer-neu-synchronisierung)-----------------
-- sync_i (9 downto 0) wird auch als datenquelle für den B8B10 Decoder verwendet !!		
syncsearch:process (clk)
begin
   if rising_edge (clk) then  
      sync_i <= sync_i(58 downto 0)& din_s2;
      if (sync_i (9  downto  0) = s1 or sync_i (9  downto  0) = s2) and
			(sync_i (19 downto 10) = s1 or sync_i (19 downto 10) = s2) and
			(sync_i (29 downto 20) = s1 or sync_i (29 downto 20) = s2) and
			(sync_i (39 downto 30) = s1 or sync_i (39 downto 30) = s2) and
			(sync_i (49 downto 40) = s1 or sync_i (49 downto 40) = s2) and
			(sync_i (59 downto 50) = s1 or sync_i (59 downto 50) = s2) then
			sync_found<='1'; else sync_found<='0';
	   end if;		
	end if;
end process syncsearch;
--sync_found_out<=sync_found;

-----------------------WARTEN-BIS-PACKET-START----------------------------------------
framestart:process (clk)

begin
  if clk'event and clk='1' then
    if sync_found ='1' then framecounter <= 0; 
	 else framecounter <= framecounter +1;
	 end if;
	   if framecounter = 31 then framebeginn_i <= '1'; 
	   else framebeginn_i <='0'; 
	   end if; 
 end if;
end process framestart;
--frame_out<=framebeginn_i;


------------FINDEN-EINER-SYNCHRONMARKE-(Auf-Byteebene)--------------------------------
syncp:process (clk)
begin
  if clk'event and clk='1' then
	 if ce = '1' then	                                    
		 if bytecounter > (Packetlange-1) and bytecounter < (packetlange+6) 
		 and kout_i='1' and data_i = "00011100" then
		 sc_i <= sc_i + 1;
		 else 
		 sc_i <= 0;
		 end if;
    end if;
  end if;
end process syncp;

syncset:process (clk)	 
begin
  if clk'event and clk='1' then
    if bytecounter = (packetlange+6) then 
		if sc_i=6 then 
			synchron_i<='1';
		else 
			synchron_i<='0';
		end if;
	 end if;
  end if;
end process syncset;

--sc_out<=CONV_STD_LOGIC_VECTOR(sc_i,16);
synchron<=synchron_i;

-----------------BEI-BEDARF-NEU-SYNCHRONISIEREN---------------------------------------
sync_res <= not synchron_i and framebeginn_i;
--sync_reset<=sync_res;

------------------CKECKSUMME ÜBERPRÜFEN-----------------------------------------------
crc1:process (clk)-- Hi Byte aus Datenpacket
begin
if rising_edge(clk) then
  if bytecounter = (packetlange+6) then 
     crchi <=data_i ;
  end if;
end if;  
end process crc1;

crc2:process (clk)-- Lo Byte aus Datenpacket
begin
if rising_edge(clk) then
  if bytecounter = (packetlange+7) then 
     crclo <=data_i ;
  end if;
end if;  
end process crc2;

crca:process (clk)--Checksumme Zwischenspeichern
begin
if rising_edge(clk) then
  if bytecounter = (packetlange+1) then 
     crc3 <=crc_tmp ;
  end if;
end if;  
end process crca;

crcp:process (clk)-- Checksumme berechnen
begin
if rising_edge(clk) then
  if (ce = '1' and bytecounter < (packetlange)) then 
    crc_tmp <= crc_tmp + data_i;
  elsif bytecounter = (packetlange+7) then
        crc_tmp <= (others =>'0');  
  end if;	 
end if;
end process crcp;
--crc_tmpout<=crc_tmp;

crcok:process (clk)-- Impuls wenn CRC OK (jedes x. kb, abhängig von oksum)
begin
if rising_edge(clk) then
  if (ce = '1' and bytecounter = 0) and crc3 = crchi & crclo and synchron_i = '1' then
		crccount <= crccount + 1;
		if crccount = oksum then 
       crc_ok <='1';  
		 crccount <= (others => '0');
		else 
		 crc_ok <='0';
		end if; 
  else 
       crc_ok <='0';
  end if;
end if;
end process crcok;
--diag_crccount<= crccount;

crcer:process (clk)-- Impuls wenn CRC nicht OK
begin
if rising_edge(clk) then
  if (ce = '1' and bytecounter = 0) and crc3 /= crchi & crclo then
       crc_er <='1';   	 
  else 
       crc_er <='0';
  end if;
end if;
end process crcer;

syncok:process (clk)-- Impuls wenn sync OK
begin
if rising_edge(clk) then
  if (ce = '1' and bytecounter = 0) and synchron_i = '1' then
		synccount <= synccount + 1;
	   if synccount = oksum then 
	    sync_ok <='1';
		 synccount <= (others => '0');
		else sync_ok <='0';
        end if;
      else 
       sync_ok <='0';
  end if;
end if;
end process syncok;

syncer:process (clk)-- Impuls wenn sync nicht OK
begin
if rising_edge(clk) then
  if (ce = '1' and bytecounter = 0) and synchron_i = '0' then
       sync_er <='1';   	 
  else 
       sync_er <='0';
  end if;
end if;
end process syncer;
--------------------------------------------------------------------------------------
end Behavioral;


