LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;
USE ieee.numeric_std.ALL;
use IEEE.MATH_REAL.all;
use work.sim_bmppack.all;


ENTITY top_top_sch_tb IS
END top_top_sch_tb;

ARCHITECTURE behavioral OF top_top_sch_tb IS 

   COMPONENT top
   PORT( schaltin	:	IN	STD_LOGIC_VECTOR (7 DOWNTO 0); 
          tast	:	IN	STD_LOGIC_VECTOR (3 DOWNTO 0); 
          led	:	OUT	STD_LOGIC_VECTOR (7 DOWNTO 0); 
          clk50	:	OUT	STD_LOGIC; 
          clk	:	IN	STD_LOGIC; 
          out1	:	OUT	STD_LOGIC; 
          out2	:	OUT	STD_LOGIC; 
          out3	:	OUT	STD_LOGIC; 
          out4	:	OUT	STD_LOGIC; 
          trig	:	OUT	STD_LOGIC; 
          gnd3	:	OUT	STD_LOGIC; 
          gnd2	:	OUT	STD_LOGIC; 
          gnd1	:	OUT	STD_LOGIC; 
          out0	:	OUT	STD_LOGIC; 
          gnd0	:	OUT	STD_LOGIC; 
          clk100	:	OUT	STD_LOGIC; 
          gnd4	:	OUT	STD_LOGIC; 
          display	:	OUT	STD_LOGIC_VECTOR (11 DOWNTO 0); 
          sram_oe	:	OUT	STD_LOGIC; 
          sram_we	:	OUT	STD_LOGIC; 
          sram_adr	:	OUT	STD_LOGIC_VECTOR (17 DOWNTO 0); 
          sram_1_io	:	INOUT	STD_LOGIC_VECTOR (15 DOWNTO 0); 
          sram_1_ce	:	OUT	STD_LOGIC; 
          sram_1_ub	:	OUT	STD_LOGIC; 
          sram_1_lb	:	OUT	STD_LOGIC; 
          sram_2_ce	:	OUT	STD_LOGIC; 
          sram_2_io	:	INOUT	STD_LOGIC_VECTOR (15 DOWNTO 0); 
          sram_2_ub	:	OUT	STD_LOGIC; 
          sram_2_lb	:	OUT	STD_LOGIC; 
          winkel	:	IN	STD_LOGIC_VECTOR (9 DOWNTO 0); 
          winkel_ram	:	OUT	STD_LOGIC_VECTOR (9 DOWNTO 0); 
          b10code	:	OUT	STD_LOGIC; 
          rdy_diag	:	OUT	STD_LOGIC; 
          freeze_diag	:	OUT	STD_LOGIC_VECTOR (2 DOWNTO 0); 
          winkel_diag	:	OUT	STD_LOGIC_VECTOR (9 DOWNTO 0); 
          addra_diag	:	OUT	STD_LOGIC_VECTOR (11 DOWNTO 0); 
          b8_code_diag	:	OUT	STD_LOGIC_VECTOR (7 DOWNTO 0);
			 counter_diag  :	OUT	STD_LOGIC_VECTOR (9 DOWNTO 0)
			 
			 );
   END COMPONENT;

   SIGNAL schaltin	:	STD_LOGIC_VECTOR (7 DOWNTO 0);
   SIGNAL tast	:	STD_LOGIC_VECTOR (3 DOWNTO 0);
   SIGNAL led	:	STD_LOGIC_VECTOR (7 DOWNTO 0);
   SIGNAL clk50	:	STD_LOGIC;
   SIGNAL clk	:	STD_LOGIC;
   SIGNAL out1	:	STD_LOGIC;
   SIGNAL out2	:	STD_LOGIC;
   SIGNAL out3	:	STD_LOGIC;
   SIGNAL out4	:	STD_LOGIC;
   SIGNAL trig	:	STD_LOGIC;
   SIGNAL gnd3	:	STD_LOGIC;
   SIGNAL gnd2	:	STD_LOGIC;
   SIGNAL gnd1	:	STD_LOGIC;
   SIGNAL out0	:	STD_LOGIC;
   SIGNAL gnd0	:	STD_LOGIC;
   SIGNAL clk100	:	STD_LOGIC;
   SIGNAL gnd4	:	STD_LOGIC;
   SIGNAL display	:	STD_LOGIC_VECTOR (11 DOWNTO 0);
   SIGNAL sram_oe	:	STD_LOGIC;
   SIGNAL sram_we	:	STD_LOGIC;
   SIGNAL sram_adr	:	STD_LOGIC_VECTOR (17 DOWNTO 0);
   SIGNAL sram_1_io	:	STD_LOGIC_VECTOR (15 DOWNTO 0);
   SIGNAL sram_1_ce	:	STD_LOGIC;
   SIGNAL sram_1_ub	:	STD_LOGIC;
   SIGNAL sram_1_lb	:	STD_LOGIC;
   SIGNAL sram_2_ce	:	STD_LOGIC;
   SIGNAL sram_2_io	:	STD_LOGIC_VECTOR (15 DOWNTO 0);
   SIGNAL sram_2_ub	:	STD_LOGIC;
   SIGNAL sram_2_lb	:	STD_LOGIC;
   SIGNAL winkel	:	STD_LOGIC_VECTOR (9 DOWNTO 0):= (others => '0');
   SIGNAL winkel_ram	:	STD_LOGIC_VECTOR (9 DOWNTO 0);
   SIGNAL b10code	:	STD_LOGIC;
   SIGNAL rdy_diag	:	STD_LOGIC;
   SIGNAL freeze_diag	:	STD_LOGIC_VECTOR (2 DOWNTO 0);
   SIGNAL winkel_diag	:	STD_LOGIC_VECTOR (9 DOWNTO 0);
   SIGNAL addra_diag	:	STD_LOGIC_VECTOR (11 DOWNTO 0);
   SIGNAL b8_code_diag	:	STD_LOGIC_VECTOR (7 DOWNTO 0);
   SIGNAL counter_diag	:	STD_LOGIC_VECTOR (9 DOWNTO 0);



signal colordatae: std_logic_vector(15 downto 0):= x"0000" ; signal xe,ye:integer:=0;
signal colordatai: std_logic_vector(23 downto 0) ; signal xi,yi:integer;
signal colordatal: std_logic_vector(23 downto 0):= x"000000" ; signal xl,yl:integer:=0;

type memory2 is array (0 to 511, 0 to 511) of bit_vector (15 downto 0);
shared variable visual : memory2; 
shared variable input_picture : memory2;

shared variable fname : string ( 1 to 16):="lena_copy000.bmp";





BEGIN

   UUT: top PORT MAP(
		schaltin => schaltin, 
		tast => tast, 
		led => led, 
		clk50 => clk50, 
		clk => clk, 
		out1 => out1, 
		out2 => out2, 
		out3 => out3, 
		out4 => out4, 
		trig => trig, 
		gnd3 => gnd3, 
		gnd2 => gnd2, 
		gnd1 => gnd1, 
		out0 => out0, 
		gnd0 => gnd0, 
		clk100 => clk100, 
		gnd4 => gnd4, 
		display => display, 
		sram_oe => sram_oe, 
		sram_we => sram_we, 
		sram_adr => sram_adr, 
		sram_1_io => sram_1_io, 
		sram_1_ce => sram_1_ce, 
		sram_1_ub => sram_1_ub, 
		sram_1_lb => sram_1_lb, 
		sram_2_ce => sram_2_ce, 
		sram_2_io => sram_2_io, 
		sram_2_ub => sram_2_ub, 
		sram_2_lb => sram_2_lb, 
		winkel => winkel, 
		winkel_ram => winkel_ram, 
		b10code => b10code, 
		rdy_diag => rdy_diag, 
		freeze_diag => freeze_diag, 
		winkel_diag => winkel_diag, 
		addra_diag => addra_diag, 
		b8_code_diag => b8_code_diag,
		counter_diag => counter_diag
   );

-- *** Test Bench - User Defined Section ***
---------------------
--  Takterzeugung  --
---------------------

-- generiere 50 Mhz Takt
process begin
		clk <= '1';
		wait for 10 ns;
		clk <= '0';
		wait for 10 ns; 
end process;


----------------------------------------
-- Visualisierung der gelesenen Daten --
----------------------------------------
process (clk50) 
constant center_x : real := 255.5;
constant center_y : real := 255.5;
variable w			: real;
variable s,c,x,y 	: real;
variable w_int    : integer;
begin

if rising_edge (clk50) then

--  winkel berechnen
w_int := (conv_integer(winkel_diag));

-- Jeder Flügel ist 90 Grad (256) versetzt
		if    addra_diag(1 downto 0) = "01" then  
				w_int := w_int + 256;
		elsif addra_diag(1 downto 0) = "10" then  
				w_int := w_int + 512;
		elsif addra_diag(1 downto 0) = "11" then
				w_int := w_int + 768;
		end if;

-- Winkel gröser eine Umdrehung ? 
if w_int > 1023 then w_int := w_int - 1023; end if;


w := ((real(w_int) / 1023.0)* 6.28) + 3.14;

s := sin(w) * real (conv_integer (addra_diag) );
c := cos(w) * real (conv_integer (addra_diag) );

-- x / y Koordinaten fur den winkel 
if addra_diag < 512 then
	x := center_x + s;
	y := center_y + c;
else 
	x := 0.0;
	y := 0.0;
end if;
			
-- Schwarz gelesen ?? dann Rot draus machen (für debugging)
if b8_code_diag = x"0000" then 
	colordatae <= x"008f";
else
	                          colordatae (7 downto 0) <= 	b8_code_diag;
end if ;						
 								
xe <= integer(x) ; ye <= integer(y) ;


-- Entgülige Daten in einen Ram Speicher schieben
visual ((xe+0),(ye+0)):= To_bitvector(colordatae);
visual ((xe+1),(ye+0)):= To_bitvector(colordatae);
visual ((xe+0),(ye+1)):= To_bitvector(colordatae);



end if;
end process;

------------------------------------
-- LEDS nach einer zeit abdunkeln --
------------------------------------
--process 
--
--variable dunkel   : integer;
--variable read_byte: std_logic_vector(15 downto 0);
--variable r,g,b    : integer; 
--begin
--
--wait for 1500 us;
--
--for dunkel_y in 0 to 511 loop
--	for dunkel_x in 0 to 511 loop
--	
--	read_byte := To_StdLogicVector(visual (dunkel_x,dunkel_y));
--	
--
--		r := CONV_INTEGER (read_byte ( 4 downto  0));
--		g := CONV_INTEGER (read_byte (10 downto  5));
--		b := CONV_INTEGER (read_byte (15 downto 11));
--		
--		r := r - 1 ; if r < 0 then r:= 0; end if;
--		g := g - 1 ; if g < 0 then g:= 0; end if;
--		b := b - 1 ; if b < 0 then b:= 0; end if;
--		
--		read_byte   := CONV_STD_LOGIC_VECTOR (b,5) & 
--							CONV_STD_LOGIC_VECTOR (g,6) &
--							CONV_STD_LOGIC_VECTOR (r,5);
--
--	visual (dunkel_x, dunkel_y) := To_bitvector(read_byte);
--	
--	end loop;
--end loop;
--end process;


----------------------
-- SRAM Simulieren  --
----------------------

process (clk100)
type memory is array (0 to 1023, 0 to 511) of bit_vector (15 downto 0);

variable ram : memory; 
variable x,xr :integer;
variable y,yr :integer;
constant readcolor : std_logic_vector (23 downto 0) :=x"8040a0";

begin

if rising_edge (clk100) then
			x := conv_integer(sram_1_ce & sram_adr( 8 downto 0));
			y := conv_integer(sram_adr(17 downto 9));

	if sram_oe = '0' then -- lesen 
	
		if clk50 = '0' then -- lesen bei fallender flanke auf clk100
		
			if sram_1_ce = '1' then
		
				sram_1_io <= (others => 'Z');
				sram_2_io <=To_StdLogicVector (ram(x,y)); 	-- Leseadresse
				
			else					-- lesen aus speicher chip 1
		
				sram_1_io <= To_StdLogicVector (ram(x,y));
				sram_2_io <= (others => 'Z');
		
			end if;
		end if;
	else 			-- schreiben in den speicher

				sram_1_io <= (others => 'Z');-- wenn in den speicher geschrieben wird
				sram_2_io <= (others => 'Z');-- darf der Speicher keine Daten ausgeben
		
		if clk50 = '1' then -- schreiben in den speicher bei steigender flanke
		
			IF sram_1_ce = '0' then -- chip 0
		
				ram(x,y):= To_bitvector(sram_1_io) ; -- Daten in ein 1024 x 512 Array speichern
		
			
			else  -- und das selbe nochmal für den 2. Bildspeicher

				ram(x,y):= To_bitvector(sram_2_io) ;

			end if;
		end if;	
	end if;
end if;
end process;

-------------------------
--  ad eingang testen  --
-------------------------

----liest das bild unten links und gibt es über das ad interface aus; 
--
--process 
--variable x,y:integer:=0;
--variable count : integer := 0;
--variable picture_counter :integer := 0;
--variable w1,w2,w3 : integer ; 
--variable wname : string ( 1 to 14):="wurfel0000.bmp";
--variable char1,char2,char3 : character ;
--
--begin
--
--wait until rising_edge (clk50);
--
---- Daten für ad_dat / ad_adr aus dem speicher lesen
--			ad_adr <= conv_std_logic_vector(y,9) & conv_std_logic_vector(x,9);
--		
--         ad_dat <= To_StdLogicVector(input_picture(x,y));
----         ad_dat <= ad_adr (15 downto 0);
--			
--		-- nach count takten schreib-impuls erzeugen
--		count := count + 1;
--		if count = 4 then count := 0; end if;
--			
--		if count = 0 then ad_wr<='1';
--		else ad_wr<='0';
--		end if;
--			
--		if ad_wr = '1' then x := x + 1 ; end if;
--
----------------------------
---- ein neues Bild Laden --
----------------------------
--
---- aktuelles bild ist fertig
--if ad_adr = "111111111111111111" and ad_wr = '1' then  
--
---- filename zum laden 
--	
--	picture_counter := picture_counter + 1;
--	
--   w1 := picture_counter / 100;
--	w2 := ((picture_counter - (w1*100)) / 10);
--	w3 := picture_counter - (w1*100) - (w2*10); 
--	
--	char1 := character'VAL(conv_integer((w1)+48));
--	char2 := character'VAL(conv_integer((w2)+48));
--	char3 := character'VAL(conv_integer((w3)+48));
--  
--   wname := "wurfel0" & char1 & char2 & char3 & ".bmp";
--
--	report " ----------------------------------------- Lade ein neues Bild ----------------------------";
--	report wname;
--	readfile (wname); -- Datei Laden
--	
--	-- und in den speicher input_picture kopieren
--	for ybc in 0 to 511 loop
--	 	for xbc in 0 to 511 loop
--		wait for 1 ps;           
--		getpixel (xbc,ybc,colordatak);
--		xk <= xbc ; yk <= ybc;
--		
--		input_picture(xk,yk) := To_bitvector(colordatak) (23 downto 19) &
--										To_bitvector(colordatak) (15 downto 10) &
--										To_bitvector(colordatak) ( 7 downto  3) ;
--		end loop;
--	end loop;
--		
--		
--
--end if;
--
--
--if x >= 512 then x := x - 512; y := y + 1; end if;
--if y >= 512 then y := 0; end if;
--
--		
--
--end process;
--	
--	

------------------------------------------
--  Simulierte Drehung des Ventilators  --
------------------------------------------

process 
begin

wait for 20 us;
winkel <= winkel + 1;
end process;
-- 20 us = 3000 u/min
-- 30 us = 2000 u/min 
-- 60 us = 1000 u/min


----------------------------------
-- Bilder erzeugen & speichern  --
----------------------------------

process

variable char1,char2,char3 : character ;
variable filename_counter : integer := 0;
variable x,y,z : integer;
variable r,g,b : integer;


constant dunkler : integer := 1;

			
begin

-- ein erstes start bild laden
wait for 10 ns;
    ReadFile("wurfel0000.bmp");
wait for 10 ns;

-- und in den speicher input_picture kopieren
	for ybl in 0 to 511 loop
	 	for xbl in 0 to 511 loop
		wait for 1 ps;           
		getpixel (xbl,ybl,colordatal);
		xl <= xbl ; yl <= ybl;
		
		input_picture(xl,yl) := To_bitvector(colordatal) (23 downto 19) &
										To_bitvector(colordatal) (15 downto 10) &
										To_bitvector(colordatal) ( 7 downto  3) ;
		end loop;
	end loop;


	
-- bild in den ausgabe puffer kopieren 	
--	for cy in 0 to 511 loop
--		for cx in 0 to 511 loop
--		wait for 1 ps;           -- WTF ???
--		getpixel (cx,cy,colordataf);
--		xf <= cx ; yf <= cy;
--		setpixel (xf,yf,colordataf);
--		end loop;
--	end loop;
--	
--wait for 10 ns;	
--
--
---- und das bild wieder speichern
--	fname := "lena_copy000.bmp";
--	writefile (fname);
	 
		
		 
--nach einer eingestellten zeit ein neues Bild speichern 
wait for 100 us;

for xxx in 1 to 999 loop

	wait for 100 us; -- Zeit bis zum nächsten bild 200 | 5000
		
   report "Jetzt ist das Bild fertig...";

	for ny in 0 to 511 loop
		for nx in 0 to 511 loop
		wait for 1 ps;
		colordatai <= To_StdLogicVector(visual ((nx),ny) (15 downto 11)) & "000" &
						  To_StdLogicVector(visual ((nx),ny) (10 downto  5)) & "00"  &
						  To_StdLogicVector(visual ((nx),ny) ( 4 downto  0)) & "000";
								
		xi <= nx ; yi <= ny;
		setpixel (xi,yi,(colordatai));
		end loop;
	end loop;

	
	filename_counter := filename_counter + 1;
	
	x := filename_counter / 100;
	y := ((filename_counter - (x*100)) / 10);
	z := filename_counter - (x*100) - (y*10); 
	
	char1 := character'VAL(conv_integer((x)+48));
	char2 := character'VAL(conv_integer((y)+48));
	char3 := character'VAL(conv_integer((z)+48));
  
   fname := "lena_copy" & char1 & char2 & char3 & ".bmp";
	
	
	report fname;
	WriteFile (fname);


end loop;
wait;
end process;
END;
