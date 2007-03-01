--************************************************************************************************
-- Top entity for AVR microcontroller (for simulation)
-- Version 1.4
-- Designed by Ruslan Lepetenok 
-- Modified 20.05.2003
--************************************************************************************************

library IEEE;
use IEEE.std_logic_1164.all;

use WORK.AVRuCPackage.all;

entity top_avr_core_sim is generic(InsertWaitSt : boolean := FALSE;
	         					   RAMSize      : positive := 128);
		                    port(
                                ireset : in std_logic;
                                cp2    : in std_logic;
	 	 
                                porta  : inout std_logic_vector(7 downto 0);
                                portb  : inout std_logic_vector(7 downto 0);
	                            -- UART 
	                            rxd    : in  std_logic;
	                            txd    : out std_logic;
                                -- External interrupt inputs
								nINT0  : in  std_logic;
								nINT1  : in  std_logic;
								nINT2  : in  std_logic;
								nINT3  : in  std_logic;
								INT4   : in  std_logic;
								INT5   : in  std_logic;
								INT6   : in  std_logic;
								INT7   : in  std_logic
								);
end top_avr_core_sim;

architecture Struct of top_avr_core_sim is

component pport is 
	generic(
	        PORTX_Adr : std_logic_vector(IOAdrWidth-1 downto 0);
	        DDRX_Adr  : std_logic_vector(IOAdrWidth-1 downto 0);
			PINX_Adr  : std_logic_vector(IOAdrWidth-1 downto 0)
			);
	port(
	                   -- AVR Control
               ireset     : in std_logic;
               cp2	      : in std_logic;
               adr        : in std_logic_vector(5 downto 0);
               dbus_in    : in std_logic_vector(7 downto 0);
               dbus_out   : out std_logic_vector(7 downto 0);
               iore       : in std_logic;
               iowe       : in std_logic;
               out_en     : out std_logic; 
			            -- External connection
			   portx      : out std_logic_vector(7 downto 0);
			   ddrx       : out std_logic_vector(7 downto 0);
			   pinx       : in  std_logic_vector(7 downto 0));
end component;

component external_mux is port (
		  ramre              : in  std_logic;
		  dbus_out           : out std_logic_vector (7 downto 0);
		  ram_data_out       : in  std_logic_vector (7 downto 0);
		  io_port_bus        : in  ext_mux_din_type;
		  io_port_en_bus     : in  ext_mux_en_type;
          irqack             : in  std_logic;
          irqackad           : in  std_logic_vector(4 downto 0);		  
		  ind_irq_ack        : out std_logic_vector(22 downto 0)		  
		  );
end component;

component Service_Module is port(
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
end component;

component RAMDataReg is port(	                   
               ireset      : in  std_logic;
               cp2	       : in  std_logic;
               cpuwait     : in  std_logic;
			   RAMDataIn   : in  std_logic_vector(7 downto 0);
			   RAMDataOut  : out std_logic_vector(7 downto 0)
	                     );
end component;


-- SIMPLE TIMER **********************************
component simple_timer is port(
ireset        : in std_logic;
cp2           : in std_logic;
irqline       : out std_logic;
timer_irqack  : in std_logic
							  );
end component;
-- ***********************************************


component Timer_Counter is port(
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
end component;


--*************** UART ***************************
component uart is port(
	                   -- AVR Control
               ireset     : in std_logic;
               cp2	      : in std_logic;
               adr        : in std_logic_vector(5 downto 0);
               dbus_in    : in std_logic_vector(7 downto 0);
               dbus_out   : out std_logic_vector(7 downto 0);
               iore       : in std_logic;
               iowe       : in std_logic;
               out_en     : out std_logic; 

                       --UART
               rxd        : in std_logic;
               rx_en      : out std_logic;
               txd        : out std_logic;
               tx_en      : out std_logic;

                       --IRQ
               txcirq     : out std_logic;
               txc_irqack : in std_logic;
               udreirq    : out std_logic;
			   rxcirq     : out std_logic);
end component;


component PROM is port (
address_in : in  std_logic_vector (15 downto 0);
data_out   : out std_logic_vector (15 downto 0));
end component;

component DataRAM is 
		  generic(RAMSize :positive);
	      port (
          cp2         : in std_logic;
  	      address     : in  std_logic_vector (LOG2(RAMSize)-1 downto 0);
		  ramwe		  : in  std_logic;
		  din         : in  std_logic_vector (7 downto 0);
		  dout        : out	std_logic_vector (7 downto 0));
end component;

component CPUWaitGenerator is
			   generic(InsertWaitSt : boolean);
	           port(
	           ireset  : in  std_logic;
			   cp2     : in  std_logic;
			   ramre   : in  std_logic;
			   ramwe   : in  std_logic;
			   cpuwait : out std_logic                 
			        );
end component;


-- Core itself
component avr_core is port
(

cp2      : in std_logic;
ireset   : in std_logic;
cpuwait  : in std_logic;

-- PROGRAM MEMORY PORTS
pc       : out std_logic_vector (15 downto 0);   
inst     : in std_logic_vector (15 downto 0);	

-- I/O REGISTERS PORTS
adr      : out std_logic_vector (5 downto 0); 	
iore     : out std_logic;                       
iowe     : out std_logic;						

-- DATA MEMORY PORTS
ramadr   : out std_logic_vector (15 downto 0);
ramre    : out std_logic;
ramwe    : out std_logic;

dbusin   : in std_logic_vector (7 downto 0);
dbusout  : out std_logic_vector (7 downto 0);

-- INTERRUPTS PORT
irqlines : in std_logic_vector (22 downto 0);
irqack   : out std_logic;
irqackad : out std_logic_vector(4 downto 0)

);

end component;


-- ############################## Signals connected directly to the core ##########################################

signal sg_core_cpuwait  : std_logic :='0';                    

-- Program memory
signal sg_core_pc   : std_logic_vector (15 downto 0):=(others=>'0'); -- PROM address
signal sg_core_inst : std_logic_vector (15 downto 0):=(others=>'0'); -- PROM data

-- I/O registers
signal sg_core_adr  : std_logic_vector (5 downto 0):=(others=>'0');
signal sg_core_iore : std_logic :='0';                    
signal sg_core_iowe : std_logic :='0';

-- Data memery
signal sg_core_ramadr : std_logic_vector (15 downto 0):=(others=>'0');
signal sg_core_ramre  : std_logic :='0';
signal sg_core_ramwe  : std_logic :='0';

signal sg_core_dbusin   : std_logic_vector (7 downto 0):=(others=>'0');
signal sg_core_dbusout  : std_logic_vector (7 downto 0):=(others=>'0');

-- Interrupts
signal sg_core_irqlines : std_logic_vector(22 downto 0) :=(others=>'0');
signal sg_core_irqack   : std_logic :='0';
signal sg_core_irqackad : std_logic_vector(4 downto 0) :=(others=>'0');

-- ###############################################################################################################

-- ############################## Signals connected directly to the SRAM controller ###############################

signal sg_ram_din       : std_logic_vector (7 downto 0):=(others=>'0');
signal sg_ram_dout      : std_logic_vector (7 downto 0):=(others=>'0');

-- ###############################################################################################################


-- ############################## Signals connected directly to the I/O registers ################################
-- PortA
signal sg_porta_dbusout : std_logic_vector (7 downto 0) := (others => '0');
signal sg_porta_out_en  : std_logic := '0';

-- PortB
signal sg_portb_dbusout : std_logic_vector (7 downto 0) := (others => '0');
signal sg_portb_out_en : std_logic := '0';

-- UART
signal sg_uart_dbusout : std_logic_vector (7 downto 0) := (others => '0');
signal sg_uart_out_en  : std_logic := '0';

signal sg_uart_tx_en  : std_logic := '0';
signal sg_uart_rx_en  : std_logic := '0';

-- Timer/Counter
signal sg_tc_dbusout : std_logic_vector (7 downto 0) := (others => '0');
signal sg_tc_out_en  : std_logic := '0';

-- Service module
signal sg_sm_dbusout : std_logic_vector (7 downto 0) := (others => '0');
signal sg_sm_out_en  : std_logic := '0';

-- ###############################################################################################################


-- ####################### Signals connected directly to the external multiplexer ################################
signal   sg_io_port_out     : ext_mux_din_type := (others => "00000000");
signal   sg_io_port_out_en  : ext_mux_en_type      := (others => '0');
signal   sg_ind_irq_ack     : std_logic_vector(sg_core_irqlines'range) := (others => '0');
-- ###############################################################################################################

-- ********************************** External interrupts signals ********************
signal sg_ext_int_req : std_logic_vector(7 downto 0) := (others => '0');

-- ***********************************************************************************

-- ################################## Reset signals #############################################
signal sg_nrst_cp2    : std_logic :='0';
-- ##############################################################################################

-- Port signals
signal PortAReg : std_logic_vector(porta'range) := (others => '0');
signal DDRAReg  : std_logic_vector(porta'range) := (others => '0');

signal PortBReg : std_logic_vector(porta'range) := (others => '0');
signal DDRBReg  : std_logic_vector(porta'range) := (others => '0');


begin

TESTING_CORE: component avr_core  port map
(

cp2      => cp2,    
ireset   => ireset,
cpuwait  => sg_core_cpuwait,

-- PROGRAM MEMORY PORTS
pc       => sg_core_pc,
inst     => sg_core_inst,

-- I/O REGISTERS PORTS
adr      => sg_core_adr,
iore     => sg_core_iore,
iowe     => sg_core_iowe,

-- DATA MEMORY PORTS
ramadr  => sg_core_ramadr,
ramre   => sg_core_ramre,
ramwe   => sg_core_ramwe,

dbusin   => sg_core_dbusin,
dbusout  => sg_core_dbusout,

-- INTERRUPTS PORT
irqlines => sg_core_irqlines, 
irqack   => sg_core_irqack,
irqackad => sg_core_irqackad 

);

RAM_Data_Register:component RAMDataReg port map(	                   
               ireset      => ireset,
               cp2	       => cp2,
               cpuwait     => sg_core_cpuwait,
			   RAMDataIn   => sg_core_dbusout,
			   RAMDataOut  => sg_ram_din
	                     );

-- Program memory
PM:component PROM port map(
               address_in => sg_core_pc,
               data_out   => sg_core_inst
			              );

-- Data memory
DM:component DataRAM 
		  generic map(RAMSize => RAMSize)
	      port map(
          cp2         => cp2,
  	      address     => sg_core_ramadr(LOG2(RAMSize)-1 downto 0),
		  ramwe		  => sg_core_ramwe,
		  din         => sg_ram_din,
		  dout        => sg_ram_dout);

		  
-- cpuwait generation
cpuwait_Gen:component CPUWaitGenerator 
			   generic map(InsertWaitSt => InsertWaitSt)
	           port map(
	           ireset  => ireset,
			   cp2     => cp2,
			   ramre   => sg_core_ramre,
			   ramwe   => sg_core_ramwe,
			   cpuwait => sg_core_cpuwait
			        );

					
EXT_MUX:component external_mux port map(
		  ramre              => sg_core_ramre,		  -- ramre output of the core
		  dbus_out           => sg_core_dbusin,       -- Data input of the core 
		  ram_data_out       => sg_ram_dout,          -- Data output of the RAM
		  io_port_bus        => sg_io_port_out,       -- Data outputs of the I/O
		  io_port_en_bus     => sg_io_port_out_en,    -- Out enable outputs of I/O
		  irqack             => sg_core_irqack,		  
		  irqackad			 => sg_core_irqackad,
		  ind_irq_ack		 =>	sg_ind_irq_ack		  -- Individual interrupt acknolege for the peripheral
                                            );


-- ******************  PORTA **************************				
PORTA_COMP:component pport  
	generic map(PORTX_Adr => PORTA_Address,DDRX_Adr => DDRA_Address,PINX_Adr => PINA_Address)
	port map(
	                   -- AVR Control
               ireset     => ireset,
               cp2	      => cp2, 
               adr        => sg_core_adr,
               dbus_in    => sg_core_dbusout,
               dbus_out   => sg_porta_dbusout,
               iore       => sg_core_iore,
               iowe       => sg_core_iowe,
               out_en     => sg_porta_out_en,
			            -- External connection
			   portx      => PortAReg,
			   ddrx       => DDRAReg,
			   pinx       => porta);
						
-- PORTA connection to the external multiplexer
sg_io_port_out(0) <= sg_porta_dbusout;
sg_io_port_out_en(0) <= sg_porta_out_en;

-- Tri-state control for PORTA
PortAZCtrl:for i in porta'range generate
porta(i) <= PortAReg(i) when DDRAReg(i)='1' else 'Z'; 	
end generate;

-- ******************  PORTB **************************		
PORTB_COMP:component pport 
	generic map (PORTX_Adr => PORTB_Address,DDRX_Adr => DDRB_Address,PINX_Adr => PINB_Address)
	port map(
	                   -- AVR Control
               ireset     => ireset,
               cp2	      => cp2, 
               adr        => sg_core_adr,
               dbus_in    => sg_core_dbusout,
               dbus_out   => sg_portb_dbusout,
               iore       => sg_core_iore,
               iowe       => sg_core_iowe,
               out_en     => sg_portb_out_en,
			            -- External connection
			   portx      => PortBReg,
			   ddrx       => DDRBReg,
			   pinx       => portb);

-- PORTB connection to the external multiplexer
sg_io_port_out(1) <= sg_portb_dbusout;
sg_io_port_out_en(1) <= sg_portb_out_en;

-- Tri-state control for PORTB
PortBZCtrl:for i in portb'range generate
portb(i) <= PortBReg(i) when DDRBReg(i)='1' else 'Z'; 	
end generate;

-- ************************************************

-- Simple timer		  
TIMER:component simple_timer port map(
ireset       => ireset,
cp2          => cp2,
irqline      => sg_ext_int_req(0),    
timer_irqack => sg_ind_irq_ack(0)
                                     );

sg_core_irqlines(22 downto 20) <= (others => '0');
sg_core_irqlines(13 downto 10)  <= (others => '0');
-- ************************


UART_AVR:component uart port map(
	                   -- AVR Control
               ireset     => ireset,
               cp2	      => cp2,
               adr        => sg_core_adr,
               dbus_in    => sg_core_dbusout,
               dbus_out   => sg_uart_dbusout, 
               iore       => sg_core_iore,
               iowe       => sg_core_iowe,
               out_en     => sg_uart_out_en,

                       --UART
               rxd        => rxd,
               rx_en      => sg_uart_rx_en,
               txd        => txd,
               tx_en      => sg_uart_tx_en,

                       --IRQ
               txcirq     => sg_core_irqlines(19),    -- UART TX Comleet Handler ($0028)
               txc_irqack => sg_ind_irq_ack(19),
               udreirq    => sg_core_irqlines(18),	-- UART Empty ($0026)
			   rxcirq     => sg_core_irqlines(17)   -- UART RX Comleet Handler ($0024)
                              );

-- UART connection to the external multiplexer							  
sg_io_port_out(2)    <= sg_uart_dbusout;
sg_io_port_out_en(2) <= sg_uart_out_en;

-- Timer/Counter
TIM_CNT:component Timer_Counter port map(
	                   -- AVR Control
               ireset     => ireset,
               cp2	      => cp2,
               adr        => sg_core_adr,
               dbus_in    => sg_core_dbusout,
               dbus_out   => sg_tc_dbusout, 
               iore       => sg_core_iore,
               iowe       => sg_core_iowe,
               out_en     => sg_tc_out_en,

                       --Timer/Counters
               EXT1           => '0',
               EXT2           => '0',
			   Tosc1	      => '0',
			   OC0_PWM0       => open,
			   OC1A_PWM1A     => open,
			   OC1B_PWM1B     => open,
			   OC2_PWM2       => open,
			   		   
			           --IRQ
               TC0OvfIRQ      => sg_core_irqlines(15),  -- Timer/Counter0 overflow ($0020)
			   TC0OvfIRQ_Ack  => sg_ind_irq_ack(15),
			   TC0CmpIRQ      => sg_core_irqlines(14),  -- Timer/Counter0 Compare Match ($001E)
			   TC0CmpIRQ_Ack  => sg_ind_irq_ack(14),
			   TC2OvfIRQ      => sg_core_irqlines(9),	-- Timer/Counter2 overflow ($0014)
			   TC2OvfIRQ_Ack  => sg_ind_irq_ack(9),
			   TC2CmpIRQ      => sg_core_irqlines(8),	-- Timer/Counter2 Compare Match ($0012)
			   TC2CmpIRQ_Ack  => sg_ind_irq_ack(8),
			   TC1OvfIRQ      => open,
			   TC1OvfIRQ_Ack  => '0',
			   TC1CmpAIRQ     => open,
			   TC1CmpAIRQ_Ack => '0',
			   TC1CmpBIRQ     => open,
			   TC1CmpBIRQ_Ack => '0',
			   TC1ICIRQ       => open,
			   TC1ICIRQ_Ack   => '0');

-- Timer/Counter connection to the external multiplexer							  
sg_io_port_out(3)    <= sg_tc_dbusout;
sg_io_port_out_en(3) <= sg_tc_out_en;


Serv_Module:component Service_Module port map(
	                   -- AVR Control
               ireset         => ireset,
               cp2	          => cp2,
               adr            => sg_core_adr,
               dbus_in        => sg_core_dbusout,
               dbus_out       => sg_sm_dbusout, 
               iore           => sg_core_iore,
               iowe           => sg_core_iowe,
               out_en         => sg_sm_out_en,
			        -- SLEEP mode signals
			   sleep_en       => open,
		   	        -- SRAM control signals
			   ESRAM_en       => open,
			   ESRAM_WS       => open,
			   	    --IRQ
               ExtInt_IRQ     => sg_core_irqlines(7 downto 0),
			   ExtInt_IRQ_Ack => sg_ind_irq_ack(7 downto 4),
			   
			   -- External interrupts (inputs)
			   Ext_Int_In     => sg_ext_int_req);

-- Service module connection to the external multiplexer							  
sg_io_port_out(4)    <= sg_sm_dbusout;
sg_io_port_out_en(4) <= sg_sm_out_en;                  

-- External interrupt inputs
--sg_ext_int_req(0) <= nINT0;  -- May be uncommented only if simple_timer is disconnected from sg_ext_int_req(0)
sg_ext_int_req(1) <= nINT1;
sg_ext_int_req(2) <= nINT2;
sg_ext_int_req(3) <= nINT3;
sg_ext_int_req(4) <= INT4;
sg_ext_int_req(5) <= INT5;
sg_ext_int_req(6) <= INT6;
sg_ext_int_req(7) <= INT7;

end Struct;
