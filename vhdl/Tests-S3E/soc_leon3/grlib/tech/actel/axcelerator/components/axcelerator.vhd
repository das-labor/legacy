
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity RAM64K36 is port(
        DEPTH3, DEPTH2, DEPTH1, DEPTH0,
        WRAD15, WRAD14, WRAD13, WRAD12, WRAD11, WRAD10, WRAD9 ,
        WRAD8 , WRAD7 , WRAD6 , WRAD5 , WRAD4 , WRAD3 , WRAD2 ,
        WRAD1 , WRAD0 , WD35  , WD34  , WD33  , WD32  , WD31  ,
        WD30  , WD29  , WD28  , WD27  , WD26  , WD25  , WD24  ,
        WD23  , WD22  , WD21  , WD20  , WD19  , WD18  , WD17  ,
        WD16  , WD15  , WD14  , WD13  , WD12  , WD11  , WD10  ,
        WD9   , WD8   , WD7   , WD6   , WD5   , WD4   , WD3   ,
        WD2   , WD1   , WD0   , WW2   , WW1   , WW0   , WEN   ,
        WCLK  , RDAD15, RDAD14, RDAD13, RDAD12, RDAD11, RDAD10,
        RDAD9 , RDAD8 , RDAD7 , RDAD6 , RDAD5 , RDAD4 , RDAD3 ,
        RDAD2 , RDAD1 , RDAD0 , RW2   , RW1   , RW0   , REN   ,
        RCLK   : in  std_ulogic ;
        RD35  , RD34  , RD33  , RD32  , RD31  , RD30  , RD29  ,
        RD28  , RD27  , RD26  , RD25  , RD24  , RD23  , RD22  ,
        RD21  , RD20  , RD19  , RD18  , RD17  , RD16  , RD15  ,
        RD14  , RD13  , RD12  , RD11  , RD10  , RD9   , RD8   ,
        RD7   , RD6   , RD5   , RD4   , RD3   , RD2   , RD1   ,
        RD0    : out std_ulogic);
end;

architecture rtl of RAM64K36 is
  signal re : std_ulogic;
begin

  rp : process(RCLK, WCLK)
  constant words : integer := 2**16;
  subtype word is std_logic_vector(35 downto 0);
  type dregtype is array (0 to words - 1) of word;
  variable rfd : dregtype;
  variable wa, ra : std_logic_vector(15 downto 0);
  variable q : std_logic_vector(35 downto 0);
  variable first : boolean := true;
  begin
--    assert not first report "RAM64K" severity note;
--    first := false;

    if rising_edge(RCLK) then
      ra := RDAD15 & RDAD14 & RDAD13 & RDAD12 & RDAD11 & RDAD10 & RDAD9 &
            RDAD8 & RDAD7 & RDAD6 & RDAD5 & RDAD4 & RDAD3 & RDAD2 & RDAD1 & RDAD0;
      if not (is_x (ra)) and REN = '1' then 
        q := rfd(to_integer(unsigned(ra)) mod words);
      else q := (others => 'X'); end if;
    end if;
    if rising_edge(WCLK) and (wen = '1') then
      wa := WRAD15 & WRAD14 & WRAD13 & WRAD12 & WRAD11 & WRAD10 & WRAD9 &
            WRAD8 & WRAD7 & WRAD6 & WRAD5 & WRAD4 & WRAD3 & WRAD2 & WRAD1 & WRAD0;
      if not is_x (wa) then 
   	rfd(to_integer(unsigned(wa)) mod words) :=
          WD35 & WD34 & WD33 & WD32 & WD31 & WD30 & WD29 & WD28 & WD27 &
	  WD26 & WD25 & WD24 & WD23 & WD22 & WD21 & WD20 & WD19 & WD18 &
	  WD17 & WD16 & WD15 & WD14 & WD13 & WD12 & WD11 & WD10 & WD9 &
	  WD8 & WD7 & WD6 & WD5 & WD4 & WD3 & WD2 & WD1 & WD0;
      end if;
    end if;
    RD35 <= q(35); RD34 <= q(34); RD33 <= q(33); RD32 <= q(32); RD31 <= q(31);
    RD30 <= q(30); RD29 <= q(29); RD28 <= q(28); RD27 <= q(27); RD26 <= q(26);
    RD25 <= q(25); RD24 <= q(24); RD23 <= q(23); RD22 <= q(22); RD21 <= q(21);
    RD20 <= q(20); RD19 <= q(19); RD18 <= q(18); RD17 <= q(17); RD16 <= q(16);
    RD15 <= q(15); RD14 <= q(14); RD13 <= q(13); RD12 <= q(12); RD11 <= q(11);
    RD10 <= q(10); RD9 <= q(9); RD8 <= q(8); RD7 <= q(7); RD6 <= q(6);
    RD5 <= q(5); RD4 <= q(4); RD3 <= q(3); RD2 <= q(2); RD1 <= q(1);
    RD0 <= q(0);
  end process;
end;

-- PCI PADS ----------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
entity hclkbuf_pci is port( pad : in  std_logic; y   : out std_logic); end; 
architecture struct of hclkbuf_pci is begin y <= to_X01(pad); end;

library ieee;
use ieee.std_logic_1164.all;
entity clkbuf_pci is port( pad : in  std_logic; y   : out std_logic); end; 
architecture struct of clkbuf_pci is begin y <= to_X01(pad); end;

library ieee;
use ieee.std_logic_1164.all;
entity inbuf_pci is port( pad : in  std_logic; y   : out std_logic); end; 
architecture struct of inbuf_pci is begin y <= to_X01(pad) after 2 ns; end;

library ieee;
use ieee.std_logic_1164.all;
entity bibuf_pci is port
  (d, e : in  std_logic; pad : inout std_logic; y : out std_logic);
end; 
architecture struct of bibuf_pci is begin 
  y <= to_X01(pad) after 2 ns;
  pad <= d after 5 ns when to_X01(e) = '1' else
	 'Z' after 5 ns when to_X01(e) = '0' else 'X' after 5 ns;
end;

library ieee;
use ieee.std_logic_1164.all;
entity tribuff_pci is port (d, e : in  std_logic; pad : out std_logic ); end; 
architecture struct of tribuff_pci is begin 
  pad <= d after 5 ns when to_X01(e) = '1' else
	 'Z' after 5 ns when to_X01(e) = '0' else 'X' after 5 ns;
end;

library ieee;
use ieee.std_logic_1164.all;
entity outbuf_pci is port (d : in  std_logic; pad : out std_logic ); end; 
architecture struct of outbuf_pci is begin pad <= d after 5 ns; end;

-- STANDARD PADS ----------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
entity clkbuf is port( pad : in  std_logic; y   : out std_logic); end; 
architecture struct of clkbuf is begin y <= to_X01(pad); end;

library ieee;
use ieee.std_logic_1164.all;
entity hclkbuf is port( pad : in  std_logic; y   : out std_logic); end; 
architecture struct of hclkbuf is begin y <= to_X01(pad); end;

library ieee;
use ieee.std_logic_1164.all;
entity inbuf is port( pad : in  std_logic; y   : out std_logic); end; 
architecture struct of inbuf is begin y <= to_X01(pad) after 1 ns; end;

library ieee;
use ieee.std_logic_1164.all;
entity bibuf is port
  (d, e : in  std_logic; pad : inout std_logic; y : out std_logic);
end; 
architecture struct of bibuf is begin 
  y <= to_X01(pad) after 2 ns;
  pad <= d after 2 ns when to_X01(e) = '1' else
	 'Z' after 2 ns when to_X01(e) = '0' else 'X' after 2 ns;
end;

library ieee;
use ieee.std_logic_1164.all;
entity tribuff is port (d, e : in  std_logic; pad : out std_logic ); end; 
architecture struct of tribuff is begin 
  pad <= d after 2 ns when to_X01(e) = '1' else
	 'Z' after 2 ns when to_X01(e) = '0' else 'X' after 2 ns;
end;

library ieee;
use ieee.std_logic_1164.all;
entity outbuf is port (d : in  std_logic; pad : out std_logic ); end; 
architecture struct of outbuf is begin pad <= d after 2 ns; end;

library ieee;
use ieee.std_logic_1164.all;
entity outbuf_f_8 is port (d : in  std_logic; pad : out std_logic ); end; 
architecture struct of outbuf_f_8 is begin pad <= d after 2 ns; end;

library ieee;
use ieee.std_logic_1164.all;
entity outbuf_f_12 is port (d : in  std_logic; pad : out std_logic ); end; 
architecture struct of outbuf_f_12 is begin pad <= d after 2 ns; end;

library ieee;
use ieee.std_logic_1164.all;
entity outbuf_f_16 is port (d : in  std_logic; pad : out std_logic ); end; 
architecture struct of outbuf_f_16 is begin pad <= d after 2 ns; end;

library ieee;
use ieee.std_logic_1164.all;
entity outbuf_f_24 is port (d : in  std_logic; pad : out std_logic ); end; 
architecture struct of outbuf_f_24 is begin pad <= d after 2 ns; end;

library ieee;
use ieee.std_logic_1164.all;
entity inbuf_lvds is port( y : out std_logic; padp, padn : in  std_logic); end; 
architecture struct of inbuf_lvds is 
signal yn : std_ulogic := '0';
begin 
  yn <= to_X01(padp) after 1 ns when to_x01(padp xor padn) = '1' else yn after 1 ns;
  y <= yn;
end;

library ieee;
use ieee.std_logic_1164.all;
entity outbuf_lvds is port (d : in  std_logic; padp, padn : out std_logic ); end; 
architecture struct of outbuf_lvds is begin 
  padp <= d after 1 ns; 
  padn <= not d after 1 ns; 
end;

-- clock buffers ----------------------

library ieee;
use ieee.std_logic_1164.all;
entity hclkint is port( a : in  std_logic; y   : out std_logic); end; 
architecture struct of hclkint is begin y <= to_X01(a); end;

library ieee;
use ieee.std_logic_1164.all;
entity clkint is port( a : in  std_logic; y   : out std_logic); end; 
architecture struct of clkint is begin y <= to_X01(a); end;
