----------------------------------------------------------------------------
-- Simple simulation models for Xilinx block rams
-- Author: Jiri Gaisler
----------------------------------------------------------------------------

-- pragma translate_off

-- simulation models for block-rams

library ieee;
use ieee.std_logic_1164.all;
library unisim;
use unisim.simple_simprim.all;

entity RAMB4_S16 is
  port (
    do   : out std_logic_vector (15 downto 0);
    addr : in  std_logic_vector (7 downto 0);
    clk  : in  std_ulogic;
    di   : in  std_logic_vector (15 downto 0);
    en, rst, we : in std_ulogic);
end;
architecture behav of RAMB4_S16 is
begin x : ramb4_generic generic map (8,16)
          port map (di, en, we, rst, clk, addr, do); 
end;

library ieee;
use ieee.std_logic_1164.all;
library unisim;
use unisim.simple_simprim.all;

entity RAMB4_S8 is
  port (do   : out std_logic_vector (7 downto 0);
        addr : in  std_logic_vector (8 downto 0);
        clk  : in  std_ulogic;
        di   : in  std_logic_vector (7 downto 0);
        en, rst, we : in std_ulogic);
end;
architecture behav of RAMB4_S8 is
begin x : ramb4_generic generic map (9,8)
          port map (di, en, we, rst, clk, addr, do); 
end;

library ieee;
use ieee.std_logic_1164.all;
library unisim;
use unisim.simple_simprim.all;

entity RAMB4_S4 is
  port (do   : out std_logic_vector (3 downto 0);
        addr : in  std_logic_vector (9 downto 0);
        clk  : in  std_ulogic;
        di   : in  std_logic_vector (3 downto 0);
        en, rst, we : in std_ulogic);
end;
architecture behav of RAMB4_S4 is
begin x : ramb4_generic generic map (10,4)
          port map (di, en, we, rst, clk, addr, do); 
end;

library ieee;
use ieee.std_logic_1164.all;
library unisim;
use unisim.simple_simprim.all;

entity RAMB4_S2 is
  port (do   : out std_logic_vector (1 downto 0);
        addr : in  std_logic_vector (10 downto 0);
        clk  : in  std_ulogic;
        di   : in  std_logic_vector (1 downto 0);
        en, rst, we : in std_ulogic);
end;
architecture behav of RAMB4_S2 is
begin x : ramb4_generic generic map (11,2)
          port map (di, en, we, rst, clk, addr, do); 
end;

library ieee;
use ieee.std_logic_1164.all;
library unisim;
use unisim.simple_simprim.all;

entity RAMB4_S1 is
  port (do   : out std_logic_vector (0 downto 0);
        addr : in  std_logic_vector (11 downto 0);
        clk  : in  std_ulogic;
        di   : in  std_logic_vector (0 downto 0);
        en, rst, we : in std_ulogic);
end;
architecture behav of RAMB4_S1 is
begin x : ramb4_generic generic map (12,1)
          port map (di, en, we, rst, clk, addr, do); 
end;

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity RAMB4_SX_SX is
  generic (abits : integer := 10; dbits : integer := 8 );
  port (DIA    : in std_logic_vector (dbits-1 downto 0);
        DIB    : in std_logic_vector (dbits-1 downto 0);
        ENA    : in std_ulogic;
        ENB    : in std_ulogic;
        WEA    : in std_ulogic;
        WEB    : in std_ulogic;
        RSTA   : in std_ulogic;
        RSTB   : in std_ulogic;
        CLKA   : in std_ulogic;
        CLKB   : in std_ulogic;
        ADDRA  : in std_logic_vector (abits-1 downto 0);
        ADDRB  : in std_logic_vector (abits-1 downto 0);
        DOA    : out std_logic_vector (dbits-1 downto 0);
        DOB    : out std_logic_vector (dbits-1 downto 0)
       );
end;
architecture behav of RAMB4_SX_SX is
begin
  rp : process(clka, clkb)
  subtype dword is std_logic_vector(dbits-1 downto 0);
  type dregtype is array (0 to 2**abits-1) of DWord;
  variable rfd : dregtype;
  begin
    if rising_edge(clka) and not is_x (addra) then 
      if ena = '1' then
        doa <= rfd(to_integer(unsigned(addra)));
        if wea = '1' then rfd(to_integer(unsigned(addra))) := dia; end if;
      end if;
    end if;
    if rising_edge(clkb) and not is_x (addrb) then 
      if enb = '1' then
        dob <= rfd(to_integer(unsigned(addrb)));
        if web = '1' then rfd(to_integer(unsigned(addrb))) := dib; end if;
      end if;
    end if;
  end process;
end;

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
library unisim;
use unisim.simple_simprim.all;

entity RAMB4_S1_S1 is
  port (
        doa    : out std_logic_vector (0 downto 0);
        dob    : out std_logic_vector (0 downto 0);
	addra  : in  std_logic_vector (11 downto 0);
	addrb  : in  std_logic_vector (11 downto 0);
	clka   : in  std_ulogic;
	clkb   : in  std_ulogic;
	dia    : in  std_logic_vector (0 downto 0);
	dib    : in  std_logic_vector (0 downto 0);
	ena    : in  std_ulogic;
	enb    : in  std_ulogic;
	rsta   : in  std_ulogic;
	rstb   : in  std_ulogic;
	wea    : in  std_ulogic;
	web    : in  std_ulogic
       );
end;
architecture behav of RAMB4_S1_S1 is
begin
  u0 : RAMB4_Sx_Sx generic map (12, 1)
       port map (DIA, DIB, ENA, ENB, WEA, WEB, RSTA, RSTB, CLKA, CLKB, ADDRA,
       ADDRB, DOA, DOB);
end;

library ieee;
use ieee.std_logic_1164.all;
library unisim;
use unisim.simple_simprim.all;

entity RAMB4_S2_S2 is
  port (
        doa    : out std_logic_vector (1 downto 0);
        dob    : out std_logic_vector (1 downto 0);
	addra  : in  std_logic_vector (10 downto 0);
	addrb  : in  std_logic_vector (10 downto 0);
	clka   : in  std_ulogic;
	clkb   : in  std_ulogic;
	dia    : in  std_logic_vector (1 downto 0);
	dib    : in  std_logic_vector (1 downto 0);
	ena    : in  std_ulogic;
	enb    : in  std_ulogic;
	rsta   : in  std_ulogic;
	rstb   : in  std_ulogic;
	wea    : in  std_ulogic;
	web    : in  std_ulogic
       );
end;
architecture behav of RAMB4_S2_S2 is
begin
  u0 : RAMB4_Sx_Sx generic map (11, 2)
       port map (DIA, DIB, ENA, ENB, WEA, WEB, RSTA, RSTB, CLKA, CLKB, ADDRA,
       ADDRB, DOA, DOB);
end;

library ieee;
use ieee.std_logic_1164.all;
library unisim;
use unisim.simple_simprim.all;

entity RAMB4_S8_S8 is
  port (
        doa    : out std_logic_vector (7 downto 0);
        dob    : out std_logic_vector (7 downto 0);
	addra  : in  std_logic_vector (8 downto 0);
	addrb  : in  std_logic_vector (8 downto 0);
	clka   : in  std_ulogic;
	clkb   : in  std_ulogic;
	dia    : in  std_logic_vector (7 downto 0);
	dib    : in  std_logic_vector (7 downto 0);
	ena    : in  std_ulogic;
	enb    : in  std_ulogic;
	rsta   : in  std_ulogic;
	rstb   : in  std_ulogic;
	wea    : in  std_ulogic;
	web    : in  std_ulogic
       );
end;

architecture behav of RAMB4_S8_S8 is
begin
  u0 : RAMB4_Sx_Sx generic map (9, 8)
       port map (DIA, DIB, ENA, ENB, WEA, WEB, RSTA, RSTB, CLKA, CLKB, ADDRA,
       ADDRB, DOA, DOB);
end;

library ieee;
use ieee.std_logic_1164.all;
library unisim;
use unisim.simple_simprim.all;

entity RAMB4_S4_S4 is
  port (
        doa    : out std_logic_vector (3 downto 0);
        dob    : out std_logic_vector (3 downto 0);
	addra  : in  std_logic_vector (9 downto 0);
	addrb  : in  std_logic_vector (9 downto 0);
	clka   : in  std_ulogic;
	clkb   : in  std_ulogic;
	dia    : in  std_logic_vector (3 downto 0);
	dib    : in  std_logic_vector (3 downto 0);
	ena    : in  std_ulogic;
	enb    : in  std_ulogic;
	rsta   : in  std_ulogic;
	rstb   : in  std_ulogic;
	wea    : in  std_ulogic;
	web    : in  std_ulogic
       );
end;
architecture behav of RAMB4_S4_S4 is
begin
  u0 : RAMB4_Sx_Sx generic map (4, 10)
       port map (DIA, DIB, ENA, ENB, WEA, WEB, RSTA, RSTB, CLKA, CLKB, ADDRA,
       ADDRB, DOA, DOB);
end;

library ieee;
use ieee.std_logic_1164.all;
library unisim;
use unisim.simple_simprim.all;

entity RAMB4_S16_S16 is
  port (
        doa    : out std_logic_vector (15 downto 0);
        dob    : out std_logic_vector (15 downto 0);
	addra  : in  std_logic_vector (7 downto 0);
	addrb  : in  std_logic_vector (7 downto 0);
	clka   : in  std_ulogic;
	clkb   : in  std_ulogic;
	dia    : in  std_logic_vector (15 downto 0);
	dib    : in  std_logic_vector (15 downto 0);
	ena    : in  std_ulogic;
	enb    : in  std_ulogic;
	rsta   : in  std_ulogic;
	rstb   : in  std_ulogic;
	wea    : in  std_ulogic;
	web    : in  std_ulogic
       );
end;
architecture behav of RAMB4_S16_S16 is
begin
  u0 : RAMB4_Sx_Sx generic map (8, 16)
       port map (DIA, DIB, ENA, ENB, WEA, WEB, RSTA, RSTB, CLKA, CLKB, ADDRA,
       ADDRB, DOA, DOB);
end;

library ieee;
use ieee.std_logic_1164.all;
library unisim;
use unisim.simple_simprim.all;

entity RAMB16_S1 is
 port (
   DO : out std_logic_vector (0 downto 0);
   ADDR : in std_logic_vector (13 downto 0);
   CLK : in std_ulogic;
   DI : in std_logic_vector (0 downto 0);
   EN : in std_ulogic;
   SSR : in std_ulogic;
   WE : in std_ulogic
 );
end;
architecture behav of RAMB16_S1 is
begin x : ramb16_sx generic map (14,1)
          port map (do, addr, di, en, clk, we, ssr); 
end;

library ieee;
use ieee.std_logic_1164.all;
library unisim;
use unisim.simple_simprim.all;

entity RAMB16_S2 is
 port (
   DO : out std_logic_vector (1 downto 0);
   ADDR : in std_logic_vector (12 downto 0);
   CLK : in std_ulogic;
   DI : in std_logic_vector (1 downto 0);
   EN : in std_ulogic;
   SSR : in std_ulogic;
   WE : in std_ulogic
 );
end;
architecture behav of RAMB16_S2 is
begin x : ramb16_sx generic map (13,2)
          port map (do, addr, di, en, clk, we, ssr); 
end;

library ieee;
use ieee.std_logic_1164.all;
library unisim;
use unisim.simple_simprim.all;

entity RAMB16_S4 is
 port (
   DO : out std_logic_vector (3 downto 0);
   ADDR : in std_logic_vector (11 downto 0);
   CLK : in std_ulogic;
   DI : in std_logic_vector (3 downto 0);
   EN : in std_ulogic;
   SSR : in std_ulogic;
   WE : in std_ulogic
 );
end;
architecture behav of RAMB16_S4 is
begin x : ramb16_sx generic map (12,4)
          port map (do, addr, di, en, clk, we, ssr); 
end;

library ieee;
use ieee.std_logic_1164.all;
library unisim;
use unisim.simple_simprim.all;

entity RAMB16_S9 is
 port (
   DO : out std_logic_vector (7 downto 0);
   DOP : out std_logic_vector (0 downto 0);
   ADDR : in std_logic_vector (10 downto 0);
   CLK : in std_ulogic;
   DI : in std_logic_vector (7 downto 0);
   DIP : in std_logic_vector (0 downto 0);
   EN : in std_ulogic;
   SSR : in std_ulogic;
   WE : in std_ulogic
 );
end;
architecture behav of RAMB16_S9 is
signal dix, dox : std_logic_vector (8 downto 0);
begin x : ramb16_sx generic map (11,9)
          port map (dox, addr, dix, en, clk, we, ssr); 
  dix <= dip & di; dop <= dox(8 downto 8); do <= dox(7 downto 0);
end;

library ieee;
use ieee.std_logic_1164.all;
library unisim;
use unisim.simple_simprim.all;

entity RAMB16_S18 is
 port (
    DO : out std_logic_vector (15 downto 0);
    DOP : out std_logic_vector (1 downto 0);
    ADDR : in std_logic_vector (9 downto 0);
    CLK : in std_ulogic;
    DI : in std_logic_vector (15 downto 0);
    DIP : in std_logic_vector (1 downto 0);
    EN : in std_ulogic;
    SSR : in std_ulogic;
    WE : in std_ulogic
 );
end;
architecture behav of RAMB16_S18 is
signal dix, dox : std_logic_vector (17 downto 0);
begin x : ramb16_sx generic map (10,18)
          port map (dox, addr, dix, en, clk, we, ssr); 
  dix <= dip & di; dop <= dox(17 downto 16); do <= dox(15 downto 0);
end;

library ieee;
use ieee.std_logic_1164.all;
library unisim;
use unisim.simple_simprim.all;

entity RAMB16_S36 is
 port (
   DO : out std_logic_vector (31 downto 0);
   DOP : out std_logic_vector (3 downto 0);
   ADDR : in std_logic_vector (8 downto 0);
   CLK : in std_ulogic;
   DI : in std_logic_vector (31 downto 0);
   DIP : in std_logic_vector (3 downto 0);
   EN : in std_ulogic;
   SSR : in std_ulogic;
   WE : in std_ulogic
 );
end;
architecture behav of RAMB16_S36 is
signal dix, dox : std_logic_vector (35 downto 0);
begin x : ramb16_sx generic map (9, 36)
          port map (dox, addr, dix, en, clk, we, ssr); 
  dix <= dip & di; dop <= dox(35 downto 32); do <= dox(31 downto 0);
end;


library ieee;
use ieee.std_logic_1164.all;
library unisim;
use unisim.simple_simprim.all;

entity RAMB16_S1_S1 is
  port (
   DOA : out std_logic_vector (0 downto 0);
   DOB : out std_logic_vector (0 downto 0);
   ADDRA : in std_logic_vector (13 downto 0);
   ADDRB : in std_logic_vector (13 downto 0);
   CLKA : in std_ulogic;
   CLKB : in std_ulogic;
   DIA : in std_logic_vector (0 downto 0);
   DIB : in std_logic_vector (0 downto 0);
   ENA : in std_ulogic;
   ENB : in std_ulogic;
   SSRA : in std_ulogic;
   SSRB : in std_ulogic;
   WEA : in std_ulogic;
   WEB : in std_ulogic
  );
end;
architecture behav of RAMB16_S1_S1 is
begin 
  x : ram16_sx_sx generic map (14, 1)
  port map (doa, dob, addra, clka, dia, ena, wea, addrb, clkb, dib, enb, web); 
end;

library ieee;
use ieee.std_logic_1164.all;
library unisim;
use unisim.simple_simprim.all;

entity RAMB16_S2_S2 is
  port (
   DOA : out std_logic_vector (1 downto 0);
   DOB : out std_logic_vector (1 downto 0);
   ADDRA : in std_logic_vector (12 downto 0);
   ADDRB : in std_logic_vector (12 downto 0);
   CLKA : in std_ulogic;
   CLKB : in std_ulogic;
   DIA : in std_logic_vector (1 downto 0);
   DIB : in std_logic_vector (1 downto 0);
   ENA : in std_ulogic;
   ENB : in std_ulogic;
   SSRA : in std_ulogic;
   SSRB : in std_ulogic;
   WEA : in std_ulogic;
   WEB : in std_ulogic
  );
end;
architecture behav of RAMB16_S2_S2 is
begin 
  x : ram16_sx_sx generic map (13, 2)
  port map (doa, dob, addra, clka, dia, ena, wea, addrb, clkb, dib, enb, web); 
end;


library ieee;
use ieee.std_logic_1164.all;
library unisim;
use unisim.simple_simprim.all;

entity RAMB16_S4_S4 is
  port (
   DOA : out std_logic_vector (3 downto 0);
   DOB : out std_logic_vector (3 downto 0);
   ADDRA : in std_logic_vector (11 downto 0);
   ADDRB : in std_logic_vector (11 downto 0);
   CLKA : in std_ulogic;
   CLKB : in std_ulogic;
   DIA : in std_logic_vector (3 downto 0);
   DIB : in std_logic_vector (3 downto 0);
   ENA : in std_ulogic;
   ENB : in std_ulogic;
   SSRA : in std_ulogic;
   SSRB : in std_ulogic;
   WEA : in std_ulogic;
   WEB : in std_ulogic
  );
end;
architecture behav of RAMB16_S4_S4 is
begin 
  x : ram16_sx_sx generic map (12, 4)
  port map (doa, dob, addra, clka, dia, ena, wea, addrb, clkb, dib, enb, web); 
end;

library ieee;
use ieee.std_logic_1164.all;
library unisim;
use unisim.simple_simprim.all;

entity RAMB16_S9_S9 is
  port (
   DOA : out std_logic_vector (7 downto 0);
   DOB : out std_logic_vector (7 downto 0);
   DOPA : out std_logic_vector (0 downto 0);
   DOPB : out std_logic_vector (0 downto 0);
   ADDRA : in std_logic_vector (10 downto 0);
   ADDRB : in std_logic_vector (10 downto 0);
   CLKA : in std_ulogic;
   CLKB : in std_ulogic;
   DIA : in std_logic_vector (7 downto 0);
   DIB : in std_logic_vector (7 downto 0);
   DIPA : in std_logic_vector (0 downto 0);
   DIPB : in std_logic_vector (0 downto 0);
   ENA : in std_ulogic;
   ENB : in std_ulogic;
   SSRA : in std_ulogic;
   SSRB : in std_ulogic;
   WEA : in std_ulogic;
   WEB : in std_ulogic
 );
end;
architecture behav of RAMB16_S9_S9 is
signal diax, doax, dibx, dobx : std_logic_vector (8 downto 0);
begin 
  x : ram16_sx_sx generic map (11, 9)
  port map (doax, dobx, addra, clka, diax, ena, wea, addrb, clkb, dibx, enb, web); 
  diax <= dipa & dia; dopa <= doax(8 downto 8); doa <= doax(7 downto 0);
  dibx <= dipb & dib; dopb <= dobx(8 downto 8); dob <= dobx(7 downto 0);
end;

library ieee;
use ieee.std_logic_1164.all;
library unisim;
use unisim.simple_simprim.all;

entity RAMB16_S18_S18 is
  port (
    DOA : out std_logic_vector (15 downto 0);
    DOB : out std_logic_vector (15 downto 0);
    DOPA : out std_logic_vector (1 downto 0);
    DOPB : out std_logic_vector (1 downto 0);
    ADDRA : in std_logic_vector (9 downto 0);
    ADDRB : in std_logic_vector (9 downto 0);
    CLKA : in std_ulogic;
    CLKB : in std_ulogic;
    DIA : in std_logic_vector (15 downto 0);
    DIB : in std_logic_vector (15 downto 0);
    DIPA : in std_logic_vector (1 downto 0);
    DIPB : in std_logic_vector (1 downto 0);
    ENA : in std_ulogic;
    ENB : in std_ulogic;
    SSRA : in std_ulogic;
    SSRB : in std_ulogic;
    WEA : in std_ulogic;
    WEB : in std_ulogic);
end;
architecture behav of RAMB16_S18_S18 is
signal diax, doax, dibx, dobx : std_logic_vector (17 downto 0);
begin 
  x : ram16_sx_sx generic map (10, 18)
  port map (doax, dobx, addra, clka, diax, ena, wea, addrb, clkb, dibx, enb, web); 
  diax <= dipa & dia; dopa <= doax(17 downto 16); doa <= doax(15 downto 0);
  dibx <= dipb & dib; dopb <= dobx(17 downto 16); dob <= dobx(15 downto 0);
end;

library ieee;
use ieee.std_logic_1164.all;
library unisim;
use unisim.simple_simprim.all;

entity RAMB16_S36_S36 is
  port (
    DOA : out std_logic_vector (31 downto 0);
    DOB : out std_logic_vector (31 downto 0);
    DOPA : out std_logic_vector (3 downto 0);
    DOPB : out std_logic_vector (3 downto 0);
    ADDRA : in std_logic_vector (8 downto 0);
    ADDRB : in std_logic_vector (8 downto 0);
    CLKA : in std_ulogic;
    CLKB : in std_ulogic;
    DIA : in std_logic_vector (31 downto 0);
    DIB : in std_logic_vector (31 downto 0);
    DIPA : in std_logic_vector (3 downto 0);
    DIPB : in std_logic_vector (3 downto 0);
    ENA : in std_ulogic;
    ENB : in std_ulogic;
    SSRA : in std_ulogic;
    SSRB : in std_ulogic;
    WEA : in std_ulogic;
    WEB : in std_ulogic);
end;
architecture behav of RAMB16_S36_S36 is
signal diax, doax, dibx, dobx : std_logic_vector (35 downto 0);
begin 
  x : ram16_sx_sx generic map (9, 36)
  port map (doax, dobx, addra, clka, diax, ena, wea, addrb, clkb, dibx, enb, web); 
  diax <= dipa & dia; dopa <= doax(35 downto 32); doa <= doax(31 downto 0);
  dibx <= dipb & dib; dopb <= dobx(35 downto 32); dob <= dobx(31 downto 0);
end;

-- pragma translate_on
