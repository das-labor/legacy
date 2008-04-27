#ifndef DBZ_STRINGS_H_
#define DBZ_STRINGS_H_

/*
 * author: Daniel Otte
 * email:  daniel.otte@rub.de
 * license: GPLv3
 * 
 */

#include <stdint.h>
#include <avr/pgmspace.h>

uint8_t dbz_strcount(const char* str);
void dbz_splitup(char* dbzstr, char** strings);
uint8_t dbz_strcount_P(PGM_P str);
void dbz_splitup_P(PGM_P dbzstr, PGM_P* strings);

#endif /*DBZ_STRINGS_H_*/
