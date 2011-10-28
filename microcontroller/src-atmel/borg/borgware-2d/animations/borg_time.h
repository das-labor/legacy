/*
 * Description:	Request time strings from a can-master
 * 				and show them in an animation
 * Author: 		hansi
 */

#ifndef BORG_TIME_H_
#define BORG_TIME_H_

//send a time request packet via can
void time_request(void);

//update time via can, possibly blocking
uint8_t time_update(void);

//display the time
void time_anim(void);

#endif /* BORG_TIME_H_ */
