#ifdef __cplusplus    
extern "C" { 
#endif

int new_custom_mode (int pixelclk, int hsync, int vsync);
int apply_mode (void);
int find_VGA_output(char *name);
int get_VGA_position(int *x, int *y);
int init_xrandr(void);

#ifdef __cplusplus
}
#endif