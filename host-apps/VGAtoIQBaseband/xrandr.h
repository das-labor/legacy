#ifdef __cplusplus    
extern "C" { 
int new_custom_mode (int pixelclk, int hsync, int vsync);
int apply_mode (void);
int size_of_primary(void);
int find_VGA_output(char *name);
}
#else
int new_custom_mode (int pixelclk, int hsync, int vsync);
int apply_mode (void);
int size_of_primary(void);
int find_VGA_output(char *name);
#endif