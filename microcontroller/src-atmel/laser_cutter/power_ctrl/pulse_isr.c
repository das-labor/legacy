#include <avr/io.h>
#include <avr/interrupt.h>

#include "ioport.h"
#include "pulse_isr.h"
#include "hardware.h"


extern uint16_t fire_period;
extern uint8_t command_fire;
extern uint8_t command_charge;
volatile extern uint8_t  fire_in_buffer;

volatile uint8_t fire_process_running = 0;    //Zeigt an, dass die Feuerprozess läuft

void init_pulse(void) {
    TCCR0 |= _BV(CS01) | _BV(CS00) | _BV(WGM01); //Prescaler 64, CTC
    OCR0 = 250; //=1,000ms
    TIMSK |= _BV(OCIE0);
    fire_process_running=0;
}

/**
    Startet den Feuerzyklus, wenn
    command_fire gesetzt ist
    oder wenn
    FIRE_IJ auf Masse gezogen wird
*/

ISR(TIMER0_COMP_vect) {
    static uint16_t ticks=0;


   if (!fire_process_running) {
        if (command_fire || ( fire_in_buffer == 1)   ) {
            fire_in_buffer =0;  //Merker wieder löschen
            ticks = TICK_CHARGE_START;
            fire_process_running = 1;
        } else {
            if (command_charge) {
                OUTPUT_OFF(NT_INHIBIT);
            } else {
                OUTPUT_ON(NT_INHIBIT);
            }
            return; //Beendet die ISR, wenn Prozess nicht läuft
        }
    }

    //Start der Periode
    switch (ticks) {
        //Periodenbeginn: Inhibit aus -->Laden startet
        case TICK_CHARGE_START:
            OUTPUT_OFF(NT_INHIBIT);
            break;

        //Ladeende prüfen
        case TICK_END_OF_CHARGE:
            //Prüfe ob End of Charge okay ist
            //ToDo
            break;

        //Inhibit HIGH
        case TICK_CHARGE_STOP:
            OUTPUT_ON(NT_INHIBIT);
            break;

        //Puls ausgeben
        case TICK_PULSE_ON:
            OUTPUT_ON(FIRE);
            break;

        //Puls beenden
        case TICK_PULSE_OFF:
            OUTPUT_OFF(FIRE);
            break;
    }

    //Prozess beenden: zu kurze Wartezeit verhindern und periodendauer einhalten
    //  (Wenn fire_periode zu klein und Anzahl der minimalen Ticks erreicht) ODER
    if (((fire_period < MIN_FIRE_PERIOD) && (ticks >= MIN_FIRE_PERIOD)) || (ticks >= fire_period)) {
        fire_process_running = 0; //Durchlauf für beendet erklären
    }

    ticks++;
}
