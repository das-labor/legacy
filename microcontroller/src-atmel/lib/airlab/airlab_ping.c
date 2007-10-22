#include "airlab.h"
#include "airlab_ping.h"
#ifdef AL_STATES
	#include "airlab_state.h"
#endif

#ifdef AL_PROTO_PING

void airlab_ping (airlab_addr in_dst)
{
	uint8_t txdata[10];

	txdata[] = "  PING1234"
	
	txdata[0] = 10;
	txdata[1] = AL_TYPE_PING;

	airlab_tx_raw (10, &txdata);

#ifdef AL_STATES
	airlab_state newstate;
	newstate.addr = in_dst;
	newstate.state = AL_STATE_AWAITING_PONG;
#endif
}

/* @description handle incoming pong packet and set state accordingly.
 */
inline void airlab_handle_pong ( uint8_t *in_packet, uint8_t in_len)
{
#ifdef AL_STATE
	airlab_state mystate;
	mystate.addr = AL_SRC(in_packet);
	mystate.state = AL_STATE_PONG_RECEIVED;
	airlab_statehandler (mystate);
#endif
}

/* @description see if a pong has arrived from the given address.
 * @note this function will also delete the entry from the state table
 */
inline uint8_t airlab_poll_pong ( airlab_addr in_src )
{
#ifdef AL_STATE
	airlab_state mystate;
	mystate.addr = in_src;
	mystate.state = AL_STATE_RETR;
	mystate = airlab_statehandler (mystate);
	if (mystate.state == AL_STATE_PONG_RECEIVED)
	{
		/* delete from state table */
		mystate.state = AL_STATE_DEL;
		airlab_statehandler (mystate);

		return 1;
	}
	return 0;
#endif
}
#endif
