#include "rfm_config.h"
#include "rfm_transceiver.h"

#include <avr/io.h>

void rfm_setMode (uint8_t in_mode)
{
}
/* @brief initialize ports, ddrs and pins.
 *
 * @note this function will NOT set the module in a defined
 * 	mode. Use rfm_setMode() to set a mode.
 */


void rfm_init ( void )
{
	/* chip select */
	RFM_DDR_CS  |= RFM_PIN_CS;
	RFM_PORT_CS &= ~(RFM_PIN_CS); // cs is active low
	
	/* serial data output (trans -> mcu) */
	RFM_DDR_SDO  &= ~(RFM_PIN_SDO); // set sdo to input
	RFM_PORT_SDO |= RFM_PIN_SDO; // enable pullup

	/* spi data input (mcu -> trans) */
	RFM_DDR_SDI  |= RFM_PIN_SDI;
	RFM_PORT_SDI &= ~(RFM_PIN_SDI);
	
	/* spi clock output (trans -> mcu) */
	RFM_DDR_CKO  &= ~(RFM_PIN_CKO);
	RFM_PORT_CKO |= RFM_PIN_CKO;
	
	/* spi clock in (mcu -> trans) */
	RFM_DDR_CKI  |= (RFM_PIN_CKI); // input
	RFM_SPICK_OFF();

	/* interrupt input from module */
	#ifdef RFM_IRQ_ENABLE
		RFM_DDR_NIRQ &= ~(RFM_PIN_NIRQ); // input
		RFM_PORT_NIRQ |= (RFM_PIN_NIRQ); // pullup
	#endif
}

rfm_sendCommand ( uint16_t in_cmd )
{
	RFM_SPICK_ON();
	
}

void rfm_setMode (void)
{
	
}
