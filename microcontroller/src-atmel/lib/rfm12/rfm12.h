

/* Configuration setting command
	Bit el enables the internal data register.
	Bit ef enables the FIFO mode. If ef=0 then DATA (pin 6) and DCLK (pin 7) are used for data and data clock output.
	x3 x2 x1 x0 Crystal Load Capacitance [pF]
	0 0 0 0 8.5
	0 0 0 1 9.0
	0 0 1 0 9.5
	0 0 1 1 10.0
	....
	1 1 1 0 15.5
	1 1 1 1 16.0
*/
#define RFM12_CMD_CFG 0x8000
#	define RFM12_CFG_EL 0x80
#	define RFM12_CFG_EF 0x40
#	define RFM12_BAND_315 0x00
#	define RFM12_BAND_433 0x10
#	define RFM12_BAND_868 0x20
#	define RFM12_BAND_915 0x30
#	define RFM12_XTAL_12PF 0x07

/*
	2. Power Management Command
	Bit Function of the control bit Related blocks
	er Enables the whole receiver chain RF front end, baseband, synthesizer, oscillator
	ebb The receiver baseband circuit can be separately switched on Baseband
	et Switches on the PLL, the power amplifier, and starts the
	transmission (If TX register is enabled) Power amplifier, synthesizer, oscillator
	es Turns on the synthesizer Synthesizer
	ex Turns on the crystal oscillator Crystal oscillator
	eb Enables the low battery detector Low battery detector
	ew Enables the wake-up timer Wake-up timer
	dc Disables the clock output (pin 8) Clock output buffer
*/
#define RFM12_CMD_PWRMGT 0x8200
#define RFM12_PWRMGT_ER 0x80
#define RFM12_PWRMGT_EBB 0x40
#define RFM12_PWRMGT_ET 0x20
#define RFM12_PWRMGT_ES 0x10
#define RFM12_PWRMGT_EX 0x08
#define RFM12_PWRMGT_EB 0x04
#define RFM12_PWRMGT_EW 0x02
#define RFM12_PWRMGT_DC 0x01

/*
	3. Frequency Setting Command
	Bit 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0 POR
	1 0 1 0 f11 f10 f9 f8 f7 f6 f5 f4 f3 f2 f1 f0 A680h
	The 12-bit parameter F (bits f11 to f0) should be in the range
	of 96 and 3903. When F value sent is out of range, the
	previous value is kept. The synthesizer center frequency f0 can
	be calculated as:
	f0 = 10 * C1 * (C2 + F/4000) [MHz]
	The constants C1 and C2 are determined by
	the selected band as:
	Band [MHz] C1 C2
	433 		1 43
	868 		2 43
	915 		3 30
*/

#define RFM12_CMD_FREQUENCY 0xA000

/*
	4. Data Rate Command
	Bit 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0 POR
	1 1 0 0 0 1 1 0 cs r6 r5 r4 r3 r2 r1 r0 C623h
	The actual bit rate in transmit mode and the expected bit rate of the received data stream in receive mode is determined by the 7-bit
	parameter R (bits r6 to r0) and bit cs.
	BR = 10000 / 29 / (R+1) / (1+cs*7) [kbps]
	In the receiver set R according to the next function:
	R= (10000 / 29 / (1+cs*7) / BR) – 1, where BR is the expected bit rate in kbps.
	Apart from setting custom values, the standard bit rates from 600 bps to 115.2 kbps can be approximated with small error.
	Data rate accuracy requirements:
	Clock recovery in slow mode: ΔBR/BR < 1/(29*Nbit) Clock recovery in fast mode: ΔBR/BR < 3/(29*Nbit)
	BR is the bit rate set in the receiver and ΔBR is the bit rate difference between the transmitter and the receiver. Nbit is the maximum
	number of consecutive ones or zeros in the data stream. It is recommended for long data packets to include enough 1/0 and 0/1
	transitions, and to be careful to use the same division ratio in the receiver and in the transmitter.
*/

#define RFM12_CMD_DATARATE 0xC600
#define RFM12_DATARATE_CS 0x80

/*
	5. Receiver Control Command
	Bit 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0 POR
	1 0 0 1 0 p16 d1 d0 i2 i1 i0 g1 g0 r2 r1 r0 9080h
	Bit 10 (p16): pin16 function select
	
	p16 Function of pin 16
	0 Interrupt input
	1 VDI output
	
	Bits 9-8 (d1 to d0): VDI (valid data indicator) signal response time setting:
	d1 d0 Response
	0 0 Fast
	0 1 Medium
	1 0 Slow
	1 1 Always on

	Bits 7-5 (i2 to i0): Receiver baseband bandwidth (BW) select:
	i2 i1 i0 BW [kHz]
	0 0 0 reserved
	0 0 1 400
	0 1 0 340
	0 1 1 270
	1 0 0 200
	1 0 1 134
	1 1 0 67
	1 1 1 reserved
	Bits 4-3 (g1 to g0): LNA gain select:
	g1 g0 relative to maximum [dB]
	0 0 0
	0 1 -6
	1 0 -14
	1 1 -20

	Bits 2-0 (r2 to r0): RSSI detector threshold:
	r2 r1 r0 RSSIsetth [dBm]
	0 0 0 -103
	0 0 1 -97
	0 1 0 -91
	0 1 1 -85
	1 0 0 -79
	1 0 1 -73
	1 1 0 Reserved
	1 1 1 Reserved
	The RSSI threshold depends on the LNA gain, the real RSSI threshold can be calculated:
	RSSIth=RSSIsetth+GLNA

*/

#define RFM12_CMD_RXCTRL 0x9000
#define RFM12_RXCTRL_P16_VDI 0x400
#define RFM12_RXCTRL_VDI_FAST 0x000
#define RFM12_RXCTRL_VDI_MEDIUM 0x100
#define RFM12_RXCTRL_VDI_SLOW 0x200
#define RFM12_RXCTRL_VDI_ALWAYS_ON 0x300
#define RFM12_RXCTRL_BW_400 0x20
#define RFM12_RXCTRL_BW_340 0x40
#define RFM12_RXCTRL_BW_270 0x60
#define RFM12_RXCTRL_BW_200 0x80
#define RFM12_RXCTRL_BW_134 0xA0
#define RFM12_RXCTRL_BW_67 0xC0
#define RFM12_RXCTRL_LNA_0 0x00
#define RFM12_RXCTRL_LNA_6 0x08
#define RFM12_RXCTRL_LNA_14 0x10
#define RFM12_RXCTRL_LNA_20 0x18
#define RFM12_RXCTRL_RSSI_103 0x00
#define RFM12_RXCTRL_RSSI_97 0x01
#define RFM12_RXCTRL_RSSI_91 0x02
#define RFM12_RXCTRL_RSSI_85 0x03
#define RFM12_RXCTRL_RSSI_79 0x04
#define RFM12_RXCTRL_RSSI_73 0x05


/*
	6. Data Filter Command
	Bit 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0 POR
	1 1 0 0 0 0 1 0 al ml 1 s 1 f2 f1 f0 C22Ch
	Bit 7 (al): Clock recovery (CR) auto lock control, if set.
	CR will start in fast mode, then after locking it will automatically switch to slow mode.
	Bit 6 (ml): Clock recovery lock control
	1: fast mode, fast attack and fast release (4 to 8 bit preamble (1010...) is recommended)
	0: slow mode, slow attack and slow release (12 to 16 bit preamble is recommended)
	Using the slow mode requires more accurate bit timing (see Data Rate Command).
	Bits 4 (s): Select the type of the data filter:
	s Filter Type
	0 Digital filter
	1 Analog RC filter
	Digital: This is a digital realization of an analog RC filter followed by a comparator with hysteresis. The time constant is
	automatically adjusted to the bit rate defined by the Data Rate Command.
	Note: Bit rate can not exceed 115 kpbs in this mode.
	Analog RC filter: The demodulator output is fed to pin 7 over a 10 kOhm resistor. The filter cut-off frequency is set by the
	external capacitor connected to this pin and VSS.
	Bits 2-0 (f2 to f0): DQD threshold parameter.
	Note: To let the DQD report "good signal quality" the threshold parameter should be 4 in cases where the bitrate is close to the
	deviation. At higher deviation/bitrate settings, a higher threshold parameter can report "good signal quality" as well.
*/

#define RFM12_CMD_DATAFILTER 0xC228
#define RFM12_DATAFILTER_AL 0x80
#define RFM12_DATAFILTER_ML 0x40
#define RFM12_DATAFILTER_S 0x10


/*
	7. FIFO and Reset Mode Command
	Bit 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0 POR
	1 1 0 0 1 0 1 0 f3 f2 f1 f0 sp al ff dr CA80h
	Bits 7-4 (f3 to f0): FIFO IT level. The FIFO generates IT when the number of received data bits reaches this level.
	Bit 3 (sp): Select the length of the synchron pattern:
	sp Byte1 Byte0 (POR) Synchron Pattern (Byte1+Byte0)
	0 2Dh D4h 2DD4h
	1 Not used D4h D4h
	Note: Byte0 can be programmed by the Synchron Pattern Command.
	Bit 2 (al): Set the input of the FIFO fill start condition:
	al
	0 Synchron pattern
	1 Always fill

	Bit 1 (ff): FIFO fill will be enabled after synchron pattern reception. The FIFO fill stops when this bit is cleared.
	Bit 0 (dr): Disables the highly sensitive RESET mode.

*/
