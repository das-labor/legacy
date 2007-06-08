/* Config file for rfmxx library.
 *
 * Author: Soeren Heisrath <soeren@heisrath.org>
 * License: GNU - General Public License (version 2)
 *
 * Set all ports/pins/ddrs here to suite your needs.
 *
 * Names of these configuration variables name the pin of
 * the transceiver's output pins and from the TRANSCEIVER's
 * POINT OF VIEW.
 *
 * All variables named RFM_PIN_* are BIT-VECTORS.
 */

/* XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX
 * XXX AGAIN:                                              XXX
 * XXX Everything is named after the TRANSCEIVER's POINT   XXX
 * XXX OF VIEW!                                            XXX
 * XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX
 */

/* chip select */
#define RFM_PORT_CS PORTD
#define RFM_PIN_CS  _BV(PD0)
#define RFM_DDR_CS  DDRD

/* serial data out (trans -> mcu) */
#define RFM_DDR_SDO  DDRD
#define RFM_PIN_SDO  _BV(PD1)
#define RFM_PORT_SDO PORTD

/* serial data in (mcu -> trans) */
#define RFM_DDR_SDI  DDRD
#define RFM_PIN_SDI  _BV(PD2)
#define RFM_PORT_SDI PORTD

/* serial clock in (mcu -> trans) */
#define RFM_DDR_CKI  DDRD
#define RFM_PIN_CKI  _BV(PD3)
#define RFM_PORT_CKI PORTD

/* serial clock out (trans -> mcu) */
#define RFM_DDR_CKO  DDRD
#define RFM_PIN_CKO  _BV(PD4)
#define RFM_PORT_CKO PORTD


/* ===== END OF CONFIGURATION ======
 *
 * everything below doesn't need to be changed...
 **/

#define RFM_SPICK_ON  RFM_PORT_CKI |= RFM_PIN_CKI;
#define RFM_SPICK_OFF RFM_PORT_CKI &= ~( RFM_PIN_CKI );

