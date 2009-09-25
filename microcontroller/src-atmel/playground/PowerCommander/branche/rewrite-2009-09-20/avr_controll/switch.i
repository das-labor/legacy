/* -*- Mode: C; tab-width: 2 -*- */

/*
	std-switch oder optokoppler
*/
inline void switch_fkt(struct t_switch_parameter *sw_para, struct t_busdata *data)	{
	switch( (*data).in_data[1]) 
		{
		case A_SW_OFF:
			{
				(*sw_para).write_port &= ~_BV((*sw_para).pin);
				(*data).write_data = HASNDATA;
			}
			break;
		case A_SW_ON:
			{
				(*sw_para).write_port |= _BV((*sw_para).pin);
				(*data).write_data = HASNDATA;
			}
			break;
		case A_SW_STATUS:
			{
				(*data).out_data=((*sw_para).read_port >> (*sw_para).pin) & 1;
				(*data).write_data = HASDATA;
			}
			break;
		default:
			break;
		}

}

		
/*
	readonly switch
*/

inline void switch_ro_fkt(struct t_switch_parameter_ro *sw_para, struct t_busdata *data)	{
	switch( (*data).in_data[1]) 
		{
		case A_SW_STATUS:
			{
				(*data).out_data=((*sw_para).read_port >> (*sw_para).pin) & 1;
				(*data).write_data = HASDATA;
			}
			break;
		default:
			break;
		}
}
