#ifdef __cplusplus    
extern "C" { 
#endif

int add_custom_mode (char *name, int pixelclk, int hsync, int vsync);
int enable_output (char *output_name, char* mode_name, int x, int y);
int find_VGA_output(char *name,int *x, int *y, char* mode_active);
int rm_mode (char* output_name, char* mode_name);
void disable_output(char* name);
int init_xrandr(void);

#ifdef __cplusplus
}
#endif
