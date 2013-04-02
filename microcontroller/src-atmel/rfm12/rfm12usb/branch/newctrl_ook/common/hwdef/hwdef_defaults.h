/* this file contains (empty) default definitions for
 * macros that haven't been set by hardware configurations.
 * -> cleaner code by omitting #ifdef's around hardware-specific stuff...
 */

#ifndef TX_LED
	#define TX_LED(a)
#endif
#ifndef RX_LED
	#define RX_LED(a)
#endif
#ifndef DEBUG_LED
	#define DEBUG_LED(a)
#endif
#ifndef USB_LED
	#define USB_LED(a)
#endif

#ifndef HW_INIT
	#define HW_INIT(a)
#endif

#ifndef FSK_SET
	#define FSK_SET(a)
#endif

#ifndef FSK_INIT
	#define FSK_INIT(a)
#endif
