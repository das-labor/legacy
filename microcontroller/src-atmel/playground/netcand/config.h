#ifndef _CONFIG_H
#define _CONFIG_H

#include <avr/version.h>

/* check for avr-libc version */
#if __AVR_LIBC_VERSION__ < 10404UL
#error "newer libc version (>= 1.4.4) needed!"
#endif

/* check if cpu speed is defined */
#ifndef F_CPU
#error "please define F_CPU!"
#endif

#define UART_BAUD_RATE      38400

// can.[ch] defines
#define CAN_INTERRUPT		//set this to enable interrupt driven and buffering version
//#define CAN_HANDLEERROR
#define CAN_RX_BUFFER_SIZE 8	//only used for Interrupt
#define CAN_TX_BUFFER_SIZE 8	//only used for Interrupt
#define F_MCP 16000000
#define MCP_INT_VEC INT1_vect
#define MCP_INT_MASK INT1
#define MCP_INT_FLAG ISC11

#include "pinout.h"
#include "rfm12_config.h"

/*
 * Network protocols
 */
#define TCP_SUPPORT 0
#define UDP_SUPPORT 1
#define BROADCAST_SUPPORT 1
#define ICMP_SUPPORT 1
#undef  BOOTP_SUPPORT
#undef  BOOTP_TO_EEPROM_SUPPORT

/* configure main callback function for uip */
#define UIP_APPCALL network_handle_tcp
#define UIP_UDP_APPCALL network_handle_udp

#define UIP_SUPPORT 1
#undef  IPV6_SUPPORT
#define IPV4_SUPPORT 1
#undef ROUTER_SUPPORT 
#undef  IP_FORWARDING_SUPPORT
#define UIP_MULTI_STACK 1
#define ENC28J60_SUPPORT 1
#define NET_SUPPORT 1
#undef RFM12_SUPPORT

/*
 * Interfaces
 */

/*
 * ENC28J60 configuration
 */
 /* network controller hardware bug defines */
#define ENC28J60_REV4_WORKAROUND
 
 /* configure duplex mode */
#define NET_FULL_DUPLEX 1
 
#undef  IPV6_STATIC_SUPPORT
#define CONF_ETHERRAPE_MAC  "\xac\xde\x48\xfd\x0f\xd0"
#define CONF_HOSTNAME "netcand"
#define CONF_ETHERRAPE_IP uip_ipaddr(ip, 10,2,0,240 )
#define CONF_ETHERRAPE_IP4_NETMASK uip_ipaddr(ip, 255,255,255,0 )
#define CONF_ETHERRAPE_GATEWAY uip_ipaddr(ip, 10,2,0,1 )

#endif /* _CONFIG_H */
