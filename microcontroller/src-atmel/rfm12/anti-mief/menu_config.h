#include <stdint.h>

#include "menu.h"

#ifndef MENUCONFIG_H

extern const menuentry_t mymenu[] =
{
	{
		[name] = "5  Minuten",
		[next] = 1,
		[func] = NULL
	},
	{
		[name] = "10 Minuten",
		[next] = 2,
		[func] = NULL
	},
	{
		[name] = "15 Minuten",
		[next] = 3,
		[func] = NULL
	},
	{
		[name] = "20 Minuten",
		[next] = 4,
		[func] = NULL
	},
	{
		[name] = "25 Minuten",
		[next] = 5,
		[func] = NULL
	},
	{
		[name] = "Duschprogramm"
		[next] = 6,
		[func] = NULL
	}
	{
		[name] = "Waesche",
		[next] = 7,
		[func] = NULL
	},
	{
		[name] = "Aus",
		[next] = 8,
		[func] = NULL
	},
	{
		[name] = "Dauer-An",
		[next] = 0,
		[func] = NULL
	}

}

#define MENUCONFIG_H
#endif
