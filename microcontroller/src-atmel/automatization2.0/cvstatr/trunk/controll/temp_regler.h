#ifndef _TEMP_REGLER_H
#define _TEMP_REGLER_H

extern void set_ventil(uint8_t stufe);
extern void temp_regler(void);
extern void reset_ventil(void);

extern uint8_t temp_soll;
extern uint8_t temp_ist;

#endif // _TEMP_REGLER

