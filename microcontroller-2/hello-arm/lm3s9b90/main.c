
#include <stdint.h>
#include <stdlib.h>
#include "cli.h"
#include "dump.h"
#include "uart_lowlevel.h"
#include "sysclock.h"

void uart0_putc(char byte){
	uart_putc(UART_0, byte);
}

char uart0_getc(void){
	return uart_getc(UART_0);
}

void hello(void){
	cli_putstr("\r\nHello World!\r\n");
}

void endianess(void){
	uint32_t t=1;
	cli_putstr("\r\nYour system has ");
	cli_putstr(((uint8_t*)(&t))[0]?"little":"big");
	cli_putstr(" endianess.\r\n");
}

void casts(void){
	uint32_t t=1;
	cli_putstr("\r\nCasts work");
	if(!((uint8_t)t)){
		cli_putstr(" not");
	}
	cli_putstr(" as expected\r\n");
}

void rcc_dump(void){
	uint32_t tmp_rcc;
	tmp_rcc = HW_REG(SYSCTL_BASE+RCC_OFFSET);
	cli_putstr("\r\nRCC  = ");
	cli_hexdump_rev(&tmp_rcc, 4);
	tmp_rcc = HW_REG(SYSCTL_BASE+RCC2_OFFSET);
	cli_putstr("\r\nRCC2 = ");
	cli_hexdump_rev(&tmp_rcc, 4);
	cli_putstr("\r\n");
}

static const
cmdlist_entry_t cmdlist[] = {
	{ "Hello",          NULL,  hello},
	{ "endianess",      NULL,  endianess},
	{ "casts",          NULL,  casts},
	{ "RCCs",           NULL,  rcc_dump},
	{ "dump",       (void*)1, (void_fpt)dump},
	{ "echo",       (void*)1, (void_fpt)echo_ctrl},
	{ NULL,             NULL, NULL}
};

int main(void) {

	sysclk_setrawclock();
	sysclk_mosc_verify_enable();


    uart_init(UART_0, 115200, 8, UART_PARATY_NONE, UART_STOPBITS_ONE);

	cli_rx = uart0_getc;
    cli_tx = uart0_putc;

    cli_putstr("\r\nSystem initialized...\r\n");

	for(;;){
    	cmd_interface(cmdlist);
    }

}
