#ifndef _MENU_BROWSER_H
#define _MENU_BROWSER_H

#define V_CONTAINER   0x10
#define H_CONTAINER   0x11
#define END_CONTAINER 0x1f
#define GUI_BUTTON    0x20
#define CAN_BUTTON    0x21
#define CAN_V_SLIDER  0x22
#define CAN_H_SLIDER  0x23
#define V_SLIDER      0x30
#define H_SLIDER      0x31
#define CAN_INFO      0x40
#define GRAPH_VIEW    0x41

void menu_browser_set_dir(uint8_t *dir_data);
void menu_browser_init();
void menu_browser_draw();
void menu_browser_touch_handler(touch_event_t t);

#endif // _MENU_BROWSER

