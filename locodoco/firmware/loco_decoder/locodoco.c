#include "locodoco.h"

void ldc_packet_handler ()
{
	uint8_t rxlen;
	uint8_t *rxbuf;

	if (rfm12_rx_status() != STATUS_COMPLETE)
		return;
	
	rxbuf = rfm12_rx_buffer();
	if (!LDC_MATCH(rxbuf))
	{
		rfm12_rx_clear();
		return;
	}

	switch (LDC_TYPE_GET(rxbuf))
	{
		case LDC_CMD_STOP:
		break;

		case LDC_TIMESLOT_END:
			if (!ldc_state.txlen)
				break;
		break;

		case LDC_TIMESLOT_ACK:
			ldc_timeslot_set (rxbuf);
		break;

		case LDC_CMD_RESUME:
		break;

		case LDC_DISASSOC:
			ldc_disassoc (rxbuf);
		break;

		case LDC_CMD_CONFIG_SET:
		break;

		case LDC_CMD_CONFIG_GET:
		break;

		case LDC_CMD_REBOOT:
			ldc_disassoc (rxbuf);
		break;

		case LDC_CMD_CALIBRATE:
		break;

		/* loco control */
		case LDC_CMD_LIGHT_SET:
		break;

		case LDC_CMD_LIGHT_GET:
		break;

		case LDC_CMD_SERVO_SET:
		break;

		case LDC_CMD_SERVO_GET:
		break;


		case LDC_CMD_COUPLER_SET:
		break;

		case LDC_CMD_COUPLER_GET:  
		break;


		case LDC_CMD_PANTO_SET:
		break;

		case LDC_CMD_PANTO_GET:
		break;


		case LDC_CMD_SMOKE_SET:
		break;

		case LDC_CMD_SMOKE_GET:
		break;


		case LDC_CMD_SPEED_SET:
		break;

		case LDC_CMD_SPEED_GET:
		break;

		default:
		break;
	}

	rfm12_rx_clear();
}
