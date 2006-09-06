#define BLOCKSIZE            512

#define BIT_LED 2
#define PORT_LED PORTE
#define LED_OFF PORT_LED|=(1<<BIT_LED);
#define LED_ON PORT_LED&=~(1<<BIT_LED);




//Debug over RS232

//#define DEBUG
#define USART_CHANNELS 1

//Device has ATA connected
#define ATA

//ATA Address lines 0 and 1 run over latch
#define ATA_ADDRESSLATCH

#define BIT_ALE 1
#define PORT_ALE PORTE
