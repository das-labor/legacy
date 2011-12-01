#ifndef _MENU_BROWSER_H
#define _MENU_BROWSER_H

#define V_CONTAINER   0x10
#define H_CONTAINER   0x11
#define END_CONTAINER 0x1f
#define GUI_BUTTON    0x20
#define CAN_BUTTON    0x21
#define V_SLIDER      0x30


void menu_browser_set_dir (uint8_t * dir_data);
void menu_browser_init ();
void menu_browser_draw ();
void menu_browser_touch_handler(touch_event_t t);

#endif // _MENU_BROWSER

