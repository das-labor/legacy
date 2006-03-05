/********************************************************************
 *  Title:    Nixie DCF-Clock
 *  Author:   Dario Carluccio
 *  Date:     03/2004
 *  Purpose:  Receives and decodes the DCF-Signal.
 *            Build in internal Interrupt controlled Software Clock
 *            Shows Time multiplexed on 6 Nixie Tubes
 *  Software: AVR-GCC to compile
 *  Hardware: ATS90S2313
 *            DCF-Receiver (Not inverted) at INT0
 *            6 Nixies connected to:
 *              PB 0-5: Multiplxed Anodes driven by MPS 92/42
 *               PD 3-6: Kathodes driven by BCD-Decoder 74141
 *            High Voltage for Nixies generated with
 *              DC/DC Converter MC-34063
 *            You might find more AVR related stuff at my homepage:
 *                http://www.carluccio.de
 ********************************************************************/


#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <avr/pgmspace.h>



/********************************************************************
 * Prototypes
 ********************************************************************/
int main (void);
void DCFBitReceived(uint8_t);
void InitChip (void);
void InternalClock(void);
uint8_t GetDCFBitValue(uint8_t);



/********************************************************************
 * SET  OC1, OCR, DDROC according to Target
 ********************************************************************/
#if defined(__AVR_AT90S2313__)
#  define OC1 PB3
#  define OCR OCR1
#  define DDROC DDRB
#elif defined(__AVR_AT90S2333__) || defined(__AVR_AT90S4433__)
#  define OC1 PB1
#  define DDROC DDRB
#  define OCR OCR1
#elif defined(__AVR_AT90S4414__) || defined(__AVR_AT90S8515__) || \
      defined(__AVR_AT90S4434__) || defined(__AVR_AT90S8535__) || \
      defined(__AVR_ATmega163__)
#  define OC1 PD5
#  define DDROC DDRD
#  define OCR OCR1A
#else
#  error "Don't know what kind of MCU you are compiling for"
#endif



/********************************************************************
 * SET  XCOM11 according to Target
 ********************************************************************/
#if defined(COM11)
#  define XCOM11 COM11
#elif defined(COM1A1)
#  define XCOM11 COM1A1
#else
#  error "need either COM1A1 or COM11"
#endif



/********************************************************************
 * Defines
 ********************************************************************/
#define CRYSTAL_FREQ 9216000        // Crystal Frequency e.g. 9,216 Mhz
#define UART_BAUD_RATE 9600         // Baud Rate e.g. 9600

#define DCFLowMin   7               // min. duration Low:      80ms
#define DCFLowMax   11              // max. duration Low:     100ms
#define DCFHighMin  17              // min. duration High:    180ms
#define DCFHighMax  21              // max. duration High:    200ms
#define DCF59Min    170             // sec.59 min. dur. LOW: 1700ms
#define DCF59Max    200             // sec.59 max. dur. LOW: 2000ms

#define DCFYear     aDCFTime[5]     // DCF clock year
#define DCFMonth    aDCFTime[4]     // DCF clock month
#define DCFDay      aDCFTime[3]     // DCF clock day
#define DCFHour     aDCFTime[2]     // DCF clock hours
#define DCFMin      aDCFTime[1]     // DCF clock minutes
#define DCFSec      aDCFTime[0]     // DCF clock seconds

#define ITCYear     aITCTime[5]     // internal clock year
#define ITCMonth    aITCTime[4]     // internal clock month
#define ITCDay      aITCTime[3]     // internal clock day
#define ITCHour     aITCTime[2]     // internal clock hours
#define ITCMin      aITCTime[1]     // internal clock minutes
#define ITCSec      aITCTime[0]     // internal clock seconds

#define DCFPause    0               // valDcfSig: Pause received
#define DCFPulse    1               // valDcfSig: Pulse received

#define CtrlSecPuls 0               // CtrlReg:  Flag to do things once per second

#define ErrParity   0               // ErrorReg: Parity Error            = 1
#define ErrDcfBit   1               // ErrorReg: Bitduration Error       = 2
#define ErrDCFStart 2               // ErrorReg:

#define BitSet(Byte, BitNo)     Byte |= (1<<BitNo)
#define BitClear(Byte, BitNo)   Byte &= ~(1<<BitNo)

const uint8_t aDCFValues[] PROGMEM ={                                        \
  0x1,0x2,0x4,0x8,0xA,0x14,0x28,  0x0,0x1,0x2,0x4,0x8,0xA,0x14,              \
  0x0,0x1,0x2,0x4,0x8,0xA,0x14,   0x1,0x2,0x4,                               \
  0x1,0x2,0x4,0x8,0xA,            0x1,0x2,0x4,0x8,0xA,0x14,0x28,0x50,0x0};

volatile uint8_t cntDcf;                // 10ms counter for DCF pulse measure
volatile uint8_t cntIntClk;             // 10ms counter for internal clock
volatile uint8_t DcfSigVal;             // DCFPulse or DCFPause
volatile uint8_t DcfValue;              // Value for received DCF-Bit
volatile uint8_t DcfParity;             // Parity for DCF-Signal
volatile uint8_t LastSigVal;            // Last received DCF Signal
volatile uint8_t DcfSigLen;             // duration of signal in 10ms
volatile uint8_t CtrlReg;               // Control flag register
volatile uint8_t ErrorReg;              // Error flag register
volatile uint8_t aDCFTime[6];           // Time of DCF-Clock
volatile uint8_t aITCTime[6];           // Time of Internal-Clock
volatile uint8_t NixieH;                // Value for Nixie Valve 1+2
volatile uint8_t NixieM;                // Value for Nixie Valve 3+4
volatile uint8_t NixieS;                // Value for Nixie Valve 5+6



/********************************************************************
 *   Timer / Counter Interrupt every 2ms
 *   - increment DCF-pluse Counter every 10ms
 *   - increment internal Clock Counter every 10ms
 *   - multiplex Time to 6 Nixie Tubes
 ********************************************************************/
SIGNAL (SIG_OUTPUT_COMPARE1A){
  static uint8_t Nixie;                  // Which Nixie is shown
  static uint8_t msk = 0x01;	//shift mask for nixie selects 
  static uint8_t NixZiffer;              // What is shown on actual Nixie
  static uint8_t iCnt;	        //Internal Counter 
  
  if (++iCnt==5){           // increment Counters every 5 x 2ms = 10ms
	iCnt=0;
	cntDcf++;         // counter DCF-Pulse measure
    cntIntClk++;      // counter internal Clock
  }
  // Multiplexing of 6 nixie Tubes
  PORTB &= 192;             // Turn all Nixies off (Reset PB 0-5)
  PORTD |= 0x78;  			// Blank Nixie with 141 driver
  
  msk <<= 1;
  if (++Nixie==6){          // Next Nixie
    Nixie=0;
	msk=0x01;
    }
    // get byte to bee show on actual nixie
  if ((Nixie==0) | (Nixie==1)){
    NixZiffer=NixieH;
    } else {
    if ((Nixie==2) | (Nixie==3)){
      NixZiffer=NixieM;
    } else {
      if ((Nixie==4) | (Nixie==5)){
      NixZiffer=NixieS;
      }
    }
  }
    // upper or lower nibble?
  if (Nixie & 0x01){
    NixZiffer &= 15;
  } else {
	NixZiffer >>= 4;
  }
  
  PORTD &= 3;               // Reset BCD-Values
  PORTD |= (NixZiffer<<3);  // BCD-Value to Nixie at PD 3-6
  PORTB |= msk;      		// Turn on actual Nixie
}



/********************************************************************
 *   DCF Interrupt on raising/falling edge at INT1
 *   - detect raising / falling edge,
 *   - change int to other edge
 *   - set signal tag
 *   - set signal time
 *   - set LED on / off
 ********************************************************************/
SIGNAL(SIG_INTERRUPT0){
  DcfSigLen=cntDcf;
  if (!(MCUCR & (1<<ISC00))) {
    // This is done every falling edge (End of Pulse)
    BitSet(MCUCR, ISC00);       // next int on rising edge
    DcfSigVal = DCFPulse;       // HIGH received
    } else {
    // This is done every rising edge (End of Pause)
    BitClear(MCUCR, ISC00);     // next int on falling edge
    DcfSigVal = DCFPause;       // LOW  received
  }
  cntDcf=0;                           // reset DCF-counter
}



/********************************************************************
 *
 * main
 *
 ********************************************************************/
int main (void)
{
    // unsigned int chUart;
  uint8_t lNib;
  uint8_t hNib;

  // Init Ports, Timers, UART and Interrupts
  InitChip();

  // Enable Interrupts
  sei();

  // Main Loop
  for (;;) {
    // Internal Clock
    if (cntIntClk>=100){                    // every 100 * 10ms
      cntIntClk -= 100;               // subtract 100 * 10ms from counter
      InternalClock();                // add 1 second
      BitSet (CtrlReg, CtrlSecPuls);  // set Flag second
    }

    // Set Values to be displayed on Nixies
    if (CtrlReg & (1<<CtrlSecPuls)) {
      // Nixie 1 + 2
      hNib = ITCHour / 10;
      lNib = ITCHour - hNib * 10;
      NixieH = (16 * hNib + lNib);
      // Nixie 3 + 4
      hNib = ITCMin / 10;
      lNib = ITCMin - hNib * 10;
      NixieM = (16 * hNib + lNib);
      // Nixie 5 + 6
      hNib = ITCSec / 10;
      lNib = ITCSec - hNib * 10;
      NixieS = (16 * hNib + lNib);
    }

    // DCF-Signal: Status changed
    if (LastSigVal != DcfSigVal) {
      LastSigVal = DcfSigVal;
      if (DcfSigVal==DCFPulse) {
        // Pulse received
        if ((DcfSigLen>DCFLowMin) & (DcfSigLen<DCFLowMax)){
          DCFBitReceived(0);
        } else {
          if ((DcfSigLen>DCFHighMin) & (DcfSigLen<DCFHighMax)) {
            DCFBitReceived(1);
          } else {
            BitSet (ErrorReg, ErrDcfBit);
          }
        }
      } else if (DcfSigVal==DCFPause) {
        // Pause received
        DCFSec++;                                       // increment DCF second
        // Second 59 ?
        if ((DcfSigLen >= DCF59Min) & (DcfSigLen <= DCF59Max)) {
          DCFSec=0;                               // set DCF second = 0
          // Set Internal Clock
          if (ErrorReg == 0) {
            cntIntClk=0;
            ITCSec=0;
            ITCMin=DCFMin;
            ITCHour=DCFHour;
            ITCDay=DCFDay;
            ITCMonth=DCFMonth;
            ITCYear=DCFYear;
          }
          ErrorReg = 0;        // clear all errors
        }
      }
    }

    // delete second tag
    BitClear (CtrlReg, CtrlSecPuls);                                // Reset bit SecPuls
    }
    return (0);
}



/********************************************************************
 * Init Ports, Timers, UART and Interrupts
 *   - Port B: Output
 *   - Port D: Input
 *   - Int1: Interrupt
 *     - ISC11 (on falling edge )
 *     - INT1  (enable external int1)
 *   - Timer 1: Interrupt every 10 ms
 *     - CK/64 (CS11 + CS 10)
 *     - CTC1  (Clear Timer on Compare match)
 *     - OCR   (Output Compare Register: [ms]*CRYSTAL_FREQ/(1000*64)
 *     - OCIE1 (Output Compare Interrupt Enable)
 *   - Serial Port 1
 *     - Enable RxD
 *     - Enable TxD
 *     - Set Baud Rate: UBRR= (F_CPU / (16*BAUD) )-1
 ********************************************************************/
void InitChip (void) {
  // Port B: Output
  DDRB = 255;               // PortB: Output
  PORTB = 0;                // clear all (activ High)

  // Port D: Input
  DDRD = 120;               // PortD: 0,1,2 In - 3,4,5,6 Out
  PORTD = (1<<2);           // activate Pull-Up on PD3

  // Int0: Interrupt
  //  - ISC01 (on falling edge )   //  - INT0  (enable external int0)
  MCUCR = (1<<ISC01);              // raise int0 on falling edge
  GIMSK = (1<<INT0);               // enable external int0

  // Timer 1: Interrupt every 10 ms
  //  - CK/64 (CS11 + CS 10)
  //  - CTC1  (Clear Timer on Compare match)
  //  - OCR   (Output Compare Register: [ms]*CRYSTAL_FREQ/(1000*64)
  //  - OCIE1 (Output Compare Interrupt Enable)
  TCCR1A = 0;                               // No Output, No PWM
  TCCR1B = (1<<CS11)|(1<<CS10)|(1<<CTC1);    // CK/64 , CTC1
  OCR = 2 * CRYSTAL_FREQ / 64000L;          // 2ms: 5A0h = 1440d
  TIMSK = (1<<OCIE1A);                      // Interrupt Enable

  // Serial Port 1
  //  - Enable RxD
  //  - Enable TxD
  //  - Set Baud Rate: UBRR= (F_CPU / (16*BAUD) )-1
  UCR = (1 << RXEN) | (1 << TXEN);
  UBRR = (CRYSTAL_FREQ / (UART_BAUD_RATE * 16L)) - 1;
}



/********************************************************************
 * Inital Clock
 *   - raise time by one second every call
 ********************************************************************/
void InternalClock(void){
  if (++ITCSec==60){                                                 // seconds
    ITCSec=0;
    if (++ITCMin==60){                                         // minutes
      ITCMin=0;
      if (++ITCHour==24){                                // hours
        ITCHour=0;
        if (++ITCDay==32){                         // days
          ITCMonth++;                        // month with 31 days
          ITCDay=1;
        } else if (ITCDay==31){                    // month with  30 days
          if ((ITCMonth==4)||(ITCMonth==6)||(ITCMonth==9)||(ITCMonth==11)){
            ITCMonth++;                // apr, may, sep, nov
            ITCDay=1;
          }
        } else if (ITCDay==30){                    // feb 29 days
          if(ITCMonth==2){
            ITCMonth++;
            ITCDay=1;
          }
        } else if (ITCDay==29){                    // feb 28 days
          if((ITCMonth==2) && (ITCYear%4)){
            ITCMonth++;
            ITCDay=1;
          }
        }
        if (ITCMonth==13){                         // year
          ITCMonth=1;
          ITCYear++;
        }
      }
    }
  }
}


#define PRG_RDB(a) pgm_read_byte(a)

/********************************************************************
 * Get value for each DCF Bit (second)
 ********************************************************************/
uint8_t GetDCFBitValue(uint8_t nBitNo)
{
    return(PRG_RDB(&aDCFValues[nBitNo]));
}



/********************************************************************
 * DCF Bit recived routine
 ********************************************************************/
void DCFBitReceived(uint8_t nBit){
  // from second 21 get value for DCF clock
  if (DCFSec>=21) {
    if (nBit & DCFPulse)
      DcfValue += GetDCFBitValue(DCFSec-21);  // add bit to DcfValue
    DcfParity ^= nBit;                          // DcfParity XOR received bit
  }

  switch (DCFSec) {
    case 21:
    case 29:
    case 36:
      DcfParity=(nBit & 1);                   // Reset Parity h, m, date
    break;
    case 27:
      DCFMin=DcfValue;                        // set DCF-Minute
      DcfValue=0;
    break;
    case 34:
      DCFHour = DcfValue;                     // set DCF-Hour
      DcfValue=0;
    break;
    case 41:
      DCFDay=DcfValue;                        // set DCF-Day
      DcfValue=0;
    break;
    case 44:
      DcfValue=0;                             // Reset DcfValue
    break;
    case 49:
      DCFMonth=DcfValue;                      // set DCF-Month
      DcfValue=0;
    break;
    case 28:
    case 35:
    case 58:
      if (DCFSec == 58){
        DCFYear=DcfValue;               // DCF-Year
        DcfValue=0;
      }
      if (DcfParity){                         // Check Parity 3 Times
        BitSet (ErrorReg, ErrParity);
        DcfParity=0;
      }
    break;
  }
}
