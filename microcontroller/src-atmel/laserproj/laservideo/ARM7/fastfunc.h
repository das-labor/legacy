#define FASTRUN __attribute__ ((__long_call__,section (".fastrun")))

extern FASTRUN void fastfunc_test(void);
