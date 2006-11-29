----------------------------------------------------------------------------
-- Simple simulation models for some Xilinx blocks
-- Author: Jiri Gaisler
----------------------------------------------------------------------------

-- pragma translate_off

library ieee;
use ieee.std_logic_1164.all;
library STD;
use STD.TEXTIO.all;

package simple_simprim_pck is
  PROCEDURE GenericValueCheckMessage (
    CONSTANT HeaderMsg      : IN STRING := " Attribute Syntax Error ";        
    CONSTANT GenericName : IN STRING := "";
    CONSTANT EntityName : IN STRING := "";
    CONSTANT InstanceName : IN STRING := "";
    CONSTANT GenericValue : IN STRING := "";
    Constant Unit : IN STRING := "";
    Constant ExpectedValueMsg : IN STRING := "";
    Constant ExpectedGenericValue : IN STRING := "";
    CONSTANT TailMsg      : IN STRING;                    

    CONSTANT MsgSeverity    : IN SEVERITY_LEVEL := WARNING
    
    );

  PROCEDURE GenericValueCheckMessage (
    CONSTANT HeaderMsg      : IN STRING := " Attribute Syntax Error ";        
    CONSTANT GenericName : IN STRING := "";
    CONSTANT EntityName : IN STRING := "";
    CONSTANT InstanceName : IN STRING := "";
    CONSTANT GenericValue : IN INTEGER;
    Constant Unit : IN STRING := "";
    Constant ExpectedValueMsg : IN STRING := "";
    Constant ExpectedGenericValue : IN INTEGER;
    CONSTANT TailMsg      : IN STRING;                    

    CONSTANT MsgSeverity    : IN SEVERITY_LEVEL := WARNING
    
    );

  PROCEDURE GenericValueCheckMessage (
    CONSTANT HeaderMsg      : IN STRING := " Attribute Syntax Error ";        
    CONSTANT GenericName : IN STRING := "";
    CONSTANT EntityName : IN STRING := "";
    CONSTANT InstanceName : IN STRING := "";
    CONSTANT GenericValue : IN BOOLEAN;
    Constant Unit : IN STRING := "";
    Constant ExpectedValueMsg : IN STRING := "";
    Constant ExpectedGenericValue : IN STRING := "";
    CONSTANT TailMsg      : IN STRING;                    

    CONSTANT MsgSeverity    : IN SEVERITY_LEVEL := WARNING
    
    );   

  PROCEDURE GenericValueCheckMessage (
    CONSTANT HeaderMsg      : IN STRING := " Attribute Syntax Error ";        
    CONSTANT GenericName : IN STRING := "";
    CONSTANT EntityName : IN STRING := "";
    CONSTANT InstanceName : IN STRING := "";
    CONSTANT GenericValue : IN INTEGER;
    CONSTANT Unit : IN STRING := "";
    CONSTANT ExpectedValueMsg : IN STRING := "";
    CONSTANT ExpectedGenericValue : IN STRING := "";
    CONSTANT TailMsg      : IN STRING;                    
    CONSTANT MsgSeverity    : IN SEVERITY_LEVEL := WARNING
    );

  PROCEDURE GenericValueCheckMessage (
    CONSTANT HeaderMsg      : IN STRING := " Attribute Syntax Error ";        
    CONSTANT GenericName : IN STRING := "";
    CONSTANT EntityName : IN STRING := "";
    CONSTANT InstanceName : IN STRING := "";
    CONSTANT GenericValue : IN REAL;
    CONSTANT Unit : IN STRING := "";
    CONSTANT ExpectedValueMsg : IN STRING := "";
    CONSTANT ExpectedGenericValue : IN STRING := "";
    CONSTANT TailMsg      : IN STRING;                    

    CONSTANT MsgSeverity    : IN SEVERITY_LEVEL := WARNING
    
    );

  procedure detect_resolution ( constant model_name : in string);
end;

package body simple_simprim_pck is
  procedure detect_resolution (
    constant model_name : in string
    ) IS
    
    variable test_value : time;
    variable Message : LINE;
  BEGIN
    test_value := 1 ps;
    if (test_value = 0 ps) then
      Write (Message, STRING'(" Simulator Resolution Error : "));
      Write (Message, STRING'(" Simulator resolution is set to a value greater than 1 ps. "));
      Write (Message, STRING'(" In order to simulate the "));
      Write (Message, model_name);
      Write (Message, STRING'(", the simulator resolution must be set to 1ps or smaller "));
      ASSERT FALSE REPORT Message.ALL SEVERITY ERROR;
      DEALLOCATE (Message);      
    end if;
  END detect_resolution;      

  PROCEDURE GenericValueCheckMessage (
    CONSTANT HeaderMsg      : IN STRING := " Attribute Syntax Error ";        
    CONSTANT GenericName : IN STRING := "";
    CONSTANT EntityName : IN STRING := "";
    CONSTANT InstanceName : IN STRING := "";
    CONSTANT GenericValue : IN STRING := "";
    Constant Unit : IN STRING := "";
    Constant ExpectedValueMsg : IN STRING := "";
    Constant ExpectedGenericValue : IN STRING := "";
    CONSTANT TailMsg      : IN STRING;                    

    CONSTANT MsgSeverity    : IN SEVERITY_LEVEL := WARNING
    
    ) IS
    VARIABLE Message : LINE;
  BEGIN

    Write ( Message, HeaderMsg );
    Write ( Message, STRING'(" The attribute ") );                
    Write ( Message, GenericName );
    Write ( Message, STRING'(" on ") );
    Write ( Message, EntityName );
    Write ( Message, STRING'(" instance ") );
    Write ( Message, InstanceName );
    Write ( Message, STRING'(" is set to  ") );        
    Write ( Message, GenericValue );
    Write ( Message, Unit );        
    Write ( Message, '.' & LF );
    Write ( Message, ExpectedValueMsg );
    Write ( Message, ExpectedGenericValue );        
    Write ( Message, Unit );
    Write ( Message, TailMsg );

    ASSERT FALSE REPORT Message.ALL SEVERITY MsgSeverity;

    DEALLOCATE (Message);
  END GenericValueCheckMessage;

  PROCEDURE GenericValueCheckMessage (
    CONSTANT HeaderMsg      : IN STRING := " Attribute Syntax Error ";        
    CONSTANT GenericName : IN STRING := "";
    CONSTANT EntityName : IN STRING := "";
    CONSTANT InstanceName : IN STRING := "";
    CONSTANT GenericValue : IN INTEGER;
    CONSTANT Unit : IN STRING := "";
    CONSTANT ExpectedValueMsg : IN STRING := "";
    CONSTANT ExpectedGenericValue : IN INTEGER;
    CONSTANT TailMsg      : IN STRING;                    

    CONSTANT MsgSeverity    : IN SEVERITY_LEVEL := WARNING
    
    ) IS
    VARIABLE Message : LINE;
  BEGIN

    Write ( Message, HeaderMsg );
    Write ( Message, STRING'(" The attribute ") );                
    Write ( Message, GenericName );
    Write ( Message, STRING'(" on ") );
    Write ( Message, EntityName );
    Write ( Message, STRING'(" instance ") );
    Write ( Message, InstanceName );
    Write ( Message, STRING'(" is set to  ") );        
    Write ( Message, GenericValue );
    Write ( Message, Unit );        
    Write ( Message, '.' & LF );
    Write ( Message, ExpectedValueMsg );
    Write ( Message, ExpectedGenericValue );        
    Write ( Message, Unit );
    Write ( Message, TailMsg );        

    ASSERT FALSE REPORT Message.ALL SEVERITY MsgSeverity;

    DEALLOCATE (Message);
  END GenericValueCheckMessage;

  PROCEDURE GenericValueCheckMessage (
    CONSTANT HeaderMsg      : IN STRING := " Attribute Syntax Error ";        
    CONSTANT GenericName : IN STRING := "";
    CONSTANT EntityName : IN STRING := "";
    CONSTANT InstanceName : IN STRING := "";
    CONSTANT GenericValue : IN BOOLEAN;
    Constant Unit : IN STRING := "";
    CONSTANT ExpectedValueMsg : IN STRING := "";
    CONSTANT ExpectedGenericValue : IN STRING := "";
    CONSTANT TailMsg      : IN STRING;                    

    CONSTANT MsgSeverity    : IN SEVERITY_LEVEL := WARNING
    
    ) IS
    VARIABLE Message : LINE;
  BEGIN

    Write ( Message, HeaderMsg );
    Write ( Message, STRING'(" The attribute ") );                
    Write ( Message, GenericName );
    Write ( Message, STRING'(" on ") );
    Write ( Message, EntityName );
    Write ( Message, STRING'(" instance ") );
    Write ( Message, InstanceName );
    Write ( Message, STRING'(" is set to  ") );        
    Write ( Message, GenericValue );
    Write ( Message, Unit );        
    Write ( Message, '.' & LF );
    Write ( Message, ExpectedValueMsg );
    Write ( Message, ExpectedGenericValue );        
    Write ( Message, Unit );
    Write ( Message, TailMsg );        

    ASSERT FALSE REPORT Message.ALL SEVERITY MsgSeverity;

    DEALLOCATE (Message);
  END GenericValueCheckMessage;

  PROCEDURE GenericValueCheckMessage (
    CONSTANT HeaderMsg      : IN STRING := " Attribute Syntax Error ";        
    CONSTANT GenericName : IN STRING := "";
    CONSTANT EntityName : IN STRING := "";
    CONSTANT InstanceName : IN STRING := "";
    CONSTANT GenericValue : IN INTEGER;
    CONSTANT Unit : IN STRING := "";
    CONSTANT ExpectedValueMsg : IN STRING := "";
    CONSTANT ExpectedGenericValue : IN STRING := "";
    CONSTANT TailMsg      : IN STRING;                    

    CONSTANT MsgSeverity    : IN SEVERITY_LEVEL := WARNING
    
    ) IS
    VARIABLE Message : LINE;
  BEGIN

    Write ( Message, HeaderMsg );
    Write ( Message, STRING'(" The attribute ") );                
    Write ( Message, GenericName );
    Write ( Message, STRING'(" on ") );
    Write ( Message, EntityName );
    Write ( Message, STRING'(" instance ") );
    Write ( Message, InstanceName );
    Write ( Message, STRING'(" is set to  ") );        
    Write ( Message, GenericValue );
    Write ( Message, Unit );        
    Write ( Message, '.' & LF );
    Write ( Message, ExpectedValueMsg );
    Write ( Message, ExpectedGenericValue );        
    Write ( Message, Unit );
    Write ( Message, TailMsg );        

    ASSERT FALSE REPORT Message.ALL SEVERITY MsgSeverity;

    DEALLOCATE (Message);
  END GenericValueCheckMessage;
  PROCEDURE GenericValueCheckMessage (
    CONSTANT HeaderMsg      : IN STRING := " Attribute Syntax Error ";        
    CONSTANT GenericName : IN STRING := "";
    CONSTANT EntityName : IN STRING := "";
    CONSTANT InstanceName : IN STRING := "";
    CONSTANT GenericValue : IN REAL;
    CONSTANT Unit : IN STRING := "";
    CONSTANT ExpectedValueMsg : IN STRING := "";
    CONSTANT ExpectedGenericValue : IN STRING := "";
    CONSTANT TailMsg      : IN STRING;                    

    CONSTANT MsgSeverity    : IN SEVERITY_LEVEL := WARNING
    
    ) IS
    VARIABLE Message : LINE;
  BEGIN

    Write ( Message, HeaderMsg );
    Write ( Message, STRING'(" The attribute ") );                
    Write ( Message, GenericName );
    Write ( Message, STRING'(" on ") );
    Write ( Message, EntityName );
    Write ( Message, STRING'(" instance ") );
    Write ( Message, InstanceName );
    Write ( Message, STRING'(" is set to  ") );        
    Write ( Message, GenericValue );
    Write ( Message, Unit );        
    Write ( Message, '.' & LF );
    Write ( Message, ExpectedValueMsg );
    Write ( Message, ExpectedGenericValue );        
    Write ( Message, Unit );
    Write ( Message, TailMsg );        

    ASSERT FALSE REPORT Message.ALL SEVERITY MsgSeverity;

    DEALLOCATE (Message);
  END GenericValueCheckMessage;

end;

library ieee;
use ieee.std_logic_1164.all;

package simple_simprim is

  component ramb4_generic
  generic ( abits : integer := 10; dbits : integer := 8 );
  port (DI     : in std_logic_vector (dbits-1 downto 0);
        EN     : in std_ulogic;
        WE     : in std_ulogic;
        RST    : in std_ulogic;
        CLK    : in std_ulogic;
        ADDR   : in std_logic_vector (abits-1 downto 0);
        DO     : out std_logic_vector (dbits-1 downto 0)
       );
  end component;     

  component ramb4_sx_sx
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
  end component;

  component ramb16_sx
  generic (abits : integer := 10; dbits : integer := 8 );
  port (
    DO : out std_logic_vector (dbits-1 downto 0);
    ADDR : in std_logic_vector (abits-1 downto 0);
    DI : in std_logic_vector (dbits-1 downto 0);
    EN : in std_ulogic;
    CLK : in std_ulogic;
    WE : in std_ulogic;
    SSR : in std_ulogic);
  end component;


  component ram16_sx_sx 
  generic (abits : integer := 10; dbits : integer := 8 );
  port (
   DOA : out std_logic_vector (dbits-1 downto 0);
   DOB : out std_logic_vector (dbits-1 downto 0);
   ADDRA : in std_logic_vector (abits-1 downto 0);
   CLKA : in std_ulogic;
   DIA : in std_logic_vector (dbits-1 downto 0);
   ENA : in std_ulogic;
   WEA : in std_ulogic;
   ADDRB : in std_logic_vector (abits-1 downto 0);
   CLKB : in std_ulogic;
   DIB : in std_logic_vector (dbits-1 downto 0);
   ENB : in std_ulogic;
   WEB : in std_ulogic);
  end component;

end;

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity ramb4_generic is
  generic ( abits : integer := 10; dbits : integer := 8 );
  port (DI     : in std_logic_vector (dbits-1 downto 0);
        EN     : in std_ulogic;
        WE     : in std_ulogic;
        RST    : in std_ulogic;
        CLK    : in std_ulogic;
        ADDR   : in std_logic_vector (abits-1 downto 0);
        DO     : out std_logic_vector (dbits-1 downto 0)
       );
end;     

architecture behavioral of ramb4_generic is
  type mem is array(0 to (2**abits -1)) 
	of std_logic_vector((dbits -1) downto 0);
begin
  main : process(clk)
  variable memarr : mem;
  begin
    if rising_edge(clk)then
      if (en = '1') and not (is_x(addr)) then
        do <= memarr(to_integer(unsigned(addr)));
      end if;
      if (we and en) = '1' then
        if not is_x(addr) then
   	  memarr(to_integer(unsigned(addr))) := di;
        end if;
      end if;
    end if;
  end process;

end;

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity ramb16_sx is
  generic ( abits : integer := 10; dbits : integer := 8 );
  port (
    DO : out std_logic_vector (dbits-1 downto 0);
    ADDR : in std_logic_vector (abits-1 downto 0);
    DI : in std_logic_vector (dbits-1 downto 0);
    EN : in std_ulogic;
    CLK : in std_ulogic;
    WE : in std_ulogic;
    SSR : in std_ulogic
  );
end;
architecture behav of ramb16_sx is
begin
  rp : process(clk)
  subtype dword is std_logic_vector(dbits-1 downto 0);
  type dregtype is array (0 to 2**abits -1) of DWord;
  variable rfd : dregtype;
  begin
    if rising_edge(clk) and not is_x (addr) then 
      if en = '1' then
        do <= rfd(to_integer(unsigned(addr)));
        if we = '1' then rfd(to_integer(unsigned(addr))) := di; end if;
      end if;
    end if;
  end process;
end;

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity ram16_sx_sx is
  generic ( abits : integer := 10; dbits : integer := 8 );
  port (
   DOA : out std_logic_vector (dbits-1 downto 0);
   DOB : out std_logic_vector (dbits-1 downto 0);
   ADDRA : in std_logic_vector (abits-1 downto 0);
   CLKA : in std_ulogic;
   DIA : in std_logic_vector (dbits-1 downto 0);
   ENA : in std_ulogic;
   WEA : in std_ulogic;
   ADDRB : in std_logic_vector (abits-1 downto 0);
   CLKB : in std_ulogic;
   DIB : in std_logic_vector (dbits-1 downto 0);
   ENB : in std_ulogic;
   WEB : in std_ulogic
  );
end;

architecture behav of ram16_sx_sx is
    signal async : std_ulogic := '0';
begin
  
  ramproc : process(clka, clkb)
    subtype dword is std_logic_vector(dbits-1 downto 0);
    type dregtype is array (0 to 2**abits -1) of DWord;
    variable rfd : dregtype;    
  begin
    
    if rising_edge(clka) and not is_x (addra) then
      if ena = '1' then
        if wea = '1' then
          rfd(to_integer(unsigned(addra))) := dia;
        end if;
          doa <= rfd(to_integer(unsigned(addra)));
      end if;
    end if;
    
    if rising_edge(clkb) and not is_x (addrb) then
      if enb = '1' then
        if web = '1' then
          rfd(to_integer(unsigned(addrb))) := dib;
        end if;
          dob <= rfd(to_integer(unsigned(addrb)));
        end if;
    end if;    

  end process;
  
end;
library ieee;
use ieee.std_logic_1164.all;

entity BSCAN_VIRTEX2 is
  port (CAPTURE : out STD_ULOGIC;
        DRCK1 : out STD_ULOGIC;
        DRCK2 : out STD_ULOGIC;
        RESET : out STD_ULOGIC;
        SEL1 : out STD_ULOGIC;
        SEL2 : out STD_ULOGIC;
        SHIFT : out STD_ULOGIC;
        TDI : out STD_ULOGIC;
        UPDATE : out STD_ULOGIC;
        TDO1 : in STD_ULOGIC;
        TDO2 : in STD_ULOGIC);
end;

architecture behav of BSCAN_VIRTEX2 is
begin
  CAPTURE <= '0'; DRCK1 <= '0'; DRCK2 <= '0'; 
  RESET <= '0'; SEL1 <= '0'; SEL2 <= '0'; 
  SHIFT <= '0'; TDI  <= '0'; UPDATE <= '0'; 
end;

library ieee;
use ieee.std_logic_1164.all;

entity BSCAN_VIRTEX4 is
  generic(
        JTAG_CHAIN : integer := 1
        );

  port(
    CAPTURE : out std_ulogic ;
    DRCK    : out std_ulogic ;
    RESET   : out std_ulogic ;
    SEL     : out std_ulogic ;
    SHIFT   : out std_ulogic ;
    TDI     : out std_ulogic ;
    UPDATE  : out std_ulogic ;
    TDO     : in std_ulogic
    );

end BSCAN_VIRTEX4;

architecture behav of BSCAN_VIRTEX4 is
begin
  CAPTURE <= '0'; DRCK <= '0';
  RESET <= '0'; SEL <= '0'; 
  SHIFT <= '0'; TDI  <= '0'; UPDATE <= '0'; 
end;

library ieee;
use ieee.std_logic_1164.all;
  
entity BSCAN_SPARTAN3 is
  port (CAPTURE : out STD_ULOGIC;
        DRCK1 : out STD_ULOGIC;
        DRCK2 : out STD_ULOGIC;
        RESET : out STD_ULOGIC;
        SEL1 : out STD_ULOGIC;
        SEL2 : out STD_ULOGIC;
        SHIFT : out STD_ULOGIC;
        TDI : out STD_ULOGIC;
        UPDATE : out STD_ULOGIC;
        TDO1 : in STD_ULOGIC;
        TDO2 : in STD_ULOGIC);
end;

architecture behav of BSCAN_SPARTAN3 is
begin
  CAPTURE <= '0'; DRCK1 <= '0'; DRCK2 <= '0'; 
  RESET <= '0'; SEL1 <= '0'; SEL2 <= '0'; 
  SHIFT <= '0'; TDI  <= '0'; UPDATE <= '0';       
end;

library ieee; use ieee.std_logic_1164.all;
entity BUFGMUX is port (O : out std_logic; I0, I1, S : in std_logic); end;
architecture beh of BUFGMUX is 
begin o <= to_X01(I0) when to_X01(S) = '0' else I1; end;

library ieee; use ieee.std_logic_1164.all;
entity BUFG is port (O : out std_logic; I : in std_logic); end;
architecture beh of BUFG is begin o <= to_X01(i); end;

library ieee; use ieee.std_logic_1164.all;
entity BUFGP is port (O : out std_logic; I : in std_logic); end;
architecture beh of BUFGP is begin o <= to_X01(i); end;

library ieee; use ieee.std_logic_1164.all;
entity BUFGDLL is port (O : out std_logic; I : in std_logic); end;
architecture beh of BUFGDLL is begin o <= to_X01(i); end;

library ieee; use ieee.std_logic_1164.all;
entity IBUFG is generic (
    CAPACITANCE : string := "DONT_CARE"; IOSTANDARD : string := "LVCMOS25");
  port (O : out std_logic; I : in std_logic); end;
architecture beh of IBUFG is begin o <= to_X01(i) after 1 ns; end;

library ieee; use ieee.std_logic_1164.all;
entity IBUF is generic (
    CAPACITANCE : string := "DONT_CARE"; IOSTANDARD : string := "LVCMOS25");
  port (O : out std_logic; I : in std_logic); end;
architecture beh of IBUF is begin o <= to_X01(i) after 1 ns; end;

library ieee;
use ieee.std_logic_1164.all;
entity OBUF is generic (
    CAPACITANCE : string := "DONT_CARE"; DRIVE : integer := 12;
    IOSTANDARD  : string := "LVCMOS25"; SLEW : string := "SLOW");
  port (O : out std_ulogic; I : in std_ulogic); end;
architecture beh of OBUF is
begin o <= to_X01(i) after 2 ns when slew = "SLOW" else  to_X01(i) after 1 ns; end;

library ieee;
use ieee.std_logic_1164.all;
entity IOBUF is  generic (
    CAPACITANCE : string := "DONT_CARE"; DRIVE : integer := 12;
    IOSTANDARD  : string := "LVCMOS25"; SLEW : string := "SLOW");
  port ( O  : out std_ulogic; IO : inout std_logic; I, T : in std_ulogic);
end;
architecture beh of IOBUF is
begin
  io <= 'X' after 2 ns when to_X01(t) = 'X' else 
        I after 2 ns  when (to_X01(t) = '0')  else 
       'Z' after 2 ns  when to_X01(t) = '1';
  o <= to_X01(io) after 1 ns;
end;

library ieee;
use ieee.std_logic_1164.all;
entity OBUFT is  generic (
    CAPACITANCE : string := "DONT_CARE"; DRIVE : integer := 12;
    IOSTANDARD  : string := "LVCMOS25"; SLEW : string := "SLOW");
  port ( O  : out std_ulogic; I, T : in std_ulogic);
end;
architecture beh of OBUFT is
begin
  o <= I after 2 ns when to_X01(t) = '0' else 
       'Z' after 2 ns  when to_X01(t) = '1' else 
	'X' after 2 ns ;
end;

library ieee; use ieee.std_logic_1164.all;
entity IBUFDS_LVDS_25 is
  port (O : out std_logic; I, IB : in std_logic); end;
architecture beh of IBUFDS_LVDS_25 is 
signal old : std_ulogic;
begin 
  
  old <= '1' after 1 ns when (to_X01(I) = '1') and (to_X01(IB) = '0') else
       '0' after 1 ns when (to_X01(I) = '0') and (to_X01(IB) = '1') else old;
  o <= old;
end;

library ieee; use ieee.std_logic_1164.all;
entity IBUFDS_LVDS_33 is
  port (O : out std_logic; I, IB : in std_logic); end;
architecture beh of IBUFDS_LVDS_33 is 
signal old : std_ulogic;
begin 
  
  old <= '1' after 1 ns when (to_X01(I) = '1') and (to_X01(IB) = '0') else
       '0' after 1 ns when (to_X01(I) = '0') and (to_X01(IB) = '1') else old;
  o <= old;
end;

library ieee; use ieee.std_logic_1164.all;
entity OBUFDS_LVDS_25 is
  port (O, OB : out std_ulogic; I : in std_ulogic); end;
architecture beh of OBUFDS_LVDS_25 is
begin 
  o <= to_X01(i) after 1 ns; ob <= not to_X01(i) after 1 ns;
end;

library ieee; use ieee.std_logic_1164.all;
entity OBUFDS_LVDS_33 is
  port (O, OB : out std_ulogic; I : in std_ulogic); end;
architecture beh of OBUFDS_LVDS_33 is
begin 
  o <= to_X01(i) after 1 ns; ob <= not to_X01(i) after 1 ns;
end;

----- CELL DCM  -----
----- dcm_clock_divide_by_2        -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity dcm_clock_divide_by_2 is
  port(
    clock_out : out std_ulogic := '0';

    clock : in std_ulogic;
    clock_type : in integer;
    rst : in std_ulogic
    );
end dcm_clock_divide_by_2;

architecture dcm_clock_divide_by_2_V of dcm_clock_divide_by_2 is
  signal clock_div2 : std_ulogic := '0';
  signal rst_reg : std_logic_vector(2 downto 0);
begin

  CLKIN_DIVIDER : process
  begin
    if (rising_edge(clock)) then
      clock_div2 <= not clock_div2;
    end if;
    wait on clock;
  end process CLKIN_DIVIDER;

  gen_reset : process
    begin
    if (rising_edge(clock)) then      
      rst_reg(0) <= rst;
      rst_reg(1) <= rst_reg(0) and rst;
      rst_reg(2) <= rst_reg(1) and rst_reg(0) and rst;
    end if;      
    wait on clock;    
  end process gen_reset;

  assign_clkout : process
  begin
    if ((clock_type = 0) and (rst = '0')) then
      clock_out <= clock;
    elsif ((clock_type = 1) and (rst = '0')) then
      clock_out <= clock_div2;
    elsif (rst = '1') then
      clock_out <= '0';
      wait until falling_edge(rst_reg(2));
    end if;
    wait on clock_div2, clock, rst;
  end process assign_clkout;
end dcm_clock_divide_by_2_V;

----- dcm_maximum_period_check  -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;

library STD;
use STD.TEXTIO.all;

entity dcm_maximum_period_check is
  generic (
    InstancePath : string := "*";

    clock_name : string := "";
    maximum_period : time);
  port(
    clock : in std_ulogic
    );
end dcm_maximum_period_check;

architecture dcm_maximum_period_check_V of dcm_maximum_period_check is
begin

  MAX_PERIOD_CHECKER : process
    variable clock_edge_previous : time := 0 ps;
    variable clock_edge_current : time := 0 ps;
    variable clock_period : time := 0 ps;
    variable Message : line;
  begin

    clock_edge_previous := clock_edge_current;
    clock_edge_current := NOW;

    if (clock_edge_previous > 0 ps) then
      clock_period := clock_edge_current - clock_edge_previous;
    end if;

    if (clock_period > maximum_period) then
      Write ( Message, string'(" Timing Violation Warning : Input Clock Period of"));
      Write ( Message, clock_period );
      Write ( Message, string'(" on the ") );
      Write ( Message, clock_name );      
      Write ( Message, string'(" port ") );      
      Write ( Message, string'(" of DCM instance ") );
      Write ( Message, InstancePath );
      Write ( Message, string'(" exceeds allotted value of ") );
      Write ( Message, maximum_period);
      Write ( Message, string'(" at simulation time ") );
      Write ( Message, clock_edge_current);
      Write ( Message, '.' & LF );
      assert false report Message.all severity warning;
      DEALLOCATE (Message);
    end if;
    wait on clock;
  end process MAX_PERIOD_CHECKER;
end dcm_maximum_period_check_V;

----- dcm_clock_lost  -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity dcm_clock_lost is
  port(
    lost : out std_ulogic := '1';

    clock : in std_ulogic
    );
end dcm_clock_lost;

architecture dcm_clock_lost_V of dcm_clock_lost is
  signal period : time := 0 ps;
  signal lost_r : std_ulogic := '0';
  signal lost_f : std_ulogic := '0';
  signal clock_negedge, clock_posedge : std_ulogic;
  signal temp1 : boolean := false;
  signal temp2 : boolean := false;
  signal clock_low, clock_high : std_ulogic := '0';
begin
  determine_period : process
    variable clock_edge_previous : time := 0 ps;
    variable clock_edge_current : time := 0 ps;    
  begin
    if (rising_edge(clock)) then
      clock_edge_previous := clock_edge_current;
      clock_edge_current := NOW;
      if (period /= 0 ps and ((clock_edge_current - clock_edge_previous) <= (1.5 * period))) then
        period <= NOW - clock_edge_previous;
      elsif (period /= 0 ps and ((NOW - clock_edge_previous) > (1.5 * period))) then
        period <= 0 ps;
      elsif ((period = 0 ps) and (clock_edge_previous /= 0 ps)) then
        period <= NOW - clock_edge_previous;
      end if;
    end if;
    wait on clock;
  end process determine_period;
  
  CLOCK_LOST_CHECKER : process
    variable clock_low, clock_high : std_ulogic := '0';

  begin
    if (rising_edge(clock)) then
      clock_low := '0';
      clock_high := '1';
      clock_posedge <= '0';              
      clock_negedge <= '1';            
    end if;

    if (falling_edge(clock)) then
      clock_high := '0';
      clock_low := '1';
      clock_posedge <= '1';
      clock_negedge <= '0';      
    end if;
    wait on clock;
  end process CLOCK_LOST_CHECKER;    

  SET_RESET_LOST_R : process
    begin
    if (rising_edge(clock)) then
      wait for 0 ps;
      wait for 0 ps;
      wait for 0 ps;            
      if (period /= 0 ps) then
        lost_r <= '0';        
      end if;    
      wait for (period * 9.1)/10;
      if ((clock_low /= '1') and (clock_posedge /= '1')) then
        lost_r <= '1';
      end if;
    end if;
    wait on clock;    
  end process SET_RESET_LOST_R;

  SET_RESET_LOST_F : process
    begin
    if (falling_edge(clock)) then    
      if (period /= 0 ps) then      
        lost_f <= '0';
      end if;
      wait for (period * 9.1)/10;
      if ((clock_high /= '1') and (clock_negedge /= '1')) then
        lost_f <= '1';
      end if;      
    end if;
    wait on clock;    
  end process SET_RESET_LOST_F;      

  assign_lost : process
    begin
      if (lost_r'event) then
        lost <= lost_r;
      end if;
      if (lost_f'event) then
        lost <= lost_f;
      end if;
      wait on lost_r, lost_f;
    end process assign_lost;
end dcm_clock_lost_V;


----- DCM  -----
library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.VITAL_Timing.all;

library STD;
use STD.TEXTIO.all;

library unisim;
use unisim.simple_simprim_pck.all;

entity DCM is
  generic (
    TimingChecksOn : boolean := true;
    InstancePath : string := "*";
    Xon : boolean := true;
    MsgOn : boolean := false;

    thold_PSEN_PSCLK_negedge_posedge : VitalDelayType := 0.000 ns;
    thold_PSEN_PSCLK_posedge_posedge : VitalDelayType := 0.000 ns;
    thold_PSINCDEC_PSCLK_negedge_posedge : VitalDelayType := 0.000 ns;
    thold_PSINCDEC_PSCLK_posedge_posedge : VitalDelayType := 0.000 ns;

    tipd_CLKFB : VitalDelayType01 := (0.000 ns, 0.000 ns);
    tipd_CLKIN : VitalDelayType01 := (0.000 ns, 0.000 ns);
    tipd_DSSEN : VitalDelayType01 := (0.000 ns, 0.000 ns);
    tipd_PSCLK : VitalDelayType01 := (0.000 ns, 0.000 ns);
    tipd_PSEN : VitalDelayType01 := (0.000 ns, 0.000 ns);
    tipd_PSINCDEC : VitalDelayType01 := (0.000 ns, 0.000 ns);
    tipd_RST : VitalDelayType01 := (0.000 ns, 0.000 ns);

    tpd_CLKIN_LOCKED : VitalDelayType01 := (0.000 ns, 0.000 ns);
    tpd_PSCLK_PSDONE : VitalDelayType01 := (0.000 ns, 0.000 ns);    

    tperiod_CLKIN_POSEDGE : VitalDelayType := 0.000 ns;
    tperiod_PSCLK_POSEDGE : VitalDelayType := 0.000 ns;

    tpw_CLKIN_negedge : VitalDelayType := 0.000 ns;
    tpw_CLKIN_posedge : VitalDelayType := 0.000 ns;
    tpw_PSCLK_negedge : VitalDelayType := 0.000 ns;
    tpw_PSCLK_posedge : VitalDelayType := 0.000 ns;
    tpw_RST_posedge : VitalDelayType := 0.000 ns;

    tsetup_PSEN_PSCLK_negedge_posedge : VitalDelayType := 0.000 ns;
    tsetup_PSEN_PSCLK_posedge_posedge : VitalDelayType := 0.000 ns;
    tsetup_PSINCDEC_PSCLK_negedge_posedge : VitalDelayType := 0.000 ns;
    tsetup_PSINCDEC_PSCLK_posedge_posedge : VitalDelayType := 0.000 ns;

    CLKDV_DIVIDE : real := 2.0;
    CLKFX_DIVIDE : integer := 1;
    CLKFX_MULTIPLY : integer := 4;
    CLKIN_DIVIDE_BY_2 : boolean := false;
    CLKIN_PERIOD : real := 0.0;                         --non-simulatable
    CLKOUT_PHASE_SHIFT : string := "NONE";
    CLK_FEEDBACK : string := "1X";
    DESKEW_ADJUST : string := "SYSTEM_SYNCHRONOUS";     --non-simulatable
    DFS_FREQUENCY_MODE : string := "LOW";
    DLL_FREQUENCY_MODE : string := "LOW";
    DSS_MODE : string := "NONE";                        --non-simulatable
    DUTY_CYCLE_CORRECTION : boolean := true;
    FACTORY_JF : bit_vector := X"C080";                 --non-simulatable
    MAXPERCLKIN : time := 1000000 ps;                   --non-modifiable simulation parameter
    MAXPERPSCLK : time := 100000000 ps;                 --non-modifiable simulation parameter
    PHASE_SHIFT : integer := 0;
    SIM_CLKIN_CYCLE_JITTER : time := 300 ps;            --non-modifiable simulation parameter
    SIM_CLKIN_PERIOD_JITTER : time := 1000 ps;          --non-modifiable simulation parameter
    STARTUP_WAIT : boolean := false                     --non-simulatable
    );

  port (
    CLK0 : out std_ulogic := '0';
    CLK180 : out std_ulogic := '0';
    CLK270 : out std_ulogic := '0';
    CLK2X : out std_ulogic := '0';
    CLK2X180 : out std_ulogic := '0';
    CLK90 : out std_ulogic := '0';
    CLKDV : out std_ulogic := '0';
    CLKFX : out std_ulogic := '0';
    CLKFX180 : out std_ulogic := '0';
    LOCKED : out std_ulogic := '0';
    PSDONE : out std_ulogic := '0';
    STATUS : out std_logic_vector(7 downto 0) := "00000000";
    
    CLKFB : in std_ulogic := '0';
    CLKIN : in std_ulogic := '0';
    DSSEN : in std_ulogic := '0';
    PSCLK : in std_ulogic := '0';
    PSEN : in std_ulogic := '0';
    PSINCDEC : in std_ulogic := '0';
    RST : in std_ulogic := '0'
    );

  attribute VITAL_LEVEL0 of DCM : entity is true;

end DCM;

architecture DCM_V of DCM is
  
  component dcm_clock_divide_by_2
    port(
      clock_out : out std_ulogic;

      clock : in std_ulogic;
      clock_type : in integer;
      rst : in std_ulogic
      );
  end component;

  component dcm_maximum_period_check
    generic (
      InstancePath : string := "*";

      clock_name : string := "";
      maximum_period : time);
    port(
      clock : in std_ulogic
      );
  end component;

  component dcm_clock_lost
    port(
      lost : out std_ulogic;

      clock : in std_ulogic
      );    
  end component;


  signal CLKFB_ipd, CLKIN_ipd, DSSEN_ipd : std_ulogic;
  signal PSCLK_ipd, PSEN_ipd, PSINCDEC_ipd, RST_ipd : std_ulogic;
  signal clk0_out : std_ulogic;
  signal clk2x_out, clkdv_out : std_ulogic := '0';
  signal clkfx_out, locked_out, psdone_out, ps_overflow_out, ps_lock : std_ulogic := '0';

  signal clkfb_type : integer;
  signal divide_type : integer;
  signal clkin_type : integer;
  signal ps_type : integer;
  signal deskew_adjust_mode : integer;
  signal dfs_mode_type : integer;
  signal dll_mode_type : integer;
  signal clk1x_type : integer;

  signal lock_period, lock_delay, lock_clkin, lock_clkfb : std_ulogic := '0';
  signal lock_out : std_logic_vector(1 downto 0) := "00";  

  signal lock_fb : std_ulogic := '0';
  signal fb_delay_found : std_ulogic := '0';

  signal clkin_div : std_ulogic;
  signal clkin_ps, clkin_ps0, clkin_ps1, clkin_ps2 : std_ulogic;
  signal clkin_fb, clkin_fb0, clkin_fb1, clkin_fb2 : std_ulogic;


  signal ps_delay : time := 0 ps;
  signal clkin_period_real : VitalDelayArrayType(2 downto 0) := (0.000 ns, 0.000 ns, 0.000 ns);
  signal period : time := 0 ps;
  signal period_div : time := 0 ps;
  signal period_orig : time := 0 ps;
  signal period_ps : time := 0 ps;
  signal clkout_delay : time := 0 ps;
  signal fb_delay : time := 0 ps;
  signal period_fx, remain_fx : time := 0 ps;
  signal period_dv_high, period_dv_low : time := 0 ps;
  signal cycle_jitter, period_jitter : time := 0 ps;

  signal clkin_window, clkfb_window : std_ulogic := '0';
  signal clkin_5050 : std_ulogic := '0';
  signal rst_reg : std_logic_vector(2 downto 0) := "000";
  signal numerator, denominator, gcd : integer := 1;

  signal clkin_lost_out : std_ulogic;
  signal clkfx_lost_out : std_ulogic;

  signal remain_fx_temp : integer := 0;

  signal clkin_period_real0_temp : time := 0 ps;
  signal ps_lock_temp : std_ulogic := '0';

  signal clk0_temp : std_ulogic := '0';
  signal clk2x_temp : std_ulogic := '0';

  signal no_stop : boolean := false;

  signal clkfx180_en : std_ulogic := '0';

  signal status_out  : std_logic_vector(7 downto 0) := "00000000";  

begin
  INITPROC : process
  begin
    detect_resolution
      (model_name => "DCM"
       );
    
    if (CLKDV_DIVIDE = 1.5) then
      divide_type <= 3;
    elsif (CLKDV_DIVIDE = 2.0) then
      divide_type <= 4;
    elsif (CLKDV_DIVIDE = 2.5) then
      divide_type <= 5;
    elsif (CLKDV_DIVIDE = 3.0) then
      divide_type <= 6;
    elsif (CLKDV_DIVIDE = 3.5) then
      divide_type <= 7;
    elsif (CLKDV_DIVIDE = 4.0) then
      divide_type <= 8;
    elsif (CLKDV_DIVIDE = 4.5) then
      divide_type <= 9;
    elsif (CLKDV_DIVIDE = 5.0) then
      divide_type <= 10;
    elsif (CLKDV_DIVIDE = 5.5) then
      divide_type <= 11;
    elsif (CLKDV_DIVIDE = 6.0) then
      divide_type <= 12;
    elsif (CLKDV_DIVIDE = 6.5) then
      divide_type <= 13;
    elsif (CLKDV_DIVIDE = 7.0) then
      divide_type <= 14;
    elsif (CLKDV_DIVIDE = 7.5) then
      divide_type <= 15;
    elsif (CLKDV_DIVIDE = 8.0) then
      divide_type <= 16;
    elsif (CLKDV_DIVIDE = 9.0) then
      divide_type <= 18;
    elsif (CLKDV_DIVIDE = 10.0) then
      divide_type <= 20;
    elsif (CLKDV_DIVIDE = 11.0) then
      divide_type <= 22;
    elsif (CLKDV_DIVIDE = 12.0) then
      divide_type <= 24;
    elsif (CLKDV_DIVIDE = 13.0) then
      divide_type <= 26;
    elsif (CLKDV_DIVIDE = 14.0) then
      divide_type <= 28;
    elsif (CLKDV_DIVIDE = 15.0) then
      divide_type <= 30;
    elsif (CLKDV_DIVIDE = 16.0) then
      divide_type <= 32;
    else
      GenericValueCheckMessage
        (HeaderMsg => "Attribute Syntax Error",
         GenericName => "CLKDV_DIVIDE",
         EntityName => "DCM",
         InstanceName => InstancePath,
         GenericValue => CLKDV_DIVIDE,
         Unit => "",
         ExpectedValueMsg => "Legal Values for this attribute are 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0, 6.5, 7.0, 7.5, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, or 16.0",
         ExpectedGenericValue => "",
         TailMsg => "",
         MsgSeverity => error
         );
    end if;

    if ((CLKFX_DIVIDE <= 0) or (32 < CLKFX_DIVIDE)) then
      GenericValueCheckMessage
        (HeaderMsg => "Attribute Syntax Error",
         GenericName => "CLKFX_DIVIDE",
         EntityName => "DCM",
         InstanceName => InstancePath,
         GenericValue => CLKFX_DIVIDE,
         Unit => "",
         ExpectedValueMsg => "Legal Values for this attribute are 1....32",
         ExpectedGenericValue => "",
         TailMsg => "",
         MsgSeverity => error
         );
    end if;
    if ((CLKFX_MULTIPLY <= 1) or (32 < CLKFX_MULTIPLY)) then
      GenericValueCheckMessage
        (HeaderMsg => "Attribute Syntax Error",
         GenericName => "CLKFX_MULTIPLY",
         EntityName => "DCM",
         InstanceName => InstancePath,
         GenericValue => CLKFX_MULTIPLY,
         Unit => "",
         ExpectedValueMsg => "Legal Values for this attribute are 2....32",
         ExpectedGenericValue => "",
         TailMsg => "",
         MsgSeverity => error
         );
    end if;
    case CLKIN_DIVIDE_BY_2 is
      when false => clkin_type <= 0;
      when true => clkin_type <= 1;
      when others =>
        GenericValueCheckMessage
          (HeaderMsg => "Attribute Syntax Error",
           GenericName => "CLKIN_DIVIDE_BY_2",
           EntityName => "DCM",
           InstanceName => InstancePath,
           GenericValue => CLKIN_DIVIDE_BY_2,
           Unit => "",
           ExpectedValueMsg => "Legal Values for this attribute are TRUE or FALSE",
           ExpectedGenericValue => "",
           TailMsg => "",
           MsgSeverity => error
           );
    end case;


    if ((CLKOUT_PHASE_SHIFT = "none") or (CLKOUT_PHASE_SHIFT = "NONE")) then
      ps_type <= 0;      
    elsif ((CLKOUT_PHASE_SHIFT = "fixed") or (CLKOUT_PHASE_SHIFT = "FIXED")) then
      ps_type <= 1;
    elsif ((CLKOUT_PHASE_SHIFT = "variable") or (CLKOUT_PHASE_SHIFT = "VARIABLE")) then
      ps_type <= 2;
    else
      GenericValueCheckMessage
        (HeaderMsg => "Attribute Syntax Error",
         GenericName => "CLKOUT_PHASE_SHIFT",
         EntityName => "DCM",
         InstanceName => InstancePath,
         GenericValue => CLKOUT_PHASE_SHIFT,
         Unit => "",
         ExpectedValueMsg => "Legal Values for this attribute are NONE, FIXED or VARIABLE",
         ExpectedGenericValue => "",
         TailMsg => "",
         MsgSeverity => error
         );
    end if;

    if ((CLK_FEEDBACK = "none") or (CLK_FEEDBACK = "NONE")) then
      clkfb_type <= 0;
    elsif ((CLK_FEEDBACK = "1x") or (CLK_FEEDBACK = "1X")) then
      clkfb_type <= 1;
    elsif ((CLK_FEEDBACK = "2x") or (CLK_FEEDBACK = "2X")) then
      clkfb_type <= 2;
    else
      GenericValueCheckMessage
        (HeaderMsg => "Attribute Syntax Error",
         GenericName => "CLK_FEEDBACK",
         EntityName => "DCM",
         InstanceName => InstancePath,
         GenericValue => CLK_FEEDBACK,
         Unit => "",
         ExpectedValueMsg => "Legal Values for this attribute are NONE, 1X or 2X",
         ExpectedGenericValue => "",
         TailMsg => "",
         MsgSeverity => error
         );
    end if;



    if ((DESKEW_ADJUST = "source_synchronous") or (DESKEW_ADJUST = "SOURCE_SYNCHRONOUS")) then
      DESKEW_ADJUST_mode <= 8;
    elsif ((DESKEW_ADJUST = "system_synchronous") or (DESKEW_ADJUST = "SYSTEM_SYNCHRONOUS")) then
      DESKEW_ADJUST_mode <= 11;
    elsif ((DESKEW_ADJUST = "0")) then
      DESKEW_ADJUST_mode <= 0;
    elsif ((DESKEW_ADJUST = "1")) then
      DESKEW_ADJUST_mode <= 1;
    elsif ((DESKEW_ADJUST = "2")) then
      DESKEW_ADJUST_mode <= 2;
    elsif ((DESKEW_ADJUST = "3")) then
      DESKEW_ADJUST_mode <= 3;
    elsif ((DESKEW_ADJUST = "4")) then
      DESKEW_ADJUST_mode <= 4;
    elsif ((DESKEW_ADJUST = "5")) then
      DESKEW_ADJUST_mode <= 5;
    elsif ((DESKEW_ADJUST = "6")) then
      DESKEW_ADJUST_mode <= 6;
    elsif ((DESKEW_ADJUST = "7")) then
      DESKEW_ADJUST_mode <= 7;
    elsif ((DESKEW_ADJUST = "8")) then
      DESKEW_ADJUST_mode <= 8;
    elsif ((DESKEW_ADJUST = "9")) then
      DESKEW_ADJUST_mode <= 9;
    elsif ((DESKEW_ADJUST = "10")) then
      DESKEW_ADJUST_mode <= 10;
    elsif ((DESKEW_ADJUST = "11")) then
      DESKEW_ADJUST_mode <= 11;
    elsif ((DESKEW_ADJUST = "12")) then
      DESKEW_ADJUST_mode <= 12;
    elsif ((DESKEW_ADJUST = "13")) then
      DESKEW_ADJUST_mode <= 13;
    elsif ((DESKEW_ADJUST = "14")) then
      DESKEW_ADJUST_mode <= 14;
    elsif ((DESKEW_ADJUST = "15")) then
      DESKEW_ADJUST_mode <= 15;
    else
      GenericValueCheckMessage
        (HeaderMsg => "Attribute Syntax Error",
         GenericName => "DESKEW_ADJUST_MODE",
         EntityName => "DCM",
         InstanceName => InstancePath,
         GenericValue => DESKEW_ADJUST_MODE,
         Unit => "",
         ExpectedValueMsg => "Legal Values for this attribute are SOURCE_SYNCHRONOUS, SYSTEM_SYNCHRONOUS or 1....15",
         ExpectedGenericValue => "",
         TailMsg => "",
         MsgSeverity => error
         );
    end if;

    if ((DFS_FREQUENCY_MODE = "high") or (DFS_FREQUENCY_MODE = "HIGH")) then
      dfs_mode_type <= 1;
    elsif ((DFS_FREQUENCY_MODE = "low") or (DFS_FREQUENCY_MODE = "LOW")) then
      dfs_mode_type <= 0;
    else
      GenericValueCheckMessage
        (HeaderMsg => "Attribute Syntax Error",
         GenericName => "DFS_FREQUENCY_MODE",
         EntityName => "DCM",
         InstanceName => InstancePath,
         GenericValue => DFS_FREQUENCY_MODE,
         Unit => "",
         ExpectedValueMsg => "Legal Values for this attribute are HIGH or LOW",
         ExpectedGenericValue => "",
         TailMsg => "",
         MsgSeverity => error
         );
    end if;

    if ((DLL_FREQUENCY_MODE = "high") or (DLL_FREQUENCY_MODE = "HIGH")) then
      dll_mode_type <= 1;
    elsif ((DLL_FREQUENCY_MODE = "low") or (DLL_FREQUENCY_MODE = "LOW")) then
      dll_mode_type <= 0;
    else
      GenericValueCheckMessage
        (HeaderMsg => "Attribute Syntax Error",
         GenericName => "DLL_FREQUENCY_MODE",
         EntityName => "DCM",
         InstanceName => InstancePath,
         GenericValue => DLL_FREQUENCY_MODE,
         Unit => "",
         ExpectedValueMsg => "Legal Values for this attribute are HIGH or LOW",
         ExpectedGenericValue => "",
         TailMsg => "",
         MsgSeverity => error
         );
    end if;

    if ((DSS_MODE = "none") or (DSS_MODE = "NONE")) then
    else
      GenericValueCheckMessage
        (HeaderMsg => "Attribute Syntax Error",
         GenericName => "DSS_MODE",
         EntityName => "DCM",
         InstanceName => InstancePath,
         GenericValue => DSS_MODE,
         Unit => "",
         ExpectedValueMsg => "Legal Values for this attribute are NONE",
         ExpectedGenericValue => "",
         TailMsg => "",
         MsgSeverity => error
         );
    end if;

    case DUTY_CYCLE_CORRECTION is
      when false => clk1x_type <= 0;
      when true => clk1x_type <= 1;
      when others =>
        GenericValueCheckMessage
          (HeaderMsg => "Attribute Syntax Error",
           GenericName => "DUTY_CYCLE_CORRECTION",
           EntityName => "DCM",
           InstanceName => InstancePath,
           GenericValue => DUTY_CYCLE_CORRECTION,
           Unit => "",
           ExpectedValueMsg => "Legal Values for this attribute are TRUE or FALSE",
           ExpectedGenericValue => "",
           TailMsg => "",
           MsgSeverity => error
           );
    end case;

    if ((PHASE_SHIFT < -255) or (PHASE_SHIFT > 255)) then
      GenericValueCheckMessage
        (HeaderMsg => "Attribute Syntax Error",
         GenericName => "PHASE_SHIFT",
         EntityName => "DCM",
         InstanceName => InstancePath,
         GenericValue => PHASE_SHIFT,
         Unit => "",
         ExpectedValueMsg => "Legal Values for this attribute are -255 ... 255",
         ExpectedGenericValue => "",
         TailMsg => "",
         MsgSeverity => error
         );
    end if;

    period_jitter <= SIM_CLKIN_PERIOD_JITTER;
    cycle_jitter <= SIM_CLKIN_CYCLE_JITTER;
    
    case STARTUP_WAIT is
      when false => null;
      when true => null;
      when others =>
        GenericValueCheckMessage
          (HeaderMsg => "Attribute Syntax Error",
           GenericName => "STARTUP_WAIT",
           EntityName => "DCM",
           InstanceName => InstancePath,
           GenericValue => STARTUP_WAIT,
           Unit => "",
           ExpectedValueMsg => "Legal Values for this attribute are TRUE or FALSE",
           ExpectedGenericValue => "",
           TailMsg => "",
           MsgSeverity => error
           );
    end case;

--
-- fx parameters
--    
    gcd <= 1;
    for i in 2 to CLKFX_MULTIPLY loop
      if (((CLKFX_MULTIPLY mod i) = 0) and ((CLKFX_DIVIDE mod i) = 0)) then
        gcd <= i;
      end if;
    end loop;    
    numerator <= CLKFX_MULTIPLY / gcd;
    denominator <= CLKFX_DIVIDE / gcd;          
    wait;
  end process INITPROC;

--
-- input wire delays
--  

  WireDelay : block
  begin
    VitalWireDelay (CLKIN_ipd, CLKIN, tipd_CLKIN);
    VitalWireDelay (CLKFB_ipd, CLKFB, tipd_CLKFB);
    VitalWireDelay (DSSEN_ipd, DSSEN, tipd_DSSEN);
    VitalWireDelay (PSCLK_ipd, PSCLK, tipd_PSCLK);
    VitalWireDelay (PSEN_ipd, PSEN, tipd_PSEN);
    VitalWireDelay (PSINCDEC_ipd, PSINCDEC, tipd_PSINCDEC);
    VitalWireDelay (RST_ipd, RST, tipd_RST);
  end block;

  i_clock_divide_by_2 : dcm_clock_divide_by_2
    port map (
      clock => clkin_ipd,
      clock_type => clkin_type,
      rst => rst_ipd,
      clock_out => clkin_div);

  i_max_clkin : dcm_maximum_period_check
    generic map (
      clock_name => "CLKIN",
      maximum_period => MAXPERCLKIN)

    port map (
      clock => clkin_ipd);

  i_max_psclk : dcm_maximum_period_check
    generic map (
      clock_name => "PSCLK",
      maximum_period => MAXPERPSCLK)

    port map (
      clock => psclk_ipd);

  i_clkin_lost : dcm_clock_lost
    port map (
      lost  => clkin_lost_out,
      clock => clkin_ipd);

  i_clkfx_lost : dcm_clock_lost
    port map (
      lost  => clkfx_lost_out,
      clock => clkfx_out);  
  
  clkin_ps0 <= transport clkin_div after ps_delay/4;
  clkin_ps1 <= transport clkin_ps0 after ps_delay/4;
  clkin_ps2 <= transport clkin_ps1 after ps_delay/4;
  clkin_ps <= transport clkin_ps2 after ps_delay/4;

  clkin_fb0 <= transport (clkin_ps and lock_fb) after period_ps/4;
  clkin_fb1 <= transport clkin_fb0 after period_ps/4;
  clkin_fb2 <= transport clkin_fb1 after period_ps/4;
  clkin_fb <= transport clkin_fb2 after period_ps/4;
  
  determine_period_div : process
    variable clkin_div_edge_previous : time := 0 ps; 
    variable clkin_div_edge_current : time := 0 ps;
  begin
    if (rst_ipd'event) then
      clkin_div_edge_previous := 0 ps; 
      clkin_div_edge_current := 0 ps;
      period_div <= 0 ps;
    else
      if (rising_edge(clkin_div)) then
        clkin_div_edge_previous := clkin_div_edge_current;
        clkin_div_edge_current := NOW;
        if ((clkin_div_edge_current - clkin_div_edge_previous) <= (1.5 * period_div)) then
          period_div <= clkin_div_edge_current - clkin_div_edge_previous;
        elsif ((period_div = 0 ps) and (clkin_div_edge_previous /= 0 ps)) then
          period_div <= clkin_div_edge_current - clkin_div_edge_previous;      
        end if;          
      end if;    
    end if;
    wait on clkin_div, rst_ipd;
  end process determine_period_div;

  determine_period_ps : process
    variable clkin_ps_edge_previous : time := 0 ps; 
    variable clkin_ps_edge_current : time := 0 ps;    
  begin
    if (rst_ipd'event) then
      clkin_ps_edge_previous := 0 ps; 
      clkin_ps_edge_current := 0 ps;
      period_ps <= 0 ps;
    else    
      if (rising_edge(clkin_ps)) then
        clkin_ps_edge_previous := clkin_ps_edge_current;
        clkin_ps_edge_current := NOW;
        wait for 0 ps;
        if ((clkin_ps_edge_current - clkin_ps_edge_previous) <= (1.5 * period_ps)) then
          period_ps <= clkin_ps_edge_current - clkin_ps_edge_previous;
        elsif ((period_ps = 0 ps) and (clkin_ps_edge_previous /= 0 ps)) then
          period_ps <= clkin_ps_edge_current - clkin_ps_edge_previous;      
        end if;
      end if;
    end if;
    wait on clkin_ps, rst_ipd;    
  end process determine_period_ps;

  assign_lock_ps_fb : process
  variable lock_ps : std_ulogic := '0';
  begin
    if (rst_ipd'event) then
      lock_fb <= '0';
      lock_ps := '0';                                
    else
      if (rising_edge(clkin_ps)) then
        lock_ps := lock_period;
        lock_fb <= lock_ps;            
      end if;          
    end if;
    wait on clkin_ps, rst_ipd;
  end process assign_lock_ps_fb;

  calculate_clkout_delay : process
  begin
    if (rst_ipd'event) then
      clkout_delay <= 0 ps;        
    elsif (period'event or fb_delay'event) then
      clkout_delay <= period - fb_delay;        
    end if;
    wait on period, fb_delay, rst_ipd;
  end process calculate_clkout_delay;

--
--generate master reset signal
--  

  gen_master_rst : process
  begin
    if (rising_edge(clkin_ipd)) then    
      rst_reg(2) <= rst_reg(1) and rst_reg(0) and rst_ipd;    
      rst_reg(1) <= rst_reg(0) and rst_ipd;
      rst_reg(0) <= rst_ipd;
    end if;
    wait on clkin_ipd;     
  end process gen_master_rst;

  check_rst_width : process
    variable Message : line;    
    begin
      if (falling_edge(rst_ipd)) then
        if ((rst_reg(2) and rst_reg(1) and rst_reg(0)) = '0') then
          Write ( Message, string'(" Timing Violation Error : RST on instance "));
          Write ( Message, Instancepath );                    
          Write ( Message, string'(" must be asserted for 3 CLKIN clock cycles. "));          
          assert false report Message.all severity error;
          DEALLOCATE (Message);
        end if;        
      end if;

      wait on rst_ipd;
    end process check_rst_width;

--
--phase shift parameters
--  

  determine_phase_shift : process
    variable Message : line;
    variable  FINE_SHIFT_RANGE : time;
    variable first_time : boolean := true;
    variable ps_in : integer;        
  begin
    if (first_time = true) then
      if ((CLKOUT_PHASE_SHIFT = "none") or (CLKOUT_PHASE_SHIFT = "NONE")) then
        ps_in := 256;      
      elsif ((CLKOUT_PHASE_SHIFT = "fixed") or (CLKOUT_PHASE_SHIFT = "FIXED")) then
        ps_in := 256 + PHASE_SHIFT;
      elsif ((CLKOUT_PHASE_SHIFT = "variable") or (CLKOUT_PHASE_SHIFT = "VARIABLE")) then
        ps_in := 256 + PHASE_SHIFT;
      end if;
      first_time := false;
    end if;
    if (rst_ipd'event) then
      if ((CLKOUT_PHASE_SHIFT = "none") or (CLKOUT_PHASE_SHIFT = "NONE")) then
        ps_in := 256;      
      elsif ((CLKOUT_PHASE_SHIFT = "fixed") or (CLKOUT_PHASE_SHIFT = "FIXED")) then
        ps_in := 256 + PHASE_SHIFT;
      elsif ((CLKOUT_PHASE_SHIFT = "variable") or (CLKOUT_PHASE_SHIFT = "VARIABLE")) then
        ps_in := 256 + PHASE_SHIFT;
      else
      end if;
      ps_lock <= '0';
      ps_overflow_out <= '0';
      ps_delay <= 0 ps;
    else
      if (rising_edge (lock_period)) then
        if (ps_type = 1) then
          FINE_SHIFT_RANGE := 10000 ps;
        elsif (ps_type = 2) then
          FINE_SHIFT_RANGE := 5000 ps;
        end if;
        ps_delay <= (ps_in * period_div / 256);
        if (PHASE_SHIFT > 0) then
          if (((ps_in * period_orig) / 256) > (period_orig + FINE_SHIFT_RANGE)) then
            Write ( Message, string'(" Timing Violation Error : FINE_SHIFT_RANGE on instance "));
            Write ( Message, Instancepath );          
            Write ( Message, string'(" exceeds "));
            Write ( Message, FINE_SHIFT_RANGE / 1000.0);
            Write ( Message, string'(" PHASE_SHIFT * PERIOD/256 = "));
            Write ( Message, PHASE_SHIFT);
            Write ( Message, string'(" * "));            
            Write ( Message, period_orig / 1000.0/256);            
            Write ( Message, string'(" = "));
            Write ( Message, (PHASE_SHIFT) * period_orig / 256 / 1000.0);                      
            assert false report Message.all severity error;
            DEALLOCATE (Message);          
          end if;
        elsif (PHASE_SHIFT < 0) then
          if ((period_orig > FINE_SHIFT_RANGE) and ((ps_in * period_orig / 256) < period_orig - FINE_SHIFT_RANGE)) then
            Write ( Message, string'(" Timing Violation Error : FINE_SHIFT_RANGE on instance "));
            Write ( Message, Instancepath );          
            Write ( Message, string'(" exceeds "));
            Write ( Message, FINE_SHIFT_RANGE / 1000.0);
            Write ( Message, string'(" PHASE_SHIFT * PERIOD/256 = "));
            Write ( Message, PHASE_SHIFT);
            Write ( Message, string'(" * "));            
            Write ( Message, period_orig / 1000.0/256);            
            Write ( Message, string'(" = "));
            Write ( Message, -(PHASE_SHIFT) * period_orig / 256 / 1000.0);                      
            assert false report Message.all severity error;
            DEALLOCATE (Message);                  
          end if;
        end if;
      end if;      
      if (rising_edge(PSCLK_ipd)) then
        if (ps_type = 2) then
          if (psen_ipd = '1') then
            if (ps_lock = '1') then
              Write ( Message, string'(" Timing Violation Warning : Please wait for PSDONE signal before adjusting the Phase Shift. "));
              assert false report Message.all severity warning;
              DEALLOCATE (Message);              
            else
              if (psincdec_ipd = '1') then
                if (ps_in = 511) then
                  ps_overflow_out <= '1';
                elsif (((ps_in + 1) * period_orig / 256) > period_orig + FINE_SHIFT_RANGE) then
                  ps_overflow_out <= '1';
                else
                  ps_in := ps_in + 1;
                  ps_delay <= (ps_in * period_div / 256);
                  ps_overflow_out <= '0';
                end if;
                ps_lock <= '1';                
              elsif (psincdec_ipd = '0') then
                if (ps_in = 1) then
                  ps_overflow_out <= '1';
                elsif ((period_orig > FINE_SHIFT_RANGE) and (((ps_in - 1) * period_orig / 256) < period_orig - FINE_SHIFT_RANGE)) then
                  ps_overflow_out <= '1';
                else
                  ps_in := ps_in - 1;
                  ps_delay <= (ps_in * period_div / 256);
                  ps_overflow_out <= '0';
                end if;
                ps_lock <= '1';                                
              end if;
            end if;
          end if;
        end if;
      end if;          
    end if;
    if (ps_lock_temp'event) then
      ps_lock <= ps_lock_temp;
    end if;
    wait on lock_period, psclk_ipd, ps_lock_temp, rst_ipd;
  end process determine_phase_shift;

  determine_psdone_out : process
  begin
    if (rising_edge(ps_lock)) then
      ps_lock_temp <= '1';      
      wait until (rising_edge(clkin_ps));
      wait until (rising_edge(psclk_ipd));
      wait until (rising_edge(psclk_ipd));
      wait until (rising_edge(psclk_ipd));
      psdone_out <= '1';
      wait until (rising_edge(psclk_ipd));
      psdone_out <= '0';
      ps_lock_temp <= '0';
    end if;
    wait on ps_lock;
  end process determine_psdone_out;      
  
--
--determine clock period
--    
  determine_clock_period : process
    variable clkin_edge_previous : time := 0 ps; 
    variable clkin_edge_current : time := 0 ps;
  begin
    if (rst_ipd'event) then
      clkin_period_real(0) <= 0 ps;
      clkin_period_real(1) <= 0 ps;
      clkin_period_real(2) <= 0 ps;
      clkin_edge_previous := 0 ps;      
      clkin_edge_current := 0 ps;
    elsif (rising_edge(clkin_div)) then
      clkin_edge_previous := clkin_edge_current;
      clkin_edge_current := NOW;
      clkin_period_real(2) <= clkin_period_real(1);
      clkin_period_real(1) <= clkin_period_real(0);      
      if (clkin_edge_previous /= 0 ps) then
	clkin_period_real(0) <= clkin_edge_current - clkin_edge_previous;
      end if;
    end if;      
    if (no_stop'event) then
      clkin_period_real(0) <= clkin_period_real0_temp;
    end if;
    wait on clkin_div, no_stop, rst_ipd;
  end process determine_clock_period;
  
  evaluate_clock_period : process
    variable clock_stopped : std_ulogic := '1';
    variable Message : line;
  begin
    if (rst_ipd'event) then
      lock_period <= '0';
      clock_stopped := '1';
      clkin_period_real0_temp <= 0 ps;                  
    else
      if (falling_edge(clkin_div)) then
        if (lock_period = '0') then
          if ((clkin_period_real(0) /= 0 ps ) and (clkin_period_real(0) - cycle_jitter <= clkin_period_real(1)) and (clkin_period_real(1) <= clkin_period_real(0) + cycle_jitter) and (clkin_period_real(1) - cycle_jitter <= clkin_period_real(2)) and (clkin_period_real(2) <= clkin_period_real(1) + cycle_jitter)) then
            lock_period <= '1';
            period_orig <= (clkin_period_real(0) + clkin_period_real(1) + clkin_period_real(2)) / 3;
            period <= clkin_period_real(0);
          end if;
        elsif (lock_period = '1') then
          if (100000000 ps < clkin_period_real(0)/1000) then
            Write ( Message, string'(" Timing Violation Error : CLKIN stopped toggling on instance "));
            Write ( Message, Instancepath );          
            Write ( Message, string'(" exceeds "));
            Write ( Message, string'(" 10000 "));
            Write ( Message, string'(" Current CLKIN Period = "));
            Write ( Message, string'(" clkin_period(0) / 10000.0 "));
            Write ( Message, string'(" ns "));            
            assert false report Message.all severity warning;
            DEALLOCATE (Message);
            lock_period <= '0';
            wait until (falling_edge(rst_reg(2)));            
          elsif ((period_orig * 2 < clkin_period_real(0)) and (clock_stopped = '0')) then
              clkin_period_real0_temp <= clkin_period_real(1);            
              no_stop <= not no_stop;
              clock_stopped := '1';
          elsif ((clkin_period_real(0) < period_orig - period_jitter) or (period_orig + period_jitter < clkin_period_real(0))) then
            Write ( Message, string'(" Timing Violation Error : Input Clock Period Jitter on instance "));
            Write ( Message, Instancepath );          
            Write ( Message, string'(" exceeds "));
            Write ( Message, period_jitter / 1000.0 );
            Write ( Message, string'(" Locked CLKIN Period =  "));
            Write ( Message, period_orig / 1000.0 );
            Write ( Message, string'(" Current CLKIN Period =  "));
            Write ( Message, clkin_period_real(0) / 1000.0 );
            assert false report Message.all severity warning;
            DEALLOCATE (Message);
            lock_period <= '0';
            wait until (falling_edge(rst_reg(2)));                                  
          elsif ((clkin_period_real(0) < clkin_period_real(1) - cycle_jitter) or (clkin_period_real(1) + cycle_jitter < clkin_period_real(0))) then
            Write ( Message, string'(" Timing Violation Error : Input Clock Cycle Jitter on on instance "));
            Write ( Message, Instancepath );
            Write ( Message, string'(" exceeds "));
            Write ( Message, cycle_jitter / 1000.0 );
            Write ( Message, string'(" Previous CLKIN Period =  "));
            Write ( Message, clkin_period_real(1) / 1000.0 );
            Write ( Message, string'(" Current CLKIN Period =  "));
            Write ( Message, clkin_period_real(0) / 1000.0 );                    
            assert false report Message.all severity warning;
            DEALLOCATE (Message);
            lock_period <= '0';
            wait until (falling_edge(rst_reg(2)));
          else           
            period <= clkin_period_real(0);
            clock_stopped := '0';
          end if;
        end if;  
      end if;          
    end if;
    wait on clkin_div, rst_ipd;
  end process evaluate_clock_period;    

--
--determine clock delay
--  
  
  determine_clock_delay : process
    variable delay_edge : time := 0 ps;
    variable temp1 : integer := 0;
    variable temp2 : integer := 0;        
    variable temp : integer := 0;
    variable delay_edge_current : time := 0 ps;    
  begin
    if (rst_ipd'event) then
      fb_delay <= 0 ps;
      fb_delay_found <= '0';                        
    else
      if (rising_edge(lock_period)) then
        if  ((lock_period = '1') and (clkfb_type /= 0)) then
          if (clkfb_type = 1) then
            wait until ((rising_edge(clk0_temp)) or (rst_ipd'event));                                
            delay_edge := NOW;
          elsif (clkfb_type = 2) then
            wait until ((rising_edge(clk2x_temp)) or (rst_ipd'event));            
            delay_edge := NOW;
          end if;
          wait until ((rising_edge(clkfb_ipd)) or (rst_ipd'event));          
          temp1 := ((NOW*1) - (delay_edge*1))/ (1 ps);
          temp2 := (period_orig * 1)/ (1 ps);
          temp := temp1 mod temp2;
          fb_delay <= temp * 1 ps;
        end if;
      end if;
      fb_delay_found <= '1';          
    end if;
    wait on lock_period, rst_ipd;
  end process determine_clock_delay;
--
--  determine feedback lock
--  
  GEN_CLKFB_WINDOW : process
  begin
    if (rst_ipd'event) then
      clkfb_window <= '0';  
    else
      if (rising_edge(CLKFB_ipd)) then
        wait for 0 ps;
        clkfb_window <= '1';
        wait for cycle_jitter;        
        clkfb_window <= '0';
      end if;          
    end if;      
    wait on clkfb_ipd, rst_ipd;
  end process GEN_CLKFB_WINDOW;

  GEN_CLKIN_WINDOW : process
  begin
    if (rst_ipd'event) then
      clkin_window <= '0';
    else
      if (rising_edge(clkin_fb)) then
        wait for 0 ps;
        clkin_window <= '1';
        wait for cycle_jitter;        
        clkin_window <= '0';
      end if;          
    end if;      
    wait on clkin_fb, rst_ipd;
  end process GEN_CLKIN_WINDOW;  

  set_reset_lock_clkin : process
  begin
    if (rst_ipd'event) then
      lock_clkin <= '0';                  
    else
      if (rising_edge(clkin_fb)) then
        wait for 1 ps;
        if ((clkfb_window = '1') and (fb_delay_found = '1')) then
          lock_clkin <= '1';
        else
          lock_clkin <= '0';        
        end if;
      end if;          
    end if;
    wait on clkin_fb, rst_ipd;
  end process set_reset_lock_clkin;

  set_reset_lock_clkfb : process
  begin
    if (rst_ipd'event) then
      lock_clkfb <= '0';                  
    else
      if (rising_edge(clkfb_ipd)) then
        wait for 1 ps;
        if ((clkin_window = '1') and (fb_delay_found = '1')) then
          lock_clkfb <= '1';
        else
          lock_clkfb <= '0';        
        end if;
      end if;          
    end if;
    wait on clkfb_ipd, rst_ipd;
  end process set_reset_lock_clkfb;

  assign_lock_delay : process
  begin
    if (rst_ipd'event) then
      lock_delay <= '0';          
    else
      if (falling_edge(clkin_fb)) then
        lock_delay <= lock_clkin or lock_clkfb;
      end if;
    end if;
    wait on clkin_fb, rst_ipd;    
  end process;

--
--generate lock signal
--  
  
  generate_lock : process
  begin
    if (rst_ipd'event) then
      lock_out <= "00";
      locked_out <= '0';      
    else
      if (rising_edge(clkin_ps)) then
        if (clkfb_type = 0) then
          lock_out(0) <= lock_period;
        else
          lock_out(0) <= lock_period and lock_delay and lock_fb;
        end if;                           
          lock_out(1) <= lock_out(0);
          locked_out <= lock_out(1);          

      end if;
    end if;
    wait on clkin_ps, rst_ipd;
  end process generate_lock;  

--
--generate the clk1x_out
--  
  
  gen_clk1x : process
  begin
    if (rst_ipd'event) then
      clkin_5050 <= '0';
    else
      if (rising_edge(clkin_ps)) then
        clkin_5050 <= '1';
        wait for (period/2);
        clkin_5050 <= '0';          
      end if;          
    end if;
    wait on clkin_ps, rst_ipd;
  end process gen_clk1x;
  
  clk0_out <= clkin_5050 when (clk1x_type = 1) else clkin_ps;

--
--generate the clk2x_out
--    

  gen_clk2x : process
  begin
    
    if (rising_edge(clkin_ps)) then
      clk2x_out <= '1';
      wait for (period / 4);
      clk2x_out <= '0';
      if (lock_out(0) = '1') then
        wait for (period / 4);
        clk2x_out <= '1';
        wait for (period / 4);
        clk2x_out <= '0';
      else 
        wait for (period / 2);
      end if;
    end if;
    wait on clkin_ps;
  end process gen_clk2x;
  
-- 
--generate the clkdv_out
-- 

  determine_clkdv_period : process
  begin
    if (period'event) then
      if (dll_mode_type = 1) then
        period_dv_high <= (period / 2) * (divide_type / 2);
        period_dv_low <= (period / 2) * (divide_type / 2 + divide_type mod 2);
      else 
        period_dv_high <= (period * divide_type) / 4;
        period_dv_low <= (period * divide_type) / 4;
      end if;      
    end if;
    wait on period;
  end process determine_clkdv_period;

  
  gen_clkdv : process
  begin
    if (rising_edge(clkin_ps)) then
      if (lock_out(0) = '1') then
        clkdv_out <= '1';
        wait for (period_dv_high);
        clkdv_out <= '0';
        wait for (period_dv_low);        
        clkdv_out <= '1';
        wait for (period_dv_high);        
        clkdv_out <= '0';
        wait for (period_dv_low - period/2);
      end if;    
    end if;
    wait on clkin_ps;
  end process gen_clkdv;

--
-- generate fx output signal
--
  
  calculate_period_fx : process
  begin
    if (rising_edge(rst_ipd)) then
      period_fx <= 0 ps;
    else
      if (lock_period = '1') then
        period_fx <= (period * denominator) / (numerator * 2);
        remain_fx_temp <= ((period/1 ps) * denominator) mod (numerator * 2);        
      end if;
    end if;
    wait on lock_period, period, denominator, numerator, rst_ipd;
  end process calculate_period_fx;

    remain_fx <= remain_fx_temp * 1 ps;  
  
  generate_clkfx : process
    variable a : integer := 0;
    variable temp : integer;
  begin
    if (rst_ipd = '1') then
      clkfx_out <= '0';
    elsif (rising_edge(clkin_lost_out)) then
      wait until (falling_edge(rst_reg(2)));        
    elsif (rising_edge(clkin_ps)) then
      if (lock_out(0) = '1') then
        a := 0;
        clkfx_out <= '1';
        temp := numerator * 2 - 1 - 1;
        for p in 0 to temp loop
          wait for (period_fx);
          if (a < (remain_fx/1 ps * 1)) then
            wait for 1 ps;
          end if;
          a := a + 1;
          clkfx_out <= not clkfx_out;
        end loop;
        if (period_fx > (period / 2)) then
          wait for (period_fx - (period / 2));
        end if;
      end if;
    end if;
    wait on clkin_ps, clkin_lost_out, rst_ipd;
  end process generate_clkfx;

  set_reset_clkfx180_en : process
    begin
      if ((rising_edge(clkfx_out)) or (rising_edge(rst_ipd))) then
        if (rst_ipd = '1') then
          clkfx180_en <= '0';          
        else
          clkfx180_en <= '1';                    
        end if;
      end if;
      wait on clkfx_out, rst_ipd;
    end process;
    
--
--generate all output signal
--
  CLKFX180 <= transport ((not clkfx_out) and clkfx180_en) after clkout_delay;        
  schedule_outputs : process
    variable PSDONE_GlitchData : VitalGlitchDataType;
    variable LOCKED_GlitchData : VitalGlitchDataType;
  begin
    if (CLK0_out'event) then
      if (clkfb_type /= 0) then
        CLK0 <= transport CLK0_out after clkout_delay;
        clk0_temp <= transport CLK0_out after clkout_delay; 
      end if;                 
      if ((dll_mode_type = 0) and (clkfb_type /= 0)) then
        CLK90 <= transport clk0_out after (clkout_delay + period / 4);
      end if;
      if (clkfb_type /= 0) then        
        CLK180 <= transport clk0_out after (clkout_delay + period / 2);
      end if;
      if ((dll_mode_type = 0) and (clkfb_type /= 0)) then        
        CLK270 <= transport clk0_out after (clkout_delay + (3 * period) / 4);
      end if;          
    end if;

    if (clk2x_out'event) then
      if ((dll_mode_type = 0) and (clkfb_type /= 0)) then
        CLK2X <= transport clk2x_out after clkout_delay;
        clk2x_temp <= transport clk2x_out after clkout_delay;  
      end if;
      if ((dll_mode_type = 0) and (clkfb_type /= 0)) then
        CLK2X180 <= transport clk2x_out after (clkout_delay + period/4);  
      end if;        
    end if;

    if (clkdv_out'event) then
      if (clkfb_type /= 0) then                
        CLKDV <= transport clkdv_out after clkout_delay;
      end if;
    end if;

     if (clkfx_out'event) then
       CLKFX <= transport clkfx_out after clkout_delay;               

     end if;
    
    if (status_out(0)'event) then
      status(0) <= status_out(0);
    end if;

    if (status_out(1)'event) then
      status(1) <= status_out(1);
    end if;

    if (status_out(2)'event) then
      status(2) <= status_out(2);
    end if;
    
    VitalPathDelay01 (
      OutSignal  => PSDONE,
      GlitchData => PSDONE_GlitchData,
      OutSignalName => "PSDONE",
      OutTemp => psdone_out,
      Paths => (0 => (psdone_out'last_event, tpd_PSCLK_PSDONE, true)),
      Mode => OnEvent,
      Xon => Xon,
      MsgOn => MsgOn,
      MsgSeverity => warning
      );
    
    VitalPathDelay01 (
      OutSignal  => LOCKED,
      GlitchData => LOCKED_GlitchData,
      OutSignalName => "LOCKED",
      OutTemp => locked_out,
      Paths => (0 => (locked_out'last_event, tpd_CLKIN_LOCKED, true)),
      Mode => OnEvent,
      Xon => Xon,
      MsgOn => MsgOn,
      MsgSeverity => warning
      );
    wait on clk0_out, clk2x_out, clkdv_out, clkfx_out, locked_out, psdone_out, status_out;
  end process schedule_outputs;

  assign_status_out : process
    begin
      if (rst_ipd = '1') then
        status_out(0) <= '0';
        status_out(1) <= '0';
        status_out(2) <= '0';
      elsif (ps_overflow_out'event) then
        status_out(0) <= ps_overflow_out;
      elsif (clkin_lost_out'event) then  
        status_out(1) <= clkin_lost_out;
      elsif (clkfx_lost_out'event) then    
        status_out(2) <= clkfx_lost_out;
      end if;
      wait on clkin_lost_out, clkfx_lost_out, ps_overflow_out, rst_ipd;
    end process assign_status_out;


  VitalTimingCheck : process
    variable Tviol_PSINCDEC_PSCLK_posedge : std_ulogic := '0';
    variable Tmkr_PSINCDEC_PSCLK_posedge  : VitalTimingDataType := VitalTimingDataInit;
    variable Tviol_PSEN_PSCLK_posedge        : std_ulogic := '0';
    variable Tmkr_PSEN_PSCLK_posedge : VitalTimingDataType := VitalTimingDataInit;
    variable Pviol_CLKIN   : std_ulogic := '0';
    variable PInfo_CLKIN   : VitalPeriodDataType := VitalPeriodDataInit;   
    variable Pviol_PSCLK   : std_ulogic := '0';
    variable PInfo_PSCLK   : VitalPeriodDataType := VitalPeriodDataInit;
    variable Pviol_RST   : std_ulogic := '0';
    variable PInfo_RST   : VitalPeriodDataType := VitalPeriodDataInit;
    
  begin
    if (TimingChecksOn) then
      VitalSetupHoldCheck (
        Violation               => Tviol_PSINCDEC_PSCLK_posedge,
        TimingData              => Tmkr_PSINCDEC_PSCLK_posedge,
        TestSignal              => PSINCDEC_ipd,
        TestSignalName          => "PSINCDEC",
        TestDelay               => 0 ns,
        RefSignal               => PSCLK_ipd,
        RefSignalName          => "PSCLK",
        RefDelay                => 0 ns,
        SetupHigh               => tsetup_PSINCDEC_PSCLK_posedge_posedge,
        SetupLow                => tsetup_PSINCDEC_PSCLK_negedge_posedge,
        HoldLow                => thold_PSINCDEC_PSCLK_posedge_posedge,
        HoldHigh                 => thold_PSINCDEC_PSCLK_negedge_posedge,
        CheckEnabled            => (TO_X01(((NOT RST_ipd)) AND (PSEN_ipd)) /= '0'),
        RefTransition           => 'R',
        HeaderMsg               => InstancePath & "/DCM",
        Xon                     => Xon,
        MsgOn                   => MsgOn,
        MsgSeverity             => warning);

      VitalSetupHoldCheck (
        Violation               => Tviol_PSEN_PSCLK_posedge,
        TimingData              => Tmkr_PSEN_PSCLK_posedge,
        TestSignal              => PSEN_ipd, 
        TestSignalName          => "PSEN",
        TestDelay               => 0 ns,
        RefSignal               => PSCLK_ipd,
        RefSignalName          => "PSCLK",
        RefDelay                => 0 ns,
        SetupHigh               => tsetup_PSEN_PSCLK_posedge_posedge,
        SetupLow                => tsetup_PSEN_PSCLK_negedge_posedge,
        HoldLow                => thold_PSEN_PSCLK_posedge_posedge,
        HoldHigh                 => thold_PSEN_PSCLK_negedge_posedge,
        CheckEnabled            => TO_X01(NOT RST_ipd)  /= '0',
        RefTransition           => 'R',
        HeaderMsg               => InstancePath & "/DCM",
        Xon                     => Xon,
        MsgOn                   => MsgOn,
        MsgSeverity             => warning);

      VitalPeriodPulseCheck (
        Violation               => Pviol_PSCLK,
        PeriodData              => PInfo_PSCLK,
        TestSignal              => PSCLK_ipd,
        TestSignalName          => "PSCLK",
        TestDelay               => 0 ns,
        Period                  => tperiod_PSCLK_POSEDGE,
        PulseWidthHigh          => tpw_PSCLK_posedge,
        PulseWidthLow           => tpw_PSCLK_negedge,
        CheckEnabled            => true,
        HeaderMsg               => InstancePath &"/DCM",
        Xon                     => Xon,
        MsgOn                   => MsgOn,
        MsgSeverity             => warning);

      VitalPeriodPulseCheck (
        Violation               => Pviol_CLKIN,
        PeriodData              => PInfo_CLKIN,
        TestSignal              => CLKIN_ipd, 
        TestSignalName          => "CLKIN",
        TestDelay               => 0 ns,
        Period                  => tperiod_CLKIN_POSEDGE,
        PulseWidthHigh          => tpw_CLKIN_posedge,
        PulseWidthLow           => tpw_CLKIN_negedge,
        CheckEnabled            => TO_X01(NOT RST_ipd)  /= '0',
        HeaderMsg               => InstancePath &"/DCM",
        Xon                     => Xon,
        MsgOn                   => MsgOn,
        MsgSeverity             => warning);         

      VitalPeriodPulseCheck (
        Violation               => Pviol_RST,
        PeriodData              => PInfo_RST,
        TestSignal              => RST_ipd, 
        TestSignalName          => "RST",
        TestDelay               => 0 ns,
        Period                  => 0 ns,
        PulseWidthHigh          => tpw_RST_posedge,
        PulseWidthLow           => 0 ns,
        CheckEnabled            => true,
        HeaderMsg               => InstancePath &"/DCM",
        Xon                     => Xon,
        MsgOn                   => MsgOn,
        MsgSeverity             => warning);
    end if;
    wait on PSINCDEC_ipd, PSCLK_ipd, PSEN_ipd, CLKIN_ipd, RST_ipd;
  end process VITALTimingCheck;
end DCM_V;

----- CELL CLKDLL                     -----
----- x_clkdll_maximum_period_check     -----
library IEEE;
use IEEE.STD_LOGIC_1164.all;

library STD;
use STD.TEXTIO.all;

entity x_clkdll_maximum_period_check is
  generic (
    InstancePath : string := "*";

    clock_name : string := "";
    maximum_period : time);
  port(
    clock : in std_ulogic
    );
end x_clkdll_maximum_period_check;

architecture x_clkdll_maximum_period_check_V of x_clkdll_maximum_period_check is
begin

  MAX_PERIOD_CHECKER : process
    variable clock_edge_previous : time := 0 ps;
    variable clock_edge_current : time := 0 ps;
    variable clock_period : time := 0 ps;
    variable Message : line;
  begin

    clock_edge_previous := clock_edge_current;
    clock_edge_current := NOW;

    if (clock_edge_previous > 0 ps) then
      clock_period := clock_edge_current - clock_edge_previous;
    end if;

    if (clock_period > maximum_period) then
      Write ( Message, string'(" Timing Violation Error : Input Clock Period of"));
      Write ( Message, clock_period/1000.0 );
      Write ( Message, string'(" on the ") );
      Write ( Message, clock_name );
      Write ( Message, string'(" port ") );
      Write ( Message, string'(" of CLKDLL instance ") );
      Write ( Message, InstancePath );
      Write ( Message, string'(" exceeds allotted value of ") );
      Write ( Message, maximum_period/1000.0 );
      Write ( Message, string'(" at simulation time ") );
      Write ( Message, clock_edge_current/1000.0 );
      Write ( Message, '.' & LF );
      assert false report Message.all severity warning;
      DEALLOCATE (Message);
    end if;
    wait on clock;
  end process MAX_PERIOD_CHECKER;
end x_clkdll_maximum_period_check_V;

----- CLKDLL  -----
library IEEE;
use IEEE.std_logic_1164.all;

library STD;
use STD.TEXTIO.all;

library IEEE;
use Ieee.Vital_Primitives.all;
use Ieee.Vital_Timing.all;
library unisim;
use unisim.simple_simprim_pck.all;

entity CLKDLL is
  generic (
    TimingChecksOn : boolean := true;
    InstancePath : string := "*";
    Xon : boolean := true;
    MsgOn : boolean := false;

    tipd_CLKFB : VitalDelayType01 := (0.000 ns, 0.000 ns);
    tipd_CLKIN : VitalDelayType01 := (0.000 ns, 0.000 ns);
    tipd_RST : VitalDelayType01 := (0.000 ns, 0.000 ns);

    tpd_CLKIN_LOCKED : VitalDelayType01 := (0.000 ns, 0.000 ns);

    tperiod_CLKIN_POSEDGE : VitalDelayType := 0.000 ns;

    tpw_CLKIN_negedge : VitalDelayType := 0.000 ns;
    tpw_CLKIN_posedge : VitalDelayType := 0.000 ns;
    tpw_RST_posedge : VitalDelayType := 0.000 ns;

    CLKDV_DIVIDE : real := 2.0;
    DUTY_CYCLE_CORRECTION : boolean := true;
    FACTORY_JF : bit_vector := X"C080";  --non-simulatable
    MAXPERCLKIN : time := 40000 ps;  --simulation parameter
    SIM_CLKIN_CYCLE_JITTER : time := 300 ps;  --simulation parameter
    SIM_CLKIN_PERIOD_JITTER : time := 1000 ps;  --simulation parameter
    STARTUP_WAIT : boolean := false  --non-simulatable
    );

  port (
    CLK0 : out std_ulogic := '0';
    CLK180 : out std_ulogic := '0';
    CLK270 : out std_ulogic := '0';
    CLK2X : out std_ulogic := '0';
    CLK90 : out std_ulogic := '0';
    CLKDV : out std_ulogic := '0';
    LOCKED : out std_ulogic := '0';

    CLKFB : in std_ulogic := '0';
    CLKIN : in std_ulogic := '0';
    RST : in std_ulogic := '0'
    );

  attribute VITAL_LEVEL0 of CLKDLL : entity is true;

end CLKDLL;

architecture CLKDLL_V of CLKDLL is

  component x_clkdll_maximum_period_check
    generic (
      InstancePath : string := "*";

      clock_name : string := "";
      maximum_period : time);
    port(
      clock : in std_ulogic
      );
  end component;

  signal CLKFB_ipd, CLKIN_ipd, RST_ipd : std_ulogic;
  signal clk0_out : std_ulogic;
  signal clk2x_out, clkdv_out, locked_out : std_ulogic := '0';

  signal clkfb_type : integer;
  signal divide_type : integer;
  signal clk1x_type : integer;

  signal lock_period, lock_delay, lock_clkin, lock_clkfb : std_ulogic := '0';
  signal lock_out : std_logic_vector(1 downto 0) := "00";

  signal lock_fb : std_ulogic := '0';
  signal fb_delay_found : std_ulogic := '0';

  signal clkin_ps : std_ulogic;
  signal clkin_fb, clkin_fb0, clkin_fb1, clkin_fb2 : std_ulogic;

  signal clkin_period_real : VitalDelayArrayType(2 downto 0) := (0.000 ns, 0.000 ns, 0.000 ns);
  signal period : time := 0 ps;
  signal period_orig : time := 0 ps;
  signal period_ps : time := 0 ps;
  signal clkout_delay : time := 0 ps;
  signal fb_delay : time := 0 ps;
  signal period_dv_high, period_dv_low : time := 0 ps;
  signal cycle_jitter, period_jitter : time := 0 ps;

  signal clkin_window, clkfb_window : std_ulogic := '0';
  signal clkin_5050 : std_ulogic := '0';
  signal rst_reg : std_logic_vector(2 downto 0) := "000";

  signal clkin_period_real0_temp : time := 0 ps;
  signal ps_lock_temp : std_ulogic := '0';

  signal clk0_temp : std_ulogic := '0';
  signal clk2x_temp : std_ulogic := '0';

  signal no_stop : boolean := false;

begin
  INITPROC : process
  begin
    detect_resolution
      (model_name => "CLKDLL"
       );
    if (CLKDV_DIVIDE = 1.5) then
      divide_type <= 3;
    elsif (CLKDV_DIVIDE = 2.0) then
      divide_type <= 4;
    elsif (CLKDV_DIVIDE = 2.5) then
      divide_type <= 5;
    elsif (CLKDV_DIVIDE = 3.0) then
      divide_type <= 6;
    elsif (CLKDV_DIVIDE = 4.0) then
      divide_type <= 8;
    elsif (CLKDV_DIVIDE = 5.0) then
      divide_type <= 10;
    elsif (CLKDV_DIVIDE = 8.0) then
      divide_type <= 16;
    elsif (CLKDV_DIVIDE = 16.0) then
      divide_type <= 32;
    else
      GenericValueCheckMessage
        (HeaderMsg => "Attribute Syntax Error",
         GenericName => "CLKDV_DIVIDE",
         EntityName => "CLKDLL",
         InstanceName => InstancePath,
         GenericValue => CLKDV_DIVIDE,
         Unit => "",
         ExpectedValueMsg => "Legal Values for this attribute are 1.5, 2.0, 2.5, 3.0, 4.0, 5.0, 8.0 or 16.0",
         ExpectedGenericValue => "",
         TailMsg => "",
         MsgSeverity => error
         );
    end if;

    clkfb_type <= 2;

    period_jitter <= SIM_CLKIN_PERIOD_JITTER;
    cycle_jitter <= SIM_CLKIN_CYCLE_JITTER;

    case DUTY_CYCLE_CORRECTION is
      when false => clk1x_type <= 0;
      when true => clk1x_type <= 1;
      when others =>
        GenericValueCheckMessage
          (HeaderMsg => "Attribute Syntax Error",
           GenericName => "DUTY_CYCLE_CORRECTION",
           EntityName => "CLKDLL",
           InstanceName => InstancePath,
           GenericValue => DUTY_CYCLE_CORRECTION,
           Unit => "",
           ExpectedValueMsg => "Legal Values for this attribute are TRUE or FALSE",
           ExpectedGenericValue => "",
           TailMsg => "",
           MsgSeverity => error
           );
    end case;

    case STARTUP_WAIT is
      when false => null;
      when true => null;
      when others =>
        GenericValueCheckMessage
          (HeaderMsg => "Attribute Syntax Error",
           GenericName => "STARTUP_WAIT",
           EntityName => "CLKDLL",
           InstanceName => InstancePath,
           GenericValue => STARTUP_WAIT,
           Unit => "",
           ExpectedValueMsg => "Legal Values for this attribute are TRUE or FALSE",
           ExpectedGenericValue => "",
           TailMsg => "",
           MsgSeverity => error
           );
    end case;
    wait;
  end process INITPROC;

--
-- input wire delays
--

  WireDelay : block
  begin
    VitalWireDelay (CLKIN_ipd, CLKIN, tipd_CLKIN);
    VitalWireDelay (CLKFB_ipd, CLKFB, tipd_CLKFB);
    VitalWireDelay (RST_ipd, RST, tipd_RST);
  end block;

  i_max_clkin : x_clkdll_maximum_period_check
    generic map (
      clock_name => "CLKIN",
      maximum_period => MAXPERCLKIN)

    port map (
      clock => clkin_ipd);

  assign_clkin_ps : process
  begin
    if (rst_ipd = '0') then
      clkin_ps <= clkin_ipd;
    elsif (rst_ipd = '1') then
      clkin_ps <= '0';      
      wait until (falling_edge(rst_reg(2)));
    end if;
    wait on clkin_ipd, rst_ipd;
  end process assign_clkin_ps;

  clkin_fb0 <= transport (clkin_ps and lock_fb) after period_ps/4;
  clkin_fb1 <= transport clkin_fb0 after period_ps/4;
  clkin_fb2 <= transport clkin_fb1 after period_ps/4;
  clkin_fb <= transport clkin_fb2 after period_ps/4;

  determine_period_ps : process
    variable clkin_ps_edge_previous : time := 0 ps;
    variable clkin_ps_edge_current : time := 0 ps;
  begin
    if (rst_ipd'event) then
      clkin_ps_edge_previous := 0 ps;
      clkin_ps_edge_current := 0 ps;
      period_ps <= 0 ps;
    else
      if (rising_edge(clkin_ps)) then
        clkin_ps_edge_previous := clkin_ps_edge_current;
        clkin_ps_edge_current := NOW;
        wait for 0 ps;
        if ((clkin_ps_edge_current - clkin_ps_edge_previous) <= (1.5 * period_ps)) then
          period_ps <= clkin_ps_edge_current - clkin_ps_edge_previous;
        elsif ((period_ps = 0 ps) and (clkin_ps_edge_previous /= 0 ps)) then
          period_ps <= clkin_ps_edge_current - clkin_ps_edge_previous;
        end if;
      end if;
    end if;
    wait on clkin_ps, rst_ipd;
  end process determine_period_ps;

  assign_lock_fb : process
  begin
    if (rising_edge(clkin_ps)) then
      lock_fb <= lock_period;
    end if;
    wait on clkin_ps;
  end process assign_lock_fb;

  calculate_clkout_delay : process
  begin
    if (rst_ipd'event) then
      clkout_delay <= 0 ps;        
    elsif (period'event or fb_delay'event) then
      clkout_delay <= period - fb_delay;
    end if;
    wait on period, fb_delay, rst_ipd;
  end process calculate_clkout_delay;

--
--generate master reset signal
--

  gen_master_rst : process
  begin
    if (rising_edge(clkin_ipd)) then
      rst_reg(2) <= rst_reg(1) and rst_reg(0) and rst_ipd;
      rst_reg(1) <= rst_reg(0) and rst_ipd;
      rst_reg(0) <= rst_ipd;
    end if;
    wait on clkin_ipd;
  end process gen_master_rst;

  check_rst_width : process
    variable Message : line;
  begin
    if (falling_edge(rst_ipd)) then
      if ((rst_reg(2) and rst_reg(1) and rst_reg(0)) = '0') then
        Write ( Message, string'(" Timing Violation Error : RST on instance "));
        Write ( Message, Instancepath );
        Write ( Message, string'(" must be asserted for 3 CLKIN clock cycles. "));
        assert false report Message.all severity error;
        DEALLOCATE (Message);
      end if;
    end if;

    wait on rst_ipd;
  end process check_rst_width;

--
--determine clock period
--
  determine_clock_period : process
    variable clkin_edge_previous : time := 0 ps;
    variable clkin_edge_current : time := 0 ps;
  begin
    if (rst_ipd'event) then
      clkin_period_real(0) <= 0 ps;
      clkin_period_real(1) <= 0 ps;
      clkin_period_real(2) <= 0 ps;
    elsif (rising_edge(clkin_ps)) then
      clkin_edge_previous := clkin_edge_current;
      clkin_edge_current := NOW;
      clkin_period_real(2) <= clkin_period_real(1);
      clkin_period_real(1) <= clkin_period_real(0);
      if (clkin_edge_previous /= 0 ps) then
        clkin_period_real(0) <= clkin_edge_current - clkin_edge_previous;
      end if;
    end if;
    if (no_stop'event) then
      clkin_period_real(0) <= clkin_period_real0_temp;
    end if;
    wait on clkin_ps, no_stop, rst_ipd;
  end process determine_clock_period;

  evaluate_clock_period : process
    variable clock_stopped : std_ulogic := '1';    
    variable Message : line;
  begin
    if (rst_ipd'event) then
      lock_period <= '0';
      clock_stopped := '1';
      clkin_period_real0_temp <= 0 ps;                              
    else
      if (falling_edge(clkin_ps)) then
        if (lock_period = '0') then
          if ((clkin_period_real(0) /= 0 ps ) and (clkin_period_real(0) - cycle_jitter <= clkin_period_real(1)) and (clkin_period_real(1) <= clkin_period_real(0) + cycle_jitter) and (clkin_period_real(1) - cycle_jitter <= clkin_period_real(2)) and (clkin_period_real(2) <= clkin_period_real(1) + cycle_jitter)) then
            lock_period <= '1';
            period_orig <= (clkin_period_real(0) + clkin_period_real(1) + clkin_period_real(2)) / 3;
            period <= clkin_period_real(0);
          end if;
        elsif (lock_period = '1') then
          if (100000000 ps < clkin_period_real(0)/1000) then
            Write ( Message, string'(" Timing Violation Error : CLKIN stopped toggling on instance "));
            Write ( Message, Instancepath );
            Write ( Message, string'(" exceeds "));
            Write ( Message, string'(" 10000 "));
            Write ( Message, string'(" Current CLKIN Period = "));
            Write ( Message, string'(" clkin_period(0) / 10000.0 "));
            Write ( Message, string'(" ns "));
            assert false report Message.all severity warning;
            DEALLOCATE (Message);
            lock_period <= '0';
            wait until (falling_edge(rst_reg(2)));
          elsif ((period_orig * 2 < clkin_period_real(0)) and (clock_stopped = '0')) then
            clkin_period_real0_temp <= clkin_period_real(1);
            no_stop <= not no_stop;
            clock_stopped := '1';            
          elsif ((clkin_period_real(0) < period_orig - period_jitter) or (period_orig + period_jitter < clkin_period_real(0))) then
            Write ( Message, string'(" Timing Violation Error : Input Clock Period Jitter on instance "));
            Write ( Message, Instancepath );
            Write ( Message, string'(" exceeds "));
            Write ( Message, period_jitter / 1000.0 );
            Write ( Message, string'(" Locked CLKIN Period = "));
            Write ( Message, period_orig / 1000.0 );
            Write ( Message, string'(" Current CLKIN Period = "));
            Write ( Message, clkin_period_real(0) / 1000.0 );
            assert false report Message.all severity warning;
            DEALLOCATE (Message);
            lock_period <= '0';
            wait until (falling_edge(rst_reg(2)));
          elsif ((clkin_period_real(0) < clkin_period_real(1) - cycle_jitter) or (clkin_period_real(1) + cycle_jitter < clkin_period_real(0))) then
            Write ( Message, string'(" Timing Violation Error : Input Clock Cycle Jitter on on instance "));
            Write ( Message, Instancepath );
            Write ( Message, string'(" exceeds "));
            Write ( Message, cycle_jitter / 1000.0 );
            Write ( Message, string'(" Previous CLKIN Period = "));
            Write ( Message, clkin_period_real(1) / 1000.0 );
            Write ( Message, string'(" Current CLKIN Period = "));
            Write ( Message, clkin_period_real(0) / 1000.0 );
            assert false report Message.all severity warning;
            DEALLOCATE (Message);
            lock_period <= '0';
            wait until (falling_edge(rst_reg(2)));
          else
            period <= clkin_period_real(0);
            clock_stopped := '0';            
          end if;
        end if;
      end if;
    end if;
    wait on clkin_ps, rst_ipd;
  end process evaluate_clock_period;

--
--determine clock delay
--

  determine_clock_delay : process
    variable delay_edge : time := 0 ps;
    variable temp1 : integer := 0;
    variable temp2 : integer := 0;
    variable temp : integer := 0;
    variable delay_edge_current : time := 0 ps;
  begin
    if (rst_ipd'event) then
      fb_delay <= 0 ps;      
      fb_delay_found <= '0';
    else
      if (rising_edge(lock_period)) then
        if ((lock_period = '1') and (clkfb_type /= 0)) then
          if (clkfb_type = 1) then
            wait until ((rising_edge(clk0_temp)) or (rst_ipd'event));                                
            delay_edge := NOW;
          elsif (clkfb_type = 2) then
            wait until ((rising_edge(clk2x_temp)) or (rst_ipd'event));            
            delay_edge := NOW;
          end if;
          wait until ((rising_edge(clkfb_ipd)) or (rst_ipd'event));          
          temp1 := ((NOW*1) - (delay_edge*1))/ (1 ps);
          temp2 := (period_orig * 1)/ (1 ps);
          temp := temp1 mod temp2;
          fb_delay <= temp * 1 ps;
        end if;
      end if;
      fb_delay_found <= '1';
    end if;
    wait on lock_period, rst_ipd;
  end process determine_clock_delay;
--
-- determine feedback lock
--
  GEN_CLKFB_WINDOW : process
  begin
    if (rst_ipd'event) then
      clkfb_window <= '0';
    else
      if (rising_edge(CLKFB_ipd)) then
        wait for 0 ps;
        clkfb_window <= '1';
        wait for cycle_jitter;
        clkfb_window <= '0';
      end if;
    end if;
    wait on clkfb_ipd, rst_ipd;
  end process GEN_CLKFB_WINDOW;

  GEN_CLKIN_WINDOW : process
  begin
    if (rst_ipd'event) then
      clkin_window <= '0';
    else
      if (rising_edge(clkin_fb)) then
        wait for 0 ps;
        clkin_window <= '1';
        wait for cycle_jitter;
        clkin_window <= '0';
      end if;
    end if;
    wait on clkin_fb, rst_ipd;
  end process GEN_CLKIN_WINDOW;

  set_reset_lock_clkin : process
  begin
    if (rst_ipd'event) then
      lock_clkin <= '0';
    else
      if (rising_edge(clkin_fb)) then
        wait for 1 ps;
        if ((clkfb_window = '1') and (fb_delay_found = '1')) then
          lock_clkin <= '1';
        else
          lock_clkin <= '0';
        end if;
      end if;
    end if;
    wait on clkin_fb, rst_ipd;
  end process set_reset_lock_clkin;

  set_reset_lock_clkfb : process
  begin
    if (rst_ipd'event) then
      lock_clkfb <= '0';
    else
      if (rising_edge(clkfb_ipd)) then
        wait for 1 ps;
        if ((clkin_window = '1') and (fb_delay_found = '1')) then
          lock_clkfb <= '1';
        else
          lock_clkfb <= '0';
        end if;
      end if;
    end if;
    wait on clkfb_ipd, rst_ipd;
  end process set_reset_lock_clkfb;

  assign_lock_delay : process
  begin
    if (rst_ipd'event) then
      lock_delay <= '0';
    else
      if (falling_edge(clkin_fb)) then
        lock_delay <= lock_clkin or lock_clkfb;
      end if;
    end if;
    wait on clkin_fb, rst_ipd;
  end process;

--
--generate lock signal
--

  generate_lock : process
  begin
    if (rst_ipd'event) then
      lock_out <= "00";
      locked_out <= '0';
    else
      if (rising_edge(clkin_ps)) then
        if (clkfb_type = 0) then
          lock_out(0) <= lock_period;
        else
          lock_out(0) <= lock_period and lock_delay and lock_fb;
        end if;
        lock_out(1) <= lock_out(0);
        locked_out <= lock_out(1);

      end if;
    end if;
    wait on clkin_ps, rst_ipd;
  end process generate_lock;

--
--generate the clk1x_out
--

  gen_clk1x : process
  begin
    if (rst_ipd'event) then
      clkin_5050 <= '0';
    else
      if (rising_edge(clkin_ps)) then
        clkin_5050 <= '1';
        wait for (period/2);
        clkin_5050 <= '0';
      end if;
    end if;
    wait on clkin_ps, rst_ipd;
  end process gen_clk1x;

  clk0_out <= clkin_5050 when (clk1x_type = 1) else clkin_ps;

--
--generate the clk2x_out
--

  gen_clk2x : process
  begin

    if (rising_edge(clkin_ps)) then
      clk2x_out <= '1';
      wait for (period / 4);
      clk2x_out <= '0';
      if (lock_out(0) = '1') then
        wait for (period / 4);
        clk2x_out <= '1';
        wait for (period / 4);
        clk2x_out <= '0';
      else
        wait for (period / 2);
      end if;
    end if;
    wait on clkin_ps;
  end process gen_clk2x;

--
--generate the clkdv_out
--

  determine_clkdv_period : process
  begin
    if (period'event) then
      period_dv_high <= (period / 2) * (divide_type / 2);
      period_dv_low <= (period / 2) * (divide_type / 2 + divide_type mod 2);
    end if;
    wait on period;
  end process determine_clkdv_period;


  gen_clkdv : process
  begin
    if (rising_edge(clkin_ps)) then
      if (lock_out(0) = '1') then
        clkdv_out <= '1';
        wait for (period_dv_high);
        clkdv_out <= '0';
        wait for (period_dv_low);
        clkdv_out <= '1';
        wait for (period_dv_high);
        clkdv_out <= '0';
        wait for (period_dv_low - period/2);
      end if;
    end if;
    wait on clkin_ps;
  end process gen_clkdv;


--
--generate all output signal
--
  schedule_outputs : process
    variable LOCKED_GlitchData : VitalGlitchDataType;
  begin
    if (CLK0_out'event) then
      CLK0 <= transport CLK0_out after clkout_delay;
      clk0_temp <= transport CLK0_out after clkout_delay;
      CLK90 <= transport clk0_out after (clkout_delay + period / 4);
      CLK180 <= transport clk0_out after (clkout_delay + period / 2);
      CLK270 <= transport clk0_out after (clkout_delay + (3 * period) / 4);
    end if;

    if (clk2x_out'event) then
      CLK2X <= transport clk2x_out after clkout_delay;
      clk2x_temp <= transport clk2x_out after clkout_delay;
    end if;

    if (clkdv_out'event) then
      CLKDV <= transport clkdv_out after clkout_delay;
    end if;

    VitalPathDelay01 (
      OutSignal => LOCKED,
      GlitchData => LOCKED_GlitchData,
      OutSignalName => "LOCKED",
      OutTemp => locked_out,
      Paths => (0 => (locked_out'last_event, tpd_CLKIN_LOCKED, true)),
      Mode => OnEvent,
      Xon => Xon,
      MsgOn => MsgOn,
      MsgSeverity => warning
      );
    wait on clk0_out, clk2x_out, clkdv_out, locked_out;
  end process schedule_outputs;

  VitalTimingCheck : process
    variable Tviol_PSINCDEC_PSCLK_posedge : std_ulogic := '0';
    variable Tmkr_PSINCDEC_PSCLK_posedge : VitalTimingDataType := VitalTimingDataInit;
    variable Tviol_PSEN_PSCLK_posedge : std_ulogic := '0';
    variable Tmkr_PSEN_PSCLK_posedge : VitalTimingDataType := VitalTimingDataInit;
    variable Pviol_CLKIN : std_ulogic := '0';
    variable PInfo_CLKIN : VitalPeriodDataType := VitalPeriodDataInit;
    variable Pviol_PSCLK : std_ulogic := '0';
    variable PInfo_PSCLK : VitalPeriodDataType := VitalPeriodDataInit;
    variable Pviol_RST : std_ulogic := '0';
    variable PInfo_RST : VitalPeriodDataType := VitalPeriodDataInit;

  begin
    if (TimingChecksOn) then
      VitalPeriodPulseCheck (
        Violation => Pviol_CLKIN,
        PeriodData => PInfo_CLKIN,
        TestSignal => CLKIN_ipd,
        TestSignalName => "CLKIN",
        TestDelay => 0 ns,
        Period => tperiod_CLKIN_POSEDGE,
        PulseWidthHigh => tpw_CLKIN_posedge,
        PulseWidthLow => tpw_CLKIN_negedge,
        CheckEnabled => TO_X01(not RST_ipd) /= '0',
        HeaderMsg => InstancePath &"/CLKDLL",
        Xon => Xon,
        MsgOn => MsgOn,
        MsgSeverity => warning);

      VitalPeriodPulseCheck (
        Violation => Pviol_RST,
        PeriodData => PInfo_RST,
        TestSignal => RST_ipd,
        TestSignalName => "RST",
        TestDelay => 0 ns,
        Period => 0 ns,
        PulseWidthHigh => tpw_RST_posedge,
        PulseWidthLow => 0 ns,
        CheckEnabled => true,
        HeaderMsg => InstancePath &"/CLKDLL",
        Xon => Xon,
        MsgOn => MsgOn,
        MsgSeverity => warning);
    end if;
    wait on CLKIN_ipd, RST_ipd;
  end process VITALTimingCheck;
end CLKDLL_V;

library IEEE;
use IEEE.STD_LOGIC_1164.all;


--use unisim.vpkg.all;
library unisim;
use unisim.simple_simprim_pck.all;

entity IDDR is

  generic(

      DDR_CLK_EDGE : string := "OPPOSITE_EDGE";
      INIT_Q1      : bit    := '0';
      INIT_Q2      : bit    := '0';
      SRTYPE       : string := "SYNC"
      );

  port(
      Q1          : out std_ulogic;
      Q2          : out std_ulogic;

      C           : in  std_ulogic;
      CE          : in  std_ulogic;
      D           : in  std_ulogic;
      R           : in  std_ulogic;
      S           : in  std_ulogic
    );

end IDDR;

architecture IDDR_V OF IDDR is


  constant SYNC_PATH_DELAY : time := 100 ps;

  signal C_ipd	        : std_ulogic := 'X';
  signal CE_ipd	        : std_ulogic := 'X';
  signal D_ipd	        : std_ulogic := 'X';
  signal GSR            : std_ulogic := '0';
  signal GSR_ipd	: std_ulogic := 'X';
  signal R_ipd		: std_ulogic := 'X';
  signal S_ipd		: std_ulogic := 'X';

  signal C_dly	        : std_ulogic := 'X';
  signal CE_dly	        : std_ulogic := 'X';
  signal D_dly	        : std_ulogic := 'X';
  signal GSR_dly	: std_ulogic := 'X';
  signal R_dly		: std_ulogic := 'X';
  signal S_dly		: std_ulogic := 'X';

  signal Q1_zd	        : std_ulogic := 'X';
  signal Q2_zd	        : std_ulogic := 'X';

  signal Q1_viol        : std_ulogic := 'X';
  signal Q2_viol        : std_ulogic := 'X';

  signal Q1_o_reg	: std_ulogic := 'X';
  signal Q2_o_reg	: std_ulogic := 'X';
  signal Q3_o_reg	: std_ulogic := 'X';
  signal Q4_o_reg	: std_ulogic := 'X';

  signal ddr_clk_edge_type	: integer := -999;
  signal sr_type		: integer := -999;
begin

  ---------------------
  --  INPUT PATH DELAYs
  --------------------

  C_dly          	 <= C              	after 0 ps;
  CE_dly         	 <= CE             	after 0 ps;
  D_dly          	 <= D              	after 0 ps;
  GSR_dly        	 <= GSR            	after 0 ps;
  R_dly          	 <= R              	after 0 ps;
  S_dly          	 <= S              	after 0 ps;

  --------------------
  --  BEHAVIOR SECTION
  --------------------

--####################################################################
--#####                     Initialize                           #####
--####################################################################
  prcs_init:process

  begin
      if((DDR_CLK_EDGE = "OPPOSITE_EDGE") or (DDR_CLK_EDGE = "opposite_edge")) then
         ddr_clk_edge_type <= 1;
      elsif((DDR_CLK_EDGE = "SAME_EDGE") or (DDR_CLK_EDGE = "same_edge")) then
         ddr_clk_edge_type <= 2;
      elsif((DDR_CLK_EDGE = "SAME_EDGE_PIPELINED") or (DDR_CLK_EDGE = "same_edge_pipelined")) then
         ddr_clk_edge_type <= 3;
      else
        GenericValueCheckMessage
          (  HeaderMsg  => " Attribute Syntax Warning ",
             GenericName => " DDR_CLK_EDGE ",
             EntityName => "/IDDR",
             GenericValue => DDR_CLK_EDGE,
             Unit => "",
             ExpectedValueMsg => " The Legal values for this attribute are ",
             ExpectedGenericValue => " OPPOSITE_EDGE or SAME_EDGE or  SAME_EDGE_PIPELINED.",
             TailMsg => "",
             MsgSeverity => ERROR 
         );
      end if;

      if((SRTYPE = "ASYNC") or (SRTYPE = "async")) then
         sr_type <= 1;
      elsif((SRTYPE = "SYNC") or (SRTYPE = "sync")) then
         sr_type <= 2;
      else
        GenericValueCheckMessage
          (  HeaderMsg  => " Attribute Syntax Warning ",
             GenericName => " SRTYPE ",
             EntityName => "/IDDR",
             GenericValue => SRTYPE,
             Unit => "",
             ExpectedValueMsg => " The Legal values for this attribute are ",
             ExpectedGenericValue => " ASYNC or SYNC. ",
             TailMsg => "",
             MsgSeverity => ERROR
         );
      end if;

     wait;
  end process prcs_init;
--####################################################################
--#####                    q1_q2_q3_q4 reg                       #####
--####################################################################
  prcs_q1q2q3q4_reg:process(C_dly, D_dly, GSR_dly, R_dly, S_dly)
  variable Q1_var : std_ulogic := TO_X01(INIT_Q1);
  variable Q2_var : std_ulogic := TO_X01(INIT_Q2);
  variable Q3_var : std_ulogic := TO_X01(INIT_Q1);
  variable Q4_var : std_ulogic := TO_X01(INIT_Q2);
  begin
     if(GSR_dly = '1') then
         Q1_var := TO_X01(INIT_Q1);
         Q3_var := TO_X01(INIT_Q1);
         Q2_var := TO_X01(INIT_Q2);
         Q4_var := TO_X01(INIT_Q2);
     elsif(GSR_dly = '0') then
        case sr_type is
           when 1 => 
                   if(R_dly = '1') then
                      Q1_var := '0';
                      Q2_var := '0';
                      Q3_var := '0';
                      Q4_var := '0';
                   elsif((R_dly = '0') and (S_dly = '1')) then
                      Q1_var := '1';
                      Q2_var := '1';
                      Q3_var := '1';
                      Q4_var := '1';
                   elsif((R_dly = '0') and (S_dly = '0')) then
                      if(CE_dly = '1') then
                         if(rising_edge(C_dly)) then
                            Q3_var := Q1_var;
                            Q1_var := D_dly;
                            Q4_var := Q2_var;
                         end if;
                         if(falling_edge(C_dly)) then
                            Q2_var := D_dly;
                         end if;
                      end if;
                   end if;

           when 2 => 
                   if(rising_edge(C_dly)) then
                      if(R_dly = '1') then
                         Q1_var := '0';
                         Q3_var := '0';
                         Q4_var := '0';
                      elsif((R_dly = '0') and (S_dly = '1')) then
                         Q1_var := '1';
                         Q3_var := '1';
                         Q4_var := '1';
                      elsif((R_dly = '0') and (S_dly = '0')) then
                         if(CE_dly = '1') then
                               Q3_var := Q1_var;
                               Q1_var := D_dly;
                               Q4_var := Q2_var;
                         end if;
                      end if;
                   end if;
                        
                   if(falling_edge(C_dly)) then
                      if(R_dly = '1') then
                         Q2_var := '0';
                      elsif((R_dly = '0') and (S_dly = '1')) then
                         Q2_var := '1';
                      elsif((R_dly = '0') and (S_dly = '0')) then
                         if(CE_dly = '1') then
                               Q2_var := D_dly;
                         end if;
                      end if;
                   end if;
 
           when others =>
                   null; 
        end case;
     end if;

     q1_o_reg <= Q1_var;
     q2_o_reg <= Q2_var;
     q3_o_reg <= Q3_var;
     q4_o_reg <= Q4_var;

  end process prcs_q1q2q3q4_reg;
--####################################################################
--#####                        q1 & q2  mux                      #####
--####################################################################
  prcs_q1q2_mux:process(q1_o_reg, q2_o_reg, q3_o_reg, q4_o_reg)
  begin
     case ddr_clk_edge_type is
        when 1 => 
                 Q1_zd <= q1_o_reg;
                 Q2_zd <= q2_o_reg;
        when 2 => 
                 Q1_zd <= q1_o_reg;
                 Q2_zd <= q4_o_reg;
       when 3 => 
                 Q1_zd <= q3_o_reg;
                 Q2_zd <= q4_o_reg;
       when others =>
                 null;
     end case;
  end process prcs_q1q2_mux;
--####################################################################

--####################################################################
--#####                         OUTPUT                           #####
--####################################################################
  prcs_output:process(Q1_zd, Q2_zd)
  begin
      Q1 <= Q1_zd after SYNC_PATH_DELAY;
      Q2 <= Q2_zd after SYNC_PATH_DELAY;
  end process prcs_output;
--####################################################################


end IDDR_V;


library IEEE;
use IEEE.STD_LOGIC_1164.all;

--use unisim.vpkg.all;

library unisim;
use unisim.simple_simprim_pck.all;

entity ODDR is

  generic(

      DDR_CLK_EDGE : string := "OPPOSITE_EDGE";
      INIT         : bit    := '0';
      SRTYPE       : string := "SYNC"
      );

  port(
      Q           : out std_ulogic;

      C           : in  std_ulogic;
      CE          : in  std_ulogic;
      D1          : in  std_ulogic;
      D2          : in  std_ulogic;
      R           : in  std_ulogic;
      S           : in  std_ulogic
    );

end ODDR;

architecture ODDR_V OF ODDR is


  constant SYNC_PATH_DELAY : time := 100 ps;

  signal C_ipd	        : std_ulogic := 'X';
  signal CE_ipd	        : std_ulogic := 'X';
  signal D1_ipd	        : std_ulogic := 'X';
  signal D2_ipd	        : std_ulogic := 'X';
  signal GSR            : std_ulogic := '0';
  signal GSR_ipd	: std_ulogic := 'X';
  signal R_ipd		: std_ulogic := 'X';
  signal S_ipd		: std_ulogic := 'X';

  signal C_dly	        : std_ulogic := 'X';
  signal CE_dly	        : std_ulogic := 'X';
  signal D1_dly	        : std_ulogic := 'X';
  signal D2_dly	        : std_ulogic := 'X';
  signal GSR_dly	: std_ulogic := 'X';
  signal R_dly		: std_ulogic := 'X';
  signal S_dly		: std_ulogic := 'X';

  signal Q_zd		: std_ulogic := 'X';

  signal Q_viol		: std_ulogic := 'X';

  signal ddr_clk_edge_type	: integer := -999;
  signal sr_type		: integer := -999;

begin

  ---------------------
  --  INPUT PATH DELAYs
  --------------------

  C_dly          	 <= C              	after 0 ps;
  CE_dly         	 <= CE             	after 0 ps;
  D1_dly         	 <= D1             	after 0 ps;
  D2_dly         	 <= D2             	after 0 ps;
  GSR_dly        	 <= GSR            	after 0 ps;
  R_dly          	 <= R              	after 0 ps;
  S_dly          	 <= S              	after 0 ps;

  --------------------
  --  BEHAVIOR SECTION
  --------------------

--####################################################################
--#####                     Initialize                           #####
--####################################################################
  prcs_init:process

  begin
      if((DDR_CLK_EDGE = "OPPOSITE_EDGE") or (DDR_CLK_EDGE = "opposite_edge")) then
         ddr_clk_edge_type <= 1;
      elsif((DDR_CLK_EDGE = "SAME_EDGE") or (DDR_CLK_EDGE = "same_edge")) then
         ddr_clk_edge_type <= 2;
      else
        GenericValueCheckMessage
          (  HeaderMsg  => " Attribute Syntax Warning ",
             GenericName => " DDR_CLK_EDGE ",
             EntityName => "/ODDR",
             GenericValue => DDR_CLK_EDGE,
             Unit => "",
             ExpectedValueMsg => " The Legal values for this attribute are ",
             ExpectedGenericValue => " OPPOSITE_EDGE or SAME_EDGE.",
             TailMsg => "",
             MsgSeverity => ERROR 
         );
      end if;

      if((SRTYPE = "ASYNC") or (SRTYPE = "async")) then
         sr_type <= 1;
      elsif((SRTYPE = "SYNC") or (SRTYPE = "sync")) then
         sr_type <= 2;
      else
        GenericValueCheckMessage
          (  HeaderMsg  => " Attribute Syntax Warning ",
             GenericName => " SRTYPE ",
             EntityName => "/ODDR",
             GenericValue => SRTYPE,
             Unit => "",
             ExpectedValueMsg => " The Legal values for this attribute are ",
             ExpectedGenericValue => " ASYNC or SYNC. ",
             TailMsg => "",
             MsgSeverity => ERROR
         );
      end if;

     wait;
  end process prcs_init;
--####################################################################
--#####                       q1_q2_q3 reg                       #####
--####################################################################
  prcs_q1q2q3_reg:process(C_dly, GSR_dly, R_dly, S_dly)
  variable Q1_var         : std_ulogic := TO_X01(INIT);
  variable Q2_posedge_var : std_ulogic := TO_X01(INIT);
  begin
     if(GSR_dly = '1') then
         Q1_var         := TO_X01(INIT);
         Q2_posedge_var := TO_X01(INIT);
     elsif(GSR_dly = '0') then
        case sr_type is
           when 1 => 
                   if(R_dly = '1') then
                      Q1_var := '0';
                      Q2_posedge_var := '0';
                   elsif((R_dly = '0') and (S_dly = '1')) then
                      Q1_var := '1';
                      Q2_posedge_var := '1';
                   elsif((R_dly = '0') and (S_dly = '0')) then
                      if(CE_dly = '1') then
                         if(rising_edge(C_dly)) then
                            Q1_var         := D1_dly;
                            Q2_posedge_var := D2_dly;
                         end if;
                         if(falling_edge(C_dly)) then
                             case ddr_clk_edge_type is
                                when 1 => 
                                       Q1_var :=  D2_dly;
                                when 2 => 
                                       Q1_var :=  Q2_posedge_var;
                                when others =>
                                          null;
                              end case;
                         end if;
                      end if;
                   end if;

           when 2 => 
                   if(rising_edge(C_dly)) then
                      if(R_dly = '1') then
                         Q1_var := '0';
                         Q2_posedge_var := '0';
                      elsif((R_dly = '0') and (S_dly = '1')) then
                         Q1_var := '1';
                         Q2_posedge_var := '1';
                      elsif((R_dly = '0') and (S_dly = '0')) then
                         if(CE_dly = '1') then
                            Q1_var         := D1_dly;
                            Q2_posedge_var := D2_dly;
                         end if;
                      end if;
                   end if;
                        
                   if(falling_edge(C_dly)) then
                      if(R_dly = '1') then
                         Q1_var := '0';
                      elsif((R_dly = '0') and (S_dly = '1')) then
                         Q1_var := '1';
                      elsif((R_dly = '0') and (S_dly = '0')) then
                         if(CE_dly = '1') then
                             case ddr_clk_edge_type is
                                when 1 => 
                                       Q1_var :=  D2_dly;
                                when 2 => 
                                       Q1_var :=  Q2_posedge_var;
                                when others =>
                                          null;
                              end case;
                         end if;
                      end if;
                   end if;
 
           when others =>
                   null; 
        end case;
     end if;

     Q_zd <= Q1_var;

  end process prcs_q1q2q3_reg;
--####################################################################

--####################################################################
--#####                         OUTPUT                           #####
--####################################################################
  prcs_output:process(Q_zd)
  begin
      Q <= Q_zd after SYNC_PATH_DELAY;
  end process prcs_output;
--####################################################################


end ODDR_V;

library IEEE;
use IEEE.STD_LOGIC_1164.all;

library UNISIM;
use UNISIM.VCOMPONENTS.all;

entity OFDDRRSE is
  port(
    Q : out std_ulogic;

    C0 : in std_ulogic;
    C1 : in std_ulogic;
    CE : in std_ulogic;
    D0 : in std_ulogic;
    D1 : in std_ulogic;
    R  : in std_ulogic;
    S  : in std_ulogic
    );
end OFDDRRSE;

architecture OFDDRRSE_V of OFDDRRSE is

  signal Q_out : std_ulogic := 'X';

begin
  O1 : OBUF
    port map (
      I => Q_out,
      O => Q
      );

  F0 : FDDRRSE
    generic map (INIT => '0'
                 )

    port map (
      C0 => C0,
      C1 => C1,
      CE => CE,
      R  => R,
      D0 => D0,
      D1 => D1,
      S  => S,
      Q  => Q_out
      );
end OFDDRRSE_V;

library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity FDDRRSE is
  generic(
    INIT : bit := '0'
    );

  port(
    Q : out std_ulogic;

    C0 : in std_ulogic;
    C1 : in std_ulogic;
    CE : in std_ulogic;
    D0 : in std_ulogic;
    D1 : in std_ulogic;
    R  : in std_ulogic;
    S  : in std_ulogic
    );
end FDDRRSE;

architecture FDDRRSE_V of FDDRRSE is
begin

  VITALBehavior         : process(C0, C1)
    variable FIRST_TIME : boolean := true;
  begin

    if (FIRST_TIME) then
      Q <= TO_X01(INIT);
      FIRST_TIME := false ;
    end if;

    if ( rising_edge(C0) = true) then
      if (R = '1') then
        Q <= '0' after 100 ps;
      elsif (S = '1' ) then
        Q <= '1' after 100 ps;
      elsif (CE = '1' ) then
        Q <= D0 after 100 ps;
      end if;
    elsif (rising_edge(C1) = true ) then
      if (R = '1') then
        Q <= '0' after 100 ps;
      elsif (S = '1' ) then
        Q <= '1' after 100 ps;
      elsif (CE = '1') then
        Q <= D1 after 100 ps;
      end if;
    end if;
  end process VITALBehavior;
end FDDRRSE_V;


library IEEE;
use IEEE.STD_LOGIC_1164.all;

library UNISIM;
use UNISIM.VCOMPONENTS.all;

entity IFDDRRSE is
  port(
    Q0 : out std_ulogic;
    Q1 : out std_ulogic;

    C0 : in std_ulogic;
    C1 : in std_ulogic;
    CE : in std_ulogic;
    D  : in std_ulogic;
    R  : in std_ulogic;
    S  : in std_ulogic
    );
end IFDDRRSE;

architecture IFDDRRSE_V of IFDDRRSE is
  signal D_in : std_ulogic := 'X';
begin
  I1          : IBUF
    port map (
      I => D,
      O => D_in
      );

  F0 : FDRSE
    generic map (
      INIT => '0')
    port map (
      C    => C0,
      CE   => CE,
      R    => R,
      D    => D_in,
      S    => S,
      Q    => Q0
      );

  F1 : FDRSE
    generic map (
      INIT => '0')
    port map (
      C    => C1,
      CE   => CE,
      R    => R,
      D    => D,
      S    => S,
      Q    => Q1
      );
end IFDDRRSE_V;

library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity FDRSE is
  generic(
    INIT : bit := '0'
    );

  port(
    Q : out std_ulogic;

    C  : in std_ulogic;
    CE : in std_ulogic;
    D  : in std_ulogic;
    R  : in std_ulogic;
    S  : in std_ulogic
    );
end FDRSE;

architecture FDRSE_V of FDRSE is
begin
  VITALBehavior         : process(C)
    variable FIRST_TIME : boolean := true ;
  begin

    if (FIRST_TIME = true) then
      Q <= TO_X01(INIT);
      FIRST_TIME := false;
    end if;

    if (rising_edge(C)) then
      if (R = '1') then
        Q <= '0' after 100 ps;
      elsif (S = '1') then
        Q <= '1' after 100 ps;
      elsif (CE = '1') then
        Q <= D after 100 ps;
      end if;
    end if;
  end process;
end FDRSE_V;

library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity FD is
  generic(
    INIT : bit := '0'
    );

  port(
    Q : out std_ulogic;

    C : in std_ulogic;
    D : in std_ulogic
    );
end FD;

architecture FD_V of FD is
begin

  VITALBehavior : process(C)
    variable FIRST_TIME : boolean := true ;    
  begin
    if (FIRST_TIME = true) then
      Q <= TO_X01(INIT);
      FIRST_TIME := false;
    end if;
    
    if (rising_edge(C)) then
      Q <= D after 100 ps;
    end if;
  end process;
end FD_V;

library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity FDR is
  generic(
    INIT : bit := '0'
    );

  port(
    Q : out std_ulogic;

    C : in std_ulogic;
    D : in std_ulogic;
    R : in std_ulogic
    );
end FDR;

architecture FDR_V of FDR is
begin
  VITALBehavior         : process(C)
    variable FIRST_TIME : boolean := true ;
  begin

    if (FIRST_TIME = true) then
      Q <= TO_X01(INIT);
      FIRST_TIME := false;
    end if;

    if (rising_edge(C)) then
      if (R = '1') then
        Q <= '0' after 100 ps;
      else
        Q <= D after 100 ps;
      end if;
    end if;
  end process;
end FDR_V;

library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity FDRE is
  generic(
    INIT : bit := '0'
    );

  port(
    Q : out std_ulogic;

    C  : in std_ulogic;
    CE : in std_ulogic;
    D  : in std_ulogic;
    R  : in std_ulogic
    );
end FDRE;

architecture FDRE_V of FDRE is
begin
  VITALBehavior         : process(C)
    variable FIRST_TIME : boolean := true ;
  begin

    if (FIRST_TIME = true) then
      Q <= TO_X01(INIT);
      FIRST_TIME := false;
    end if;

    if (rising_edge(C)) then
      if (R = '1') then
        Q <= '0' after 100 ps;
      elsif (CE = '1') then
        Q <= D after 100 ps;
      end if;
    end if;
  end process;
end FDRE_V;

library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity FDRS is
  generic(
    INIT : bit := '0'
    );

  port(
    Q : out std_ulogic;

    C : in std_ulogic;
    D : in std_ulogic;
    R : in std_ulogic;
    S : in std_ulogic
    );
end FDRS;

architecture FDRS_V of FDRS is
begin
  VITALBehavior         : process(C)
    variable FIRST_TIME : boolean := true ;
  begin

    if (FIRST_TIME = true) then
      Q <= TO_X01(INIT);
      FIRST_TIME := false;
    end if;

    if (rising_edge(C)) then
      if (R = '1') then
        Q <= '0' after 100 ps;
      elsif (S = '1') then
        Q <= '1' after 100 ps;
      else
        Q <= D after 100 ps;
      end if;
    end if;
  end process;
end FDRS_V;

library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity VCC is
  port(
    P : out std_ulogic := '1'
    );
end VCC;

architecture VCC_V of VCC is
begin
  P <= '1';
end VCC_V;

library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity GND is
  port(
    G : out std_ulogic := '0'
    );
end GND;

architecture GND_V of GND is
begin

  G <= '0';
end GND_V;

library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity MUXF5 is
  port(
    O : out std_ulogic;

    I0 : in std_ulogic;
    I1 : in std_ulogic;
    S  : in std_ulogic
    );
end MUXF5;

architecture MUXF5_V of MUXF5 is
begin
  VITALBehavior   : process (I0, I1, S)
  begin
    if (S = '0') then
      O <= I0;
    elsif (S = '1') then
      O <= I1;      
    end if;    
  end process;
end MUXF5_V;

library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity FDE is
  generic(
    INIT : bit := '0'
    );

  port(
    Q : out std_ulogic;

    C  : in std_ulogic;
    CE : in std_ulogic;
    D  : in std_ulogic
    );
end FDE;

architecture FDE_V of FDE is
begin
  VITALBehavior         : process(C)
    variable FIRST_TIME : boolean := true ;
  begin

    if (FIRST_TIME = true) then
      Q <= TO_X01(INIT);
      FIRST_TIME := false;
    end if;

    if (rising_edge(C)) then
      if (CE = '1') then
        Q <= D after 100 ps;
      end if;
    end if;
  end process;
end FDE_V;

library IEEE;
use IEEE.STD_LOGIC_1164.all;

library unisim;
use unisim.simple_simprim_pck.all;

entity IDELAY is

  generic(

      IOBDELAY_TYPE  : string := "DEFAULT";
      IOBDELAY_VALUE : integer := 0
      );

  port(
      O      : out std_ulogic;

      C      : in  std_ulogic;
      CE     : in  std_ulogic;
      I      : in  std_ulogic;
      INC    : in  std_ulogic;
      RST    : in  std_ulogic
      );

end IDELAY;

architecture IDELAY_V OF IDELAY is

  constant SIM_TAPDELAY_VALUE : integer := 75;

  ---------------------------------------------------------
  -- Function  str_2_int converts string to integer
  ---------------------------------------------------------
  function str_2_int(str: in string ) return integer is
  variable int : integer;
  variable val : integer := 0;
  variable neg_flg   : boolean := false;
  variable is_it_int : boolean := true;
  begin
    int := 0;
    val := 0;
    is_it_int := true;
    neg_flg   := false;

    for i in  1 to str'length loop
      case str(i) is
         when  '-'
           =>
             if(i = 1) then
                neg_flg := true;
                val := -1;
             end if;
         when  '1'
           =>  val := 1;
         when  '2'
           =>   val := 2;
         when  '3'
           =>   val := 3;
         when  '4'
           =>   val := 4;
         when  '5'
           =>   val := 5;
         when  '6'
           =>   val := 6;
         when  '7'
           =>   val := 7;
         when  '8'
           =>   val := 8;
         when  '9'
           =>   val := 9;
         when  '0'
           =>   val := 0;
         when others
           => is_it_int := false;
        end case;
        if(val /= -1) then
          int := int *10  + val;
        end if;
        val := 0;
    end loop;
    if(neg_flg) then
      int := int * (-1);
    end if;

    if(NOT is_it_int) then
      int := -9999;
    end if;
    return int;
  end;
-----------------------------------------------------------

  constant	SYNC_PATH_DELAY	: time := 100 ps;

  constant	MIN_TAP_COUNT	: integer := 0;
  constant	MAX_TAP_COUNT	: integer := 63;

  signal	C_ipd		: std_ulogic := 'X';
  signal	CE_ipd		: std_ulogic := 'X';
  signal GSR            : std_ulogic := '0';
  signal	GSR_ipd		: std_ulogic := 'X';
  signal	I_ipd		: std_ulogic := 'X';
  signal	INC_ipd		: std_ulogic := 'X';
  signal	RST_ipd		: std_ulogic := 'X';

  signal	C_dly		: std_ulogic := 'X';
  signal	CE_dly		: std_ulogic := 'X';
  signal	GSR_dly		: std_ulogic := 'X';
  signal	I_dly		: std_ulogic := 'X';
  signal	INC_dly		: std_ulogic := 'X';
  signal	RST_dly		: std_ulogic := 'X';

  signal	O_zd		: std_ulogic := 'X';
  signal	O_viol		: std_ulogic := 'X';

  signal	TapCount	: integer := 0;
  signal	IsTapDelay	: boolean := true; 
  signal	IsTapFixed	: boolean := false; 
  signal	IsTapDefault	: boolean := false; 
  signal	Delay		: time := 0 ps; 

begin

  ---------------------
  --  INPUT PATH DELAYs
  --------------------

  C_dly          	 <= C              	after 0 ps;
  CE_dly         	 <= CE             	after 0 ps;
  GSR_dly        	 <= GSR            	after 0 ps;
  I_dly          	 <= I              	after 0 ps;
  INC_dly        	 <= INC            	after 0 ps;
  RST_dly        	 <= RST            	after 0 ps;

  --------------------
  --  BEHAVIOR SECTION
  --------------------

--####################################################################
--#####                     Initialize                           #####
--####################################################################
  prcs_init:process
  variable TapCount_var   : integer := 0;
  variable IsTapDelay_var : boolean := true; 
  variable IsTapFixed_var : boolean := false; 
  variable IsTapDefault_var : boolean := false; 
  begin
--     if((IOBDELAY_VALUE = "OFF") or (IOBDELAY_VALUE = "off")) then
--        IsTapDelay_var := false;
--     elsif((IOBDELAY_VALUE = "ON") or (IOBDELAY_VALUE = "on")) then
--        IsTapDelay_var := false;
--     else
--       TapCount_var := str_2_int(IOBDELAY_VALUE); 
       TapCount_var := IOBDELAY_VALUE; 
       If((TapCount_var >= 0) and (TapCount_var <= 63)) then 
         IsTapDelay_var := true;

       else
          GenericValueCheckMessage
          (  HeaderMsg  => " Attribute Syntax Warning ",
             GenericName => " IOBDELAY_VALUE ",
             EntityName => "/IOBDELAY_VALUE",
             GenericValue => IOBDELAY_VALUE,
             Unit => "",
             ExpectedValueMsg => " The Legal values for this attribute are ",
             ExpectedGenericValue => " OFF, 1, 2, ..., 62, 63 ",
             TailMsg => "",
             MsgSeverity => failure 
          );
        end if;
--     end if;

     if(IsTapDelay_var) then
        if((IOBDELAY_TYPE = "FIXED") or (IOBDELAY_TYPE = "fixed")) then
           IsTapFixed_var := true;
        elsif((IOBDELAY_TYPE = "VARIABLE") or (IOBDELAY_TYPE = "variable")) then
           IsTapFixed_var := false;
        elsif((IOBDELAY_TYPE = "DEFAULT") or (IOBDELAY_TYPE = "default")) then
           IsTapDefault_var := true;
        else
          GenericValueCheckMessage
          (  HeaderMsg  => " Attribute Syntax Warning ",
             GenericName => " IOBDELAY_TYPE ",
             EntityName => "/IOBDELAY_TYPE",
             GenericValue => IOBDELAY_TYPE,
             Unit => "",
             ExpectedValueMsg => " The Legal values for this attribute are ",
             ExpectedGenericValue => " FIXED or VARIABLE ",
             TailMsg => "",
             MsgSeverity => failure 
          );
        end if; 
     end if; 

     IsTapDelay   <= IsTapDelay_var;
     IsTapFixed   <= IsTapFixed_var;
     IsTapDefault <= IsTapDefault_var;
     TapCount     <= TapCount_var;

     wait;
  end process prcs_init;
--####################################################################
--#####                  CALCULATE DELAY                         #####
--####################################################################
  prcs_refclk:process(C_dly, GSR_dly, RST_dly)
  variable TapCount_var : integer :=0;
  variable FIRST_TIME   : boolean :=true;
  variable BaseTime_var : time    := 1 ps ;
  variable delay_var    : time    := 0 ps ;
  begin
     if(IsTapDelay) then
       if((GSR_dly = '1') or (FIRST_TIME))then
          TapCount_var := TapCount; 
          Delay        <= TapCount_var * SIM_TAPDELAY_VALUE * BaseTime_var; 
          FIRST_TIME   := false;
       elsif(GSR_dly = '0') then
          if(rising_edge(C_dly)) then
             if(RST_dly = '1') then
               TapCount_var := TapCount; 
             elsif((RST_dly = '0') and (CE_dly = '1')) then
-- CR fix CR 213995
                  if(INC_dly = '1') then
                     if (TapCount_var < MAX_TAP_COUNT) then
                        TapCount_var := TapCount_var + 1;
                     else 
                        TapCount_var := MIN_TAP_COUNT;
                     end if;
                  elsif(INC_dly = '0') then
                     if (TapCount_var > MIN_TAP_COUNT) then
                         TapCount_var := TapCount_var - 1;
                     else
                         TapCount_var := MAX_TAP_COUNT;
                     end if;
                         
                  end if; -- INC_dly
             end if; -- RST_dly
             Delay <= TapCount_var *  SIM_TAPDELAY_VALUE * BaseTime_var;
          end if; -- C_dly
       end if; -- GSR_dly

     end if; -- IsTapDelay 
  end process prcs_refclk;

--####################################################################
--#####                      DELAY INPUT                         #####
--####################################################################
  prcs_i:process(I_dly)
  begin
     if(IsTapFixed) then
       O_zd <= transport I_dly after (TapCount *SIM_TAPDELAY_VALUE * 1 ps);
     else
        O_zd <= transport I_dly after delay;
     end if;
  end process prcs_i;


--####################################################################
--#####                         OUTPUT                           #####
--####################################################################
  prcs_output:process(O_zd)
  begin
      O <= O_zd after SYNC_PATH_DELAY;
  end process prcs_output;
--####################################################################


end IDELAY_V;

library IEEE;
use IEEE.STD_LOGIC_1164.all;


library unisim;
use unisim.simple_simprim_pck.all;

entity IDELAYCTRL is

  port(
      RDY	: out std_ulogic;

      REFCLK	: in  std_ulogic;
      RST	: in  std_ulogic
  );

end IDELAYCTRL;

architecture IDELAYCTRL_V OF IDELAYCTRL is


  constant SYNC_PATH_DELAY : time := 100 ps;

  signal REFCLK_ipd	: std_ulogic := 'X';
  signal RST_ipd	: std_ulogic := 'X';

  signal GSR_dly	: std_ulogic := '0';
  signal REFCLK_dly	: std_ulogic := 'X';
  signal RST_dly	: std_ulogic := 'X';

  signal RDY_zd		: std_ulogic := '0';
  signal RDY_viol	: std_ulogic := 'X';

-- taken from DCM_adv
  signal period : time := 0 ps;
  signal lost   : std_ulogic := '0';
  signal lost_r : std_ulogic := '0';
  signal lost_f : std_ulogic := '0';
  signal clock_negedge, clock_posedge, clock : std_ulogic;
  signal temp1 : boolean := false;
  signal temp2 : boolean := false;
  signal clock_low, clock_high : std_ulogic := '0';


begin

  ---------------------
  --  INPUT PATH DELAYs
  --------------------

  REFCLK_dly     	 <= REFCLK         	after 0 ps;
  RST_dly        	 <= RST            	after 0 ps;

  --------------------
  --  BEHAVIOR SECTION
  --------------------

--####################################################################
--#####                             RDY                          #####
--####################################################################
   prcs_rdy:process(RST_dly, lost)
   begin
      if((RST_dly = '1') or (lost = '1')) then
         RDY_zd <= '0';
      elsif((RST_dly = '0') and (lost = '0')) then
         RDY_zd <= '1';
      end if;
   end process prcs_rdy;
--####################################################################
--#####                prcs_determine_period                     #####
--####################################################################
  prcs_determine_period : process
    variable clock_edge_previous : time := 0 ps;
    variable clock_edge_current  : time := 0 ps;
  begin
    if (rising_edge(REFCLK_dly)) then
      clock_edge_previous := clock_edge_current;
      clock_edge_current := NOW;
      if (period /= 0 ps and ((clock_edge_current - clock_edge_previous) <= (1.5 * period))) then
        period <= NOW - clock_edge_previous;
      elsif (period /= 0 ps and ((NOW - clock_edge_previous) > (1.5 * period))) then
        period <= 0 ps;
      elsif ((period = 0 ps) and (clock_edge_previous /= 0 ps)) then
        period <= NOW - clock_edge_previous;
      end if;
    end if;
    wait on REFCLK_dly;
  end process prcs_determine_period;

--####################################################################
--#####                prcs_clock_lost_checker                   #####
--####################################################################
  prcs_clock_lost_checker : process
    variable clock_low, clock_high : std_ulogic := '0';

  begin
    if (rising_edge(clock)) then
      clock_low := '0';
      clock_high := '1';
      clock_posedge <= '0';
      clock_negedge <= '1';
    end if;

    if (falling_edge(clock)) then
      clock_high := '0';
      clock_low := '1';
      clock_posedge <= '1';
      clock_negedge <= '0';
    end if;
    wait on clock;
  end process prcs_clock_lost_checker;

--####################################################################
--#####                prcs_set_reset_lost_r                     #####
--####################################################################
  prcs_set_reset_lost_r : process
    begin
    if (rising_edge(clock)) then
      if (period /= 0 ps) then
        lost_r <= '0';
      end if;
      wait for (period * 9.1)/10;
      if ((clock_low /= '1') and (clock_posedge /= '1')) then
        lost_r <= '1';
      end if;
    end if;
    wait on clock;
  end process prcs_set_reset_lost_r;

--####################################################################
--#####                     prcs_assign_lost                     #####
--####################################################################
  prcs_assign_lost : process
    begin
      if (lost_r'event) then
        lost <= lost_r;
      end if;
      if (lost_f'event) then
        lost <= lost_f;
      end if;
      wait on lost_r, lost_f;
    end process prcs_assign_lost;

--####################################################################
--#####                         OUTPUT                           #####
--####################################################################
  prcs_output:process(RDY_zd)
  begin
      RDY <= RDY_zd after SYNC_PATH_DELAY;
  end process prcs_output;
--####################################################################


end IDELAYCTRL_V;

library IEEE;
use IEEE.STD_LOGIC_1164.all;

entity BUFIO is
  port(
    O : out std_ulogic;

    I : in std_ulogic
    );

end BUFIO;

architecture BUFIO_V of BUFIO is
begin
  O <= I after 0 ps;
end BUFIO_V;

library IEEE;
use IEEE.STD_LOGIC_1164.all;


library unisim;
use unisim.simple_simprim_pck.all;

entity BUFR is

  generic(

      BUFR_DIVIDE   : string := "BYPASS";
      SIM_DEVICE    : string := "VIRTEX4"
      );

  port(
      O           : out std_ulogic;

      CE          : in  std_ulogic;
      CLR         : in  std_ulogic;
      I           : in  std_ulogic
      );

end BUFR;

architecture BUFR_V OF BUFR is


--    06/30/2005 - CR # 211199 -- 
--  constant SYNC_PATH_DELAY : time := 100 ps;

  signal CE_ipd	        : std_ulogic := 'X';
  signal GSR            : std_ulogic := '0';
  signal GSR_ipd	: std_ulogic := '0';
  signal I_ipd	        : std_ulogic := 'X';
  signal CLR_ipd	: std_ulogic := 'X';

  signal CE_dly       	: std_ulogic := 'X';
  signal GSR_dly	: std_ulogic := '0';
  signal I_dly       	: std_ulogic := 'X';
  signal CLR_dly	: std_ulogic := 'X';

  signal O_zd	        : std_ulogic := 'X';
  signal O_viol	        : std_ulogic := 'X';

  signal q4_sig	        : std_ulogic := 'X';
  signal ce_en	        : std_ulogic;

  signal divide   	: boolean    := false;
  signal divide_by	: integer    := -1;
  signal FIRST_TOGGLE_COUNT     : integer    := -1;
  signal SECOND_TOGGLE_COUNT    : integer    := -1;

begin

  ---------------------
  --  INPUT PATH DELAYs
  --------------------

  CE_dly         	 <= CE             	after 0 ps;
  CLR_dly        	 <= CLR            	after 0 ps;
  GSR_dly        	 <= GSR            	after 0 ps;
  I_dly          	 <= I              	after 0 ps;

  --------------------
  --  BEHAVIOR SECTION
  --------------------

--####################################################################
--#####                     Initialize                           #####
--####################################################################
  prcs_init:process
  variable FIRST_TOGGLE_COUNT_var  : integer    := -1;
  variable SECOND_TOGGLE_COUNT_var : integer    := -1;
  variable ODD                     : integer    := -1;
  variable divide_var  	   : boolean    := false;
  variable divide_by_var           :  integer    := -1;

  begin
      if(BUFR_DIVIDE = "BYPASS") then
         divide_var := false;
      elsif(BUFR_DIVIDE = "1") then
         divide_var    := true;
         divide_by_var := 1;
         FIRST_TOGGLE_COUNT_var  := 1;
         SECOND_TOGGLE_COUNT_var := 1;
      elsif(BUFR_DIVIDE = "2") then
         divide_var    := true;
         divide_by_var := 2;
         FIRST_TOGGLE_COUNT_var  := 2;
         SECOND_TOGGLE_COUNT_var := 2;
      elsif(BUFR_DIVIDE = "3") then
         divide_var    := true;
         divide_by_var := 3;
         FIRST_TOGGLE_COUNT_var  := 2;
         SECOND_TOGGLE_COUNT_var := 4;
      elsif(BUFR_DIVIDE = "4") then
         divide_var    := true;
         divide_by_var := 4;
         FIRST_TOGGLE_COUNT_var  := 4;
         SECOND_TOGGLE_COUNT_var := 4;
      elsif(BUFR_DIVIDE = "5") then
         divide_var    := true;
         divide_by_var := 5;
         FIRST_TOGGLE_COUNT_var  := 4;
         SECOND_TOGGLE_COUNT_var := 6;
      elsif(BUFR_DIVIDE = "6") then
         divide_var    := true;
         divide_by_var := 6;
         FIRST_TOGGLE_COUNT_var  := 6;
         SECOND_TOGGLE_COUNT_var := 6;
      elsif(BUFR_DIVIDE = "7") then
         divide_var    := true;
         divide_by_var := 7;
         FIRST_TOGGLE_COUNT_var  := 6;
         SECOND_TOGGLE_COUNT_var := 8;
      elsif(BUFR_DIVIDE = "8") then
         divide_var    := true;
         divide_by_var := 8;
         FIRST_TOGGLE_COUNT_var  := 8;
         SECOND_TOGGLE_COUNT_var := 8;
      else
        GenericValueCheckMessage
          (  HeaderMsg  => " Attribute Syntax Warning ",
             GenericName => " BUFR_DIVIDE ",
             EntityName => "/BUFR",
             GenericValue => BUFR_DIVIDE,
             Unit => "",
             ExpectedValueMsg => " The Legal values for this attribute are ",
             ExpectedGenericValue => " BYPASS, 1, 2, 3, 4, 5, 6, 7 or 8 ",
             TailMsg => "",
             MsgSeverity => ERROR 
         );
      end if;

     if (SIM_DEVICE /= "VIRTEX4" and SIM_DEVICE /= "VIRTEX5") then
        GenericValueCheckMessage
          (  HeaderMsg  => " Attribute Syntax Warning ",
             GenericName => " SIM_DEVICE ",
             EntityName => "/BUFR",
             GenericValue => SIM_DEVICE,
             Unit => "",
             ExpectedValueMsg => " The Legal values for this attribute are ",
             ExpectedGenericValue => " VIRTEX4 or VIRTEX5 ",
             TailMsg => "",
             MsgSeverity => ERROR
         );
      end if;

      FIRST_TOGGLE_COUNT  <= FIRST_TOGGLE_COUNT_var; 
      SECOND_TOGGLE_COUNT <= SECOND_TOGGLE_COUNT_var; 

      divide    <= divide_var;
      divide_by <= divide_by_var;

     wait;
  end process prcs_init;
--####################################################################
--#####                      CLOCK_ENABLE                        #####
--####################################################################
   prcs_ce:process(I_Dly, GSR_dly)
   variable fall_i_count : integer    := 0;
   variable q4_var       : std_ulogic := '0';
   variable q3_var       : std_ulogic := '0';
   variable q2_var       : std_ulogic := '0';
   variable q1_var       : std_ulogic := '0';
   begin
--    06/30/2005 - CR # 211199 -- removed CLR_dly dependency
      if(GSR_dly = '1')  then
         q4_var := '0';
         q3_var := '0';
         q2_var := '0';
         q1_var := '0';
      elsif(GSR_dly = '0') then
         if(falling_edge(I_dly)) then
            q4_var := q3_var;
            q3_var := q2_var;
            q2_var := q1_var;
            q1_var := CE_dly;
         end if;
  
         q4_sig	 <= q4_var;
      end if;
   end process prcs_ce;

   ce_en <= CE_dly when (SIM_DEVICE = "VIRTEX5") else q4_sig;

--####################################################################
--#####                       CLK-I                              #####
--####################################################################
  prcs_I:process(I_dly, GSR_dly, CLR_dly, ce_en)
  variable clk_count      : integer := 0;
  variable toggle_count   : integer := 0;
  variable first          : boolean := true;
  variable FIRST_TIME     : boolean := true;
  begin
       if(divide) then
          if((GSR_dly = '1') or (CLR_dly = '1')) then
            O_zd       <= '0';
            clk_count  := 0;
            FIRST_TIME := true;
          elsif((GSR_dly = '0') and (CLR_dly = '0')) then
             if(ce_en = '1') then
                if((I_dly='1') and (FIRST_TIME)) then
                    O_zd <= '1';
                    first        := true;
                    toggle_count := FIRST_TOGGLE_COUNT;
                    FIRST_TIME := false;
                elsif ((I_dly'event) and ( FIRST_TIME = false)) then
                    if(clk_count = toggle_count) then
                       O_zd <= not O_zd;
                       clk_count := 0;
                       first := not first;
                       if(first = true) then
                         toggle_count := FIRST_TOGGLE_COUNT;
                       else
                         toggle_count := SECOND_TOGGLE_COUNT;
                       end if;
                    end if;
                 end if;
 
                 if (FIRST_TIME = false) then
                       clk_count := clk_count + 1;
                end if;
             else
                 clk_count := 0;
                 FIRST_TIME := true;
             end if;
          end if;
       else
          O_zd <= I_dly;
       end if;
  end process prcs_I;

--####################################################################
--#####                         OUTPUT                           #####
--####################################################################
  prcs_output:process(O_zd)
  begin
--    06/30/2005 - CR # 211199 -- 
--    O <= O_zd after SYNC_PATH_DELAY;
      O <= O_zd;
  end process prcs_output;
--####################################################################


end BUFR_V;

-- pragma translate_on

