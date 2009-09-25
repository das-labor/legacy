/* -*- Mode: C; tab-width: 2 -*- */
/*
	in der click_events.i wurden die events erfasst
	nun muessen sie ausgewertet werden und entsprechende
	Aktionen getaetigt werden. hier sind nur die 
	aktionen hinterlegt fuer die Lounge
*/

{
/*
	einmal kurz druecken
*/
	if ( (rb[taster_i].clickstate[0] <= T_CLICKDELAY) && 
			 (rb[taster_i].clickstate_free[1] > T_CLICKDELAY))
		{
			/*
				squenz hinzufuegen
				t=0 an
				0<t<=20sec = hell  k=1,2,3,4
				20<t<=25 = dunkel  k=5
				25<t<30 = hell     k=6
				t=3 aus
			*/
			lounge_cur.onoff=A_SW_ON;
			lounge_cur.bright_lounge=MAXHELL;
			lounge_cur.bright_free=MAXHELL;
			/*
				20 sekunden
			*/
			for (i=0;i<20;i++)	{
				add_queue_l(QUEUE_A8,&lounge_cur);
			}
			lounge_cur.bright_lounge=MAXDUNKEL;
			lounge_cur.bright_free=MAXDUNKEL;
			/*
				5 sekunden
			*/
			for (i=0;i<5;i++)	{
				add_queue_l(QUEUE_A8,&lounge_cur);
			}
			lounge_cur.bright_lounge=MAXHELL;
			lounge_cur.bright_free=MAXHELL;
			/*
				5 sekunden
			*/
			for (i=0;i<5;i++)	{
				add_queue_l(QUEUE_A8,&lounge_cur);
			}
			/*
				aus
			*/
			lounge_cur.onoff=A_SW_OFF;
			add_queue_l(QUEUE_A8,&lounge_cur);
		}

	/*
		einmal lang - und halten
	*/

	else if ( (rb[taster_i].clickstate[0] > T_CLICKDELAY) )
		{
			/*
				sichergehen, dass die lampen alle an sind
				klar, das signal geht sehr oft raus, aber das ist ok
			*/
			lounge_cur.onoff=A_SW_ON;
			/*
				alle Lampen gleich hell machen. fipper ist ref
			*/
			lounge_cur.bright_lounge=lounge_cur.bright_free;
			if(lounge_cur.dimDirection == MACHHELL){
				lounge_cur.bright_lounge--;
				lounge_cur.bright_free--;
			} else {
				lounge_cur.bright_lounge++;
				lounge_cur.bright_free++;
			}
			if(lounge_cur.bright_lounge <= MAXHELL) lounge_cur.dimDirection = MACHDUNKEL;
			if(lounge_cur.bright_lounge >= MAXDUNKEL)	lounge_cur.dimDirection = MACHHELL;
			
			add_queue_l(QUEUE_A5,&lounge_cur);
		}

	/*
		zwei mal schnell klicken
	*/
	else if ( (rb[taster_i].clickstate[0] <= T_CLICKDELAY) &&
						(rb[taster_i].clickstate_free[1] <= T_CLICKDELAY) &&
						(rb[taster_i].clickstate[1] <= T_CLICKDELAY) && 
						(rb[taster_i].clickstate_free[2] > T_CLICKDELAY) )
		{

		}
	/*
		druecken loslassen druecken halten
	*/
	else if ( (rb[taster_i].clickstate[0] <= T_CLICKDELAY) &&
						(rb[taster_i].clickstate_free[1] <= T_CLICKDELAY) &&
						(rb[taster_i].clickstate[1] > T_CLICKDELAY))
		{

		}
	/*
		drei mal schnell klicken
	*/
	else if ( (rb[taster_i].clickstate[0] <= T_CLICKDELAY) &&
						(rb[taster_i].clickstate_free[1] <= T_CLICKDELAY) &&
						(rb[taster_i].clickstate[1] <= T_CLICKDELAY) && 
						(rb[taster_i].clickstate_free[2] <= T_CLICKDELAY) &&
						(rb[taster_i].clickstate[2] <= T_CLICKDELAY) )
		{

		}
	/*
		druecken ->loslassen, druecken->loslassen, druecken->halten
	*/
	else if ( (rb[taster_i].clickstate[0] <= T_CLICKDELAY) &&
						(rb[taster_i].clickstate_free[1] <= T_CLICKDELAY) &&
						(rb[taster_i].clickstate[1] <= T_CLICKDELAY) && 
						(rb[taster_i].clickstate_free[2] <= T_CLICKDELAY) &&
						(rb[taster_i].clickstate[2] > T_CLICKDELAY) )
		{

		}

	ringbuffer_flush(&(rb[taster_i]));
}

