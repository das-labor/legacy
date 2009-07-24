/******************************************************
 *    THIS FILE IS BEING INCLUDED DIRECTLY		*
 *		(for performance reasons)				*
 ******************************************************/


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