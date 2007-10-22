#include "airlab.h"

/* @description uses the hardware's direct sending function
 * @note change this if you don't want to use some other
 * library in favour of librfm12. The length field is not
 * set by this function, it is rather used within the function
 * to determine the size of the incoming data chunk.
 */
inline void airlab_tx_raw (uint8_t in_len, uint8_t *in_data)
{
	// rfm12_tx(); < hey, where'd this one go?
}

/* @description receive a single byte in raw
 * @note same goes for this one: change this if you plan to
 * use some other software/hardware to run airlab on.
 */
inline uint8_t airlab_rx_raw (uint8_t *out_len, uint8_t *out_data)
{
	// rx function done yet? which one?
}

/* @description check if new packets have arrived. return value
 * is the number of pending packets.
 */
uint8_t airlab_get_rx_status ( void )
{
	
}

/* @description return or set the airlab address.
 * @param in_addr if in_addr is == 0xff, the function returns
 * the current address.
 */
airlab_addr airlab_ifconfig (airlab_addr in_addr)
{
	static airlab_addr myaddr;
	
	if (in_addr != 0xff)
		myaddr = in_addr;

	return myaddr;
}

