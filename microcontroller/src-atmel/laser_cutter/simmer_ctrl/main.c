
#include "uart/uart.h"

int main(){
  uart_init();
  uart_putstr("Hellow World\r\n");
}