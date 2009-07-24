/******************************************************
 *    THIS FILE IS BEING INCLUDED DIRECTLY		*
 *		(for performance reasons)				*
 ******************************************************/

void rfm12_init()
{
	SS_RELEASE();
	DDR_SS |= (1<<BIT_SS);
	
	spi_init();

	#if RFM12_RAW_TX > 0
	rfm12_raw_tx = 0;
	#endif

	rf_tx_buffer.sync[0] = SYNC_MSB;
	rf_tx_buffer.sync[1] = SYNC_LSB;
	
	rf_rx_buffer.rf_buffer_out = &rf_rx_buffer.rf_buffers[0];
	rf_rx_buffer.rf_buffer_in  = &rf_rx_buffer.rf_buffers[0];
	
	//disable all power
	//this was no good idea, because when one writes the PWRMGT register
	//two times in a short time, the second write seems to be delayed.
	//the PWRMGT register is written at the end of this function.
	rfm12_data(RFM12_CMD_PWRMGT | PWRMGT_DEFAULT);

	//enable internal data register and fifo
	rfm12_data(RFM12_CMD_CFG | RFM12_CFG_EL | RFM12_CFG_EF | RFM12_BAND_433 | RFM12_XTAL_12PF);
	
	//automatic clock lock control(AL), digital Filter(!S), Data quality
	//detector value 3
	rfm12_data(RFM12_CMD_DATAFILTER | RFM12_DATAFILTER_AL | 3);
	
	//2 Byte Sync Pattern, Start fifo fill when sychron pattern received,
	//disable sensitive reset, Fifo filled interrupt at 8 bits
	rfm12_data(RFM12_CMD_FIFORESET | RFM12_FIFORESET_DR | (8<<4));

	//set AFC to automatic, (+4 or -3)*2.5kHz Limit, fine mode, active and enabled
	rfm12_data(RFM12_CMD_AFC | RFM12_AFC_AUTO_KEEP | RFM12_AFC_LIMIT_4
				| RFM12_AFC_FI | RFM12_AFC_OE | RFM12_AFC_EN);
	
	//set frequency
	rfm12_data(RFM12_CMD_FREQUENCY | RFM12_FREQUENCY_CALC_433(FREQ) );

	//set datarate
	rfm12_data(RFM12_CMD_DATARATE | DATARATE_VALUE );

	//set rx parameters: int-in/vdi-out pin is vdi-out,
	//Bandwith, LNA, RSSI
	rfm12_data(RFM12_CMD_RXCTRL | RFM12_RXCTRL_P16_VDI 
			| RFM12_RXCTRL_VDI_FAST | RFM12_RXCTRL_BW_400 | RFM12_RXCTRL_LNA_6 
			| RFM12_RXCTRL_RSSI_79 );
	
	//set TX Power to -0dB, frequency shift = +-125kHz
	rfm12_data(RFM12_CMD_TXCONF | RFM12_TXCONF_POWER_0 | RFM12_TXCONF_FS_CALC(125000) );
	
	//enable receiver
	rfm12_data(RFM12_CMD_PWRMGT | PWRMGT_DEFAULT | RFM12_PWRMGT_ER);
		
	//init receiver fifo, we now begin receiving.
	rfm12_data(CLEAR_FIFO);
	rfm12_data(ACCEPT_DATA);

	//setup interrupt for falling edge trigger
	RFM12_INT_SETUP();
	
	//activate the interrupt
	RFM12_INT_ON();

#if RFM12_RECEIVE_CW > 0
	adc_init();
#endif
}


#if RFM12_LIVECTRL
inline void rfm12_set_rate (uint16_t in_datarate)
{
	rfm12_data(RFM12_CMD_DATARATE | DATARATE_VALUE );
}

inline void rfm12_set_frequency (uint16_t in_freq)
{
	rfm12_data(RFM12_CMD_FREQUENCY | in_freq );
}
#endif