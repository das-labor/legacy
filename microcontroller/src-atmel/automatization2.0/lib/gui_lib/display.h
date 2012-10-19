
//This header defines the interface from the gui library to the display hardware.
//The user must deliver an implementation for these functions.


///draw a pixel on the display in given color. The color variable is an index in the color pallet
//0 is background color, 1 is foreground color, other colors are user defined.
void display_set_pixel(uint16_t x, uint16_t y, uint8_t color);

//function to clear the screen
void display_clear_screen();
