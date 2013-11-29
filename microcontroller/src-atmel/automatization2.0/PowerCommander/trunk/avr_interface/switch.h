#ifndef _POWERCOMMANDER_SWITCH_H
#define _POWERCOMMANDER_SWITCH_H


extern void switch_handler(void);
extern void switch_timer_init(void);

extern void tog_dimdir_vortrag(void);
extern void tog_dimdir_kueche(void);
extern void dim_vortrag(void);
extern void dim_kueche(void);
extern void toggle_vortrag(void);

#endif // _POWERCOMMANDER_SWITCH_H

