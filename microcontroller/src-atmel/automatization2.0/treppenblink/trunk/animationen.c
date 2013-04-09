#include <stdint.h>
#include <util/delay.h>

#include "config.h"
#include "led_driver.h"
#include "animationen.h"

uint8_t animation = 8;

void do_ani()
{
	switch (animation)
	{
		case 8: {
			static const uint16_t lightblob[] = {0x03f, 0x009f, 0x00ff, 0x01ff, 0x03ff, 0x06ff, 0x0fff, 0x0fff, 0x06ff, 0x3ff, 0x01ff, 0x00ff, 0x009f, 0x003f};
			//static const uint8_t lightblob[] = {0x40, 0x50, 0x70, 0xa0, 0xd0, 0xf0, 0xff, 0xff, 0xf0, 0xd0, 0xa0, 0x70, 0x50, 0x40};
			static uint8_t start_r = 0;
			static uint8_t start_b = LAMPS;
			static uint8_t start_g = 25;
			static uint16_t count_r = 50 * 256;
			static uint16_t count_g = 50 * 256;
			static uint16_t count_b = 50 * 256;
			#define lb_len 14

			dot_t mydot = {0, 0, 0};
			for (uint8_t i = 0; i < LAMPS; i++)
			{
				for (uint8_t j = 0; j < NUM_COLORS; j++)
				{
					if ((uint8_t) ((LAMPS + i - start_r) % LAMPS) < lb_len) //XXX start_r
						*(((uint16_t *) &mydot) + i) = lightblob[(LAMPS + i - start_r) % LAMPS]; // XXX same
				}
				pushDot(&mydot);
			}

			count_r += 200;
			count_b += 150;
			count_g -= 175;

			count_r = (count_r + (LAMPS * 256)) % (LAMPS * 256);
			count_b = (count_b + (LAMPS * 256)) % (LAMPS * 256);
			count_g = (count_g + (LAMPS * 256)) % (LAMPS * 256);

			start_r = count_r >> 8;
			start_b = count_b >> 8;
			start_g = count_g >> 8;

			//taskDelayFromNow(1);
			_delay_ms(1);
			updateDots();
			//taskDelayFromNow(70);
			_delay_ms(70);
		}
		break;
		// zufällige farben an zufälligen stellen einfügen
		case 9: {
		}
		break;
		// angereter pixel mit nachschwingen in eigenfrequenz
		case 10: {
		}
		break;
	}
}

