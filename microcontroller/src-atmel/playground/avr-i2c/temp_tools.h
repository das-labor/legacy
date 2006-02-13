#ifndef TEMP_TOOLS_H_
#define TEMP_TOOLS_H_

#include <stdint.h>
/* these are helper functions wich are usefull 
 * with some/many(?) temperature-sensors
 */

double		temp2double(uint16_t);
uint16_t		double2temp(double);

#endif /*TEMP_TOOLS_H_*/
