
#define BITSPERLAMP 12

typedef struct {
  uint16_t red;
  uint16_t green;
  uint16_t blue;
} element_t;

element_t band[LAMPS];

element_set(elemnet_t *myel)
{
    for(k=0<BITSPERLAMP;k++)
      {
	FOO = (1 <<k);
	OUT = ((myel->blue & FOO) >> k);
	send();
	clock()
      }
    for(k=0<BITSPERLAMP;k++)
      {
	FOO = (1 <<k);
	OUT = ((myel->green & FOO) >> k);
	send();
	clock()
      }
    for(k=0<BITSPERLAMP;k++)
      {
	FOO = (1 <<k);
	OUT = ((myel->red & FOO) >> k);
	send();
	clock()
      }
}

band_shif(element_t *band,uint8_t size)
{
  element_t tmp_el={0,0,0};
  uint8_t i;
  tmp_el.red = band[0].red;
  tmp_el.green = band[0].green;
  tmp_el.blue = band[0].blue;
  for(i=0;i<size-1;i++){
    band[i].red = band[i+1].red;
    band[i].green = band[i+1].green;
    band[i].blue = band[i+1].blue;
  }
  band[size-1].red=tmp_el.red;
  band[size-1].green=tmp_el.green;
  band[size-1].blue=tmp_el.blue;
  
}

main(){
  uint8 i,k,j;
  for(i=0;i<LAMPS;i++){
    element_set(band[i]);
  }

}

