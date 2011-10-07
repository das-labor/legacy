/* uart_i.c */
/*
    This file is part of the OpenARMWare.
    Copyright (C) 2010 Daniel Otte (daniel.otte@rub.de)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdint.h>
#include "sysclock.h"
#include "hw_regs.h"
#include "hw_uart_regs.h"
#include "uart_defines.h"
#include "circularbytebuffer.h"


static const
uint32_t uart_base[] = { UART0_BASE, UART1_BASE, UART2_BASE };

static const
uint32_t gpio_base[] =
	{ GPIOA_BASE, GPIOB_BASE, GPIOC_BASE, GPIOD_BASE,
	  GPIOE_BASE, GPIOF_BASE, GPIOG_BASE, GPIOH_BASE,
	  GPIOJ_BASE,
	};

static const
uint8_t uart_tx_gpio[] = { GPIOA, GPIOD, GPIOG };
static const
uint8_t uart_rx_gpio[] = { GPIOA, GPIOD, GPIOG };
static const
uint8_t uart_tx_pin[] = { 1, 1, 1 };
static const
uint8_t uart_rx_pin[] = { 0, 0, 0 };
static const
uint8_t uart_tx_pctl[] = {1, 5, 1};
static const
uint8_t uart_rx_pctl[] = {1, 5, 1};
static const
uint8_t uart_isr_vector[] = {5, 6, 33};

static const
uint32_t uart_rx_buffersize[] = {128, 128, 128};
static const
uint32_t uart_tx_buffersize[] = {256, 256, 256};

static
circularbytebuffer_t uart_rx_buffer[3];
static
circularbytebuffer_t uart_tx_buffer[3];


static
void uart_tx_isr(uint8_t uartno);
static
void uart_rx_isr(uint8_t uartno);

void uart0_isr(void){

	if(HW_REG(UART0_BASE+UARTMIS_OFFSET)&_BV(UART_TXMIS)){
//		HW_REG(uart_base[0]+UARTDR_OFFSET) = 'X';
		uart_tx_isr(UART_0);
	}
	if(HW_REG(UART0_BASE+UARTMIS_OFFSET)&_BV(UART_RXMIS)){
		uart_rx_isr(UART_0);
	}
}

static
void uart_tx_isr(uint8_t uartno){
	uint32_t tmp;
	tmp = circularbytebuffer_cnt(&(uart_tx_buffer[uartno]));
	while(tmp-- && (!(HW_REG(uart_base[uartno]+UARTFR_OFFSET)&_BV(UART_TXFF)))){
		HW_REG(uart_base[uartno]+UARTDR_OFFSET)
				= (uint32_t)circularbytebuffer_get_fifo(&(uart_tx_buffer[uartno]));
	}
	HW_REG(uart_base[uartno]+UARTICR_OFFSET) |= _BV(UART_TXIC);
}

static
void uart_rx_isr(uint8_t uartno){
	uint8_t c;
	while(!(HW_REG(uart_base[uartno]+UARTFR_OFFSET)&_BV(UART_RXFE))){
		c = HW_REG(uart_base[uartno]+UARTDR_OFFSET);
		circularbytebuffer_append(c, &(uart_rx_buffer[uartno]));
	}
	HW_REG(uart_base[uartno]+UARTICR_OFFSET) |= _BV(UART_RXIC);
}

void calc_baud_values(uint32_t baudrate, uint16_t* intdivider, uint8_t* fracdivider, uint8_t* highspeed){
	uint32_t tmp;
	uint32_t uart_freq;
	if(baudrate==0){
		return;
	}
	uart_freq = sysclk_get_freq();
	*highspeed = ((baudrate*16L)>uart_freq)?1:0;
//	tmp = (((uint64_t)UART_FREQ)*128LL)/(((*highspeed)?8L:16L)*baudrate);
	tmp = uart_freq<<((*highspeed)?(7-3):(7-4));
	tmp /= baudrate;
	tmp++;
	tmp>>=1;
	*fracdivider = (uint8_t)(tmp&0x3f);
	*intdivider = (uint16_t)(tmp>>6);
}

uint8_t uart_init(uint8_t uartno, uint32_t baudrate, uint8_t databits, uint8_t paraty, uint8_t stopbits){
	uint32_t tmp;
	if(databits>=5){
		databits-=5;
	}
	if(uartno>UART_MAX){
		return UART_ERROR_WRONG_UART;
	}
	if(databits>UART_DATABITS_8){
		return UART_ERROR_WRONG_DATABITS;
	}
	if(paraty>UART_PARATY_SPACE){
		return UART_ERROR_WRONG_PARATY;
	}
	if(stopbits>UART_STOPBITS_TWO){
		return UART_ERROR_WRONG_STOPBITS;
	}
	if(0==circularbytebuffer_init(uart_rx_buffersize[uartno], &(uart_rx_buffer[uartno]))){
		return UART_ERROR_RX_BUFFER_INIT;
	}
	if(0==circularbytebuffer_init(uart_tx_buffersize[uartno], &(uart_tx_buffer[uartno]))){
		return UART_ERROR_TX_BUFFER_INIT;
	}
	/* enable clock for gpio*/
	HW_REG(SYSCTL_BASE+RCGC2_OFFSET) |= _BV(uart_rx_gpio[uartno]) | _BV(uart_tx_gpio[uartno]);
    for(tmp=0; tmp<100; ++tmp)
    	;
    HW_REG(SYSCTL_BASE+RCGC2_OFFSET) |= 1;
    for(tmp=0; tmp<100; ++tmp)
    	;
	/* enable clock for uart */
	HW_REG(SYSCTL_BASE+RCGC1_OFFSET) |= _BV(uartno);
    for(tmp=0; tmp<100; ++tmp)
    	;
    HW_REG(gpio_base[uart_rx_gpio[uartno]] + GPIO_ODR_OFFSET) &= ~_BV(uart_rx_pin[uartno]); /* open drain */
    HW_REG(gpio_base[uart_tx_gpio[uartno]] + GPIO_ODR_OFFSET) &= ~_BV(uart_tx_pin[uartno]); /* open drain */
    HW_REG(gpio_base[uart_rx_gpio[uartno]] + GPIO_PUR_OFFSET) &= ~_BV(uart_rx_pin[uartno]); /* pull-up */
    HW_REG(gpio_base[uart_tx_gpio[uartno]] + GPIO_PUR_OFFSET) &= ~_BV(uart_tx_pin[uartno]); /* pull-up */
    HW_REG(gpio_base[uart_rx_gpio[uartno]] + GPIO_PDR_OFFSET) &= ~_BV(uart_rx_pin[uartno]); /* pull-down*/
    HW_REG(gpio_base[uart_tx_gpio[uartno]] + GPIO_PDR_OFFSET) &= ~_BV(uart_tx_pin[uartno]); /* pull-down*/
    HW_REG(gpio_base[uart_rx_gpio[uartno]] + GPIO_DEN_OFFSET) |=  _BV(uart_rx_pin[uartno]); /* digital enable */
    HW_REG(gpio_base[uart_tx_gpio[uartno]] + GPIO_DEN_OFFSET) |=  _BV(uart_tx_pin[uartno]); /* digital enable */

	/* switch to alternate function for rx */
	HW_REG(gpio_base[uart_rx_gpio[uartno]]+GPIO_AFSEL_OFFSET) |= _BV(uart_rx_pin[uartno]);
	 /* switch to alternate function for tx */
	HW_REG(gpio_base[uart_tx_gpio[uartno]]+GPIO_AFSEL_OFFSET) |= _BV(uart_tx_pin[uartno]);
	/* switch multiplexer to uart for rx */
	HW_REG(gpio_base[uart_rx_gpio[uartno]]+GPIO_PCTL_OFFSET) &= ~(0x0f<<(uart_rx_pin[uartno]*4));
	HW_REG(gpio_base[uart_rx_gpio[uartno]]+GPIO_PCTL_OFFSET) |= ((uart_rx_pctl[uartno])<<(uart_rx_pin[uartno]*4));
	/* switch multiplexer to uart for tx */
	HW_REG(gpio_base[uart_tx_gpio[uartno]]+GPIO_PCTL_OFFSET) &= ~(0x0f<<(uart_tx_pin[uartno]*4));
	HW_REG(gpio_base[uart_tx_gpio[uartno]]+GPIO_PCTL_OFFSET) |= ((uart_tx_pctl[uartno])<<(uart_tx_pin[uartno]*4));
	/* set pins to be 2mA */
	HW_REG(gpio_base[uart_rx_gpio[uartno]]+GPIO_DR2R_OFFSET) |= _BV(uart_rx_pin[uartno]);
	HW_REG(gpio_base[uart_tx_gpio[uartno]]+GPIO_DR2R_OFFSET) |= _BV(uart_tx_pin[uartno]);
	/* configure rx pin as input */
	HW_REG(gpio_base[uart_rx_gpio[uartno]]+GPIO_DIR_OFFSET) &= ~_BV(uart_rx_pin[uartno]);
	/* configure tx pin as output */
	HW_REG(gpio_base[uart_tx_gpio[uartno]]+GPIO_DIR_OFFSET) |= _BV(uart_tx_pin[uartno]);
    for(tmp=0; tmp<100; ++tmp)
    	;
	/* disable uart */
	HW_REG(uart_base[uartno]+UARTCTL_OFFSET) &= ~_BV(UART_UARTEN);
	/* set baudrate parameters */
	uint8_t highspeed, fbrd;
	uint16_t ibrd;
	calc_baud_values(baudrate, &ibrd, &fbrd, &highspeed);
	tmp=HW_REG(uart_base[uartno]+UARTLCRH_OFFSET);
	HW16_REG(uart_base[uartno]+UARTIBRD_OFFSET) = ibrd;
    HW8_REG(uart_base[uartno]+UARTFBRD_OFFSET) = fbrd;
    HW_REG(uart_base[uartno]+UARTLCRH_OFFSET) = tmp;
	/* wait until uart is no longer busy */
	while(HW_REG(uart_base[uartno]+UARTFR_OFFSET)&_BV(UART_BUSY))
		;

	/* flush FIFOs */
	HW_REG(uart_base[uartno]+UARTLCRH_OFFSET) &= ~_BV(UART_FEN);

	/* set line parameters (bits, paraty, stopbits*/
	tmp = HW_REG(uart_base[uartno]+UARTLCRH_OFFSET);
	tmp &= ~0xff;
	tmp |= (paraty==UART_PARATY_MARK||paraty==UART_PARATY_SPACE)?_BV(7):0; /* set flag for mark or space paraty*/
	tmp |= databits<<5;
	tmp |= _BV(UART_FEN); /* enable FIFOs */
	tmp |= (stopbits==UART_STOPBITS_TWO)?_BV(3):0;
	tmp |= (paraty==UART_PARATY_EVEN || paraty==UART_PARATY_MARK)?_BV(2):0;
	tmp |= (paraty!=UART_PARATY_NONE)?_BV(1):0;
	HW_REG(uart_base[uartno]+UARTLCRH_OFFSET) = tmp;
	/* set the highspeed bit accordingly */
	if(highspeed){
		HW_REG(uart_base[uartno]+UARTCTL_OFFSET) |= _BV(UART_HSE);
	} else {
		HW_REG(uart_base[uartno]+UARTCTL_OFFSET) &= ~_BV(UART_HSE);
	}
	/* uart interrupt enable */
	HW_REG(uart_base[uartno]+UARTIM_OFFSET) |= _BV(UART_TXIM) | _BV(UART_RXIM);
	HW_REG(ISR_ENABLE_VECTOR+4*(uart_isr_vector[uartno]/32)) |=
			_BV(uart_isr_vector[uartno]%32);

	HW_REG(uart_base[uartno]+UARTCTL_OFFSET) |= _BV(UART_EOT);
	HW_REG(uart_base[uartno]+UARTFR_OFFSET) = 0;
	HW_REG(uart_base[uartno]+UARTCTL_OFFSET) |= _BV(UART_RXE) | _BV(UART_TXE);
	HW_REG(uart_base[uartno]+UARTCTL_OFFSET) |= _BV(UART_UARTEN);

	return UART_ERROR_OK;
}


void uart_putc(uint8_t uartno, uint8_t byte){
	if(uartno>UART_MAX){
		return;
	}
	/* wait while buffer is full */
	while(circularbytebuffer_cnt(&(uart_tx_buffer[uartno]))==uart_tx_buffersize[uartno]){
	}
	if(circularbytebuffer_cnt(&(uart_tx_buffer[uartno]))>0){
		circularbytebuffer_append(byte, &(uart_tx_buffer[uartno]));
		return;
	}
	/* if we have a full uart, append to buffer */
	if(HW_REG(uart_base[uartno]+UARTFR_OFFSET)&_BV(UART_TXFF)){
		circularbytebuffer_append(byte, &(uart_tx_buffer[uartno]));
		return;
	}
	HW_REG(uart_base[uartno]+UARTDR_OFFSET) = (uint32_t)byte;
}

uint16_t uart_getc(uint8_t uartno){
	if(uartno>UART_MAX){
		return 0xffff;
	}
	if(circularbytebuffer_cnt(&(uart_rx_buffer[uartno]))){
		return circularbytebuffer_get_fifo(&(uart_rx_buffer[uartno]));
	}
	/* wait while the FIFO is empty */
	while(HW_REG(uart_base[uartno]+UARTFR_OFFSET)&_BV(UART_RXFE))
		;
	return (uint16_t)HW_REG(uart_base[uartno]+UARTDR_OFFSET);
}

uint32_t uart_dataavail(uint8_t uartno){
	if(uartno>UART_MAX){
		return 0;
	}
	/* wait while the FIFO is empty */
	if(circularbytebuffer_cnt(&(uart_rx_buffer[uartno]))){
		return 1;
	}
	return(HW_REG(uart_base[uartno]+UARTFR_OFFSET)&_BV(UART_RXFE))?0:1;
}


void uart_flush(uint8_t uartno){
	if(uartno>UART_MAX){
		return;
	}
	while(circularbytebuffer_cnt(&(uart_tx_buffer[uartno])))
		;
	while((HW_REG(uart_base[uartno]+UARTCTL_OFFSET)&_BV(UART_EOT)) == 0)
		;
}
