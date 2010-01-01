//by hansi

//send a time request packet via can
void time_request(void);

//update time via can, possibly blocking
uint8_t time_update(void);

//display the time
void time_anim(void);