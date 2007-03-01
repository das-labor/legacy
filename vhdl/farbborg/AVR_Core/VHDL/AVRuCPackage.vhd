-- *****************************************************************************************
-- AVR constants and type declarations
-- Version 0.3
-- Modified 25.03.2003
-- Designed by Ruslan Lepetenok
-- *****************************************************************************************

library	IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
--use IEEE.std_logic_unsigned.all;

package AVRuCPackage is
-- Old package
constant ext_mux_in_num : integer := 63;
type ext_mux_din_type is array(0 to ext_mux_in_num) of std_logic_vector(7 downto 0);
subtype ext_mux_en_type  is std_logic_vector(0 to ext_mux_in_num);
-- End of old package

-- I/O port addresses
constant IOAdrWidth    : positive := 6;

-- I/O register file
constant RAMPZ_Address : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#3B#,IOAdrWidth);
constant SPL_Address   : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#3D#,IOAdrWidth);
constant SPH_Address   : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#3E#,IOAdrWidth);
constant SREG_Address  : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#3F#,IOAdrWidth);
-- End of I/O register file

-- UART
constant UDR_Address  : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#0C#,IOAdrWidth);
constant UBRR_Address : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#09#,IOAdrWidth);
constant USR_Address  : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#0B#,IOAdrWidth);
constant UCR_Address  : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#0A#,IOAdrWidth);
-- End of UART	

-- Timer/Counter
constant TCCR0_Address  : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#33#,IOAdrWidth);
constant TCCR1A_Address : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#2F#,IOAdrWidth);
constant TCCR1B_Address : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#2E#,IOAdrWidth);
constant TCCR2_Address  : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#25#,IOAdrWidth);
constant ASSR_Address   : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#30#,IOAdrWidth);
constant TIMSK_Address  : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#37#,IOAdrWidth);
constant TIFR_Address   : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#36#,IOAdrWidth);
constant TCNT0_Address  : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#32#,IOAdrWidth);
constant TCNT2_Address  : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#24#,IOAdrWidth);
constant OCR0_Address   : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#31#,IOAdrWidth);
constant OCR2_Address   : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#23#,IOAdrWidth);
constant TCNT1H_Address : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#2D#,IOAdrWidth);
constant TCNT1L_Address : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#2C#,IOAdrWidth);
constant OCR1AH_Address : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#2B#,IOAdrWidth);
constant OCR1AL_Address : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#2A#,IOAdrWidth);
constant OCR1BH_Address : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#29#,IOAdrWidth);
constant OCR1BL_Address : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#28#,IOAdrWidth);
constant ICR1AH_Address : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#27#,IOAdrWidth);
constant ICR1AL_Address : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#26#,IOAdrWidth);
-- End of Timer/Counter	

-- Service module
constant MCUCR_Address : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#35#,IOAdrWidth);
constant EIMSK_Address : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#39#,IOAdrWidth);
constant EIFR_Address  : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#38#,IOAdrWidth);
constant EICR_Address  : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#3A#,IOAdrWidth);
constant MCUSR_Address : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#34#,IOAdrWidth);
constant XDIV_Address  : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#3C#,IOAdrWidth);
-- End of service module

-- PORTA addresses 
constant PORTA_Address : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#1B#,IOAdrWidth);
constant DDRA_Address  : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#1A#,IOAdrWidth);
constant PINA_Address  : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#19#,IOAdrWidth);

-- PORTB addresses 
constant PORTB_Address : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#18#,IOAdrWidth);
constant DDRB_Address  : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#17#,IOAdrWidth);
constant PINB_Address  : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#16#,IOAdrWidth);

-- PORTC addresses 
constant PORTC_Address : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#15#,IOAdrWidth);

-- PORTD addresses 
constant PORTD_Address : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#12#,IOAdrWidth);
constant DDRD_Address  : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#11#,IOAdrWidth);
constant PIND_Address  : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#10#,IOAdrWidth);

-- PORTE addresses 
constant PORTE_Address : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#03#,IOAdrWidth);
constant DDRE_Address  : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#02#,IOAdrWidth);
constant PINE_Address  : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#01#,IOAdrWidth);

-- PORTF addresses
constant PINF_Address  : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#00#,IOAdrWidth);

-- Analog to digital converter
constant ADCL_Address  : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#04#,IOAdrWidth);
constant ADCH_Address  : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#05#,IOAdrWidth);
constant ADCSR_Address : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#06#,IOAdrWidth);
constant ADMUX_Address : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#07#,IOAdrWidth);

-- Analog comparator
constant ACSR_Address  : std_logic_vector(IOAdrWidth-1 downto 0) := CONV_STD_LOGIC_VECTOR(16#08#,IOAdrWidth);

-- Function declaration
function LOG2(Number : positive) return natural;

end AVRuCPackage;

package	body AVRuCPackage is

-- Functions	
function LOG2(Number : positive) return natural is
variable Temp : positive := 1;
begin
if Number=1 then 
 return 0;
  else 
   for i in 1 to integer'high loop
    Temp := 2*Temp; 
     if Temp>=Number then 
      return i;
     end if;
end loop;
end if;	
end LOG2;	
-- End of functions	

end AVRuCPackage;	
