#ifdef __cplusplus    
extern "C" { 
#endif

typedef struct 
{
	char 		activemode[255];
	char 		outputname[255];
	int 		pos_x;
	int 		pos_y;
	int 		mode;
} vga_t;

int add_custom_mode ( vga_t *vga_ptr , int pixelclk, int height, int hsync, int vsync);
int enable_output ( vga_t *vga_ptr );
int find_VGA_output( vga_t *vga_ptr );
int rm_mode ( vga_t *vga_ptr );
void disable_output( vga_t *vga_ptr );
int init_xrandr(void);

#ifdef __cplusplus
}
#endif
