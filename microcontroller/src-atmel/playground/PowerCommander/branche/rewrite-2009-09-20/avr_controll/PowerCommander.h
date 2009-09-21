#ifndef POWERCOMMANDER_H
#define POWERCOMMANDER_H

#include <inttypes.h>

#define MACHDUNKEL 0
#define MACHHELL 1

// pwm ... daher hier invers
#define MAXHELL 20
#define MAXDUNKEL 249


#define HASDATA (0x01)
#define HASNDATA (0x00)

// der schalter generiert $Anzahl von events pro sekunde


// immer wieder der gleiche scheiss!
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))

#define I2CADRESSE 15 // I2C Adresse


/*
  userinterface Taster
 */
#define U_VORTRAG (0x00)
#define U_LOUNGE (0x01)
#define U_COUNT (0x02)
/* 
   Actionen als defines
*/
#define A_SW_OFF     (0x00)
#define A_SW_ON      (0x01)
#define A_SW_STATUS  (0x02)
#define A_SW_COUNT   (0x03)

#define A_BR_SET     (0x00)
#define A_BR_GET     (0x01)
#define A_BR_COUNT   (0x02)

/*
  Objekte als defines
*/
#define O_SW_FIRST (0x00)
#define O_SW00 (0x00)
#define O_SW01 (0x01)
#define O_SW02 (0x02)
#define O_SW03 (0x03)
#define O_SW04 (0x04)
#define O_SW05 (0x05)
#define O_SW06 (0x06)
#define O_SW07 (0x07)
#define O_SW08 (0x08)
#define O_SW09 (0x09)
#define O_OPTO00 (O_SW08)
#define O_OPTO01 (O_SW09)
#define O_SW_LAST (0x09)

#define O_LAMP_FIRST   (0x0A)
#define O_LAMP_TAFEL   (0x0A)
#define O_LAMP_BEAMER  (0x0B)
#define O_LAMP_SCHRANK (0x0C)
#define O_LAMP_FLIPPER (0x0D)
#define O_LAMP_FREE    (0x0E)
#define O_LAMP_LOUNGE  (0x0F)
#define O_LAMP_LAST    (0x0F)

#define O_TASTER_FIRST (0x10)
#define O_TASTER_LOUNGE (0x10)
#define O_TASTER_VORTRAG (0x11)
#define O_TASTER_MAIN (0x12)
#define O_TASTER_LAST (0x12)

#define O_SW_COUNT (0x0A)
#define O_LAMP_COUNT (0x06)
#define O_TASTER_COUNT (0x03)
#define O_COUNT   (0x13)

// ist 0.005sekunden * DOUBLECLICK
#define T_CLICKDELAY 50

// bei Timer/counter = 256 auf 16MHz
// etwa 0.000512 sekunden
#define TICKS_A0 (0x0020)  
// etwa 0.0025
#define TICKS_A1 (0x009D)
// etwa 0.005
#define TICKS_A2 (0x0138)
// etwa 0.01
#define TICKS_A3 (0x0271)
// etwa 0.04
#define TICKS_A4 (0x09C4)
// etwa 0.125 sekunden
#define TICKS_A5 (0x1E84)
// etwa 0.25 sekunden
#define TICKS_A6 (0x3D09)
// etwa 0.5 sekunden
#define TICKS_A7 (0x7A12)
// etwa 1.0 sekunden
#define TICKS_A8 (0xF424)


#define RINGBUFFERSIZE (3)

// struktur die daten enthaelt und das FLAG wenn daten
// auf den I2C geschrieben werden sollen
struct t_busdata {
  uint8_t in_data[3];
  uint8_t out_data;
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


struct t_switch_parameter {
  uint8_t write_port;
  uint8_t read_port;
  uint8_t pin;
};

struct t_switch_parameter_ro {
  uint8_t read_port;
  uint8_t pin;
};


#endif
