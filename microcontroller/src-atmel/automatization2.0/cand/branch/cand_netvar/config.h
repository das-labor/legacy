#ifndef CONFIG_H
#define CONFIG_H

// uart.[ch] defines
#define DEFAULT_UART_BAUD_RATE B230400

#ifndef __CYGWIN__
	#define SIGINFO_HAS_SI_BAND
	#define SHOW_UCONTEXT_INFO
#endif

#define NETVAR_AUTO_REGISTER

#endif
