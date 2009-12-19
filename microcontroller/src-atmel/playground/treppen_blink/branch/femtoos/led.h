#ifndef AVRX_LEDTASK_H
#define AVRX_LEDTASK_H



// #define NEED_PINGPONG
// #define NEED_PINGPONG_BACK
// #define NEED_EPI


#define COLOR_SET (0x00) 
#define COLOR_ADD ((0x01) << BITSPERLAMP)
#define COLOR_SUB ((0x02) << BITSPERLAMP)

#define Melement_set(x)						\
  uint8_t k;							\
  for (k = 0 ; k< BITSPERLAMP; k++)					\
    {									\
      OUTPORT = ( ( ((x)->blue >> (BITSPERLAMP-k-1)) & 1 ) << DATA);	\
      OUTPORT |= _BV(CLK);						\
      OUTPORT &= ~_BV(CLK);						\
    }									\
  for (k = 0; k < BITSPERLAMP; k++)					\
    {									\
      OUTPORT = ( ( ((x)->green >> (BITSPERLAMP-k-1)) & 1 ) << DATA);	\
      OUTPORT |= _BV(CLK);						\
      OUTPORT &= ~_BV(CLK);						\
    }									\
  for (k = 0; k < BITSPERLAMP; k++)					\
    {									\
      OUTPORT = ( ( ((x)->red >> (BITSPERLAMP-k-1)) & 1 ) << DATA);	\
      OUTPORT |= _BV(CLK);						\
      OUTPORT &= ~_BV(CLK);						\
    }


#define Mband_shift(x,y,z)				\
  (z).red = (x)[(y)-1].red;				\
    (z).green = (x)[(y)-1].green;			\
	(z).blue = (x)[(y)-1].blue;			\
	   for (i = ((y)-1); i > 0; i--)		\
	     {						\
	       (x)[i].red = (x)[i-1].red;		\
	       (x)[i].green = (x)[i-1].green;		\
	       (x)[i].blue = (x)[i-1].blue;		\
	     }						\
	   (x)[0].red=(z).red;				\
			 (x)[0].green=(z).green;		\
					 (x)[0].blue=(z).blue;	
							

typedef struct {
	uint16_t red;
	uint16_t green;
	uint16_t blue;
} element_t;

void update();
//void element_set(element_t *myel);
void band_shift(element_t *band, uint8_t size);
void band_shiftback(element_t *band, uint8_t size);
void band_redraw(element_t *band,uint8_t size);
#ifdef NEED_PINGPONG
void band_pingpong(element_t *band,uint8_t size,uint8_t ballsize,uint16_t speed);
#endif
#ifdef NEED_PINGPONG_BACK
void band_pingpong_back(element_t *band,uint8_t size,uint8_t ballsize,uint16_t speed);
#endif
void band_insert(element_t *final, uint8_t pos, element_t *toinsert, uint8_t size);
#ifdef NEED_EPI
void band_epi(element_t *band, uint8_t size);
#endif

AVRX_EXTERNTASK(led);

#endif
