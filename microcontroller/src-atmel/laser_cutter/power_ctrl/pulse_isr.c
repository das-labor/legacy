#include <avr/io.h>
#include <avr/interrupt.h>

#include <pulse_isr.h>


extern uint16_t fire_period;
extern uint8_t command_fire;

volatile uint8_t fire_process_running = 0;    //Zeigt an, dass die Feuerprozess läuft

void init_pulse(void) {
    TCCR0 |= _BV(CS02);
    OCR0 = 63; //=1,008ms
    TIMSK |= _BV(OCIE0);
}

ISR(TIMER0_COMP_vect) {
    static uint16_t ticks=0;


   if (!fire_process_running) {
        if (command_fire) {
            ticks = 0;
            fire_process_running = 1;
        } else {
            if (command_charge) {
                INHIBIT_OFF();
            } else {
                INHIBIT_ON();
            }

            return; //Beendet die ISR, wenn Prozess nicht läuft
        }
    }

    //Start der Periode
    switch (ticks) {
        //Periodenbeginn: Inhibit aus -->Laden startet
        case TICK_CHARGE_START:
            INHIBIT_OFF();
            break;

        //Ladeende prüfen
        case TICK_END_OF_CHARGE:
            //Prüfe ob End of Charge okay ist
            //ToDo
            break;

        //Inhibit HIGH
        case TICK_CHARGE_STOP:
            INHIBIT_ON();
            break;

        //Puls ausgeben
        case TICK_PULSE_ON:
            FIRE_ON();
            break;

        //Puls beenden
        case TICK_PULSE_OFF:
            FIRE_OFF();
            break;
    }

    //Prozess beenden: zu kurze Wartezeit verhindern und periodendauer einhalten
    if ((fire_period < TICK_SEQUENCE_COUNT && ticks > MIN_FIRE_PERIOD) || ticks > (fire_period- TICK_SEQUENCE_COUNT)) {
        fire_process_running = 0; //Durchlauf für beendet erklären
    }

    ticks++;
}
