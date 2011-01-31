/**
 * 2011 Martin Ongsiek
 *
 * Dies ist ein Debugmenu um auf dem LaborBoard 8 Zahlenwerte auszugeben und zu 
 * verändern. Dies macht Sinn wenn keine Serielle Schnittstelle zur Verfügung
 * steht.
 * Man kann mit Taste 0 und 1 das zwischen den 8 Zahlen wechsel, die 
 * durch eine dunkeler leuchtende LED kurz dargestellt wird. 
 * Das Bit welches dabei leuchtet symbolisiert zur die Ausgewählte Zahl.
 * 
 * Dannach wird der Zahlenwert in dem angegebenen Formatz ausgegeben. 
 * Dieser kan mit Taste 2 und 3 verändert werden.       
 */  
#ifndef DEBUG_MENU
#define DEBUG_MENU

#include <inttypes.h>

typedef enum {
	DM_FRMT_VOLUME,    // Darstellung als Lautstärkeregler  Wertebereich 0 bis 7
	DM_FRMT_BIN,       // Binäre Darstellung                Wertebereich 0 bis 255                    
} format_t;

/**
 * Initialisierung. Falls DEBUG_MIT_INTTERUPT definiert ist läuft das Debugmenu
 * danach direkt und die Interrupts sind freigeschaltet.  
 */ 
void debugMenuInit();

/**
 * @param led Auswahl der Zahl..
 * @param wert Der neue Zahlenwert. 
 */ 
void debugMenuSetValue(uint8_t led, uint8_t wert);

/**
 * @param led Auswahl der Zahl..
 * @param format Definiert das Format (Binär- oder Lautstärkeformat). 
 */ 
void debugMenuSetFormat(uint8_t led, format_t format);

/**
 * @param led Auswahl der Zahl.
 * @return Gibt den aktuellen Wert zurück.
 */ 
uint8_t debugMenuGetValue(uint8_t led);

// Falls DEBUG_MIT_INTTERUPT nicht definiert ist muss jede 10 ms debugMenuCont() 
// aufgerufen werden.
#define DEBUG_MIT_INTTERUPT
// Andernfalls wird automatisch der TimerInterrupt von Timer 0 dazu benutzt.


#ifndef DEBUG_MIT_INTTERUPT
/**
 * Kontinuierlicher Aufruf. Muss alle 10 ms aufgerufen werden falls 
 * DEBUG_MIT_INTTERUPT nicht definiert ist. (Falls 10 ms nicht passen sollte man 
 * die Entprellzeit die Zeit zur anzeige des Menus anpassen.)    
 */ 
void debugMenuCont();
#endif  // DEBUG_MIT_INTTERUPT

#endif  // DEBUG_MENU
