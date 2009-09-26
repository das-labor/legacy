/* -*- Mode: C; tab-width: 2 -*- */

inline void bright_fkt(volatile uint8_t *port, struct t_busdata *data)	{
	switch( (*data).in_data[1])
		{
		case A_BR_SET:
			{
				(*port) = (*data).in_data[2];
				(*data).write_data = HASNDATA;
			}
			break;
		case A_BR_GET:
			{
				(*data).out_data = (*port);
				(*data).write_data = HASDATA;
			}
			break;
		default:
			break;
		}
}

