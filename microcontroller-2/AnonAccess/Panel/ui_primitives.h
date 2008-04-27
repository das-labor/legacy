#ifndef UI_PRIMITIVES_H_
#define UI_PRIMITIVES_H_


void ui_primitives_init(void);
void print_progressbar(double percent, uint8_t xpos, uint8_t ypos, uint8_t width);
uint8_t radioselect(char* opts);

#endif /*UI_PRIMITIVES_H_*/
