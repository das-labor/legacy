
#include <stdint.h>
#include <avr/pgmspace.h>
#include "keypad_charset.h"

const char alphanum_cs[] PROGMEM = 
	"0\0"
	"1\0"
	"ABC2\0"
	"DEF3\0"
	"GHI4\0"
	"JKL5\0"
	"MNO6\0"
	"PQRS7\0"
	"TUV8\0"
	"XYZ9\0"
	"\0";


const char alphanumext_cs[] PROGMEM = 
	"0 +-*/=\0"
	"1\\?!\"|\0"
	"ABCabc2\0"
	"DEFdef3\0"
	"GHIghi4\0"
	"JKLjkl5\0"
	"MNOmno6\0"
	"PQRSpqrs7\0"
	"TUVtuv8\0"
	"XYZxyz9\0"
	"(){}[]<>\0" /* A */
	".:;,\0";  /* B */












