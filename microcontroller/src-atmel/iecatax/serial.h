void InitSerial0(uint16_t ubrr);
int put_c0(char c);

#define BAUD(A) ((F_CPU/BAUDX/(A))-1)
#define BAUDX	8		// Baud rate divisor.
