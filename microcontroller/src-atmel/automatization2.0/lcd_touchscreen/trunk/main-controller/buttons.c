#include <stdlib.h>

#include "gui_lib/gui.h"
#include "netvar/netvar.h"
#include "buttons.h"
#include "uart/uart.h"



void buttons_nv_handler(netvar_desc *nd, void *ref) {
}


typedef struct {
	netvar_desc * tx_nv[6];
	netvar_desc * rx_nv[6];
} buttons_t;

static buttons_t buttons;

static void register_button_netvars(uint8_t i, uint16_t idx, uint8_t sidx) {
	buttons.tx_nv[i] = netvar_register(idx, 0x20 + sidx, 1);
	buttons.rx_nv[i] = netvar_register(idx, 0x30 + sidx, 1);
	netvar_add_handler(buttons.rx_nv[i], buttons_nv_handler, (void *)(uint16_t)i);
}

void init_buttons() {
	register_button_netvars(0, 0x0100, 0);
	register_button_netvars(1, 0x0100, 1);
	register_button_netvars(2, 0x0100, 2);
	register_button_netvars(3, 0x0100, 3);
	register_button_netvars(4, 0x0100, 4);
	register_button_netvars(5, 0x0100, 0x0f);
}

void update_buttons() {
	char c; uint8_t val;
	while (uart_getc_nb(&c)) {
		uart_putc(c);
		if (c < 'a') {
			c -= 'A';
			val = 0xff;
			netvar_write(buttons.tx_nv[c], &val);
		} else {
			c -= 'a';
			val = 0x00;
			netvar_write(buttons.tx_nv[c], &val);
		}
	}
}
