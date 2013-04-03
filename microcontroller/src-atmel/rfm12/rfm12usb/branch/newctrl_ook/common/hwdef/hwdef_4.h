/* RFM12USB v2 prototype board - hardware definition
 */
#include <avr/io.h>
#include <avr/interrupt.h>

#pragma once

/* -- GENERAL STUFF ---- */
#ifndef F_CPU
	#define F_CPU 20000000UL
#endif
#define RFMUSB_DEBUG 0 /* enable debug printing (not yet implemented) */

/* -- LEDs ------------- */ 

#define RX_LED(a) \
	PORTC &= ~(_BV(PC3) | _BV(PC4)); \
	PORTC |= ((uint8_t) a<<PC3)
#define TX_LED(a) \
	PORTC &= ~(_BV(PC3) | _BV(PC4)); \
	PORTC |= ((uint8_t) a<<PC4)

#define DEBUG_LED(a) \
	PORTC &= ~(_BV(PC2) | _BV(PC5)); \
	PORTC |= ((uint8_t) a<<PC2)

#define USB_LED(a) \
	PORTC &= ~(_BV(PC2) | _BV(PC5)); \
	PORTC |= ((uint8_t) a<<PC5)

#define HW_INIT(a) \
	DDRC |= (_BV(PC2) | _BV(PC3) | _BV(PC4) | _BV(PC5))

/* -- BUFFERS --------- */
#define RFMUSB_USB_BUF_SIZE        64  /* buffer containing data from/to the usb host */
#define RFMUSB_DEBUG_BUF_SIZE      16  /* buffer for debug messages, only active when RFMUSB_DEBUG is set to 1 */
#define RFMUSB_OOK_MAXLEN          16  /* buffer for ook bits */

/* -- USB ------------- */
#define USB_CFG_IOPORTNAME             D
#define USB_CFG_DMINUS_BIT             4
#define USB_CFG_DPLUS_BIT              2
#define USB_CFG_CLOCK_KHZ              (F_CPU/1000)
#define USB_CFG_CHECK_CRC              0
#define USB_CFG_HAVE_INTRIN_ENDPOINT   0
#define USB_CFG_EP3_NUMBER             3
#define USB_CFG_IMPLEMENT_HALT          0
#define USB_CFG_SUPPRESS_INTR_CODE      0
#define USB_CFG_INTR_POLL_INTERVAL      200
#define USB_CFG_IS_SELF_POWERED         0
#define USB_CFG_MAX_BUS_POWER           100
#define USB_CFG_IMPLEMENT_FN_WRITE      1
#define USB_CFG_IMPLEMENT_FN_READ       0
#define USB_CFG_IMPLEMENT_FN_WRITEOUT   0
#define USB_CFG_HAVE_FLOWCONTROL        0
#define USB_CFG_DRIVER_FLASH_PAGE       0
#define USB_CFG_LONG_TRANSFERS          0
#define USB_COUNT_SOF                   0
#define USB_CFG_CHECK_DATA_TOGGLING     0
#define USB_CFG_HAVE_MEASURE_FRAME_LENGTH   0
#define USB_USE_FAST_CRC                0
#define USB_CFG_VENDOR_ID       0xa0, 0x20 
#define USB_CFG_DEVICE_ID       0x98, 0x41 /* officially assigned to the rfm12usb */
#define USB_CFG_DEVICE_VERSION  0x00, 0x01
#define USB_CFG_VENDOR_NAME     's', 'o', 'l', 'd', 'e', 'r', 'w', 'a', 'r', 'e', '.', 'o', 'r', 'g'
#define USB_CFG_VENDOR_NAME_LEN 14
#define USB_CFG_DEVICE_NAME     'R', 'F', 'M', '1', '2', 'U', 'S', 'B'
#define USB_CFG_DEVICE_NAME_LEN 8
#define USB_CFG_DEVICE_CLASS        0xff    /* set to 0 if deferred to interface */
#define USB_CFG_DEVICE_SUBCLASS     0
#define USB_CFG_INTERFACE_CLASS     0   /* define class here if not at device level */
#define USB_CFG_INTERFACE_SUBCLASS  0
#define USB_CFG_INTERFACE_PROTOCOL  0
#define USB_CFG_DESCR_PROPS_DEVICE                  0
#define USB_CFG_DESCR_PROPS_CONFIGURATION           0
#define USB_CFG_DESCR_PROPS_STRINGS                 0
#define USB_CFG_DESCR_PROPS_STRING_0                0
#define USB_CFG_DESCR_PROPS_STRING_VENDOR           0
#define USB_CFG_DESCR_PROPS_STRING_PRODUCT          0
#define USB_CFG_DESCR_PROPS_STRING_SERIAL_NUMBER    0
#define USB_CFG_DESCR_PROPS_HID                     0
#define USB_CFG_DESCR_PROPS_HID_REPORT              0
#define USB_CFG_DESCR_PROPS_UNKNOWN                 0
#define usbMsgPtr_t unsigned short

/* --- RFM12 CONFIG ------------------ */

#define DDR_SPI    DDRB
#define PORT_SPI   PORTB
#define PIN_SPI    PINB
#define BIT_MOSI   3
#define BIT_MISO   4
#define BIT_SCK    5
#define BIT_SPI_SS 2
#define PORT_SS    PORTB
#define DDR_SS     DDRB
#define BIT_SS     0

#define FSK_SET(a) \
	PORTC &= ~(_BV(PC0)); \
	PORTC |=  (a << PC0)

/* set as _INPUT_ pin, we're just driving the FSK via pullup */
#define FSK_INIT() \
	DDRC &= ~(_BV(PC0)); \
	PORTC |= (_BV(PC0))

/* NOTE: Values below are for the default initialization.
 * They may be changed by the USB host at runtime
 */
#define RFM12_BASEBAND        RFM12_BAND_433
#define RFM12_FREQUENCY       433920000UL
//#define FSK_SHIFT             125000
#define DATARATE_VALUE        RFM12_DATARATE_CALC_HIGH(9600.0)
#define RFM12_RSSI_THRESHOLD  RFM12_RXCTRL_RSSI_79
#define RFM12_FILTER_BW       RFM12_RXCTRL_BW_400
#define RFM12_POWER           RFM12_TXCONF_POWER_0
#define RFM12_LNA_GAIN        RFM12_RXCTRL_LNA_6
#define RFM12_XTAL_LOAD       RFM12_XTAL_11_5PF


#define RFM12_TX_BUFFER_SIZE  64
#define RFM12_RX_BUFFER_SIZE  64

/* interrupt */
#define RFM12_INT_VECT        (INT1_vect)
#define RFM12_INT_MSK         (EIMSK)
#define RFM12_INT_BIT         (INT1)
#define RFM12_INT_FLAG        (EIFR)
#define RFM12_FLAG_BIT        (INTF1)
#define RFM12_INT_SETUP()     (EICRA |= (1<<ISC11))


/************************
 * FEATURE CONFIGURATION
 */

#define RFM12_LIVECTRL             1
#define RFM12_NORETURNS            0
#define RFM12_NOCOLLISIONDETECTION 1
#define RFM12_TRANSMIT_ONLY        0
#define RFM12_SPI_SOFTWARE         0
#define RFM12_USE_POLLING          0
#define RFM12_RECEIVE_ASK          0
#define RFM12_TRANSMIT_ASK         1
#define RFM12_USE_WAKEUP_TIMER     0
#define RFM12_USE_POWER_CONTROL    0
#define RFM12_LOW_POWER            0
#define RFM12_USE_CLOCK_OUTPUT     0
#define RFM12_UART_DEBUG           0
