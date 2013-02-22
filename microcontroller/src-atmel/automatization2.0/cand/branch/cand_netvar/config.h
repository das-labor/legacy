#ifndef CONFIG_H
#define CONFIG_H

// uart.[ch] defines
#define UART_BAUD_RATE B230400
//#define UART_BAUD_RATE B57600


#ifndef __CYGWIN__
	#define SIGINFO_HAS_SI_BAND
	#define SHOW_UCONTEXT_INFO
#endif

#endif
