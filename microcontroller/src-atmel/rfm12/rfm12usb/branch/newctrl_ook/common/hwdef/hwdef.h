#ifdef HWREV
	#if HWREV == 4
		#include "hwdef_4.h"
	#else
		#error "unknown hwref defined."
	#endif
#endif

#include "hwdef_defaults.h"
