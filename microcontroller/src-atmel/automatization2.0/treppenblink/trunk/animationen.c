#include <stdint.h>


#include "config.h"
#include "led_driver.h"
#include "animationen.h"

uint8_t animation = 2;


void do_ani()
{
	switch (animation)
	{
		case 0: // schwarz - aus
			{
				dot_t mydot = {0, 0, 0};
				for (uint8_t i = 0; i < LAMPS; i++) {
					pushDot(&mydot);
				}
				updateDots();
			}
			break;
		case 1: // weiss
			{
				dot_t mydot = {0xff, 0xff, 0xff};
				for (uint8_t i = 0; i < LAMPS; i++) {
					pushDot(&mydot);
				}
				updateDots();
			}
			break;
		case 2: // laufende r,g und b balken
			{
				//static const uint16_t lightblob[] = {0x003f, 0x009f, 0x00ff, 0x01ff, 0x03ff, 0x06ff, 0x0fff, 0x0fff, 0x06ff, 0x3ff, 0x01ff, 0x00ff, 0x009f, 0x003f};
				static const uint8_t lightblob[] = {0x20, 0x70, 0xa0, 0xb0, 0xd0, 0xf0, 0xff, 0xff, 0xf0, 0xd0, 0xb0, 0xa0, 0x70, 0x20};
				static uint8_t start[] = {0, LAMPS, 25};
				static uint16_t count_r = 50 * 256;
				static uint16_t count_g = 50 * 256;
				static uint16_t count_b = 50 * 256;
				#define lb_len 14

				for (uint8_t i = 0; i < LAMPS; i++)
				{
					dot_t mydot = {0, 0, 0};
					for (uint8_t j = 0; j < NUM_COLORS; j++)
					{
						if ((uint8_t) ((LAMPS + i - start[j]) % LAMPS) < lb_len)
							//*(((uint16_t *) &mydot) + j) = lightblob[(LAMPS + i - start[j]) % LAMPS];
							*(((uint8_t *) &mydot) + j) = lightblob[(LAMPS + i - start[j]) % LAMPS];
					}
					pushDot(&mydot);
				}

				count_r += 200;
				count_b += 150;
				count_g -= 175;

				count_r = (count_r + (LAMPS * 256)) % (LAMPS * 256);
				count_b = (count_b + (LAMPS * 256)) % (LAMPS * 256);
				count_g = (count_g + (LAMPS * 256)) % (LAMPS * 256);

				start[0] = count_r >> 8;
				start[1] = count_b >> 8;
				start[2] = count_g >> 8;

			updateDots();
			}
			break;
		case 3: // pause
			break;
		// zufällige farben an zufälligen stellen einfügen
		case 9:
			{
			}
			break;
		// angeregter pixel mit nachschwingen in eigenfrequenz
		case 10:
			{
			}
			break;
	}
}

