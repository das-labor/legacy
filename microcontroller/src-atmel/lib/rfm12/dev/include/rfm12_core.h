#pragma once

/******************************************************
* 	VARIOUS RFM RELATED DEFINES FOR INTERNAL USE		*
*	(defines which shall be visible to the user are located in rfm12.h	*
 ******************************************************/

//default preamble (altrernating 1s and 0s)
#define PREAMBLE 0xAA

//default synchronization pattern
#define SYNC_MSB 0x2D
#define SYNC_LSB 0xD4

//these are the states for the receive/transmit state machine
#define STATE_RX_IDLE 0
#define STATE_RX_ACTIVE 1
#define STATE_TX 2
//FIXME: raw mode...
#define MODE_RAW 2


/******************************************************
* 	LIBRARY DEFAULT SETTINGS						*
 ******************************************************/
//if notreturns is not defined, we won't use this feature
#ifndef RFM12_NORETURNS
	#define RFM12_NORETURNS 0
#endif

//if nocollisiondetection is not defined, we won't use this feature
#ifndef RFM12_NOCOLLISIONDETECTION
	#define RFM12_NOCOLLISIONDETECTION 0
#endif

//if livectrl is not defined, we won't use this feature
#ifndef RFM12_LIVECTRL
	#define RFM12_LIVECTRL 0
#endif

//if low power is not defined, we won't use this feature
#ifndef RFM12_LOW_POWER
	#define RFM12_LOW_POWER 0
#endif

//if wakeuptimer is not defined, we won't use this feature
#ifndef RFM12_USE_WAKEUP_TIMER
	#define RFM12_USE_WAKEUP_TIMER 0
#endif

//if raw tx is not defined, we won't use this feature
#ifndef RFM12_RAW_TX
	#define RFM12_RAW_TX 0
#endif

//if receive cw is not defined, we won't use this feature
#ifndef RFM12_RECEIVE_CW
	#define RFM12_RECEIVE_CW 0
#endif
 
//default value for powermanagement register
//this allows one to select whether the reset generator, the oscillator or the wakeup timer shall be active
#ifndef PWRMGT_DEFAULT
	#define PWRMGT_DEFAULT (RFM12_PWRMGT_DC)
#endif

/*
 * backward compatibility for the spi stuff
 * these values weren't set in older revisions of this library
 * so they're now assumed to be on the same pin/port.
 * otherwise these defines serve to configure the software SPI ports
 */
#ifndef DDR_MOSI
	#define DDR_MOSI DDR_SPI
	#define PORT_MOSI PORT_SPI
#endif

#ifndef DDR_MISO
	#define DDR_MISO DDR_SPI
	#define PIN_MISO PIN_SPI
#endif

#ifndef DDR_SCK
	#define DDR_SCK DDR_SPI
	#define PORT_SCK PORT_SPI
#endif

#ifndef DDR_SPI_SS
	#define DDR_SPI_SS DDR_SPI
	#define PORT_SPI_SS PORT_SPI
#endif


/******************************************************
* 	HELPER MACROS			*
 ******************************************************/

//macros to turn the int on and off
#define RFM12_INT_ON() RFM12_INT_MSK |= (1<<RFM12_INT_BIT)
#define RFM12_INT_OFF() RFM12_INT_MSK &= ~(1<<RFM12_INT_BIT)

/*
 * the following macros help to manage the rfm12 fifo
 * default fiforeset is as follows:
 * 2 Byte Sync Pattern, disable sensitive reset, fifo filled interrupt at 8 bits
 */
//default fiforeset register value to accept data
#define ACCEPT_DATA (RFM12_CMD_FIFORESET | RFM12_FIFORESET_DR | (8<<4) | RFM12_FIFORESET_FF)
#define ACCEPT_DATA_INLINE (RFM12_FIFORESET_DR | (8<<4) | RFM12_FIFORESET_FF)
//default fiforeset register value to clear fifo
#define CLEAR_FIFO (RFM12_CMD_FIFORESET | RFM12_FIFORESET_DR | (8<<4))
#define CLEAR_FIFO_INLINE (RFM12_FIFORESET_DR | (8<<4))

//this macro helps to encapsulate the return values, when noreturn is set to on
#if (RFM12_NORETURNS)
	#define TXRETURN(a)
#else
	#define TXRETURN(a) (a)
#endif