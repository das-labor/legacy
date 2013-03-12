
#include "led_driver.h"


void do_ani()
{
		case 8: {
			static const uint16_t lightblob[] = {0x03f, 0x009f, 0x00ff, 0x01ff, 0x03ff, 0x06ff, 0x0fff, 0x0fff, 0x06ff, 0x3ff, 0x01ff, 0x00ff, 0x009f, 0x003f};
			uint8_t start_r = 0;
			uint8_t start_b = LAMPS;
			uint8_t start_g = 25;
			uint16_t count_r = 50 * 256;
			uint16_t count_g = 50 * 256;
			uint16_t count_b = 50 * 256;
			#define lb_len 14
			
			dot_t mydot;
			for (uint8_t i = 0; i < LAMPS; i++)
			{
				for (uint8_t i = 0; i < NUM_COLORS; i++)
				{
					*(((uint16_t *) dot) + i = 0x00;

					if ((uint8_t) ((LAMPS + i - start_r) % LAMPS) < lb_len)
						*(((uint16_t *) dot) + i = lightblob[(LAMPS + i - start_r) % LAMPS];
				}
				pushValue(myelement);
			}

			count_r += 200;
			count_b += 150;
			count_g -= 175;

			count_r = (count_r + (LAMPS*256))%(LAMPS*256);
			count_b = (count_b + (LAMPS*256))%(LAMPS*256);
			count_g = (count_g + (LAMPS*256))%(LAMPS*256);

			start_r = count_r >> 8;
			start_b = count_b >> 8;
			start_g = count_g >> 8;

			taskDelayFromNow(1);
			updateLEDs();
			taskDelayFromNow(70);
		}
		// zufällige farben an zufälligen stellen einfügen
		case 9: {
		}
		// angereter pixel mit nachschwingen in eigenfrequenz
		case 10: {
		}
}
