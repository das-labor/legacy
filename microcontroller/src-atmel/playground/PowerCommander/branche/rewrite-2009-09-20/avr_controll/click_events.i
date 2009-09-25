/* -*- Mode: C; tab-width: 2 -*- */
/* 
	 wir behandeln hier einfach lounge und vortrag in einer for-schleife
*/
{
	for(taster_i=0;taster_i<U_COUNT;taster_i++)
		{
			/*
				wenn gedrueckt wird, bestimmen wir den zeitraum
			*/
			if ( rb[taster_i].tastercounter > 0) rb[taster_i].clickstate[rb[taster_i].clickpos]++; 
			else {
				/* 
					 sonst bestimmen wir den zeitraum in dem wir nichts druecken
					 wenn clickpos==0 dann ignorieren wir eigentlich, sonst aber eben nicht
				*/
				rb[taster_i].clickstate_free[rb[taster_i].clickpos]++;
			}
			/*
				ring-buffer
			*/
			if ((rb[taster_i].clickstate_free[rb[taster_i].clickpos] > 0) && 
					(rb[taster_i].clickstate[rb[taster_i].clickpos] > 0) )
				{
					rb[taster_i].clickpos++;
					rb[taster_i].clickstate[rb[taster_i].clickpos]=0;
					rb[taster_i].clickstate_free[rb[taster_i].clickpos]=0;
				}
			if (rb[taster_i].clickpos > 2) ringbuffer_flush(&(rb[taster_i]));
		}
}
