#include "airlab.h"
#include "airlab_state.h"

/* @description set a new/changed state or get the number of states
 * or connections that are open.
 * @note the address field shall always contain the SOURCE of the
 * incoming packet, since we know our address ;)
 */

airlab_state airlab_statehandler (airlab_state in_state)
{
	static airlab_state *mystates;
	uint8_t i;

	if (in_state.state != 0xff)
	{
		/* search the state table for pending connections */
		for (i=0;i < (sizeof(mystates) - sizeof(airlab_state));i += sizeof(airlab_state))
		{
			if ((airlab_state *)(&mystates + i).addr  == in_state.addr)
			{
				(airlab_state *)(&mystates + i) = in_newstate;

				/* delete entry */
				if (in_state.state == AL_STATE_DEL)
				{
					for (;i<sizeof(mystates)-(sizeof(airlab_state)*2);i+=sizeof(airlab_state))
					{
						/* shift all entries to the left */
						(airlab_state *)(&mystates + i) =
							(airlab_state *)(mystates + i + sizeof(airlab_state));
						/* free space again */
						mystates = realloc(mystates, sizeof(mystates) - sizeof(airlab_state));
					}
				}
				i=sizeof(mystates); // force loop break
			}
		}
		/* make room for another one */
		mystates = realloc(mystates, sizeof(mystates) + sizeof(airlab_state));
		i = (sizeof(mystates) - sizeof(airlab_state) - 1);
	}
}

