#define BLOCKSIZE            512

#define LED_BIT 2
#define LED_PORT PORTE
#define LED_OFF LED_PORT|=(1<<LED_BIT);
#define LED_ON LED_PORT&=~(1<<LED_BIT);




//Debug over RS232

#define DEBUG
#define USART_CHANNELS 1

//Device has ATA connected
#define ATA

//ATA Address lines 0 and 1 run over latch
#define ATA_ADDRESSLATCH

#define BIT_ALE 1
#define PORT_ALE PORTE
