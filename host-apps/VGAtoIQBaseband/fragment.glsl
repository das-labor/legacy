#define KERNEL_SIZE 7

// Gaussian kernel
// 1 2 1
// 2 4 2
// 1 2 1	
float kernel[KERNEL_SIZE];

uniform sampler2D colorMap;
uniform float width;
uniform float height;

float step_w = 1.0/width;
float step_h = 1.0/height;

void main(void)
{
   int i = 0;
   float lastline = 0.0;
   float lastpixel = 0.0;
   vec4 sum = vec4(0.0);
   
   kernel[0] = 0.0470400026866;
   kernel[1] = 0.454648713413;
   kernel[2] =0.841470984808;
   kernel[3] =1.0;
   kernel[4] =0.841470984808;
   kernel[5] =0.454648713413;
   kernel[6] =0.0470400026866;

	   for( i=0; i<KERNEL_SIZE; i++ )
	   {
               if(float(gl_TexCoord[0].s - float(i)*step_w) < step_w)
               {
                   lastline = 1.0;
                   if( lastpixel < 1.0 ){
                       lastpixel = float(i);
                   }
               }
			vec4 tmp = texture2D( colorMap, gl_TexCoord[0].st +  vec2(-step_w * (float(i)-lastpixel) + lastline, -step_h * lastline));
			sum += tmp * kernel[i] * 0.2712;
	   }

   gl_FragColor = sum;
}