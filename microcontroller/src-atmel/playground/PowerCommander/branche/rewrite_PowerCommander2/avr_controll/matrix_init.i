/* -*- Mode: C; tab-width: 2 -*- */
{
	/*
		taster-parameter-matrix ... readonly schalter
	*/

	tp_matrix[0].read_port = &(PINB);
	tp_matrix[0].pin       = PINB7; // hauptschalter
	tp_matrix[1].read_port = &(PIND);
	tp_matrix[1].pin       = PIND2; // Taster 1 int0
	tp_matrix[2].read_port = &(PIND);
	tp_matrix[2].pin       = PIND4;  // Taster 2 pcint20
	
	/*
		lampen-parameter - pwm-ports
	*/

	lp_matrix[0] = &(OCR0A); // tafel
	lp_matrix[1] = &(OCR0B); // beamer
	lp_matrix[2] = &(OCR1A); // schraenke
	lp_matrix[3] = &(OCR1B); // flipper
	lp_matrix[4] = &(OCR2A); // free
	lp_matrix[5] = &(OCR2B); // lounge

	/*
		schalter und optokoppler
	 */
	swp_matrix[0].write_port = &(PORTC); 
	swp_matrix[0].read_port  = &(PORTC);
	swp_matrix[0].pin        = PC3; // relai 00 - kuechenlicht */

	swp_matrix[1].write_port = &(PORTC);
	swp_matrix[1].read_port  = &(PORTC);
	swp_matrix[1].pin        = PC2; // 01  - beamer */

	swp_matrix[2].write_port = &(PORTC);
	swp_matrix[2].read_port  = &(PORTC);
	swp_matrix[2].pin        = PC1; // 02  - licht vortrag */

	swp_matrix[3].write_port = &(PORTC);
	swp_matrix[3].read_port  = &(PORTC);
	swp_matrix[3].pin = PC0; // 03  - licht lounge */

	swp_matrix[4].write_port = &(PORTB);
	swp_matrix[4].read_port  = &(PORTB);
	swp_matrix[4].pin        = PB5; // 04 - tischsteckdose*/

	swp_matrix[5].write_port = &(PORTB);
	swp_matrix[5].read_port  = &(PORTB);
	swp_matrix[5].pin        = PB4; // 05 - klo */

	swp_matrix[6].write_port = &(PORTB);
	swp_matrix[6].read_port  = &(PORTB);
	swp_matrix[6].pin        = PB0; // 06 - hauptschuetze */

	swp_matrix[7].write_port = &(PORTD);
	swp_matrix[7].read_port  = &(PORTD);
	swp_matrix[7].pin        = PD7; // 07 - frei */

	swp_matrix[8].write_port = &(PORTD);
	swp_matrix[8].read_port  = &(PORTD);
	swp_matrix[8].pin        = PD0; // opto 00 - licht+serverschrank-schuetz*/

	swp_matrix[9].write_port = &(PORTD);
	swp_matrix[9].read_port  = &(PORTD);
	swp_matrix[9].pin        = PD1; // opto 01 - alle steckdosen (herd) schuetz */
}
