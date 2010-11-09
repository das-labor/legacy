#include "view.h"

void view_init (vp_t* in_vp)
{
	in_vp->pos[0] = 0.0f;
	in_vp->pos[1] = 0.0f;
	in_vp->pos[2] = 0.0f;

	in_vp->rot[0] = 0.0f;
	in_vp->rot[1] = 0.0f;
	in_vp->rot[2] = 0.0f;
	in_vp->rot[3] = 0.0f;
}

void view_set (vp_t* in_vp)
{
	glLoadIdentity();
	glTranslatef (in_vp->pos[0], in_vp->pos[1], in_vp->pos[2]);
	glRotatef (in_vp->rot[0], in_vp->rot[1], in_vp->rot[2], in_vp->rot[3]);
}

void view_move (vp_t* in_vp, float in_x, float in_y, float in_z)
{
	in_vp->pos[0] += in_x;
	in_vp->pos[1] += in_y;
	in_vp->pos[2] += in_z;
}
