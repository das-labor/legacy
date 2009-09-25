/* -*- Mode: C; tab-width: 2 -*- */

static uint8_t add_queue(struct t_queue_object *queue_o)
{
	uint8_t i=0;
	uint8_t pos=QUEUE_SIZE;
	/*
		durchlaufe die queue und suche nach einem freiem platz
	*/
	for(;i<QUEUE_SIZE;i++){
		if ( queue[i]._state==QUEUE_FREE) {
			pos=i;
			break;
		}
	}
	/*
		wenn kein platz frei, dann abbruch
	*/
	if (pos==QUEUE_SIZE){
		return 1;
	}
	/*
		wenn prio zu hoch ist, dann wird es irgendwann geloescht
		da es nie ausgefuehrt wird
		zumindest, solange noch platz ist und andere queue elemente
		ausgefuehrt werden. wenn dies nicht mehr der fall ist, haben wir ein problem
		XXX dummy-objekt einbauen das immer free ist und bloedsinn rausloescht

		queue-objekt auf default setzen aka tue nichts! ;P
	*/
	queue[pos]._state=(*queue_o)._state;
	for(i=0;i<O_LAMP_COUNT;i++){
		queue[pos].bright[i]=(*queue_o).bright[i];
	}
	for(i=0;i<O_SW_COUNT;i++){
		queue[pos].switch_relai[i]=(*queue).switch_relai[i];
	}
	return 0;
}
/*
	fuegt eine volle vortrags-anweisung zur queue hinzu
	return != 0 wenn queue voll ist
	prio ist QUEUE_A0->QUEUE_A8
*/
static uint8_t add_queue_v(uint8_t prio,struct t_state_vortrag *state_vortrag)
{
	uint8_t i=0;
	uint8_t pos=QUEUE_SIZE;
	/*
		durchlaufe die queue und suche nach einem freiem platz
	*/
	for(;i<QUEUE_SIZE;i++){
		if ( queue[i]._state==QUEUE_FREE) {
			pos=i;
			break;
		}
	}
	/*
		wenn kein platz frei, dann abbruch
	*/
	if (pos==QUEUE_SIZE){
		return 1;
	}
	/*
		wenn prio zu hoch ist, dann wird es irgendwann geloescht
		da es nie ausgefuehrt wird
		zumindest, solange noch platz ist und andere queue elemente
		ausgefuehrt werden. wenn dies nicht mehr der fall ist, haben wir ein problem
		XXX dummy-objekt einbauen das immer free ist und bloedsinn rausloescht

		queue-objekt auf default setzen aka tue nichts! ;P
	*/
	queue[pos]._state=prio;
	for(i=0;i<O_LAMP_COUNT;i++){
		queue[pos].bright[i]=BRIGHT_NO_CHANGE;
	}
	for(i=0;i<O_SW_COUNT;i++){
		queue[pos].switch_relai[i]=A_SW_NOCHANGE;
	}

	/*
		auf werte setzen die verlangt sind via state_vortrag
	*/
	queue[pos].bright[O_LAMP_TAFEL-O_LAMP_FIRST]=(*state_vortrag).bright_tafel;
	queue[pos].bright[O_LAMP_BEAMER-O_LAMP_FIRST]=(*state_vortrag).bright_beamer;
	queue[pos].bright[O_LAMP_SCHRANK-O_LAMP_FIRST]=(*state_vortrag).bright_schraenke;
	queue[pos].bright[O_LAMP_FLIPPER-O_LAMP_FIRST]=(*state_vortrag).bright_flipper;

	/*
		XXX - hier das licht fuer die lampen auch ein und ausschalten
	*/
	queue[pos].switch_relai[O_SW02]=(*state_vortrag).onoff;

	return 0;	
}

/*
	fuegt lounge_objekt in queue ein
*/
static uint8_t add_queue_l(uint8_t prio,struct t_state_lounge *state_lounge)
{
	uint8_t i=0;
	uint8_t pos=QUEUE_SIZE;
	/*
		s.o.
	*/
	for(;i<QUEUE_SIZE;i++){
		if ( queue[i]._state==QUEUE_FREE) {
			pos=i;
			break;
		}
	}
	if (pos==QUEUE_SIZE){
		return 1;
	}
	/*
		wenn prio zu hoch ist, dann wird es irgendwann geloscht
		da es nie ausgefuehrt wird
		zumindest, solange noch platz ist und andere queue elemente
		ausgefuehrt werden. wenn dies nicht mehr der fall ist, haben wir ein problem
		XXX dummy-objekt einbauen das immer free ist und bloedsinn rausloescht

		queue-objekt auf default setzen aka tue nichts! ;P
	*/
	queue[pos]._state=prio;
	for(i=0;i<O_LAMP_COUNT;i++){
		queue[pos].bright[i]=BRIGHT_NO_CHANGE;
	}
	for(i=0;i<O_SW_COUNT;i++){
		queue[pos].switch_relai[i]=A_SW_NOCHANGE;
	}
	
	/*
		auf werte setzen die verlangt sind via state_lounge
	*/

	queue[pos].bright[O_LAMP_LOUNGE-O_LAMP_FIRST]=(*state_lounge).bright_lounge;
	queue[pos].bright[O_LAMP_FREE-O_LAMP_FIRST]=(*state_lounge).bright_free;

	/*
		XXX - hier das licht fuer die lampen auch ein und ausschalten
	*/
	queue[pos].switch_relai[O_SW02]=(*state_lounge).onoff;

	return 0;	
}

/*
	das eigentliche abarbeiten der queue
	dabei ist queue eine globale queue
*/
static void deque(uint8_t inst_auto)
{
	struct t_busdata tmp_bus = { { 0, 0, 0 }, 0, 0 };
	
	uint8_t i=0;
	uint8_t pos=QUEUE_SIZE;
	
	/* 
		 durchlaufen der queue bis wir den passenden Prozess gefunden haben
	*/
	for(;i<QUEUE_SIZE;i++){
		if(queue[i]._state == inst_auto) {
			pos = i;
			break;
		}
	}
	/*
		wenn keiner zum aufruf passt, brechen wir ab
	*/
	if (pos == QUEUE_SIZE) return;
	/*
		entferne alle objekte aus der queue die fuer
		spaeter gedacht sind - sinn: aktuelle anforderung
		ueberschreibt alte anforderung
	*/
	
	for(i=0;i<QUEUE_SIZE;i++){
		if(queue[i]._state > inst_auto) {
			queue[i]._state = QUEUE_FREE;
		}
	}
	/*
		durchlaufe alle Lampen und aendere die Helligkeit
		wie angefordert. wenn die Helligkeit BRIGHT_NO_CHANGE ist
		dann mache keine Aenderung an der Helligkeit
	*/
	for (i=0;i<O_LAMP_COUNT;i++)
		{
			if ( queue[pos].bright[i] != BRIGHT_NO_CHANGE ) {
				tmp_bus.in_data[2]=queue[pos].bright[i];
				bright_fkt(lp_matrix[i],&tmp_bus);
			}
		}
	/*
		durchlaufe alle switche/relais/optokoppler und setze sie
		entweder auf an oder aus
		wenn keiner der beiden zustaenge gefordert ist, bleibt 
		alles wie es ist
	*/
	for (i=0;i<O_SW_COUNT;i++){
		if ( queue[pos].switch_relai[i] <= A_SW_ON ) switch_fkt(&(swp_matrix[i]),&tmp_bus);
	}
	/*
		final markiere das Queue_objekt als abgearbeitet
	*/
	queue[pos]._state = QUEUE_FREE;
}
