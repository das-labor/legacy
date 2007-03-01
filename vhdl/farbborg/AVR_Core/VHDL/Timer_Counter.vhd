--**********************************************************************************************
-- Timers/Counters Block Peripheral for the AVR Core
-- Version 1.01 
-- Modified 20.05.2003
-- Synchronizer for EXT1/EXT2/Tosc1 inputs was added
-- Designed by Ruslan Lepetenok
--**********************************************************************************************

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;

use WORK.AVRuCPackage.all;

entity Timer_Counter is port(
	                   -- AVR Control
               ireset         : in std_logic;
               cp2	          : in std_logic;
               adr            : in std_logic_vector(5 downto 0);
               dbus_in        : in std_logic_vector(7 downto 0);
               dbus_out       : out std_logic_vector(7 downto 0);
               iore           : in std_logic;
               iowe           : in std_logic;
               out_en         : out std_logic; 

                       --Timer/Counters
               EXT1           : in std_logic;
               EXT2           : in std_logic;
			   Tosc1	      : in std_logic;
			   OC0_PWM0       : out std_logic;
			   OC1A_PWM1A     : out std_logic;
			   OC1B_PWM1B     : out std_logic;
			   OC2_PWM2       : out std_logic;
			   		   
			           --IRQ
               TC0OvfIRQ      : out std_logic;
			   TC0OvfIRQ_Ack  : in std_logic;
			   TC0CmpIRQ      : out std_logic;
			   TC0CmpIRQ_Ack  : in std_logic;
			   TC2OvfIRQ      : out std_logic;
			   TC2OvfIRQ_Ack  : in std_logic;
			   TC2CmpIRQ      : out std_logic;
			   TC2CmpIRQ_Ack  : in std_logic;
			   TC1OvfIRQ      : out std_logic;
			   TC1OvfIRQ_Ack  : in std_logic;
			   TC1CmpAIRQ     : out std_logic;
			   TC1CmpAIRQ_Ack : in std_logic;
			   TC1CmpBIRQ     : out std_logic;
			   TC1CmpBIRQ_Ack : in std_logic;			   
			   TC1ICIRQ       : out std_logic;
			   TC1ICIRQ_Ack   : in std_logic);
end Timer_Counter;

architecture rtl of Timer_Counter is

-- Copies of the external signals
signal OC0_PWM0_Int   :	std_logic := '0';
signal OC2_PWM2_Int   :	std_logic := '0';

-- Registers
signal TCCR0  : std_logic_vector(7 downto 0) := (others => '0');
signal TCCR1A : std_logic_vector(7 downto 0) := (others => '0');
signal TCCR1B : std_logic_vector(7 downto 0) := (others => '0');
signal TCCR2  : std_logic_vector(7 downto 0) := (others => '0');
signal ASSR   : std_logic_vector(7 downto 0) := (others => '0'); -- Asynchronous status register (for TCNT0)
signal TIMSK  : std_logic_vector(7 downto 0) := (others => '0');
signal TIFR   : std_logic_vector(7 downto 0) := (others => '0');
signal TCNT0  : std_logic_vector(7 downto 0) := (others => '0');
signal TCNT2  : std_logic_vector(7 downto 0) := (others => '0');
signal OCR0   : std_logic_vector(7 downto 0) := (others => '0');
signal OCR2   : std_logic_vector(7 downto 0) := (others => '0');
signal TCNT1H : std_logic_vector(7 downto 0) := (others => '0');
signal TCNT1L : std_logic_vector(7 downto 0) := (others => '0');
signal OCR1AH : std_logic_vector(7 downto 0) := (others => '0');
signal OCR1AL : std_logic_vector(7 downto 0) := (others => '0');
signal OCR1BH : std_logic_vector(7 downto 0) := (others => '0');
signal OCR1BL : std_logic_vector(7 downto 0) := (others => '0');
signal ICR1AH : std_logic_vector(7 downto 0) := (others => '0');
signal ICR1AL : std_logic_vector(7 downto 0) := (others => '0');

signal TCCR0_Sel  : std_logic := '0';
signal TCCR1A_Sel : std_logic := '0';
signal TCCR1B_Sel : std_logic := '0';
signal TCCR2_Sel  : std_logic := '0';
signal ASSR_Sel   : std_logic := '0';
signal TIMSK_Sel  : std_logic := '0';
signal TIFR_Sel   : std_logic := '0';
signal TCNT0_Sel  : std_logic := '0';
signal TCNT2_Sel  : std_logic := '0';
signal OCR0_Sel   : std_logic := '0';
signal OCR2_Sel   : std_logic := '0';
signal TCNT1H_Sel : std_logic := '0';
signal TCNT1L_Sel : std_logic := '0';
signal OCR1AH_Sel : std_logic := '0';
signal OCR1AL_Sel : std_logic := '0';
signal OCR1BH_Sel : std_logic := '0';
signal OCR1BL_Sel : std_logic := '0';
signal ICR1AH_Sel : std_logic := '0';
signal ICR1AL_Sel : std_logic := '0';

  -- TCCR0 Bits
alias CS00  : std_logic is TCCR0(0);
alias CS01  : std_logic is TCCR0(1);
alias CS02  : std_logic is TCCR0(2);
alias CTC0  : std_logic is TCCR0(3);
alias COM00 : std_logic is TCCR0(4);
alias COM01 : std_logic is TCCR0(5);
alias PWM0  : std_logic is TCCR0(6);

  -- TCCR1A Bits
alias PWM10  : std_logic is TCCR1A(0);
alias PWM11  : std_logic is TCCR1A(1);
alias COM1B0 : std_logic is TCCR1A(4);
alias COM1B1 : std_logic is TCCR1A(5);
alias COM1A0 : std_logic is TCCR1A(4);
alias COM1A1 : std_logic is TCCR1A(5);

  -- TCCR1B Bits
alias CS10  : std_logic is TCCR1A(0);
alias CS11  : std_logic is TCCR1A(1);
alias CS12  : std_logic is TCCR1A(2);
alias CTC1  : std_logic is TCCR1A(3);
alias ICES1 : std_logic is TCCR1A(6);
alias ICNC1 : std_logic is TCCR1A(7);

  -- TCCR2 Bits
alias CS20  : std_logic is TCCR2(0);
alias CS21  : std_logic is TCCR2(1);
alias CS22  : std_logic is TCCR2(2);
alias CTC2  : std_logic is TCCR2(3);
alias COM20 : std_logic is TCCR2(4);
alias COM21 : std_logic is TCCR2(5);
alias PWM2  : std_logic is TCCR2(6);

-- ASSR bits
alias TCR0UB  : std_logic is ASSR(0);
alias OCR0UB  : std_logic is ASSR(1);
alias TCN0UB  : std_logic is ASSR(2);
alias AS0     : std_logic is ASSR(3);

-- TIMSK bits
alias TOIE0     : std_logic is TIMSK(0);
alias OCIE0     : std_logic is TIMSK(1);
alias TOIE1     : std_logic is TIMSK(2);
alias OCIE1B    : std_logic is TIMSK(3);
alias OCIE1A    : std_logic is TIMSK(4);
alias TICIE1    : std_logic is TIMSK(5);
alias TOIE2     : std_logic is TIMSK(6);
alias OCIE2     : std_logic is TIMSK(7);

-- TIFR bits
alias TOV0     : std_logic is TIFR(0);
alias OCF0     : std_logic is TIFR(1);
alias TOV1     : std_logic is TIFR(2);
alias OCF1B    : std_logic is TIFR(3);
alias OCF1A    : std_logic is TIFR(4);
alias ICF1     : std_logic is TIFR(5);
alias TOV2     : std_logic is TIFR(6);
alias OCF2     : std_logic is TIFR(7);

-- Prescaler1 signals
signal CK8    : std_logic := '0';
signal CK64   : std_logic := '0';
signal CK256  : std_logic := '0';
signal CK1024 : std_logic := '0';

signal Pre1Cnt : std_logic_vector(9 downto 0) := (others => '0'); -- Prescaler 1 counter (10-bit)

signal EXT1RE : std_logic := '0'; -- Rising edge of external input EXT1 (for TCNT1 only)
signal EXT1FE : std_logic := '0'; -- Falling edge of external input EXT1 (for TCNT1 only)

signal EXT2RE : std_logic := '0'; -- Rising edge of external input EXT2	(for TCNT2 only)
signal EXT2FE : std_logic := '0'; -- Falling edge of external input EXT2 (for TCNT2 only)

-- Risign/falling edge detectors	
signal EXT1Latched : std_logic := '0';	
signal EXT2Latched : std_logic := '0';	

-- Prescalers outputs 
signal TCNT0_En : std_logic := '0'; -- Output of the prescaler 0
signal TCNT1_En : std_logic := '0';	-- Output of the prescaler 1
signal TCNT2_En : std_logic := '0';	-- Output of the prescaler 1

-- Prescaler0 signals	
signal PCK0     : std_logic := '0';
signal PCK08    : std_logic := '0';
signal PCK032   : std_logic := '0';
signal PCK064   : std_logic := '0';
signal PCK0128  : std_logic := '0';
signal PCK0256  : std_logic := '0';
signal PCK01024 : std_logic := '0';

signal Tosc1RE      : std_logic := '0'; -- Rising edge detector for TOSC1 input
signal Tosc1Latched : std_logic := '0';
signal Pre0Cnt      : std_logic_vector(9 downto 0) := (others => '0'); -- Prescaler 0 counter (10-bit)

signal PCK0_Del     : std_logic := '0';

-- Timer/counter 0 signals
signal TCNT0_Tmp     : std_logic_vector(7 downto 0) := (others => '0');
signal TCNT0_In      : std_logic_vector(7 downto 0) := (others => '0');
signal TCNT0_Imm_In  : std_logic_vector(7 downto 0) := (others => '0'); -- Immediate data input 
signal TCCR0_Tmp     : std_logic_vector(7 downto 0) := (others => '0');
signal TCCR0_In      : std_logic_vector(7 downto 0) := (others => '0');
signal OCR0_Tmp      : std_logic_vector(7 downto 0) := (others => '0');
signal OCR0_In       : std_logic_vector(7 downto 0) := (others => '0');
signal TCNT0_Cnt_Dir : std_logic := '0'; -- Count up(0) down (1)
signal TCNT0_Clr     : std_logic := '0'; -- Clear (syncronously) TCNT0
signal TCNT0_Ld_Imm  : std_logic := '0'; -- Load immediate value (syncronously) TCNT0
signal TCNT0_Cmp_Out : std_logic := '0'; -- Output of the comparator
signal TCNT0_Inc     : std_logic := '0'; -- Increment (not load) took place

-- For asynchronous mode only
signal TCR0UB_Tmp     : std_logic := '0';
signal OCR0UB_Tmp     : std_logic := '0';
signal TCN0UB_Tmp     : std_logic := '0';

-- Timer/counter 2 signals
signal TCNT2_In      : std_logic_vector(7 downto 0) := (others => '0');
signal OCR2_Tmp      : std_logic_vector(7 downto 0) := (others => '0');
signal TCNT2_Cnt_Dir : std_logic := '0'; -- Count up(0) down (1)
signal TCNT2_Clr     : std_logic := '0'; -- Clear (syncronously) TCNT0
signal TCNT2_Imm_In  : std_logic_vector(7 downto 0) := (others => '0'); -- Immediate data input 
signal TCCR2_Tmp     : std_logic_vector(7 downto 0) := (others => '0');
signal OCR2_In       : std_logic_vector(7 downto 0) := (others => '0');
signal TCNT2_Ld_Imm  : std_logic := '0'; -- Load immediate value (syncronously) TCNT2
signal TCNT2_Cmp_Out : std_logic := '0'; -- Output of the comparator
signal TCNT2_Inc     : std_logic := '0'; -- Increment (not load) took place


-- Synchronizer signals
signal EXT1SA  : std_logic := '0';
signal EXT1SB  : std_logic := '0'; -- Output of the synchronizer for EXT1
signal EXT2SA  : std_logic := '0';
signal EXT2SB  : std_logic := '0'; -- Output of the synchronizer for EXT1
signal Tosc1SA : std_logic := '0';
signal Tosc1SB : std_logic := '0'; -- Output of the synchronizer for Tosc1

-- TBD

-- Timer/counter 1 signals
-- TBD

-- Additonal signals (These signals are added in order to emulate the behaviour of the real chip )
-- !!! TBD !!!
--signal PORTB4_Out : std_logic := '0';
--signal PORTB5_Out : std_logic := '0';
--signal PORTB6_Out : std_logic := '0';
--signal PORTB7_Out : std_logic := '0';

begin
	
-- Synchronizers
SyncDFFs:process(cp2,ireset)	
begin	
 if ireset='0' then      -- Reset
  EXT1SA <= '0';  
  EXT1SB <= '0';   
  EXT2SA <= '0';   
  EXT2SB <= '0';   
  Tosc1SA <= '0';  
  Tosc1SB <= '0';  
   elsif cp2='1' and cp2'event then -- Clock
    EXT1SA <= EXT1;  
    EXT1SB <= EXT1SA;   
    EXT2SA <= EXT2;   
    EXT2SB <= EXT2SA;   
    Tosc1SA <= Tosc1;  
    Tosc1SB <= Tosc1SA; 
end if;	 
end process;	
	
-- -------------------------------------------------------------------------------------------
-- Prescalers
-- -------------------------------------------------------------------------------------------	
	
-- Prescaler 1 for TCNT1 and TCNT2
Prescaler_1:process(cp2,ireset)
begin
if  ireset='0'  then                 -- Reset
 Pre1Cnt <= (others => '0'); 
 CK8 <= '0';
 CK64 <= '0';
 CK256 <= '0';
 CK1024 <= '0';
 EXT1RE <= '0';
 EXT1FE <= '0';
 EXT2RE <= '0';
 EXT2FE <= '0';
 EXT1Latched <= '0';
 EXT2Latched <= '0';
  elsif  cp2='1' and cp2'event  then -- Clock
    Pre1Cnt <= Pre1Cnt+1;
	 CK8 <= not CK8 and(Pre1Cnt(0) and Pre1Cnt(1)and Pre1Cnt(2));
     CK64 <= not CK64 and(Pre1Cnt(0) and Pre1Cnt(1) and Pre1Cnt(2) and Pre1Cnt(3) and Pre1Cnt(4) and Pre1Cnt(5));
     CK256 <= not CK256 and(Pre1Cnt(0) and Pre1Cnt(1) and Pre1Cnt(2) and Pre1Cnt(3) and Pre1Cnt(4) and Pre1Cnt(5) and Pre1Cnt(6) and Pre1Cnt(7));
     CK1024 <= not CK1024 and(Pre1Cnt(0) and Pre1Cnt(1) and Pre1Cnt(2) and Pre1Cnt(3) and Pre1Cnt(4) and Pre1Cnt(5) and Pre1Cnt(6) and Pre1Cnt(7) and Pre1Cnt(8) and Pre1Cnt(9));
     EXT1RE <= not EXT1RE and (EXT1SB and not EXT1Latched);
     EXT1FE <= not EXT1FE and (not EXT1SB and EXT1Latched);
     EXT2RE <= not EXT2RE and (EXT2SB and not EXT2Latched);
     EXT2FE <= not EXT2FE and (not EXT2SB and EXT2Latched);
     EXT1Latched <= EXT1SB;
     EXT2Latched <= EXT2SB;  
  end if;
end process;		

TCNT1_En <= (not CS12 and not CS11 and CS10) or            -- CK             "001"
            (CK8 and not CS12 and CS11 and not CS10) or    -- CK/8			 "010"
			(CK64 and not CS12 and CS11 and CS10)    or    -- CK/64			 "011"
			(CK256 and CS12 and not CS11 and not CS10) or  -- CK/256		 "100"
            (CK1024 and CS12 and not CS11 and CS10)    or  -- CK/1024		 "101"
			(EXT1FE and CS12 and CS11 and not CS10) or     -- Falling edge	 "110"
            (EXT1RE and CS12 and CS11 and CS10);           -- Rising edge	 "111"
			
TCNT2_En <= (not CS22 and not CS21 and CS20) or            -- CK             "001"
            (CK8 and not CS22 and CS21 and not CS20) or    -- CK/8			 "010"
			(CK64 and not CS22 and CS21 and CS20)    or    -- CK/64			 "011"
			(CK256 and CS22 and not CS21 and not CS20) or  -- CK/256		 "100"
            (CK1024 and CS22 and not CS21 and CS20)    or  -- CK/1024		 "101"
			(EXT2FE and CS22 and CS21 and not CS20) or     -- Falling edge	 "110"
            (EXT2RE and CS22 and CS21 and CS20);           -- Rising edge	 "111"


-- Prescaler 0 for TCNT0
Tosc1_Rising_Edge_Detector:process(cp2,ireset)
begin
 if ireset='0' then  --Reset
  Tosc1RE <= '0';
  Tosc1Latched <= '0';
     elsif  cp2='1' and cp2'event  then -- Clock
	Tosc1RE <= not Tosc1RE and (not Tosc1Latched and Tosc1SB); 
    Tosc1Latched <= Tosc1SB;
	 end if;	
end process;	

PCK0 <= Tosc1RE when AS0='1' else '1';
	
Prescaler_0_Cnt:process(cp2,ireset)
begin
if  ireset='0'  then                   -- Reset
 Pre0Cnt <= (others => '0'); 
  elsif  cp2='1' and cp2'event  then -- Clock
    if PCK0='1' then                   -- Clock enable
	 Pre0Cnt <= Pre0Cnt+1;
	end if;
	   end if;
end process;					

Prescaler_0:process(cp2,ireset)
begin
if  ireset='0'  then                   -- Reset
 PCK08 <= '0';
 PCK032 <= '0';
 PCK064 <= '0';
 PCK0128 <= '0';
 PCK0256 <= '0';
 PCK01024 <= '0';
 PCK0_Del <= '0';
  elsif  cp2='1' and cp2'event  then -- Clock
	 PCK08 <= (not PCK08 and(Pre0Cnt(0) and Pre0Cnt(1)and Pre0Cnt(2)) and PCK0_Del);
     PCK032 <= (not PCK032 and(Pre0Cnt(0) and Pre0Cnt(1) and Pre0Cnt(2) and Pre0Cnt(3) and Pre0Cnt(4)) and PCK0_Del); 
	 PCK064 <= (not PCK064 and(Pre0Cnt(0) and Pre0Cnt(1) and Pre0Cnt(2) and Pre0Cnt(3) and Pre0Cnt(4) and Pre0Cnt(5)) and PCK0_Del);
     PCK0128 <= (not PCK0128 and(Pre0Cnt(0) and Pre0Cnt(1) and Pre0Cnt(2) and Pre0Cnt(3) and Pre0Cnt(4) and Pre0Cnt(5) and Pre0Cnt(6) and PCK0_Del));
	 PCK0256 <= (not PCK0256 and(Pre0Cnt(0) and Pre0Cnt(1) and Pre0Cnt(2) and Pre0Cnt(3) and Pre0Cnt(4) and Pre0Cnt(5) and Pre0Cnt(6) and Pre0Cnt(7)) and PCK0_Del);
     PCK01024 <= (not PCK01024 and(Pre0Cnt(0) and Pre0Cnt(1) and Pre0Cnt(2) and Pre0Cnt(3) and Pre0Cnt(4) and Pre0Cnt(5) and Pre0Cnt(6) and Pre0Cnt(7) and Pre0Cnt(8) and Pre0Cnt(9)) and PCK0_Del); 
	 PCK0_Del <= PCK0;
  end if;
end process;					


TCNT0_En <= (PCK0 and not CS02 and not CS01 and CS00) or     -- PCK            "001" !!??
            (PCK08 and not CS02 and CS01 and not CS00) or    -- PCK/8		   "010"
			(PCK032 and not CS02 and CS01 and CS00)or	     -- PCK/32		   "011"
			(PCK064 and CS02 and not CS01 and not CS00)or    -- PCK/64		   "100"
			(PCK0128 and CS02 and not CS01 and CS00)or       -- PCK/64		   "101"
			(PCK0256 and CS02 and CS01 and not CS00)or       -- PCK/256		   "110"
            (PCK01024 and CS02 and CS01 and CS00);           -- PCK/1024	   "111"

-- -------------------------------------------------------------------------------------------
-- End of prescalers
-- -------------------------------------------------------------------------------------------
			
-- I/O address decoder
TCCR0_Sel  <= '1' when adr=TCCR0_Address else '0';
TCCR1A_Sel <= '1' when adr=TCCR1A_Address else '0';
TCCR1B_Sel <= '1' when adr=TCCR1B_Address else '0';
TCCR2_Sel  <= '1' when adr=TCCR2_Address else '0';
ASSR_Sel   <= '1' when adr=ASSR_Address else '0';
TIMSK_Sel  <= '1' when adr=TIMSK_Address else '0';
TIFR_Sel   <= '1' when adr=TIFR_Address else '0';
TCNT0_Sel  <= '1' when adr=TCNT0_Address else '0';
TCNT2_Sel  <= '1' when adr=TCNT2_Address else '0';
OCR0_Sel   <= '1' when adr=OCR0_Address else '0';
OCR2_Sel   <= '1' when adr=OCR2_Address else '0';
TCNT1H_Sel <= '1' when adr=TCNT1H_Address else '0';
TCNT1L_Sel <= '1' when adr=TCNT1L_Address else '0';
OCR1AH_Sel <= '1' when adr=OCR1AH_Address else '0';
OCR1AL_Sel <= '1' when adr=OCR1AL_Address else '0';
OCR1BH_Sel <= '1' when adr=OCR1BH_Address else '0';
OCR1BL_Sel <= '1' when adr=OCR1BL_Address else '0';
ICR1AH_Sel <= '1' when adr=ICR1AH_Address else '0';
ICR1AL_Sel <= '1' when adr=ICR1AL_Address else '0';
		

-- -------------------------------------------------------------------------------------------
-- Timer/Counter0
-- -------------------------------------------------------------------------------------------

-- Attention !!! This model only emulates the asynchronous mode of TCNT0. 
-- Real TCNT0 of ATmega103/603 operates with two separate clock sources.

Counter_0:process(cp2,ireset)
begin
if  ireset='0'  then                                      -- Reset
 TCNT0 <= (others => '0'); 
   elsif  cp2='1' and cp2'event  then                      -- Clock
    if (TCNT0_En or TCNT0_Clr or TCNT0_Ld_Imm)='1' then   -- Clock enable
	 TCNT0 <= TCNT0_In;
	end if;
	   end if;
end process;				

Counter_0_Inc:process(cp2,ireset)
begin
if  ireset='0'  then                                      -- Reset
  	TCNT0_Inc <= '0'; 
   elsif  cp2='1' and cp2'event  then                      -- Clock
	 TCNT0_Inc <= (not TCNT0_Inc and(TCNT0_En and not TCNT0_Ld_Imm))or(TCNT0_Inc and not(TCNT0_Ld_Imm));
	   end if;
end process;				


TCNT0_In <= TCNT0_Imm_In    when TCNT0_Ld_Imm = '1' else -- Immediate value (from dbus_in or TCNT0_Tmp)
            (others => '0') when TCNT0_Clr ='1'	else     -- Synchronous clear (for OCR)
			TCNT0-1 when TCNT0_Cnt_Dir='1' else			 -- Decrement (for PWM)
			TCNT0+1;									 -- Icrement (for counter and PWM)
				

TCNT0_Imm_In <= TCNT0_Tmp when (TCN0UB and not TCN0UB_Tmp and PCK0 and AS0)='1' else dbus_in;
TCNT0_Clr <= TCNT0_Cmp_Out and CTC0 and not PWM0;
TCNT0_Cmp_Out <= '1' when (TCNT0=OCR0 and OCR0/=x"00" and TCNT0_Inc='1') else '0';
TCNT0_Ld_Imm <= (TCNT0_Sel and iowe and not AS0) or (TCN0UB and not TCN0UB_Tmp and PCK0 and AS0);
	
	
TCCR0_Control:process(cp2,ireset)
begin
if ireset='0' then                  -- Reset
 TCNT0_Cnt_Dir <= '0';
 
 TCR0UB_Tmp <= '0';
 OCR0UB_Tmp <= '0';
 TCN0UB_Tmp <= '0';
 
 ASSR <= (others => '0');
  
elsif cp2='1' and cp2'event then  -- Clock
   TCNT0_Cnt_Dir <= (not TCNT0_Cnt_Dir and(TCNT0(7) and TCNT0(6) and TCNT0(5) and TCNT0(4) and TCNT0(3) and TCNT0(2) and TCNT0(1) and not TCNT0(0) and TCNT0_En and PWM0)) or           -- 0xFE
   (TCNT0_Cnt_Dir and not ((not TCNT0(7) and not TCNT0(6) and not TCNT0(5) and not TCNT0(4) and not TCNT0(3) and not TCNT0(2) and not TCNT0(1) and TCNT0(0)and TCNT0_En) or not PWM0));	-- 0x01
   
   TCR0UB_Tmp <= (not TCR0UB_Tmp and (TCCR0_Sel and iowe and AS0))or
   				 (TCR0UB_Tmp and not PCK0);
   OCR0UB_Tmp <= (not OCR0UB_Tmp and (OCR0_Sel and iowe and AS0))or
                 (OCR0UB_Tmp and not PCK0);
   TCN0UB_Tmp <= (not TCN0UB_Tmp and (TCNT0_Sel and iowe and AS0))or
   				 (TCN0UB_Tmp and not PCK0);
   
  TCR0UB <= (not TCR0UB and (TCCR0_Sel and iowe and AS0))or
   		    (TCR0UB and not (not TCR0UB_Tmp and PCK0));
  OCR0UB <= (not OCR0UB and (OCR0_Sel and iowe and AS0))or
            (OCR0UB and not (not OCR0UB_Tmp and PCK0));
  TCN0UB <= (not TCN0UB and (TCNT0_Sel and iowe and AS0))or
   			(TCN0UB and not (not TCN0UB_Tmp and PCK0));
  
  AS0    <= (not AS0 and(ASSR_Sel and iowe and dbus_in(3)))or 
            (AS0 and not(ASSR_Sel and iowe and not dbus_in(3)));		   

  end if;
end process;				
	
	
-- Temp registers of TCNT0

-- This register is used only when TCNT0 operates in the asynchronous mode
TCNT0_Temp_Control:process(cp2,ireset)
begin
if  ireset='0'  then                                      -- Reset
 TCCR0_Tmp <= (others => '0'); 
  elsif  cp2='1' and cp2'event  then                      -- Clock
    if (TCCR0_Sel and iowe and AS0)='1' then              -- Clock enable
	 TCCR0_Tmp <= dbus_in;
	end if;
	   end if;
end process;				

-- This register is used only when TCNT0 operates in the asynchronous mode
TCNT0_Temp_CNT:process(cp2,ireset)
begin
if  ireset='0'  then                                      -- Reset
 TCNT0_Tmp <= (others => '0'); 
  elsif  cp2='1' and cp2'event  then                      -- Clock
    if (TCNT0_Sel and iowe and AS0)='1' then              -- Clock enable
	 TCNT0_Tmp <= dbus_in;
	end if;
	   end if;
end process;				

TCNT0_Temp_Compare:process(cp2,ireset)
begin
if  ireset='0'  then                                      -- Reset
 OCR0_Tmp <= (others => '0'); 
  elsif  cp2='1' and cp2'event  then                      -- Clock
    if (OCR0_Sel and iowe )='1' then     -- Clock enable ??!! was "and (AS0 or PWM0)"
	 OCR0_Tmp <= dbus_in;
	end if;
	   end if;
end process;				

-- Main registers of TCNT0
TCNT0_Control:process(cp2,ireset)
begin
if  ireset='0'  then                                      -- Reset
 TCCR0 <= (others => '0'); 
  elsif  cp2='1' and cp2'event  then                      -- Clock
    if ((TCCR0_Sel and iowe and not AS0)or
		(TCR0UB and not TCR0UB_Tmp and PCK0 and AS0))='1' then              -- Clock enable
	 TCCR0(6 downto 0) <= TCCR0_In(6 downto 0);
	end if;
	   end if;
end process;				

TCCR0_In <= TCCR0_Tmp when (TCR0UB and not TCR0UB_Tmp and PCK0 and AS0)='1' else dbus_in;

TCNT0_Compare:process(cp2,ireset)
begin
if  ireset='0'  then                                      -- Reset
 OCR0 <= (others => '0'); 
  elsif  cp2='1' and cp2'event  then                      -- Clock
    if  ((((OCR0_Sel and iowe and not AS0)or                              -- Synchronous non PWM mode 
		(OCR0UB and not OCR0UB_Tmp and PCK0 and AS0))and not PWM0)or      -- Asynchronous non PWM mode
		(TCNT0(7) and TCNT0(6) and TCNT0(5) and TCNT0(4) and TCNT0(3) and TCNT0(2) and TCNT0(1) and TCNT0(0) and PWM0 and TCNT0_En)) -- Reload OCR0 with new value when TCNT0=0xFF (for PWM)
		 ='1' then     -- Clock enable ??!!
	  OCR0 <= OCR0_In;
	end if;
	   end if;
end process;				

-- OCR0 can be loaded from OCR0_Tmp (asynchronous non PWM mode, synchronous PWM mode ) or from dbus_in	(synchronous non PWM mode only)
OCR0_In <= OCR0_Tmp when ((TCR0UB and not TCR0UB_Tmp and PCK0 and AS0 and not PWM0)or
(TCNT0(7) and TCNT0(6) and TCNT0(5) and TCNT0(4) and TCNT0(3) and TCNT0(2) and TCNT0(1) and TCNT0(0) and PWM0 and TCNT0_En))='1' else dbus_in;

--OC0_PWM0 output 
-- Attention!!! In the real ATmega103 chip this output of the TCNT0 combined with PB4

Output_TCNT0:process(cp2,ireset)
begin
if  ireset='0'  then                                      -- Reset
 OC0_PWM0_Int <= '0'; 
  elsif  cp2='1' and cp2'event  then                      -- Clock
     
case OC0_PWM0_Int is
-- Set
when '0' =>
if (not PWM0 and COM00 and TCNT0_Cmp_Out)='1' or	 -- Compare mode - Toggle/Set ??!!
((PWM0 and COM01 and TCNT0_En)='1' and				 -- PWM modes : 
((TCNT0=x"FF" and COM00='1' and OCR0_Tmp=x"00")or	 -- Inverted PWM
(TCNT0=x"FF" and COM00='0' and OCR0_Tmp=x"FF")or	 -- Non-inverted PWM
(TCNT0=x"FF" and COM00='1' and OCR0_Tmp/=x"FF")or	 -- Inverted PWM
(TCNT0/=x"FF" and TCNT0_Cnt_Dir='1' and COM00='0' and OCR0_Tmp/=x"00" and OCR0=TCNT0)or	 -- Non-inverted PWM	
(TCNT0/=x"FF" and TCNT0_Cnt_Dir='0' and COM00='1' and OCR0_Tmp/=x"00" and OCR0=TCNT0)))	 -- Inverted PWM	
then 	
OC0_PWM0_Int <= '1';
end if;
-- Reset
when '1' =>	
if (not PWM0 and (COM01 or COM00) and TCNT0_Cmp_Out)='1' or	-- Compare mode - Toggle/Reset ??!!
((PWM0 and COM01 and TCNT0_En)='1' and				-- PWM modes : 
((TCNT0=x"FF" and COM00='1' and OCR0_Tmp=x"FF")or	-- Inverted PWM
(TCNT0=x"FF" and COM00='0' and OCR0_Tmp=x"00")or	-- Non-inverted PWM
(TCNT0=x"FF" and COM00='0' and OCR0_Tmp/=x"FF")or	-- Non-inverted PWM
(TCNT0/=x"FF" and TCNT0_Cnt_Dir='0' and COM00='0' and OCR0_Tmp/=x"00" and OCR0=TCNT0)or	 -- Non-inverted PWM	
(TCNT0/=x"FF" and TCNT0_Cnt_Dir='1' and COM00='1' and OCR0_Tmp/=x"00" and OCR0=TCNT0)))	 -- Inverted PWM		
then 	
OC0_PWM0_Int <= '0';
end if;

when others => null;
	
end case;	
	  
end if;																		  
end process;				

OC0_PWM0 <= OC0_PWM0_Int;

-- -------------------------------------------------------------------------------------------
-- Timer/Counter0
-- -------------------------------------------------------------------------------------------


-- -------------------------------------------------------------------------------------------
-- Timer/Counter 2
-- -------------------------------------------------------------------------------------------

Counter_2:process(cp2,ireset)
begin
if ireset='0' then                                        -- Reset
 TCNT2 <= (others => '0'); 
   elsif cp2='1' and cp2'event then                       -- Clock
    if (TCNT2_En or TCNT2_Clr or TCNT2_Ld_Imm)='1' then   -- Clock enable
	 TCNT2 <= TCNT2_In;
	end if;
	   end if;
end process;				

TCNT2_Ld_Imm <= TCNT2_Sel and iowe; -- Write to TCNT2

Counter_2_Inc:process(cp2,ireset)
begin
if ireset='0' then                                      -- Reset
  	TCNT2_Inc <= '0'; 
   elsif cp2='1' and cp2'event then                      -- Clock
	 TCNT2_Inc <= (not TCNT2_Inc and(TCNT2_En and not (TCNT2_Sel and iowe)))or
	              (TCNT2_Inc and not (TCNT2_Sel and iowe));
	   end if;
end process;				

-- Input muultiplexer of TCCR2 register
TCNT2_In <= dbus_in when TCNT2_Ld_Imm = '1' else -- Immediate value (from dbus_in)
            (others => '0') when TCNT2_Clr ='1'	else     -- Synchronous clear (for OCR)
			TCNT2-1 when TCNT2_Cnt_Dir='1' else			 -- Decrement (for PWM)
			TCNT2+1;									 -- Icrement (for counter and PWM)

TCNT2_Clr <= TCNT2_Cmp_Out and CTC2 and not PWM2;
TCNT2_Cmp_Out <= '1' when TCNT2=OCR2 and OCR2/=x"00" and TCNT2_Inc='1' else '0';
	
TCCR2_Control:process(cp2,ireset)
begin
if ireset='0' then                  -- Reset
 TCNT2_Cnt_Dir <= '0';
 elsif cp2='1' and cp2'event then  -- Clock
   TCNT2_Cnt_Dir <= (not TCNT2_Cnt_Dir and(TCNT2(7) and TCNT2(6) and TCNT2(5) and TCNT2(4) and TCNT2(3) and TCNT2(2) and TCNT2(1) and not TCNT2(0) and TCNT2_En and PWM2)) or           -- 0xFE
                    (TCNT2_Cnt_Dir and not ((not TCNT2(7) and not TCNT2(6) and not TCNT2(5) and not TCNT2(4) and not TCNT2(3) and not TCNT2(2) and not TCNT2(1) and TCNT2(0)and TCNT2_En) or not PWM2));	-- 0x01
  end if;
end process;					
	
TCNT2_Temp_Compare:process(cp2,ireset)
begin
if ireset='0' then                                      -- Reset
 OCR2_Tmp <= (others => '0'); 
  elsif cp2='1' and cp2'event then                      -- Clock
    if (OCR2_Sel and iowe )='1' then                    -- Clock enable 
	 OCR2_Tmp <= dbus_in;
	end if;
	   end if;
end process;				

-- Main registers of TCNT2
TCNT2_Control:process(cp2,ireset)
begin
if ireset='0' then                                      -- Reset
 TCCR2 <= (others => '0'); 
  elsif cp2='1' and cp2'event then                      -- Clock
    if (TCCR2_Sel and iowe)='1' then                    -- Clock enable
	 TCCR2(6 downto 0) <= dbus_in(6 downto 0);
	end if;
	   end if;
end process;				

TCNT2_Compare:process(cp2,ireset)
begin
if ireset='0' then                                      -- Reset
 OCR2 <= (others => '0'); 
  elsif cp2='1' and cp2'event then                      -- Clock
    if  ((OCR2_Sel and iowe)or            -- Non PWM mode 
		(TCNT2(7) and TCNT2(6) and TCNT2(5) and TCNT2(4) and TCNT2(3) and TCNT2(2) and TCNT2(1) and TCNT2(0) and PWM2 and TCNT2_En)) -- Reload OCR2 with new value when TCNT2=0xFF (for PWM)
		                          ='1' then     -- Clock enable ??!!
	  OCR2 <= OCR2_In;
	end if;
	   end if;
end process;				

-- OCR2 can be loaded from OCR2_Tmp (PWM mode ) or from dbus_in	(non PWM mode only)
OCR2_In <= OCR2_Tmp when ((TCNT2(7) and TCNT2(6) and TCNT2(5) and TCNT2(4) and TCNT2(3) and TCNT2(2) and TCNT2(1) and TCNT2(0) and PWM2 and TCNT2_En))='1' else dbus_in;

--OC2_PWM2 output 
-- Attention!!! In the real ATmega103 chip this output of the TCNT2 combined with PB7

Output_TCNT2:process(cp2,ireset)
begin
if ireset='0' then                                      -- Reset
 OC2_PWM2_Int <= '0'; 
  elsif cp2='1' and cp2'event then                      -- Clock
     
case OC2_PWM2_Int is
-- Set
when '0' =>
if (not PWM2 and COM20 and TCNT2_Cmp_Out)='1' or	 -- Compare mode - Toggle/Set ??!!
((PWM2 and COM21 and TCNT2_En)='1' and				 -- PWM modes : 
((TCNT2=x"FF" and COM20='1' and OCR2_Tmp=x"00")or	 -- Inverted PWM
(TCNT2=x"FF" and COM20='0' and OCR2_Tmp=x"FF")or	 -- Non-inverted PWM
(TCNT2=x"FF" and COM20='1' and OCR2_Tmp/=x"FF")or	 -- Inverted PWM
(TCNT2/=x"FF" and TCNT2_Cnt_Dir='1' and COM20='0' and OCR2_Tmp/=x"00" and OCR2=TCNT2)or	 -- Non-inverted PWM	
(TCNT2/=x"FF" and TCNT2_Cnt_Dir='0' and COM20='1' and OCR2_Tmp/=x"00" and OCR2=TCNT2)))	 -- Inverted PWM	
then 	
OC2_PWM2_Int <= '1';
end if;
-- Reset
when '1' =>	
if (not PWM2 and (COM21 or COM20) and TCNT2_Cmp_Out)='1' or	-- Compare mode - Toggle/Reset ??!!
((PWM2 and COM21 and TCNT2_En)='1' and				-- PWM modes : 
((TCNT2=x"FF" and COM20='1' and OCR2_Tmp=x"FF")or	-- Inverted PWM
(TCNT2=x"FF" and COM20='0' and OCR2_Tmp=x"00")or	-- Non-inverted PWM
(TCNT2=x"FF" and COM20='0' and OCR2_Tmp/=x"FF")or	-- Non-inverted PWM
(TCNT2/=x"FF" and TCNT2_Cnt_Dir='0' and COM20='0' and OCR2_Tmp/=x"00" and OCR2=TCNT0)or	 -- Non-inverted PWM	
(TCNT2/=x"FF" and TCNT2_Cnt_Dir='1' and COM20='1' and OCR2_Tmp/=x"00" and OCR2=TCNT0)))	 -- Inverted PWM		
then 	
OC2_PWM2_Int <= '0';
end if;

when others => null;
	
end case;	
	  
end if;																		  
end process;				

OC2_PWM2 <= OC2_PWM2_Int;	
	
-- -------------------------------------------------------------------------------------------
-- End of Timer/Counter2
-- -------------------------------------------------------------------------------------------

-- -------------------------------------------------------------------------------------------
-- Common (Control/Interrupt) bits
-- ------------------------------------------------------------------------------------------- 

TIFR_Bits:process(cp2,ireset)
begin
if ireset='0' then 
TIFR <= (others => '0');
elsif  cp2='1' and cp2'event  then
-- Timer/Counter0
TOV0 <= (not TOV0 and (TCNT0_En and(
(not PWM0 and TCNT0(7) and TCNT0(6) and TCNT0(5) and TCNT0(4) and TCNT0(3) and TCNT0(2) and TCNT0(1) and TCNT0(0))or
(PWM0 and not(TCNT0(7) or TCNT0(6) or TCNT0(5) or TCNT0(4) or TCNT0(3) or TCNT0(2) or TCNT0(1)) and TCNT0(0) and TCNT0_Cnt_Dir ))))or	
(TOV0 and not(TC0OvfIRQ_Ack or(TIFR_Sel and iowe and dbus_in(0))));
--OCF0 <= (not OCF0 and(not PWM0 and COM00 and TCNT0_Cmp_Out))or(OCF0 and not(TC0CmpIRQ_Ack or(TIFR_Sel and iowe and dbus_in(1))));
OCF0 <= (not OCF0 and TCNT0_En and TCNT0_Cmp_Out)or
        (OCF0 and not(TC0CmpIRQ_Ack or(TIFR_Sel and iowe and dbus_in(1))));

-- Timer/Counter2
TOV2 <= (not TOV2 and (TCNT2_En and(
(not PWM2 and TCNT2(7) and TCNT2(6) and TCNT2(5) and TCNT2(4) and TCNT2(3) and TCNT2(2) and TCNT2(1) and TCNT2(0))or
(PWM2 and not(TCNT2(7) or TCNT2(6) or TCNT2(5) or TCNT2(4) or TCNT2(3) or TCNT2(2) or TCNT2(1)) and TCNT2(0) and TCNT2_Cnt_Dir ))))or	
(TOV2 and not(TC2OvfIRQ_Ack or(TIFR_Sel and iowe and dbus_in(6))));
--OCF2 <= (not OCF2 and(not PWM2 and COM20 and TCNT2_Cmp_Out))or(OCF2 and not(TC2CmpIRQ_Ack or(TIFR_Sel and iowe and dbus_in(7))));
OCF2 <= (not OCF2 and TCNT2_En and TCNT2_Cmp_Out)or
        (OCF2 and not(TC2CmpIRQ_Ack or(TIFR_Sel and iowe and dbus_in(7))));

end if;	
end process;

TIMSK_Bits:process(cp2,ireset)
begin
if ireset='0' then 
TIMSK <= (others => '0');
elsif  cp2='1' and cp2'event  then
if (TIMSK_Sel and iowe)='1' then
TIMSK <= dbus_in;	
end if;	
end if;	
end process;

-- Interrupt flags of Timer/Counter0
TC0OvfIRQ <= TOV0 and TOIE0;	  -- Interrupt on overflow of TCNT0
TC0CmpIRQ <= OCF0 and OCIE0;	  -- Interrupt on compare match	of TCNT0

-- Interrupt flags of Timer/Counter0
TC2OvfIRQ <= TOV2 and TOIE2;	  -- Interrupt on overflow of TCNT2
TC2CmpIRQ <= OCF2 and OCIE2;	  -- Interrupt on compare match	of TCNT2

-- -------------------------------------------------------------------------------------------
-- End of common (Control/Interrupt) bits
-- -------------------------------------------------------------------------------------------

-- -------------------------------------------------------------------------------------------
-- Bus interface
-- -------------------------------------------------------------------------------------------
out_en <= (TCCR0_Sel or TCCR1A_Sel or TCCR1B_Sel or TCCR2_Sel or ASSR_Sel or TIMSK_Sel or 
          TIFR_Sel  or TCNT0_Sel  or TCNT2_Sel or OCR0_Sel or OCR2_Sel or TCNT1H_Sel or 
		  TCNT1L_Sel or OCR1AH_Sel or OCR1AL_Sel or OCR1BH_Sel or OCR1BL_Sel or ICR1AH_Sel or  
          ICR1AL_Sel) and iore;

Common_Out_Mux: for i in dbus_out'range generate
dbus_out(i)	<= (TCCR0(i) and (TCCR0_Sel and not AS0))or  -- TCCR0 (Synchronous mode of TCNT0)
               (TCCR0_Tmp(i) and (TCCR0_Sel and AS0))or	 -- TCCR0 (Asynchronous mode of TCNT0)
			   (OCR0(i) and (OCR0_Sel and not AS0))or	 -- OCR0  (Synchronous mode of TCNT0)
               (OCR0_Tmp(i) and (OCR0_Sel and AS0)) or	 -- OCR0  (Asynchronous mode of TCNT0)
               (TCNT0(i) and TCNT0_Sel) or 				 -- TCNT0 (Both modes of TCNT0)
			   
			   (TCCR2(i) and TCCR2_Sel )or               -- TCCR2
               (OCR2(i) and OCR2_Sel)or	                 -- OCR2
               (TCNT2(i) and TCNT2_Sel) or 				 -- TCNT2
			   			   
			   (TIFR(i) and TIFR_Sel) or				 -- TIFR
			   (TIMSK(i) and TIMSK_Sel);				 -- TIMSK
end generate;			  
-- -------------------------------------------------------------------------------------------
-- End of bus interface
-- -------------------------------------------------------------------------------------------

end rtl;
