#define RFM_SPICLK_ON() RFM_PORT_SPICLK |= RFM_PIN_SPICLK;
#define RFM_SPICLK_OFF() RFM_PORT_SPICLK &= ~(RFM_PIN_SPICLK);

void rfm_init_rx ( void );
void rfm_init_ddr ( void );
uint8_t rfm_init_tx ( void );
void rfm_tx ( uint8_t in_byte );
uint16_t rfm_rx_nb ( void );

