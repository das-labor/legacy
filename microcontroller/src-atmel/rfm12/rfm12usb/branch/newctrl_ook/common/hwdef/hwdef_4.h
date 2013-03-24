/* RFM12USB v2 prototype board - hardware definition
 */
#include <avr/io.h>

/* -- GENERAL STUFF ---- */
#define F_CPU 20000000

/* -- LEDs ------------- */ 
#define RX_LED(a) \
	PORTC &= ~(_BV(PC3) | _BV(PC4)) \
	PORTC |= (a<<PC3)
#define TX_LED(a) \
	PORTC &= ~(_BV(PC3) | _BV(PC4)) \
	PORTC |= (a<<PC4)

#define DEBUG_LED(a) \
	PORTC &= ~(_BV(PC2) | _BV(PC5)) \
	PORTC |= (a<<PC2)

#define HW_INIT(a) \
	DDRC |= (_BV(PC2) | _BV(PC3) | _BV(PC4) | _BV(PC5))


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
#define USB_CFG_INTR_POLL_INTERVAL      10
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
#define  USB_CFG_VENDOR_ID       0x20, 0xa0 
#define  USB_CFG_DEVICE_ID       0x41, 0x98 /* officially assigned to the rfm12usb */
#define USB_CFG_DEVICE_VERSION  0x00, 0x01
#define USB_CFG_VENDOR_NAME     's', 'o', 'l', 'd', 'e', 'r', 'w', 'a', 'r', 'e'
#define USB_CFG_VENDOR_NAME_LEN 10
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

