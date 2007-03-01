--**********************************************************************************************
--  Some additional control registers for the AVR Core
--  Version 0.7 20.05.2003
--  Designed by Ruslan Lepetenok
--**********************************************************************************************

library IEEE;
use IEEE.std_logic_1164.all;

use WORK.AVRuCPackage.all;

entity Service_Module is port(
	                   -- AVR Control
               ireset         : in  std_logic;
               cp2	          : in  std_logic;
               adr            : in  std_logic_vector(5 downto 0);
               dbus_in        : in  std_logic_vector(7 downto 0);
               dbus_out       : out std_logic_vector(7 downto 0);
               iore           : in  std_logic;
               iowe           : in  std_logic;
               out_en         : out std_logic; 
			        -- SLEEP mode signals
			   sleep_en       : out std_logic;
		   	        -- SRAM control signals
			   ESRAM_en       : out std_logic;
			   ESRAM_WS       : out std_logic;
			   	    --IRQ
               ExtInt_IRQ     : out std_logic_vector(7 downto 0);
			   ExtInt_IRQ_Ack : in  std_logic_vector(3 downto 0);
			   
			   -- External interrupts (inputs)
			   Ext_Int_In     : in  std_logic_vector(7 downto 0));

end Service_Module;

architecture rtl of Service_Module is

-- Registers
signal MCUCR  : std_logic_vector(7 downto 0) := (others => '0');
signal EIMSK  : std_logic_vector(7 downto 0) := (others => '0');
signal EIFR   : std_logic_vector(7 downto 0) := (others => '0');
signal EICR   : std_logic_vector(7 downto 0) := (others => '0');
signal MCUSR  : std_logic_vector(7 downto 0) := (others => '0');
signal XDIV   : std_logic_vector(7 downto 0) := (others => '0');

signal MCUCR_Sel : std_logic := '0';
signal EIMSK_Sel : std_logic := '0';
signal EIFR_Sel  : std_logic := '0';
signal EICR_Sel  : std_logic := '0';
signal MCUSR_Sel : std_logic := '0';
signal XDIV_Sel  : std_logic := '0';

  -- MCUCR Bits
alias SM0   : std_logic is MCUCR(3);
alias SM1   : std_logic is MCUCR(4);
alias SE    : std_logic is MCUCR(5);
alias SRW   : std_logic is MCUCR(6);
alias SRE   : std_logic is MCUCR(7);

  -- EIMSK Bits
alias INT0  : std_logic is EIMSK(0);
alias INT1  : std_logic is EIMSK(1);
alias INT2  : std_logic is EIMSK(2);
alias INT3  : std_logic is EIMSK(3);
alias INT4  : std_logic is EIMSK(4);
alias INT5  : std_logic is EIMSK(5);
alias INT6  : std_logic is EIMSK(6);
alias INT7  : std_logic is EIMSK(7);

  -- EIFR Bits
alias INTF4 : std_logic is EIFR(4);
alias INTF5 : std_logic is EIFR(5);
alias INTF6 : std_logic is EIFR(6);
alias INTF7 : std_logic is EIFR(7);

  -- EICR Bits
alias ISC40  : std_logic is EICR(0);
alias ISC41  : std_logic is EICR(1);
alias ISC50  : std_logic is EICR(2);
alias ISC51  : std_logic is EICR(3);
alias ISC60  : std_logic is EICR(4);
alias ISC61  : std_logic is EICR(5);
alias ISC70  : std_logic is EICR(6);
alias ISC71  : std_logic is EICR(7);

  -- MCUSR Bits
alias PORF   : std_logic is MCUSR(0);
alias EXTRF  : std_logic is MCUSR(1);

  -- XDIV Bits
alias XDIV0  : std_logic is XDIV(0);
alias XDIV1  : std_logic is XDIV(1);
alias XDIV2  : std_logic is XDIV(2);
alias XDIV3  : std_logic is XDIV(3);
alias XDIV4  : std_logic is XDIV(4);
alias XDIV5  : std_logic is XDIV(5);
alias XDIV6  : std_logic is XDIV(6);
alias XDIVEN : std_logic is XDIV(7);

-- Edge detector signals for the external interrupt inputs
signal Ext_Int_Del  : std_logic_vector(3 downto 0) := (others=> '0'); -- Register for edge detecting
signal Edge_Det_Out : std_logic_vector(3 downto 0) := (others=> '0'); -- Output of the edge detector

-- Resynchronization signals for the external interrupt inputs
signal Ext_Int_Resync : std_logic_vector(Ext_Int_In'range) := (others => '0');
signal Ext_Int_InReg  : std_logic_vector(Ext_Int_In'range) := (others => '0');

-- Constants for conditional synthesis control
constant CImlXDIV : boolean := FALSE; -- XDIV register
	
begin

MCUCR_Sel <= '1' when adr=MCUCR_Address else '0';	
EIMSK_Sel <= '1' when adr=EIMSK_Address else '0';	
EIFR_Sel  <= '1' when adr=EIFR_Address else '0';	
EICR_Sel  <= '1' when adr=EICR_Address else '0';		
MCUSR_Sel <= '1' when adr=MCUSR_Address else '0';		
XDIV_Sel  <= '1' when adr=XDIV_Address else '0';		

ExternalIntSynchronizer:process(cp2,ireset)
begin
if (ireset='0') then                   -- Reset
 Ext_Int_Resync <= (others => '0'); 
  elsif (cp2='0' and cp2'event) then   -- Clock (falling edge)
	 Ext_Int_Resync <= Ext_Int_In;
	   end if;
end process;						
	
ExternalIntReg:process(cp2,ireset)
begin
if (ireset='0') then                   -- Reset
 Ext_Int_InReg <= (others => '0'); 
  elsif (cp2='1' and cp2'event) then   -- Clock
	 Ext_Int_InReg <= Ext_Int_Resync;
	   end if;
end process;					
	
MCUCR_Reg:process(cp2,ireset)
begin
if (ireset='0') then                   -- Reset
 MCUCR <= (others => '0'); 
  elsif (cp2='1' and cp2'event) then   -- Clock
    if (MCUCR_Sel and iowe)='1' then   -- Clock enable
	 MCUCR(7 downto 3) <= dbus_in(7 downto 3);
	end if;
	   end if;
end process;					

sleep_en <= SE;

EIMSK_Reg:process(cp2,ireset)
begin
if (ireset='0') then                   -- Reset
 EIMSK <= (others => '0'); 
  elsif (cp2='1' and cp2'event) then   -- Clock
    if (EIMSK_Sel and iowe)='1' then   -- Clock enable
	 EIMSK <= dbus_in;
	end if;
	   end if;
end process;				

EICR_Reg:process(cp2,ireset)
begin
if (ireset='0') then                   -- Reset
 EICR <= (others => '0'); 
  elsif (cp2='1' and cp2'event) then   -- Clock
    if (EICR_Sel and iowe)='1' then   -- Clock enable
	 EICR <= dbus_in;
	end if;
	   end if;
end process;				

MCUSR_Reg:process(cp2,ireset)
begin
if (ireset='0') then                   -- Reset
 MCUSR <= (others => '0'); 
  elsif (cp2='1' and cp2'event) then   -- Clock
    if (MCUSR_Sel and iowe)='1' then   -- Clock enable
	 MCUSR(1 downto 0) <= dbus_in(1 downto 0);
	end if;
	   end if;
end process;				

XDIVImplementation :if CImlXDIV generate
XDIV_Reg:process(cp2,ireset)
begin
if (ireset='0') then                   -- Reset
 XDIV <= (others => '0'); 
  elsif (cp2='1' and cp2'event) then   -- Clock
    if (XDIV_Sel and iowe)='1' then    -- Clock enable
	 XDIV <= dbus_in;
	end if;
	   end if;
end process;				
end generate;


EIFR_Reg:process(cp2,ireset)
begin
if ireset='0' then 				       -- Reset
 EIFR <= (others => '0');
 Ext_Int_Del <= (others => '0');
  elsif (cp2='1' and cp2'event) then   -- Clock
   -- External interrupts 4 to 7
   for i in 3 downto 0 loop
   EIFR(i+4) <= (not EIFR(i+4) and Edge_Det_Out(i)) or  
   (EIFR(i+4) and not ((EIFR_Sel and iowe and dbus_in(i+4))or(ExtInt_IRQ_Ack(i) and not Edge_Det_Out(i)))); -- ?? Edge_Det_Out(i) ??
   end loop;
   Ext_Int_Del <= Ext_Int_InReg(3 downto 0); 
   end if;	
end process;

Edge_Detector_Logic: for i in 3 downto 0 generate
Edge_Det_Out(i) <= ((not Ext_Int_InReg(i+4) and Ext_Int_Del(i)) and (EICR(2*i+1) and not EICR(2*i))) or -- Falling edge
				   ((Ext_Int_InReg(i+4) and not Ext_Int_Del(i)) and (EICR(2*i+1) and EICR(2*i)));       -- Rising edge
end generate;

ExtInt_IRQ(3 downto 0) <= not Ext_Int_InReg(3 downto 0) and EIMSK(3 downto 0);

HighIRQs:for i in 3 downto 0 generate
ExtInt_IRQ(i+4) <= (EIFR(i+4) and EIMSK(i+4)) when EICR(2*i+1)='1' else  -- Edge control 
                   (not Ext_Int_InReg(i+4) and EIMSK(i+4)) when  EICR(2*i+1)='0' and EICR(2*i)='0' else -- Level(low) control
                   '0';
end generate;

-- External SRAM control
ESRAM_en <= SRE;
ESRAM_WS <= SRW;

out_en <= (MCUCR_Sel or EIMSK_Sel or EIFR_Sel or EICR_Sel or MCUSR_Sel or XDIV_Sel) and iore;

Output_Data_Mux:for i in dbus_out'range generate
dbus_out(i) <= 	(MCUCR(i) and MCUCR_Sel)or(EIMSK(i) and EIMSK_Sel)or(EIFR(i) and EIFR_Sel)or
                (EICR(i) and EICR_Sel)or(MCUSR(i) and MCUSR_Sel)or(XDIV(i) and XDIV_Sel);
end generate;

end rtl;
