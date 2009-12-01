#ifndef POWERCOMMANDER_H
#define POWERCOMMANDER_H

#include <inttypes.h>

#define MACHDUNKEL 0
#define MACHHELL 1

// pwm ... daher hier invers
#define MAXHELL 20
#define MAXDUNKEL 249

// der schalter generiert $Anzahl von events pro sekunde


// immer wieder der gleiche scheiss!
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))

#define ADRESSE 15 // I2C Adresse

// was willst du von dem Relais / Optokoppler
enum action { 
  swoff,   // einschalten
  swon,    // ausschalten
  swstatus, // status
  num_action // ist immer der letzte eintrag
};

// was soll mit der lampe passieren?
enum brightaction {
  brset = 0,   // helligkeit setzen
  num_brightaction // ist immer der letzte eintrag
};

// welches relai willst du ansteuern?

enum relais {
  switch00 = 0,
  switch01,
  switch02,
  switch03,
  switch04,
  switch05,
  switch06,
  switch07,
  optokopp00,
  optokopp01,
  num_switch // muss immer der letzte sein
};



// welche lampe willst du veraendern?
enum bright {
  tafel=0,
  beamer,
  schraenke,
  flipper,
  free1,
  lounge,
  num_bright // ist immer der letzte eintrag
};

// struktur die daten enthaelt und das FLAG wenn daten
// auf den I2C geschrieben werden sollen
struct t_status {
  uint8_t data;
  uint8_t write_data;
};

// den status nur fuer den Vortrag
struct t_state_vortrag {
  uint8_t bright_tafel;
  uint8_t bright_beamer;
  uint8_t bright_schraenke;
  uint8_t bright_flipper;
  uint8_t dimDirection;
};


// den status nur fuer die lounge
struct t_state_lounge {
  uint8_t bright_lounge;
  uint8_t bright_free;
  uint8_t dimDirection;
};

// mitzaehlen der Events von einem Taster
// und mitbekommen, wann er losgelassen wurde
struct t_counter_status {
  uint16_t tastercounter_vortrag;
  uint16_t tastercounter_lounge;
  uint16_t tastercounter_vortrag_last;
  uint16_t tastercounter_lounge_last;
  uint16_t tickscounter;
};




#endif
